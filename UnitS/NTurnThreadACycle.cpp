#include "stdafx.h"
#include "ConstDef.h"
#include "math.h"
#include "BMatr.h"
#include "NCadrGeom.h"
#include "NTurnThreadACycle.h"

double NTurnThreadACycle::stat_dZ = 0.;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

NTurnThreadACycle::NTurnThreadACycle(BGeomArray &Cont, double ThreadQ1, double ThreadR1, double ThreadP1, double ThreadQ2, double ThreadR2, double ThreadP2, double Feed, double iSyncroTime)
{
	pCont = &Cont;

	if(pCont->GetSize() <= 0)
		return;

	Q1 = ThreadQ1;
	R1 = ThreadR1;
	P1 = ThreadP1;
	Q2 = ThreadQ2;
	R2 = ThreadR2;
	P2 = ThreadP2;
	FeedPerRev = Feed;
	SyncroTime = iSyncroTime;

	if(MakePath() != 0)
	{
		bOK = false;
		Cont.RemoveAll();
	}
}

NTurnThreadACycle::~NTurnThreadACycle(void)
{
}

int NTurnThreadACycle::MakePath(void)
{
	if(FeedPerRev == 0.)
		return -1;

	BPoint V = pCont->GetAt(0)->GetStartDir();
	if(V.GetX() < 0)
		R2 *= -1.;
	if(V.GetZ() < 0)
		FeedPerRev *= -1.;

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

bool NTurnThreadACycle::MakeOneStep(BGeomArray & OneStep, const BPoint & StartPoint)
{
	OneStep.RemoveAll();
	
	double dX = pCont->GetAt(0)->GetStartDir().GetX();
	double dZ = pCont->GetAt(0)->GetStartDir().GetZ();

	if(dX < 0.)
		R2 *= -1.;
	double dXprev = dX + R2;
	double dZprev = 0.;

	NCadrGeom *pCadr;
	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(fast);
	pCadr->SetB(StartPoint);
	pCadr->SetE(StartPoint + BPoint(dXprev, 0., 0., 0.));
	OneStep.Add(pCadr);
	int NCoils = int(fabs( dZ / FeedPerRev ));
	for(int i = 0; i < NCoils; ++i)
	{
		pCadr = new NCadrGeom;
		pCadr->SetAttr(7);// Thread attr
		pCadr->SetType(line);
		pCadr->SetB(StartPoint + BPoint(dXprev, 0., dZprev, 0.));
		dZprev += FeedPerRev ;
		dXprev += -R2 * FeedPerRev / dZ;
		pCadr->SetE(StartPoint + BPoint(dXprev, 0., dZprev, 0.));
		OneStep.Add(pCadr);
	}
	if((pCadr->GetE() - StartPoint - BPoint(dX, 0., dZ, 0.)).D2() > MIND * MIND)
	{
		pCadr = new NCadrGeom;
		pCadr->SetAttr(7);// Thread attr
		pCadr->SetType(line);
		pCadr->SetB(StartPoint + BPoint(dXprev, 0., dZprev, 0.));
		pCadr->SetE(StartPoint + BPoint(dX, 0., dZ, 0.));
		OneStep.Add(pCadr);
	}
	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->Set(undef, StartPoint + BPoint(dX, 0., dZ, 0.), StartPoint + BPoint(dX, 0., dZ, 0.));
	pCadr->SetWorkTimeS(SyncroTime);
	OneStep.Add(pCadr);
	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(fast);
	pCadr->SetB(StartPoint + BPoint(dX, 0., dZ, 0.));
	pCadr->SetE(StartPoint + BPoint(0., 0., dZ, 0.));
	OneStep.Add(pCadr);

	return true;
}




int NTurnThreadACycle::CalcPassesNum(void)
{
	if(Q1 <= 0. || Q2 <= 0)
		return -1;

	double D = P2 - R1; // Depth
	double Q12 = Q1 * Q1;
	double Q22 = Q2 * Q2;
	double b = Q12 + Q22;
	int Imax = int(b * b / (4. * Q12 * Q22)); // Max number of steps while step is grater than Q1 (Q2 - first step)

	double Dcur = D;
	int Icur = 1;
	for(double Delta = 0.; Dcur - Delta >= 0 && Icur < Imax; ++Icur, Dcur -= Delta)
		Delta = Q2 * (sqrt(double(Icur)) - sqrt(double(Icur - 1)));
	--Icur;
	Icur += int(P1 + 0.5); // Finish passes number
	if(Dcur < 0.)
		return Icur;
	Icur += int((Dcur + 1.e-6) / Q1);

	return Icur;
}
