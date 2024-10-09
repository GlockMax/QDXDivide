#include "stdafx.h"
#include "ConstDef.h"
#include "math.h"
#include "BSpacePos.h"
#include "ganimatecadrRot.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

GAnimateCadrRot::GAnimateCadrRot(GBodyRender *piStart, const NSpeedPar* pPar)
	: GAnimateCadr(piStart, pPar)
{
}
GAnimateCadrRot::GAnimateCadrRot(const GAnimateCadr & In)
	: GAnimateCadr(In)
{
}

//GAnimateCadrRot::GAnimateCadrRot(void)
//{
//}

GAnimateCadrRot::~GAnimateCadrRot(void)
{
}
bool GAnimateCadrRot::Set(const NCadrGeom & InGeom, const BMatr &RMatr, GBodyRender &End, double t)
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
	if(InGeom.IsUndef())
	{
		StepsNum = 0;
		return false;
	}
	double Length = pSpeedPar->IsTrackFeed() ? Geom.GetWholeTime() : GetCadrLength(Geom);
	if(InGeom.GetType().type == addcoord)
	{
		double LocStep = pSpeedPar->IsTrackFeed() ? pSpeedPar->GetTimeStep() * pSpeedPar->GetTrackFeedScale() : GetCurLengthStep();

		StepsNum = max(1, int(Length / LocStep));
		pEnd = &End;
		return true;
	}
	StepsNum = 1;
	Surplus = 0.;
	if (InGeom.IsRotate() || InGeom.IsLine() || InGeom.IsArc())
	{
		pEnd = &End;
		return true;
	}
	StepsNum = 0;
	return false;
}
bool GAnimateCadrRot::NextStep(void)
{
	bool res = true;
	if(!Geom.IsRotate() && Geom.GetType().type != addcoord)
	{
		IncrCurTVal();
	}
	else
	{
		// Rotation
		IncrCurTVal();
	}
	if (CurTVal >= 1.)
		res = false;
	SetMT2CurPos();
	return res;
}

double GAnimateCadrRot::GetCurParam(void) noexcept
{
	return tPart;
}

void GAnimateCadrRot::CalcCurAddCoord(SMachState &AddCoord, double t)
{
	if(Geom.GetType().type == addcoord)
		GAnimateCadr::CalcCurAddCoord(AddCoord, GetGeomT());
	else
		GAnimateCadr::CalcCurAddCoord(AddCoord, tPart);
}