// EduKeyGenDlg.h : header file
//

#pragma once


// CEduKeyGenDlg dialog
class CEduKeyGenDlg : public CDialog
{
// Construction
public:
	CEduKeyGenDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_EDUKEYGEN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString In;
	CString Out;
	CString Config;
	afx_msg void OnBnClickedButton1();
	BOOL m_Permanent;
	CString m_Version;
};
