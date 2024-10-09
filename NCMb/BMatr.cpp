#include "..\Common\BMatr.h"
#include "..\Common\BMatr.h"
// BMatr.cpp: implementation of the BMatr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConstDef.h"
#include "math.h"
#include "BPoint.h"
#include "BMatr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const BMatr BMatr::MatrZX2XY(0., 1., 0., 0.,
							0., 0., 1., 0.,
							1., 0., 0., 0.,
							0., 0., 0., 1.);

const BMatr BMatr::MatrYZ2XY(0., 0., 1., 0.,
							1., 0., 0., 0.,
							0., 1., 0., 0.,
							0., 0., 0., 1.);


BMatr::BMatr(double m00, double m01, double m02, double m03,
				double m10, double m11, double m12, double m13,
				double m20, double m21, double m22, double m23,
				double m30, double m31, double m32, double m33)
{
	Set(m00, m01, m02, m03,
		m10, m11, m12, m13,
		m20, m21, m22, m23,
		m30, m31, m32, m33);
}

BMatr::~BMatr()
{

}

void BMatr::SetRow(int i,const BPoint &p)
{
// Set i'th row to p
	c[i][0] = p.c[0];
	c[i][1] = p.c[1];
	c[i][2] = p.c[2];
	c[i][3] = p.c[3];
}
const BPoint BMatr::GetRow(int i) const
{
// Return i'ts row as a point
	return BPoint(c[i][0], c[i][1], c[i][2], c[i][3]);
}
double BMatr::GetRowD2(int i) const
{
	const double* cl = c[i];
	return cl[0] * cl[0] + cl[1] * cl[1] + cl[2] * cl[2];
}
int BMatr::CalcDecomp(double& Simm, BPoint& Scale, BPoint Ang[2]) const
{
	// find rotation angles in representation of this matrix as
	// this = scale(Simm, Simm, Simm) * scale( Scale.X, Scale.Y, Scale.Z) * RotX(Ang[*].X) * RotY(Ang[*].Y) * RotZ(Ang[*].Z
	// Simm = +-1
	Simm = (GetRow(0) % GetRow(1)) * GetRow(2) > 0 ? 1. : -1.;//
	Scale.SetX(sqrt(GetRow(0).D2()));
	Scale.SetY(sqrt(GetRow(1).D2()));
	Scale.SetZ(sqrt(GetRow(2).D2()));
	BMatr Tmp = BMatr().Scale(Simm / Scale.GetX(), Simm / Scale.GetY(), Simm / Scale.GetZ()) * (*this);
	BPoint ResX = Tmp.GetRow(0);
	// Find Ang.Z
	// Rotate ResX around Z axis until it lies in XZ plane
	int Res = 0;// 0 - 2 decisions, 1 - continuum decisions (return 2 equal decisions whith Z = 0
	BPoint ResXProj = ResX;
	ResXProj.ProjXY();
	Ang[0].Set(0., 0., 0., 0.);
	Ang[1].Set(0., 0., 0., 0.);
	if (ResXProj.D2() < MIND * MIND)
	{
		Res = 3;
	}
	else
	{
		Ang[0].SetZ(ResXProj.Angle(BPoint(1., 0., 0., 0.)));
		Ang[1].SetZ(ResXProj.Angle(BPoint(-1., 0., 0., 0.)));
	}
	// Z[0] and Z[1] are founded
	for (int d = 0; d < 2; ++d)
	{
		BMatr RotZ = BMatr().RotZ(0., 0., 0., Ang[d].GetZ());
		BPoint ResXz = ResX * RotZ;// ResXz lies in XZ plane
		Ang[d].SetY(ResXz.Angle0_180(BPoint(1., 0., 0., 0.)));
		double sign = ResXz.GetZ() >= 0. ? 1. : -1.;
		Ang[d].SetY(Ang[d].GetY() * sign);
		BPoint CurZ = (Tmp.GetRow(2) * RotZ) * BMatr().RotY(0., 0., 0., Ang[d].GetY());// CurZ lies in YZ plane
		Ang[d].SetX(CurZ.Angle0_180(BPoint(0., 0., 1., 0.)));
		sign = CurZ.GetY() >= 0. ? 1. : -1.;
		Ang[d].SetX(Ang[d].GetX() * sign);
		Ang[d] = Ang[d] * (-1.);
	}

	return Res;
}
int BMatr::ExtractYRot(BMatr& MRotY)
{
	// Find Y rotation matrix MY in reprezentation of this matrix as this = X * MY.
	// Return reciprocal to MY matrix MRotY
	// Assuming that this is pure rotation matrix and Z lies in XZ
	// Matrix X - doesn't contain Y rotation
	// 
	// Find Ang.Z
	// Rotate ResZ around Y axis until it lies in XZ plane
	int Res = 0;// 0 - 2 decisions, 1 - continuum decisions (return 2 equal decisions whith Z = 0
	BPoint ResY = GetRow(1);
	BPoint ResZ = GetRow(2);

	if (ResZ.D2() < MIND * MIND)
	{
		MRotY.SetE();
		return -1;
	}
	ResZ = ResZ * (ResY.GetY() >= 0. ? 1.: -1.);
	double Angle = ResZ.Angle(BPoint(0., 0., 1., 0.), BPoint(0., 1., 0., 0.));
	MRotY.RotY(0., 0., 0., Angle);
	return 0;
}
BMatr BMatr::operator *(const BMatr &m) const
{
// Matrix Multiplication
// (*this) * m
	BMatr r;
	r.c[0][0] = c[0][0] * m.c[0][0] + c[0][1] * m.c[1][0] + c[0][2] * m.c[2][0] + c[0][3] * m.c[3][0];
	r.c[0][1] = c[0][0] * m.c[0][1] + c[0][1] * m.c[1][1] + c[0][2] * m.c[2][1] + c[0][3] * m.c[3][1];
	r.c[0][2] = c[0][0] * m.c[0][2] + c[0][1] * m.c[1][2] + c[0][2] * m.c[2][2] + c[0][3] * m.c[3][2];
	r.c[0][3] = c[0][0] * m.c[0][3] + c[0][1] * m.c[1][3] + c[0][2] * m.c[2][3] + c[0][3] * m.c[3][3];

	r.c[1][0] = c[1][0] * m.c[0][0] + c[1][1] * m.c[1][0] + c[1][2] * m.c[2][0] + c[1][3] * m.c[3][0];
	r.c[1][1] = c[1][0] * m.c[0][1] + c[1][1] * m.c[1][1] + c[1][2] * m.c[2][1] + c[1][3] * m.c[3][1];
	r.c[1][2] = c[1][0] * m.c[0][2] + c[1][1] * m.c[1][2] + c[1][2] * m.c[2][2] + c[1][3] * m.c[3][2];
	r.c[1][3] = c[1][0] * m.c[0][3] + c[1][1] * m.c[1][3] + c[1][2] * m.c[2][3] + c[1][3] * m.c[3][3];

	r.c[2][0] = c[2][0] * m.c[0][0] + c[2][1] * m.c[1][0] + c[2][2] * m.c[2][0] + c[2][3] * m.c[3][0];
	r.c[2][1] = c[2][0] * m.c[0][1] + c[2][1] * m.c[1][1] + c[2][2] * m.c[2][1] + c[2][3] * m.c[3][1];
	r.c[2][2] = c[2][0] * m.c[0][2] + c[2][1] * m.c[1][2] + c[2][2] * m.c[2][2] + c[2][3] * m.c[3][2];
	r.c[2][3] = c[2][0] * m.c[0][3] + c[2][1] * m.c[1][3] + c[2][2] * m.c[2][3] + c[2][3] * m.c[3][3];

	r.c[3][0] = c[3][0] * m.c[0][0] + c[3][1] * m.c[1][0] + c[3][2] * m.c[2][0] + c[3][3] * m.c[3][0];
	r.c[3][1] = c[3][0] * m.c[0][1] + c[3][1] * m.c[1][1] + c[3][2] * m.c[2][1] + c[3][3] * m.c[3][1];
	r.c[3][2] = c[3][0] * m.c[0][2] + c[3][1] * m.c[1][2] + c[3][2] * m.c[2][2] + c[3][3] * m.c[3][2];
	r.c[3][3] = c[3][0] * m.c[0][3] + c[3][1] * m.c[1][3] + c[3][2] * m.c[2][3] + c[3][3] * m.c[3][3];
	//for(int i = 3 ; i >= 0 ; --i)
	//	for(int j = 3 ; j >= 0 ; --j) 
	//		r.c[i][j] = c[i][0] * m.c[0][j] + c[i][1] * m.c[1][j] + c[i][2] * m.c[2][j] + c[i][3] * m.c[3][j];
	return r;
}

const BMatr & BMatr::SetE()
{
	c[0][0] = 1.;c[1][0] = 0.;c[2][0] = 0.;c[3][0] = 0.;
	c[0][1] = 0.;c[1][1] = 1.;c[2][1] = 0.;c[3][1] = 0.;
	c[0][2] = 0.;c[1][2] = 0.;c[2][2] = 1.;c[3][2] = 0.;
	c[0][3] = 0.;c[1][3] = 0.;c[2][3] = 0.;c[3][3] = 1.;
	return *this;
}

const BMatr & BMatr::Trans(double x, double y, double z)
{
	SetE();
	c[3][0] = x; c[3][1] = y; c[3][2] = z;
	return *this;
}
const BMatr & BMatr::RotX(double x, double y, double z, double a)
{
// Rotate around OX parallel axis starting in x,y,z point
// Angle a in degrees
	SetE();
	double ang = a*PI/180.;
	double ca = cos(ang), sa = sin(ang);
	c[1][1] = ca; c[1][2] = sa;
	c[2][1] =-sa; c[2][2] = ca;
	if(x != 0. || y != 0. || z != 0.)
	{
		BMatr t,d;
		t.Trans(-x, -y, -z);
		d.Trans(x, y, z);
		*this = t * (*this) * d;
	}
	return *this;
}
const BMatr & BMatr::RotY(double x, double y, double z, double a)
{
// Rotate around OY parallel axis starting in x,y,z point
// Angle a in degrees
	SetE();
	double ang = a * PI / 180.;
	double ca = cos(ang), sa = sin(ang);
	c[0][0] = ca; c[0][2] = -sa;
	c[2][0] = sa; c[2][2] = ca;
	if(x != 0. || y != 0. || z != 0.)
	{
		BMatr t,d;
		t.Trans(-x, -y, -z);
		d.Trans(x, y, z);
		*this = t * (*this) * d;
	}
	return *this;
}
const BMatr & BMatr::RotZ(double x, double y, double z, double a)
{
// Rotate around OZ parallel axis starting in x,y,z point
// Angle a in degrees
	double ang = a * PI / 180.;
	double ca = cos(ang), sa = sin(ang);
	c[0][0] = ca; c[0][1] = sa; c[0][2] = 0.; c[0][3] = 0.; 
	c[1][0] =-sa; c[1][1] = ca; c[1][2] = 0.; c[1][3] = 0.;
	c[2][0] = 0.; c[2][1] = 0.; c[2][2] = 1.; c[2][3] = 0.;
	//if(x != 0. || y != 0. || z != 0.)
	//{
	//	BMatr t,d;
	//	t.Trans(-x,-y,-z);
	//	d.Trans(x,y,z);
	//	*this = t*(*this)*d;
	//}
	// Strings above replaced 31.01.11 for a productivity
	double c1 = 1. - ca;
	c[3][0] = c1 * x + sa * y; 
	c[3][1] = c1 * y - sa * x;
	c[3][2] = 0.;
	c[3][3] = 1.;
	return *this;
}
const BMatr & BMatr::Scale(double x, double y, double z)
{
	SetE();
	c[0][0] = x;
	c[1][1] = y;
	c[2][2] = z;
	return *this;
}
const BMatr & BMatr::RotX(const BPoint& p, double a)
{
	return RotX(p.c[0], p.c[1], p.c[2], a);
}
const BMatr & BMatr::RotY(const BPoint& p, double a)
{
	return RotY(p.c[0], p.c[1], p.c[2], a);
}
const BMatr & BMatr::RotZ(const BPoint& p, double a)
{
	return RotZ(p.c[0], p.c[1], p.c[2], a);
}

const BMatr& BMatr::rotg(double angle,const BPoint &p0,const BPoint &p1)
{
// Rotation around an arbitrary axis
// P0 - base axis point; P1 may be second axis point if H1>0 or an axis
// directional vector if H1 == 0
	BPoint p2(p0), p3(p1);
	(*this).SetE();
	if(p2.GetH() <= 0.)  return *this;
	if(p3.GetH() <  0.) return *this;
	if(p3.GetH() > 0.)
	{
	  p3.Norm();
	  p2.Norm();
	  p3 -= p2;
	}
	double a =sqrt(p3.D2());
	if(a < LMIN)
		return *this;
	p3 = p3 * (1. / a);
	a = angle * PI / 180.;
	double b = sin(a);
	a = cos(a);
	SetE();
	SetRow(3,p3);
	c[0][0]=c[3][0]*c[3][0]+(1-c[3][0]*c[3][0])*a;
	c[0][1]=c[3][0]*c[3][1]*(1-a)+c[3][2]*b;
	c[0][2]=c[3][0]*c[3][2]*(1-a)-c[3][1]*b;
	c[1][0]=c[3][0]*c[3][1]*(1-a)-c[3][2]*b;
	c[1][1]=c[3][1]*c[3][1]+(1-c[3][1]*c[3][1])*a;
	c[1][2]=c[3][1]*c[3][2]*(1-a)+c[3][0]*b;
	c[2][0]=c[3][0]*c[3][2]*(1-a)+c[3][1]*b;
	c[2][1]=c[3][1]*c[3][2]*(1-a)-c[3][0]*b;
	c[2][2]=c[3][2]*c[3][2]+(1-c[3][2]*c[3][2])*a;
	p3.Set(0.,0.,0.,1.);
	SetRow(3,p3);
	if(p2.D2() >= MIND * MIND)
	{
		BMatr t0,t1;
		t0.Trans(p2,p3);
		t1.Trans(p3,p2);
		*this=t0*(*this)*t1;
	}
	return *this;
}

const BMatr& BMatr::rotgDer(double angle,const BPoint &p0,const BPoint &p1, BMatr &Der)
{
// Rotation around an arbitrary axis
// P0 - base axis point; P1 may be second axis point if H1>0 or an axis
// directional vector if H1 == 0
	BPoint p2(p0), p3(p1);
	(*this).SetE();
	if(p2.GetH() <= 0.)  return *this;
	if(p3.GetH() <  0.) return *this;
	if(p3.GetH() > 0.)
	{
	  p3.Norm();
	  p2.Norm();
	  p3 -= p2;
	}
	double a =sqrt(p3.D2());
	if(a < LMIN)
		return *this;
	p3 = p3 * (1. / a);
	a = angle * PI / 180.;
	double b = sin(a);
	a = cos(a);
	SetE();
	SetRow(3,p3);

	Der.SetE();
	Der.c[0][0]=(1-c[3][0]*c[3][0])*(-b);
	Der.c[0][1]=c[3][0]*c[3][1]*b+c[3][2]*a;
	Der.c[0][2]=c[3][0]*c[3][2]*b-c[3][1]*a;
	Der.c[1][0]=c[3][0]*c[3][1]*b-c[3][2]*a;
	Der.c[1][1]=(1-c[3][1]*c[3][1])*(-b);
	Der.c[1][2]=c[3][1]*c[3][2]*b+c[3][0]*a;
	Der.c[2][0]=c[3][0]*c[3][2]*b+c[3][1]*a;
	Der.c[2][1]=c[3][1]*c[3][2]*b-c[3][0]*a;
	Der.c[2][2]=(1-c[3][2]*c[3][2])*(-b);

	Der.c[3][3] = 0;

	c[0][0]=c[3][0]*c[3][0]+(1-c[3][0]*c[3][0])*a;
	c[0][1]=c[3][0]*c[3][1]*(1-a)+c[3][2]*b;
	c[0][2]=c[3][0]*c[3][2]*(1-a)-c[3][1]*b;
	c[1][0]=c[3][0]*c[3][1]*(1-a)-c[3][2]*b;
	c[1][1]=c[3][1]*c[3][1]+(1-c[3][1]*c[3][1])*a;
	c[1][2]=c[3][1]*c[3][2]*(1-a)+c[3][0]*b;
	c[2][0]=c[3][0]*c[3][2]*(1-a)+c[3][1]*b;
	c[2][1]=c[3][1]*c[3][2]*(1-a)-c[3][0]*b;
	c[2][2]=c[3][2]*c[3][2]+(1-c[3][2]*c[3][2])*a;
	p3.Set(0.,0.,0.,1.);
	SetRow(3,p3);
	if(p2.D2() >= MIND * MIND)
	{
		BMatr t0,t1;
		t0.Trans(p2,p3);
		t1.Trans(p3,p2);
		*this=t0*(*this)*t1;
		Der = t0 * Der * t1;
	}
	return *this;
}
const BMatr & BMatr::Trans(const BPoint &Pb, const BPoint &Pe)
{

	return Trans(	Pe.c[0] - Pb.c[0],
					Pe.c[1] - Pb.c[1],
					Pe.c[2] - Pb.c[2]);
}

const BMatr & BMatr::Trans(const BPoint &V)
{

	return Trans(	V.c[0],
					V.c[1],
					V.c[2]);
}

const BMatr& BMatr::e0cong(const BPoint &p1,const BPoint &p2)
{
// Find the matrix which transforms p1 to p2 by rotation
// p1 and p2 must be non zero
	BPoint p(0., 0., 0., 1.);
	p = p1 % p2;
	double dp = sqrt(p.D2());
	double dp1 = sqrt(p1.D2());
	double dp2 = sqrt(p2.D2());
	double a = p1 * p2;
	if(dp < dp1 * dp2 * sin(PS))//A.
	{
		if (a > 0.)
		{// p1 and p2 have same directions
			SetE();
			return *this;
		}
		// opposite directions
		BPoint t(1., 0., 0., 0.);
		p = t % p1;
		dp = sqrt(p.D2());
		if (dp < dp1 * sin(PS))
		{
			p = BPoint(0., 1., 0., 0.) % p1;
			dp = sqrt(p.D2());
		}
	}
	p = p * (1. / dp);
	a /= dp1 * dp2;
	double b = sqrt(fabs(1. - a * a));
	double	x = p.c[0],
			y = p.c[1],
			z = p.c[2];
	c[0][0] = x*x+(1-x*x)*a;
	c[0][1] = x*y*(1-a)+z*b;
	c[0][2] = x*z*(1-a)-y*b;
	c[0][3] = 0.;
	c[1][0] = x*y*(1-a)-z*b;
	c[1][1] = y*y+(1-y*y)*a;
	c[1][2] = y*z*(1-a)+x*b;
	c[1][3] = 0.;
	c[2][0] = x*z*(1-a)+y*b;
	c[2][1] = y*z*(1-a)-x*b;
	c[2][2] = z*z+(1-z*z)*a;
	c[2][3] = 0.;
	c[3][0] = 0.;
	c[3][1] = 0.;
	c[3][2] = 0.;
	c[3][3] = 1.;
	return *this;
}

double * BMatr::GetArray() const
{
	return (double *)c;
}

BMatr::BMatr(double a[])
{
	// constructor from array
	int k=0;
	for(int i=0; i<4 ; ++i)
		for(int j=0; j<4 ; ++j)
		{
			c[i][j] = a[k++];
		}
}
BMatr::BMatr(double a[4][4])
{
	// constructor from array
	for(int i=0; i<4 ; ++i)
		for(int j=0; j<4 ; ++j)
		{
			c[i][j] = a[i][j];
		}
}
// Set All matrix
void BMatr::Set(double m00,double m01,double m02,double m03,
				double m10,double m11,double m12,double m13,
				double m20,double m21,double m22,double m23,
				double m30,double m31,double m32,double m33)
{
	c[0][0]=m00;c[0][1]=m01;c[0][2]=m02;c[0][3]=m03;
	c[1][0]=m10;c[1][1]=m11;c[1][2]=m12;c[1][3]=m13;
	c[2][0]=m20;c[2][1]=m21;c[2][2]=m22;c[2][3]=m23;
	c[3][0]=m30;c[3][1]=m31;c[3][2]=m32;c[3][3]=m33;
}
// умножение матрицы 4x4 на точку
BPoint BMatr::operator *(const BPoint &p) const
{
	double pt[4];
	for (int i=0; i<4; i++)
	{
		pt[i] = 0.;
		for(int j=0;j<4;j++)
			pt[i] += c[i][j]*p.c[j];
	}
	BPoint res(pt[0],pt[1],pt[2],pt[3]);
    return res;
}

#define CH(i,j) b=c[i][j];c[i][j]=c[j][i];c[j][i]=b
const BMatr & BMatr::T(void)
{
  // transpose this matrix 
	double b;
	CH(0,1);CH(0,2);CH(0,3);
	CH(1,2);CH(1,3);
	CH(2,3);
	return *this;
}
double BMatr::det() const
{
    return
    c[0][0]*(c[1][1]*(c[2][2]*c[3][3]-c[2][3]*c[3][2])+
             c[1][2]*(c[2][3]*c[3][1]-c[2][1]*c[3][3])+
             c[1][3]*(c[2][1]*c[3][2]-c[2][2]*c[3][1]))-
    c[0][1]*(c[1][0]*(c[2][2]*c[3][3]-c[2][3]*c[3][2])+
             c[1][2]*(c[2][3]*c[3][0]-c[2][0]*c[3][3])+
             c[1][3]*(c[2][0]*c[3][2]-c[2][2]*c[3][0]))+
    c[0][2]*(c[1][0]*(c[2][1]*c[3][3]-c[2][3]*c[3][1])+
             c[1][1]*(c[2][3]*c[3][0]-c[2][0]*c[3][3])+
             c[1][3]*(c[2][0]*c[3][1]-c[2][1]*c[3][0]))-
    c[0][3]*(c[1][0]*(c[2][1]*c[3][2]-c[2][2]*c[3][1])+
             c[1][1]*(c[2][2]*c[3][0]-c[2][0]*c[3][2])+
             c[1][2]*(c[2][0]*c[3][1]-c[2][1]*c[3][0]));
  }
const BMatr BMatr::invr() const
// find the reciprocal matrix
{
	double d = det();
	if (fabs(d) < DMIN * DMIN * DMIN)
		return *this;
	d = 1. / d;
	BMatr ai;
	for (int i = 0, sign = 1; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			ai.c[i][j] = adop(i, j) * d * sign;
			sign = -sign;
		}
		sign = -sign;
	}
	return ai.T();
}
double BMatr::adop(int i,int j) const
//  find the algebraic adjunct to i,j element
{
	double buf[3][3];
	for (int k = 0, kk = 0; k < 4; k++)
	{
		if (k != i)
		{
			for (int l = 0, ll = 0; l < 4; l++)
				if (l != j)
					buf[kk][ll++] = c[k][l];
			kk++;
		}
	}
	return buf[0][0] * (buf[1][1] * buf[2][2] - buf[2][1] * buf[1][2]) -
		buf[0][1] * (buf[1][0] * buf[2][2] - buf[2][0] * buf[1][2]) +
		buf[0][2] * (buf[1][0] * buf[2][1] - buf[2][0] * buf[1][1]);
}

const BMatr BMatr::Transpose(const BMatr & M) const
{
// Returns transposition matrix from this to M
	BMatr buf = *this;
	return M * buf.invr();
}

const BMatr& BMatr::drotg(double angle1, double angle2, const BPoint &p0,const BPoint &p1)//!!!
{
	//angle1 - угол a0*(1-t)+a1*t
	//angle2 - угол a1-a0;, т.е. производная от линейной функции угла 
	double a,b;
	BPoint p2(p0), p3(p1);
	BMatr t0,t1;
	//(*this).SetE();
	if(p2.GetH() <= 0.)  return *this;
	if(p3.GetH() <  0.) return *this;
	if(p3.GetH() > 0.)
	{
	  p3.Norm();p2.Norm();
	  p3=p3-p2;
	}
	a=sqrt(p3.D2());
	if(a<LMIN)
		return *this;
	p3=p3*(1./a);
	//a=angle*PI/180.;
	angle1 *= PI/180.;
	angle2 *= PI/180.;
	b = angle2 * cos(angle1);
	a = angle2 * (-sin(angle1));
	Set0();
	SetRow(3,p3);
	c[0][0]=/*c[3][0]*c[3][0]+*/(1-c[3][0]*c[3][0])*a;
	c[0][1]=c[3][0]*c[3][1]*(/*1*/-a)+c[3][2]*b;
	c[0][2]=c[3][0]*c[3][2]*(/*1*/-a)-c[3][1]*b;
	c[1][0]=c[3][0]*c[3][1]*(/*1*/-a)-c[3][2]*b;
	c[1][1]=/*c[3][1]*c[3][1]+*/(1-c[3][1]*c[3][1])*a;
	c[1][2]=c[3][1]*c[3][2]*(/*1*/-a)+c[3][0]*b;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	c[2][0]=c[3][0]*c[3][2]*(/*1*/-a)+c[3][1]*b;
	c[2][1]=c[3][1]*c[3][2]*(/*1*/-a)-c[3][0]*b;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	c[2][2]=/*c[3][2]*c[3][2]+*/(1-c[3][2]*c[3][2])*a;
	//p3.Set(0.,0.,0.,1.);
	p3.Set(0.,0.,0.,0.);
	//SetRow(2,p3);
	SetRow(3,p3);
	p3.Set(0.,0.,0.,1.);
	t0.Trans(p2,p3);
	t1.Trans(p3,p2);
	*this=t0*(*this)*t1;
	return *this;
}
void BMatr::Set0()
{
	c[0][0] = 0.;c[1][0] = 0.;c[2][0] = 0.;c[3][0] = 0.;
	c[0][1] = 0.;c[1][1] = 0.;c[2][1] = 0.;c[3][1] = 0.;
	c[0][2] = 0.;c[1][2] = 0.;c[2][2] = 0.;c[3][2] = 0.;
	c[0][3] = 0.;c[1][3] = 0.;c[2][3] = 0.;c[3][3] = 0.;
}
BMatr BMatr::operator +(const BMatr &m) const//!!!
{
	return BMatr(
		c[0][0] + m.c[0][0], c[0][1] + m.c[0][1], c[0][2] + m.c[0][2], c[0][3] + m.c[0][3],
		c[1][0] + m.c[1][0], c[1][1] + m.c[1][1], c[1][2] + m.c[1][2], c[1][3] + m.c[1][3],
		c[2][0] + m.c[2][0], c[2][1] + m.c[2][1], c[2][2] + m.c[2][2], c[2][3] + m.c[2][3],
		c[3][0] + m.c[3][0], c[3][1] + m.c[3][1], c[3][2] + m.c[3][2], c[3][3] + m.c[3][3]
		);
}

const BMatr &BMatr::operator +=(const BMatr &m)
{
	BMatr r;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			c[i][j] += m.c[i][j];
	return *this;
}

const BMatr & BMatr::NormRows(void)
{
	for(int i = 0; i < 3; ++i)
	{
		double d = c[i][0] * c[i][0] + c[i][1] * c[i][1] +c[i][2] * c[i][2];
		if(d > 0)
			d = 1. / sqrt(d);
		c[i][0] *= d;
		c[i][1] *= d;
		c[i][2] *= d;
	}
	return *this;
}

BMatr BMatr::operator *(double d) const
{
	return BMatr(
		c[0][0] * d, c[0][1] * d, c[0][2] * d, c[0][3] * d,
		c[1][0] * d, c[1][1] * d, c[1][2] * d, c[1][3] * d,
		c[2][0] * d, c[2][1] * d, c[2][2] * d, c[2][3] * d,
		c[3][0] * d, c[3][1] * d, c[3][2] * d, c[3][3] * d
		);
}

BMatr BMatr::operator -(const BMatr &m) const
{
	return BMatr(
		c[0][0] - m.c[0][0], c[0][1] - m.c[0][1], c[0][2] - m.c[0][2], c[0][3] - m.c[0][3],
		c[1][0] - m.c[1][0], c[1][1] - m.c[1][1], c[1][2] - m.c[1][2], c[1][3] - m.c[1][3],
		c[2][0] - m.c[2][0], c[2][1] - m.c[2][1], c[2][2] - m.c[2][2], c[2][3] - m.c[2][3],
		c[3][0] - m.c[3][0], c[3][1] - m.c[3][1], c[3][2] - m.c[3][2], c[3][3] - m.c[3][3]
		);
}

bool BMatr::IsE(void) const
{
	return HasNotRot() && HasNotTran();
}

bool BMatr::HasNotRot(void) const
{
	if (fabs(c[0][0] - 1.) > MIND)
		return false;
	if (fabs(c[1][1] - 1.) > MIND)
		return false;
	if (fabs(c[2][2] - 1.) > MIND)
		return false;
	if (fabs(c[3][3] - 1.) > MIND)
		return false;

	if (fabs(c[0][1]) > MIND)
		return false;
	if (fabs(c[0][2]) > MIND)
		return false;
	if (fabs(c[0][3]) > MIND)
		return false;

	if (fabs(c[1][0]) > MIND)
		return false;
	if (fabs(c[1][2]) > MIND)
		return false;
	if (fabs(c[1][3]) > MIND)
		return false;

	if (fabs(c[2][1]) > MIND)
		return false;
	if (fabs(c[2][0]) > MIND)
		return false;
	if (fabs(c[2][3]) > MIND)
		return false;

	return true;
}

bool BMatr::HasNotTran(void) const
{
	if (fabs(c[3][1]) > MIND)
		return false;
	if (fabs(c[3][2]) > MIND)
		return false;
	if (fabs(c[3][0]) > MIND)
		return false;
	return true;
}

BMatr BMatr::GetRot() const
{
	return BMatr(c[0][0], c[0][1], c[0][2], c[0][3],
		c[1][0], c[1][1], c[1][2], c[1][3],
		c[2][0], c[2][1], c[2][2], c[2][3],
		0., 0., 0., 1.);
}

BMatr BMatr::GetTran() const
{
	return BMatr(1., 0., 0., 0.,
				0., 1., 0., 0.,
				0., 0., 1., 0.,
				c[3][0], c[3][1], c[3][2], c[3][3]);
}

const BMatr& BMatr::MakeNonDeg()
{
	// assuming this matrix should be ortogonal basis
	BPoint p2 = GetRow(2);
	if (GetRowD2(0) < MIND * MIND)
		SetRow(0, GetRow(1) % GetRow(2));
	if (GetRowD2(1) < MIND * MIND)
		SetRow(1, GetRow(2) % GetRow(0));
	if (GetRowD2(2) < MIND * MIND)
		SetRow(2, GetRow(0) % GetRow(1));
	return *this;
}

bool BMatr::operator ==(const BMatr &M) const
{
	return 0 == memcmp(this, &M, sizeof(BMatr));
}

bool BMatr::operator !=(const BMatr &M) const
{
	return !operator == (M);
}

bool BMatr::IsApprSame(const BMatr& m)
{
	return fabs(c[0][0] - m.c[0][0]) < MIND 
		&& fabs(c[0][1] - m.c[0][1]) < MIND
		&& fabs(c[0][2] - m.c[0][2]) < MIND
		&& fabs(c[0][3] - m.c[0][3]) < MIND

		&& fabs(c[1][0] - m.c[1][0]) < MIND
		&& fabs(c[1][1] - m.c[1][1]) < MIND
		&& fabs(c[1][2] - m.c[1][2]) < MIND
		&& fabs(c[1][3] - m.c[1][3]) < MIND

		&& fabs(c[2][0] - m.c[2][0]) < MIND
		&& fabs(c[2][1] - m.c[2][1]) < MIND
		&& fabs(c[2][2] - m.c[2][2]) < MIND
		&& fabs(c[2][3] - m.c[2][3]) < MIND

		&& fabs(c[3][0] - m.c[3][0]) < MIND
		&& fabs(c[3][1] - m.c[3][1]) < MIND
		&& fabs(c[3][2] - m.c[3][2]) < MIND
		&& fabs(c[3][3] - m.c[3][3]) < MIND
		;
}
