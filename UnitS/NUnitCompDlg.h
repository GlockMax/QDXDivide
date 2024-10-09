#pragma once
// NUnitCompDlg.h : header file
//
#include "ResLocal.h"
#include "..\source\gridctrl\gridctrl.h"
/////////////////////////////////////////////////////////////////////////////
// NUnitCompDlg dialog
#define HIDC_LIST1					0x0150E
#define HIDC_COMP_KEY				0x0150F
#define HIDC_COMP_VAL				0x01510
#define HIDC_COMP_NULL				0x01511

typedef CMap<CString,LPCSTR,double,double> CMapStringToDouble;
class NMapIntToDouble;
class NUnitCompDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(NUnitCompDlg)

// Construction
public:
	void SetComp(NMapIntToDouble* ipDComp, NMapIntToDouble* ipHComp);
	NUnitCompDlg();
	~NUnitCompDlg();

// Dialog Data
	//{{AFX_DATA(NUnitCompDlg)
	enum { IDD = IDD_UNIT_COMP };
	CGridCtrl	m_D_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(NUnitCompDlg)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	NMapIntToDouble * pDComp;
	NMapIntToDouble * pHComp;
	CMapStringToDouble Comp;
	// Generated message map functions
	//{{AFX_MSG(NUnitCompDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCompNull();
	afx_msg void OnCompDel();
	void ClearCompArrays(void);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
