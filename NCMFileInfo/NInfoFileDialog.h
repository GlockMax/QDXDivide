#if !defined(AFX_NINFOFILEDIALOG_H__7BA73244_CA10_11D5_B0CE_00403328DC2D__INCLUDED_)
#define AFX_NINFOFILEDIALOG_H__7BA73244_CA10_11D5_B0CE_00403328DC2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NInfoFileDialog.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// NInfoFileDialog dialog
#include "NCMFileInfoDlg.h"	// Added by ClassView

class NInfoFileDialog : public CFileDialog
{
	DECLARE_DYNAMIC(NInfoFileDialog)

public:
	void OnFileNameChange( );
	NInfoFileDialog(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);

protected:
	CNCMFileInfoDlg ShowDialog;
	//{{AFX_MSG(NInfoFileDialog)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NINFOFILEDIALOG_H__7BA73244_CA10_11D5_B0CE_00403328DC2D__INCLUDED_)
