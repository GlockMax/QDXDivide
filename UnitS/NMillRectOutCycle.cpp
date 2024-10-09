#include "stdafx.h"
#include "math.h"
#include "BMatr.h"
#include "NCadrGeom.h"
#include "NMillRectOutCycle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

NMillRectOutCycle::NMillRectOutCycle(BGeomArray &Cont
		, const	BPoint &StartPointI
		, const	BPoint &CenterI
		, const	BPoint &EndPointI 
		, const	BPoint &EXI 
		, const BPoint &EYI
		, double DXroughI
		, double DYroughI
		, double DXfinishI
		, double DYfinishI
		, bool CCWDirI
		, bool NeedRoughI
		, bool NeedFinishI
		, double XYdistI
		, double ZdistRoI
		, double ZdistFiI
		, double DZdownRoI
		, double DZdownFiI
		, double CornerRI
		, double XYclrI
		, double DZupI)
		: StartPoint(StartPointI), Center(CenterI), EndPoint(EndPointI), EX(EXI), EY(EYI)
{
	pCont = &Cont;
	DXrough = DXroughI;
	DYrough = DYroughI;
	DXfinish = DXfinishI;
	DYfinish = DYfinishI;
	CCWDir = CCWDirI;
	NeedRough = NeedRoughI;
	NeedFinish = NeedFinishI;
	XYdist = XYdistI;
	ZdistRo = ZdistRoI;
	ZdistFi = ZdistFiI;
	DZdownRo = DZdownRoI;
	DZdownFi = DZdownFiI;
	CornerR = CornerRI;
	XYclr = XYclrI;
	DZup = DZupI;
	bOK = true;
	ArcType = CCWDir ? ccwarc : cwarc;

	if(MakePath() != 0)
	{
		bOK = false;
		Cont.RemoveAll();
	}
}

NMillRectOutCycle::~NMillRectOutCycle(void)
{
}
bool NMillRectOutCycle::IsOK()
{
	return bOK;
}
int NMillRectOutCycle::MakePath(void)
{

	if(pCont->GetSize() != 1)
		return -1;

	double Width = (Center - StartPoint) * EX - DXfinish / 2.;
	int Npas = int(Width / fabs(XYdist));
	if(fabs(Npas * XYdist - Width) > 1.e-6)
		++Npas;
	XYdist = Width / Npas;
	if(CCWDir)
		StartPoint = StartPoint + BPoint( - XYclr, XYdist, 0., 0.);
	else
		StartPoint = StartPoint + BPoint( XYdist, - XYclr, 0., 0.);

	NCadrGeom *pCadr;
	pCadr = pCont->GetAt(0);
	pCadr->SetAttr(1);
	pCadr->SetType(fast);
	pCadr->SetB(StartPoint.GetX(), StartPoint.GetY(), pCont->GetAt(0)->GetB().GetZ());
	BPoint CurPoint = StartPoint;
	pCadr->SetE(CurPoint);

	int NLayers = int(ZdistRo / DZdownRo);
	if(NLayers * DZdownRo < ZdistRo - 1.e-6)
		++NLayers;
	double DZdown = ZdistRo / NLayers;

	if(NeedRough)
	{
		for(int i = 0; i < NLayers; ++i)
		{
			bool FirstLayer = (i == 0);
			double DownStep = FirstLayer ? DZdown : DZdown + DZup;
			double UpStep = DZup;
			NCadrGeom *pCadr;
			pCadr = new NCadrGeom;
			pCadr->SetAttr(1);
			pCadr->SetType(fast);
			pCadr->SetB(CurPoint);
			CurPoint = CurPoint - BPoint(0., 0., DownStep, 0.);
			pCadr->SetE(CurPoint);
			pCont->Add(pCadr);

			BPoint EndPoint(StartPoint.GetX(), StartPoint.GetY(), CurPoint.GetZ() + UpStep, 1.);
			MakeLayerRough(CurPoint, EndPoint, Npas);
		}
		pCadr = new NCadrGeom;
		pCadr->SetAttr(1);
		pCadr->SetType(fast);
		pCadr->SetB(CurPoint);
		pCadr->SetE(CurPoint);
		pCont->Add(pCadr);
	}
	if(NeedFinish)
	{
		NLayers = int(ZdistFi / DZdownFi);
		if(NLayers * DZdownFi < ZdistFi - 1.e-6)
			++NLayers;
		double DownStep = ZdistFi / NLayers;
		BPoint FinStartPoint = Center - EX * (DXrough / 2.) - EY * (DYrough / 2.) - BPoint(0., 0., DownStep, 0.);

		FinStartPoint = FinStartPoint + (CCWDir ? 
			BPoint(- XYclr, (DYrough - DYfinish) / 2., 0., 0.) :
			BPoint((DXrough - DXfinish) / 2., - XYclr, 0., 0.));

		// Change last point
		NCadrGeom *pCadr;
		pCadr = pCont->GetAt(pCont->GetSize() - 1);
		pCadr->SetType(fast);
		CurPoint = FinStartPoint;
		pCadr->SetE(CurPoint);

		for(int i = 0; i < NLayers; ++i)
		{
			bool LastLayer = (i == NLayers - 1);
			bool FirstLayer = (i == 0);
			pCadr = new NCadrGeom;
			pCadr->SetAttr(1);
			pCadr->SetType(fast);
			pCadr->SetB(CurPoint);
			CurPoint = FinStartPoint - BPoint(0., 0., DownStep * i, 0.);
			pCadr->SetE(CurPoint);
			pCont->Add(pCadr);
			MakeLayerFinSide(CurPoint);
			if(!LastLayer)
			{
				pCadr = new NCadrGeom;
				pCadr->SetAttr(1);
				pCadr->SetType(fast);
				pCadr->SetB(CurPoint);
				CurPoint = CurPoint - (CCWDir ? EX : EY) * CornerR;
				pCadr->SetE(CurPoint);
				pCont->Add(pCadr);
			}
		}
		//pCadr = new NCadrGeom;
		//pCadr->SetAttr(1);
		//pCadr->SetType(fast);
		//pCadr->SetB(CurPoint);
		//CurPoint = EndPoint;
		//pCadr->SetE(CurPoint);
		//pCont->Add(pCadr);
	}

	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(fast);
	pCadr->SetB(StartPoint.GetX(), StartPoint.GetY(), pCont->GetAt(0)->GetB().GetZ());
	pCadr->SetE(StartPoint);

	delete pCont->GetAt(0);
	pCont->SetAt(0, pCadr);

	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(fast);
	pCadr->SetB(CurPoint);
	CurPoint = BPoint(CurPoint.GetX(), CurPoint.GetY(), EndPoint.GetZ(), 1.);
	pCadr->SetE(CurPoint);
	pCont->Add(pCadr);

	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(fast);
	pCadr->SetB(CurPoint);
	pCadr->SetE(EndPoint);
	pCont->Add(pCadr);
	return 0;
}
int NMillRectOutCycle::MakeLayerRough(BPoint &CurPoint, const BPoint &EndPoint, int Npas)
{
	double SideStep = XYdist;
	NCadrGeom *pCadr;
	double CornerRCon = CornerR;
	if(CornerR != 0)
		CornerRCon += fabs(DXrough - DXfinish) / 2. + Npas * SideStep;
	for(int i = 0; i < Npas; ++i)
	{
		if(CornerRCon != 0)
			CornerRCon -= SideStep;
		MakeOnePassRough(CurPoint, EX * (DXrough + 2. * (Npas - (i + 1)) * SideStep)
			, EY * (DYrough + 2. * (Npas - (i + 1)) * SideStep)
			, CornerRCon);

		if(i < Npas - 1)
		{
			pCadr = new NCadrGeom;
			pCadr->SetAttr(1);
			pCadr->SetType(line);
			pCadr->SetB(CurPoint);
			CurPoint = CurPoint - (CCWDir ? EX : EY) * (CornerR + XYclr - XYdist);
			pCadr->SetE(CurPoint);
			pCont->Add(pCadr);

			pCadr = new NCadrGeom;
			pCadr->SetAttr(1);
			pCadr->SetType(line);
			pCadr->SetB(CurPoint);
			CurPoint = CurPoint + (CCWDir ? EY : EX) * (XYdist + XYclr);
			pCadr->SetE(CurPoint);
			pCont->Add(pCadr);
		}
	}
	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(fast);
	pCadr->SetB(CurPoint);
	CurPoint = EndPoint;
	pCadr->SetE(CurPoint);
	pCont->Add(pCadr);
	return 0;
}

int NMillRectOutCycle::MakeLayerFinSide(BPoint &CurPoint)
{
	MakeOnePassRough(CurPoint, EX * DXfinish, EY * DYfinish, CornerR);

	double D = (DXrough - DXfinish) / 2.;
	// Change last cadr
	NCadrGeom *pCadr;
	pCadr = pCont->GetAt(pCont->GetSize() - 1);
	pCadr->SetType(CCWDir ? cwarc : ccwarc);
	CurPoint = pCadr->GetB() + (CCWDir ? BPoint(0., -D, 0., 0.) : BPoint(-D, 0., 0., 0.));
	pCadr->SetE(CurPoint);
	pCadr->SetI((CCWDir ? BPoint(0., -D / 2., 0., 0.) : BPoint(-D / 2, 0., 0., 0.)));

	return 0;
}
int NMillRectOutCycle::MakeOnePassRough(BPoint &CurPoint, const BPoint & XDir, const BPoint & YDir, double CornerR)
{
	// XDir and YDir have full motion length

	BPoint EX = XDir * (1. / sqrt(XDir.D2()));
	BPoint EY = YDir * (1. / sqrt(YDir.D2()));
	BPoint PX = XDir - EX * (2. * CornerR);
	BPoint PY = YDir - EY * (2. * CornerR);

	BPoint EndPoint = CurPoint + (CCWDir ? EY : EX) * XYdist;

	NCadrGeom *pCadr;
	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(line);
	pCadr->SetB(CurPoint);
	CurPoint = BPoint(Center.GetX(), Center.GetY(), CurPoint.GetZ(), 1.) + (CCWDir ? (PX - PY) * 0.5 - EY * CornerR : (PY - PX) * 0.5 - EX * CornerR);
	pCadr->SetE(CurPoint);
	pCont->Add(pCadr);

	if(CornerR > 0.)
	{
		pCadr = new NCadrGeom;
		pCadr->SetAttr(1);
		pCadr->SetType(ArcType);
		pCadr->SetB(CurPoint);
		CurPoint = CurPoint + (EX + EY) * CornerR;
		pCadr->SetE(CurPoint);
		pCadr->SetI((CCWDir ? EY : EX) * CornerR);
		pCont->Add(pCadr);
	}

	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(line);
	pCadr->SetB(CurPoint);
	CurPoint = CurPoint + (CCWDir ? PY : PX);
	pCadr->SetE(CurPoint);
	pCont->Add(pCadr);

	if(CornerR > 0.)
	{
		pCadr = new NCadrGeom;
		pCadr->SetAttr(1);
		pCadr->SetType(ArcType);
		pCadr->SetB(CurPoint);
		CurPoint = CurPoint + (CCWDir ? (EY - EX) : (EX - EY)) * CornerR;
		pCadr->SetE(CurPoint);
		pCadr->SetI((CCWDir ? EX : EY) * (-CornerR));
		pCont->Add(pCadr);
	}

	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(line);
	pCadr->SetB(CurPoint);
	CurPoint = CurPoint + (CCWDir ? PX : PY) * (-1.);
	pCadr->SetE(CurPoint);
	pCont->Add(pCadr);

	if(CornerR > 0.)
	{
		pCadr = new NCadrGeom;
		pCadr->SetAttr(1);
		pCadr->SetType(ArcType);
		pCadr->SetB(CurPoint);
		CurPoint = CurPoint + (EY + EX) * (-CornerR);
		pCadr->SetE(CurPoint);
		pCadr->SetI((CCWDir ? EY : EX) * (-CornerR));
		pCont->Add(pCadr);
	}

	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(line);
	pCadr->SetB(CurPoint);
	CurPoint = CurPoint + (CCWDir ? PY : PX) * (-1.);
	pCadr->SetE(CurPoint);
	pCont->Add(pCadr);

	if(CornerR > 0.)
	{
		pCadr = new NCadrGeom;
		pCadr->SetAttr(1);
		pCadr->SetType(ArcType);
		pCadr->SetB(CurPoint);
		CurPoint = CurPoint + (CCWDir ? (EX - EY) : (EY - EX)) * CornerR;
		pCadr->SetE(CurPoint);
		pCadr->SetI((CCWDir ? EX : EY) * CornerR);
		pCont->Add(pCadr);
	}

	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(line);
	pCadr->SetB(CurPoint);
	CurPoint = CurPoint - (CCWDir ? EY : EX) * XYclr;
	pCadr->SetE(CurPoint);
	pCont->Add(pCadr);

	return 0;
}


const BPoint & NMillRectOutCycle::GetStartPoint(void)
{
	return StartPoint;
}
