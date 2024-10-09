#if !defined(AFX_RINSPARAMDLG_H__3E2B5EF8_B4C0_4C22_B685_BDCC21EEA275__INCLUDED_)
#define AFX_RINSPARAMDLG_H__3E2B5EF8_B4C0_4C22_B685_BDCC21EEA275__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RInsParamDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// RInsParamDlg dialog

class RInsParamDlg : public CDialog
{
// Construction
public:
	RInsParamDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(RInsParamDlg)
	enum { IDD = IDD_REPORTS_INSPARAM };
	CString	m_Sym;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RInsParamDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(RInsParamDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RINSPARAMDLG_H__3E2B5EF8_B4C0_4C22_B685_BDCC21EEA275__INCLUDED_)
