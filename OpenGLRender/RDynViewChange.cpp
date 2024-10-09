#include "StdAfx.h"
#include "mmsystem.h"
#include "math.h"
#include "ConstDef.h"
#include "RDynViewChange.h"

unsigned int RDynViewChange::TimePeriod = 500;

RDynViewChange::RDynViewChange(const BMatr &M0, const BMatr &M1)
: StartM(M0), Dir1(0., 0., 0., 1.), Dir2(0., 0., 0., 1.), P1(0., 0., 0., 1.), P2(0., 0., 0., 1.)
{
	t = 0.;
	CurStep = 0;
	CurTime = 0;
	StepsNum = TimePeriod / 10;
	TIMECAPS TC;
	timeGetDevCaps(&TC, sizeof(TC));
	MinTimePeriod = max(TC.wPeriodMin, 1);
	timeBeginPeriod(MinTimePeriod);
	StartTime = 0;

	Dir1 = M0.GetRow(2) % M1.GetRow(2);
	Alf1 = M0.GetRow(2).Angle(M1.GetRow(2), Dir1);
	if(fabs(fabs(Alf1) - 180.) <= MINAR)
		Dir1 = M0.GetRow(1);
	Dir2 = M1.GetRow(2);
	BMatr Temp;
	if(fabs(Alf1) > MINAR)
		Temp.rotg(Alf1, BPoint (0., 0., 0., 1.), Dir1);
	Alf2 = (M0.GetRow(0) * Temp).Angle(M1.GetRow(0), Dir2);
	double S0 = sqrt(M0.GetRow(0).D2());
	double S1 = sqrt(M1.GetRow(0).D2());
	Scale = S1 / S0;
	P1 = M0.GetRow(3);
	P2 = M1.GetRow(3);
}

RDynViewChange::~RDynViewChange(void)
{
	timeEndPeriod(MinTimePeriod);
}
bool RDynViewChange::GetNextMatr(BMatr &M)
{
	const double MIN_STEP = 0.001;
	if(TimePeriod <= CurTime)
		return false;
	int Time = timeGetTime();
	if(CurStep != 0)
	{
		CurTime = Time - StartTime;
		double CalcT = double(CurTime) / TimePeriod;
		double MinT = MIN_STEP * CurStep;
		if(MinT > CalcT)
			Sleep(DWORD((MinT - CalcT) * TimePeriod));
		t = fmax(MIN_STEP * CurStep, double(CurTime) / TimePeriod);
	}
	else
	{
		t = MIN_STEP;
		StartTime = Time;
	}
	++CurStep;
	if(t >= 1.)
		return false;
	CalcCurMatr(M);
	return true;
}
void RDynViewChange::CalcCurMatr(BMatr &M)
{
	BMatr Rot1;
	if(fabs(Alf1) > MINAR)
		Rot1.rotg(Alf1 * t, BPoint(0., 0., 0., 1.), Dir1);
	BMatr Rot2;
	if(fabs(Alf2) > MINAR)
	{
		Rot2.rotg(Alf2 * t, BPoint(0., 0., 0., 1.), Dir2);
	}
	BMatr Trans0;
	Trans0.SetE();
	Trans0.Trans(P1, BPoint(0., 0., 0., 1.));
	BMatr Trans1;
	Trans1.Trans(BPoint(0., 0., 0., 1), P1 * (1. - t) + P2 * t);
	double S = (1. - t) + Scale * t; 
	BMatr Scale;
	Scale.Scale(S, S, S);
	M = StartM * Trans0 * Rot1 * Rot2 * Scale * Trans1;
}