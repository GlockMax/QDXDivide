#include "stdafx.h"
#include "ConstDef.h"
#include "NToolsCrTBOut.h"
#include "NToolStd.h"
#include "NToolTurnBladeMill.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


IMPLEMENT_SERIAL(NToolTurnBladeMill, NToolTurnBlade, 0)
NToolTurnBladeMill::NToolTurnBladeMill(void) : NToolTurnBlade()
	, pMillTool(NULL)
{
	SetType(START_TURNBLADEMILL);
}

NToolTurnBladeMill::~NToolTurnBladeMill(void)
{
	delete pMillTool;
}

void NToolTurnBladeMill::FillPage(CDialog *pPage)
{
//	if(!pPage->IsKindOf(RUNTIME_CLASS(NToolsCrTBOut)))
//		return;// error

	NToolTurnBlade::FillPage(pPage);

//	NToolsCrTBOut *pP = (NToolsCrTBOut *)pPage;
}

void NToolTurnBladeMill::FillFromPage(CDialog *pPage)
{
//	if(!pPage->IsKindOf(RUNTIME_CLASS(NToolsCrTBOut)))
//		return;// error

	NToolTurnBlade::FillFromPage(pPage);

//	NToolsCrTBOut *pP = (NToolsCrTBOut *)pPage;
}
// создание копии инструмента
NToolTurnBladeMill* NToolTurnBladeMill::CreateCopyTool()
{
	NToolTurnBladeMill *pCreatedTool = new NToolTurnBladeMill;

	NToolTurnBlade::CreateCopyTo(pCreatedTool);
	
	pCreatedTool->pMillTool = pMillTool->CreateCopyTool();
	return pCreatedTool;
}
void NToolTurnBladeMill::Serialize(CArchive &ar)
{
	NToolTurnBlade::Serialize(ar);
	if (ar.IsStoring())
		ar << pMillTool;
	else
	{
		ar >> (CObject *&)pMillTool;
		GenInit();
	}
}

int NToolTurnBladeMill::GenInit(void)
{
	// Create BCont
	NContour Contour;
	if(!NTool::CreateSimGeneratrix(pMillTool, Contour))
		return -1;
	BCont.MakeContour(Contour);
	BCont.Invert();
	BCont.Trans(BMatr().RotX(BPoint(0.,0.,0.,1.), -90.));
	for(TBladeEdge *pCur = BCont.GetStart(); pCur != NULL; pCur = BCont.GetNext(pCur))
		pCur->SetAttr(BE_CUT);
	ToolSkel.RemoveAll();
	ToolSkel.Append(pMillTool->GetToolSkel());
	ToolSkel.Transform(BMatr().RotX(0., 0., 0., -90.));
	pMillTool->GetToolSkel().SetSize(0);

	BMatr M;// RotY(0,0,0,90)
	M.Set(	0., 0., -1., 0.,
			0., 1., 0., 0.,
			1., 0., 0., 0.,
			0., 0., 0., 1.);
	Gabar = pMillTool->GetGabar(); 
	Gabar.Transform(M * GetPosMatr());
	return 0;
}

void NToolTurnBladeMill::Regenerate(void)
{
	GenInit();
}

void NToolTurnBladeMill::SetMillTool(NToolMill *pT)
{
	pMillTool = pT;
	SetType(START_TURNBLADEMILL + pMillTool->GetType());
	Name = pMillTool->GetName();
}
NToolMill *  NToolTurnBladeMill::GetMillTool(void)
{
	return pMillTool;
}

void NToolTurnBladeMill::Draw(RenderMode GlobRMode, int step)
{
	if(!IsVisible())
		return;
	pMillTool->WFrame = WFrame;
	if(pMillTool)
	{
		glPushMatrix();
		glRotated(90.,0.,1.,0.);
		pMillTool->SetColor(Color);
		pMillTool->SetColorPassive(ColorPassive);
		pMillTool->Draw(GlobRMode, step);
		glPopMatrix();
	}
}

bool NToolTurnBladeMill::GetStdParams(double &Diameter , double &CornerRadius , double &ELength , double &FLength , double &BottomAngle , double &SideAngle , double &Height)
{
	if(!pMillTool)
		return false;
	return pMillTool->GetStdParams
		(Diameter, CornerRadius, ELength, FLength, BottomAngle, SideAngle, Height);
}