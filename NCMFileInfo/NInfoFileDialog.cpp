// NInfoFileDialog.cpp : implementation file
//

#include "stdafx.h"
#include "NCMFileInfo.h"
#include "NCMFileInfoDlg.h"
#include "NInfoFileDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NInfoFileDialog

IMPLEMENT_DYNAMIC(NInfoFileDialog, CFileDialog)

NInfoFileDialog::NInfoFileDialog(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
}


BEGIN_MESSAGE_MAP(NInfoFileDialog, CFileDialog)
	//{{AFX_MSG_MAP(NInfoFileDialog)
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void NInfoFileDialog::OnFileNameChange()
{
	ShowDialog.Update(GetFileName());
}

int NInfoFileDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFileDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL NInfoFileDialog::OnInitDialog() 
{
	CFileDialog::OnInitDialog();
	
//	ShowDialog.SetWindowPos(&Non
	SetWindowPos(&ShowDialog,0,0,0,0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW );

	ShowDialog.Create(CNCMFileInfoDlg::IDD,this);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void NInfoFileDialog::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
//	CFileDialog::OnLButtonDblClk(nFlags, point);
}
