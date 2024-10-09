
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "NCUnitHelpPane.h"
#include "NCUnitTreePane.h"
#include "OutputWnd.h"
#include "PropertiesWnd.h"

class CMainFrame : public CFrameWndEx
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	/// Получение указателя на плавающую панель с деревом
	CNCUnitTreePane* GetTreeView();
	/// Получение указателя на панель свойств
	CPropertiesWnd* GetPropertiesWnd();
	/// Получение указателя на панель помощи
	CNCUnitHelpPane * GetHelpPane();

protected:  // control bar embedded members
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	CNCUnitHelpPane   m_wndHelpPane;
	CNCUnitTreePane   m_wndClassView;
	COutputWnd        m_wndOutput;
	CPropertiesWnd    m_wndProperties;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
public:
//	afx_msg void OnEditUndo();
	afx_msg void OnEditUndo1();
	afx_msg void OnEditRedo1();
	afx_msg void OnUpdateEditUndo1(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditRedo1(CCmdUI *pCmdUI);
	afx_msg void OnClose();
	virtual BOOL DestroyWindow();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};


