#pragma once

// NStockTurnSecDlg dialog

class NStockTurnSecDlg : public CDialog
{
	DECLARE_DYNAMIC(NStockTurnSecDlg)

public:
	NStockTurnSecDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~NStockTurnSecDlg();

// Dialog Data
	enum { IDD = IDD_STOCK_SEC_VIEW };

protected:
	double m_StartAngle;
	double m_AngleSize;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSecviewApply();
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
