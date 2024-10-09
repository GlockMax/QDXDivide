#pragma once
// NToolsCrFasetPage.h : header file
//
#include "NToolsCrCommonPage.h"
#include "NFloatEdit.h"

/////////////////////////////////////////////////////////////////////////////
// NToolsCrFasetPage dialog

class NToolsCrFasetPage : public NToolsCrCommonPage
{
	DECLARE_DYNCREATE(NToolsCrFasetPage)

// Construction
public:
	bool IsOkInput();
	void IsError(int IDErr);
	bool IsNameAuto;
	CString& GetName();
	NToolsCrFasetPage();
	~NToolsCrFasetPage();

// Dialog Data
	//{{AFX_DATA(NToolsCrFasetPage)
	enum { IDD = IDD_TOOLS_CREATE_FASET };
	NFloatEdit	m_h2_ctrl;
	NFloatEdit	m_h1_ctrl;
	CStatic	m_Draw2;
	CStatic	m_Draw1;
	CEdit	m_name_ctrl;
	NFloatEdit	m_faset2_ctrl;
	NFloatEdit	m_faset1_ctrl;
	NFloatEdit	m_alf2_ctrl;
	NFloatEdit	m_alf1_ctrl;
	NFloatEdit	m_diam_ctrl;
	NFloatEdit	m_b_ctrl;
	double	m_b;
	double	m_diam;
	BOOL	m_check1;
	BOOL	m_check2;
	double	m_displacement;
	NFloatEdit	m_displacement_ctrl;
	CString	m_name;
	double	m_alf1;
	double	m_alf2;
	double	m_faset1;
	double	m_faset2;
	BOOL	m_check3;
	double	m_h1;
	double	m_h2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(NToolsCrFasetPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString GenName();
	// Generated message map functions
	//{{AFX_MSG(NToolsCrFasetPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeDiskB();
	afx_msg void OnDiskCheck1();
	afx_msg void OnDiskCheck2();
	afx_msg void OnChangeDiskDiam();
	afx_msg void OnChangeFasetAlf1();
	afx_msg void OnChangeFasetAlf2();
	afx_msg void OnChangeFasetFaset1();
	afx_msg void OnChangeFasetFaset2();
	afx_msg void OnDiskCheck3();
	afx_msg void OnKillfocusDiskName();
//	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
//	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
