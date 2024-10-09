#pragma once
// NModePainterDlg.h : header file
//
#include "ResLocal.h"

/////////////////////////////////////////////////////////////////////////////
// NModePainterDlg dialog

class NModePainterDlg : public CPropertyPage
{
// Construction
public:
	int GetMode();
	NModePainterDlg();   // standard constructor
	int ModeType;

// Dialog Data
	//{{AFX_DATA(NModePainterDlg)
	enum { IDD = IDD_MODE_PAINTER };
	BOOL	m_CurveType;
	BOOL	m_ToolN;
	BOOL	m_Correction;
	BOOL	m_CorrectLine;
	BOOL	m_Coolant;
	BOOL	m_Feed;
	BOOL	m_Cycle;
	BOOL	m_NotPaint;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NModePainterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void UpdateControlData();
	void SetValues(int);
	bool m_IsIndependent;
	int ModeID;

	// Generated message map functions
	//{{AFX_MSG(NModePainterDlg)
	afx_msg void OnClickCurveType();
	afx_msg void OnClickToolN();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickCoorect();
	afx_msg void OnClickCoolant();
	afx_msg void OnClickCorrectLine();
	afx_msg void OnClickCorrectCont();
	afx_msg void OnClickFeed();
	afx_msg void OnClickCycle();
	afx_msg void OnNotPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

