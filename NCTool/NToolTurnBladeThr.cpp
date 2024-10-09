#include "stdafx.h"
#include "math.h"
#include "ConstDef.h"
#include "NToolsCrTBThr.h"
#include "ntoolturnbladeout.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(NToolTurnBladeThr, NToolTurnBlade, 0)
NToolTurnBladeThr::NToolTurnBladeThr(void) : NToolTurnBlade()
	, m_R(0.)
	, m_Thickness((MM_INCH == M_MM) ? 4.76 : 0.188)
	, m_ThreadAngle(60.)
	, m_CutWhide((MM_INCH == M_MM) ? 4.76 : 0.188)
	, m_CutLength((MM_INCH == M_MM) ? 10. : 0.33)
	, m_HalfT((MM_INCH == M_MM) ? 2.38 : 0.094)
{
	SetType(54);
	Angle1 = 90.;
	Angle2 = 90.;
	Length1 = (MM_INCH == M_MM) ? 0.51 : 0.02;
	Length2 = 0.;
}

NToolTurnBladeThr::~NToolTurnBladeThr(void)
{
}

void NToolTurnBladeThr::FillPage(CDialog *pPage)
{
	if(!pPage->IsKindOf(RUNTIME_CLASS(NToolsCrTBThr)))
		return;// error

	NToolTurnBlade::FillPage(pPage);

	NToolsCrTBThr *pP = (NToolsCrTBThr *)pPage;

	pP->m_ThreadAngle = m_ThreadAngle;
	pP->m_CutWhide = m_CutWhide;
	pP->m_CutLength = m_CutLength;
	pP->m_HalfT = m_HalfT;
	pP->m_R = m_R;
	pP->m_RB1 = m_RB1;
	pP->m_Thickness = Thickness;
	pP->m_AngleP = Slope;
	if(MainAngle == 0)
	{
		if(pP->m_Zdisp < 0)
            pP->m_FormTread = 0;
		else
            pP->m_FormTread = 2;
	}
	else
	{
		if(pP->m_Zdisp < 0)
            pP->m_FormTread = 1;
		else
            pP->m_FormTread = 3;
	}
}

void NToolTurnBladeThr::FillFromPage(CDialog *pPage)
{
	if(!pPage->IsKindOf(RUNTIME_CLASS(NToolsCrTBThr)))
		return;// error

	NToolTurnBlade::FillFromPage(pPage);

	NToolsCrTBThr *pP = (NToolsCrTBThr *)pPage;


	m_ThreadAngle = pP->m_ThreadAngle;
	m_CutWhide = pP->m_CutWhide;
	m_CutLength = pP->m_CutLength;
	m_HalfT = pP->m_HalfT;
	m_R = pP->m_R;
	m_RB1 = pP->m_RB1;
	Thickness = pP->m_Thickness;
	Slope = pP->m_AngleP;
	switch(pP->m_FormTread)
	{
	case 0:
		pP->m_Zdisp = - m_HalfT;
		pP->m_Xdisp = - m_R;
		MainAngle = 0;
		break;
	case 1:
		pP->m_Zdisp = - (m_CutWhide - m_HalfT);
		pP->m_Xdisp = m_R;
		MainAngle = 180;
		break;
	case 2:
		pP->m_Zdisp = m_CutWhide - m_HalfT;
		pP->m_Xdisp = - m_R;
		MainAngle = 0;
		break;
	case 3:
		pP->m_Zdisp = m_HalfT;
		pP->m_Xdisp = m_R;
		MainAngle = 180;
		break;
	}
	Displacement.Set(pP->m_Xdisp, pP->m_Zdisp, 0., 0.);
}
// создание копии инструмента
NToolTurnBladeThr* NToolTurnBladeThr::CreateCopyTool()
{
	NToolTurnBladeThr *pCreatedTool = new NToolTurnBladeThr;

	NToolTurnBlade::CreateCopyTo(pCreatedTool);
	
	pCreatedTool->m_ThreadAngle = m_ThreadAngle;
	pCreatedTool->m_CutWhide = m_CutWhide;
	pCreatedTool->m_CutLength = m_CutLength;
	pCreatedTool->m_HalfT = m_HalfT;
	pCreatedTool->m_R = m_R;
	pCreatedTool->m_RB1 = m_RB1;
	pCreatedTool->m_Thickness = Thickness;
	pCreatedTool->m_AngleP = Slope;

	return pCreatedTool;
}
void NToolTurnBladeThr::Serialize(CArchive &ar)
{
	NToolTurnBlade::Serialize(ar);

	SerializeElements(ar, &m_ThreadAngle, 1);
	SerializeElements(ar, &m_CutWhide, 1);
	SerializeElements(ar, &m_CutLength, 1);
	SerializeElements(ar, &m_HalfT, 1);
	SerializeElements(ar, &m_R, 1);
	SerializeElements(ar, &m_PointPos, 1);

	if(ar.IsLoading())
		GenInit();
}

int NToolTurnBladeThr::GenInit(void)
{
	if(m_HalfT >= m_CutWhide)
		return 1;

// Contour must lie in XY plane
	NCadrGeom Geom[5];
	double ARad = m_ThreadAngle * PI / 180.;
	double yLeft = (m_CutWhide - m_HalfT) / tan(ARad * 0.5);
	double yRight = m_HalfT / tan(ARad * 0.5);
	Geom[0].Set(line, m_CutWhide - m_HalfT, - yRight, 0., 0., 0., 0.);
	Geom[1].Set(line, 0., 0., 0., - m_HalfT, - yLeft, 0.);
	Geom[2].Set(line, - m_HalfT, -yLeft, 0., - m_HalfT, - m_CutLength, 0.);
	Geom[3].Set(line, - m_HalfT, - m_CutLength, 0., m_CutWhide - m_HalfT, - m_CutLength, 0.);
	Geom[4].Set(line, m_CutWhide - m_HalfT, - m_CutLength, 0., m_CutWhide - m_HalfT, - yRight, 0.);
	BCont.MakeContour(Geom, 5);
	if(m_R > 0.)
	{
		BCont.GetStart()->GetNext()->InsertTangRad(m_R - TOOL_OFFSET);
		BCont.GetStart()->GetNext()->SetAttr(BE_CUT);
		BCont.GetStart()->SetAttr(BE_CUT);
		BCont.GetStart()->GetNext()->GetNext()->SetAttr(BE_CUT);
		BCont.Trans(BMatr().Trans(BPoint(0.,0.,0.,-1.), BCont.GetStart()->GetNext()->GetCircCenter() * (-1.))
			* BMatr().RotZ(BPoint(0.,0.,0.,1.), MainAngle - 270.)
			* BMatr().Trans(BPoint(0.,0.,0.,1.), Displacement * (-1.)));
		BCont.BreakArcs();
	}
	else
	{
		BCont.GetStart()->GetNext()->SetAttr(BE_CUT);
		BCont.GetStart()->SetAttr(BE_CUT);
		BCont.Trans(BMatr().RotZ(BPoint(0.,0.,0.,1.), MainAngle - 270.)
			* BMatr().Trans(BPoint(0.,0.,0.,1.), Displacement * (-1.)));
	}
	BCont.RemoveShortEdges();
	FillGabar();
	// Define Length1 Length2
	if(m_HalfT < Length1 + Length2)
	{
		Length1 = m_HalfT;
		Length2 = 0.;
	}

	MakeBContSkeleton();
	return 0;
}

void NToolTurnBladeThr::Regenerate(void)
{
	GenInit();
}
NToolTurnBladeThr::NToolTurnBladeThr(const CString& TName, double ThreadAngle
	, double CutWhide, double CutLength, double HalfT, double R, double AngleC, double Height, double MountAngle, double ShiftX, double ShiftZ, bool RB1) : NToolTurnBlade()
	, m_R(0.)
	, m_Thickness((MM_INCH == M_MM) ? 4.76 : 0.188)
	, m_ThreadAngle(60.)
	, m_CutWhide((MM_INCH == M_MM) ? 4.76 : 0.188)
	, m_CutLength((MM_INCH == M_MM) ? 10. : 0.33)
	, m_HalfT((MM_INCH == M_MM) ? 2.38 : 0.094)
{
	SetType(54);
	Name = TName;
	Angle1 = 90.;
	Angle2 = 90.;
	Length1 = (MM_INCH == M_MM) ? 0.5 : 0.02;
	Length2 = 0.;
	m_ThreadAngle = ThreadAngle;
	m_CutWhide = CutWhide;
	m_CutLength = CutLength;
	m_HalfT = HalfT;
	m_R = R;
	Displacement.Set(ShiftX, ShiftZ, 0., 0.);
	MainAngle = MountAngle;
	Slope = AngleC;
	if(Height != 0.)
		Thickness = Height;
//	m_RB1 = RB1;

}
bool NToolTurnBladeThr::GetStdParams(double &Diameter , double &CornerRadius , double &ELength , double &FLength , double &BottomAngle , double &SideAngle , double &Height)
{
	Diameter = 	this->m_R * 2.;
	CornerRadius = 0.;
    ELength = 0.;
	FLength = 0.;
	BottomAngle = 0.;
	SideAngle = 0.;
	Height = 0.;
	return true;
}