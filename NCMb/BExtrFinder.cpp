#include "stdafx.h"
#include "math.h"
#include "ConstDef.h"
#include "BExtrFinder.h"


BExtrFinder::BExtrFinder(void)
{
}


BExtrFinder::~BExtrFinder(void)
{
}

void BExtrFinder::Start(BPoint &P)
{
	StartP = LastP = P;
	PlaneN.Set(0., 0., 0., 0.);
	PlaneNDefined = false;
}
	
bool BExtrFinder::NewPoint(BPoint &P)
{// Returns true if extremum found. Can change P.
	const double TOL = 2.e-3;
	BPoint Delta = P - LastP;
	BPoint Signs = (LastP - StartP) ^ Delta;

	bool SignsHasNegative = false;
	for(int i = 0; i < 3; ++i)
	{
		if(Signs[i] < 0.)
		{
			SignsHasNegative = true;
			if(fabs(Delta[i]) >= TOL)
				return true;
		}
	}
	if(SignsHasNegative) // Negative signs found but extremum is not fixed
	{
		for(int i = 0; i < 3; ++i)
		{
			if(Signs[i] < 0.)
			{
				if(fabs(Delta[i]) < TOL)
					P[i] = LastP[i];
			}
		}
	}
	// Check for a plane
	BPoint V = P - LastP;
	if(!PlaneNDefined)
	{// Try to define
		BPoint N = (LastP - StartP) % V;
		double d2 = N.D2();
		if(d2 >= MIND * MIND)
		{
			PlaneN = N.Align2CoordAxis();
			PlaneNDefined = true;
		}
	}
	if(PlaneNDefined)
	{
		double Dist = (PlaneN * V) / sqrt(V.D2());
		if(fabs(Dist) > MIND)
			return true;
	}
	LastP = P;
	return false;
}
