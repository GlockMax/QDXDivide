#include "stdafx.h"
#include "math.h"
#include "constdef.h"
#include "BMatr.h"
#include "NCadrGeom.h"
#include "NMillRectInCycle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

NMillRectInCycle::NMillRectInCycle(BGeomArray &Cont
		, const	BPoint &StartPoint1I
		, const	BPoint &StartPoint2I
		, const	BPoint &CenterI
		, const	BPoint &EndPointI 
		, const	BPoint &EXI // единичный вектор длинной стороны
		, const BPoint &EYI // единичный вектор короткой стороны
		, double DXroughI
		, double DYroughI
		, double DXfinishI
		, double DYfinishI
		, bool CCWDirI
		, bool NeedApprI
		, bool NeedRoughI
		, bool NeedFinishI
		, double XYdistI
		, double ZdistRoI
		, double ZdistFiI
		, double DZdownRoI
		, double DZdownFiI
		, double CornerRI
		, double ApprAngleI
		, int ApprTypeI // 1 - по слоям, 2 - под заданным углом, 3 - по квадрату, 4 - вертикально
		, double DZupI)
		: 	StartPoint1(StartPoint1I)
		, StartPoint2(StartPoint2I), Center(CenterI), EndPoint(EndPointI), EX(EXI), EY(EYI)

{
	pCont = &Cont;
	DXrough = DXroughI;
	DYrough = DYroughI;
	DXfinish = DXfinishI;
	DYfinish = DYfinishI;
	CCWDir = CCWDirI;
	NeedAppr = NeedApprI;
	NeedRough = NeedRoughI;
	NeedFinish = NeedFinishI;
	XYdist = XYdistI;
	ZdistRo = ZdistRoI;
	ZdistFi = ZdistFiI;
	DZdownRo = DZdownRoI;
	DZdownFi = DZdownFiI;
	CornerR = CornerRI;
	ApprAngle = ApprAngleI;
	ApprType = ApprTypeI;
	DZup = DZupI;
	bOK = true;
	ArcType = CCWDir ? ccwarc : cwarc;

	if(MakePath() != 0)
	{
		bOK = false;
		Cont.RemoveAll();
	}
}

NMillRectInCycle::~NMillRectInCycle(void)
{
}
bool NMillRectInCycle::IsOK()
{
	return bOK;
}
int NMillRectInCycle::MakePath(void)
{

	if(pCont->GetSize() != 1)
		return -1;

	int NLayers = int(ZdistRo / DZdownRo);
	if(NLayers * DZdownRo < ZdistRo - 1.e-6)
		++NLayers;
	DZdownRo = ZdistRo / NLayers;


	StartPoint = StartPoint2;
	if(ApprType == 2)
	{
		if(ApprAngle <= 1.e-8)
			return -1;
		double D = sqrt((StartPoint1 - StartPoint2).D2());
		if(D < 1.e-8)
			return -1;
		double H = D * tan(ApprAngle * PI / 180.);
		int NApprSteps = int(DZdownRo / H);
		if(DZdownRo - NApprSteps * H < 1.e-6 || NApprSteps <= 0)
			++NApprSteps;
		H = DZdownRo / NApprSteps;
		if(NApprSteps % 2 != 0)
			StartPoint = StartPoint1;
	}
	BPoint CurPoint = StartPoint;
	if(NeedRough)
	{
		for(int i = 0; i < NLayers; ++i)
		{
			bool LastLayer = (i == NLayers - 1);
			bool FirstLayer = (i == 0);
			double DownStep = FirstLayer ? DZdownRo : DZdownRo + (NeedRough ? DZup : 0);
			double UpStep = DZup;
			if(NeedAppr)
			{
				MakeLayerAppr(CurPoint, DownStep);
			}
			else
			{
				NCadrGeom *pCadr;
				pCadr = new NCadrGeom;
				pCadr->SetAttr(1);
				pCadr->SetType(fast);
				pCadr->SetB(CurPoint);
				pCadr->SetE(StartPoint2.GetX(), StartPoint2.GetY(), CurPoint.GetZ());
				pCont->Add(pCadr);
				pCadr = new NCadrGeom;
				pCadr->SetAttr(1);
				pCadr->SetType(line);
				pCadr->SetB(StartPoint2.GetX(), StartPoint2.GetY(),CurPoint.GetZ());
				CurPoint = BPoint(StartPoint2.GetX(), StartPoint2.GetY(), CurPoint.GetZ() - DownStep, 1.);
				pCadr->SetE(CurPoint);
				pCont->Add(pCadr);
			}
			if(NeedRough)
			{
				BPoint EndPoint(StartPoint.GetX(), StartPoint.GetY(), CurPoint.GetZ() + UpStep, 1.);
				MakeLayerRough(CurPoint, EndPoint);
			}
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
			{
				MakeLayerAppr(CurPoint, DownStep);
			}
			else
			{
				NCadrGeom *pCadr;
				pCadr = new NCadrGeom;
				pCadr->SetAttr(1);
				pCadr->SetType(fast);
				pCadr->SetB(CurPoint);
				pCadr->SetE(StartPoint2.GetX(), StartPoint2.GetY(), CurPoint.GetZ());
				pCont->Add(pCadr);
				pCadr = new NCadrGeom;
				pCadr->SetAttr(1);
				pCadr->SetType(line);
				pCadr->SetB(StartPoint2.GetX(), StartPoint2.GetY(), StartPoint.GetZ());
				CurPoint = BPoint(StartPoint2.GetX(), StartPoint2.GetY(), StartPoint2.GetZ() - DownStep, 1.);
				pCadr->SetE(CurPoint);
				pCont->Add(pCadr);
			}
			BPoint EndPoint(StartPoint.GetX(), StartPoint.GetY(), CurPoint.GetZ() + DZup, 1.);
			MakeLayerRough(CurPoint, EndPoint);	
		}
		// Change last point
		NCadrGeom *pCadr;
		pCadr = pCont->GetAt(pCont->GetSize() - 1);
		pCadr->SetType(line);
		CurPoint = pCadr->GetE() - EX * ((DXfinish - DXrough) * 0.5);
		pCadr->SetE(CurPoint);


		NLayers = int(ZdistFi / DZdownFi);
		if(NLayers * DZdownFi < ZdistFi - 1.e-6)
			++NLayers;
		DownStep = ZdistFi / NLayers;
		BPoint FinStartPoint = Center + EX * (DXrough / 2.) + EY * ((DYrough / 2. - CornerR) * (CCWDir ? 1. : -1.));
		FinStartPoint = BPoint(FinStartPoint.GetX(), FinStartPoint.GetY(), Center.GetZ() - DownStep, 1.);

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
			if(MakeLayerFinSide(CurPoint) < 0)
				return -1;
		}
		pCadr = new NCadrGeom;
		pCadr->SetAttr(1);
		pCadr->SetType(fast);
		pCadr->SetB(CurPoint);
		CurPoint = EndPoint;
		pCadr->SetE(CurPoint);
		pCont->Add(pCadr);

	}
	else
	{
		// Change last point
		NCadrGeom *pCadr;
		pCadr = pCont->GetAt(pCont->GetSize() - 1);
		pCadr->SetType(fast);
		CurPoint = BPoint(CurPoint.GetX(), CurPoint.GetY(), EndPoint.GetZ(), 1.);
		pCadr->SetE(CurPoint);

		pCadr = new NCadrGeom;
		pCadr->SetAttr(1);
		pCadr->SetType(fast);
		pCadr->SetB(CurPoint);
		CurPoint = EndPoint;
		pCadr->SetE(CurPoint);
		pCont->Add(pCadr);

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
int NMillRectInCycle::MakeLayerAppr(BPoint &CurPoint, double DownStep)
{
	NCadrGeom *pCadr;
	switch(ApprType)// 1 - по слоям, 2 - под заданным углом, 3 - по квадрату, 4 - вертикально
	{
	case 1:
		pCadr = new NCadrGeom;
		pCadr->SetAttr(1);
		pCadr->SetType(line);
		pCadr->SetB(CurPoint);
		CurPoint = BPoint(StartPoint1.GetX(), StartPoint1.GetY(), CurPoint.GetZ() - DownStep);
		pCadr->SetE(CurPoint);
		pCont->Add(pCadr);
		pCadr = new NCadrGeom;
		pCadr->SetAttr(1);
		pCadr->SetType(line);
		pCadr->SetB(CurPoint);
		CurPoint = BPoint(StartPoint2.GetX(), StartPoint2.GetY(), CurPoint.GetZ(), 1.);
		pCadr->SetE(CurPoint);
		pCont->Add(pCadr);
		break;
	case 2:
		{
			if(ApprAngle <= 1.e-8)
				return -1;
			double D = sqrt((StartPoint1 - StartPoint2).D2());
			if(D < 1.e-8)
				return -1;
			double H = D * tan(ApprAngle * PI / 180.);
			int NApprSteps = int(DZdownRo / H);
			if(DZdownRo - NApprSteps * H < 1.e-6 || NApprSteps <= 0)
				++NApprSteps;
			H = DownStep / NApprSteps;
			if(NApprSteps % 2 != 0)
			{
				pCadr = new NCadrGeom;
				pCadr->SetAttr(1);
				pCadr->SetType(line);
				pCadr->SetB(CurPoint);
				CurPoint = BPoint(StartPoint2.GetX(), StartPoint2.GetY(), CurPoint.GetZ() - H, 1.);
				pCadr->SetE(CurPoint);
				pCont->Add(pCadr);
				--NApprSteps;
			}
			for(int i = 0; i < NApprSteps; i = i + 2)
			{
				pCadr = new NCadrGeom;
				pCadr->SetAttr(1);
				pCadr->SetType(line);
				pCadr->SetB(CurPoint);
				CurPoint = CurPoint + (StartPoint1 - StartPoint2) - BPoint(0., 0., H, 0.);
				pCadr->SetE(CurPoint);
				pCont->Add(pCadr);
				pCadr = new NCadrGeom;
				pCadr->SetAttr(1);
				pCadr->SetType(line);
				pCadr->SetB(CurPoint);
				CurPoint = CurPoint + (StartPoint2 - StartPoint1) - BPoint(0., 0., H, 0.);
				pCadr->SetE(CurPoint);
				pCont->Add(pCadr);
			}
			pCadr = new NCadrGeom;
			pCadr->SetAttr(1);
			pCadr->SetType(line);
			pCadr->SetB(CurPoint);
			CurPoint = BPoint(StartPoint2.GetX(), StartPoint2.GetY(), CurPoint.GetZ(), 1.);
			pCadr->SetE(CurPoint);
			pCont->Add(pCadr);
		}
		break;
	case 3:
		{
			double ED = sqrt((StartPoint1 - StartPoint2).D2());
			pCadr = new NCadrGeom;
			pCadr->SetAttr(1);
			pCadr->SetType(line);
			pCadr->SetB(CurPoint);
			CurPoint = CurPoint + BPoint(0., ED / 2., -DownStep / 8., 0.);
			pCadr->SetE(CurPoint);
			pCont->Add(pCadr);

			pCadr = new NCadrGeom;
			pCadr->SetAttr(1);
			pCadr->SetType(line);
			pCadr->SetB(CurPoint);
			CurPoint = CurPoint + BPoint(-ED, 0., -DownStep / 4., 0.);
			pCadr->SetE(CurPoint);
			pCont->Add(pCadr);

			pCadr = new NCadrGeom;
			pCadr->SetAttr(1);
			pCadr->SetType(line);
			pCadr->SetB(CurPoint);
			CurPoint = CurPoint + BPoint(0., -ED, -DownStep / 4., 0.);
			pCadr->SetE(CurPoint);
			pCont->Add(pCadr);

			pCadr = new NCadrGeom;
			pCadr->SetAttr(1);
			pCadr->SetType(line);
			pCadr->SetB(CurPoint);
			CurPoint = CurPoint + BPoint(ED, 0., -DownStep / 4., 0.);
			pCadr->SetE(CurPoint);
			pCont->Add(pCadr);

			pCadr = new NCadrGeom;
			pCadr->SetAttr(1);
			pCadr->SetType(line);
			pCadr->SetB(CurPoint);
			CurPoint = CurPoint + BPoint(0., ED / 2., -DownStep / 8., 0.);
			pCadr->SetE(CurPoint);
			pCont->Add(pCadr);
		}
		break;
	case 4:
	default:
		pCadr = new NCadrGeom;
		pCadr->SetAttr(1);
		pCadr->SetType(line);
		pCadr->SetB(CurPoint);
		CurPoint = BPoint(StartPoint2.GetX(), StartPoint2.GetY(), CurPoint.GetZ() - DownStep, 1.);
		pCadr->SetE(CurPoint);
		pCont->Add(pCadr);

		pCadr = new NCadrGeom;
		pCadr->SetAttr(1);
		pCadr->SetType(line);
		pCadr->SetB(CurPoint);
		CurPoint = CurPoint + (StartPoint1 - StartPoint2);
		pCadr->SetE(CurPoint);
		pCont->Add(pCadr);

		pCadr = new NCadrGeom;
		pCadr->SetAttr(1);
		pCadr->SetType(line);
		pCadr->SetB(CurPoint);
		CurPoint = CurPoint + (StartPoint2 - StartPoint1);
		pCadr->SetE(CurPoint);
		pCont->Add(pCadr);
		break;
	}
	return 0;
}
int NMillRectInCycle::MakeLayerRough(BPoint &CurPoint, const BPoint &EndPoint)
{
	double Dist2 = sqrt(((StartPoint2 - Center).D2()));
	double Width = DXrough / 2. - Dist2;
	int Npas = int(Width / fabs(XYdist));
	if(fabs(Npas * XYdist - Width) > 1.e-6)
		++Npas;
	double SideStep = Width / Npas;
	NCadrGeom *pCadr;
	double CornerRCon = CornerR;
	if(CornerR != 0)
		CornerRCon -= (DXfinish - DXrough) / 2.;
	for(int i = 0; i < Npas; ++i)
	{
		MakeOnePassRough(CurPoint, EX * (2. * (Dist2 + (i + 1) * SideStep))
			, EY * (DYrough - 2. * Width + 2. * (i + 1) * SideStep)
			, SideStep, CornerRCon - (Npas - i - 1) * SideStep, CCWDir);
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

int NMillRectInCycle::MakeLayerFinSide(BPoint &CurPoint)
{
	double Width = DYfinish;
	if(Width  + 1.e-12 < CornerR * 2. || CornerR < 0.)
		return -1;

	double D = (DXfinish - DXrough) / 2.;

	NCadrGeom *pCadr;

	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(ArcType);
	pCadr->SetB(CurPoint);
	CurPoint = CurPoint + EX * D + EY * (CCWDir ? D : -D);
	pCadr->SetE(CurPoint );
	pCadr->SetI(EY * (CCWDir ? D : -D));
	pCont->Add(pCadr);

	if(CornerR > 0.)
	{
		pCadr = new NCadrGeom;
		pCadr->SetAttr(1);
		pCadr->SetType(ArcType);
		pCadr->SetB(CurPoint);
		CurPoint = CurPoint  + EX * (-CornerR) + EY * (CCWDir ? CornerR : -CornerR);
		pCadr->SetE(CurPoint );
		pCadr->SetI(EX * (-CornerR));
		pCont->Add(pCadr);
	}

	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(line);
	pCadr->SetB(CurPoint);
	CurPoint = CurPoint - EX * (DXfinish - 2. * CornerR);
	pCadr->SetE(CurPoint );
	pCont->Add(pCadr);

	if(CornerR > 0.)
	{
		pCadr = new NCadrGeom;
		pCadr->SetAttr(1);
		pCadr->SetType(ArcType);
		pCadr->SetB(CurPoint);
		CurPoint = CurPoint  + EX * (-CornerR) + EY * (CCWDir ? -CornerR :  CornerR);
		pCadr->SetE(CurPoint );
		pCadr->SetI(EY * (CCWDir ? -CornerR :  CornerR));
		pCont->Add(pCadr);
	}

	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(line);
	pCadr->SetB(CurPoint);
	CurPoint = CurPoint + EY * ((DYfinish - 2. * CornerR) * (CCWDir ? -1. : 1.));
	pCadr->SetE(CurPoint );
	pCont->Add(pCadr);

	if(CornerR > 0.)
	{
		pCadr = new NCadrGeom;
		pCadr->SetAttr(1);
		pCadr->SetType(ArcType);
		pCadr->SetB(CurPoint);
		CurPoint = CurPoint  + EX * (CornerR) + EY * (CCWDir ? -CornerR :  CornerR);
		pCadr->SetE(CurPoint );
		pCadr->SetI(EX * CornerR);
		pCont->Add(pCadr);
	}

	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(line);
	pCadr->SetB(CurPoint);
	CurPoint = CurPoint + EX * (DXfinish - 2. * CornerR);
	pCadr->SetE(CurPoint );
	pCont->Add(pCadr);

	if(CornerR > 0.)
	{
		pCadr = new NCadrGeom;
		pCadr->SetAttr(1);
		pCadr->SetType(ArcType);
		pCadr->SetB(CurPoint);
		CurPoint = CurPoint  + EX * (CornerR) + EY * (CCWDir ? CornerR : -CornerR);
		pCadr->SetE(CurPoint );
		pCadr->SetI(EY * (CCWDir ? CornerR :  -CornerR));
		pCont->Add(pCadr);
	}

	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(line);
	pCadr->SetB(CurPoint);
	CurPoint = CurPoint - EY * ((DYfinish - 2. * CornerR) * (CCWDir ? -1. : 1.));
	pCadr->SetE(CurPoint );
	pCont->Add(pCadr);

	if(CornerR > 0.)
	{
		pCadr = new NCadrGeom;
		pCadr->SetAttr(1);
		pCadr->SetType(ArcType);
		pCadr->SetB(CurPoint);
		CurPoint = CurPoint  + EX * (-CornerR) + EY * (CCWDir ? CornerR : -CornerR);
		pCadr->SetE(CurPoint );
		pCadr->SetI(EX * (-CornerR));
		pCont->Add(pCadr);
	}
	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(ArcType);
	pCadr->SetB(CurPoint);
	CurPoint = CurPoint - EX * D - EY * (CCWDir ? D : -D);
	pCadr->SetE(CurPoint );
	pCadr->SetI(EY * (CCWDir ? -D : D));
	pCont->Add(pCadr);

	return 0;
}
int NMillRectInCycle::MakeOnePassRough(BPoint &CurPoint, const BPoint & XDir, const BPoint & YDir, double DAppr, double CornerR, bool CCW)
{
	// XDir and YDir have full motion length
	double Width = sqrt(YDir.D2());
	if(Width  + 1.e-12< CornerR * 2. || CornerR < 0.)
		CornerR = 0.;

	BPoint EX = XDir * (1. / sqrt(XDir.D2()));
	BPoint EY = YDir * (1. / Width);
	BPoint PX = XDir - EX * (2. * CornerR);
	BPoint PY = EY * ((CCW ? 1. : -1.) * (Width - 2. * CornerR));

	NCadrGeom *pCadr;
	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(line);
	pCadr->SetB(CurPoint);
	CurPoint = CurPoint + EX * DAppr;
	pCadr->SetE(CurPoint);
	pCont->Add(pCadr);


	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(line);
	pCadr->SetB(CurPoint);
	CurPoint = CurPoint + PY * 0.5;
	pCadr->SetE(CurPoint);
	pCont->Add(pCadr);

	if(CornerR > 0.)
	{
		pCadr = new NCadrGeom;
		pCadr->SetAttr(1);
		pCadr->SetType(CCW ? ccwarc : cwarc);
		pCadr->SetB(CurPoint);
		CurPoint = CurPoint - EX * CornerR + EY * ((CCW ? 1. : -1.) * CornerR) ;
		pCadr->SetE(CurPoint);
		pCadr->SetI(EX * (-CornerR));
		pCont->Add(pCadr);
	}

	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(line);
	pCadr->SetB(CurPoint);
	CurPoint = CurPoint - PX;
	pCadr->SetE(CurPoint);
	pCont->Add(pCadr);

	if(CornerR > 0.)
	{
		pCadr = new NCadrGeom;
		pCadr->SetAttr(1);
		pCadr->SetType(CCW ? ccwarc : cwarc);
		pCadr->SetB(CurPoint);
		CurPoint = CurPoint - EX * CornerR + EY * ( - (CCW ? 1. : -1.) * CornerR) ;
		pCadr->SetE(CurPoint);
		pCadr->SetI(EY * ((CCW ? -1. : 1.) * CornerR));
		pCont->Add(pCadr);
	}

	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(line);
	pCadr->SetB(CurPoint);
	CurPoint = CurPoint - PY;
	pCadr->SetE(CurPoint);
	pCont->Add(pCadr);

	if(CornerR > 0.)
	{
		pCadr = new NCadrGeom;
		pCadr->SetAttr(1);
		pCadr->SetType(CCW ? ccwarc : cwarc);
		pCadr->SetB(CurPoint);
		CurPoint = CurPoint + EX * CornerR + EY * ( - (CCW ? 1. : -1.) * CornerR) ;
		pCadr->SetE(CurPoint);
		pCadr->SetI(EX * (+CornerR));
		pCont->Add(pCadr);
	}

	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(line);
	pCadr->SetB(CurPoint);
	CurPoint = CurPoint + PX;
	pCadr->SetE(CurPoint);
	pCont->Add(pCadr);

	if(CornerR > 0.)
	{
		pCadr = new NCadrGeom;
		pCadr->SetAttr(1);
		pCadr->SetType(CCW ? ccwarc : cwarc);
		pCadr->SetB(CurPoint);
		CurPoint = CurPoint + EX * CornerR + EY * ((CCW ? 1. : -1.) * CornerR) ;
		pCadr->SetE(CurPoint);
		pCadr->SetI(EY * ((CCW ? 1. : -1.)* CornerR));
		pCont->Add(pCadr);
	}

	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(line);
	pCadr->SetB(CurPoint);
	CurPoint = CurPoint + PY * 0.5;
	pCadr->SetE(CurPoint);
	pCont->Add(pCadr);

	return 0;
}

