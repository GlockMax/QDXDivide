#include "StdAfx.h"
#include "math.h"
#include "ConstDef.h"
#include "NCadrGeom.h"
#include "MTMashineTool.h"
#include "BSpacePos.h"
#include "NToolCombined.h"
#include "GAnimateCadrMill.h"
#include "GAnimateCadrRot.h"
#include "SMachStateFix.h"
#include "BProgram.h"
#include "NCUnit.h"
#include "NMachUnitPair.h"
#include "NProgWalker.h"

NProgWalker::NProgWalker(NMachUnitPair* pMU, const BBox& StockGab, double LengthS,
						 class SMachState *pPrev, class SMachState *pNext, bool UseTrackFeed)
{
	pMUPair = pMU;
	LengthStep = LengthS;
	pTool = NULL;
	Surplus = 0;
	StepsNum = 0;
	CurStep = 0;
	StockGabar = StockGab;
	pGeom = nullptr;
	TrackFeed = UseTrackFeed;

	if(pPrev->GetSize() > 1)
		pAddCPrev = pPrev;
	else
		pAddCPrev = NULL;

	if(pNext->GetSize() > 1)
		pAddCNext = pNext;
	else
		pAddCNext = NULL;
}

NProgWalker::~NProgWalker(void)
{
}
double NProgWalker::CalcCurT()
{
	if (pGeom == nullptr)
		return -1.;
	if (pGeom->IsUndef())
		return -1.;
	//if (pGeom->IsCycle())
	//	return -1.;

	if(StepsNum <= 0)
	{
		if(CurStep != 0)
			return -1.;
		else
			return 0.;
	}
	return double(CurStep) / StepsNum;
}

TimeSlot NProgWalker::GetCurTimeSlot() const
{
	std::pair<MainTime, MainTime> Res;
	// get current time
	if (pGeom == nullptr)
		return TimeSlot(0, 0);
	const auto TimeStep = StepsNum == 0 ? 0 : pGeom->GetWholeTime() / StepsNum;
	Res.first = pGeom->GetStartTime() + TimeStep * CurStep;
	Res.second = Res.first + TimeStep;
	return Res;
}

MainTime NProgWalker::GetCurTime() const
{
	if (pGeom == nullptr)
		return 0;
	double t = GetCurGeomT();
	return pGeom->GetStartTime() + pGeom->GetWaitTime0() + MainTime(pGeom->GetWorkTime() * t);
}

double NProgWalker::GetCurGeomT() const
{
	double t = double(CurStep) / StepsNum;
	if (TrackFeed)
	{
		double CurTime = t * pGeom->GetWholeTime() - pGeom->GetWaitTime0();
		if (CurTime < 0.)
			t = 0.;
		else if (CurTime < pGeom->GetWorkTime())
			t = CurTime / pGeom->GetWorkTime();
		else
			t = 1.;
	}
	return t;
}

bool NProgWalker::CalcCurCoordPW(BPoint &XYZ, BPoint &ABC, class SMachState &AddCoord)
{
	if (pGeom == nullptr)
		return false;
	double t = 0.;
	if(StepsNum <= 0)
	{
		if(CurStep != 0)
			return false;
	}
	else
	{
		t = GetCurGeomT();
	}

	if (pGeom->IsRCP() && pGeom->IsRotate())
	{
		SMachState P(&pMUPair->GetUnit().GetFixedDef());
		BPoint XYZ0, XYZ1, ABC0, ABC1;
		pGeom->CalcCurCoordMCS(0., MCS, XYZ0, ABC0);
		pGeom->CalcCurCoordMCS(1., MCS, XYZ1, ABC1);
		int Tur = 0;
		int Att = 0;
		if (pTool)
		{
			Tur = pTool->Turret;
			Att = pTool->Attach;
		}
		pMUPair->GetMach().CalcIntState(pGeom->GetRCPMask(), t, SMachState(XYZ0, ABC0, &pMUPair->GetUnit().GetFixedDef())
			, SMachState(XYZ1, ABC1, &pMUPair->GetUnit().GetFixedDef()), P, pGeom->GetStockPos(), Tur, Att);
		XYZ.Set(P.GetCoord(0), P.GetCoord(1), P.GetCoord(2), 1.);
		ABC.Set(P.GetCoord(3), P.GetCoord(4), P.GetCoord(5), 1.);
	}
	else
		pGeom->CalcCurCoordMCS(t, MCS, XYZ, ABC);

	if (pAddCNext != NULL && pAddCPrev != NULL)
	{
		AddCoord = (*pAddCPrev) * (1. - t) + (*pAddCNext) * t;
	}
	return true;
}

void NProgWalker::SetMT2CurPos()
{
	if(!pMUPair)
		return;
	if (pGeom->IsUndef())
		return;
	BPoint XYZ(0., 0., 0., 1.), ABC(0., 0., 0., 1.);
	SMachState AddCoord(NULL);
	if(pAddCPrev)
		AddCoord = *pAddCPrev;
	CalcCurCoordPW(XYZ, ABC, AddCoord);
	pMUPair->GetMach().SetStatePartial(pGeom->GetMTLink(), SMachState(XYZ, ABC, &pMUPair->GetUnit().GetFixedDef()), AddCoord);
	if(pTool)
		pMUPair->GetMach().SetComp(pMUPair->GetUnit(), pTool->GetGabar().Zmax, pTool->GetCompVect(), pTool->Turret, pTool->Attach, StockGabar);
	else
		pMUPair->GetMach().SetComp(pMUPair->GetUnit(), 0., BPoint(0., 0., 0., 0.), 0, 0, StockGabar);
}
bool NProgWalker::NextStep(void)
{
	++CurStep;
	if (CurStep > StepsNum)
		return false;
	SetMT2CurPos();
	return true;
}
int NProgWalker::CalcDichCount() const
{
	double Val = LengthStep / MIND;
	return static_cast<int>(log2(Val));
}
bool NProgWalker::SetStart(void)
{// Set machine tool to the start point of the pGeom
	if (pGeom == nullptr)
		return false;
	if (pGeom->IsUndef())
		return false;
	CurStep = 0;
	SetMT2CurPos();
	return true;
}
bool NProgWalker::SetEnd(void)
{// Set machine tool to the end point of the pGeom
	if (pGeom == nullptr)
		return false;
	if (pGeom->IsUndef())
		return false;
	if(StepsNum == 0)
		StepsNum = 1;
	CurStep = StepsNum;
	SetMT2CurPos();
	return true;
}

void NProgWalker::SetAnimLengthStep()
{
	if (pGeom == nullptr)
		return;
	NCMApplicationType CurType = AT_MILL;
	if(pTool)
		CurType = pTool->GetClass();
	if (TrackFeed)
		SetLengthStep(NSpeedPar::Extern.GetTimeStep() * NSpeedPar::Extern.GetTrackFeedScale());
	else
		SetLengthStep((CurType != AT_TURN) ? 
			NSpeedPar::Extern.GetCurLengthStepMill(*pGeom) :
			NSpeedPar::Extern.GetCurLengthStepRot(*pGeom));

}

bool NProgWalker::SetGeom(const class NCadrGeom *pG)
{
	CurStep = 0;
	if (pG == nullptr)
		return false;
	if (TrackFeed)
	{
		pGeom = pG;
		const auto CadrTime = pG->GetWholeTime();
		StepsNum = int((CadrTime + Surplus) / double(LengthStep));
		if (StepsNum <= 0)
			Surplus = max(0., Surplus + LengthStep);
		else
			Surplus = 0.;
	}
	else
	{
		if (pG->IsUndef())
		{
			StepsNum = 0;
			return false;
		}
		pGeom = pG;
		MCS = pMUPair->GetMach().GetMCS(pGeom, pTool, pMUPair->GetUnit().GetFixedDef());

		double Length = pGeom->GetCadrLength(pAddCPrev, pAddCNext);
		StepsNum = int((Length + Surplus) / LengthStep);
		if (StepsNum <= 0)
			Surplus = max(0., Surplus + Length);
		else
			Surplus = 0.;
	}
	return true;
}
bool NProgWalker::SetLengthStep(double LengthS)
{
	if(LengthStep == LengthS)
		return false;
	if (pGeom == nullptr)
		return false;

	LengthStep = LengthS;
	if(StepsNum <= 0 && Surplus == 0.)
		return true;
	int NewStepsNum = 1;
	if (TrackFeed)
	{
		const auto CadrTime = pGeom->GetWholeTime();
		NewStepsNum = max(1, int(CadrTime / double(LengthStep)));
	}
	else
	{
		double Length = pGeom->GetCadrLength(pAddCPrev, pAddCNext);
		NewStepsNum = max(1, int(Length / LengthStep));
	}
	if (CurStep > 0)
		CurStep = int(CurStep * (double(NewStepsNum) / StepsNum)) + 1;
	StepsNum = NewStepsNum;
	return true;
}

bool NProgWalker::FillAddCoords(const class NCadrGeom *pG, const class BProgram *pProg)
{
	if(pProg == nullptr || pG == nullptr)
		return false;
	if(pAddCNext == NULL || pAddCPrev == NULL)
		return false;

	pProg->GetAddCoord(pG->GetMSHistNum(), *pAddCNext);
	if(pG->GetType().flags & CU_ADDC)
		pProg->GetAddCoordPrev(pG->GetMSHistNum(), *pAddCPrev);
	else
		*pAddCPrev = *pAddCNext;
	return true;
}
