#pragma once

#include "NStateUnitCtrl.h"
// NStateParamsPane.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// NStateParamsPane view

class NStateParamsPane : public CDockablePane
{
protected:
	DECLARE_DYNCREATE(NStateParamsPane)
	class CNCMDoc* GetDocument();

// Attributes
public:
	NMapStringToOb * GetAllElemList();

// Operations
public:
	NStateParamsPane();
	virtual ~NStateParamsPane();
	void GetUnvisElemList(CStringArray & UnvisElemList);
	CStringArray & GetVisElemList();
	void Update();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NStateParamsPane)
	public:
	//virtual void InitialUpdate();
	virtual void OnUpdate(CDockablePane* pSender, LPARAM lHint, CObject* pHint);
	protected:
	//}}AFX_VIRTUAL

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	NStateUnitCtrl lc;
	CStringArray VisElemList;
	NMapStringToOb AllElemList;
protected:
	void UpdateControl();
	virtual void CreateStateView();
	virtual void DeleteElements();
	virtual void ResetElements(void);
	virtual BOOL LoadState(LPCTSTR lpszProfileName = NULL, int nIndex = -1, UINT uiID = (UINT) -1);
	virtual BOOL SaveState(LPCTSTR lpszProfileName = NULL, int nIndex = -1, UINT uiID = (UINT) -1);

//	CImageList ImageList;
	//{{AFX_MSG(NStateParamsPane)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnHideItem();
	afx_msg void OnStateViewSettings();
	afx_msg void OnShowWindow( BOOL bShow, UINT nStatus );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	CString GetExportString(bool Title = false, bool VisibleOnly = true, CStringArray *pElemList = NULL);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
