// LanguageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LanguageDlg.h"


// LanguageDlg dialog

IMPLEMENT_DYNAMIC(LanguageDlg, CDialog)

LanguageDlg::LanguageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(LanguageDlg::IDD, pParent)
{

}

LanguageDlg::~LanguageDlg()
{
}

void LanguageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LANG, LangCtrl);
}


BEGIN_MESSAGE_MAP(LanguageDlg, CDialog)
END_MESSAGE_MAP()


// LanguageDlg message handlers

BOOL LanguageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	LangCtrl.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void LanguageDlg::OnOK()
{
	UpdateData(FALSE);
	Lang = LangCtrl.GetCurSel();
	CDialog::OnOK();
}
