
#pragma once

#include "NStateGeomCtrl.h"	// Added by ClassView
// NStateGeomPane.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// NStateGeomPane view
class NCUnitState;

class NStateGeomPane : public CDockablePane
{
protected:
	DECLARE_DYNCREATE(NStateGeomPane)
	CNCMDoc* GetDocument();

public:
	NStateGeomPane();
	virtual ~NStateGeomPane();
	void Update();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NStateGeomPane)
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
	NStateGeomCtrl GeomCtrl;
	void UpdateGeom();
	//{{AFX_MSG(NStateGeomPane)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
