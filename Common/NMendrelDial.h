#pragma once

#include "NToolStd.h"
#include "ResLocal.h"
#include "NFloatEdit.h"
class NToolStd;
class NToolCombined;
class NMapStringToOb;

/////////////////////////////////////////////////////////////////////////////
// NMendrelDial dialog
/*
#define HIDC_STANDART				0x01301
#define HIDC_STANDART_NAME			0x01302
#define HIDC_NOTSTANDART			0x01303
#define HIDC_MENDREL_D2				0x01304
#define HIDC_MENDREL_H				0x01305
#define HIDC_MENDREL_D				0x01306
#define HIDC_MENDREL_HM				0x01307
#define HIDC_ALL_TOOL_LIST			0x01308
#define HIDC_SELECT_TOOL_LIST		0x01309
#define HIDC_ADD					0x0130A
#define HIDC_DELETE					0x0130B
#define HIDC_MENDREL_NAME			0x0130C
#define HIDC_MENDREL_DISPLACEMENT	0x0130D
#define HIDC_MENDRELHELP			0x01310
#define HIDC_MENDREL_PIC			0x01311
*/
class AFX_EXT_CLASS NMendrelDial : public CDialog
{
// Construction
public:

	NToolStd* GetNewTool();
	CString& GetName();
	bool IsNameAuto;
	NMendrelDial(CWnd* pParent = NULL);   // standard constructor
	~NMendrelDial();

// Dialog Data
	//{{AFX_DATA(NMendrelDial)
	enum { IDD = IDD_MENDREL };
	CListBox	m_select_tools;
	CListBox	m_all_tools;
	CEdit	m_name_ctrl;
	NFloatEdit	m_h_ctrl;
	NFloatEdit	m_diam_ctrl;
	NFloatEdit	m_d2_ctrl;
	NFloatEdit	m_hm_ctrl;
	CButton	m_radio1_ctrl;
	CButton	m_radio2_ctrl;
	double	m_dbot;
	double	m_h;
	double	m_hm;
	double	m_dtop;
	double	m_displacement;
	double m_dprevious;
	NFloatEdit	m_displacement_ctrl;
	CString	m_name;
	BOOL	m_check_shank;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NMendrelDial)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	NToolStd *pTool;
	NMapStringToOb * tools_list;

	CString GenName();

	// Generated message map functions
	//{{AFX_MSG(NMendrelDial)
	virtual void OnOK();
	afx_msg void OnKillfocusMendrelName();
	afx_msg void OnChangeMendrelDTop();
	afx_msg void OnChangeMendrelH();
	afx_msg void OnChangeMendrelDBot();
	afx_msg void OnChangeMendrelHm();
	virtual void OnCancel();
//	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
//	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnMendrelHelp();
	afx_msg void OnShankCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};

