#include "stdafx.h"
#include "math.h"
#include "BMatr.h"
#include "NCadrGeom.h"
#include "nturngroovcycle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

NTurnGroovCycle::NTurnGroovCycle(BGeomArray &Cont, double XStep, double ZStep, bool Xfeed, double AddTime)
{
	pCont = &Cont;

	if(pCont->GetSize() <= 0)
		return;

	Step = XStep;
	OrtStep = ZStep;
	orient = Xfeed;
	StepAddTime = AddTime;

	OrientCont();

	if(MakePath() != 0)
	{
		bOK = false;
		Cont.RemoveAll();
	}

	RestoreCont();
}

NTurnGroovCycle::~NTurnGroovCycle(void)
{
}

int NTurnGroovCycle::MakePath(void)
{
	BPoint V = pCont->GetAt(0)->GetStartDir();
	int NSteps = (Step == 0.) ? 0 : int(fabs((fabs(V.GetX()) - 1.e-4) / Step) + 1); 
	if(V.GetX() < 0)
		Step *= -1.;
	BGeomArray OneStep, Path;
	BPoint StartPoint = pCont->GetAt(0)->GetB();
	for(int i = 0; i < NSteps; ++i)
	{
		MakeOneStep(OneStep, StartPoint, Step);
		Path.Append(OneStep);
		StartPoint = OneStep[OneStep.GetSize() - 1]->GetE();
		OneStep.SetSize(0);
	}
	// Last move must be in the given position
	StartPoint.Set(pCont->GetAt(0)->GetE().GetX(), StartPoint.GetY(), StartPoint.GetZ(), StartPoint.GetH());
	if(Path.GetSize() > 0)
		Path.GetAt(Path.GetSize() - 1)->SetE(StartPoint);
	MakeOneStep(OneStep, StartPoint, Step);
	Path.Append(OneStep);
	OneStep.SetSize(0);

	pCont->RemoveAll();
	pCont->Append(Path);
	Path.SetSize(0);
	return 0;
}

bool NTurnGroovCycle::MakeOneStep(BGeomArray & OneStep, const BPoint & StartPoint, double StepX)
{
	OneStep.RemoveAll();
	
	double dZ = pCont->GetAt(0)->GetStartDir().GetZ();

	NCadrGeom *pCadr;
	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(line);
	pCadr->SetB(StartPoint);
	pCadr->SetE(StartPoint + BPoint(0., 0., dZ, 0.));
	OneStep.Add(pCadr);
	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->Set(undef, StartPoint + BPoint(0., 0., dZ, 0.), StartPoint + BPoint(0., 0., dZ, 0.));
	pCadr->SetWorkTimeS(int((OrtStep == 0 ? 1. : dZ / OrtStep)) * StepAddTime);
	OneStep.Add(pCadr);
	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(fast);
	pCadr->SetB(StartPoint + BPoint(0., 0., dZ, 0.));
	pCadr->SetE(StartPoint + BPoint(0., 0., 0., 0.));
	OneStep.Add(pCadr);
	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(fast);
	pCadr->SetB(StartPoint + BPoint(0., 0., 0., 0.));
	pCadr->SetE(StartPoint + BPoint(StepX + 0., 0., 0., 0.));
	OneStep.Add(pCadr);

	return true;
}


void NTurnGroovCycle::OrientCont(void)
{
	if(!orient)
		return;
	// Change orientation
	BMatr Matr;
	Matr.RotY(pCont->GetAt(0)->GetB(), 90.);
	for(int i = 0; i < pCont->GetSize(); ++i)
		pCont->GetAt(i)->Tr(Matr);
}
void NTurnGroovCycle::RestoreCont(void)
{
	if(!orient)
		return;

	// Change orientation
	BMatr Matr;
	Matr.RotY(pCont->GetAt(0)->GetB(), -90.);
	for(int i = 0; i < pCont->GetSize(); ++i)
		pCont->GetAt(i)->Tr(Matr);
}
