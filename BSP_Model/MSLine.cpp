// MSLine.cpp: implementation of the MSLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MSLine.h"
#include "BPoint.h"
#include "MPlane.h"
#include "MBSPForest.h"
#include "math.h"
#include "ConstDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MSLine::MSLine()
{
	m_b = -1;
	m_e = -1;
}

MSLine::~MSLine()
{

}
bool MSLine::IsDefined()
{
	return m_b >= 0 && m_e >= 0;
}

const BPoint & MSLine::GetPoint() const
{
	return *MFOREST.GetPoint(m_b);
}

const BPoint & MSLine::GetE() const
{
	return *MFOREST.GetPoint(m_e);
}

int MSLine::GetEN()
{
	return m_e;
}

int MSLine::GetPointN()
{
	return m_b;
}

BPoint MSLine::IntersPlane(const MPlane &Pl) const
{ // Find an intersection (if any) between the stright line 
	// and the plane Pl
	BPoint n = Pl.GetNormal();
	double z =	n * GetE();
	if(fabs(z) < MINAR)
		return BPoint(0.,0.,0.,-1.);
	z = 1./z;
	double t = (Pl.GetPoint() - GetPoint()) * n * z;
	return GetPoint() + GetE() * t;
}

MSLine::MSLine(const MPlane &Pl0, const MPlane &Pl1)
{ // Planes intersection
	BPoint	e0 = Pl0.GetNormal(),
			e1 = Pl1.GetNormal();
	BPoint e = e0 % e1; // Direction

	BPoint	p0 = Pl0.GetPoint(),
			p1 = Pl1.GetPoint();
	BPoint r = (p1 - p0)*0.5;
	double s = e0*e1;
	double z = 1. - s*s;
	if( z == 0 )
		return;
	double a = r * (e0 + e1*s) * z * (-1.);
	double b = r * (e1 + e0*s) * z;
	BPoint p = (p0 + p1)*0.5 + e0*a + e1*b; // Base point
	m_e = MFOREST.AddPoints(e);
	m_b = MFOREST.AddPoints(p);
}


MSLine::MSLine( const BPoint & P, const BPoint & V)
{
	m_e = MFOREST.AddPoints(V);
	m_b = MFOREST.AddPoints(P);
}

MSLine::MSLine(int n_p, int n_e)
{
	m_e = n_e;
	m_b = n_p;
}
