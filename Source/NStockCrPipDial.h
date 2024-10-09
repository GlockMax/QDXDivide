#pragma once

#include "BBox.h"
#include "ResLocal.h"
#include "NExprEdit.h"

class NStock;

// NStockCrPipDial dialog

class NStockCrPipDial : public CDialog
{
	DECLARE_DYNAMIC(NStockCrPipDial)

public:
	NStockCrPipDial(CWnd* pParent = NULL);   // standard constructor
	NStockCrPipDial(int ID, CWnd* pParent = NULL);   // standard constructor
	virtual ~NStockCrPipDial();
	class NStock * GetNewStock();

// Dialog Data
	enum { IDD = IDD_STOCK_CREATE_CONE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_ExtRad0;
	double m_ExtRad1;
	double m_IntRad0;
	double m_IntRad1;
	double m_Length;
	double m_RotAngle;
	double m_x;
	double m_y;
	double m_z;
	double m_shift;
	NExprEdit m_ExtRad0_ctrl;
	NExprEdit m_ExtRad1_ctrl;
	NExprEdit m_IntRad0_ctrl;
	NExprEdit m_IntRad1_ctrl;
	NExprEdit m_Length_ctrl;
	NExprEdit m_RotAngle_ctrl;
	NExprEdit m_x_ctrl;
	NExprEdit m_y_ctrl;
	NExprEdit m_z_ctrl;
	NExprEdit m_shift_ctrl;
	int m_PolyNum;
	int m_AxisInt; 
	BOOL Auto; 
	BOOL FillG54;
protected:
	BBox Gabar;
	class NStock * pStock;
	bool MillDialog;
	int m_Pos;

public:
	void UnSet(void);
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	virtual void Serialize(CArchive& ar);
	afx_msg void OnBnClickedCalcDim();
	INT_PTR DoModal(const BBox &gabar, class NStock *pExStock);
	INT_PTR GetID() const { return (INT_PTR)m_lpszTemplateName; }
	bool NeedAuto(void) const { return Auto == TRUE; }
	bool NeedFillG54(void) const { return FillG54 == TRUE; }
	void SetMillDialog(bool Val) { MillDialog = Val; }
};
