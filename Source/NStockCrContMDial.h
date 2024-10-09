#pragma once

#include "ResLocal.h"
#include "math.h"
#include "BBox.h"
#include "NExprEdit.h"

class NStockSweep;

// NStockCrContMDial dialog

class NStockCrContMDial : public CDialog
{
	DECLARE_DYNAMIC(NStockCrContMDial)

public:
	NStockCrContMDial(CWnd* pParent = NULL);   // standard constructor
	virtual ~NStockCrContMDial();
	class NStock * GetNewStock();
	void UnSet();
	INT_PTR DoModal(class NStock *pOldStock);

// Dialog Data
	enum { IDD = IDD_STOCK_CREATE_CONTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
protected:
	NStockSweep * pStock;
	BOOL OriginalStock;
	BBox Gabar;
	BPoint Vector;

public:
	CString m_FileName;
	afx_msg void OnBnClickedBrowse();
	virtual BOOL OnInitDialog();
public:
	double m_heig;
	double m_len;
	double m_wid;
	double m_x;
	double m_y;
	double m_z;
	double m_a;
	double m_b;
	double m_c;
	NExprEdit m_heig_ctrl;
	NExprEdit m_len_ctrl;
	NExprEdit m_wid_ctrl;
	NExprEdit m_x_ctrl;
	NExprEdit m_y_ctrl;
	NExprEdit m_z_ctrl;
	int m_Pos;
protected:
	virtual void OnOK();
public:
	int LoadFile(CString & FileName);
};
