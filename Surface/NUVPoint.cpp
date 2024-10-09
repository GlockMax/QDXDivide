// NUVPoint.cpp: implementation of the NUVPoint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConstDef.h"
#include "BPoint.h"
#include "NUVPoint.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NUVPoint::NUVPoint(double uu, double vv)
{
	u = uu;
	v = vv;
}

NUVPoint::NUVPoint(const NUVPoint &uv)
{
	*this = uv;
}


NUVPoint::~NUVPoint()
{

}

void NUVPoint::Set(double uu, double vv)
{
	u = uu;
	v = vv;
}

double NUVPoint::GetU() const
{
	return u;
}

double NUVPoint::GetV() const
{
	return v;
}

NUVPoint NUVPoint::operator +(const NUVPoint &p) const
{
	NUVPoint uv;

	uv.Set(u+p.GetU(), v+p.GetV());

	return uv;
}

NUVPoint NUVPoint::operator -(const NUVPoint &p) const
{
	NUVPoint uv;
 
 	uv.Set(u-p.GetU(), v-p.GetV());
 
 	return uv;
}

NUVPoint NUVPoint::operator *(double a) const
{
	NUVPoint uv;

	uv.Set(u*a, v*a);

	return uv;
}

double NUVPoint::operator *(const NUVPoint &p) const
{
	return	u*p.GetU() + v*p.GetV();
}

double NUVPoint::D2() const
{
	return (*this)*(*this);
}

double NUVPoint::Cos2(const NUVPoint &p1,const NUVPoint &p2)
{
	double	d1 = p1.D2(),
			d2 = p2.D2();
	if(d1 < DMIN*DMIN || d2 < DMIN*DMIN)
		return 0;
	return p1*p2/(d1*d2);
}