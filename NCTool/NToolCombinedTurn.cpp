#include "stdafx.h"
#include "NCMProject.h"
#include "ntoolturnblade.h"
#include "NToolTurnBladeCont.h"
#include "NToolsConCrTurnPage.h"
#include "NToolTurnBladeMill.h"
#include "ntoolcombinedturn.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(NToolCombinedTurn, NToolCombined, 0)

NToolCombinedTurn::NToolCombinedTurn(void) 
	: Displacement(0.,0.,0.,0.), MainAngle(0.)
{
	CompShiftAuto = false;
}
NToolCombinedTurn::NToolCombinedTurn(const CString &Name)
 : Displacement(0.,0.,0.,0.), MainAngle(0.), NToolCombined(Name)
{
	CompShiftAuto = false;
}

NToolCombinedTurn::~NToolCombinedTurn(void)
{
}

NToolTurn * NToolCombinedTurn::GetTool(int n) const
{
	return (NToolTurn *)NToolCombined::GetTool(n);
}
NToolCombinedTurn* NToolCombinedTurn::CreateCopyTool() const
{

	NToolCombinedTurn *pCreatedTool = new NToolCombinedTurn(Name);

	pCreatedTool->CompShift = CompShift;
	pCreatedTool->CompShiftAuto = CompShiftAuto;

	pCreatedTool->Zdisp = Zdisp;
	pCreatedTool->Color = Color;
	pCreatedTool->Visible = Visible;
	pCreatedTool->PointSize = PointSize;
	pCreatedTool->IsNameAuto = IsNameAuto;
	pCreatedTool->ColorPassive = ColorPassive;
	pCreatedTool->Position = Position;
	pCreatedTool->Turret = Turret;
	pCreatedTool->Attach = Attach;
	pCreatedTool->Displacement = Displacement;
	pCreatedTool->MainAngle = MainAngle;
	pCreatedTool->LifeTime = LifeTime;
	pCreatedTool->IgnoreUpToolStockColl = IgnoreUpToolStockColl;

	for (int i = 0; i < num_tools; i++)
	{
		pCreatedTool->AddTool(GetTool(i)->CreateCopyTool());
	}

	return pCreatedTool;
}
void NToolCombinedTurn::FillDialog(CDialog *pDial) const
{
	NToolCombined::FillDialog(pDial);

	NToolsConCrTurnPage *pD = (NToolsConCrTurnPage *)pDial;
	pD->SetToolCombined(this);

	pD->m_Xdisplacement = Displacement.GetX();
	pD->m_Zdisplacement = Displacement.GetY();	
	pD->m_MainAngle = MainAngle;	
}

void NToolCombinedTurn::SetChildrenDispl(void)
{
	// Change children Displacements
	for(int i = 0; i < num_tools; ++i)
	{
		GetTool(i)->SetDisplacement(GetTool(i)->GetDisplacement() - Displacement
						, GetTool(i)->GetMainAngle() - MainAngle);
		GetTool(i)->GenInit();
	}
}

void NToolCombinedTurn::RestChildrenDispl(void)
{
	// MainAngle should be = 0 if there is NToolTurnBladeMill 
	for (int i = 0; i < num_tools; ++i)
	{
		if (dynamic_cast<NToolTurnBladeMill*>(GetTool(i)) != nullptr)
			MainAngle = 0;
	}

	// Restore children Displacements
	for(int i = 0; i < num_tools; ++i)
	{
		GetTool(i)->SetDisplacement(GetTool(i)->GetDisplacement() + Displacement
			, GetTool(i)->GetMainAngle() + MainAngle);
		GetTool(i)->GenInit();
	}

}
void NToolCombinedTurn::Serialize(CArchive &ar)
{
	NToolCombined::Serialize(ar);
	if(!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("3.5")))
	{
		SerializeElements(ar,&Displacement,1);
	}
}

BPoint NToolCombinedTurn::GetCompVect(void) const
{
	BPoint tmp = CompShift * BMatr().RotY(0., 0., 0., 90.);
	return BPoint(tmp.GetX(), -tmp.GetY(), -tmp.GetZ(), 0.);
}

bool NToolCombinedTurn::MakeShapedTools(const TBladeCont &ToolCont, bool CutCont, double Thick, double Angle)
{
	if(ToolCont.GetStart() == NULL)
		return false;
	if(CutCont)
	{
		NToolTurnBladeCont *pShaped = new NToolTurnBladeCont(_T("SHTool"), ToolCont, CutCont, Thick, Angle);
		pShaped->GenInit();
		AddTool(pShaped);
	}
	return false;
}