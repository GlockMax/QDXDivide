
#pragma once

// NOGLPane.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// NOGLPane view

#include "BOpenGLWnd.h"

class NOGLPane : public CDockablePane
{
	DECLARE_SERIAL(NOGLPane);
	static const int FixedPanesNum = 2;
public:
	NOGLPane(class NOpenGLWnd* pWnd, int WID, bool ShowOnMenu = true);
	NOGLPane(bool StockType = true, int WID = 0, bool ShowOnMenu = true);
	virtual ~NOGLPane();
	void SerializeMatrices(CArchive & ar);
	void NAnimateUpdate(void);
	virtual void LoadTBFromIcons(void);
	virtual BOOL AllowShowOnPaneMenu() const;
	void SetShowOnPaneMenu(BOOL Val) { ShowOnPaneMenu = Val; }
	bool HasFocus(void);
	int GetID() const { return WinIDs[WndID]; }
	static int GetFixedPanesNum() { return FixedPanesNum; }
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NOGLPane)
public:
	virtual void OnUpdate(CDockablePane* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL
	static int GetMaxWinNum() { return MaxWinNum; }
protected:
	NOpenGLWnd* pOpenGLWnd;
	NOpenGLWnd* pOpenGLWndStock;
	NOpenGLWnd* pOpenGLWndMach;
	int WndID;
	BOOL ShowOnPaneMenu; 
	static const int MaxWinNum = 15;// the number of IDs in NOGLPane::WinIDs[] minus 1 for main window
	static int WinIDs[MaxWinNum + 1];
// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(NOGLPane)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnChangeType();
	afx_msg void OnUpdateChangeType(CCmdUI* pCmdUI);
	BOOL LoadState(LPCTSTR lpszProfileName = NULL, int nIndex = -1, UINT uiID = (UINT)-1) override;
	BOOL SaveState(LPCTSTR lpszProfileName = NULL, int nIndex = -1, UINT uiID = (UINT)-1) override;

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnVolumeTree();
	NOpenGLWnd *GetOpenGLWnd(void) {return pOpenGLWnd;}
	BOOL IsPaneVisible() const { if (m_hWnd == NULL) return FALSE;  return CDockablePane::IsPaneVisible(); }
	void SetType(bool StockType);
	const CString MakeTitle();
	bool GetType() const;
	void SetRotCenter(const BOpenGLWnd::NRotCenter& iRotCenter);
	bool CreateFirst();
	static NOGLPane* MakeOGLPane(bool StockType, int WID, CWnd* pParentWnd, bool HasGripper = true, bool ShowOnMenu = true);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
