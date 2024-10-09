#include "stdafx.h"
#include "NCadrGeom.h"
#include "nmillrectcycle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

NMillRectCycle::NMillRectCycle(BGeomArray &Cont
		, const	BPoint &SP
		, const	BPoint &EP
		, const	BPoint &CD // Вектор рабочего хода
		, const BPoint &BD // Вектор перемещения между рабочими ходами
		, int Np
		, double Zd
		, double DZd
		, double DZu
		, bool ODC)
{
	pCont = &Cont;
	StartPoint = SP;
	EndPoint = EP;
	CDir = CD;
	BDir = BD;
	Npas = Np;
	Zdist = Zd;
	DZdown = DZd;
	DZup = DZu;
	OneDirCut = ODC;
	bOK = true;

	if(MakePath() != 0)
	{
		bOK = false;
		Cont.RemoveAll();
	}
}

NMillRectCycle::~NMillRectCycle(void)
{
}
bool NMillRectCycle::IsOK()
{
	return bOK;
}
int NMillRectCycle::MakePath(void)
{

	if(pCont->GetSize() != 1)
		return -1;

	NCadrGeom *pCadr;
	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(fast);
	pCadr->SetB(StartPoint.GetX(), StartPoint.GetY(), pCont->GetAt(0)->GetB().GetZ());
	pCadr->SetE(StartPoint);

	delete pCont->GetAt(0);
	pCont->SetAt(0, pCadr);

	int NLayers = int(Zdist / DZdown);
	if(NLayers * DZdown < Zdist - 1.e-6)
		++NLayers;

	BPoint CurPoint = StartPoint;
	for(int i = 0; i < NLayers; ++i)
	{
		bool LastLayer = (i == NLayers - 1);
		bool FirstLayer = (i == 0);
		double DownStep = FirstLayer ? DZdown : DZdown + DZup;
		if(LastLayer)
			DownStep = Zdist - (FirstLayer ? 0. : i * DZdown - DZup);
		double UpStep = LastLayer ? (EndPoint - CurPoint).GetZ() + DownStep : DZup;
		MakeLayer(CurPoint, DownStep, UpStep, DZup);
	}
	pCadr = pCont->GetAt(pCont->GetSize() - 1);
	pCadr->SetAttr(1);
	pCadr->SetType(fast);
	pCadr->SetE(EndPoint);

	return 0;
}
int NMillRectCycle::MakeLayer(BPoint &CurPoint, double DownStep, double UpStep, double IntStep)
{
	NCadrGeom *pCadr;
	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(fast);
	pCadr->SetB(CurPoint);
	CurPoint = CurPoint + BPoint(0., 0., -DownStep, 0.);
	pCadr->SetE(CurPoint);
	pCont->Add(pCadr);

	BPoint P1 = CurPoint;
	BPoint P2 = P1 + CDir;
	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(line);
	pCadr->SetB(P1);
	pCadr->SetE(P2);
	pCont->Add(pCadr);
	for(int i = 0; i < Npas - 1; ++i)
	{
		P1 = P2;
		P2 = P1 + (OneDirCut ? BDir - CDir : BDir);
		if(OneDirCut)
		{
			pCadr = new NCadrGeom;
			pCadr->SetAttr(1);
			pCadr->SetType(fast);
			pCadr->SetB(P1);
			P1 = P1 + BPoint(0., 0., IntStep, 0.);
			pCadr->SetE(P1);
			pCont->Add(pCadr);
			P2 = P2 + BPoint(0., 0., IntStep, 0.);
		}
		pCadr = new NCadrGeom;
		pCadr->SetAttr(1);
		pCadr->SetType(OneDirCut ? fast : line);
		pCadr->SetB(P1);
		pCadr->SetE(P2);
		pCont->Add(pCadr);
		if(OneDirCut)
		{
			pCadr = new NCadrGeom;
			pCadr->SetAttr(1);
			pCadr->SetType(fast);
			pCadr->SetB(P2);
			P2 = P2 - BPoint(0., 0., IntStep, 0.);
			pCadr->SetE(P2);
			pCont->Add(pCadr);
		}
		P1 = P2;
		P2 = P1 + (OneDirCut ? CDir : CDir * ( i % 2 == 0 ? -1. : 1.));
		pCadr = new NCadrGeom;
		pCadr->SetAttr(1);
		pCadr->SetType(line);
		pCadr->SetB(P1);
		pCadr->SetE(P2);
		pCont->Add(pCadr);
	}
	CurPoint = CurPoint + BPoint(0., 0., UpStep, 0.);
	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(fast);
	pCadr->SetB((pCont->GetAt(pCont->GetSize() - 1))->GetE());
	pCadr->SetE(pCadr->GetB() + BPoint(0., 0., UpStep, 0.));
	pCont->Add(pCadr);
	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(fast);
	pCadr->SetB((pCont->GetAt(pCont->GetSize() - 1))->GetE());
	pCadr->SetE(CurPoint);
	pCont->Add(pCadr);
	return 0;
}
