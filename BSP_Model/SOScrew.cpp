#include "stdafx.h"
#include "ConstDef.h"
#include "math.h"
#include "SOPlane.h"
#include "SOCyl.h"
#include "SOScrewO.h"
#include "SOScrew.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

SOrigSurf * SOScrew::GenScrew(const BPoint & C, const BPoint & V, double S, const NLine &Line, bool Arc)
{
	if(!Arc)
	{// Cyl or ScrewO 
		BPoint V1(Line.p[3] - Line.p[0]);
		if(fabs(V1 * V) > sqrt(V1.D2()) * cos(MINAR))
		{// Cylinder
			V1 = Line.p[0] - C;
			V1 = V1 - V * (V1 * V);
			return new SOCyl(C, V, sqrt(V1.D2()));
		}
		// ScrewO
		if(S < MIND)
		{
			if(fabs(V1 * V) < sqrt(V1.D2()) * MINAR)
				return new SOPlane(V, Line.p[0]);
			else
				return NULL;
		}
		return new SOScrewO(C, V, S, Line.p[0], Line.p[3], 1.);
	}
	// ScrewA
	return NULL;
}
SOScrew::SOScrew(const BPoint & C, const BPoint & V, double S) : Center(C), Vector(V)
{
	AngleSign = (S > 0.) ? 1 : -1;
	ScStep = fabs(S);
}

SOScrew::~SOScrew(void)
{
}

const BPoint SOScrew::GetNormal(const BPoint & P) const
{
	// Rsmall > 0 - external normal; < 0 - internal

	//BPoint SCCenter = P - Center;
	//SCCenter = SCCenter - Normal * (SCCenter * Normal);// Project SCCenter to torus main plane
	//double d2 = SCCenter.D2();
	//if(d2 < DMIN * DMIN)
	//	return BPoint(0,0,-1,0);
	//SCCenter = SCCenter*((1./sqrt(d2))*RLarge);
	//BPoint N = P - SCCenter - Center;
	//N = N*((RSmall > 0 ? 1. : -1.)/sqrt(N.D2()));
	return P;
}

const BPoint SOScrew::GetPoint(const BPoint & P) const
{
	//BPoint SCCenter = P - Center;
	//SCCenter = SCCenter - Normal * (SCCenter * Normal);// Project SCCenter to torus main plane
	//double d2 = SCCenter.D2();
	//if(d2 < DMIN * DMIN)
	//	return P;
	//SCCenter = SCCenter*((1./sqrt(d2))*RLarge);
	//BPoint N = P - SCCenter - Center;
	//return P + N * (1. - RSmall/sqrt(N.D2()));
	return P;
}

void SOScrew::Save(CFile * f, const BMatr &M) const
{
	SSurfType i = GetType();
	f->Write((void *)&i, sizeof(i));
	BPoint Tmp;
	Tmp = Center * M;
	f->Write((void *)&Tmp, sizeof(Tmp));
	Tmp = Vector * M;
	f->Write((void *)&Tmp, sizeof(Tmp));
	f->Write((void *)&ScStep, sizeof(ScStep));
	return;
}

void SOScrew::Load(CFile * f)
{
	f->Read((void *)&Center, sizeof(Center));
	f->Read((void *)&Vector, sizeof(Vector));
	f->Read((void *)&ScStep, sizeof(ScStep));
	return;
}

bool SOScrew::RefiningPoint(BPoint & RefPoint, const BPoint & DirectVector) const
{
	DirectVector;
	RefPoint;
	return false;
}

void SOScrew::Invert()
{
}

void SOScrew::Transform(const BMatr & T)
{
	Center = Center * T;
	Vector = Vector * T;
}

double SOScrew::GetRp2(const BPoint &P) const
{
	// Return the distance from P to rotation axis
	return 
		(Vector * ((P - Center) * Vector) + Center - P).D2();
}

