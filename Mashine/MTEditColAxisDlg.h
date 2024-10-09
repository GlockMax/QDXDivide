#pragma once

#include "ResLocal.h"
#include "MTMashineTool.h"

// MTEditColAxisDlg dialog

class AFX_EXT_CLASS MTEditColAxisDlg : public CDialog
{
	DECLARE_DYNAMIC(MTEditColAxisDlg)

public:
	MTEditColAxisDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~MTEditColAxisDlg();
	INT_PTR DoModal(MTMachineTool *MT, CString Axis);
	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_EDIT_COL_AXIS };
	CString	m_name;

protected:
	CListBox	m_ListBox;
	MTMachineTool *pMT;
	CString AxisName;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnDblclkAxisName();

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
};
