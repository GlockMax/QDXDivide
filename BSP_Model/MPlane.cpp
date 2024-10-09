// MPlane.cpp: implementation of the MPlane class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MPlane.h"
#include "MFace.h"
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
MPlane::MPlane()
{
}

MPlane::MPlane(const MPlane &pl)
{
	m_b = pl.m_b;
	m_n = pl.m_n;
}

MPlane::~MPlane()
{

}

MPlane::MPlane(const BPoint& n, const BPoint& b)
{
	m_n = MFOREST.AddPoints(n);
	m_b = MFOREST.AddPoints(b);
}
MPlane::MPlane(int n, int b)
{
	m_n = n;
	m_b = b;
}

void MPlane::Set(const BPoint& n, const BPoint& b)
{
	m_n = MFOREST.AddPoints(n);
	m_b = MFOREST.AddPoints(b);
}

void MPlane::SetNormal(const BPoint& n)
{
	m_n = MFOREST.AddPoints(n);
}

void MPlane::SetPointOnPlane(const BPoint& b)
{
	m_b = MFOREST.AddPoints(b);
}

const BPoint &MPlane::GetNormal() const
{
	return *MFOREST.GetPoint(m_n);
}

const BPoint &MPlane::GetPoint() const
{
	return *MFOREST.GetPoint(m_b);
}

bool MPlane::operator ==(const MPlane &Pl) const
{
	if(! (*this || Pl))
		return false;
	if(fabs((GetPoint() - Pl.GetPoint()) * GetNormal()) > MIND)
		return false;

	return true;
}

bool MPlane::operator ||(const MPlane & Pl) const
{
	if( (GetNormal() % Pl.GetNormal()).D2() > MINAR*MINAR)
		return false;
	return true;
}

bool MPlane::IsAbove(const BPoint & P) const
{
	return (P - GetPoint())*GetNormal() > 0.;
}
bool MPlane::IsAbove(const MFace & P) const
{
	for(int i=0 ; i<P.GetCount() ; ++i)
		if((*P.GetPoint(i) - GetPoint())*GetNormal() > 0.)
			return true;
	return false;
}

double MPlane::CalcSign(const BPoint &P) const
{
	return (P - GetPoint()) * GetNormal();
}

void MPlane::SetPointN(int N)
{
	m_b = N;
}

void MPlane::SetNormalN(int N)
{
	m_n = N;
}

int MPlane::GetNormalN(void) const
{
	return m_n;
}

bool MPlane::IntersLine(const BPoint & P, const BPoint & e, BPoint &R) const
{
// Find an intersection (if any) between the stright line 
	// and this plane
	double z = GetNormal() * e;
	if (fabs(z) < MINAR)
		return false;
	z = 1. / z;
	double t = ((GetPoint() - P) * GetNormal()) * z;
	R = P + e * t;
	return true;
}

double MPlane::IntersLine(const BPoint& P, const BPoint& e) const
{
	double z = GetNormal() * e;
	if (fabs(z) < MINAR)
		return -1.;
	z = 1. / z;
	return ((GetPoint() - P) * GetNormal()) * z;
}

bool MPlane::Contains(const BPoint & P) const
{
	if(fabs((GetPoint() - P) * GetNormal()) > MIND)
		return false;

	return true;
}

int MPlane::GetPointN(void) const
{
	return m_b;
}
void MPlane::Save(CFile * f, const BMatr &M)
{
	f->Write((void *)&m_b,sizeof(m_b));
	f->Write((void *)&m_n,sizeof(m_n));
	return;
}

void MPlane::Load(CFile * f)
{
	f->Read((void *)&m_b,sizeof(m_b));
	f->Read((void *)&m_n,sizeof(m_n));
	return;
}
