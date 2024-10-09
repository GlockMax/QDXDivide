#include "stdafx.h"
#include "ConstDef.h"
#include "math.h"
#include "NToolsCrTBOut.h"
#include "ntoolturnbladeout.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(NToolTurnBladeOut, NToolTurnBlade, 0)
NToolTurnBladeOut::NToolTurnBladeOut(void) : NToolTurnBlade()
	, m_AngleA(0.)
	, m_AngleB(0.)
	, m_L1(0.)
	, m_L2(0.)
	, m_R(0.)
{
	Angle1 = 115.;
	Angle2 = 45.;
	Length1 = (MM_INCH == M_MM) ? 2. : 0.1;
	Length2 = (MM_INCH == M_MM) ? 1. : 0.05;
	SetType(50);
}
NToolTurnBladeOut::NToolTurnBladeOut(const CString& TName, double AngleA, double AngleB, double L1, double L2, double NoseR,
		double AngleC, double Height, double MountAngle, double ShiftX, double ShiftZ, bool RB1) : NToolTurnBlade()
{
	SetType(50);
	Name = TName;
	m_AngleA = AngleA;
	m_AngleB = AngleB;
	m_L1 = L1;
	m_L2 = L2;
	m_R = NoseR;
	Displacement.Set(ShiftX, ShiftZ, 0., 0.);
	MainAngle = MountAngle;
	Slope = AngleC;
	if(Height != 0.)
		Thickness = Height;
//	m_RB1 = RB1;
}

NToolTurnBladeOut::~NToolTurnBladeOut(void)
{
}

void NToolTurnBladeOut::FillPage(CDialog *pPage)
{
	if(!pPage->IsKindOf(RUNTIME_CLASS(NToolsCrTBOut)))
		return;// error

	NToolTurnBlade::FillPage(pPage);

	NToolsCrTBOut *pP = (NToolsCrTBOut *)pPage;

	switch(int(MainAngle))
	{
	case 0:
		pP->m_FormOut = 0;
		pP->m_AngleA = m_AngleA - 90.;
		pP->m_AngleB = 180 - m_AngleA - m_AngleB;
		pP->m_L1 = m_L1;
		pP->m_L2 = m_L2;
		pP->m_Xdisp = - m_R;
		pP->m_Zdisp = - m_R;
		break;
	case 90:
		pP->m_FormOut = 1;
		pP->m_AngleA = m_AngleB;
		pP->m_AngleB = 180 - m_AngleA - m_AngleB;
		pP->m_L1 = m_L2;
		pP->m_L2 = m_L1;
		pP->m_Xdisp =   m_R;
		pP->m_Zdisp = - m_R;
		break;
	case 180:
		pP->m_FormOut = 3;
		pP->m_AngleA = m_AngleA - 90.;
		pP->m_AngleB = 180 - m_AngleA - m_AngleB;
		pP->m_L1 = m_L1;
		pP->m_L2 = m_L2;
		pP->m_Xdisp =   m_R;
		pP->m_Zdisp =   m_R;
		break;
	case 270:
		pP->m_FormOut = 2;
		pP->m_AngleA = m_AngleB;
		pP->m_AngleB = 180 - m_AngleA - m_AngleB;
		pP->m_L1 = m_L2;
		pP->m_L2 = m_L1;
		pP->m_Xdisp = - m_R;
		pP->m_Zdisp =   m_R;
		break;
	}

	pP->m_R = m_R;
	pP->m_Thickness = Thickness;
	pP->m_AngleP = Slope;
}

void NToolTurnBladeOut::FillFromPage(CDialog *pPage)
{
	if(!pPage->IsKindOf(RUNTIME_CLASS(NToolsCrTBOut)))
		return;// error

	NToolTurnBlade::FillFromPage(pPage);

	NToolsCrTBOut *pP = (NToolsCrTBOut *)pPage;

	m_R = pP->m_R;
	switch(pP->m_FormOut)
	{
	case 0:
		m_AngleA = pP->m_AngleA + 90.;
		m_AngleB = 90 - pP->m_AngleA - pP->m_AngleB;
		m_L1 = pP->m_L1;
		m_L2 = pP->m_L2;
		Displacement.Set(- m_R, - m_R, 0., 0.);
		MainAngle = 0.;
		break;
	case 1:
		m_AngleA = 180. - pP->m_AngleA - pP->m_AngleB;
		m_AngleB = pP->m_AngleA;
		m_L1 = pP->m_L2;
		m_L2 = pP->m_L1;
		Displacement.Set(  m_R, - m_R, 0., 0.);
		MainAngle = 90.;
		break;
	case 2:
		m_AngleA = 180. - pP->m_AngleA - pP->m_AngleB;
		m_AngleB = pP->m_AngleA;
		m_L1 = pP->m_L2;
		m_L2 = pP->m_L1;
		Displacement.Set(- m_R,   m_R, 0., 0.);
		MainAngle = 270.;
		break;
	case 3:
		m_AngleA = pP->m_AngleA + 90.;
		m_AngleB = 90 - pP->m_AngleA - pP->m_AngleB;
		m_L1 = pP->m_L1;
		m_L2 = pP->m_L2;
		Displacement.Set(  m_R,   m_R, 0., 0.);
		MainAngle = 180;
		break;
	}
	Thickness = pP->m_Thickness ;
	Slope = pP->m_AngleP;
}
// создание копии инструмента
NToolTurnBladeOut* NToolTurnBladeOut::CreateCopyTool()
{
	NToolTurnBladeOut *pCreatedTool = new NToolTurnBladeOut;

	NToolTurnBlade::CreateCopyTo(pCreatedTool);
	
	pCreatedTool->m_AngleA = m_AngleA;
	pCreatedTool->m_AngleB = m_AngleB;
	pCreatedTool->m_L1 = m_L1;
	pCreatedTool->m_L2 = m_L2;
	pCreatedTool->m_R = m_R;

	return pCreatedTool;
}
void NToolTurnBladeOut::Serialize(CArchive &ar)
{
	NToolTurnBlade::Serialize(ar);

	SerializeElements(ar, &m_AngleA, 1);
	SerializeElements(ar, &m_AngleB, 1);
	SerializeElements(ar, &m_L1, 1);
	SerializeElements(ar, &m_L2, 1);
	SerializeElements(ar, &m_R, 1);
	SerializeElements(ar, &m_PointPos, 1);

	if(ar.IsLoading())
		GenInit();
}

int NToolTurnBladeOut::GenInit(void)
{
	if(m_AngleB + m_AngleA >= 179.)
		return 1;
	if(m_AngleB < 0. || m_AngleA < 0.)
		return 2;
	if(m_L1 < MIND || m_L2 < MIND)
		return 3;

// Contour must lie in XY plane
	NCadrGeom Geom[4];
	Geom[0].Set(line, m_L2, 0., 0., 0., 0., 0.);
	Geom[1].Set(line, 0., 0., 0., - m_L1, 0., 0.);
	Geom[1].Tr(BMatr().RotZ(BPoint(0.,0.,0.,1), m_AngleA + m_AngleB));
	Geom[2] = Geom[1];
	Geom[2].SetB(Geom[1].GetE());
	Geom[2].SetE(Geom[1].GetE() + BPoint(m_L2, 0., 0., 0.));
	Geom[3] = Geom[2];
	Geom[3].SetB(Geom[2].GetE());
	Geom[3].SetE(Geom[0].GetB());
	BCont.MakeContour(Geom, 4);
//	BCont.GetStart()->SetAttr(BE_CUT);
//	BCont.GetStart()->GetPrev()->SetAttr(BE_CUT);
	if(m_R > 0.)
	{
		BCont.GetStart()->GetNext()->InsertTangRad(m_R);
		BCont.GetStart()->GetNext()->SetAttr(BE_CUT);
		BCont.GetStart()->SetAttr(BE_CUT);
		BCont.GetStart()->GetNext()->GetNext()->SetAttr(BE_CUT);
		BCont.Trans(BMatr().Trans(BPoint(0.,0.,0.,-1.), BCont.GetStart()->GetNext()->GetCircCenter() * (-1.))
			* BMatr().RotZ(BPoint(0.,0.,0.,1.), - m_AngleB + MainAngle - 270.)
			* BMatr().Trans(BPoint(0.,0.,0.,1.), Displacement * (-1.)));
		BCont.BreakArcs();
	}
	else
	{
		BCont.GetStart()->GetNext()->SetAttr(BE_CUT);
		BCont.GetStart()->SetAttr(BE_CUT);
		BCont.Trans(BMatr().RotZ(BPoint(0.,0.,0.,1.), - m_AngleB + MainAngle - 270.)
			* BMatr().Trans(BPoint(0.,0.,0.,1.), Displacement * (-1.)));
	}

	FillGabar();
	// Define Length1 Length2
	double SharpA = m_AngleA + m_AngleB;
	if(SharpA > 90.)
		SharpA = 180. - SharpA;
	SharpA = SharpA * PI / 180.;

	double B = min(m_L1, m_L2) * 0.5 * sin(SharpA);
	if(B < Length1 + Length2)
	{
		Length1 = 2./3. * B;
		Length2 = 1./3. * B;
	}

	MakeBContSkeleton();
	return 0;
}

void NToolTurnBladeOut::Regenerate(void)
{
	GenInit();
}

bool NToolTurnBladeOut::GetStdParams(double &Diameter , double &CornerRadius , double &ELength , double &FLength , double &BottomAngle , double &SideAngle , double &Height)
{
	Diameter = 	this->m_R * 2.;
	switch(int(MainAngle))
	{
	case 0:
		CornerRadius = 3.;
		break;
	case 90:
		CornerRadius = 2.;
		break;
	case 180:
		CornerRadius = 1.;
		break;
	case 270:
		CornerRadius = 4.;
		break;
	default:
		CornerRadius = 0.;
		break;
	}
    ELength = 0.;
	FLength = 0.;
	BottomAngle = 0.;
	SideAngle = 0.;
	Height = 0.;
	return true;
}