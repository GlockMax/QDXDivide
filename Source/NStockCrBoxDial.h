#pragma once

#include "BBox.h"	// Added by ClassView
#include "NCStatic.h"
#include "ResLocal.h"
#include "NFloatEdit.h"
#include "NSetupSetBoxPointDlg.h"

class NStock;

/////////////////////////////////////////////////////////////////////////////
// NStockCrBoxDial dialog
#define HIDC_STOCK_SHIFT			0x01401
#define HIDC_CALC_DIM				0x01402
#define HIDC_STOCK_LEN				0x01403
#define HIDC_STOCK_WID				0x01403
#define HIDC_STOCK_HEIG				0x01404
#define HIDC_STOCK_X				0x01405
#define HIDC_STOCK_Y				0x01406
#define HIDC_STOCK_Z				0x01407
#define HIDC_BOX_SMALL				0x01408
#define HIDC_BOX_LARGE				0x01409
#define HIDC_R0						0x0140A
#define HIDC_R1						0x0140B
#define HIDC_R2						0x0140C
#define HIDC_R3						0x0140D
#define HIDC_R4						0x01410
#define HIDC_R5						0x01411
#define HIDC_R6						0x01412
#define HIDC_R7						0x01413
#define HIDC_R8						0x01414
#define HIDC_R9						0x01415

class NStockCrBoxDial : public NSetupSetBoxPointDlg
{
// Construction
public:
	void UnSet();
	class NStock * GetNewStock();
	INT_PTR DoModal(const BBox &gabar, class NStock *pExStock);
	NStockCrBoxDial(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(NStockCrBoxDial)
	enum { IDD = IDD_STOCK_CREATE_BOX };
	double	m_shift;
	NExprEdit	m_shift_ctrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NStockCrBoxDial)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	class NStock * pStock;
	BBox Gabar;

	// Generated message map functions
	//{{AFX_MSG(NStockCrBoxDial)
	//}}AFX_MSG
	void Recalc();
	afx_msg void OnCalcDim();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
	void UpdateStockGabar(void);
public:
	virtual void Serialize(CArchive& ar);
};
