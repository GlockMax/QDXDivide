#include "stdafx.h"
#include <direct.h>
#include "NCMProject.h"
#include "MainFrm.h"
#include "ConstDef.h"
#include "NCUnit.h"
#include "FCutting.h"
#include "FAnimateDisp.h"
#include "GAnimateCadr.h"
#include "NCadr.h"
#include "NMachUnitPair.h"
#include "GAnimateCadrMill.h"
#include "GAnimateCadrMillDX.h"
#include "GAnimateCadrRot.h"
#include "GReRenderCadr.h"
#include "FProg2BSP.h"
#include "NCMDoc.h"
#include "NCollManagerTouch.h"
#include "NCollManagerProbe.h"
#include "NSyncGenerator.h"
#include "FRange.h"
#include "NToolCombinedMill.h"
#include "NToolStd.h"
#include "MTConfig.h"
#include "NChannel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


NChannel::NChannel() : Cutting(this)
{
	Unit = nullptr;
	ProgrToDispTextNum = -1;
	ChannelInd = -1;
	pDoc = nullptr;
	pDynamicCadr = nullptr;
	ChainEnded = true;
	CuttingResult = 0;
	flNeedProbe = false;
	flNeedTouch = false;
	flProbePresent = false;
	flTouchPresent = false;
}

NChannel::~NChannel()
{
	Unit->ClearConf();
	delete Unit;
}

NCUnit* NChannel::GetUnitCn() const
{
	return Unit;
}

CMapStringToNProgram* NChannel::NGetProgramListCn()
{
	return &NProgramList;
}

const CMapStringToNProgram* NChannel::NGetProgramListCn() const
{
	return &NProgramList;
}

void NChannel::WaitCuttingThreadCn(void)
{
	if (CuttingThread._GetImpl() == nullptr)// CuttingThread was created with default constructor
		return;
	try
	{
		CuttingThread.wait();
	}
	catch(...)
	{
	};
}

bool NChannel::StartCutCn(RunStepsStopReason StopReason)
{
	if (Cutting.Started)
		return false;
	Cutting.Started = true; // Added to ensure Started = true before its checking in main thread (OnIdle)

	CWnd* pMainWnd = AfxGetMainWnd();
	WaitCuttingThreadCn();
	CuttingThread = create_task([&, StopReason, pMainWnd] { return Cutting.CuttingThreadProc((LPVOID)(StopReason == SR_PROJ_END), pMainWnd); });
	return true;
}

void NChannel::Serialize(CArchive& ar)
{
	SerializeElements(ar, &ChannelInd, 1);
	SerializeElements(ar, &ProgrToDispText, 1);
	Unit->Serialize(ar);
	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("3.91")))
		SerializeElements(ar, &ProgrToDispTextNum, 1);
	NProgramList.Serialize(ar);
#ifdef NC_DEMO
	// Special DEMO comments
	CString Key;
	NProgram* pProg;
	if (NProgramList.GetAt(0, Key, (CObject*&)pProg))
	{
		if (pProg->GetComment().GetLength() != Key.GetLength())
		{
			AfxMessageBox(IDS_MES_DEMO);
			DeleteContents();
			GetDoc()->SetBlockUpdateLevel(0);
			return;
		}
	}
#endif
}

void NChannel::DeleteContents()
{
	NProgramList.FullRemoveAll();
	Unit->ClearCompArrays();
	Unit->Reset(false);
	ProgrToDispTextNum = -1;
}

bool NChannel::DeleteProgCn(const CString& prog)
{
	if (Unit->GetState()->GetCurProg() == prog)
		Unit->Reset();

	CString CurProg(Unit->GetState()->GetCurProg());
	int CurHCadr = Unit->GetState()->GetCurHCadr();
	NProgram* pProg;
	if (!NGetProgramListCn()->Lookup(prog, pProg))
		return false;;

	CNCMDoc::DeleteProgObj(pProg);
	NGetProgramListCn()->RemoveKey(prog);
	if (prog == GetProgrTDTCn())
	{
		ProgrToDispTextNum = -1;
		ProgrToDispText.Empty();
	}
	return true;
}

void NChannel::DeleteAllProgCn()
{
	Unit->Reset();
	ProgrToDispTextNum = -1;
	ProgrToDispText.Empty();

	//Clear ProgramList
	CString key;
	NProgram* pProg;
	for (auto i = NGetProgramListCn()->GetMainProgCount() - 1; i >= 0; --i)
	{
		if (NGetProgramListCn()->GetAt(i, key, (CObject*&)pProg))
			NGetProgramListCn()->RemoveAt(i);
	}

}

void NChannel::DeleteAllSubProgCn()
{
	Unit->Reset();
	ProgrToDispTextNum = -1;
	ProgrToDispText.Empty();

	//Clear ProgramList
	CString key;
	NProgram* pProg;
	for (auto i = NGetProgramListCn()->size() - 1; i >= NGetProgramListCn()->GetMainProgCount(); --i)
	{
		if (NGetProgramListCn()->GetAt(i, key, (CObject*&)pProg))
			NGetProgramListCn()->RemoveAt(i);
	}
}

void NChannel::MakeNewChannel(CNCMDoc* ipDoc, int InChannelInd)
{
	pDoc = ipDoc;
	ChannelInd = InChannelInd;
	delete Unit;
	Unit = new NCUnit(pDoc, InChannelInd);//NCUnit constructor
	Unit->Reset(true);
}

int NChannel::RegenerateProgramsCn(const std::set<int>& PositionsSet)
{
	flProbePresent = false;
	flTouchPresent = false;
	AxesBases.clear();

	int WholeProgSize = 0;
	CMapStringToNProgram* pPList = NGetProgramListCn();
	for (int i = 0; i < pPList->GetSize(); ++i)
	{
		CString Key;
		NProgram* pProg = NULL;
		pPList->GetAt(i, Key, (CObject*&)pProg);
		if (pProg)
		{
			pProg->ClearEvaluated(PositionsSet);
			WholeProgSize += pProg->GetSize();
		}
	}
	GetUnitCn()->ClearSyncHistory();
	GetUnitCn()->GetStates().Init(WholeProgSize);
	// make program groups
	if (GetUnitCn()->GetSinumerikStyle())
	{
		GetUnitCn()->GetProgGroupList().Restart(pPList->GetSize());
		for (int i = 0; i < pPList->GetSize(); ++i)
		{
			CString Key;
			NProgram* pProg = nullptr;
			pPList->GetAt(i, Key, (CObject*&)pProg);
			if (pProg == nullptr)
				continue;
			const auto res = GetUnitCn()->MakeProgGroupList(pProg, i, pPList->GetProgName(i));
		}
	}

	auto pUnit = std::make_unique<NCUnit>(GetUnitCn()->GetDoc(), *GetUnitCn());//NCUnit constructor
	NCUnit& Unit = *pUnit;
	int res = 0;
	bool FirstRegen = true;
	for each (auto StockPos in PositionsSet)
	{
		Unit.Reset(true);
		Unit.AddStartMatr(FirstRegen, StockPos);
		FirstRegen = false;
		Unit.SetCurStockPosition(StockPos);
		for (int i = 0; i < pPList->GetMainProgCount(); ++i)
		{
			CString Key;
			NProgram* pProg = NULL;
			pPList->GetAt(i, Key, (CObject*&)pProg);
			if (!pProg)
				continue;
			pProg->ClearEvaluated();
			Unit.SetCurProg(Key);
			Unit.GetState()->SetCurCadrEx(cadrID(i, 0));
			if (GetUnitCn()->GetCheckProgramText())
				Unit.CheckProgramText(pProg);
			Unit.GetState()->SetCurCadr(0);
			Unit.GetState()->ProgramStarted = false;

			Unit.GetState()->EndProgram = false;
			res = Unit.RunProgramProbePart(pProg, pProg, WholeProgSize);
			Unit.AddGroupErrors2Prog(pProg, i);
			if (res == 1)
				break;// error
			if (Unit.GetState()->ProbeCadr)
				break;
		}
		for (int i = pPList->GetMainProgCount(); i < pPList->GetSize(); ++i)
		{
			CString Key;
			NProgram* pProg = NULL;
			pPList->GetAt(i, Key, (CObject*&)pProg);
			if (!pProg)
				continue;
			Unit.AddGroupErrors2Prog(pProg, i);
		}
	}
	GetUnitCn()->GetUsedUCSs().insert(Unit.GetState()->UCSs.GetUsed().begin(), Unit.GetState()->UCSs.GetUsed().end());
	GetUnitCn()->GetStates().AddState(*Unit.GetState(), *Unit.GetState(), *Unit.GetLastGeomState());// Very last state
	GetUnitCn()->GetStates().Close();

	if (res == 1) // error or cancel
		GetUnitCn()->JumpForToHCadr(Unit.GetState()->GetCurCadrIDEx());
	else
		GetUnitCn()->JumpForToHCadr(GetUnitCn()->GetState()->GetCurCadrIDEx());
	Unit.Reset();// To set the machine tool to a proper position
	GetUnitCn()->GetStartState().ForceMakeEqualTo(*Unit.GetState());
	return res;
}

bool NChannel::SetProgrTDTCn(const CString& str)
{
	int ProgNum = NGetProgramListCn()->GetIndex(str);
	ProgrToDispTextNum = -1;
	return SetProgrTDTCn(ProgNum);
}

bool NChannel::SetProgrTDTCn(int ProgNum)
{
	if (ProgNum == ProgrToDispTextNum)
		return true;
	if (ProgNum < 0)
	{
		ProgrToDispTextNum = ProgNum;
		ProgrToDispText.Empty();
		return true;
	}
	//if(ProgNum >= NGetProgramList()->GetMainProgCount())
	//	return false;
	cadrID CadrID;
	CadrID.Prog = ProgNum;
	if (NCUnit::IsFileProtectionEnabled())
	{
		if (NGetProgramListCn()->IsCadrProtected(CadrID))
			return false;
	}
	ProgrToDispTextNum = ProgNum;
	ProgrToDispText = NGetProgramListCn()->GetProgName(CadrID);
	return true;
}

void NChannel::SetCurProgrTDTCn()
{
	if (NGetProgramListCn()->empty())
		SetProgrTDTCn(-1);
	else
		SetProgrTDTCn(GetUnitCn()->GetState()->GetCurCadrID().Prog);
}

const CString& NChannel::GetProgrTDTCn() const
{
	return ProgrToDispText;
}

void NChannel::PauseAnimWait(void)
{
	//while (CuttingThreadHandle && !FAnimateDisp::RendersQueue.wasFull())
	//{
	//	DWORD res = WaitForSingleObject(CuttingThreadHandle, 10 * GAnimateCadr::GetTimeStep());
	//	if (res == WAIT_OBJECT_0)
	//	{
	//		//			delete CuttingThreadHandle;
	//		CloseCuttingThreadHandleCn();
	//	}
	//}
	while (!CuttingThread.is_done() && !GetAnimateDisp().RendersQueue.wasFull())
	{
		wait(10 * NSpeedPar::Extern.GetTimeStep());
	}
}

void NChannel::ImplReplCn(const BProgram* pProg) const
{
	if (pProg == nullptr)
		return;
	// remove previous replacements
	for (int i = 0; i < pProg->GetSize(); ++i)
	{
		NCadr& Cadr = *(*pProg)[i];
		Cadr.ClearReText();
	}
	// External processing
	CString TPName;
	if (Unit->GetOtherValue(_T("TextProcessor"), TPName))
	{
		if (!TPName.IsEmpty())
		{
			CString TempProgsFolder;
			TempProgsFolder.LoadString(IDS_PROCEXE_FOLDER);
			CString Path = NCM_PROJECT.GetUnitsPath() + TempProgsFolder;
			_mkdir(Path);
			CStdioFile fo(Path + _T("TempProg"),
				CFile::typeText | CFile::modeWrite | CFile::modeCreate);

			for (int i = 0; i < pProg->GetSize(); i++)
				fo.WriteString(pProg->GetCadrText(i) + "\n");
			fo.Close();
			const auto res = _spawnl(_P_WAIT, Path + TPName, Path + TPName, "TempProg", nullptr);
			if ( res == 0)
//			if (system(Path + TPName + " TempProg") >= 0)
			{
				CStdioFile fi(Path + _T("TempProg"),
					CFile::typeText | CFile::modeRead);
				bool Error = false;
				CString str;
				for (int i = 0; i < pProg->GetSize(); i++)
				{
					if (!fi.ReadString(str))
						break;
					(*pProg)[i]->SetReText(str);
					(*pProg)[i]->CheckReText();
				}
				if (!Error)
					Error = !fi.ReadString(str);
				fi.Close();
				DeleteFile(Path + _T("TempProg"));
			}
		}
	}
	// Internal processing
	for (int i = 0; i < pProg->GetSize(); ++i)
	{
		NCadr& Cadr = *(*pProg)[i];
		if(!Cadr.IsTextChanged())
			Cadr.GetReText2Ch() = Cadr.GetOrText();
		Unit->DoReplace(Cadr.GetReText2Ch());
		Cadr.CheckReText();
	}
}

double NChannel::GetRunAnimateT(void)
{
	return WireAnimWalker.GetProgWalker().CalcCurT();
}

int NChannel::GetRunAnimateCadr(void)
{
	return WireAnimWalker.GetHStart();
}

const NCadrGeom* NChannel::GetRunAnimateCadrGeom() const
{
	return WireAnimWalker.GetProgWalker().GetGeom();
}

RunStepsStopReason NChannel::RunAnimateStepCn(CNCMDoc* pDoc, int& MaxRunStepsNum)
{
	int End = -1;
	int Start = 0;
	int HStart = 0;
	int HEnd = -1;
	RunStepsStopReason res = SR_OK;
	WireAnimWalker.GetProgWalker().SetTool(pDoc->GetTool(GetUnitCn()->GetState()->CurToolN));
	while (!WireAnimWalker.NextStep())// *pAddCPrev è *AddCNext in ProgWalker should be actual
	{
		if (WireAnimWalker.GetCurProg().IsEmpty())// Very first step
			HStart = GetUnitCn()->GetState()->GetCurHCadr();
		else
			HStart = 0;
		if (res == SR_ERROR || res == SR_PROJ_END || (res == SR_BREAK && MaxRunStepsNum < 0))
		{
			WireAnimWalker.SetStopped();
			return SR_BREAK;
		}
		if (MaxRunStepsNum == 0)
		{
			WireAnimWalker.SetStopped();
			MaxRunStepsNum = -1;
			return SR_BREAK;
		}
		const bool bufFileProt = NCUnit::IsFileProtectionEnabled();// We need actual State and St after every single cadr
		//for additional coord simulation so we should disable file protection temporary
		// but in this case we will not pass protected cadrs after breakpoint so we need special processing for it
		NCUnit::SetFileProtection(false);
		res = GetUnitCn()->RunSteps(NEvalStopCrit(1));
		NCUnit::SetFileProtection(bufFileProt);
		const bool CurCadrProtected = GetUnitCn()->IsCurCadrProtected();
		// Special processing mentioned above
		if (res == SR_BREAK)
		{
			WireAnimWalker.SetBreakWasDetected(true);
			res = SR_OK;
		}
		if (!CurCadrProtected)
		{
			if (WireAnimWalker.GetBreakWasDetected())
			{
				WireAnimWalker.SetStopped();
				res = SR_BREAK;
			}
		}
		// END:Special processing mentioned above

		if (MaxRunStepsNum > 0 && !CurCadrProtected)
			--MaxRunStepsNum;

		const CString& NewProg = GetUnitCn()->GetState()->GetCurProg();
		if (NewProg.IsEmpty())
		{// Internal error
			return SR_EMPTY;
		}
		const cadrID NewID = GetUnitCn()->GetState()->GetCurCadrID();
		SetCurProgrTDTCn();
		if (NewProg != WireAnimWalker.GetCurProg())// Program changed
		{
			WireAnimWalker.SetCurProg(NewProg);
			NProgram* pProg = nullptr;
			if (!NGetProgramListCn()->Lookup(NewProg, pProg))
			{// Internal error
				return SR_EMPTY;
			}
			WireAnimWalker.SetProgram(*pProg);
			Start = pProg->GetHistByHCadr(HStart);
			HEnd = GetUnitCn()->GetState()->GetCurHCadr();
			End = pProg->GetHistByHCadr(HEnd);
		}
		else
		{
			Start = WireAnimWalker.GetEndAbsI();
			HStart = WireAnimWalker.GetHEnd();
			HEnd = GetUnitCn()->GetState()->GetCurHCadr();
			End = WireAnimWalker.GetProgram().GetHistByHCadr(HEnd);
		}
		WireAnimWalker.SetInterval(Start, End, HStart, HEnd);
	}
	return res;
}

void NChannel::StartAnimWalkerCn(bool UseSetHStart, const BBox& StockGabar, MTMachineTool& MT)
{
	WireAnimWalker.Set(NProgWalker(new NMachUnitPair(MT, *GetUnitCn()), StockGabar, 1.,
		&GetUnitCn()->GetSt()->AddCoords, &GetUnitCn()->GetState()->AddCoords, NSpeedPar::Extern.IsTrackFeed()));// memory leaks
	if (UseSetHStart)
		WireAnimWalker.SetHStart(GetUnitCn()->GetState()->GetCurHCadr());
}

bool NChannel::IsInsideCadrCn() const
{
	return WireAnimWalker.IsInsideCadr();
}

GDynamicCadr* NChannel::StartResultCadr(void)
{
	delete pDynamicCadr;
	pDynamicCadr = nullptr;
	pDynamicCadr = new GReRenderCadr(nullptr);
	if (pDynamicCadr)
		pDynamicCadr->SetMT(&pDoc->GetMachTool(), GetUnitCn(), &GetDoc()->GetCollManager().GetCollTimeLine());
	return pDynamicCadr;
}

GDynamicCadr* NChannel::GetDynamicCadr(void) const
{
	return pDynamicCadr;
}

cadrID NChannel::GetDynamicCadrCadrID(void) const
{
	return (pDynamicCadr == nullptr) ? cadrID(-1, 0) : pDynamicCadr->GetCadrID();
}

void NChannel::StopDynamicCadr(void)
{
	delete pDynamicCadr;
	pDynamicCadr = nullptr;
}

void NChannel::SetAnimateCadr(GAnimateCadr* pACadr)
{
	delete pDynamicCadr;
	pDynamicCadr = pACadr;
}

void NChannel::FillMachCopy(void)
{
	MachCopy.CopyKinematics(pDoc->GetMachTool());
}

int NChannel::CutProgFragmentsCn(RunStepsStopReason StopReason)
{
	auto pRange = Cutting.GetFirstRange();
	if (pRange != nullptr)
		pDoc->NGetStockList()->ApplyStocksState(GetCutting().GetNextState(), pRange->GetStockGr4Turn());
	bool HaveCutting = GetCutting().HasCutting();
	if (!HaveCutting)
	{
		GetCutting().Clear();
		return -1;
	}
	GetProg2BSP().Init(pDoc->GetCollManager().GetStopOnColl());// place here. Render loaded in StartRender4Run
	switch (NCM_PROJECT.GetGlobalState().GetAnimMode())
	{
	case NGlobalState::Animation:
		StartRender4Run(false);
		break;
	case NGlobalState::Result:
		StartResultCadr();
		break;
	case NGlobalState::Anim_no:
		break;
	}

	StartCutCn(StopReason);
	switch (NCM_PROJECT.GetGlobalState().GetAnimMode())
	{
	case NGlobalState::Animation:
		GetProg2BSP().NeedNewPosition = false;
		break;
	case NGlobalState::Result:
		GetProg2BSP().NeedNewPosition = true;
		break;
	}
	pDoc->SetBlockUpdateLevel(1);
	return 0;
}

BProgram* NChannel::GetProgram(int ProgNum)
{
	CString key;
	BProgram* pProg = NULL;
	NGetProgramListCn()->GetAt(ProgNum, key, (CObject*&)pProg);
	return pProg;
}

BMatr NChannel::GetProgMatr()
{
	MTMachineTool& MT = pDoc->GetMachTool();

	const NCUnit& Un = *GetUnitCn();

	auto pXAxis = MT.GetCurConfig().GetAxis(Un.GetFixedDef().GetName(0));
	auto pZAxis = MT.GetCurConfig().GetAxis(Un.GetFixedDef().GetName(2));
	BPoint Vx(0., 0., 0., 0.), Vz(0., 0., 0., 0.);
	if (pXAxis != nullptr)
	{
		Vx = pXAxis->GetVector();
		if (MT.IsAnyStockAncestor(pXAxis))
			Vx = Vx * (-1);
	}
	if (!Vx.Make1(MIND))
		return BMatr();
	if (pZAxis != nullptr)
	{
		Vz = pZAxis->GetVector();
		if (MT.IsAnyStockAncestor(pZAxis))
			Vz = Vz * (-1);
	}
	BPoint Vy = Vz % Vx;
	if(!Vy.Make1(MIND))
		return BMatr();
	Vz = Vx % Vy;
	BMatr ProgMatr;
	ProgMatr.SetRow(0, Vx);
	ProgMatr.SetRow(1, Vy);
	ProgMatr.SetRow(2, Vz);
	return ProgMatr;
}

void NChannel::ProcProbeTouch(RunStepsStopReason StopReason, const NEvalStopCrit& SC)
{
	flNeedProbe = (StopReason == SR_NEED_PROBE);
	flNeedTouch = (StopReason == SR_NEED_TOUCH);
	if (flNeedProbe || flNeedTouch)
	{
		ProbeCadrID = GetUnitCn()->GetState()->GetCurHCadrAbsEx();
		ProbeSteps2Go = SC;
	}
}

void NChannel::SetProbePresent()
{
	flProbePresent = true;
}

void NChannel::SetTouchPresent()
{
	flTouchPresent = true;
}

void NChannel::DoTouching(GBodyRender* pActiveRender)
{
	if (pActiveRender == nullptr)
		return;
	GetDoc()->SetBlockUpdateLevel(1);
	NCUnit& Unit = *GetUnitCn();
	NProgram* pProg = Unit.GetCurRunProg();
	int HCadr = ProbeCadrID.Cadr;
	if (pProg == nullptr)
		return; // internal error
	// find current cadr original geom
	NCadrGeom* pCompGeom = pProg->GetCompGeomArray(0).GetAt(HCadr);// Last geom
	if (pCompGeom == nullptr)
		return; // internal error
	if (!(pCompGeom->GetType().flags & CU_ADDC))
	{// Additional coordinates are not changing in this cadr
		return;
	}
	// Additional coordinates change in this cadr
	pCompGeom->SetFlags(pCompGeom->GetType().flags & (~CU_TOUC));// Should remove flag for ProcessTouch works properly
	SMachState AddCoord(&Unit.GetAddCoordsDef());
	SMachState End(AddCoord);
	SMachState Start(AddCoord);
	pProg->GetAddCoordPrev(pCompGeom->GetMSHistNum(), Start);
	pProg->GetAddCoord(pCompGeom->GetMSHistNum(), End);

	// Calculate part of current cadr
	MTMachineTool& MT = pDoc->GetMachTool();
	list<int> MovingAxInds;
	for (int c = 0; c < Start.GetSize() - 1; ++c)
	{
		if (Start.GetCoord(c) != End.GetCoord(c))
		{
			int Index = MT.GetCurConfig().GetAxisIndex(End.GetAName(c));
			if (Index < 0) // axis dosn't exist
				continue;
			MovingAxInds.push_back(MT.GetCurConfig().GetAxisIndex(End.GetAName(c)));
		}
	}
	NCollManagerTouch LocCollManager(pDoc, MovingAxInds, pActiveRender);
	LocCollManager.SetCheckStep(2.);// I can't come up with a smarter approach
	LocCollManager.SetGroupMask(OG_MACHINE | OG_FIXTURES | OG_STOCKS | OG_TOOLS | OG_HOLD);

	Unit.JumpToHCadrRel(pProg, HCadr, true);
	auto Res = Unit.EvaluateCurrent(false);// needed to get correct ProbeCollision value
	if (Res == NCUnitStatePrev::ECR_ERROR)
		return;
	BProgram::ProbePos Pos;
	Pos.SetProbeResult(Unit.GetState()->ProbeCollision ? BProgram::ProbePos::PR_Collision : BProgram::ProbePos::PR_Release);
	CalcProbePart(pCompGeom, pProg, LocCollManager, Pos);
	if (!Pos.IsProcessed())
		return; // internal error
	// store probe results
	double t = Pos.GetActPart();
	NCadrGeom NewGeom;
	NCadrGeom::DivideT(*pCompGeom, t, NewGeom);
	NCadrGeom* pGeom = pProg->GetGeomArray(0).GetAt(HCadr);// Last geom
	if (pGeom == nullptr)
		return; // internal error
	NCadrGeom::DivideT(*pGeom, t, NewGeom);

	AddCoord = Start * (1. - t) + End * t;

	GetUnitCn()->ChangeAddCoords(*NGetProgramListCn(), ProbeCadrID, AddCoord);

	NSyncGenerator SyncGen(*GetDoc());
	SyncGen.SetTimes();

	Unit.JumpToHCadrRel(pProg, HCadr, true);
	GetDoc()->SetBlockUpdateLevel(0);
	RunStepsCn(ProbeSteps2Go);
}

RunStepsStopReason NChannel::RunStepsCn(const NEvalStopCrit& SC)
{
	RunStepsStopReason res = SR_OK;
	//StopDynamicCadr();
	SetChainEnded(true);
	NEvalStopCrit N = SC;
	res = GetUnitCn()->RunSteps(N);
	ProcProbeTouch(res, N);
	if (GetCutting().Started)
	{
		static_cast<CMainFrame*>(AfxGetMainWnd())->StartAppropTimer();
	}
	else
	{
		if (IsNeedProbe() || IsNeedTouch())
			static_cast<CMainFrame*>(AfxGetMainWnd())->PostMessage(WM_APP_CUT_STATE_CHANGED, GetChannelInd());
	}
	return res;
}

void NChannel::StartRender4Run(bool MakeCopy)
{
	auto pRange = Cutting.GetFirstRange();
	if (pRange != nullptr)
	{
		auto RangeState = pRange->GetStockState();
		GQuadRender* pNewStartRender = nullptr;
		if (GetChannelInd() == 0)
		{
			CStocksCollection& Stocks = *pDoc->NGetStockList();
			if (NCMComData::GetType() == AT_MILL_TURN)
			{
				Stocks.ApplyStocksState(RangeState, pRange->GetStockGr4Turn());
			}
			if (!Stocks.GetVisibleRendersList()) //if(!pRender)
				Stocks.CreateStocksRender(); //CreateRenderMill();
			pNewStartRender = static_cast<GQuadRender*>(Stocks.GetVisibleRendersList());
			GetDoc()->NGetStockList()->ChangeVisibleRendersToNULL(MakeCopy);//pRenderRot = nullptr;
		}
		NAnimState* pAnimState = new NAnimState(pNewStartRender, RangeState);
		while (!GetAnimateDisp().Place2RQueue(GetDoc()->IsInterrupted(), pAnimState))
		{
		}
	}
}

bool NChannel::CalcProbePart(const NCadrGeom* pGeom, const BProgram* pProg, NCollManager& LocCollManager, BProgram::ProbePos& Pos)
{
	// Make collision scene
	MTMachineTool& MT = pDoc->GetMachTool();
	MT.MakeActual(); // Spinning flag is not correct without this
	LocCollManager.Reset(MT);
	LocCollManager.Prepare(true);

	BBox StockGabar;
	if (pDoc->IsThereAnyStock())
	{
		StockGabar = pDoc->GetStocksGabar();
		BBox ExObjGabar = pDoc->GetExObjGabar();
		if (ExObjGabar.IsDefined())
		{
			StockGabar.Zmax = max(ExObjGabar.Zmax, StockGabar.Zmax);
			StockGabar.Zmin = min(ExObjGabar.Zmin, StockGabar.Zmin);
		}
	}
	SMachState StartB(&GetUnitCn()->GetAddCoordsDef());// Create empty arrays these arrays will be filled in FillAddCoords
	SMachState EndB(&GetUnitCn()->GetAddCoordsDef());
	auto* pTool = GetCurToolCn();
	NMachUnitPair MUPair(MT, *GetUnitCn());
	NProgWalker ProgWalker(&MUPair, StockGabar, LocCollManager.GetCheckStep(), &StartB, &EndB, false);
	ProgWalker.SetTool(pTool);
	ProgWalker.FillAddCoords(pGeom, pProg);
	if (!ProgWalker.SetGeom(pGeom))
		return false;
	ProgWalker.SetStart();
	auto ProbeResult = Pos.GetProbeResult();
	bool FirstPosColl = LocCollManager.CheckLocCurPosition(&MUPair, pTool);
	if ((ProbeResult == BProgram::ProbePos::PR_Collision) == FirstPosColl)
	{
		Pos.SetProcessed(0.);
		return true;
	}

	double t = -1.;
	while (ProgWalker.NextStep())
	{
		if (FirstPosColl != LocCollManager.CheckLocCurPosition(&MUPair, pTool))
		{
			t = ProgWalker.CalcCurT();
			break;
		}
	}
	if (t < 0)
	{
		Pos.SetProbeResult(BProgram::ProbePos::PR_Empty);
		Pos.SetProcessed(1.);
		return true;
	}
	// Dichotomy
	ProgWalker.StepBack();
	int DichCount = ProgWalker.CalcDichCount();
	for (int s = 0; s < DichCount; ++s)
	{
		ProgWalker.DoubleSteps();
		ProgWalker.NextStep();
		if (FirstPosColl != LocCollManager.CheckLocCurPosition(&MUPair, pTool))
			ProgWalker.StepBack();
	}

	Pos.SetProcessed(ProgWalker.CalcCurT());
	return true;
}

NToolCombined* NChannel::GetCurToolCn()
{
	if (!GetUnitCn())
		return NULL;

	NToolCombined* pTool = NULL;
	if (!GetDoc()->NGetToolList()->Lookup(GetUnitCn()->GetState()->CurToolN, pTool))// There is no current tool
		return NULL;

	return pTool;
}

void NChannel::PrepProgRendersCn(double Eps)
{
	for each(const auto& el in *NGetProgramListCn())
	{
		NProgram* pProg = static_cast<NProgram*>(el.second);
		if(pProg != nullptr)
			pProg->PrepRenders(Eps);
	}
}

void NChannel::PrepErrorsVector(ErrVector& ErrorsVector)
{
	auto& ProgList = *NGetProgramListCn();
	for (auto i = ProgList.GetMainProgCount() - 1; i >= 0; --i)
	{
		CString key;
		NProgram* pProg;
		NGetProgramListCn()->GetAt(i, key, (CObject*&)pProg);
		if (pProg == nullptr)
			continue;
		for (POSITION k = pProg->GetErrorHistory().GetStartPosition(); k != NULL;)
		{
			int Pos;
			NProgError Err;
			pProg->GetErrorHistory().GetNextAssoc(k, Pos, Err);
			ErrorsVector.push_back(std::pair<int, NProgError>(Pos, Err));
		}
	}
	// add group errors
	for (auto i = ProgList.GetSize() - 1; i >= ProgList.GetMainProgCount(); --i)
	{
		CString key;
		NProgram* pProg;
		NGetProgramListCn()->GetAt(i, key, (CObject*&)pProg);
		if (pProg == nullptr)
			continue;
		for (POSITION k = pProg->GetErrorHistory().GetStartPosition(); k != NULL;)
		{
			int Pos;
			NProgError Err;
			pProg->GetErrorHistory().GetNextAssoc(k, Pos, Err);
			if(Pos < 0)// group error
				ErrorsVector.push_back(std::pair<int, NProgError>(Pos, Err));
		}
	}
}

void NChannel::PrepErrorHistory()
{
	// if errors are in a subroutine than add it to subroutine ErrHistory
	// needed for error marks in text view only
	auto& ProgList = *NGetProgramListCn();
	for (auto i = ProgList.GetMainProgCount() - 1; i >= 0; --i)
	{
		CString key;
		NProgram* pProg;
		NGetProgramListCn()->GetAt(i, key, (CObject*&)pProg);
		if (pProg == nullptr)
			continue;
		for (POSITION k = pProg->GetErrorHistory().GetStartPosition(); k != NULL;)
		{
			int Pos;
			NProgError Err;
			pProg->GetErrorHistory().GetNextAssoc(k, Pos, Err);
			const int SubProgInd = Err.GetCadr().Prog;
			if (SubProgInd != i)// error in subprogram
			{
				BProgram* pProg = ProgList.GetProg(Err.GetCadr().Prog);
				if (pProg != nullptr)
					pProg->AddErrorPos(Err.GetCadr().Cadr, Err);
			}
		}
	}
	// if errors are in cycle than change cadrID
	for (auto i = ProgList.GetMainProgCount() - 1; i >= 0; --i)
	{
		CString key;
		NProgram* pProg;
		NGetProgramListCn()->GetAt(i, key, (CObject*&)pProg);
		if (pProg == nullptr)
			continue;
		for (POSITION k = pProg->GetErrorHistory().GetStartPosition(); k != NULL;)
		{
			int Pos;
			NProgError Err;
			pProg->GetErrorHistory().GetNextAssoc(k, Pos, Err);
			NCadrGeom* pGeom = pProg->GetGeomArray(0).GetAt(Pos);
			if (pGeom != nullptr)
			{
				int i = Pos + 1;
				if (pGeom->IsCycle())
				{
					for(; pGeom->IsCycle() && i < pProg->GetHistorySize(); ++i)
						pGeom = pProg->GetGeomArray(0).GetAt(i);
					if (i != pProg->GetHistorySize())
					{
						Err.Set(--pProg->GetCadrByHist(i), Err.GetProg(), Err.GetError());
						pProg->GetErrorHistory().SetAt(Pos, Err);
					}
				}
			}
		}
	}
}

bool NChannel::HaveMinorErrors()
{
	for each (const auto & el in *NGetProgramListCn())
	{
		NProgram* pProg = static_cast<NProgram*>(el.second);
		if (pProg == nullptr)
			continue;
		if (pProg->GetErrorHistory().GetSize() != 0)
			return true;
	}
	return false;
}

bool NChannel::MakeUsedTools(void)
{
	CStringArray T;// It is realy NToolID array
	BProgram::ToolAutoGenHist TAGList;
	GetNeededToolsList(T, TAGList);
	for (int l = 0; l < T.GetSize(); ++l)
	{
		NToolCombined* pToolCom = nullptr;
		if (!GetDoc()->NGetToolList()->Lookup(T[l], pToolCom))
		{
			pToolCom = GetDoc()->MakeNewTool(NToolID(T[l]), _T(""));
		}
		pToolCom->SetIsUsed(true);
	}
	bool AutoZDisp = false;
	CString sval;
	if (GetUnitCn()->GetOtherValue(_T("AddZDispToComp"), sval))
		if (sval == "Yes")
			AutoZDisp = true;
	bool res = false;
	for (int l = 0; l < TAGList.GetSize(); ++l)
	{
		NToolCombined* pToolCom = nullptr;
		if (!GetDoc()->NGetToolList()->Lookup(TAGList[l].AToolID, pToolCom))
			continue;
		if (pToolCom == nullptr)
			continue;
		if (pToolCom->GetToolsNum() == 0)
		{
			CreateAutoTool(*pToolCom, TAGList[l], AutoZDisp);
			res = true;
		}
	}
	return res;
}

void NChannel::DoProbing(GBodyRender* pActiveRender)
{
	if (pActiveRender == nullptr)
		return;
	NCUnit& Unit = *GetUnitCn();
	NProgram* pProg = Unit.GetCurRunProg();
	if (pProg == nullptr)
		return; // internal error
	// find current cadr original geom
	NCadrGeom* pCompGeom = pProg->GetCompGeomArray(0).GetAt(pProg->GetGeomArraySize() - 1);// Last geom
	if (pCompGeom == nullptr)
		return; // internal error
	auto& Pos = pProg->GetProbeHistory().GetAt(pProg->GetProbeHistory().GetSize() - 1);
	// Calculate part of current cadr
	auto* pTool = GetCurToolCn();
	if (pTool == nullptr)
		return;
	NCollManagerProbe LocCollManager(pDoc, pActiveRender, pTool);
	LocCollManager.SetCheckStep(pTool->GetProbeDiam() * 0.5);
	LocCollManager.SetGroupMask(OG_MACHINE | OG_FIXTURES | OG_STOCKS);

	CalcProbePart(pCompGeom, pProg, LocCollManager, Pos);
	if (!Pos.IsProcessed())
		return; // internal error
	// store probe results
	double t = Pos.GetActPart();
	NCadrGeom NewGeom;
	NCadrGeom::DivideT(*pCompGeom, t, NewGeom);
	NCadrGeom* pGeom = pProg->GetGeomArray(0).GetAt(pProg->GetGeomArraySize() - 1);// Last geom
	if (pGeom == nullptr)
		return; // internal error
	NCadrGeom::DivideT(*pGeom, t, NewGeom);
	int HCadr = ProbeCadrID.Cadr;
	Unit.JumpToHCadrRel(pProg, HCadr, true);
	auto Res = Unit.EvaluateCurrent(false);
	if (Res == NCUnitStatePrev::ECR_ERROR)
		return;
	GetDoc()->RegenerateProgramsProbePart(this);

	NSyncGenerator SyncGen(*GetDoc());
	SyncGen.SetTimes();

	Unit.JumpToHCadrRel(pProg, HCadr, true);
	RunStepsCn(ProbeSteps2Go);
}

void NChannel::CrNewAnimateCadr(const NAnimState& AnimState, const NSpeedPar* pSpeedPar)
{
	CStocksCollection& Stocks = *pDoc->NGetStockList();
	if (Stocks.empty())
		return;

	GQuadRender* pStartRender = (GQuadRender*)AnimState.GetpRender();
	GDynamicCadr* pNewDynamicCadr = nullptr;
	switch (AnimState.GetStockState())
	{
	case AT_MILL:
		switch (NCM_PROJECT.GetGlobalState().GetModelMode())
		{
		case NGlobalState::BSP:
			pNewDynamicCadr = new GAnimateCadrMill(pStartRender, pSpeedPar);
			break;
		case NGlobalState::DX:
			pNewDynamicCadr = new GAnimateCadrMillDX(pStartRender, pSpeedPar);
			break;
		}
		break;
	case AT_TURN:
		pNewDynamicCadr = new GAnimateCadrRot(pStartRender, pSpeedPar);
		break;
	}
	delete pDynamicCadr;
	pDynamicCadr = nullptr;
	if (pNewDynamicCadr == nullptr)
		return;
	pNewDynamicCadr->SetMT(&pDoc->GetMachTool(), GetUnitCn(), &GetDoc()->GetCollManager().GetCollTimeLine());
	pDynamicCadr = pNewDynamicCadr;
}

void NChannel::DrawAxes(bool DrawStockMarker, double Brightness, int Tur, int Att)
{
	Shader::ShaderOFF();
	//	Axes.DrawBase(DrawStockMarker, Brightness);
	glPushName(OG_UCS);
	if (Axes.ZeroPos & ZPS_AXIS)
	{
		glPushName(0);
		AxesBases[0] = BPoint(0., 0., 0., 1.);
		Axes.DrawZero(false, Brightness);
		glPopName();
	}
	for each (auto i in GetUnitCn()->GetUsedUCSs())
		DrawBase(i, Tur, Att);
	if (Axes.ZeroPos & ZPS_FSYS)
		DrawBase(-1, Tur, Att);
	if (Axes.ZeroPos & ZPS_SSYS)
		DrawBase(-2, Tur, Att);
	glPopName();
}

void NChannel::DrawBase(int ucs, int Tur, int Att)
{
	if (!GetUnitCn())
		return;
	NCUnitState* pState = GetUnitCn()->GetState();
	if (pState == nullptr)
		return;
	bool Active = (ucs == pState->ActiveUCS);
	NCBase& Base = pState->UCSs[ucs];
	if (ucs >= 0 && (!Base.Visible || (Base.IsEmpty() && pState->DontDrawIfEmpty)))
		return;

	MTMachineTool& Mach = pDoc->GetMachTool();
	SCoordsDef CDef(0);
	SMachState MStateWork(NULL);
	Mach.GetState(MStateWork, CDef);

	CString Str;
	BMatr RMatr;
	if (ucs == -1)
	{
		RMatr = Mach.GetRespMatr(BMatr().Trans(BPoint(0., 0., 0., 1), pState->CurBase.GetBPoint()) * pState->CSMatr, BPoint(pState->MashA, pState->MashB, pState->MashC, 1.)
			, Tur, Att, GetUnitCn()->GetCurStockPosition(), &GetUnitCn()->GetFixedDef());
		Str = _T("FSys");
	}
	else if (ucs == -2)
	{
		RMatr = Mach.GetRespMatr(BMatr().Trans(BPoint(0., 0., 0., 1), pState->CurBase.GetBPoint()) * pState->CSMatr, BPoint(pState->CurBase.a0, pState->CurBase.b0, pState->CurBase.c0, 1.)
			, Tur, Att, GetUnitCn()->GetCurStockPosition(), &GetUnitCn()->GetFixedDef());
		Str = _T("SSys");
	}
	else
	{
		RMatr = Mach.GetRespMatr(BMatr().Trans(BPoint(Base.x0, Base.y0, Base.z0, 1.)), BPoint(Base.a0, Base.b0, Base.c0, 1.)
			, Tur, Att, GetUnitCn()->GetCurStockPosition(), &GetUnitCn()->GetFixedDef());
		if (Base.GetName().IsEmpty() || !pState->UCSs.GetDrawName())
			Str.Format(_T("%d"), ucs + 1);
		else
			Str = Base.GetName();
	}

	if (RMatr.GetRow(0).D2() < 1.e-5)
		RMatr.SetRow(0, RMatr.GetRow(1) % RMatr.GetRow(2));
	if (RMatr.GetRow(1).D2() < 1.e-5)
		RMatr.SetRow(1, RMatr.GetRow(2) % RMatr.GetRow(0));
	if (RMatr.GetRow(2).D2() < 1.e-5)
		RMatr.SetRow(2, RMatr.GetRow(0) % RMatr.GetRow(1));

	glPushMatrix();
	UINT Namegl = ucs + 3;
	glPushName(Namegl);
	AxesBases[Namegl] = RMatr.GetRow(3);

	glMultMatrixd(RMatr.GetArray());
	Mach.SetStateWORecalc(BMTLinkage().SetIndW1(), MStateWork);
	Axes.DrawZero(false, 1., Str.GetBuffer(), Active);

	glPopName();
	glPopMatrix();
}

void NChannel::CreateAutoTool(NToolCombined& ComTool, const NAutoToolGenParams& AutoGenParams, bool AutoZDisp)
{
	NToolCombinedMill* pToolComMill = dynamic_cast<NToolCombinedMill*>(&ComTool);
	if (pToolComMill != nullptr)
	{
		// cutting tool
		double DBot = AutoGenParams.CrMillDiameter;
		if (DBot == 0)
			return;
		double H = AutoGenParams.CrMillHeight;
		if (H == 0)
			H = 40;
		double Alf = AutoGenParams.CrMillAngle;
		double R = AutoGenParams.CrMillRadius;
		double L = AutoGenParams.CrMillLength;
		double Beta = 0.;
		if (AutoGenParams.CrToolType != 0.)
		{
			Beta = (180. - Alf) / 2.;
			R = Alf = 0.;
			L = 0.;
		}
		if (L != 0.)
		{
			R = L;
			Beta = 90 - Alf;
			Alf = 0.;
		}
		auto pTool = new NToolStd(DBot, H, Alf, R, Beta, 0, 0, H);
		pTool->GenToolName();
		if (pTool->GenInit() != 0)
		{
			delete pTool;
			return;
		}
		pToolComMill->AddToolSafe(pTool);

		//shank
		DBot = AutoGenParams.CrShankD;
		if (DBot != 0)
		{

			H = AutoGenParams.CrShankH;
			if (H == 0)
				H = DBot;
			auto pToolSh = new NToolStd(DBot, H, 0, 0, 0, 0, 0, H);
			pToolSh->SetShank(true);
			pToolSh->GenToolName(_T("Shank"));
			if (pToolSh->GenInit() != 0)
				delete pTool;
			else
				pToolComMill->AddToolSafe(pToolSh);
		}
		//holder
		const double HDTop = AutoGenParams.CrHolderD;
		if (HDTop != 0.)
		{
			double HH = AutoGenParams.CrHolderH;
			if (HH == 0.)
				HH = HDTop;
			auto pToolHolder = new NToolStd(CString("Holder")
				, HDTop, HH, 0., 0, 0, 0., 0., HH);
			pToolHolder->SetType(43);
			pToolHolder->SetCutting(false);
			pToolHolder->GenToolName(_T("Hold"));
			if (pToolHolder->GenInit() != 0)
				delete pToolHolder;
			else
				pToolComMill->AddToolSafe(pToolHolder);
		}
		pToolComMill->Attach = AutoGenParams.CrAxisNum;
		pToolComMill->SetColor(pToolComMill->GetColor());
		pToolComMill->SetColorPassive(pToolComMill->ColorPassive);
		pToolComMill->SetColorHolder(pToolComMill->ColorHolder);
		pToolComMill->SetName(pTool->GetName());
		pToolComMill->Zdisp = AutoZDisp ? pToolComMill->GetGabar().dZ() : 0;
		pToolComMill->SetCompShiftAuto(!AutoZDisp);
		pToolComMill->CalcCompShift();
		pToolComMill->IsNameAuto = true;
	}
}

void NChannel::GetNeededToolsList(CStringArray& ToolPosList, BProgram::ToolAutoGenHist& TAGList)
{

	//	Fill array of all tools used in the project
	for (int i = 0; i < NProgramList.GetSize(); ++i)
	{
		CString Key;
		NProgram* pProg = nullptr;
		NProgramList.GetAt(i, Key, (CObject*&)pProg);
		if (pProg == nullptr)
			continue;
		for (int k = 0; k < pProg->GetTCHistSize(); ++k)
		{
			int b;
			NToolID ToolID;
			pProg->GetTCHist(k, b, ToolID);
			if (ToolID.GetToolNum() != 0)
				ToolPosList.Add(ToolID);
		}
		pProg->SyncTCandTAG();
		TAGList.Append(pProg->GetTAGHistory());
	}
}

void NChannel::GetStPosSet(std::set<int>& Inters)
{
	// find intersection of 2 sets
	MTMachineTool& MT = pDoc->GetMachTool();

	const auto& StockPosSet = GetUnitCn()->GetMyStockPos();
	for each (auto el in MT.GetStPosSet())
	{
		if (StockPosSet.find(el) != StockPosSet.end())
			Inters.insert(el);
	}
}

MainTime NChannel::GetEndTime() const
{
	CString Key;
	NProgram* pLastProg = nullptr;
	NGetProgramListCn()->GetAt(NGetProgramListCn()->GetMainProgCount() - 1, Key, (CObject*&)pLastProg);
	if (pLastProg == nullptr)
		return 0;
	if (pLastProg->GetGeomArraysCount() == 0)
		return 0;
	const BGeomArray& LastGeomArray = pLastProg->GetCompGeomArray(0);
	const auto Size = LastGeomArray.GetSize();
	if (Size == 0)
		return 0;
	const NCadrGeom& LastGeom = *LastGeomArray.GetAt(Size - 1);
	return LastGeom.GetEndTime();
}

void NChannel::SetEndTime(MainTime EndTime)
{
	CString Key;
	NProgram* pLastProg = nullptr;
	NGetProgramListCn()->GetAt(NGetProgramListCn()->GetMainProgCount() - 1, Key, (CObject*&)pLastProg);
	if (pLastProg == nullptr)
		return;
	for (int i = 0; i < pLastProg->GetGeomArraysCount(); ++i)
	{
		const BGeomArray& LastGeomArray = pLastProg->GetCompGeomArray(i);
		const auto Size = LastGeomArray.GetSize();
		if(Size > 1)
			LastGeomArray.GetAt(Size - 2)->SetEndTime(EndTime);
		if(Size > 0)
			LastGeomArray.GetAt(Size - 1)->ShiftStartTime2(EndTime);
	}
}

bool NChannel::GetAxesBase(UINT Name, BPoint& P) const
{
	const auto it = AxesBases.find(Name);
	if (it == AxesBases.end())
		return false;
	P = it->second;
	return true;
}

