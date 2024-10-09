#pragma once
#include "ResLocal.h"


// NTextErrDlg dialog

class NTextErrDlg : public CDialog
{
	DECLARE_DYNAMIC(NTextErrDlg)
	static bool Busy;
public:
	NTextErrDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~NTextErrDlg();

// Dialog Data
	enum { IDD = IDD_TEXT_ERR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	CString message1;
	CString message2;
	CListCtrl m_Line1;
	CListCtrl m_Line2;
public:
	void SetMes(const CString & mes1, const CString & mes2);
	afx_msg void OnBnClickedYes();
	afx_msg void OnBnClickedNo();
};
