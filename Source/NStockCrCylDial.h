#pragma once
// NStockCrCylDial.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// NStockCrCylDial dialog
#include "ResLocal.h"
#include "BBox.h"
#include "NFloatEdit.h"
#include "NSetupSetBoxPointDlg.h"
#include "NCStatic.h"

class NStockCrCylDial : public NSetupSetBoxPointDlg
{
// Construction
public:
	class NStock * GetNewStock();
	INT_PTR DoModal(const BBox &gabar);
	void UnSet();
	NStockCrCylDial(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(NStockCrCylDial)
	enum { IDD = IDD_STOCK_CREATE_CYL };
	NCStatic	m_title4;
	NCStatic	m_title;
	CStatic	m_Pict_s;
	double	m_diam;
	double	m_heig;
	double	m_shift;
	NExprEdit	m_diam_ctrl;
	NExprEdit	m_heig_ctrl;
	NExprEdit	m_shift_ctrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NStockCrCylDial)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BBox CylDim;
	NStock * pStock;
	int m_Pos;

	// Generated message map functions
	//{{AFX_MSG(NStockCrCylDial)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnCalcDim();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	CComboBox m_AxisBox;
public:
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	int m_AxisInt;
public:
	virtual void Serialize(CArchive& ar);
protected:
	void GetBaseShift(int &x,int &y,int &z) const;
};
