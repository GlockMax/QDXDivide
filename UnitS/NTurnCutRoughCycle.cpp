#include "stdafx.h"
#include "math.h"
#include "BMatr.h"
#include "NCadrGeom.h"
#include "NTurnCutRoughCycle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

NTurnCutRoughCycle::NTurnCutRoughCycle(BGeomArray &Cont, double XStep, double Rz, bool Xfeed)
{
	pCont = &Cont;

	if(pCont->GetSize() <= 0)
		return;

	Step = XStep;
	R = Rz;
	orient = Xfeed;

	OrientCont();

	if(MakePath() != 0)
	{
		bOK = false;
		Cont.RemoveAll();
	}

	RestoreCont();
}

NTurnCutRoughCycle::~NTurnCutRoughCycle(void)
{
}

int NTurnCutRoughCycle::MakePath(void)
{
	BPoint V = pCont->GetAt(0)->GetStartDir();
	int NSteps = int(fabs(V.GetX() / Step) + 1); 
	if(V.GetX() < 0.)
		Step *= -1.;
	if(V.GetZ() < 0.)
		R *= -1.;
	BGeomArray OneStep, Path;
	BPoint StartPoint = pCont->GetAt(0)->GetB();
	double CurStep = Step;
	for(int i = 0; i < NSteps - 1; ++i)
	{
		MakeOneStep(OneStep, StartPoint, CurStep);
		CurStep = 2. * Step;
		Path.Append(OneStep);
		StartPoint = OneStep[OneStep.GetSize() - 1]->GetE();
		OneStep.SetSize(0);
	}
	CurStep = NSteps > 1 ? V.GetX() - (NSteps - 2) * Step : V.GetX();
	Step = NSteps > 1 ? CurStep - Step : CurStep;  
	MakeOneStep(OneStep, StartPoint, CurStep);
	Path.Append(OneStep);
//	StartPoint = OneStep[OneStep.GetSize() - 1]->GetE();
	OneStep.SetSize(0);
	pCont->RemoveAll();
	pCont->Append(Path);
	Path.SetSize(0);
	return 0;
}

bool NTurnCutRoughCycle::MakeOneStep(BGeomArray & OneStep, const BPoint & StartPoint, double StepX)
{
	OneStep.RemoveAll();
	
	double dZ = pCont->GetAt(0)->GetStartDir().GetZ();
	double dX = pCont->GetAt(0)->GetStartDir().GetX();
	double dXc = StartPoint.GetX() - pCont->GetAt(0)->GetB().GetX() + StepX;
	double dZc = dZ - dXc * R / dX;
	double RetrZ = - Step * R / dX;

	NCadrGeom *pCadr;
	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(fast);
	pCadr->SetB(StartPoint);
	pCadr->SetE(StartPoint + BPoint(StepX, 0., 0., 0.));
	OneStep.Add(pCadr);
	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(line);
	pCadr->SetB(StartPoint + BPoint(StepX, 0., 0., 0.));
	pCadr->SetE(StartPoint + BPoint(StepX, 0., dZc, 0.));
	OneStep.Add(pCadr);
	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(line);
	pCadr->SetB(StartPoint + BPoint(StepX, 0., dZc, 0.));
	pCadr->SetE(StartPoint + BPoint(StepX - Step, 0., dZc - RetrZ, 0.));
	OneStep.Add(pCadr);
	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(fast);
	pCadr->SetB(StartPoint + BPoint(StepX - Step, 0., dZc - RetrZ, 0.));
	pCadr->SetE(StartPoint + BPoint(StepX - Step, 0., 0., 0.));
	OneStep.Add(pCadr);

	return true;
}


void NTurnCutRoughCycle::OrientCont(void)
{
	if(!orient)
		return;
	// Change orientation
	BMatr Matr;
	Matr.RotY(pCont->GetAt(0)->GetB(), -90.);
	for(int i = 0; i < pCont->GetSize(); ++i)
		pCont->GetAt(i)->Tr(Matr);
}
void NTurnCutRoughCycle::RestoreCont(void)
{
	if(!orient)
		return;

	// Change orientation
	BMatr Matr;
	Matr.RotY(pCont->GetAt(0)->GetB(), 90.);
	for(int i = 0; i < pCont->GetSize(); ++i)
		pCont->GetAt(i)->Tr(Matr);
}
