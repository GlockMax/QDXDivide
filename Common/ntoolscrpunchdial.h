#if !defined(AFX_NTOOLSCRPUNCHDIAL_H__AC47AF04_37A2_11D4_8F89_000021667618__INCLUDED_)
#define AFX_NTOOLSCRPUNCHDIAL_H__AC47AF04_37A2_11D4_8F89_000021667618__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NToolsCrPunchDial.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// NToolsCrPunchDial dialog

class NToolsCrPunchDial : public CDialog
{
// Construction
public:
	NToolsCrPunchDial(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(NToolsCrPunchDial)
	enum { IDD = IDD_TOOLS_CREATE_PUNCH };
	CString	m_name;
	CString	m_pos;
	UINT	m_PointSize;
	double	m_displacement;
	double	m_H;
	double	m_R;
	double	m_X;
	double	m_Y;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NToolsCrPunchDial)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(NToolsCrPunchDial)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NTOOLSCRPUNCHDIAL_H__AC47AF04_37A2_11D4_8F89_000021667618__INCLUDED_)
