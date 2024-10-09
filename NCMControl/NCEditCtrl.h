#pragma once
#include "afxwin.h"

class CNCEditCtrl :
	public CEdit
{
public:
	CNCEditCtrl(void);
	~CNCEditCtrl(void);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};
