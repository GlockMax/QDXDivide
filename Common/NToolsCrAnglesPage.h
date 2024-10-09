#pragma once
// NToolsCrAnglesPage.h : header file
//
#include "NFloatEdit.h"
#include "NToolsCrCommonPage.h"

/////////////////////////////////////////////////////////////////////////////
// NToolsCrAnglesPage dialog

class NToolsCrAnglesPage : public NToolsCrCommonPage
{
	DECLARE_DYNCREATE(NToolsCrAnglesPage)

// Construction
public:
	bool IsOkInput();
	void IsError(int IDErr);
	bool IsNameAuto;
	CString& GetName();
	NToolsCrAnglesPage();
	~NToolsCrAnglesPage();

// Dialog Data
	//{{AFX_DATA(NToolsCrAnglesPage)
	enum { IDD = IDD_TOOLS_CREATE_ANGLES };
	CStatic	m_Draw2;
	CStatic	m_Draw1;
	NFloatEdit	m_h2_ctrl;
	NFloatEdit	m_h1_ctrl;
	NFloatEdit	m_b_ctrl;
	NFloatEdit	m_alf2_ctrl;
	NFloatEdit	m_diam_ctrl;
	CEdit	m_name_ctrl;
	NFloatEdit	m_r_ctrl;
	NFloatEdit	m_faset_ctrl;
	NFloatEdit	m_alf1_ctrl;
	double	m_alf1;
	double	m_alf2;
	double	m_faset;
	double	m_r;
	double	m_b;
	BOOL	m_check;
	double	m_diam;
	double	m_displacement;
	NFloatEdit	m_displacement_ctrl;
	CString	m_name;
	double	m_h1;
	double	m_h2;
	BOOL	m_check2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(NToolsCrAnglesPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString GenName();
	// Generated message map functions
	//{{AFX_MSG(NToolsCrAnglesPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeAnglesAlf1();
	afx_msg void OnChangeAnglesAlf2();
	afx_msg void OnChangeAnglesFaset();
	afx_msg void OnChangeAnglesR1();
	afx_msg void OnChangeDiskB();
	afx_msg void OnChangeDiskDiam();
	afx_msg void OnDiskCheck();
	afx_msg void OnDiskCheck2();
	afx_msg void OnKillfocusDiskName();
//	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
//	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
