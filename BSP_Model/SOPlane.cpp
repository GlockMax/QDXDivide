#include "stdafx.h"
#include "ResLocal.h"
#include "math.h"
#include "ConstDef.h"
#include "soplane.h"

SOPlane::SOPlane(const BPoint & N, const BPoint &P) : Normal(N)
{
	Normal = N;
	Dist = P * N;
}

SOPlane::~SOPlane(void)
{
}

CString SOPlane::GetTypeName() const
{
	CString tmp;
	tmp.LoadString(IDS_TYPE_PLANE);
	return tmp;
}

void SOPlane::Save(CFile * f, const BMatr &M) const
{
	SSurfType i = GetType();
	f->Write((void *)&i,sizeof(i));
	double TDist = Dist + M.GetRow(3) * Normal;
	f->Write((void *)&TDist,sizeof(TDist));
	BPoint Tmp;
	Tmp = Normal * M;
	f->Write((void *)&Tmp, sizeof(Tmp));
	return;

}

void SOPlane::Load(CFile * f)
{
	f->Read((void *)&Dist,sizeof(Dist));
	f->Read((void *)&Normal,sizeof(Normal));
	return;
}

bool SOPlane::RefiningPoint(BPoint & RefPoint, const BPoint & DirectVector) const
{
	double sp = DirectVector * Normal;
	if(fabs(sp) < MINAR)
		return false;
	double t = Dist * sqrt(Normal.D2()) - (RefPoint - BPoint(0., 0., 0., 1.)) * Normal;
	t /= sp;
	RefPoint = RefPoint + DirectVector * t;
	return true;
}
void SOPlane::Invert()
{
	Normal *= -1.;
	Dist *= -1.;
}
void SOPlane::Transform(const BMatr & T)
{
	Normal = Normal * T;
	Dist += T.GetRow(3) * Normal;
}

