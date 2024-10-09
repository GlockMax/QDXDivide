#include "stdafx.h"
#include "ResLocal.h"
#include "ConstDef.h"
#include "math.h"
#include "SOScrewO.h"

SOScrewO::SOScrewO(const BPoint & C, const BPoint & V, double S, const BPoint & P0, const BPoint & P1, double Sign) 
: SOScrew(C, V, S), Np(0., 0., 0., 1.), Vd(0., 0., 1., 0)
{
	Create(P0, P1, Sign);
}

SOScrewO::~SOScrewO(void)
{
}

const BPoint SOScrewO::GetNormal(const BPoint & P) const
{
	double t, tau;
	if(CalcTTau(P, t, tau) < 0)
		return BPoint(0., 0., 0., -1.);
	double alf = AngleSign * tau * 360. / ScStep;
	BPoint N = Vd * BMatr().rotg(alf, Center, Vector);
	BPoint V1 = P - Center;
	V1 = V1 - Vector * (V1 * Vector);
	V1 = (V1 * BMatr().rotg(AngleSign * 90, Center, Vector)) * (2 * PI) + Vector * ScStep;
	N = N % V1;
	return N * (NormalSign / sqrt(N.D2()));
}

const BPoint SOScrewO::GetPoint(const BPoint & P) const
{
	double t, tau;
	CalcTTau(P, t, tau);
	return GetPoint(t, tau);
}
CString SOScrewO::GetTypeName() const
{
	CString tmp;
	tmp.LoadString(IDS_TYPE_SCREW);
	return tmp;
}

void SOScrewO::Save(CFile * f, const BMatr &M) const
{
	SSurfType i = GetType();
	f->Write((void *)&i, sizeof(i));
	BPoint Tmp;
	Tmp = Center * M;
	f->Write((void *)&Tmp, sizeof(Tmp));
	Tmp = Vector * M;
	f->Write((void *)&Tmp, sizeof(Tmp));
	f->Write((void *)&ScStep, sizeof(ScStep));
	Tmp = Np * M;
	f->Write((void *)&Tmp, sizeof(Tmp));
	Tmp = Vd * M;
	f->Write((void *)&Tmp, sizeof(Tmp));
	f->Write((void *)&Tb,sizeof(Tb));
	f->Write((void *)&Te,sizeof(Te));
	return;
}

void SOScrewO::Load(CFile * f)
{
	SOScrew::Load(f);
	f->Read((void *)&Np,sizeof(Np));
	f->Read((void *)&Vd,sizeof(Vd));
	f->Read((void *)&Tb,sizeof(Tb));
	f->Read((void *)&Te,sizeof(Te));
	return;
}

bool SOScrewO::RefiningPoint(BPoint & RefPoint, const BPoint & DirectVector) const
{
	DirectVector;
	RefPoint;
	return false;
}

void SOScrewO::Transform(const BMatr & T)
{
	SOScrew::Transform(T);
	Np = Np * T;
	Vd = Vd * T;
}

int SOScrewO::CalcTTau(const BPoint &P, double &t, double &tau) const
{
	double bt;
	int NumT = CalcT(P, bt);
	if(NumT == 0)
		return -1;

	if(NumT == 1)
	{
		tau = GetTau(P, bt);
		t = bt;
		return 0;
	}
	
	double tau0 = GetTau(P, bt);
	double tau1 = GetTau(P, -bt);
	BPoint Q0 = GetPoint(bt, tau0);
	BPoint Q1 = GetPoint(-bt, tau1);
	if((Q0 - P).D2() < (Q1 - P).D2())
	{
		t = bt;
		tau = tau0;
	}
	else
	{
		t = -bt;
		tau = tau1;
	}
	return 0;
}
double SOScrewO::GetTau(const BPoint &P, double t) const
{
	return Vector * (P - Np - Vd * t);
}
int SOScrewO::CalcT(const BPoint &P, double &t) const
{
	// returns the number of the points found
	double R2 = (Np - Center).D2();
	double Rp2 = GetRp2(P);
	double d2 = Rp2 - R2;
	if(d2 < - DMIN * DMIN)
		return 0;
	if(fabs(d2) <= DMIN * DMIN)
	{
		t = 0;
		return 1;
	}
	t = sqrt(d2);
	d2 = (Vd - Vector * (Vd * Vector)).D2();
	if(d2 < MIND * MIND)
		return 0;
	t = t / sqrt(d2);

	int Num = 0;
	if(t >= Tb && t <= Te)
		++Num;
	if(-t >= Tb && -t <= Te)
	{
		++Num;
		t = -t;
	}
	return Num;
}
void SOScrewO::Create(const BPoint &Pb, const BPoint &Pe, double Sign)
{
	NormalSign = Sign;
	Np = Center;
	Vd = Pe - Pb;
	Tb = 0.;
	Te = 0.;

	double L2 = Vd.D2();
	if(L2 < MIND * MIND)
		return;
	Vd = Vd * (1. / sqrt(L2));
	BPoint Vdp = Vd - Vector * (Vd * Vector);
	L2 = Vdp.D2();
	if(L2 < MIND * MIND)
		return;
	
	BPoint Pbp = Pb - Vector * ((Pb - Center) * Vector);
	Np = Pb + Vd * (((Center - Pbp) * Vdp) / L2);
	Center = Center + Vector * ((Np - Center) * Vector); // Center is nearest to Np axis point now
	Tb = (Pb - Np) * Vd;
	Te = (Pe - Np) * Vd;
	if(Tb > Te)
	{
		double b = Tb;
		Tb = Te;
		Te = b;
	}
}
bool SOScrewO::IsMono(void) const
{
	return (Tb * Te) >= 0.;
}
const BPoint SOScrewO::GetPoint(double t, double tau) const
{
	double alf = AngleSign * tau * 360. / ScStep;
	BPoint P = Vector * tau + (Np + Vd * t) * BMatr().rotg(alf, Center, Vector);
	return P;
}
