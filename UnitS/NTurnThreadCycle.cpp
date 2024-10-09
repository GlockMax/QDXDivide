#include "stdafx.h"
#include "ConstDef.h"
#include "math.h"
#include "BMatr.h"
#include "NCadrGeom.h"
#include "NTurnThreadCycle.h"

double NTurnThreadCycle::stat_dZ = 0.;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

NTurnThreadCycle::NTurnThreadCycle(BGeomArray &Cont, double Feed)
{
	pCont = &Cont;

	if(pCont->GetSize() <= 0)
		return;

	FeedPerRev = Feed;

	if(MakePath() != 0)
	{
		bOK = false;
		Cont.RemoveAll();
	}
}

NTurnThreadCycle::~NTurnThreadCycle(void)
{
}

int NTurnThreadCycle::MakePath(void)
{
	if(FeedPerRev == 0.)
		return -1;

	BGeomArray OneStep, Path;
	MakeOneStep(OneStep, pCont->GetAt(0)->GetB());
	Path.Append(OneStep);
	OneStep.SetSize(0);

	pCont->RemoveAll();
	pCont->Append(Path);
	Path.SetSize(0);
	return 0;
}

bool NTurnThreadCycle::MakeOneStep(BGeomArray & OneStep, const BPoint & StartPoint)
{
	OneStep.RemoveAll();
	
	double dX = pCont->GetAt(0)->GetStartDir().GetX();
	double dZ = pCont->GetAt(0)->GetStartDir().GetZ();

	NCadrGeom *pCadr;
	double Length = sqrt(dX * dX + dZ * dZ);
	int NCoils = int( Length / FeedPerRev );
	if(NCoils <= 0)
	{
		Length = 1.;// To prevent zerodivide
		pCadr = new NCadrGeom;// Needed when cycle i < NCoils doesn't work
		pCadr->SetE(StartPoint);
	}

	double dXc = dX * FeedPerRev / Length;
	double dZc = dZ * FeedPerRev / Length;
	double dXprev = 0.;
	double dZprev = 0.;
	for(int i = 0; i < NCoils; ++i)
	{
		pCadr = new NCadrGeom;
		pCadr->SetAttr(7);// Thread attr
		pCadr->SetType(line);
		pCadr->SetB(StartPoint + BPoint(dXprev, 0., dZprev, 0.));
		dXprev += dXc;
		dZprev += dZc;
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
	return true;
}


