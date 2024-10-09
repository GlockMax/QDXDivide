#pragma once
#include "ResLocal.h"

// SecurityDlg dialog

class SecurityDlg : public CDialog
{
	DECLARE_DYNAMIC(SecurityDlg)

public:
	SecurityDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~SecurityDlg();

// Dialog Data
	enum { IDD = IDD_SECURITY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_code;
	CString m_answer;
	BOOL m_AllUsers;
	CButton m_AllUsers_Ctrl;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
