#pragma once
// NUnitUCSDlg.h : header file
//
#include "ResLocal.h"
#include "NUCSs.h"
#include "..\source\gridctrl\gridctrl.h"
/////////////////////////////////////////////////////////////////////////////
// NUnitUCSDlg dialog

class NUnitUCSDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(NUnitUCSDlg)

// Construction
public:
	NUCSs &UCSs;
	bool UCSChangeDone;
	BOOL m_DontDrawIfEmpty;
	BOOL m_DrawName;
	BOOL m_AXIS;
	BOOL m_FSYS;
	BOOL m_SSYS;
	NUnitUCSDlg(NUCSs& In);
	NUnitUCSDlg() : UCSs(NUCSs()) {}
	~NUnitUCSDlg();

// Dialog Data
	//{{AFX_DATA(NUnitUCSDlg)
	enum { IDD = IDD_UNIT_UCS };
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(NUnitUCSDlg)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CGridCtrl m_GridCtrl;
	// Generated message map functions
	//{{AFX_MSG(NUnitUCSDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
