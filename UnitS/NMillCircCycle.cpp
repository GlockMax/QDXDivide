#include "stdafx.h"
#include "math.h"
#include "BMatr.h"
#include "NCadrGeom.h"
#include "NMillCircCycle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

NMillCircCycle::NMillCircCycle(BGeomArray &Cont
		, const	BPoint &SP
		, const	BPoint &EP
		, const	BPoint &C
		, const	BPoint &CD // Вектор рабочего хода
		, double XY// Расстояние между проходами
		, double Zd
		, double DZd
		, double DZu
		, bool ODC)
{
	pCont = &Cont;
	StartPoint = SP;
	EndPoint = EP;
	Center = C;
	CDir = CD;
	XYdist = XY;
	Zdist = Zd;
	DZdown = DZd;
	DZup = DZu;
	OneDirCut = ODC;
	bOK = true;

	CircR = sqrt((StartPoint - Center).ProjXY().D2());
	Norm = CDir * BMatr().RotZ(0., 0., 0., 90.);

	if(MakePath() != 0)
	{
		bOK = false;
		Cont.RemoveAll();
	}
}

NMillCircCycle::~NMillCircCycle(void)
{
}
bool NMillCircCycle::IsOK()
{
	return bOK;
}
int NMillCircCycle::MakePath(void)
{

	if(pCont->GetSize() != 1)
		return -1;

	if(fabs(XYdist) < 1.e-6)
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
int NMillCircCycle::MakeLayer(BPoint &CurPoint, double DownStep, double UpStep, double IntStep)
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
	BPoint P2 = GetSimP(P1);
	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(line);
	pCadr->SetB(P1);
	pCadr->SetE(P2);
	pCont->Add(pCadr);

	double StartDist = (Center - StartPoint).ProjXY() * Norm * (XYdist > 0. ? 1. : -1.);
	int Npas = int((2. * StartDist) / fabs(XYdist)) + 1;
	for(int i = 0; i < Npas; ++i)
	{
		P1 = P2;
		P2 = OneDirCut ? GetSimP(GetNextP(P1)) : GetNextP(P1);
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
		P2 = GetSimP(P1);
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

BPoint NMillCircCycle::GetSimP(BPoint & P)
{
	BPoint D = (P - Center).ProjXY();
	D = CDir * (D * CDir * 2.);

	return P - D;
}

BPoint NMillCircCycle::GetNextP(const BPoint & P)
{
	double Hs = (P - Center).ProjXY() * Norm;
	double Rs = CircR * CircR - Hs * Hs;
	if(Rs <= 0.)
		return P;
	Rs = sqrt(Rs);

	double He = Hs + XYdist;
	double Re = CircR * CircR - He * He;
	if(Re <= 0.)
		return P;
	Re = sqrt(Re);

	double Delta = Re - Rs;
	if((P - Center).ProjXY() * CDir < 0.)
		Delta = - Delta;
	return P + Norm * XYdist + CDir * Delta;
}
