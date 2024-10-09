#pragma once

// NGraphSettingsDlg.h : header file
//
#include "ResLocal.h"

/////////////////////////////////////////////////////////////////////////////
// NGraphSettingsDlg dialog

class NGraphSettingsDlg : public CMFCPropertyPage
{
	DECLARE_DYNCREATE(NGraphSettingsDlg)

// Construction
public:
	NGraphSettingsDlg();
	~NGraphSettingsDlg();

// Dialog Data
	//{{AFX_DATA(NGraphSettingsDlg)
	enum { IDD = IDD_GRAPHICS_SETTINGS };
	CComboBox	m_ToolMashCtrl;
	double	m_Axes1Size;
	double	m_Axes2Size;
	double	m_Axes3Size;
	double	m_CircleTolerance;
	double	m_SelectionRegion;
	double	m_SnapMarker;
	double	m_ToolMarker;
	CString	m_ToolMash;
	double	m_weld_beamr;
	UINT	m_VChange_Time;
	double m_TolLineApp;
	double m_DevArcSpan;
	double m_MinRadArc;
	double m_ProgToolAxesLength;
	double m_ProgNodesSize;
	double m_ProgToolAxesThickness;
	double m_ConverterAcc;
	double m_ConverterAng;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(NGraphSettingsDlg)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(NGraphSettingsDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	double m_mod_tolerance;
	double m_contexp_tolerance;
	CButton m_CutHelix;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

