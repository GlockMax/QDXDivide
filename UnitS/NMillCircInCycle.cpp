#include "stdafx.h"
#include "math.h"
#include "BMatr.h"
#include "NCadrGeom.h"
#include "NMillCircInCycle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

NMillCircInCycle::NMillCircInCycle(BGeomArray &Cont
		, const	BPoint &StartPointI
		, const	BPoint &EndPointI
		, const	BPoint &CenterI
		, bool CCWDirI 
		, bool NeedApprI
		, bool NeedRoughI
		, bool NeedFinishI
		, double DApprI
		, double DRoughI
		, double DFinishI
		, double XYdistI
		, double ZdistRoI
		, double ZdistFiI
		, double DZdownRoI
		, double DZdownFiI
		, double DZupI)
{
	pCont = &Cont;
	StartPoint = StartPointI;
	EndPoint = EndPointI;
	Center = CenterI;
	CCWDir =  CCWDirI;
	NeedAppr = NeedApprI;
	NeedRough = NeedRoughI;
	NeedFinish = NeedFinishI;
	DAppr = DApprI;
	DRough = DRoughI;
	DFinish = DFinishI;
	XYdist = XYdistI;
	ZdistRo = ZdistRoI;
	ZdistFi = ZdistFiI;
	DZdownRo = DZdownRoI;
	DZdownFi = DZdownFiI;
	DZup = DZupI;
	bOK = true;
	ArcType = CCWDir ? ccwarc : cwarc;

	if(MakePath() != 0)
	{
		bOK = false;
		Cont.RemoveAll();
	}
}

NMillCircInCycle::~NMillCircInCycle(void)
{
}
bool NMillCircInCycle::IsOK()
{
	return bOK;
}
int NMillCircInCycle::MakePath(void)
{

	if(pCont->GetSize() != 1)
		return -1;
	
	BPoint CurPoint = StartPoint;
	if(NeedAppr || NeedRough)
	{
		NCadrGeom *pCadr;
		pCadr = new NCadrGeom;
		pCadr->SetAttr(1);
		pCadr->SetType(fast);
		pCadr->SetB(StartPoint.GetX(), StartPoint.GetY(), pCont->GetAt(0)->GetB().GetZ());
		pCadr->SetE(StartPoint);
		delete pCont->GetAt(0);
		pCont->SetAt(0, pCadr);

		int NLayers = int(ZdistRo / DZdownRo);
		if(NLayers * DZdownRo < ZdistRo - 1.e-6)
			++NLayers;

		for(int i = 0; i < NLayers; ++i)
		{
			bool LastLayer = (i == NLayers - 1);
			bool FirstLayer = (i == 0);
			double DownStep = FirstLayer ? DZdownRo : DZdownRo + (NeedRough ? DZup : 0);
			if(LastLayer)
				DownStep = ZdistRo - (FirstLayer ? 0. : i * DZdownRo - (NeedRough ? DZup : 0));
			double UpStep = LastLayer ? (StartPoint - CurPoint).GetZ() + DownStep : DZup;
			if(NeedAppr)// Врезание
				MakeLayerAppr(CurPoint, DownStep);
			else
			{
				pCadr = new NCadrGeom;
				pCadr->SetAttr(1);
				pCadr->SetType(line);
				pCadr->SetB(CurPoint);
				CurPoint = CurPoint - BPoint(0., 0., DownStep, 0.);
				pCadr->SetE(CurPoint);
				pCont->Add(pCadr);
			}

			if(NeedRough)// Черновая
				MakeLayerRough(CurPoint, UpStep);
		}
	}
	if(NeedFinish)
	{
		double DownStep = ZdistFi - ZdistRo;
		if(DownStep > 0.)
		{
			NCadrGeom *pCadr;
			pCadr = new NCadrGeom;
			pCadr->SetAttr(1);
			pCadr->SetType(line);
			pCadr->SetB(CurPoint);
			CurPoint = BPoint(StartPoint.GetX(), StartPoint.GetY(), StartPoint.GetZ() - ZdistRo, 1.);
			pCadr->SetE(CurPoint);
			pCont->Add(pCadr);
			if(NeedAppr)// Врезание
				MakeLayerAppr(CurPoint, DownStep);
			else
			{
				NCadrGeom *pCadr;
				pCadr = new NCadrGeom;
				pCadr->SetAttr(1);
				pCadr->SetType(line);
				pCadr->SetB(CurPoint);
				CurPoint = CurPoint - BPoint(0., 0., DownStep, 0.);
				pCadr->SetE(CurPoint);
				pCont->Add(pCadr);
			}
			MakeLayerRough(CurPoint, ZdistFi);	
		}
		else
		{
			NCadrGeom *pCadr;
			pCadr = new NCadrGeom;
			pCadr->SetAttr(1);
			pCadr->SetType(fast);
			pCadr->SetB(CurPoint);
//			CurPoint = CurPoint + BPoint(0., 0., ZdistRo, 0.);
			pCadr->SetE(CurPoint);
			pCont->Add(pCadr);
		}
		// Change last point
		NCadrGeom *pCadr;
		pCadr = pCont->GetAt(pCont->GetSize() - 1);
		CurPoint = BPoint(Center.GetX() + DFinish / 2. - DRough, Center.GetY(), pCadr->GetE().GetZ());
		pCadr->SetE(CurPoint);
		BPoint FinStartPoint = CurPoint;

		int NLayers = int(ZdistFi / DZdownFi);
		if(NLayers * DZdownFi < ZdistFi - 1.e-6)
			++NLayers;
		for(int i = 0; i < NLayers; ++i)
		{
			bool LastLayer = (i == NLayers - 1);
			bool FirstLayer = (i == 0);
			double DownStep = DZdownFi;
			if(LastLayer)
				DownStep = ZdistFi - (FirstLayer ? 0. : i * DZdownFi);
			MakeLayerFinSide(CurPoint, DownStep, 10);
			NCadrGeom *pCadr;
			pCadr = new NCadrGeom;
			pCadr->SetAttr(1);
			pCadr->SetType(line);
			pCadr->SetB(CurPoint);
			CurPoint = BPoint(FinStartPoint.GetX(), FinStartPoint.GetY(), CurPoint.GetZ(), 1.);
			pCadr->SetE(CurPoint);
			pCont->Add(pCadr);
		}
	}

	NCadrGeom *pCadr;
	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(fast);
	pCadr->SetB(StartPoint.GetX(), StartPoint.GetY(), pCont->GetAt(0)->GetB().GetZ());
	pCadr->SetE(StartPoint);

	delete pCont->GetAt(0);
	pCont->SetAt(0, pCadr);

	pCadr = pCont->GetAt(pCont->GetSize() - 1);
	pCadr->SetType(fast);
	pCadr->SetE(StartPoint.GetX(), StartPoint.GetY(), EndPoint.GetZ());

	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(fast);
	pCadr->SetB(StartPoint.GetX(), StartPoint.GetY(), EndPoint.GetZ());
	pCadr->SetE(EndPoint);
	pCont->Add(pCadr);
	return 0;
}
int NMillCircInCycle::MakeLayerAppr(BPoint &CurPoint, double DownStep)
{
	NCadrGeom *pCadr;

	BPoint I((Center - CurPoint).ProjXY());

	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(ArcType);
	pCadr->SetB(CurPoint);
	CurPoint = CurPoint - BPoint(0., 0., DownStep, 0.);
	pCadr->SetE(CurPoint);
	pCadr->SetI(I);
	pCont->Add(pCadr);

	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(ArcType);
	pCadr->SetB(CurPoint);
	pCadr->SetE(CurPoint);
	pCadr->SetI(I);
	pCont->Add(pCadr);
	return 0;
}
int NMillCircInCycle::MakeLayerRough(BPoint &CurPoint, double UpStep)
{
	BPoint End(CurPoint);
	BPoint I((CurPoint - Center).ProjXY());
	double CurR = sqrt(I.D2());
	if(CurR < 1.e-6)
		I = BPoint(1., 0., 0., 0.);
	else
		I = I * (1. / CurR);
	int Npas = int(fabs(int((DRough / 2. - CurR)) / fabs(XYdist)) + 1);
	double SideStep = (DRough / 2. - CurR) / Npas;
	NCadrGeom *pCadr;
	for(int i = 0; i < Npas; ++i)
	{
		CurR += SideStep / 4.; 
		pCadr = new NCadrGeom;
		pCadr->SetAttr(1);
		pCadr->SetType(ArcType);
		pCadr->SetB(CurPoint);
		CurPoint = CurPoint + I * (-2. * CurR);
		pCadr->SetE(CurPoint);
		pCadr->SetI(I * (-CurR));
		pCont->Add(pCadr);

		CurR += SideStep * 0.5; 
		pCadr = new NCadrGeom;
		pCadr->SetAttr(1);
		pCadr->SetType(ArcType);
		pCadr->SetB(CurPoint);
		CurPoint = CurPoint + I * (2. * CurR);
		pCadr->SetE(CurPoint);
		pCadr->SetI(I * CurR);
		pCont->Add(pCadr);
		CurR += SideStep / 4.; 
	}
	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(ArcType);
	pCadr->SetB(CurPoint);
	pCadr->SetE(CurPoint);
	pCadr->SetI(I * (-CurR));
	pCont->Add(pCadr);
	pCadr = new NCadrGeom;

	pCadr->SetAttr(1);
	pCadr->SetType(fast);
	pCadr->SetB(CurPoint);
	CurPoint = End + BPoint(0., 0., UpStep, 0.);
	pCadr->SetE(CurPoint);
	pCont->Add(pCadr);
	return 0;
}

int NMillCircInCycle::MakeLayerFinSide(BPoint &CurPoint, double DownStep, double Angle)
{
	NCadrGeom *pCadr;

	BPoint I((Center - CurPoint).ProjXY());
	double CurR = sqrt(I.D2());
	if(CurR < 1.e-6)
		I = BPoint(1., 0., 0., 0.);
	else
		I = I * (1. / CurR);

	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(fast);
	pCadr->SetB(CurPoint);
	CurPoint = CurPoint - BPoint(0., 0., DownStep, 0.);
	pCadr->SetE(CurPoint);
	pCont->Add(pCadr);

	double ApprR = (DFinish / 2. - CurR) * 0.5;

	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(ArcType);
	pCadr->SetB(CurPoint);
	CurPoint = CurPoint - I * (2. * ApprR);
	pCadr->SetE(CurPoint);
	pCadr->SetI(I * (-ApprR));
	pCont->Add(pCadr);

	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(ArcType);
	pCadr->SetB(CurPoint);
	pCadr->SetE(CurPoint);
	pCadr->SetI(I * (DFinish / 2.));
	pCont->Add(pCadr);

	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(ArcType);
	pCadr->SetB(CurPoint);
	CurPoint = CurPoint * BMatr().RotZ(Center, CCWDir ? Angle : -Angle);
	pCadr->SetE(CurPoint );
	pCadr->SetI(I * (DFinish / 2.));
	pCont->Add(pCadr);

	I = (Center - CurPoint).ProjXY();
	I = I * (2. / DFinish);
	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(ArcType);
	pCadr->SetB(CurPoint);
	CurPoint = CurPoint + I * (2. * ApprR);
	pCadr->SetE(CurPoint);
	pCadr->SetI(I * ApprR);
	pCont->Add(pCadr);

	return 0;
}
