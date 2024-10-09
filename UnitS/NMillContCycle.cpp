#include "stdafx.h"
#include "math.h"
#include "constdef.h"
#include "BMatr.h"
#include "NCadrGeom.h"
#include "NMillContCycle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

NMillContCycle::NMillContCycle(BGeomArray &Cont, BGeomArray &CompCont
		, BGeomArray &ContProg, BGeomArray &ContComp, BGeomArray &ContRough
		, const	BPoint &StartPointI
		, const	BPoint &EndPointI 
		, bool NeedRoughI
		, bool NeedFinishI
		, double CompI
		, double ZdistRoI
		, double ZdistFiI
		, double DZdownRoI
		, double DZdownFiI
		, double DistI
		, enum LR DirCompI
		, bool OneDirCutI)
{
	pCont = &Cont;
	pCompCont = &CompCont;
	pContProg = &ContProg;
	pContComp = &ContComp;
	pContRough = &ContRough;
	StartPoint = StartPointI;
	EndPoint = EndPointI;
	NeedRough = NeedRoughI;
	NeedFinish = NeedFinishI;
	Comp = CompI;
	ZdistRo = ZdistRoI;
	ZdistFi = ZdistFiI;
	DZdownRo = DZdownRoI;
	DZdownFi = DZdownFiI;
	DistI = DistI;
	DirComp = DirCompI;
	OneDirCut = OneDirCutI;
	bOK = true;

	pCont->RemoveAll();
	pCompCont->RemoveAll();

	SubCont.Append(*pContProg);
	pContProg->SetSize(0);
	CompSubCont.Append(*pContRough);
	pContRough->SetSize(0);

	if(SubCont.GetSize() < 1)
	{
		bOK = false;
		return;
	}

	SubCont[0]->SetB(StartPoint);
	for(int i = 0; i < SubCont.GetSize(); ++i)
		SubCont[i]->ProjXY();
	for(int i = 0; i < SubCont.GetSize(); ++i)
		CompSubCont[i]->ProjXY();

	if(bOK)
		if(MakePath() != 0)
		{
			bOK = false;
			Cont.RemoveAll();
		}
	SubCont.RemoveAll();

}

NMillContCycle::~NMillContCycle(void)
{
}
bool NMillContCycle::IsOK()
{
	return bOK;
}
bool NMillContCycle::MakeCompSubCont(double Dist)
{
	CompSubCont.RemoveAll();
	for(int i = 0; i < SubCont.GetSize(); ++i)
		CompSubCont.AddElement(*SubCont[i]);
	return true;
}
int NMillContCycle::MakePath(void)
{
	BPoint CurPoint = StartPoint;


	NCadrGeom *pCadr;
	if(NeedRough)
	{
		//if(!MakeCompSubCont(Comp + Dist))
		//	return false;

		int NLayers = int(ZdistRo / DZdownRo);
		if(fabs(NLayers * DZdownRo) < fabs(ZdistRo) - 1.e-6)
			++NLayers;
		NLayers = max(NLayers, 1);

		for(int i = 0; i < NLayers; ++i)
		{
			bool LastLayer = (i == NLayers - 1);
			bool FirstLayer = (i == 0);
			double DownStep = StartPoint.GetZ() + DZdownRo * ( i + 1 );
			if(LastLayer)
				DownStep = StartPoint.GetZ() + ZdistRo;
			double UpStep = EndPoint.GetZ() - DownStep;
			DownStep -= CurPoint.GetZ();

			MakeLayerRough(CurPoint, DownStep, UpStep, i%2 == 0);
		}
		CurPoint = pCont->GetAt(pCont->GetSize() - 1)->GetE();
		
		pCadr = new NCadrGeom;
		pCadr->SetAttr(1);
		pCadr->SetType(line);
		pCadr->SetB(CurPoint);
		CurPoint.SetZ(EndPoint.GetZ());
		pCadr->SetE(CurPoint);
		pCont->Add(pCadr);

		pCadr = new NCadrGeom;
		pCadr->SetAttr(1);
		pCadr->SetType(line);
		pCadr->SetB(CurPoint);
		CurPoint = EndPoint;
		pCadr->SetE(CurPoint);
		pCont->Add(pCadr);

		CurPoint = pCompCont->GetAt(pCompCont->GetSize() - 1)->GetE();
		pCadr = new NCadrGeom;
		pCadr->SetAttr(1);
		pCadr->SetType(line);
		pCadr->SetB(CurPoint);
		CurPoint.SetZ(EndPoint.GetZ());
		pCadr->SetE(CurPoint);
		pCompCont->Add(pCadr);

		pCadr = new NCadrGeom;
		pCadr->SetAttr(1);
		pCadr->SetType(line);
		pCadr->SetB(CurPoint);
		CurPoint = EndPoint;
		pCadr->SetE(CurPoint);
		pCompCont->Add(pCadr);
	}

	if(NeedFinish)
	{
		CompSubCont.RemoveAll();
		CompSubCont.Append(*pContComp);
		pContComp->SetSize(0);
		for(int i = 0; i < SubCont.GetSize(); ++i)
			CompSubCont[i]->ProjXY();

		int NLayers = int(ZdistFi / DZdownFi);
		if(fabs(NLayers * DZdownFi) < fabs(ZdistFi) - 1.e-6)
			++NLayers;
		NLayers = max(NLayers, 1);

		for(int i = 0; i < NLayers; ++i)
		{
			bool LastLayer = (i == NLayers - 1);
			bool FirstLayer = (i == 0);
			double DownStep = StartPoint.GetZ() + DZdownFi * ( i + 1 );
			if(LastLayer)
				DownStep = StartPoint.GetZ() + ZdistFi;
			double UpStep = EndPoint.GetZ() - DownStep;
			DownStep -= CurPoint.GetZ();

			MakeLayerRough(CurPoint, DownStep, UpStep, i%2 == 0);
		}
		CurPoint = pCont->GetAt(pCont->GetSize() - 1)->GetE();
	
		pCadr = new NCadrGeom;
		pCadr->SetAttr(1);
		pCadr->SetType(line);
		pCadr->SetB(CurPoint);
		CurPoint.SetZ(EndPoint.GetZ());
		pCadr->SetE(CurPoint);
		pCont->Add(pCadr);

		pCadr = new NCadrGeom;
		pCadr->SetAttr(1);
		pCadr->SetType(line);
		pCadr->SetB(CurPoint);
		CurPoint = EndPoint;
		pCadr->SetE(CurPoint);
		pCont->Add(pCadr);

		CurPoint = pCompCont->GetAt(pCompCont->GetSize() - 1)->GetE();
		pCadr = new NCadrGeom;
		pCadr->SetAttr(1);
		pCadr->SetType(line);
		pCadr->SetB(CurPoint);
		CurPoint.SetZ(EndPoint.GetZ());
		pCadr->SetE(CurPoint);
		pCompCont->Add(pCadr);

		pCadr = new NCadrGeom;
		pCadr->SetAttr(1);
		pCadr->SetType(line);
		pCadr->SetB(CurPoint);
		CurPoint = EndPoint;
		pCadr->SetE(CurPoint);
		pCompCont->Add(pCadr);

	}

	return 0;
}
int NMillContCycle::MakeLayerRough(BPoint &CurPoint, double DownStep, double UpStep, bool Forward)
{
	if(Forward || OneDirCut)
	{
		NCadrGeom *pCadr;
		pCadr = new NCadrGeom;
		pCadr->SetAttr(1);
		pCadr->SetType(line);
		pCadr->SetB(CurPoint);
		CurPoint = CurPoint + BPoint(0., 0., DownStep, 0.);
		pCadr->SetE(CurPoint);
		pCont->Add(pCadr);
		pCompCont->AddElement(*pCadr);

		for(int l = 0; l < SubCont.GetSize(); ++l)
		{
			pCadr = new NCadrGeom;
			*pCadr = *SubCont[l];
			pCadr->ShiftZ(CurPoint.GetZ());
			pCont->Add(pCadr);

			pCadr = new NCadrGeom;
			*pCadr = *CompSubCont[l];
			pCadr->ShiftZ(CurPoint.GetZ());
			pCompCont->Add(pCadr);
		}
		CurPoint = pCont->GetAt(pCont->GetSize() - 1)->GetE();
//		pCompCont->GetAt(pCompCont->GetSize() - 1)->SetE(CurPoint);
		if(OneDirCut)
		{
			pCadr = new NCadrGeom;
			pCadr->SetAttr(1);
			pCadr->SetType(line);
			pCadr->SetB(CurPoint);
			CurPoint = CurPoint + BPoint(0., 0., UpStep, 0.);
			pCadr->SetE(CurPoint);
			pCont->Add(pCadr);

			pCadr = new NCadrGeom;
			pCadr->SetAttr(1);
			pCadr->SetType(line);
			pCadr->SetB(CurPoint);
			CurPoint = EndPoint;
			pCadr->SetE(CurPoint);
			pCont->Add(pCadr);

			CurPoint = pCompCont->GetAt(pCompCont->GetSize() - 1)->GetE();
			pCadr = new NCadrGeom;
			pCadr->SetAttr(1);
			pCadr->SetType(line);
			pCadr->SetB(CurPoint);
			CurPoint = CurPoint + BPoint(0., 0., UpStep, 0.);
			pCadr->SetE(CurPoint);
			pCompCont->Add(pCadr);

			pCadr = new NCadrGeom;
			pCadr->SetAttr(1);
			pCadr->SetType(line);
			pCadr->SetB(CurPoint);
			CurPoint = EndPoint;
			pCadr->SetE(CurPoint);
			pCompCont->Add(pCadr);
		}
	}
	else
	{// Backward
		NCadrGeom *pCadr;
		pCadr = new NCadrGeom;
		pCadr->SetAttr(1);
		pCadr->SetType(line);
		pCadr->SetB(CurPoint);
		CurPoint = CurPoint + BPoint(0., 0., DownStep, 0.);
		pCadr->SetE(CurPoint);
		pCont->Add(pCadr);
		pCompCont->AddElement(*pCadr);
		for(auto l = SubCont.GetSize() - 1; l >= 0; --l)
		{
			pCadr = new NCadrGeom;
			*pCadr = *SubCont[l];
			pCadr->ShiftZ(CurPoint.GetZ());
			pCadr->Reverse();
			pCont->Add(pCadr);

			pCadr = new NCadrGeom;
			*pCadr = *CompSubCont[l];
			pCadr->ShiftZ(CurPoint.GetZ());
			pCadr->Reverse();
			pCompCont->Add(pCadr);
		}
		CurPoint = pCont->GetAt(pCont->GetSize() - 1)->GetE();
	}

	return 0;
}

