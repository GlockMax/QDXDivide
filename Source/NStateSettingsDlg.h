#pragma once

// NStateSettingsDlg.h : header file
//
#include "ResLocal.h"
/////////////////////////////////////////////////////////////////////////////
// NStateSettingsDlg dialog

class NStateSettingsDlg : public CMFCPropertyPage
{
	DECLARE_DYNCREATE(NStateSettingsDlg)

// Construction
public:
	NStateSettingsDlg();
	~NStateSettingsDlg();

// Dialog Data
	//{{AFX_DATA(NStateSettingsDlg)
	enum { IDD = IDD_STATE_SETTINGS };
	CListBox	VisElemListBox;
	CListBox	UnvisElemListBox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(NStateSettingsDlg)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	class NStateParamsPane * pParamsPane;
	// Generated message map functions
	//{{AFX_MSG(NStateSettingsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnAddElem();
	afx_msg void OnDelElem();
	afx_msg void OnUp();
	afx_msg void OnDown();
//	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
//	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void SetParamsPane(class NStateParamsPane &ParamsPane);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

