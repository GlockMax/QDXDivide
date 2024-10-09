#pragma once
#include "nrepreportsarray.h"
#include "ResInv.h"


// NRepDlgActiveReports dialog

class NRepDlgActiveReports : public CDialog
{
	DECLARE_DYNAMIC(NRepDlgActiveReports)

public:
	NRepDlgActiveReports(CWnd* pParent = NULL);   // standard constructor
	virtual ~NRepDlgActiveReports();

// Dialog Data
	enum { IDD = IDD_REP_ACTIVEREP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	NRepReportsArray *pRepArray;
	CListBox *ListPassive,*ListActive;
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonRemove();
	afx_msg void OnBnClickedOk();
};
