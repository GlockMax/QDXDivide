// DHtmlExploreDlg.h : header file
//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Classes Reference and related electronic
// documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft C++ Libraries products.

#pragma once
#include "ResLocal.h"
#include "ResInv.h"
#include "AFXDHtml.h"
/////////////////////////////////////////////////////////////////////////////
// CDHtmlExploreDlg dialog

class CDHtmlExploreDlg : public CDHtmlDialog
{
// Construction
public:
	CDHtmlExploreDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDHtmlExploreDlg)
	enum { IDD = IDD_DHTMLEXPLORE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDHtmlExploreDlg)
	CString strProgs;
	CString strTools;
	CString strAll;
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	void RefreshDir();
	CString m_strCurrDir;
	void OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl);


	CComPtr<IHTMLElement> m_spCurrElement;
	CString m_strCtxFileName;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDHtmlExploreDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
