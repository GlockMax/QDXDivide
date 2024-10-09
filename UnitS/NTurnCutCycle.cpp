#include "stdafx.h"
#include "math.h"
#include "BMatr.h"
#include "NCadrGeom.h"
#include "NTurnCutCycle.h"

double NTurnCutCycle::stat_dX = 0.; 
double NTurnCutCycle::stat_R = 0.; 
double NTurnCutCycle::stat_dZ = 0.; 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

NTurnCutCycle::NTurnCutCycle(BGeomArray &Cont, bool Xfeed)
{
	pCont = &Cont;

	if(pCont->GetSize() <= 0)
		return;

	orient = Xfeed;
	R2 = stat_R;

	if(!pCont->GetAt(0)->HaveGeom())
	{
		pCont->GetAt(0)->SetType(fast);
	}

	pCont->GetAt(0)->SetE(pCont->GetAt(0)->GetB() + BPoint(stat_dX, 0., stat_dZ, 0.));

	OrientCont();

	if(MakePath() != 0)
	{
		bOK = false;
		Cont.RemoveAll();
	}

	RestoreCont();
}

NTurnCutCycle::~NTurnCutCycle(void)
{
}

int NTurnCutCycle::MakePath(void)
{
	BPoint V = pCont->GetAt(0)->GetStartDir();
	if(V.GetX() < 0)
		R2 *= -1.;

	int NSteps = 1;
	BGeomArray OneStep, Path;
	BPoint StartPoint = pCont->GetAt(0)->GetB();
	for(int i = 0; i < NSteps; ++i)
	{
		MakeOneStep(OneStep, StartPoint);
		Path.Append(OneStep);
		StartPoint = OneStep[OneStep.GetSize() - 1]->GetE();
		OneStep.SetSize(0);
	}
	pCont->RemoveAll();
	pCont->Append(Path);
	Path.SetSize(0);
	return 0;
}

bool NTurnCutCycle::MakeOneStep(BGeomArray & OneStep, const BPoint & StartPoint)
{
	OneStep.RemoveAll();
	
	double dX = pCont->GetAt(0)->GetStartDir().GetX();
	double dZ = pCont->GetAt(0)->GetStartDir().GetZ();

	NCadrGeom *pCadr;
	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(fast);
	pCadr->SetB(StartPoint);
	pCadr->SetE(StartPoint + BPoint(dX + R2, 0., 0., 0.));
	OneStep.Add(pCadr);
	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(line);
	pCadr->SetB(StartPoint + BPoint(dX + R2, 0., 0., 0.));
	pCadr->SetE(StartPoint + BPoint(dX, 0., dZ, 0.));
	OneStep.Add(pCadr);
	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(line);
	pCadr->SetB(StartPoint + BPoint(dX, 0., dZ, 0.));
	pCadr->SetE(StartPoint + BPoint(0., 0., dZ, 0.));
	OneStep.Add(pCadr);

	return true;
}


void NTurnCutCycle::OrientCont(void)
{
	if(!orient)
		return;
	// Change orientation
	BMatr Matr;
	Matr.RotY(pCont->GetAt(0)->GetB(), 90.);
	for(int i = 0; i < pCont->GetSize(); ++i)
		pCont->GetAt(i)->Tr(Matr);
}
void NTurnCutCycle::RestoreCont(void)
{
	if(!orient)
		return;

	// Change orientation
	BMatr Matr;
	Matr.RotY(pCont->GetAt(0)->GetB(), -90.);
	for(int i = 0; i < pCont->GetSize(); ++i)
		pCont->GetAt(i)->Tr(Matr);
}
