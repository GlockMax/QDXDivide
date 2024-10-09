#include "stdafx.h"
#include "ConstDef.h"
#include "math.h"
#include "BMatr.h"
#include "NCadrGeom.h"
#include "NTurnThreadCutCycle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

NTurnThreadCutCycle::NTurnThreadCutCycle(BGeomArray &Cont, const NCPar &CPar)
: Par(CPar), StartPoint(0., 0., 0., 1.), EndPoint(0., 0., 0., 1.)
{return;// Заморожено до выяснения
	pCont = &Cont;

	if(pCont->GetSize() > 0)
		return;

	//Step = XStep;
	//Retr = XRetr;
	//orient = Xfeed;

	OrientCont();

	if(MakePath() != 0)
	{
		bOK = false;
		Cont.RemoveAll();
	}

	RestoreCont();
}

NTurnThreadCutCycle::~NTurnThreadCutCycle(void)
{
}

int NTurnThreadCutCycle::MakePath(void)
{
	// Calculate feed
	double Dx = (Par.DM2 - Par.DM1) / 2.;
	double Dz = (Par.FPL - Par.SPL);
	double DxA = fabs(Dx);
	double DzA = fabs(Dz);
	double TanA = DxA < DzA ? DxA / DzA : DzA / DxA;
	double CosA = cos(atan(TanA));
	double Feed = Par.PIT / CosA;

	pCont->RemoveAll();
	
	// Calculate approach/retract direction
	BPoint AppRetDir(1., 0., 0., 0.); // Direction for approach and retract (Approach is positive)
	if(DxA > DzA)
		AppRetDir.Set(0., 0., 1., 0.);
	else
		AppRetDir.Set(1., 0., 0., 0.);
	switch(Par.VARI)
	{
	case 1:
	case 3:
		AppRetDir = AppRetDir * (-1.);
	case 2:
	case 4:
		break;
	default:
		return -1;
	}

	// Calculate work step
	double Scale = sqrt(Dx * Dx + Dz * Dz);
	if(Scale < MIND)
		return -3;
	Scale = Feed / Scale; 
	BPoint WorkStep(Dx * Scale, 0., Dz * Scale, 0.);

	// Calculate start and end points
	StartPoint.Set(Par.DM1, 0., Par.SPL, 1.);
	StartPoint = StartPoint + WorkStep * (-Feed * Par.APP);
	EndPoint.Set(Par.DM2, 0., Par.FPL, 1.);
	EndPoint = EndPoint + WorkStep * (Feed * Par.ROP);


	CurDepth = 0.;
	//BPoint V;
	//int NSteps = (Step == 0.) ? 1 : int(fabs((fabs(V.GetX()) - 1.e-4) / Step) + 1); 
	//if(V.GetX() < 0)
	//	Step *= -1.;
	//else
	//	Retr *= -1.;
	//BGeomArray OneStep, Path;
	//BPoint StartPoint = pCont->GetAt(0)->GetB();
	//for(int i = 0; i < NSteps; ++i)
	//{
	//	MakeOneStep(OneStep, StartPoint, Step - Retr);
	//	Path.Append(OneStep);
	//	StartPoint = OneStep[OneStep.GetSize() - 1]->GetE();
	//	OneStep.SetSize(0);
	//}
	//// Last move must be in the given position
	//StartPoint.Set(pCont->GetAt(0)->GetE().GetX(), StartPoint.GetY(), StartPoint.GetZ(), StartPoint.GetH());
	//Path.GetAt(Path.GetSize() - 1)->SetE(StartPoint);
	//MakeOneStep(OneStep, StartPoint, Step - Retr);
	//Path.Append(OneStep);
	//OneStep.SetSize(0);

	//pCont->RemoveAll();
	//pCont->Append(Path);
	//Path.SetSize(0);
	return 0;
}

bool NTurnThreadCutCycle::MakeOneStep(BGeomArray & OneStep, const BPoint & StartPoint, double StepX)
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
	pCadr->SetType(line);
	pCadr->SetB(StartPoint + BPoint(0., 0., dZ, 0.));
	pCadr->SetE(StartPoint + BPoint(Retr, 0., dZ, 0.));
	OneStep.Add(pCadr);
	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(fast);
	pCadr->SetB(StartPoint + BPoint(Retr, 0., dZ, 0.));
	pCadr->SetE(StartPoint + BPoint(Retr, 0., 0., 0.));
	OneStep.Add(pCadr);
	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(fast);
	pCadr->SetB(StartPoint + BPoint(Retr, 0., 0., 0.));
	pCadr->SetE(StartPoint + BPoint(StepX + Retr, 0., 0., 0.));
	OneStep.Add(pCadr);

	return true;
}


void NTurnThreadCutCycle::OrientCont(void)
{
//	if(!orient)
		return;
	// Change orientation
	BMatr Matr;
	Matr.RotY(pCont->GetAt(0)->GetB(), -90.);
	for(int i = 0; i < pCont->GetSize(); ++i)
		pCont->GetAt(i)->Tr(Matr);
}
void NTurnThreadCutCycle::RestoreCont(void)
{
//	if(!orient)
		return;

	// Change orientation
	BMatr Matr;
	Matr.RotY(pCont->GetAt(0)->GetB(), 90.);
	for(int i = 0; i < pCont->GetSize(); ++i)
		pCont->GetAt(i)->Tr(Matr);
}
