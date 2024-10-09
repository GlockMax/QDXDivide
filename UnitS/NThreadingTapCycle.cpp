#include "stdafx.h"
#include "math.h"
#include "ConstDef.h"
#include "NCadrGeom.h"
#include "BGeomArray.h"
#include "NThreadingTapCycle.h"

double NThreadingTapCycle::PreviousDepth = DRILL_EMPTY_Z;
double NThreadingTapCycle::StartZ = 0.;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

NThreadingTapCycle::NThreadingTapCycle(BGeomArray &Cont, double iFeed, bool First, bool RetToStart, double Ref, double ReverseT, const BMatr &M0, const BMatr &M1)
{
	pCont = &Cont;

	if(pCont->GetSize() <= 0)
		return;
	for(int i = 0; i < pCont->GetSize(); ++i)
	{
		pCont->GetAt(i)->Tr(M0);
	}
	FirstCycleCadr = First;
	ReturnToStartPlane = RetToStart;
	Feed = iFeed;
	RefZ = Ref;
	ReverseTime = ReverseT;

	if(FirstCycleCadr)
		StartZ = pCont->GetAt(0)->GetB().GetZ();
	if(MakePath() != 0)
	{
		bOK = false;
		Cont.RemoveAll();
	}
	for(int i = 0; i < pCont->GetSize(); ++i)
	{
		pCont->GetAt(i)->Tr(M1);
	}
}


NThreadingTapCycle::~NThreadingTapCycle(void)
{
}

int NThreadingTapCycle::MakePath(void)
{
	if(Feed == 0.)
		return -1;

	double Depth = pCont->GetAt(0)->GetE().GetZ();
	if(fabs(Depth - pCont->GetAt(0)->GetB().GetZ()) < MIND)
	{
		if(FirstCycleCadr)
			return -1;
		Depth = PreviousDepth;
	}
	PreviousDepth = Depth;

	BPoint EndP(pCont->GetAt(0)->GetE());
	BPoint StartP(EndP);
	EndP.SetZ(Depth);
	StartP.SetZ(pCont->GetAt(0)->GetB().GetZ());
	BPoint RetP(pCont->GetAt(0)->GetE());
	RetP.SetZ(ReturnToStartPlane ? StartZ : RefZ);

	pCont->GetAt(0)->Clear();
	pCont->GetAt(0)->SetType(fast);
	pCont->GetAt(0)->SetAttr(1);
	pCont->GetAt(0)->SetB(StartP);
	BPoint E(pCont->GetAt(0)->GetB());
	E.SetZ(RefZ);
	pCont->GetAt(0)->SetE(E);

	double StartZ = pCont->GetAt(0)->GetE().GetZ();
	if(StartZ < EndP.GetZ())
		return -1;
	BPoint RP(pCont->GetLast()->GetE());

	NCadrGeom *pGeom = new NCadrGeom;
	pGeom->Set(line, pCont->GetLast()->GetE(), EndP);
	pGeom->SetAttr(1);
	pCont->Add(pGeom);

	pGeom = new NCadrGeom;
	pGeom->Set(undef, pCont->GetLast()->GetE(), pCont->GetLast()->GetE());
	pGeom->SetWorkTimeS(ReverseTime);
	pGeom->SetAttr(1);
	pCont->Add(pGeom);

	pGeom = new NCadrGeom;
	pGeom->Set(line, pCont->GetLast()->GetE(), E);
	pGeom->SetAttr(1);
	pCont->Add(pGeom);

	pGeom = new NCadrGeom;
	pGeom->Set(fast, pCont->GetLast()->GetE(), RetP);
	pGeom->SetAttr(1);
	pCont->Add(pGeom);
	return 0;
}