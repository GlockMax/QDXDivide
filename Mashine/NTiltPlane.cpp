#include "stdafx.h"
#include "ConstDef.h"
#include "math.h"
#include "MTMashineTool.h"
#include "NMachUnitPair.h"
#include "NCUnit.h"
#include "NTiltPlane.h"

NTiltPlane::NTiltPlane(void)
: FixState(NULL), MTState(NULL), MTDef(0)
{
	Changing = false;
	Decision = 1;
	Active = false;
	Cancelling = false;
	StartRegNum = -1;
}

NTiltPlane::~NTiltPlane(void)
{
}

void NTiltPlane::Reset(void)
{
	Active = false;
	Cancelling = true;
	CSMatr.SetE();
	NewSys.SetE();
}


bool NTiltPlane::GetResAngles(const class SCoordsDef& FixedDef, double *pA, double *pB, double *pC) const
{
	if(MTState.GetAName(0) == FixedDef.GetName(3))
		*pA = MTState.GetCoord(0);
	else if(MTState.GetAName(0) == FixedDef.GetName(4))
		*pB = MTState.GetCoord(0);
	else if(MTState.GetAName(0) == FixedDef.GetName(5))
		*pC = MTState.GetCoord(0);

	if(MTState.GetAName(1) == FixedDef.GetName(3))
		*pA = MTState.GetCoord(1);
	else if(MTState.GetAName(1) == FixedDef.GetName(4))
		*pB = MTState.GetCoord(1);
	else if(MTState.GetAName(1) == FixedDef.GetName(5))
		*pC = MTState.GetCoord(1);
	return true;
}

bool NTiltPlane::CalculateParams(class NMachUnitPair* pMUPair, const BMatr &NewSystem, const CString &AName1, const CString &AName2, int Tur, int Att, int iDecision/* = 1*/)
{
	if(!Active)
		return false;

	NewSys = NewSystem;
	Turret = Tur;
	Attach = Att;
	MTState = SMachState(NULL);
	MTDef.Define(AName1 + "\n" + AName2);
	MTState = SMachState(&MTDef);
	Decision = iDecision;

	int DesCount = pMUPair->GetMach().CalculateTiltPlane(*this, pMUPair->GetUnit().GetFixedDef(), pMUPair->GetUnit().GetCurStockPosition());
	if(DesCount <= 0)
		return false;

	return true;
}

int NTiltPlane::Calc2AnglesPairs(const BPoint &V1, const BPoint &V2, const BPoint &T1, const BPoint& T2, const BPoint& X1, const BPoint &X2, double Angles[4])
{
	// ¬ход:
	// V1 - главна€ ось, V2 - зависима€ ось
	// T1 - исходна€ ось инструмента, T2 - результирующа€ ось инструмента
	// X1, X2 - оси X дл€ инструментов. »спользуютс€ только в случае континуума решений по T1, T2
	// Angles[0] [1] перва€ пара углов, [2] [3] втора€ пара
	// ¬озврат:
	// 0 - нет решени€
	// -1 - первый угол любой
	// -2 - второй угол любой
	// -3 - оба угла любые
	// +1 - одно решение
	// +2 - два решени€

	BPoint V1V2 = V1 % V2;
	double SV1V2 = sqrt(V1V2.D2());
	double CV1V2 = V1 * V2;
	double Alf = T2 * V1;
	double Bet = T1 * V2;
	if(SV1V2 < MINSR)
	{// ќбработать
		return 0;
	}
	double D = 1. / (1. - CV1V2 * CV1V2);
	double a = D * (Alf - Bet * CV1V2);
	double b = D * (Bet - Alf * CV1V2);
	// Tres = a * V1 + b * V2 + V1V2 * c;
	// a + CV1V2 * b = T2 * V1 = Alf
	// a * CV1V2 + b = T1 * V2 = Bet
	// |Tres| = 1
	BPoint Q = V1 * a + V2 * b;
	double c = (1. - Q.D2()) / V1V2.D2();
	int Count = 0;
	if(fabs(c) < MINSR * MINSR)
	{
		c = 0.;
		Count = 1;
	}
	else
		Count = 2;
	if(c < 0.)
		return 0;
	c = sqrt(c);
	double c1 = -c; 
	double c2 = +c; 
	BPoint Tres[2];
	Tres[0] = Q + V1V2 * c1;
	Tres[1] = Q + V1V2 * c2;
	// Ќайдены все возможные положени€ оси Tres после поворота вокруг V2
	BPoint T1pV2 = T1 - V2 * (T1 * V2);
	BPoint T2pV1 = T2 - V1 * (T2 * V1);
	bool Arb1 = T1pV2.D2() < MIND * MIND;// first angle is arbitrary
	bool Arb2 = T2pV1.D2() < MIND * MIND;// second angle is arbitrary
	if (Arb1 && Arb2)
		return 0; // T1 is same as V2 and T2 is same as V1
	for(int i = 0; i < Count; ++i)
	{	 
		BPoint TrespV2 = Tres[i] - V2 * (Tres[i] * V2);
		double Angle1 = T1pV2.Angle(TrespV2, V2);
		Angles[i * 2] = Angle1;
		BPoint TrespV1 = Tres[i] - V1 * (Tres[i] * V1);
		double Angle2 = T2pV1.Angle(TrespV1, V1);
		Angles[i * 2 + 1] = -Angle2;
		if (Arb1)
		{
			// Angle2 is correct
			BPoint X2respV1 = X1 * BMatr().rotg(-Angle2, BPoint(0., 0., 0., 1), V1);
			double NewAngle1 = X1.Angle(X2respV1, V1);
			Angles[i * 2] = NewAngle1;
		}
		if (Arb2)
		{
			// Angle1 is correct
			BPoint X1respV2 = X1 * BMatr().rotg(Angle1, BPoint(0., 0., 0., 1), V2);
			double NewAngle2 = X2.Angle(X1respV2, V1);
			Angles[i * 2 + 1] = -NewAngle2;
		}
	}

	return Count;
}
