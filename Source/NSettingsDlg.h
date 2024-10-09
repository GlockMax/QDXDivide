#pragma once
// NSettingsDlg.h : header file
//
#include "NAppearSettingsDlg.h"
#include "NStateSettingsDlg.h"
#include "NGraphSettingsDlg.h"
#include "NPathSettingsDlg.h"
#include "NViewInterfaceSettingsDlg.h"

/////////////////////////////////////////////////////////////////////////////
// NSettingsDlg

class NSettingsDlg : public CMFCPropertySheet
{
	DECLARE_DYNAMIC(NSettingsDlg)

// Construction
public:

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NSettingsDlg)
	public:
	virtual BOOL OnInitDialog();
	virtual INT_PTR DoModal();
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	NViewInterfaceSettingsDlg ViewInterfaceSettingsDlg;
	NAppearSettingsDlg AppearSettDlg;
	NStateSettingsDlg StateSettDlg;
	NGraphSettingsDlg GraphSettDlg;
	NPathSettingsDlg PathSettDlg;
	NSettingsDlg();
	virtual ~NSettingsDlg();

	// Generated message map functions
protected:
	//{{AFX_MSG(NSettingsDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

