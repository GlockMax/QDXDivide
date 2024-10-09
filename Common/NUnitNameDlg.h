#pragma once
// NUnitNameDlg.h : header file
//
#include "ResLocal.h"

/////////////////////////////////////////////////////////////////////////////
// NUnitNameDlg dialog
#define HIDC_UNIT_NAME				0x01516

class AFX_EXT_CLASS NUnitNameDlg : public CDialog
{
// Construction
public:
	INT_PTR DoModal(const CString &path, const CString &name);
	NUnitNameDlg(CWnd* pParent = NULL);   // standard constructor
protected:
	CString m_path;

// Dialog Data
	//{{AFX_DATA(NUnitNameDlg)
public:
	enum { IDD = IDD_UNIT_NAME };
	CString	m_name;
protected:
	CListBox	m_ListBox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NUnitNameDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(NUnitNameDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkUnitName();
//	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
//	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
