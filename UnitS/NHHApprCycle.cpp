#include "stdafx.h"
#include "math.h"
#include "constdef.h"
#include "BMatr.h"
#include "NCadrGeom.h"
#include "NHHApprCycle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

NHHApprCycle::NHHApprCycle(BGeomArray &Cont, BGeomArray &CompCont
		, double LenI
		, double CcaI
		, double RI
		, double CompValI
		, bool LeftCompDirI
		, const NCadrGeom &NextGeomI
		, CycleType CTypeI)
	: NextGeom(NextGeomI)
{
	pCont = &Cont;
	pCompCont = &CompCont;
	Len = LenI;
	Cca = CcaI;
	R = RI;
	CompVal = CompValI;
	LeftCompDir = LeftCompDirI;
	CType = CTypeI;
	bOK = true;
	Error = 0;

	if(MakePath() != 0)
	{
		bOK = false;
		Cont.RemoveAll();
	}

}

NHHApprCycle::~NHHApprCycle(void)
{
}
bool NHHApprCycle::IsOK()
{
	return bOK;
}
int NHHApprCycle::MakePath(void)
{

	if(pCont->GetSize() != 1)
	{
		Error = 1087;// level 1
		return -1;
	}

	if(pCompCont->GetSize() != 1)
	{
		Error = 1087;// level 1
		return -1;
	}

	BPoint Dir = NextGeom.GetStartDir();
	Dir.ProjXY();
	double D = sqrt(Dir.D2());
	if(D < 1.e-6)
	{
		Error = 1106;// level 1
		return -1;
	}
	Dir = Dir * (1. / D);
	BPoint Norm = Dir * BMatr().RotZ(0., 0., 0., 90.);
	if(!LeftCompDir)
		Norm = Norm * (-1.);
	switch(CType)
	{
	case APPRLT:
		{
			Make1APPRLT(pCont, Dir, Norm, 0.);
			Make1APPRLT(pCompCont, Dir, Norm, CompVal);
		}
		break;
	case APPRLN:
		{
			Make1APPRLN(pCont, Dir, Norm, 0.);
			Make1APPRLN(pCompCont, Dir, Norm, CompVal);
		}
		break;
	case APPRCT:
		{
			Make1APPRCT(pCont, Dir, Norm, 0.);
			Make1APPRCT(pCompCont, Dir, Norm, CompVal);
		}
		break;
	case APPRLCT:
		{
			Make1APPRLCT(pCont, Dir, Norm, 0.);
			Make1APPRLCT(pCompCont, Dir, Norm, CompVal);
			if(pCont->IsEmpty() || pCompCont->IsEmpty())
			{
				Error = 1107;// level 1
				return -1;
			}
		}
		break;
	case DEPLT:
		{
			Make1APPRLT(pCont, Dir, Norm, 0.);
			Make1APPRLT(pCompCont, Dir, Norm, CompVal);
			if(pCont->GetSize() < 2 || pCompCont->GetSize() < 2)
			{
				Error = 1087;// level 1
				return -1;
			}
			delete pCont->GetAt(0);
			delete pCompCont->GetAt(0);
			pCont->RemoveAt(0);
			pCompCont->RemoveAt(0);
			pCont->Reverse();
			pCompCont->Reverse();
			pCont->GetAt(pCont->GetUpperBound())->SetE(pCompCont->GetAt(pCompCont->GetUpperBound())->GetE());
		}
		break;
	case DEPLN:
		{
			Make1APPRLN(pCont, Dir, Norm, 0.);
			Make1APPRLN(pCompCont, Dir, Norm, CompVal);
			if(pCont->GetSize() < 2 || pCompCont->GetSize() < 2)
			{
				Error = 1087;// level 1
				return -1;
			}
			delete pCont->GetAt(0);
			delete pCompCont->GetAt(0);
			pCont->RemoveAt(0);
			pCompCont->RemoveAt(0);
			pCont->Reverse();
			pCompCont->Reverse();
			pCont->GetAt(pCont->GetUpperBound())->SetE(pCompCont->GetAt(pCompCont->GetUpperBound())->GetE());
		}
		break;
	case DEPCT:
		{
			Make1APPRCT(pCont, Dir, Norm, 0.);
			Make1APPRCT(pCompCont, Dir, Norm, CompVal);
			if(pCont->GetSize() < 2 || pCompCont->GetSize() < 2)
			{
				Error = 1087;// level 1
				return -1;
			}
			pCont->Reverse();
			pCompCont->Reverse();
			pCont->GetAt(pCont->GetUpperBound())->SetE(pCompCont->GetAt(pCompCont->GetUpperBound())->GetE());
		}
		break;
	case DEPLCT:
		{
			Make1APPRLCT(pCont, Dir, Norm, 0.);
			Make1APPRLCT(pCompCont, Dir, Norm, CompVal);
			if(pCont->IsEmpty() || pCompCont->IsEmpty())
			{
				Error = 1107;// level 1
				return -1;
			}
			pCont->Reverse();
			pCompCont->Reverse();
		}
		break;
	}
	return 0;
}
void NHHApprCycle::Make1APPRLT(BGeomArray *pCont, const BPoint &Dir, const BPoint &Norm, double Comp)
{
	double dZ = pCont->GetAt(0)->GetE().GetZ() - pCont->GetAt(0)->GetB().GetZ();
	BPoint Ps(pCont->GetAt(0)->GetB());
	BPoint Pa(NextGeom.GetB() + Norm * Comp);
	BPoint Ph(Pa - Dir * Len - BPoint(0., 0., dZ, 0.));
	NCadrGeom::Curve Type = pCont->GetAt(0)->GetType();
	pCont->RemoveAll();
	NCadrGeom *pCadr;
	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(Type);
	pCadr->SetB(Ps);
	pCadr->SetE(Ph);
	pCont->Add(pCadr);

	if(dZ != 0.)
	{
		pCadr = new NCadrGeom;
		pCadr->SetAttr(1);
		pCadr->SetType(Type);
		pCadr->SetB(Ph);
		Ph = Ph + BPoint(0., 0., dZ, 0.);
		pCadr->SetE(Ph);
		pCont->Add(pCadr);
	}

	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(Type);
	pCadr->SetB(Ph);
	pCadr->SetE(Pa);
	pCont->Add(pCadr);
}

void NHHApprCycle::Make1APPRLN(BGeomArray *pCont, const BPoint &Dir, const BPoint &Norm, double Comp)
{
	double dZ = pCont->GetAt(0)->GetE().GetZ() - pCont->GetAt(0)->GetB().GetZ();
	BPoint Ps(pCont->GetAt(0)->GetB());
	BPoint Pa(NextGeom.GetB() + Norm * Comp);
	BPoint Ph(Pa + Norm * Len - BPoint(0., 0., dZ, 0.));
	NCadrGeom::Curve Type = pCont->GetAt(0)->GetType();
	pCont->RemoveAll();
	NCadrGeom *pCadr;
	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(Type);
	pCadr->SetB(Ps);
	pCadr->SetE(Ph);
	pCont->Add(pCadr);

	if(dZ != 0.)
	{
		pCadr = new NCadrGeom;
		pCadr->SetAttr(1);
		pCadr->SetType(Type);
		pCadr->SetB(Ph);
		Ph = Ph + BPoint(0., 0., dZ, 0.);
		pCadr->SetE(Ph);
		pCont->Add(pCadr);
	}

	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(Type);
	pCadr->SetB(Ph);
	pCadr->SetE(Pa);
	pCont->Add(pCadr);
}

void NHHApprCycle::Make1APPRCT(BGeomArray *pCont, const BPoint &Dir, const BPoint &Norm, double Comp)
{
	double dZ = pCont->GetAt(0)->GetE().GetZ() - pCont->GetAt(0)->GetB().GetZ();
	bool CCWDir = (Dir % Norm).GetZ() > 0.;
	BPoint Ps(pCont->GetAt(0)->GetB());
	BPoint Pa(NextGeom.GetB() + Norm * Comp);
	BPoint Center(Pa + Norm * R);
	BPoint Ph(Pa * BMatr().RotZ(Center, Cca * (CCWDir ? -1. : 1.)) - BPoint(0., 0., dZ, 0.));
	NCadrGeom::Curve Type = pCont->GetAt(0)->GetType();
	pCont->RemoveAll();

	NCadrGeom *pCadr;
	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(Type);
	pCadr->SetB(Ps);
	pCadr->SetE(Ph);
	pCont->Add(pCadr);

	if(dZ != 0.)
	{
		pCadr = new NCadrGeom;
		pCadr->SetAttr(1);
		pCadr->SetType(Type);
		pCadr->SetB(Ph);
		Ph = Ph + BPoint(0., 0., dZ, 0.);
		pCadr->SetE(Ph);
		pCont->Add(pCadr);
	}

	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(CCWDir ? ccwarc : cwarc);
	pCadr->SetB(Ph);
	pCadr->SetE(Pa);
	pCadr->SetI(Center - Ph);
	pCont->Add(pCadr);
}

void NHHApprCycle::Make1APPRLCT(BGeomArray *pCont, const BPoint &Dir, const BPoint &Norm, double Comp)
{
	double dZ = pCont->GetAt(0)->GetE().GetZ() - pCont->GetAt(0)->GetB().GetZ();
	NCadrGeom::Curve Type = pCont->GetAt(0)->GetType();
	bool CCWDir = (Dir % Norm).GetZ() > 0.;
	BPoint Ps(pCont->GetAt(0)->GetB());
	pCont->RemoveAll();
	BPoint Pa(NextGeom.GetB() + Norm * Comp);
	BPoint Center(Pa + Norm * R);
	double Ls = sqrt((Center - Ps).ProjXY().D2());
	if(Ls < R - 1.e-7)
		return;
	double Alf = acos(R / Ls);
	BPoint Ph = (Ps - Center).ProjXY() * ( R / Ls);
	Ph = Center + Ph * BMatr().RotZ(0., 0., 0., (Alf * 180 / PI) * (CCWDir ? 1. : -1.)) - BPoint(0., 0., dZ, 0.);

	NCadrGeom *pCadr;
	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(Type);
	pCadr->SetB(Ps);
	pCadr->SetE(Ph);
	pCont->Add(pCadr);

	if(dZ != 0.)
	{
		pCadr = new NCadrGeom;
		pCadr->SetAttr(1);
		pCadr->SetType(Type);
		pCadr->SetB(Ph);
		Ph = Ph + BPoint(0., 0., dZ, 0.);
		pCadr->SetE(Ph);
		pCont->Add(pCadr);
	}

	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(CCWDir ? ccwarc : cwarc);
	pCadr->SetB(Ph);
	pCadr->SetE(Pa);
	pCadr->SetI(Center - Ph);
	pCont->Add(pCadr);
}
int NHHApprCycle::GetError(void)
{
	return Error;
}