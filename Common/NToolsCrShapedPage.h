#pragma once
// NToolsCrShapedPage.h : header file
//
#include "NFloatEdit.h"
#include "NToolsCrCommonPage.h"

/////////////////////////////////////////////////////////////////////////////
// NToolsCrShapedPage dialog

class NToolsCrShapedPage : public NToolsCrCommonPage
{
	DECLARE_DYNCREATE(NToolsCrShapedPage)

// Construction
public:
	bool IsOkInput();
	void IsError(int IDErr);
	bool IsNameAuto;
	CString& GetName();
	NToolsCrShapedPage();
	~NToolsCrShapedPage();

// Dialog Data
	//{{AFX_DATA(NToolsCrShapedPage)
	enum { IDD = IDD_TOOLS_CREATE_SHAPED };
	NFloatEdit	m_h2_ctrl;
	NFloatEdit	m_h1_ctrl;
	NFloatEdit	m_r4_ctrl;
	NFloatEdit	m_r3_ctrl;
	NFloatEdit	m_r2_ctrl;
	NFloatEdit	m_r1_ctrl;
	NFloatEdit	m_faset2_ctrl;
	NFloatEdit	m_faset1_ctrl;
	NFloatEdit	m_alf2_ctrl;
	NFloatEdit	m_alf1_ctrl;
	CEdit	m_name_ctrl;
	NFloatEdit	m_diam_ctrl;
	NFloatEdit	m_b_ctrl;
	double	m_b;
	double	m_diam;
	double	m_displacement;
	NFloatEdit	m_displacement_ctrl;
	CString	m_name;
	double	m_alf1;
	double	m_alf2;
	double	m_faset1;
	double	m_faset2;
	double	m_r1;
	double	m_r2;
	double	m_r3;
	double	m_r4;
	BOOL	m_check;
	double	m_h1;
	double	m_h2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(NToolsCrShapedPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString GenName();
	// Generated message map functions
	//{{AFX_MSG(NToolsCrShapedPage)
	afx_msg void OnChangeDiskDiam();
	afx_msg void OnChangeDiskB();
	afx_msg void OnChangeShapedR4();
	afx_msg void OnChangeShapedR3();
	afx_msg void OnChangeShapedR2();
	afx_msg void OnChangeShapedR1();
	afx_msg void OnChangeShapedFaset2();
	afx_msg void OnChangeShapedFaset1();
	afx_msg void OnChangeShapedAlf2();
	afx_msg void OnChangeShapedAlf1();
	afx_msg void OnDiskCheck4();
	virtual BOOL OnInitDialog();
	afx_msg void OnKillfocusDiskName();
//	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
//	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
