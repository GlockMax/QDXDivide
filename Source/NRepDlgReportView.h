#pragma once

#include "NRepReportsArray.h"
#include "ResInv.h"

// NRepDlgReportView dialog

class NRepDlgReportView : public CDialog
{
	DECLARE_DYNAMIC(NRepDlgReportView)

public:
	CTabCtrl* Tabs;
	CListCtrl* List;
	int m_SelectedTab;
	NRepReportsArray* pReps;
	NRepDlgReportView(CWnd* pParent = NULL);   // standard constructor
	virtual ~NRepDlgReportView();

// Dialog Data
	enum { IDD = IDD_DIALOG_REDPORTVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void FillList(void);
	afx_msg void OnTcnSelchangeTabTable(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonSave();
};
