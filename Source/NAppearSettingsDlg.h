#pragma once

// NAppearSettingsDlg.h : header file
//
#include "ResLocal.h"
#include "NCMProject.h"

/////////////////////////////////////////////////////////////////////////////
// NAppearSettingsDlg dialog
#define HIDC_APPEARANCE_ELEMENT		0x03
#define HIDC_COLOR					0x05


class NAppearSettingsDlg : public CMFCPropertyPage
{
	DECLARE_DYNCREATE(NAppearSettingsDlg)

// Construction
public:
	NAppearSettingsDlg();
	~NAppearSettingsDlg();

// Variables
public:
	int cyPixelsPerInch;
	static CString DefaultElement;
// Function
public:

// Dialog Data
	//{{AFX_DATA(NAppearSettingsDlg)
	enum { IDD = IDD_APPEARANCE_SETTINGS };
	CButton	m_ColorCtrl;
	CStatic	m_PictureCtrl;
	CComboBox	m_ElementCtrl;
	CString	m_Element;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(NAppearSettingsDlg)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	void UpdateColorButton(COLORREF NewColor);
	CMapStringToNColor ColorsBuffer;
	CBitmap ColorButtonBmp;
	void ElementChanged();
	// Generated message map functions
	//{{AFX_MSG(NAppearSettingsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeAppearanceElement();
	afx_msg void OnColor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL m_Grad;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

