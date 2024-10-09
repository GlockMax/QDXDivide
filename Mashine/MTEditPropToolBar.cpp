#include "stdafx.h"
#include "MTEditPropToolBar.h"
#include "MTEditPane.h"

IMPLEMENT_DYNAMIC(MTEditPropToolBar, CMFCToolBar)

BEGIN_MESSAGE_MAP(MTEditPropToolBar, CMFCToolBar)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

void MTEditPropToolBar::OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
{
	CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
}
void MTEditPropToolBar::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar)
{
	CWnd *pParent;
	pParent = GetParent();

	((MTEditPane*)pParent)->OnSlider();
}
BOOL MTEditPropToolBar::OnUserToolTip(CMFCToolBarButton *pButton, CString & strTTText) const
{
	switch (pButton->GetImage())
	{
	case 0:
		strTTText.LoadString(IDS_PROP_TOOLBAR_TOOLTIP0);
		return TRUE;
		break;
	case 1:
		strTTText.LoadString(IDS_PROP_TOOLBAR_TOOLTIP1);
		return TRUE;
		break;
	case 2:
		strTTText.LoadString(IDS_PROP_TOOLBAR_TOOLTIP3);
		return TRUE;
		break;
	case 3:
		strTTText.LoadString(IDS_PROP_TOOLBAR_TOOLTIP2);
		return TRUE;
		break;
	case 4:
		strTTText.LoadString(IDS_PROP_TOOLBAR_TOOLTIP4);
		return TRUE;
		break;
	default:
		return FALSE;
		break;
	}//switch
	return FALSE;
}//BOOL MTEditPropToolBar::OnUserToolTip(CMFCToolBarButton *pButton, CString & strTTText) const