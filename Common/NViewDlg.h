#pragma once
#include "OPENGLRENDER_API.h"

// NViewDlg dialog

class OPENGLRENDER_API NViewDlg : public CDialog
{
	DECLARE_DYNAMIC(NViewDlg)

public:
	NViewDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~NViewDlg();
	virtual BOOL Create(CWnd* pParentWnd = NULL);


protected:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo) override;

	afx_msg BOOL OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};
