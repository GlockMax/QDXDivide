#include "stdafx.h"
#include "ConstDef.h"
#include "math.h"
#include "NToolsCrTBCut.h"
#include "NToolTurnBladeCont.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(NToolTurnBladeCont, NToolTurnBlade, 0)
NToolTurnBladeCont::NToolTurnBladeCont(void) : NToolTurnBlade()
{
	SetType(56);
}

NToolTurnBladeCont::NToolTurnBladeCont(const CString& TName, const TBladeCont &ToolCont, bool CutCont, double Thick, double Angle) : NToolTurnBlade(TName)
{
	SetType(56);
	Length1 = 0;
	Length2 = 0;
	Angle1 = 0;
	Angle2 = 0;
	Thickness = Thick;
	Slope = Angle;
	BCont.MakeCopy(ToolCont);
	Cutting = CutCont;
}

NToolTurnBladeCont::~NToolTurnBladeCont(void)
{
}

void NToolTurnBladeCont::FillPage(CDialog *pPage)
{
}

void NToolTurnBladeCont::FillFromPage(CDialog *pPage)
{
}
// создание копии инструмента
NToolTurnBladeCont* NToolTurnBladeCont::CreateCopyTool()
{
	NToolTurnBladeCont *pCreatedTool = new NToolTurnBladeCont;

	NToolTurnBlade::CreateCopyTo(pCreatedTool);
	
	return pCreatedTool;
}
void NToolTurnBladeCont::Serialize(CArchive &ar)
{
	NToolTurnBlade::Serialize(ar);

	if(ar.IsLoading())
		GenInit();
}

int NToolTurnBladeCont::GenInit(void)
{
	BCont.BreakArcs();
	BCont.RemoveShortEdges();
	FillGabar();
	MakeBContSkeleton();
	return 0;
}

void NToolTurnBladeCont::Regenerate(void)
{
	GenInit();
}

bool NToolTurnBladeCont::GetStdParams(double &Diameter , double &CornerRadius , double &ELength , double &FLength , double &BottomAngle , double &SideAngle , double &Height)
{
	Diameter = 0.;
	CornerRadius = 0.;
	ELength = 0.;
	FLength = 0.;
	BottomAngle = 0.;
	SideAngle = 0.;
	Height = 0.;
	return true;
}