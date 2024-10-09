#include "stdafx.h"
#include "ConstDef.h"
#include "SOTorus.h"
#include "SOCyl.h"
#include "SOECyl.h"
#include "SOPlane.h"
#include "SOSphere.h"
#include "SOScrewO.h"
#include "SOCone.h"
#include "SOrigSurf.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

SOrigSurf::SOrigSurf(void)
{
}

SOrigSurf::~SOrigSurf(void)
{
}

void SOrigSurf::SaveDummy(CFile* f)
{
	SSurfType i = DUMMY;
	f->Write((void*)&i, sizeof(i));
}

SOrigSurf * SOrigSurf::StLoad(CFile * f)
{
	SOrigSurf *p = nullptr;
	SSurfType type;
	f->Read((void *)&type,sizeof(type));

	switch(type)
	{
	case CYLINDER:
		p=new SOCyl;
		break;
	case ECYLINDER:
		p=new SOECyl;
		break;
	case PLANE:
		p=new SOPlane;
		break;
	case SPHER:
		p=new SOSphere;
		break;
	case TORUS:
		p = new SOTorus;
		break;
	case CON:
		p = new SOCone;
		break;
	case SCREW:
		p = new SOScrewO;
		break;
	default:
		return nullptr;
	}
	p->Load(f);
	return p;
}

const BPoint SOrigSurf::GetPoint(const BPoint & P) const
{
	return P;
}

SOrigSurf* SOrigSurf::CreateECyl(const BPoint& P, const BPoint& V, const BPoint& N, double R)
{
	double Angle = V.Angle0_180(N);
	if (fabs(Angle) < MINAD || fabs(Angle - 180.) < MINAD)
		return new SOCyl(P, V, R);
	if (fabs(Angle - 90.) < MINAD)
		return new SOPlane(N, P);
	return new SOECyl(P, V, N, R); 
}
