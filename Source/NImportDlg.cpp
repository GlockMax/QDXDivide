// NImportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WM_APP_Messages.h"
#include "NImportDlg.h"


// NImportDlg dialog

IMPLEMENT_DYNAMIC(NImportDlg, CDialog)

NImportDlg::NImportDlg(CWnd* pParent /*=NULL*/)
	: CDialog(NImportDlg::IDD, pParent)
{

}

NImportDlg::~NImportDlg()
{
}

void NImportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(NImportDlg, CDialog)
END_MESSAGE_MAP()


// NImportDlg message handlers

void NImportDlg::OnOK()
{
	AfxGetMainWnd()->SendMessage(WM_APP_STOP_IMP_MODE, IDOK);
	CDialog::OnOK();
}

void NImportDlg::PostNcDestroy()
{
	CDialog::PostNcDestroy();
	delete this;
}

void NImportDlg::OnCancel()
{
	AfxGetMainWnd()->SendMessage(WM_APP_STOP_IMP_MODE, IDCANCEL);
	CDialog::OnCancel();
}
