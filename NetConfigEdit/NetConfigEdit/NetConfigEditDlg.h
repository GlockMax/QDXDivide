
// NetConfigEditDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "ResLocal.h"
#include "NCM.h"
#include "ConfigList.h"


// CNetConfigEditDlg dialog
class CNetConfigEditDlg : public CDialog
{
// Construction
public:
	CNetConfigEditDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_NETCONFIGEDIT_DIALOG };

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
	ConfigList m_ConfigList;
	NCMConfig GetConfig(void){ return m_ConfigList.Config;};
	virtual INT_PTR DoModal(NCMConfig Conf);
	CListBox m_Modules;
protected:
	virtual void OnOK();
};
