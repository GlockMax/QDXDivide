#include "stdafx.h"
#include "ResLocal.h"
#include "math.h"
#include "soecyl.h"

SOECyl::~SOECyl(void)
{
}
SOECyl::SOECyl(const BPoint & P, const BPoint & V, const BPoint & N, double R )
:SOCyl(P, V, R), Normal(N * V < 0. ? N * -1 : N)
{
}
void SOECyl::Save(CFile * f, const BMatr &M) const
{
	SSurfType i = GetType();
	f->Write((void *)&i, sizeof(i));
	BPoint Tmp;
	Tmp = Center * M;
	f->Write((void *)&Tmp, sizeof(Tmp));
	f->Write((void *)&Radius, sizeof(Radius));
	Tmp = Vector * M;
	f->Write((void *)&Tmp, sizeof(Tmp));
	Tmp = Normal * M;
	f->Write((void *)&Tmp,sizeof(Tmp));
}

void SOECyl::Load(CFile * f)
{
	SOCyl::Load(f);
	f->Read((void *)&Normal,sizeof(Normal));
}
const BPoint SOECyl::GetNormal(const BPoint & P) const
{
// Radius > 0 - external normal; < 0 - internal
	double h = (P - Center) * Normal;
	BPoint Ch = Center + Vector * ( h / (Normal * Vector));// The center of the circle containing the point P
	BPoint N = ((P - Ch) % Normal) % Vector;
	return N * ((Radius > 0 ? -1. : 1.)/sqrt(N.D2()));
}
const BPoint SOECyl::GetPoint(const BPoint & P) const
{// This version doesn't find real nearest point but the point on the surface which lies near the point P
	double h = (P - Center) * Normal;
	BPoint Ch = Center + Vector * ( h / (Normal * Vector));// The center of the circle containing the point P
	BPoint N = P - Ch;
	return P + N * (1. - Radius/sqrt(N.D2()));
}
CString SOECyl::GetTypeName() const
{
	CString tmp;
	tmp.LoadString(IDS_TYPE_ECYL);
	return tmp;
}
bool SOECyl::RefiningPoint(BPoint & RefPoint, const BPoint & DirectVector) const
{
	double h = (RefPoint - Center) * Normal;
	BPoint Ch = Center + Vector * ( h / (Normal * Vector));// The center of the circle on the plane containing the point RefPoint
	BPoint Refh(RefPoint - Ch);
	double RefhDir = Refh * DirectVector;
	double z = DirectVector.D2();
	double Discr = RefhDir * RefhDir - (Refh.D2() + Radius * Radius) * z;
	if(Discr < 0.)
		return false;
    Discr = sqrt(fabs(Discr));
	double t1 = -RefhDir + Discr;
	double t2 = -RefhDir - Discr;
	double t = (fabs(t1) < fabs(t2)) ? t1 : t2; 
	RefPoint += DirectVector * (t / z);
	return true;
}
void SOECyl::Invert()
{
	Radius *= -1;
}
void SOECyl::Transform(const BMatr & T)
{
	SOCyl::Transform(T);
	Normal = Normal * T;
}
