#include "stdafx.h"
#include "math.h"
#include "ConstDef.h"
#include "NCadrGeom.h"
#include "BGeomArray.h"
#include "NMillDrillCycle.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

NMillDrillCycle::NMillDrillCycle(BGeomArray &Cont, double iFeed, bool First, bool RetToStart, double Ref, double PeckIDepth, bool RefAbs
	, const BMatr &M0, const BMatr &M1, double AddTime, double iDepth)
{
	pCont = &Cont;
	StepAddTime = AddTime;

	if(pCont->GetSize() <= 0)
		return;
	BPoint StartP(pCont->GetAt(0)->GetE() * M0);
	//for(int i = 0; i < pCont->GetSize(); ++i)
	//{
	//	pCont->GetAt(i)->Tr(M0);
	//}

	FirstCycleCadr = First;
	ReturnToStartPlane = RetToStart;
	Feed = iFeed;
	PeckIncrementDepth = PeckIDepth;
	Depth = iDepth;
	if(FirstCycleCadr)
		StartZ = StartP.GetZ();
	RefZ = RefAbs ? Ref : StartZ + Ref;
	if(MakePath(StartP) != 0)
	{
		bOK = false;
		Cont.RemoveAll();
	}
	for(int i = 0; i < pCont->GetSize(); ++i)
	{
		pCont->GetAt(i)->Tr(M1);
	}
}


NMillDrillCycle::~NMillDrillCycle(void)
{
}

int NMillDrillCycle::MakePath(const BPoint &StartP)
{
	if(Feed == 0.)
		return -1;
	if(PeckIncrementDepth < MIND)
		return -1;

	//double Depth = pCont->GetAt(0)->GetE().GetZ();
	if(fabs(Depth - pCont->GetAt(0)->GetBP().GetZ()) < MIND)
	{
		if(FirstCycleCadr)
			return -1;
		Depth = PreviousDepth;
	}
	PreviousDepth = Depth;

	BPoint EndP(StartP);
	EndP.SetZ(Depth);
	BPoint RetP(StartP);
	RetP.SetZ(ReturnToStartPlane ? StartZ : RefZ);

	pCont->GetAt(0)->SetType(fast);
	pCont->GetAt(0)->SetB(StartP);
	BPoint E(StartP);
	E.SetZ(RefZ);
	pCont->GetAt(0)->SetE(E);
	pCont->GetAt(0)->SetAttr(1);

	double StartZ = pCont->GetAt(0)->GetBP().GetZ();
	double Delta = 0.5;
	double sMIND = MIND;
	bool UpDrill = (StartZ < EndP.GetZ());
	if(UpDrill)
	{
		Delta = -Delta;
		PeckIncrementDepth = -PeckIncrementDepth;
		sMIND = -sMIND;
	}
	BPoint RP(pCont->GetLast()->GetE());
	for(double StepDownZ = RefZ - PeckIncrementDepth; (UpDrill ? -1. : 1.) * (StepDownZ - EndP.GetZ() - sMIND) > 0.; StepDownZ -= PeckIncrementDepth)
	{
		NCadrGeom *pGeom = new NCadrGeom;
		BPoint E(pCont->GetLast()->GetE());
		E.SetZ(StepDownZ);
		pGeom->Set(line, pCont->GetLast()->GetE(), E);
		pGeom->SetAttr(1);
		pCont->Add(pGeom);

		pGeom = new NCadrGeom;
		pGeom->Set(fast, pCont->GetLast()->GetE(), RP);
		pGeom->SetAttr(1);
		pCont->Add(pGeom);

		pGeom = new NCadrGeom;
		pGeom->SetAttr(1);
		pGeom->Set(undef, RP, RP);
		pGeom->SetWorkTimeS(StepAddTime);
		pCont->Add(pGeom);

		pGeom = new NCadrGeom;
		pGeom->Set(fast, pCont->GetLast()->GetE(), E + BPoint(0., 0., Delta, 0.));
		pGeom->SetAttr(1);
		pCont->Add(pGeom);
	}
	NCadrGeom *pGeom = new NCadrGeom;
	pGeom->Set(line, pCont->GetLast()->GetE(), EndP);
	pGeom->SetAttr(1);
	pCont->Add(pGeom);

	pGeom = new NCadrGeom;
	E.SetZ(E.GetZ() - PeckIncrementDepth);
	pGeom->Set(fast, pCont->GetLast()->GetE(), RetP);
	pGeom->SetAttr(1);
	pCont->Add(pGeom);
	return 0;
}