#pragma once
#include <vector>
#include "ResLocal.h"
#include "NExprEdit.h"
#include "NCStatic.h"
#include "BMatr.h"
#include "BBox.h"

// NSetupSetBoxPointDlg dialog

class NSetupSetBoxPointDlg : public CDialogEx
{
	DECLARE_DYNAMIC(NSetupSetBoxPointDlg)

public:
	NSetupSetBoxPointDlg(CWnd* pParent, class CNCMDoc *pDoc);   // standard constructor
	NSetupSetBoxPointDlg(UINT nIDTemplate, CWnd* pParentWnd, class CNCMDoc *pDoc);
	virtual ~NSetupSetBoxPointDlg();

// Dialog Data
	enum { IDD = IDD_SETUP_SETBOXPOINT };
	CStatic	m_Pict_s;
	NCStatic	m_t3;
	NCStatic	m_t2;
	NCStatic	m_t1;
	double	m_x;
	double	m_y;
	double	m_z;
	NExprEdit	m_x_ctrl;
	NExprEdit	m_y_ctrl;
	NExprEdit	m_z_ctrl;
	double	m_heig;
	double	m_len;
	double	m_wid;
	NExprEdit	m_heig_ctrl;
	NExprEdit	m_len_ctrl;
	NExprEdit	m_wid_ctrl;
	int but;
	BOOL Auto;
	BOOL FillG54;
	BMatr ToolMatr;
	int m_Pos;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void GetBaseShift(int &x,int &y,int &z) const;
	class CNCMDoc *pDoc;
	void PlaceControlsOnPict(const CWnd &Pict, const std::vector<int> IDs, const std::vector<CPoint> Positions);

	DECLARE_MESSAGE_MAP()
	virtual void UpdateStockGabar(void);
public:
	BBox StockGabar;
	void CalcToolPos(BPoint &P, BPoint &N);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg virtual void OnToolApply(void);
	bool NeedAuto(void) const { return Auto == TRUE; }
	bool NeedFillG54(void) const { return FillG54 == TRUE; }
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	void CalcStockToolGabar(void);
};
