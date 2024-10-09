#pragma once
// NToolsCrRoundPage.h : header file
//
#include "NFloatEdit.h"
#include "NToolsCrCommonPage.h"

/////////////////////////////////////////////////////////////////////////////
// NToolsCrRoundPage dialog

class NToolsCrRoundPage : public NToolsCrCommonPage
{
	DECLARE_DYNCREATE(NToolsCrRoundPage)

// Construction
public:
	bool IsOkInput();
	void IsError(int IDErr);
	bool IsNameAuto;
	CString& GetName();
	NToolsCrRoundPage();
	~NToolsCrRoundPage();

// Dialog Data
	//{{AFX_DATA(NToolsCrRoundPage)
	enum { IDD = IDD_TOOLS_CREATE_ROUND };
	NFloatEdit	m_h2_ctrl;
	NFloatEdit	m_h1_ctrl;
	CStatic	m_Draw2;
	CStatic	m_Draw1;
	NFloatEdit	m_r1_ctrl;
	CEdit	m_name_ctrl;
	NFloatEdit	m_r3_ctrl;
	NFloatEdit	m_r2_ctrl;
	NFloatEdit	m_diam_ctrl;
	NFloatEdit	m_b_ctrl;
	double	m_b;
	BOOL	m_check1;
	BOOL	m_check_probe;
	double	m_diam;
	double	m_displacement;
	NFloatEdit	m_displacement_ctrl;
	CString	m_name;
	double	m_r2;
	double	m_r3;
	double	m_r1;
	BOOL	m_check2;
	double	m_h1;
	double	m_h2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(NToolsCrRoundPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString GenName();
	// Generated message map functions
	//{{AFX_MSG(NToolsCrRoundPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeDiskB();
	afx_msg void OnChangeDiskDiam();
	afx_msg void OnDiskCheck1();
	afx_msg void OnChangeRoundR2();
	afx_msg void OnChangeRoundR3();
	afx_msg void OnDiskCheck2();
	afx_msg void OnKillfocusDiskName();
//	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
//	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


};
