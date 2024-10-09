#include "stdafx.h"
#include "ConstDef.h"
#include "ResLocal.h"
#include "math.h"
#include "sosphere.h"

SOSphere::SOSphere(double R, const BPoint & C) : Center(C), Radius(R)
{
	if(fabs(Radius) > MIND)
		ZRadius = 1. / Radius;
	else
		ZRadius = 1.e12;
}

SOSphere::~SOSphere(void)
{
}

const BPoint SOSphere::GetNormal(const BPoint & P) const
{
// Radius > 0 - external normal; < 0 - internal
	BPoint N = P - Center;
	return N * ((Radius > 0 ? 1. : -1.)/sqrt(N.D2()));
}

void SOSphere::GetNormalFast(float *BUF) const
{
	BUF[0] = float((Center.GetX() - BUF[3]) * ZRadius);
	BUF[1] = float((Center.GetY() - BUF[4]) * ZRadius);
	BUF[2] = float((Center.GetZ() - BUF[5]) * ZRadius);
}

const BPoint SOSphere::GetPoint(const BPoint & P) const
{
	BPoint N = P - Center;
	return P + N * (1. - fabs(Radius)/sqrt(N.D2()));
}
CString SOSphere::GetTypeName() const
{
	CString tmp;
	tmp.LoadString(IDS_TYPE_SPHERE);
	return tmp;
}
void SOSphere::Save(CFile * f, const BMatr &M) const
{
	SSurfType i = GetType();
	f->Write((void *)&i,sizeof(i));
	BPoint Tmp;
	Tmp = Center * M;
	f->Write((void *)&Tmp, sizeof(Tmp));
	f->Write((void *)&Radius,sizeof(Radius));
	return;
}

void SOSphere::Load(CFile * f)
{
	f->Read((void *)&Center,sizeof(Center));
	f->Read((void *)&Radius,sizeof(Radius));
	if (fabs(Radius) > MIND)
		ZRadius = 1. / Radius;
	else
		ZRadius = 1.e12;
	return;
}

bool SOSphere::RefiningPoint(BPoint & RefPoint, const BPoint & DirectVector) const
{
	double t1, t2, Discr;
	BPoint P1 = RefPoint - Center, P0 = RefPoint;
	Discr = ((2 * (DirectVector * P1)) * (2 * (DirectVector * P1)) - 
			4 * (DirectVector.D2()) * (P1.D2() - Radius*Radius));
	if(Discr<0)
		return false;
	t1 = (-(2 * (DirectVector * P1)) - sqrt(Discr)) / (2 * (DirectVector.D2()));
	t2 = (-(2 * (DirectVector * P1)) + sqrt(Discr)) / (2 * (DirectVector.D2()));
	P1 = P0 + DirectVector * t1;
	P0 = P0 + DirectVector * t2;
	if((P0-RefPoint).D2() < (P1-RefPoint).D2())
		RefPoint = P0;
	else
		RefPoint = P1;
	return true;
}
void SOSphere::Invert()
{
	Radius *= -1.;
}
void SOSphere::Transform(const BMatr & T)
{
	Center = Center * T;
}

//bool SOSphere::OutlinePN(const BPoint &P0, const BPoint &P1, BPoint &P, BPoint &N) const
//{
//	// It is assumed that P0 belongs to the projection on th XY plane
//	// If P1 belongs to it too then return false
//	// otherwise return true and calculate P and N
//	// P - intersection of the vertical plane containing P0 and P1 and outline
//	// N - normal in P
//	double XC = Center.GetX();
//	double YC = Center.GetY();
//	double X1 = P1.GetX();
//	double Y1 = P1.GetY();
//	double dX1 = X1 - XC;
//	double dY1 = Y1 - YC;
//	double Q12 = dX1 * dX1 + dY1 * dY1;
//	double R2 = Radius * Radius;
//	if(R2 > Q12)
//		return false;
//	// Find P
//	double X0 = P0.GetX();
//	double Y0 = P0.GetY();
//	double dX0 = X0 - XC;
//	double dY0 = Y0 - YC;
//	double Q02 = dX0 * dX0 + dY0 * dY0;
//	double Q0Q1 = dX0 * dX1 + dY0 * dY1;
//	double A = Q02 - 2. * Q0Q1 + Q12;
//	double B = 2. * (Q0Q1 - Q02);
//	double C = Q02 - R2;
//	double D = B * B - 4. * A * C;
//	if(D <= 0. || A == 0.)// This condition can not be satisfied 
//		return false;
//	D = sqrt(D);
//	double Z = 0.5 / A; 
//	double t = (- B + D) * Z;
//	if(t < 0. || t > 1.)
//		t = (- B - D) * Z;
//	if(t < 0. || t > 1.)// This condition can not be satisfied 
//		return false;
//	P.Set(X0 * (1. - t) + X1 * t, Y0 * (1. - t) + Y1 * t, Center.GetZ(), 1.);
//	N = (Center - P) * (1. / fabs(Radius));
//	return true;
//}
