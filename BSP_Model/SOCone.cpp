#include "stdafx.h"
#include "ResLocal.h"
#include "ConstDef.h"
#include "math.h"
#include "Bmatr.h"
#include "SOPlane.h"
#include "SOCyl.h"
#include "SOCone.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

SOCone::SOCone(const BPoint & P, const BPoint & V, double A) : Center(P), Vector(V)
{
	Angle = A;
}

SOCone::~SOCone(void)
{
}

const BPoint SOCone::GetNormal(const BPoint & P) const
{
// Angle > 0 - external normal; < 0 - internal
	BPoint N = P - Center;
	if(N.D2() < MIND*MIND)
		return BPoint(0.,0.,0.,-1.);
	N = (Vector % N) % N;
	return N * ((Angle > 0 ? 1. : -1.) / sqrt(N.D2()));
}



const BPoint SOCone::GetPoint(const BPoint & P) const
{
	return P;
}
SOCone::SOCone(const BPoint & P, const BPoint & V,  const BPoint & P0, const BPoint & P1)
: Center(0., 0., 0., 1.), Vector(0., 0., 1., 0)
{
	// P - arbitrary axis point
	// V - axis direction vector
	// P0 P1 точки на образующей
	Angle = fabs(V.Angle(P1 - P0));
	if(Angle > 90.)
		Angle = 180. - Angle;
	double d0 = (P - P0 + V * ((P0 - P) * V)).D2();
	double d1 = (P - P1 + V * ((P1 - P) * V)).D2();
	double sign = -1.;
	if( (V * (P1 - P0) > 0) == (d1 > d0))
		sign = 1.;
	BPoint P0p = P + V * ((P0 - P) * V);
	Vector = V * sign;
	Center = P0p - Vector * (sqrt(d0) / tan(Angle*PI/180.));
	// Process the case when P0P1 line intersects cone axis
	if((P1 - Center) * Vector < 0.)
	{
		if(Vector.GetZ() > 0.)// We prefer down cone because of milling specific
			Vector = Vector * -1.;
	}
}

void SOCone::InvertAngleSign(void)
{
	Angle *= -1.;
}
CString SOCone::GetTypeName() const
{
	CString tmp;
	tmp.LoadString(IDS_TYPE_CONE);
	return tmp;
}

double SOCone::GetAngle(void) const
{
	return Angle;
}

void SOCone::Save(CFile * f, const BMatr &M) const
{
	SSurfType i = GetType();
	f->Write((void *)&i,sizeof(i));
	BPoint Tmp;
	Tmp = Center * M;
	f->Write((void *)&Tmp, sizeof(Tmp));
	f->Write((void *)&Angle,sizeof(Angle));
	Tmp = Vector * M;
	f->Write((void *)&Tmp, sizeof(Tmp));
	return;
}

void SOCone::Load(CFile * f)
{
	f->Read((void *)&Center,sizeof(Center));
	f->Read((void *)&Angle,sizeof(Angle));
	f->Read((void *)&Vector,sizeof(Vector));
	return;
}

bool SOCone::RefiningPoint(BPoint & RefPoint, const BPoint & DirectVector) const
{
	if(fabs(Angle - 90.) < MINAD)
	{// The cone is plane
		double z = Vector * DirectVector;
		if(fabs(z) < MINAR)
			return false;
		BPoint p0 = Center - RefPoint;
		double t = (p0 * Vector) * (1. / z);
		RefPoint = RefPoint + DirectVector * t;
		return true;
	}
	// X^2 + Y^2 = (Z * Tg(a))^2
	// Pt = P + t * V
	BMatr Matrix, Matrix0, Matrix1;
	Matrix0.Trans(Center, BPoint(0., 0., 0., 1.));
	Matrix1.e0cong(Vector, BPoint(0., 0., 1., 0.));
	Matrix = Matrix0 * Matrix1;
	BPoint tp = RefPoint * Matrix;
	BPoint tv = DirectVector * Matrix;
	BPoint tpxy = tp;
	tpxy.ProjXY();
	tpxy = tpxy - BPoint(0., 0., 0., 1.); // преобразуем точку в вектор
	BPoint tvxy = tv;
	tvxy.ProjXY();

	double tg2 = tan(Angle * PI / 180.);
	tg2 *= tg2;
	double A = tvxy.D2() - tv.GetZ() * tv.GetZ() * tg2;
	if(fabs(A) < MIND * MIND)
		return false;
	double B = 2. * (tpxy * tvxy - tp.GetZ() * tv.GetZ() * tg2);
	double C = tpxy.D2() - tp.GetZ() * tp.GetZ() * tg2;
	double Discr = B * B - 4. * A * C;
	if(Discr <= 0.)
		return false;
	double t1 = (- B - sqrt(Discr)) / (2. * A);
	double t2 = (- B + sqrt(Discr)) / (2. * A);
	double t = fabs(t1) > fabs(t2) ? t2 : t1;
	RefPoint = RefPoint + DirectVector * t;
	return true;
}
void SOCone::Transform(const BMatr & T)
{
	Center = Center * T;
	Vector = Vector * T;
}

SOrigSurf * SOCone::GenCone(const BPoint & P, const BPoint & V,  const BPoint & P0, const BPoint & P1)
{
	// P - arbitrary axis point
	// V - axis direction vector
	// P0 P1 точки на образующей
	if(fabs(P1.GetZ() - P0.GetZ()) < MIND)
	{
		SOPlane *pPlane = new SOPlane(V, P0);
		return pPlane;
	}

	double R = sqrt(P0.LineD2(P, P + V));
	if(fabs(R - sqrt(P1.LineD2(P, P + V))) < MIND)
	{
		SOCyl *pCyl = new SOCyl(P, V, R);
		return pCyl;
	}

	SOCone *pCone = new SOCone(P, V, P0, P1);
	return pCone;
}

void SOCone::Invert()
{
	Angle *= -1.;
}

