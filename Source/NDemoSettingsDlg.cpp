// NDemoSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NDemoSettingsDlg.h"


// NDemoSettingsDlg dialog

IMPLEMENT_DYNAMIC(NDemoSettingsDlg, CDialog)

NDemoSettingsDlg::NDemoSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(NDemoSettingsDlg::IDD, pParent)
{

}

NDemoSettingsDlg::~NDemoSettingsDlg()
{
}

void NDemoSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(NDemoSettingsDlg, CDialog)
END_MESSAGE_MAP()


// NDemoSettingsDlg message handlers
