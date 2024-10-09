#include "stdafx.h"
#include "NToolsCrTurn.h"
#include "ntoolturnholder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

NToolTurnHolder::NToolTurnHolder(void)
{
	SetType(60);// Turn tool
}

NToolTurnHolder::~NToolTurnHolder(void)
{
}
void NToolTurnHolder::Draw(RenderMode GlobRMode, int step)
{
}
// создание копии инструмента
NToolTurnHolder* NToolTurnHolder::CreateCopyTool()
{
	NToolTurnHolder *pCreatedTool = new NToolTurnHolder;

	return pCreatedTool;
}
void NToolTurnHolder::FillPage(CDialog *pPage)
{
	if(!pPage->IsKindOf(RUNTIME_CLASS(NToolsCrTurn)))
		return;// error

	NToolTurn::FillPage(pPage);

	NToolsCrTurn *pLoc = (NToolsCrTurn *)pPage;

	pLoc->m_name = Name;
	pLoc->m_Xdisp = Displacement.GetX();
	pLoc->m_Zdisp = Displacement.GetY();
	pLoc->m_MainAngle = MainAngle;
}
void NToolTurnHolder::FillFromPage(CDialog *pPage)
{
	if(!pPage->IsKindOf(RUNTIME_CLASS(NToolsCrTurn)))
		return;// error

	NToolTurn::FillFromPage(pPage);

	NToolsCrTurn *pLoc = (NToolsCrTurn *)pPage;

	Name = pLoc->m_name;
	Displacement.Set(pLoc->m_Xdisp, pLoc->m_Zdisp, 0., 0.);
	MainAngle = pLoc->m_MainAngle;
}
