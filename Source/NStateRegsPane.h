
#pragma once

#include "NStateRegsCtrl.h"	// Added by ClassView
// NStateRegsPane.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// NStateRegsPane view
class NCUnitState;

class NStateRegsPane : public CDockablePane
{
protected:
	DECLARE_DYNCREATE(NStateRegsPane)
	CNCMDoc* GetDocument();


public:
	NStateRegsPane();
	virtual ~NStateRegsPane();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NStateRegsPane)
	public:
	virtual void OnUpdate(CDockablePane* pSender, LPARAM lHint, CObject* pHint);
	protected:
	//}}AFX_VIRTUAL

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	NStateRegsCtrl RegsCtrl;
	void UpdateRegs();
	//{{AFX_MSG(NStateRegsPane)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
