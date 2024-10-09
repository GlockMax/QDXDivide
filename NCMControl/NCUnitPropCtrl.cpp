#include "StdAfx.h"
#include "NCUnitPropCtrl.h"
#include "PropertiesWnd.h"
#include "NCEditor2Doc.h"

BEGIN_MESSAGE_MAP(CNCUnitPropCtrl, CMFCPropertyGridCtrl)
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

CNCUnitPropCtrl::CNCUnitPropCtrl(void)
{
}

CNCUnitPropCtrl::~CNCUnitPropCtrl(void)
{
}

void CNCUnitPropCtrl::OnPropertyChanged(CMFCPropertyGridProperty* pProp) const
{
	if(pProp->GetOriginalValue() == pProp->GetValue())
	{
		CMFCPropertyGridCtrl::OnPropertyChanged(pProp);
		return;
	}//if

	((CPropertiesWnd*)GetParent())->GetDocument()->SetInternalSelect(true);

	CMFCPropertyGridCtrl::OnPropertyChanged(pProp);
	((CPropertiesWnd*)GetParent())->OnPropertyChanged(pProp);

	((CPropertiesWnd*)GetParent())->GetDocument()->SetInternalSelect(false);
}//void CNCUnitPropCtrl::OnPropertyChanged(CMFCPropertyGridProperty* pProp) const

void CNCUnitPropCtrl::OnChangeSelection(CMFCPropertyGridProperty* pNewSel, CMFCPropertyGridProperty* pOldSel)
{
	if(pNewSel == NULL)
		return;

	((CPropertiesWnd*)GetParent())->OnChangeSelection(pNewSel);
}//void CNCUnitPropCtrl::OnChangeSelection(CMFCPropertyGridProperty* pNewSel, CMFCPropertyGridProperty* pOldSel)

void CNCUnitPropCtrl::OnKillFocus(CWnd* pNewWnd)
{
	//if(pNewWnd->IsKindOf(RUNTIME_CLASS(CNCUnitTreeCtrl)))
	//	return;
	CMFCPropertyGridCtrl::OnKillFocus(pNewWnd);

	// TODO: Add your message handler code here
}