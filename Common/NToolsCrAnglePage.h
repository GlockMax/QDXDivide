#pragma once
// NToolsCrAnglePage.h : header file
//
#include "ResLocal.h"
#include "NFloatEdit.h"
#include "NToolsCrCommonPage.h"
/////////////////////////////////////////////////////////////////////////////
// NToolsCrAnglePage dialog

class NToolsCrAnglePage : public NToolsCrCommonPage
{
	DECLARE_DYNCREATE(NToolsCrAnglePage)

// Construction
public:
	bool IsOkInput();
	void IsError(int IDErr);
	bool IsNameAuto;
	CString& GetName();
	NToolsCrAnglePage();
	~NToolsCrAnglePage();

// Dialog Data
	//{{AFX_DATA(NToolsCrAnglePage)
	enum { IDD = IDD_TOOLS_CREATE_ANGLE };
	CStatic	m_Draw2;
	CStatic	m_Draw1;
	NFloatEdit	m_h2_ctrl;
	NFloatEdit	m_h1_ctrl;
	CEdit	m_name_ctrl;
	NFloatEdit	m_diam_ctrl;
	NFloatEdit	m_b_ctrl;
	NFloatEdit	m_r_ctrl;
	NFloatEdit	m_alf_ctrl;
	double	m_alf;
	double	m_r;
	double	m_b;
	double	m_diam;
	double	m_displacement;
	NFloatEdit	m_displacement_ctrl;
	CString	m_name;
	BOOL	m_check;
	double	m_h1;
	double	m_h2;
	BOOL	m_check2;
	BOOL	m_check_probe;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(NToolsCrAnglePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString GenName();
	// Generated message map functions
	//{{AFX_MSG(NToolsCrAnglePage)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeDiskDiam();
	afx_msg void OnChangeDiskB();
	afx_msg void OnChangeAngleAlf();
	afx_msg void OnChangeAngleR1();
	afx_msg void OnDiskCheck();
	afx_msg void OnDiskCheck2();
	afx_msg void OnKillfocusDiskName();
//	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
//	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
