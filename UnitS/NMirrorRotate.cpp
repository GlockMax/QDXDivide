// NMirrorRotate.cpp: implementation of the NMirrorRotate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConstDef.h"
#include "BPoint.h"
#include "math.h"
#include "NMirrorRotate.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

NMirrorRotate::NMirrorRotate()
{
	Reset();
}

NMirrorRotate::~NMirrorRotate()
{
}

double NMirrorRotate::GetX(double x, double y)
{
	if(!Defined)
		return x;
	BPoint b(x, y, 0., 0.);
	BPoint p = b * Matr;
	return p.GetX();
}

double NMirrorRotate::GetY(double x, double y)
{
	if(!Defined)
		return y;
	BPoint b(x, y, 0., 0.);
	BPoint p = b * Matr;
	return p.GetY();
}

BOOL NMirrorRotate::IsDefined()
{
	return Defined;
}

void NMirrorRotate::Reset()
{
	Defined = FALSE;
	Matr.SetE();
}

void NMirrorRotate::SetXY(double m00, double m01, double m10, double m11)
{
	Reset();
	Defined = TRUE;
	Matr.SetRow(0,BPoint(m00,m01,0.,0.));
	Matr.SetRow(1,BPoint(m10,m11,0.,0.));

/*	Defined = TRUE;
	double m[2][2];
	m[0][0] = Matr[0][0]*m00+Matr[0][1]*m10;
	m[0][1] = Matr[0][0]*m01+Matr[0][1]*m11;
	m[1][0] = Matr[1][0]*m00+Matr[1][1]*m10;
	m[1][1] = Matr[1][0]*m01+Matr[1][1]*m11;
	Matr[0][0]=m[0][0];Matr[0][1]=m[0][1];
	Matr[1][0]=m[1][0];Matr[1][1]=m[1][1];
*/
}

BOOL NMirrorRotate::IsMirror()
{
	BOOL ret = FALSE;
	BPoint p0(0.,0.,0.,1.),p1(1.,0.,0.,1.),p2(0.,1.,0.,1.);
	p0 = p0 * Matr;
	p1 = p1 * Matr;
	p2 = p2 * Matr;
	BPoint p = (p1-p0)%(p2-p0);
	return (p.GetZ() < 0);
}

void NMirrorRotate::SetAngle(double a)
{
	Reset();
	Defined = TRUE;
	double rad = - a*PI/180.;
	Matr.SetRow(0,BPoint(cos(rad),sin(rad),0.,0.));
	Matr.SetRow(1,BPoint(-sin(rad),cos(rad),0.,0.));
}
