// MRay.cpp: implementation of the MRay class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MRay.h"
#include "BPoint.h"
#include "ConstDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MRay::MRay()
{
}
MRay::MRay( const BPoint & P, const BPoint & V) : MSLine(P,V)
{
}
MRay::~MRay()
{

}

BPoint MRay::IntersPlane(const MPlane &Pl) const
{ // Find an intersection (if any) between the ray
	// and the plane Pl
	BPoint p = MSLine::IntersPlane(Pl);
	BPoint v = p - GetPoint();
	if(v.D2() < MIND*MIND || v*GetE() > 0.)
		return p;

	return BPoint(0.,0.,0.,-2.);
}


int MRay::PointDirTest(const BPoint & P)
// returns -1 point lie in the negative s/s; +1 - in the positive; 0 - on the base point plane
{
	double s = (P - GetPoint())*GetE();
	if(s != 0.)
		return s > 0 ? 1 : -1;
	return 0;
}
