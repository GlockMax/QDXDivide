#include "stdafx.h"
#include "NCMProject.h"
#include "NCMDoc.h"
#include "FAnimateDisp.h"
#include "NCadrSpeedPar.h"
#include "FRange.h"
#include "FProg2BSP.h"
#include "NCollisionLog.h"
#include "FCutting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

FCutting::FCutting(NChannel* pChan)
{
	pChannel = pChan;
	Started = false;
	Stopped = true;
	NLastProg = 0;
	LastHCadr = 0;
}

FCutting::~FCutting(void)
{
}

INT_PTR FCutting::AddRange(FRange *pRange)
{
	// Divide range 
	for(FRange *pNewRange = pRange->ExtractPart()
		; pNewRange != nullptr
		; pNewRange = pRange->ExtractPart())
	{
		RangesArray.Add(pNewRange);
	}
	// End:Divide range 
	return RangesArray.Add(pRange);
}

UINT FCutting::CuttingThreadProc( LPVOID pParam, CWnd* pMainWnd)
{
	pChannel->SetCuttingResult(CUT_EMPTY);
	FProg2BSP& LProg2BSP = pChannel->GetProg2BSP();
	Stopped = false;
	Started = true;// Cutting thread has been started
	// This flag is to be set to false by MainFrame::OnTimer 
	// or MainFrame::OnCuttingStopped
	LProg2BSP.ResetInterrupt();
	FRange::intPair HCadrPair;
	if (!RangesArray.IsEmpty())
	{
		pChannel->FillMachCopy();
		MUPair.Set(pChannel->GetMachCopy(), *pChannel->GetUnitCn());
		LProg2BSP.SetMT(&MUPair);
		RangesArray[0]->ResetCuttingTypeChanged();
	}
	bool IsInterrupted = false;
	while(RangesArray.GetSize() > 0 && !IsInterrupted)
	{
		pLastProg = RangesArray[0]->GetProg();
		NLastProg = RangesArray[0]->GetProgNum();
		try
		{
			HCadrPair = RangesArray[0]->AnimateRange(pMainWnd);
			LastHCadr = HCadrPair.Second;
		}
		catch(const CMemoryException *pMEx)
		{
			pMEx;
			break;
		}
		// This fragment needed for AT_MILL_TURN only!
		if (NCMComData::GetType() == AT_MILL_TURN)
		{
			if (RangesArray.GetSize() > 1)
				RangesArray[1]->SetCuttingTypeChanged(*RangesArray[0]);
			else
				RangesArray[0]->ProcCuttingTypeChanged();
		}
		// END:This fragment needed for AT_MILL_TURN only
		IsInterrupted = RangesArray[0]->GetDoc()->IsInterrupted();
		delete RangesArray[0];
		RangesArray.RemoveAt(0);
	}
	// 
	if(IsInterrupted)
	{
		for(int i = 0; i < RangesArray.GetSize(); ++i)
			delete RangesArray[i];
		RangesArray.RemoveAll();
		LastHCadr += (LastHCadr >= 0) ? 1 : -1;// Added 3.09.10 
		HCadrPair.increase();
	}

	Stopped = true;
	int Res = 0;
	int Result = CUT_EMPTY;
	if(HCadrPair.Second < 0)
	{
		HCadrPair.invert();
		LastHCadr = HCadrPair.Second;
		Res = -1;// thread has errors
		Result |= CUT_INTERRUPTED;
	}
	Result |= CUT_OK;
	if (pParam)
		Result |= CUT_PROJ_END;
	pChannel->SetCuttingResult(Result);
	if(pMainWnd != nullptr)
		pMainWnd->PostMessage(WM_APP_CUTTING_STOPPED, pChannel->GetChannelInd());

	return Res;   // thread completed successfully
}
int FCutting::StoreProgFragment(const NSpeedPar* pSpeedPar, int ProgNum, const BProgram * pProg, int StartHCadr, int EndHCadr)
{
	if (EndHCadr < StartHCadr)
		return -1;
	FRange *pRange = new FRange(pChannel, pSpeedPar, ProgNum, pProg, StartHCadr, EndHCadr);
	pRange->SetAnimMode( NCM_PROJECT.GetGlobalState().GetAnimMode() );
	AddRange(pRange);
	return 0;
}

bool FCutting::IsStopped(void) const
{
	return Stopped;
}
bool FCutting::IsStoppedFull(void) const
{
	return Stopped && !(pChannel->IsNeedProbe() || pChannel->IsNeedTouch());
}
// Этапы разрезания программ
// Вход: RangesArray (Range не больше, чем одна программа и не содержит 
// внутри себя кадров с изменением типа обработки)
// 1. FCutting::AddRange по инструментам из ProgGeom в GeomArr(в главном потоке)
// 2. FRange::AnimateRange по ABC из ProgGeom в GeomArr
// 3. NCadrsProcessing::SendCadrsArray по вертикальным кадрам из InArr в Cadrs
// 4. NCadrsProcessing::FindMaxChain по цепочкам на месте
// Перед п2 возможно разбиение по одному кадру в FAnimateDisp::AnimateFragment
void FCutting::CutSynchr(void)
{
		CuttingThreadProc( NULL, AfxGetMainWnd());
}

void FCutting::Init()
{
	RangesArray.RemoveAll();
	LastHCadr = 0;
	pLastProg = NULL;
	NLastProg = 0;
	Stopped = true;
	MUPair.Clear();
}

bool FCutting::HasRanges(void) const
{
	return RangesArray.GetSize() > 0;
}

bool FCutting::HasCutting(void) const
{
	for(int i = 0; i < RangesArray.GetSize(); ++i)
	{
		FRange * pRange = RangesArray.GetAt(i);
		if(pRange->HasCutting())
			return true;
	}
	return false;
}

NCMApplicationType FCutting::GetNextState(void) const
{
	if(RangesArray.GetSize() <= 0)
		return AT_UNDEF;

	return RangesArray[0]->GetStockState();
}

const FRange * FCutting::GetFirstRange(void) const
{
	if(!HasRanges())
		return NULL;
	return RangesArray.GetAt(0);
}
const FRange * FCutting::GetLastRange(void) const
{
	if(!HasRanges())
		return NULL;
	return RangesArray.GetAt(RangesArray.GetSize() - 1);
}

void FCutting::Clear()
{
	for (int i = 0; i < RangesArray.GetSize(); ++i)
		delete RangesArray[i];
	RangesArray.RemoveAll();
}
