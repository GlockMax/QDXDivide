#include "stdafx.h"
#include "NWeldCadr.h"

double NWeldCadr::RootR = 0.5;

NWeldCadr::NWeldCadr(void)
{
	RootDist0 = -1.;
	RootDist1 = -1.;
	RootT1_0 = 0.;
	RootT1_1 = 0.;
	ConsiderF = true;
}

NWeldCadr::~NWeldCadr(void)
{
}

bool NWeldCadr::GetRootDist(double t, double *pDist, double *pT1)
{
	if(RootDist0 < 0. || RootDist1 < 0.)
		return false;
	*pDist = RootDist0 * (1. - t) + RootDist1 * t;
	*pT1 = RootT1_0 * (1. - t) + RootT1_1 * t;
	return true;
}
