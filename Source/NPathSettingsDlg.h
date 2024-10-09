#pragma once

// NPathSettingsDlg.h : header file
//
#include "ResLocal.h"


/////////////////////////////////////////////////////////////////////////////
// NPathSettingsDlg dialog

class NPathSettingsDlg : public CMFCPropertyPage
{
	DECLARE_DYNCREATE(NPathSettingsDlg)

// Construction
public:
	NPathSettingsDlg();
	~NPathSettingsDlg();

// Dialog Data
	//{{AFX_DATA(NPathSettingsDlg)
	enum { IDD = IDD_PATH_SETTINGS };
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(NPathSettingsDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(NPathSettingsDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	CStatic m_PropListLocation;
	CMFCPropertyGridCtrl m_PropList;
	CMFCPropertyGridProperty* pGroupPath;
public:
	virtual void OnOK();
};

class NCustomDlgProp : public CMFCPropertyGridProperty
{
public:
	NCustomDlgProp(const CString& strName, const COleVariant& varValue, const CString& strDesc);

protected:
	virtual BOOL HasButton() const { return TRUE; }
	virtual void OnClickButton(CPoint point);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

