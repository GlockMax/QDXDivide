#pragma once

/// Тулбар для панели свойств со скроллом
class AFX_EXT_CLASS MTEditPropToolBar : public CMFCToolBar
{
	DECLARE_DYNAMIC(MTEditPropToolBar)

	DECLARE_MESSAGE_MAP()
public:
	MTEditPropToolBar():CMFCToolBar() {};
	virtual ~MTEditPropToolBar() {}; 
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler);


	virtual BOOL AllowShowOnList() const { return FALSE; }

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
	virtual BOOL OnUserToolTip(CMFCToolBarButton *pButton, CString & strTTText) const;
};