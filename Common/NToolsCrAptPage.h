#pragma once
// NToolsCrAptPage.h : header file
//
#include "NFloatEdit.h"
#include "NToolsCrCommonPage.h"

/////////////////////////////////////////////////////////////////////////////
// NToolsCrAptPage dialog

class NToolsCrAptPage : public NToolsCrCommonPage
{
	DECLARE_DYNCREATE(NToolsCrAptPage)

// Construction
public:
	bool IsOkInput();
	void IsError(int IDErr);
	CString& GetName();
	bool IsNameAuto;
	NToolsCrAptPage();
	~NToolsCrAptPage();

// Dialog Data
	//{{AFX_DATA(NToolsCrAptPage)
	enum { IDD = IDD_TOOLS_CREATE_APT7 };
	CEdit	m_name_ctrl;
	NFloatEdit	m_hm_ctrl;
	NFloatEdit	m_h_ctrl;
	NFloatEdit	m_diam_ctrl;
	NFloatEdit	m_r_ctrl;
	NFloatEdit	m_f_ctrl;
	NFloatEdit	m_e_ctrl;
	NFloatEdit	m_beta_ctrl;
	NFloatEdit	m_alf_ctrl;
	CButton	m_radio1_ctrl;
	CButton	m_radio2_ctrl;
	double	m_alf;
	double	m_beta;
	double	m_e;
	double	m_f;
	double	m_r;
	BOOL	m_check1;
	BOOL	m_check2;
	BOOL	m_radio1;
	BOOL	m_radio2;	
	double	m_diam;
	double	m_h;
	double	m_displacement;
	CString	m_name;
	double	m_hm;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	protected:
	//{{AFX_VIRTUAL(NToolsCrAptPage)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString GenName();
	// Generated message map functions
	//{{AFX_MSG(NToolsCrAptPage)
	afx_msg void OnChangeMillDiam();
	afx_msg void OnChangeMillH();
	afx_msg void OnChangeAptAlf1();
	afx_msg void OnChangeAptAlf2();
	afx_msg void OnChangeAptR();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnDiskCheck1();
	afx_msg void OnDiskCheck2();
	afx_msg void OnKillfocusDiskName();
//	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
//	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
