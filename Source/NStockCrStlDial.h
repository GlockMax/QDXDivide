#pragma once
// NStockCrStlDial.h : header file
//
#include "BBox.h"	// Added by ClassView
#include "NStock.h"
#include "NStockStl.h"
#include "NSetupSetBoxPointDlg.h"
#include "ResLocal.h"
/////////////////////////////////////////////////////////////////////////////
// NStockCrStlDial dialog
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

class NStockCrStlDial : public NSetupSetBoxPointDlg
{
// Construction
public:
	class NStock * GetNewStock();
	void UnSet();
	INT_PTR DoModal(class NStock *pOldStock);
	NStockCrStlDial(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(NStockCrStlDial)
	enum { IDD = IDD_STOCK_CREATE_STL };
	CString	m_FileName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NStockCrStlDial)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL OriginalStock;
	BBox Gabar;
	class NStock * pStock;
	int m_Pos;

	// Generated message map functions
	//{{AFX_MSG(NStockCrStlDial)
	afx_msg void OnBrowse();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	class NStock *MakeNewStock(void);
//	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
//	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	void UpdateStockGabar(void);
public:
	virtual void Serialize(CArchive& ar);
};
