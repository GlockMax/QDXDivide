#pragma once
#include "ResLocal.h"
#include "NColorButton.h"

// NRotCenterDlg dialog

class NRotCenterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(NRotCenterDlg)

public:
	NRotCenterDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~NRotCenterDlg();

// Dialog Data
	enum { IDD = IDD_ROTCENTER };
protected:
	double m_RotCenRSize;
	double m_RotCentrX;
	double m_RotCentrY;
	double m_RotCentrZ;
	BOOL m_RotCentrActive;
	CMFCColorButton m_Color;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void Apply(void);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnClose();
};
