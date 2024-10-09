#pragma once
#include "ResLocal.h"

// NTextRenumDlg dialog

class NTextRenumDlg : public CDialog
{
	DECLARE_DYNAMIC(NTextRenumDlg)

public:
	NTextRenumDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~NTextRenumDlg();

// Dialog Data
	enum { IDD = IDD_EDIT_RENUM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	unsigned int m_StartNum;
	unsigned int m_Step;
	unsigned int m_MaxNum;
	unsigned int m_Width;
	afx_msg void OnBnClickedCheck4();
	BOOL m_LeadZero;
	BOOL m_AddBlank;
	BOOL m_RemoveNumbers;
	CString m_Sym;
	CEdit m_edit1;
	CEdit m_edit3;
	CEdit m_edit5;
	CEdit m_edit6;
	CButton m_check2;
	CButton m_check3;
	CButton m_check4;
	afx_msg void OnBnClickedCheck2();
	virtual BOOL OnInitDialog();
};
