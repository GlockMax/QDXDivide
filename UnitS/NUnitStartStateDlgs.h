#pragma once
// NUnitStartStateDlgs.h : header file
//
#include "ResLocal.h"
#include "NFloatEdit.h"
/////////////////////////////////////////////////////////////////////////////
// NUnitPositionDlg dialog
#define HIDC_START_X			0x01501
#define	HIDC_START_Y			0x01502
#define	HIDC_START_Z			0x01503
#define	HIDC_TOOL_POS			0x01504
#define	HIDC_DEBUG				0x01505

class NUnitPositionDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(NUnitPositionDlg)

// Construction
public:
	NUnitPositionDlg();
	~NUnitPositionDlg();

// Dialog Data
	//{{AFX_DATA(NUnitPositionDlg)
	enum { IDD = IDD_UNIT_POSITION };
	double	m_x;
	double	m_y;
	double	m_z;
	double	m_a;
	double	m_b;
	double	m_c;
	NFloatEdit	m_x_ctrl;
	NFloatEdit	m_y_ctrl;
	NFloatEdit	m_z_ctrl;
	NFloatEdit	m_a_ctrl;
	NFloatEdit	m_b_ctrl;
	NFloatEdit	m_c_ctrl;
	CString	m_tool_pos;
	BOOL	m_Debug;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(NUnitPositionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(NUnitPositionDlg)
//	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
//	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnEnChangeStartX();
};

/////////////////////////////////////////////////////////////////////////////
// NUnitSpeedDlg dialog
#define HIDC_FEED_MEAS_MMpMIN			0x01506
#define HIDC_FEED_MEAS_MMpREV			0x01507
#define HIDC_FEED						0x01508
#define HIDC_FEED_SCALE					0x01509

class NUnitSpeedDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(NUnitSpeedDlg)

// Construction
public:
	bool is_feed_mm_per_min;
	NUnitSpeedDlg();
	~NUnitSpeedDlg();

// Dialog Data
	//{{AFX_DATA(NUnitSpeedDlg)
	enum { IDD = IDD_UNIT_SPEED };
	CComboBox	m_feed_scale_ctrl;
	CComboBox	m_feed_ctrl;
	CString	m_feed;
	CString	m_feed_scale;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(NUnitSpeedDlg)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(NUnitSpeedDlg)
	virtual BOOL OnInitDialog();
//	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
//	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// NUnitModeDlg dialog
#define HIDC_ABSOLUTE_COORD_PROGR		0x0150A
#define HIDC_RELATIVE_COORD_PROGR		0x0150B
#define HIDC_MEAS_MM					0x0150C
#define HIDC_MEAS_INCH					0x0150D

class NUnitModeDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(NUnitModeDlg)

// Construction
public:
	BOOL metric_coord;
	BOOL absolute_coord;
	NUnitModeDlg();
	~NUnitModeDlg();

// Dialog Data
	//{{AFX_DATA(NUnitModeDlg)
	enum { IDD = IDD_UNIT_MODE };
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(NUnitModeDlg)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(NUnitModeDlg)
	virtual BOOL OnInitDialog();
//	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
//	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
