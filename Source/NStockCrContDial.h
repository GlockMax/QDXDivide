#pragma once

#include "ResLocal.h"
#include "BBox.h"
#include "NExprEdit.h"

class NStockCont;

// NStockCrContDial dialog

class NStockCrContDial : public CDialog
{
	DECLARE_DYNAMIC(NStockCrContDial)

public:
	NStockCrContDial(CWnd* pParent = NULL);   // standard constructor
	virtual ~NStockCrContDial();
	class NStock * GetNewStock();
	void UnSet();
	INT_PTR DoModal(class NStock *pOldStock);

// Dialog Data
	enum { IDD = IDD_STOCK_CREATE_CONT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
protected:
	NStock * pStock;
	BOOL OriginalStock;
	BBox Gabar;

public:
	CString m_FileName;
	afx_msg void OnBnClickedBrowse();
	afx_msg void OnBnClickedFlipXY();
	virtual BOOL OnInitDialog();
public:
	double m_heig;
	double m_len;
	double m_wid;
	double m_x;
	double m_y;
	double m_z;
	BOOL m_inv;
	NExprEdit m_heig_ctrl;
	NExprEdit m_len_ctrl;
	NExprEdit m_wid_ctrl;
	NExprEdit m_x_ctrl;
	NExprEdit m_y_ctrl;
	NExprEdit m_z_ctrl;
	BOOL m_FlipXY;
	BOOL m_FlipXYInv;
	int m_Pos;
protected:
	virtual void OnOK();
public:
	int LoadFile(CString & FileName, bool FullRead);
};
