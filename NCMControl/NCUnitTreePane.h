
#pragma once

#include "NCUnitTreeCtrl.h"
#include "NCUnitDef.h"
#include "NCTreeCtrlSnapshot.h"

class CNCEditor2Doc;

class CClassToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
	virtual BOOL OnUserToolTip(CMFCToolBarButton *pButton, CString & strTTText) const
	{
		switch (pButton->GetImage())
		{
		case 0:
			strTTText.LoadString(IDS_TREE_TOOLBAR_TOOLTIP0);
			return TRUE;
			break;
		case 1:
			strTTText.LoadString(IDS_TREE_TOOLBAR_TOOLTIP1);
			return TRUE;
			break;
		case 2:
			strTTText.LoadString(IDS_TREE_TOOLBAR_TOOLTIP2);
			return TRUE;
			break;
		case 3:
			strTTText.LoadString(IDS_TREE_TOOLBAR_TOOLTIP3);
			return TRUE;
			break;
		case 4:
			strTTText.LoadString(IDS_TREE_TOOLBAR_TOOLTIP4);
			return TRUE;
			break;
		default:
			return FALSE;
			break;
		}//switch
	return FALSE;
	};
};

/// Панель с деревом документа
class CNCUnitTreePane : public CDockablePane
{
public:
	CNCUnitTreePane();
	virtual ~CNCUnitTreePane();

	void AdjustLayout();
	void OnChangeVisualStyle();

protected:
	CClassToolBar m_wndToolBar;
	CNCUnitTreeCtrl m_wndTreeView;
	CImageList m_ClassViewImages;

	CNCUnitDef *m_unitDef;
	/// Варианты отображения дерева
	bool ShowPresenceOnly;
	bool ShowAlphaSort;
	bool ShowGroups;

	int ExpandState;

	/// Примерное заполнение дерева
	/** \deprecated */
	void FillClassView();

// Overrides
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	/// Отображение структуры файла конфигурации в панели дерева

	void ShowNCUnitDef(CNCUnitDef * unitDef, CNCTreeCtrlSnapshot *snapshot = NULL);
protected:
	void ShowNCUnitDefInternal(CNCUnitDef * unitDef, CNCTreeCtrlSnapshot *snapshot = NULL);
	void RebuildTree();
public:
	void GetTreeCtrlSnapshot(CNCTreeCtrlSnapshot &snapshot);

	CNCEditor2Doc * GetDocument();
	void SetInternalSelect(bool IS);
protected:
	void ApplySnapshot(CNCTreeCtrlSnapshot *snapshot);

	void Expand();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnClassAddMemberFunction();
	afx_msg void OnClassAddMemberVariable();
	afx_msg void OnClassDefinition();
	afx_msg void OnClassProperties();
	afx_msg void OnChangeXML();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnChangeActiveTab(WPARAM, LPARAM);

	afx_msg void OnTBExpand();
	afx_msg void OnTBPres();
	afx_msg void OnTBABC();
	afx_msg void OnTBGroup();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnUpdateExpand(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePres(CCmdUI *pCmdUI);
	afx_msg void OnUpdateABC(CCmdUI *pCmdUI);
	afx_msg void OnUpdateGroup(CCmdUI *pCmdUI);
	virtual BOOL DestroyWindow();
};

