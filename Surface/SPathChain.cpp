#include "stdafx.h"
#include "NCadrGeom.h"
#include "NTool.h"
#include "NMachUnitPair.h"
#include "MTMashineTool.h"
#include "BSpacePos.h"
#include "NCUnit.h"
#include "MTConfig.h"
#include "SPathChain.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

SPathChain::SPathChain(NMachUnitPair& MU, double Tol) : MUPair(MU), Motions(NULL), Cadrs(NULL), CadrsSamplesNum(NULL)
{
	Clear();
	SetTol(Tol);
}

SPathChain::~SPathChain(void)
{
	Clear();
}
size_t SPathChain::GetSamplesNum(void)
{
	return SamplesNum;
}

void SPathChain::CalcCurState(MTMachineTool& InMach, int CadrNum, double t, SMachState & P, BMTLinkage& ConfNum, const BStockToolInds& STInds) const
{
	const NCadrGeom *pGeom = Cadrs[CadrNum];
	ConfNum = pGeom->GetMTLink();
	BMatr CorrMatr = InMach.GetMCS(pGeom, STInds.turret_, STInds.attach_, *P.GetDef());
	if (pGeom->IsRCP())
	{
		SMachStateFix1 St, En;
		pGeom->CalcCurCoordMS(CorrMatr, 0., St);
		pGeom->CalcCurCoordMS(CorrMatr, 1., En);
		InMach.CalcIntState(pGeom->GetRCPMask(), t, SMachState(St, P.GetDef()), SMachState(En, P.GetDef()), P, STInds.stPos_, STInds.turret_, STInds.attach_);
	}
	else
	{
		SMachStateFix1 Pb;
		pGeom->CalcCurCoordMS(CorrMatr, t, Pb);
		for (int i = 0; i < P.GetSize(); ++i)
			P.SetCoord(i, Pb.GetCoord(i));
	}
}

bool SPathChain::GetSample(MTMachineTool& InMach, size_t Num, SMachState & P, BMTLinkage& ConfNum, const BStockToolInds& STInds) const
{
	if (!InMach.GetCurConfig().IsDefined())
		return false;
	if(NCadrs < 1)
		return false;
	if(SamplesNum < 2)
		return false;
	if(Num >= SamplesNum - 1)
	{// Very last point
		int i = NCadrs - 1;
		for(; i >= 0; --i)
			if(!Cadrs[i]->IsUndef())
				break;
		CalcCurState(InMach, i, 1., P, ConfNum, STInds);
	}
	else
	{
		size_t Count = 0;
		int i = 0;
		for(; Count <= Num && i < NCadrs; ++i)
			Count += CadrsSamplesNum[i];

		if(Count == Num)
		{// Last point of one cadr
			CalcCurState(InMach, i, 0., P, ConfNum, STInds);
		}
		else
		{// Intermediate point
			--i;
			Num -= Count - CadrsSamplesNum[i]; 
			double t = double(Num) / CadrsSamplesNum[i];
			CalcCurState(InMach, i, t, P, ConfNum, STInds);
		}
	}
	return true;
}
bool SPathChain::Define(void)
{
	if(Defined)
		return false;

	Defined = false;

	if(SamplesNum <= 0)
		return false;

	if(NCadrs <= 0)
		return false;

	Defined = true;
	return true;
}
void SPathChain::Clear(void)
{
	Defined = false;
	SamplesNum = 0;
	NCadrs = 0;
	delete[] Cadrs;
	Cadrs = NULL;
	delete[] CadrsSamplesNum;
	CadrsSamplesNum = NULL;
}

size_t SPathChain::GetSamplesNum(size_t i) const
{
	return CadrsSamplesNum[i];
}
int SPathChain::SendCadrs(NCadrGeom *InCadrs, int Size, int Turret, int Attachment)
{
	Clear();
	Motions.reserve(Size);
	Cadrs = new NCadrGeom *[Size];
	SamplesNum = 0;
	delete CadrsSamplesNum;
	CadrsSamplesNum = new size_t[Size];
	for (NCadrs = 0; NCadrs < Size; ++NCadrs)
	{
		auto OldMotionsSize = Motions.size();
		const NCadrGeom &CurCadr = InCadrs[NCadrs];
		MUPair.GetMach().MakeScrewMotion(CurCadr, Motions, Turret, Attachment, MUPair.GetUnit().GetFixedDef());
		Cadrs[NCadrs] = &InCadrs[NCadrs];
		SamplesNum += CadrsSamplesNum[NCadrs] = Motions.size() - OldMotionsSize;
	}
	if (SamplesNum > 0)
		++SamplesNum;
	return NCadrs;
}

bool SPathChain::IsMotionFast(int IM, bool &IsFast) const
{
	size_t SamplesNum = 0;
	for (int i = 0; i < NCadrs; ++i)
	{
		SamplesNum += CadrsSamplesNum[i];
		if (IM < SamplesNum)
		{
			IsFast = Cadrs[i]->IsFast();
			if (!IsFast)
				return false;
			switch (NTool::GetFastColorMode())
			{
			case FCM_ALL:
				return true;
			case FCM_NO:
				return false;
			case FCM_ALLBUTVERT:
				return !Cadrs[i]->IsVerticalUp();
			}
		}
	}
	return false;
}