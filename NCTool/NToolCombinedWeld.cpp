#include "stdafx.h"
#include "NToolWeld.h"
#include "NToolsConCrWeldPage.h"
#include "NToolCombinedWeld.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(NToolCombinedWeld, NToolCombined, 0)

NToolCombinedWeld::NToolCombinedWeld(void) 
{
}
NToolCombinedWeld::NToolCombinedWeld(const CString &Name)
 : NToolCombined(Name)
{
	CompShiftAuto = false;
}

NToolCombinedWeld::~NToolCombinedWeld(void)
{
}
NToolCombinedWeld* NToolCombinedWeld::CreateCopyTool() const
{
	NToolCombinedWeld *pCreatedTool = new NToolCombinedWeld();

	pCreatedTool->CompShift = CompShift;
	pCreatedTool->CompShiftAuto = CompShiftAuto;
	pCreatedTool->LifeTime = LifeTime;


	for (int i = 0; i < num_tools; ++i)
		pCreatedTool->AddTool(GetTool(i)->CreateCopyTool());

	NTool *pTool = NULL;
	if(IsNameAuto && num_tools > 0)
	{
		pTool = GetTool(0);
		for(int i = 1; i < num_tools; ++i)
		{
			NTool *pTTool = GetTool(i);
			if(pTTool->GetZdisp() < pTool->GetZdisp())
				pTool = pTTool;
		}
	}
	if (pTool)
		pCreatedTool->SetName(pTool->GetName());

	return pCreatedTool;
}
void NToolCombinedWeld::FillDialog(CDialog *pDial) const
{
	NToolCombined::FillDialog(pDial);

	NToolsConCrWeldPage *pD = (NToolsConCrWeldPage *)pDial;
	pD->SetToolCombined(this);
	
	pD->m_displacement = Zdisp;
}

void NToolCombinedWeld::SetConeLength(double Length)
{
	if(num_tools < 1)
		return;
	NTool *pTool = GetTool(0);
	if(!IsKindOf(RUNTIME_CLASS(NToolWeld)))
		return;
	((NToolWeld *)pTool)->SetLength(Length);
}

const BPoint NToolCombinedWeld::GetDirection(void) const
{
	if(num_tools < 1)
		return BPoint(0., 0., -1., 0.);
	NTool *pTool = GetTool(0);
	if(!pTool->IsKindOf(RUNTIME_CLASS(NToolWeld)))
		return BPoint(0., 0., -1., 0.);
	return ((NToolWeld *)pTool)->GetDirection();
}
BPoint NToolCombinedWeld::GetCompVect(void) const
{
	return CompShift;
}
