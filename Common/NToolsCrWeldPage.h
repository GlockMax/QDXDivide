#pragma once
// NToolsCrWeldPage.h : header file
//
#include "ResLocal.h"
#include "NFloatEdit.h"

/////////////////////////////////////////////////////////////////////////////
// NToolsCrWeldPage dialog

class NToolsCrWeldPage : public CPropertyPage
{
	DECLARE_DYNCREATE(NToolsCrWeldPage)

// Construction
public:
	bool IsOkInput();
	void IsError(int IDErr);
	bool IsNameAuto;
	CString& GetName();
	NToolsCrWeldPage();
	~NToolsCrWeldPage();
	class NToolWeld *CreateTool();

// Dialog Data
	//{{AFX_DATA(NToolsCrWeldPage)
	enum { IDD = IDD_TOOLS_CREATE_WELD };
	double	m_displacement;
	NFloatEdit	m_displacement_ctrl;
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
	//{{AFX_VIRTUAL(NToolsCrWeldPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString GenName();
	// Generated message map functions
	//{{AFX_MSG(NToolsCrWeldPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnKillfocusName();
//	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
//	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	NFloatEdit m_focus_ctrl;
	double m_focus;
	afx_msg void OnEnKillfocusWeldFocus();
	afx_msg void OnEnKillfocusWeldDiam();
};
