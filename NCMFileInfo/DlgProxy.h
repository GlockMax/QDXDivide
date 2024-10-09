// DlgProxy.h : header file
//

#if !defined(AFX_DLGPROXY_H__679A9CD0_C2D0_11D5_B0AC_0050BF4A8273__INCLUDED_)
#define AFX_DLGPROXY_H__679A9CD0_C2D0_11D5_B0AC_0050BF4A8273__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNCMFileInfoDlg;

/////////////////////////////////////////////////////////////////////////////
// CNCMFileInfoDlgAutoProxy command target

class CNCMFileInfoDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CNCMFileInfoDlgAutoProxy)

	CNCMFileInfoDlgAutoProxy();           // protected constructor used by dynamic creation

// Attributes
public:
	CNCMFileInfoDlg* m_pDialog;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNCMFileInfoDlgAutoProxy)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CNCMFileInfoDlgAutoProxy();

	// Generated message map functions
	//{{AFX_MSG(CNCMFileInfoDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CNCMFileInfoDlgAutoProxy)

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CNCMFileInfoDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROXY_H__679A9CD0_C2D0_11D5_B0AC_0050BF4A8273__INCLUDED_)
