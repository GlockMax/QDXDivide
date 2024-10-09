// NOGLInitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NOGLInitDlg.h"


// NOGLInitDlg dialog

IMPLEMENT_DYNAMIC(NOGLInitDlg, CDialog)
NOGLInitDlg::NOGLInitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(NOGLInitDlg::IDD, pParent)
{
}

NOGLInitDlg::~NOGLInitDlg()
{
}

void NOGLInitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, CheckBox);
	if(CheckBox.GetState())
	{
		AfxGetApp()->WriteProfileInt("","DisableOGLInitDlg",TRUE);
	}
}


BEGIN_MESSAGE_MAP(NOGLInitDlg, CDialog)
END_MESSAGE_MAP()



INT_PTR NOGLInitDlg::DoModal()
{
	if(AfxGetApp()->GetProfileInt("","DisableOGLInitDlg",FALSE))
		return IDOK;
	else
		return CDialog::DoModal();
}
