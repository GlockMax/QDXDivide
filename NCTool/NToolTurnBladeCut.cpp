#include "stdafx.h"
#include "ConstDef.h"
#include "math.h"
#include "NToolsCrTBCut.h"
#include "ntoolturnbladeout.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(NToolTurnBladeCut, NToolTurnBlade, 0)
NToolTurnBladeCut::NToolTurnBladeCut(void) : NToolTurnBlade()
	, m_AngleB(0.)
	, m_R(0.)
	, m_Whidth(0)
	, m_Length(0)
	, m_R1(0)
{
	SetType(52);
	Angle1 = 90.;
	Angle2 = 90.;
	Length1 = 0.51;
	Length2 = 0.;
}

NToolTurnBladeCut::NToolTurnBladeCut(const CString& TName, double NoseR, double Width, double Length
		, double MountAngle, double TSL, double NoseA, double NoseR1, double AngleC
		, double Height, double ShiftX, double ShiftZ, bool RB1, int Edge) : NToolTurnBlade()
{
	SetType(52);
	Angle1 = 90.;
	Angle2 = 90.;
	Length1 = 0.51;
	Length2 = 0.;
	Name = TName;
	m_R = NoseR;
	m_R1 = NoseR1;
	m_AngleB = NoseA;
	m_Whidth = Width;
	m_Length = Length;
	Displacement.Set(ShiftX, ShiftZ, 0., 0.);
	MainAngle = MountAngle;
	Slope = AngleC;
	if(Height != 0.)
		Thickness = Height;
	m_Edge = Edge;
}
NToolTurnBladeCut::~NToolTurnBladeCut(void)
{
}

void NToolTurnBladeCut::FillPage(CDialog *pPage)
{
	if(!pPage->IsKindOf(RUNTIME_CLASS(NToolsCrTBCut)))
		return;// error

	NToolTurnBlade::FillPage(pPage);

	NToolsCrTBCut *pP = (NToolsCrTBCut *)pPage;
	pP->m_AngleB = m_AngleB;
	pP->m_R = m_R;
	pP->m_R1 = m_R1;
	pP->m_Edge = m_Edge;
	pP->m_Thickness = Thickness;
	pP->m_AngleP = Slope;
	pP->m_Whidth = m_Whidth;
	pP->m_Length = m_Length;
}

void NToolTurnBladeCut::FillFromPage(CDialog *pPage)
{
	if(!pPage->IsKindOf(RUNTIME_CLASS(NToolsCrTBCut)))
		return;// error

	NToolTurnBlade::FillFromPage(pPage);

	NToolsCrTBCut *pP = (NToolsCrTBCut *)pPage;
	m_AngleB = pP->m_AngleB;
	m_R = pP->m_R;
	m_R1 = pP->m_R1;
	m_Edge = pP->m_Edge;
	Thickness = pP->m_Thickness ;
	Slope = pP->m_AngleP;
	m_Whidth = pP->m_Whidth;
	m_Length = pP->m_Length;
}
// создание копии инструмента
NToolTurnBladeCut* NToolTurnBladeCut::CreateCopyTool()
{
	NToolTurnBladeCut *pCreatedTool = new NToolTurnBladeCut;

	NToolTurnBlade::CreateCopyTo(pCreatedTool);
	
	pCreatedTool->m_AngleB = m_AngleB;
	pCreatedTool->m_R = m_R;
	pCreatedTool->m_R1 = m_R1;
	pCreatedTool->m_Edge = m_Edge;
	pCreatedTool->m_Whidth = m_Whidth;
	pCreatedTool->m_Length = m_Length;

	return pCreatedTool;
}
void NToolTurnBladeCut::Serialize(CArchive &ar)
{
	NToolTurnBlade::Serialize(ar);

	SerializeElements(ar, &m_AngleB, 1);
	SerializeElements(ar, &m_Whidth, 1);
	SerializeElements(ar, &m_Length, 1);
	SerializeElements(ar, &m_R, 1);
	SerializeElements(ar, &m_R1, 1);
	SerializeElements(ar, &m_Edge, 1);

	if(ar.IsLoading())
		GenInit();
}

int NToolTurnBladeCut::GenInit(void)
{
	if(m_Whidth <= 0. || m_Length <= 0.)
		return -1;
	if(m_R + m_R1 > m_Whidth)
		return -1;
	if(m_R > m_Length)
		return -1;
	if(m_R1 > m_Length)
		return -1;


// Contour must lie in XY plane
	NCadrGeom Geom[4];
	Geom[0].Set(line, m_Whidth, -m_Length, 0., m_Whidth, -m_Whidth * tan(m_AngleB * PI/ 180.), 0.);
	Geom[1].Set(line, 0., 0., 0., 0., 0., 0.);
	Geom[1].SetB(Geom[0].GetE());
	Geom[2].Set(line, 0., 0., 0., 0., - m_Length, 0.);
	Geom[3].Set(line, 0., - m_Length, 0., 0., 0., 0.);
	Geom[3].SetE(Geom[0].GetB());
	BCont.MakeContour(Geom, 4);

	BCont.GetStart()->SetAttr(BE_CUT);
	BCont.GetStart()->GetNext()->SetAttr(BE_CUT);
	BCont.GetStart()->GetNext()->GetNext()->SetAttr(BE_CUT);
	BPoint ActCenter(0.,0.,0.,1.);
	if(m_R > 0)
	{
		BCont.GetStart()->GetNext()->GetNext()->InsertTangRad(m_R - TOOL_OFFSET);
		BCont.GetStart()->GetNext()->GetNext()->GetNext()->SetAttr(BE_CUT);
		ActCenter = BCont.GetStart()->GetNext()->GetNext()->GetCircCenter();
	}
	else
	{
		ActCenter = BCont.GetStart()->GetNext()->GetNext()->GetPoint();
	}
	BPoint ActCenter1(0.,0.,0.,1.);
	if(m_R1 > 0)
	{
		BCont.GetStart()->GetNext()->InsertTangRad(m_R1 - TOOL_OFFSET);
		BCont.GetStart()->GetNext()->GetNext()->SetAttr(BE_CUT);
		ActCenter1 = BCont.GetStart()->GetNext()->GetCircCenter();
	}
	else
	{
		ActCenter1 = BCont.GetStart()->GetNext()->GetPoint();
	}
	if(m_Edge == 1)
		BCont.Trans(BMatr().Trans(ActCenter1, BPoint(0.,0.,0.,1.)));
	else
		BCont.Trans(BMatr().Trans(ActCenter, BPoint(0.,0.,0.,1.)));
	BCont.Trans(BMatr().RotZ(BPoint(0.,0.,0.,1.), MainAngle - 270.)
		* BMatr().Trans(BPoint(0.,0.,0.,1.), Displacement * (-1.)));
	BCont.BreakArcs();
	BCont.RemoveShortEdges();
	FillGabar();
	// Define Length1 Length2
	double B = min(m_Whidth, m_Length) * 0.5;
	B = min(B, fabs(m_Whidth - m_R));
	B = min(B, fabs(m_Whidth - m_R1));
	if(B < Length1 + Length2)
	{
		if(B < 0.05)
		{
			Length1 = 0;
			Length2 = 0;
		}
		else
		{
			Length1 = 2./3. * B;
			Length2 = 1./3. * B;
		}
	}

	MakeBContSkeleton();
	return 0;
}

void NToolTurnBladeCut::Regenerate(void)
{
	GenInit();
}

bool NToolTurnBladeCut::GetStdParams(double &Diameter , double &CornerRadius , double &ELength , double &FLength , double &BottomAngle , double &SideAngle , double &Height)
{
	Diameter = this->m_R * 2.;
	CornerRadius = this->m_R1;
    ELength = 0.;
	FLength = 0.;
	BottomAngle = 0.;
	SideAngle = 0.;
	Height = 0.;
	return true;
}