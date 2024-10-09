// NCAddWReplD.cpp : implementation file
//

#include "stdafx.h"
#include "NCMControl.h"
#include "NCAddWReplD.h"
#include "resource.h"


// CNCAddWReplD dialog

IMPLEMENT_DYNAMIC(CNCAddWReplD, CDialog)

CNCAddWReplD::CNCAddWReplD(CWnd* pParent /*=NULL*/)
	: CDialog(CNCAddWReplD::IDD, pParent)
	, srcString(_T(""))
	, dstString(_T(""))
{

}

CNCAddWReplD::~CNCAddWReplD()
{
}

void CNCAddWReplD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT3, srcString);
	DDX_Text(pDX, IDC_EDIT4, dstString);
}


BEGIN_MESSAGE_MAP(CNCAddWReplD, CDialog)
END_MESSAGE_MAP()


// CNCAddWReplD message handlers

void CNCAddWReplD::OnOK()
{
	UpdateData(1);
	if(srcString == _T("") || dstString == _T(""))
	{
		CString str;
		str.LoadString(IDS_WREPL_ADD_EMPTY);
		AfxMessageBox(str);
		return;
	}//if

	CDialog::OnOK();
}