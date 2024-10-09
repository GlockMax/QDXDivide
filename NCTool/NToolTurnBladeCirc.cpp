#include "stdafx.h"
#include "ConstDef.h"
#include "NToolsCrTBCirc.h"
#include "NToolTurnBladeCirc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(NToolTurnBladeCirc, NToolTurnBlade, 0)

NToolTurnBladeCirc::NToolTurnBladeCirc(void) : NToolTurnBlade()
	, m_R(0.)
{
	SetType(51);
}
NToolTurnBladeCirc::NToolTurnBladeCirc(const CString& TName, double R, double TSL
		, double ShiftX, double ShiftZ, bool RB1) : NToolTurnBlade()
{
	SetType(51);
	Name = TName;
	m_R = R;
	Displacement.Set(ShiftX, ShiftZ, 0., 0.);
}

NToolTurnBladeCirc::~NToolTurnBladeCirc(void)
{
}

void NToolTurnBladeCirc::FillPage(CDialog *pPage)
{
	if(!pPage->IsKindOf(RUNTIME_CLASS(NToolsCrTBCirc)))
		return;// error

	NToolTurnBlade::FillPage(pPage);

	NToolsCrTBCirc *pP = (NToolsCrTBCirc *)pPage;
	pP->m_R = m_R;
	pP->m_Thickness = Thickness;
	pP->m_AngleP = Slope;
}

void NToolTurnBladeCirc::FillFromPage(CDialog *pPage)
{
	if(!pPage->IsKindOf(RUNTIME_CLASS(NToolsCrTBCirc)))
		return;// error

	NToolTurnBlade::FillFromPage(pPage);

	NToolsCrTBCirc *pP = (NToolsCrTBCirc *)pPage;
	m_R = pP->m_R;
	Thickness = pP->m_Thickness ;
	Slope = pP->m_AngleP;
}
NToolTurnBladeCirc* NToolTurnBladeCirc::CreateCopyTool()
{
	NToolTurnBladeCirc *pCreatedTool = new NToolTurnBladeCirc;

	NToolTurnBlade::CreateCopyTo(pCreatedTool);
	
	pCreatedTool->m_R = m_R;

	return pCreatedTool;
}
void NToolTurnBladeCirc::Serialize(CArchive &ar)
{
	NToolTurnBlade::Serialize(ar);

	SerializeElements(ar, &m_R, 1);

	if(ar.IsLoading())
		GenInit();
}

int NToolTurnBladeCirc::GenInit(void)
{
	double Rb = m_R - TOOL_OFFSET;
// Contour must lie in XY plane
	NCadrGeom Geom[4];
	Geom[0].Set(line, Rb, 0., 0., 0., Rb, 0.);
	Geom[1].Set(line, 0., Rb, 0., - Rb, 0., 0.);
	Geom[2].Set(line, - Rb, 0., 0., 0., - Rb, 0.);
	Geom[3].Set(line, 0., - Rb, 0., Rb, 0., 0.);
	BCont.MakeContour(Geom, 4);
	for(TBladeEdge *pCur = BCont.GetStart(); pCur != NULL; pCur = BCont.GetNext(pCur))
	{
		pCur->SetCircCenter(BPoint(0.,0.,0.,1.));
		pCur->SetAttr(BE_CUT);
	}
	BCont.BreakArcs();
	BCont.RemoveShortEdges();
	BCont.Trans(BMatr().RotZ(BPoint(0.,0.,0.,1.), MainAngle)
		* BMatr().Trans(BPoint(0.,0.,0.,1.), Displacement * (-1.)));

	FillGabar();
	// Define Length1 Length2
	if(Rb < Length1 + Length2)
	{
		Length1 = 2./3. * Rb;
		Length2 = 1./3. * Rb;
	}
	MakeBContSkeleton();
	return 0;
}

void NToolTurnBladeCirc::Regenerate(void)
{
	GenInit();
}
bool NToolTurnBladeCirc::GetStdParams(double &Diameter , double &CornerRadius , double &ELength , double &FLength , double &BottomAngle , double &SideAngle , double &Height)
{
	Diameter = this->m_R * 2.;
	CornerRadius = 0.;
    ELength = 0.;
	FLength = 0.;
	BottomAngle = 0.;
	SideAngle = 0.;
	Height = 0.;
	return true;
}