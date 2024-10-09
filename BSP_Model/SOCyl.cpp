#include "stdafx.h"
#include "ConstDef.h"
#include "ResLocal.h"
#include "math.h"
#include "socyl.h"

SOCyl::SOCyl(const BPoint & P, const BPoint & V, double R) : Center(P), Vector(V)
{
	Radius = R;
	if(fabs(Radius) > MIND)
		ZRadius = 1. / Radius;
	else
		ZRadius = 1.e12;
}

SOCyl::~SOCyl(void)
{
}
const BPoint SOCyl::GetNormal(const BPoint & P) const
{
// Radius > 0 - external normal; < 0 - internal
	BPoint N = P - Center;
	N -= Vector * (N * Vector);
	double R_P = sqrt(N.D2());
	if(fabs(R_P - fabs(Radius)) > 0.5 * fabs(Radius))
		return BPoint(0., 0., 0., -1.);
	return N * ((Radius > 0 ? 1. : -1.)/R_P);
}

void SOCyl::GetNormalFast(float *BUF) const
{
// Radius > 0 - external normal; < 0 - internal
	double BUF0 = (BUF[3] - Center.GetX()) * ZRadius;
	double BUF1 = (BUF[4] - Center.GetY()) * ZRadius;
	double BUF2 = (BUF[5] - Center.GetZ()) * ZRadius;
	double NVector = BUF0 * Vector.GetX() + BUF1 * Vector.GetY() + BUF2 * Vector.GetZ();
	/* ISSUE 40
	BUF[0] = -float(Vector.GetX() * NVector - BUF0);
	BUF[1] = -float(Vector.GetY() * NVector - BUF1);
	BUF[2] = -float(Vector.GetZ() * NVector - BUF2);*/
	BUF[0] = float(Vector.GetX() * NVector - BUF0);
	BUF[1] = float(Vector.GetY() * NVector - BUF1);
	BUF[2] = float(Vector.GetZ() * NVector - BUF2);
}

const BPoint SOCyl::GetPoint(const BPoint & P) const
{
	BPoint N = P - Center;
	N = N - Vector * (N * Vector);
	return P + N * (1. - fabs(Radius)/sqrt(N.D2()));
}

void SOCyl::Save(CFile * f, const BMatr &M) const
{
	SSurfType i = GetType();
	f->Write((void *)&i,sizeof(i));
	BPoint Tmp;
	Tmp = Center * M;
	f->Write((void *)&Tmp, sizeof(Tmp));
	f->Write((void *)&Radius,sizeof(Radius));
	Tmp = Vector * M;
	f->Write((void *)&Tmp, sizeof(Tmp));
	return;
}

void SOCyl::Load(CFile * f)
{
	f->Read((void *)&Center,sizeof(Center));
	f->Read((void *)&Radius,sizeof(Radius));
	f->Read((void *)&Vector,sizeof(Vector));
	if (fabs(Radius) > MIND)
		ZRadius = 1. / Radius;
	else
		ZRadius = 1.e12;

	return;
}
CString SOCyl::GetTypeName() const
{
	CString tmp;
	tmp.LoadString(IDS_TYPE_CYL);
	return tmp;
}

bool SOCyl::RefiningPoint(BPoint & RefPoint, const BPoint & DirectVector) const
{
	BMatr Matrix, Matrix0, Matrix1;
	Matrix0.Trans(Center, BPoint(0., 0., 0., 1.));
	Matrix1.e0cong(Vector, BPoint(0., 0., 1., 0.));
	Matrix = Matrix0 * Matrix1;
	BPoint tp = RefPoint * Matrix;
	BPoint tv = DirectVector * Matrix;
	tp.ProjXY();
	tp = tp - BPoint(0., 0., 0., 1.); // преобразуем точку в вектор
	tv.ProjXY();
	double tmp = tp * tv;
	double z = tv.D2();
	double Discr = tmp * tmp - z*(tp.D2() - Radius*Radius);
	if (Discr < 0. || z == 0.)
		return false;
    Discr = sqrt(Discr);
	double t1 = -tmp + Discr;
	double t2 = -tmp - Discr;
	//BPoint p1 = RefPoint + DirectVector * t1 * z;
	//BPoint p2 = RefPoint + DirectVector * t2 * z;
	//if (((RefPoint - p1).D2()) >= ((RefPoint - p2).D2()))
	//	RefPoint = p2;
	//else
	//	RefPoint = p1;
	double t = (fabs(t1) < fabs(t2)) ? t1 : t2; 
	RefPoint += DirectVector * (t / z);
	return true;
}
void SOCyl::Invert()
{
	Radius *= -1.;
}
void SOCyl::Transform(const BMatr & T)
{
	Center = Center * T;
	Vector = Vector * T;
}

//bool SOCyl::OutlinePN(const BPoint &P0, const BPoint &P1, BPoint &P, BPoint &N) const
//{
//	// It is assumed that P0 belongs to the projection on th XY plane
//	// If P1 belongs to it too then return false
//	// otherwise return true and calculate P and N
//	// P - intersection of the vertical plane containing P0 and P1 and outline
//	// N - normal in P
//	double XC = Center.GetX();
//	double YC = Center.GetY();
//	double XV = Vector.GetX();
//	double YV = Vector.GetY();
//	double X1 = P1.GetX();
//	double Y1 = P1.GetY();
//	double dX1 = X1 - XC;
//	double dY1 = Y1 - YC;
//	double DVz2 = 1. / (XV * XV + YV * YV);
//	double B1 = (dX1 * XV + dY1 * YV);
//	double D12 = dX1 * dX1 + dY1 * dY1 - B1 * B1 * DVz2; 
//	double R2 = Radius * Radius;
//	if(R2 > D12)
//		return false;
//	// Find P
//	double X0 = P0.GetX();
//	double Y0 = P0.GetY();
//	double dX0 = X0 - XC;
//	double dY0 = Y0 - YC;
//	double B0 = (dX0 * XV + dY0 * YV);
//	double D02 = dX0 * dX0 + dY0 * dY0 - B0 * B0 * DVz2; 
//	double D1 = sqrt(D12);
//	double D0 = sqrt(D02);
//	if(fabs(D1 - D0) < MIND)
//		return false;
//	double t = (Radius - D0) / (D1 - D0);
//	if(t < 0. || t > 1.)// This condition can not be satisfied 
//		return false;
//	double Z = Vector.GetZ() * (B0 * (1. - t) + B1 * t) * DVz2 + Center.GetZ();
//	P.Set(X0 * (1. - t) + X1 * t, Y0 * (1. - t) + Y1 * t, Z, 1.);
//	N.Set(YV, -XV, 0., 0.);
//	N = N * (1. / N.D2());
//	if(N.GetX() * (P.GetX() - Center.GetX()) + N.GetY() * (P.GetY() - Center.GetY()) > 0.)
//		N = N * (-1.);
//	return true;
//}
