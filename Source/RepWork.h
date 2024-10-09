#if !defined(AFX_REPWork_H__BCF44E84_494B_11D5_8CE5_609173C10000__INCLUDED_)
#define AFX_REPWork_H__BCF44E84_494B_11D5_8CE5_609173C10000__INCLUDED_

#include "resource.h"
#include "Reports.h"	// Added by ClassView

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RepWork.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// RepWork dialog

class RepWork : public CDialog
{
// Construction
public:
	bool Active;
	void RemoveFromOutList(int n);
	void AddToOutList(CString str);
	int NumOutItems;
//	Reports WorkRep;
	CString *OutList;
//	CString *m_InList;
	RepWork(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(RepWork)
	enum { IDD = IDD_REPORTS_REPORTS };
	CString	m_ForAdd;
	CString	m_ForRemove;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RepWork)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void ReNew();

	// Generated message map functions
	//{{AFX_MSG(RepWork)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnButtonLoadReport();
	afx_msg void OnExit();
	afx_msg void OnButtonUnloadReport();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPWork_H__BCF44E84_494B_11D5_8CE5_609173C10000__INCLUDED_)
