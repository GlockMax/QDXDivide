// NKeyDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "sent.h"

// CNKeyDlg dialog
class CNKeyDlg : public CDialog
{
// Construction
public:
	CNKeyDlg(CWnd* pParent = NULL);	// standard constructor
  CString m_SN;
  CString m_CFG;
  HK_MNG *mng;
// Dialog Data
	enum { IDD = IDD_NKEY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	CEdit m_hSN;
public:
	CEdit m_hCFG;
public:
	afx_msg void OnBnClickedButton1();
public:
	CEdit m_hResult;
};
