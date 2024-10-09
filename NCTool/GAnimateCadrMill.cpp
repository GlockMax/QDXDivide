#include "stdafx.h"
#include "ConstDef.h"
#include "math.h"
#include "NCMProject.h"
#include "NMachUnitPair.h"
#include "BSpacePos.h"
#include "BProgram.h"
#include "GQuadRenderMill.h"
#include "ganimatecadrmill.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


GAnimateCadrMill::GAnimateCadrMill(GBodyRender *piStart, const NSpeedPar* pPar)
	: GAnimateCadr(piStart, pPar)
{
}
GAnimateCadrMill::GAnimateCadrMill(const GAnimateCadr & In)
	: GAnimateCadr(In)
{
}
//GAnimateCadrMill::GAnimateCadrMill(void)
//{
//}

GAnimateCadrMill::~GAnimateCadrMill(void)
{
}
bool GAnimateCadrMill::Set(const NCadrGeom & InGeom, const BMatr &RMatr, GBodyRender &End, double t)
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

	double LocStep = pSpeedPar->IsTrackFeed() ? pSpeedPar->GetTimeStep() * pSpeedPar->GetTrackFeedScale() : GetCurLengthStep();
	double Length = pSpeedPar->IsTrackFeed() ? Geom.GetWholeTime() : GetCadrLength(Geom);
	double tStepsNum = (Length - Surplus) / LocStep;
	if (tStepsNum < 1.001) // To prevent StepsNum = 1 when Length is aproximately equal to LocStep
		StepsNum = 0;
	else
		StepsNum = int(tStepsNum);
	
	if( InGeom.GetType().type == addcoord )
	{
		pEnd = &End;
		return true;
	}
	if(StepsNum <= 0)
	{
		Surplus = Surplus - Length;
		pEnd = &End;
		return false;
	}
	Surplus = 0.;
	if (InGeom.IsRotate() || InGeom.IsLine() || InGeom.IsArc())
	{
		pEnd = &End;
		return CalcPlanes();
	}
	StepsNum = 0;
	return false;
}
bool GAnimateCadrMill::NextStep(void)
{
	bool res = true;
	IncrCurTVal();
	if(CurTVal > 1.) 
		res = false;
	else
	{
		CalcNextPlane();
		if (CurTVal == 1.)
		{
			if (pSpeedPar->IsBeepOnProbe() && Geom.GetType().flags & CU_PRBE)
				Beep(1000, 300);
			res = false;
		}
	}
	SetMT2CurPos();
	return res;
}

void GAnimateCadrMill::CalcNextPlane(void)
{
	if (Geom.IsRotate())
	{
		CalcPlane4Rotate();
		return;
	}
	if (LineType)
	{
		BPoint e(TrGeom.GetE() - TrGeom.GetB());
		plane[3] = -(e * (TrGeom.GetB() * (1. - CurTVal) + TrGeom.GetE() * CurTVal));
	}
	else
	{
		BMatr Z90;
		Z90.rotg(90, BPoint(0., 0., 0., 1), TrGeom.GetN());
		BPoint e((TrGeom.GetI() * Z90) * ((TrGeom.IsCWArc()) ? 1 : -1));
		double ax, ay, angle;
		Geom.GetArcA(&ax, &ay, &angle); // We use Geom instead of TrGeom because GetArcA can't work with inclined plane
		if (fabs(angle) < MINAD)
			angle = 360.;
		BMatr Rot;
		Rot.rotg(angle * CurTVal, BPoint(0., 0., 0., 1.), TrGeom.GetN());
		e = (e * Rot);
		plane[0] = e.GetX();
		plane[1] = e.GetY();
		plane[2] = e.GetZ();
		plane[3] = -(e * TrGeom.GetC());
	}
}

bool GAnimateCadrMill::CalcPlanes()
{
	if (Geom.IsRotate())
	{
		BPoint e(Geom.GetStartDirP());
		plane[0] = e.GetX();
		plane[1] = e.GetY();
		plane[2] = e.GetZ();
		plane[3] = -(e * Geom.GetBP());
		return true;
	}
	if (Geom.IsLine())
	{
		LineType = true;
		BPoint e(TrGeom.GetE() - TrGeom.GetB());
		plane[0] = e.GetX();
		plane[1] = e.GetY();
		plane[2] = e.GetZ();
		plane[3] = -(e * TrGeom.GetB());
		return true;
	}
	if (Geom.IsArc())
	{
		LineType = false;
		BMatr Z90;
		Z90.rotg(90, BPoint(0., 0., 0., 1), TrGeom.GetN());
		BPoint e((TrGeom.GetI() * Z90) * ((TrGeom.IsCWArc()) ? 1 : -1));
		plane[0] = e.GetX();
		plane[1] = e.GetY();
		plane[2] = e.GetZ();
		plane[3] = -(e * TrGeom.GetC());
		double ax, ay, angle;
		Geom.GetArcA(&ax, &ay, &angle); // We use Geom instead of TrGeom because GetArcA can't work with inclined plane
		if (fabs(angle) < MINAD)
			angle = 360.;
		if (fabs(angle) > 160.)// 160 is an euristic number
		{
			LargeArc = true;
			BMatr Rot;
			Rot.rotg(angle * 0.5, BPoint(0., 0., 0., 1.), TrGeom.GetN());
			BPoint e1(plane[0], plane[1], plane[2], 0.);
			e1 = e1 * Rot;
			plane1[0] = e1.GetX();
			plane1[1] = e1.GetY();
			plane1[2] = e1.GetZ();
			plane1[3] = -(e1 * TrGeom.GetC());
		}
		return true;
	}
	return false;
}

void GAnimateCadrMill::CalcCurAddCoord(SMachState &AddCoord, double t)
{
	GAnimateCadr::CalcCurAddCoord(AddCoord, GetCurParam());
}
