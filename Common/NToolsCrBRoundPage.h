#pragma once
// NToolsCrBRoundPage.h : header file
//
#include "NFloatEdit.h"
#include "NToolsCrCommonPage.h"

/////////////////////////////////////////////////////////////////////////////
// NToolsCrBRoundPage dialog

class NToolsCrBRoundPage : public NToolsCrCommonPage
{
	DECLARE_DYNCREATE(NToolsCrBRoundPage)

// Construction
public:
	bool IsOkInput();
	void IsError(int IDErr);
	bool IsNameAuto;
	CString& GetName();
	NToolsCrBRoundPage();
	~NToolsCrBRoundPage();

// Dialog Data
	//{{AFX_DATA(NToolsCrBRoundPage)
	enum { IDD = IDD_TOOLS_CREATE_BROUND };
	CStatic	m_Draw2;
	CStatic	m_Draw1;
	NFloatEdit	m_b_ctrl;
	NFloatEdit	m_diam_ctrl;
	CEdit	m_name_ctrl;
	NFloatEdit	m_r_ctrl;
	NFloatEdit	m_l_ctrl;
	NFloatEdit	m_h1_ctrl;
	double	m_r;
	double	m_b;
	BOOL	m_check;
	double	m_diam;
	double	m_h1;
	double	m_displacement;
	NFloatEdit	m_displacement_ctrl;
	CString	m_name;
	BOOL	m_check2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(NToolsCrBRoundPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString GenName();
	// Generated message map functions
	//{{AFX_MSG(NToolsCrBRoundPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeDiskB();
	afx_msg void OnChangeDiskDiam();
	afx_msg void OnDiskCheck();
	afx_msg void OnDiskCheck2();
	afx_msg void OnKillfocusDiskName();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	double m_l;
	afx_msg void OnEnChangeBroundR();
};
