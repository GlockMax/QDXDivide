// NToolBRound.cpp: implementation of the NToolBRound class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConstDef.h"
#include "ResLocal.h"
#include "NToolsCrDiskSheet.h"
#include "math.h"
#include "NToolBRound.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const double H90 = sqrt(2.) / 2.;

IMPLEMENT_SERIAL(NToolBRound, NToolShaped, 0)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


NToolBRound::NToolBRound():NToolShaped()
{
	D = B = L = R = h = 0;
}

NToolBRound::~NToolBRound()
{
}

NToolBRound::NToolBRound(const CString &Name, double &diam, double &b, double &l, double &r,
		double &hm) : NToolShaped(Name)
{
	D = diam;
	B = b;
	L = l;
	R = r;
	h = hm; 
}

void NToolBRound::Serialize(CArchive &ar)
{
	// We should not call NToolShaped::Serialize
	NToolMill::Serialize(ar);
	SerializeElements(ar,&h,1);
	SerializeElements(ar,&D,1);
	SerializeElements(ar,&B,1);
	SerializeElements(ar,&L,1);
	SerializeElements(ar,&R,1);
	SerializeElements(ar,&IsNameAuto,1);
	
	if(ar.IsLoading())
	{
		GenInit();
		double bz = Zdisp;
		Zdisp = 0;
		SetZdisp(bz);
	}
}

int NToolBRound::GenInit()
{
// Calculate dimensions in object coord
	Gabar.Expand(-D/2.,-D/2.,0);
	Gabar.Expand(D/2.,D/2.,B);

// Make Contour
	Cont.Types.RemoveAll();
	BPoint p0(0., 0., B, 1.);
	BPoint p3(D / 2. , 0., B, 1.);
	BPoint p1((p0 + p3) * 0.5);
	Cont.AddFirst(p0, p1, p1, p3);
	Cont.Types[0] = HORIZONTAL;
	p0 = p3;
	p3 = p0 + BPoint(0., 0., - (B - L - R), 0.);
	p1 = (p0 + p3) * 0.5;
	Cont.AddFollow(p1, p1, p3);
	Cont.Types[1] = CONE;
	p0 = p3;
	p3 = p0 + BPoint(- R, 0., - R, 0.);
	p1 = (p0 + BPoint(- R, 0., 0., 0.)) * H90;
	Cont.AddFollow(p1, p1, p3);
	Cont.Types[2] = TORAHS;
	if(L > 0.)
	{
		p0 = p3;
		p3 = p0 + BPoint(R, 0., - R, 0.);
		p1 = (p3 + BPoint(- R, 0., 0., 0.)) * H90;
		Cont.AddFollow(p1, p1, p3);
		Cont.Types[Cont.num - 1] = TORAHS;
		p0 = p3;
		p3.Set(D / 2., 0., 0., 1.);
		p1 = (p0 + p3) * 0.5;
		Cont.AddFollow(p1, p1, p3);
		Cont.Types[Cont.num - 1] = CONE;
	}
	p0 = p3;
	p3.Set(0., 0., 0., 1.);
	p1 = (p0 + p3) * 0.5;
	Cont.AddFollow(p1, p1, p3);
	Cont.Types[Cont.num - 1] = HORIZONTAL;
	Cont = Cont * BMatr().Trans(BPoint(0., 0., 0., 1.), BPoint(0., 0., Zdisp, 1.));
	return 0;
}

void NToolBRound::FillSheet(CPropertySheet *pDial)
{
//	ASSERT(pDial->IsKindOf(RUNTIME_CLASS(NToolsCrDial)));
	NToolShaped::FillSheet(pDial);
	NToolsCrDiskSheet *pD = (NToolsCrDiskSheet *)pDial;
	int TypePage = NToolShaped::GetType();
// определение типа инструмента	
	switch(TypePage)
	{
	case 71: // 
		pD->m_broundpage.m_diam = RoundP( D );
		pD->m_broundpage.m_b = RoundP( B );
		pD->m_broundpage.m_l = RoundP( L );
		pD->m_broundpage.m_r = RoundP( R );
		pD->m_broundpage.m_h1 = RoundP( h );
		pD->m_broundpage.m_name = Name;	
		pD->m_broundpage.IsNameAuto = IsNameAuto;
		pD->tab_pos = 6;
		break;
	}
}

// определение геометрических параметров инструмента
void NToolBRound::GetParam(double &diam, double &b, double &l, double &r,
		double &hm)
{
	diam = D;
	b = B;
	l = L;
	r = R;
	hm = h; 
}

bool NToolBRound::NeedAppr(void) const
{
	return true;
}

bool NToolBRound::GetStdParams(double &Diameter , double &CornerRadius , double &ELength , double &FLength , double &BottomAngle , double &SideAngle , double &Height)
{
	Diameter = D;
	CornerRadius = -R;
	ELength = 0.;
	FLength = 0.;
	BottomAngle = 0.;
	SideAngle = 0.;
	Height = B;
	return true;
}
