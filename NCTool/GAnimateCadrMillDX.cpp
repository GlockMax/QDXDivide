#include "stdafx.h"
#include "GAnimateCadrMillDX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//GAnimateCadrMillDX::GAnimateCadrMillDX() : GAnimateCadrMill()
//{
//}

GAnimateCadrMillDX::GAnimateCadrMillDX(class GBodyRender *piStart, const NSpeedPar* pPar) : GAnimateCadrMill(piStart, pPar)
{
}

GAnimateCadrMillDX::GAnimateCadrMillDX(const GAnimateCadr & In) : GAnimateCadrMill(In)
{
}

GAnimateCadrMillDX::~GAnimateCadrMillDX()
{
}

bool GAnimateCadrMillDX::Set(const NCadrGeom & InGeom, const BMatr &RMatr, GBodyRender &End, double t)
{
	tPart = t;
	CurStep = 0;
	CurTVal = 0.;
	CurTime = InGeom.GetStartTime() + InGeom.GetWaitTime0();
	LargeArc = false;
	SetGeom(InGeom);// Geom in initial position
	// It is used for current position determination only (GetCurABC, GetCurPoint)
	TrGeom = Geom;// Geom in current position
	TrGeom.Tr(RMatr);
	StepsNum = 0;
	if (InGeom.IsUndef())
	{
		return false;
	}
	double Length = pSpeedPar->IsTrackFeed() ? Geom.GetWholeTime() : GetCadrLength(Geom);
	if (InGeom.GetType().type == addcoord)
	{
		double LocStep = pSpeedPar->IsTrackFeed() ? pSpeedPar->GetTimeStep() * pSpeedPar->GetTrackFeedScale() : GetCurLengthStep();
		double tStepsNum = Length / LocStep;
		if (tStepsNum < 1.001) // To prevent StepsNum = 1 when Length is aproximately equal to LocStep
			StepsNum = 0;
		else
			StepsNum = int(tStepsNum);
		pEnd = &End;
		return true;
	}
	pEnd = &End;
	return true;
}

bool GAnimateCadrMillDX::NextStep(void)
{
	bool res = true;
	IncrCurTVal();
	if (CurTVal >= 1.)
		res = false;
	SetMT2CurPos();
	return res;
}

