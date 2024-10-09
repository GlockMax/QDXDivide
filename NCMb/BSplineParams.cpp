#include "StdAfx.h"
#include "NCEnums.h"
#include "BSplineParams.h"

BSplineParams::BSplineParams(void)	
{
	Reset();
}

BSplineParams::~BSplineParams(void)
{
}

void BSplineParams::Reset(void)
{
	Active = false;
	Cancelling = false;
	InterpPlane = XY;
	StartCond = Free;
	EndCond = Free;
}

bool BSplineParams::NeedMove(void) const
{
	return EndCond == Tang;
}

