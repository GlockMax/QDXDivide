#pragma once
#include "ResLocal.h"


// MTEditSaveAsDlg dialog

class MTEditSaveAsDlg : public CDialog
{
	DECLARE_DYNAMIC(MTEditSaveAsDlg)

public:
	MTEditSaveAsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~MTEditSaveAsDlg();

// Dialog Data
	enum { IDD = IDD_MACHINE_SAVEAS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_Edit;
	CListBox m_ListBox;
	CString m_name;
protected:
	CString m_path;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnDblclkMachineName();
	INT_PTR DoModal(const CString &path, const CString &name);
	afx_msg void OnLbnSelchangeMachineNames();
};
