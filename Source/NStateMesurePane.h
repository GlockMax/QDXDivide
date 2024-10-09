
#pragma once

#include "NStateMeasureCtrl.h"	// Added by ClassView
// NStateMesurePane.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// NStateMesurePane view

class NStateMesurePane : public CDockablePane
{
protected:
	DECLARE_DYNCREATE(NStateMesurePane)
	CNCMDoc* GetDocument();

public:
	NStateMesurePane();
	virtual ~NStateMesurePane();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NStateMesurePane)
	public:
	virtual void OnUpdate(CDockablePane* pSender, LPARAM lHint, CObject* pHint);
	protected:
	//}}AFX_VIRTUAL

// Implementation
protected:
	CFont m_Font;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	NStateMeasureCtrl MeasureCtrl;
	void UpdateMeasure();
	//{{AFX_MSG(NStateMesurePane)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
