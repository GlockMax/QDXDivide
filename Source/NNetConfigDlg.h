#pragma once
#include "ResLocal.h"


// NNetConfigDlg dialog

class NNetConfigDlg : public CDialog
{
	DECLARE_DYNAMIC(NNetConfigDlg)

public:
	NNetConfigDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~NNetConfigDlg();

// Dialog Data
	enum { IDD = IDD_MODULES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	NCMConfig Config;
	NCMConfig Locked;
public:
	CListBox m_Modules;
	CListBox m_Status;
	virtual BOOL OnInitDialog();
	virtual INT_PTR DoModal(NCMConfig Config, NCMConfig Locked);
};
