#pragma once
// NToolsCrDial.h : header file
//
#include "ResLocal.h"
#include "NFloatEdit.h"

class NToolStd;
class NToolCombinedMill;

/////////////////////////////////////////////////////////////////////////////
// NToolsCrDial dialog
/*
#define HIDC_MILL_DIAM				0x01201
#define HIDC_MILL_H					0x01202
#define HIDC_MILL_R					0x01203
#define HIDC_MILL_ALF				0x01204
#define HIDC_MILL_FACET				0x01205
#define HIDC_MILL_HM				0x01206
#define HIDC_MILL_NAME				0x01207
#define HIDC_TOOL_CHECK				0x01209
#define HIDC_CHECK_ALL				0x0120A
#define HIDC_TOOL_OTHER				0x0120B
#define HIDC_TOOLS_TAB				0x0120C
#define HIDC_TOOL_DRAW1				0x0120D
#define HIDC_TOOL_DRAW2				0x01210
#define HIDC_TOOL_DRAW3				0x01211
#define HIDC_TOOL_DRAW4				0x01212
#define HIDC_TOOL_DRAW5				0x01213
#define HIDC_TOOL_DRAW6				0x01214
*/
class AFX_EXT_CLASS NToolsCrDial : public CDialog
{
// Construction
public:
	void SetOtherTool(bool flag);
	bool GetOtherTool();
	bool IsNameAuto;
	CString& GetName();
	NToolStd * GetNewTool();
	NToolsCrDial(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(NToolsCrDial)
	enum { IDD = IDD_TOOLS_CREATE };
	CButton	m_tool_other;
	CStatic	m_Draw6;
	CStatic	m_Draw5;
	CStatic	m_Draw4;
	CStatic	m_Draw3;
	CStatic	m_Draw2;
	CStatic	m_Draw1;
	NFloatEdit	m_hm_ctrl;
	NFloatEdit	m_faset_ctrl;
	CEdit	m_name_ctrl;
	NFloatEdit	m_r_ctrl;
	NFloatEdit	m_h_ctrl;
	NFloatEdit	m_diam_ctrl;
	NFloatEdit	m_alf_ctrl;
	CButton	m_check_flag_ctrl;
	CStatic	m_label3_ctrl;
	CStatic	m_label2_ctrl;
	CStatic	m_label1_ctrl;
	double	m_alf;
	double	m_diam;
	double	m_h;
	CString	m_name;
	double	m_r;
	int		m_PointSize;
	double	m_displacement;
	NFloatEdit	m_displacement_ctrl;
	double	m_beta;
	double	m_faset;
	double	m_hm;
	BOOL	m_check_flag;
	BOOL	m_check_flange;
	BOOL	m_vert_tool;
	BOOL	m_check_probe;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NToolsCrDial)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString GenName();
	int prev_tab_pos;
	CImageList ToolPics;
	CImageList ToolCutter;
	CTabCtrl * Tab;
	NToolStd * pTool;
	bool m_OtherTool;

	// Generated message map functions
	//{{AFX_MSG(NToolsCrDial)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSelchangeToolsTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeMillDiam();
	afx_msg void OnChangeMillH();
	afx_msg void OnChangeMillR();
	afx_msg void OnChangeMillAlf();
	afx_msg void OnChangeMillFacet();
	afx_msg void OnToolCheck();
	afx_msg void OnToolOther();
	afx_msg void OnCheckAll();
	afx_msg void OnKillfocusMillName();
//	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
//	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
