#pragma once
#include "afxdockablepane.h"
#include "PropEditor.h"
#include "PropGridCtrl.h"

class AFX_EXT_CLASS PropPane : public CDockablePane
{
public:
	PropPane();
	void Clear();
	void OnUpdate(CDockablePane* pSender, LPARAM lHint, CObject* pHint);
	void Paste(PropList *,PropSingle *,int );
	PropList * L1;
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}
	PropEditor &GetEditor(void) { return Editor;}
	void LoadStringID(CString &Str, int ID);
	CString GetTimeFromDouble(double );
	virtual ~PropPane();
	void AdjustLayout();
	PAbstractObject * GetAbsObject(void);
protected:
	PropEditor Editor;
	PropGridCtrl m_wndPropList;
	int idChanged;
	int isChanged;
	int ChGroup;
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnShowWindow( BOOL bShow, UINT nStatus );
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	DECLARE_MESSAGE_MAP()

	void InitPropList();
	void SetSettings();
};