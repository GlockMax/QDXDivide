// NSetupWriteCSDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainFrm.h"
#include "NSetupWriteCSDlg.h"
#include "afxdialogex.h"


// NSetupWriteCSDlg dialog

IMPLEMENT_DYNAMIC(NSetupWriteCSDlg, CDialogEx)

NSetupWriteCSDlg::NSetupWriteCSDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(NSetupWriteCSDlg::IDD, pParent)
{
	Ind = 0;
}

NSetupWriteCSDlg::~NSetupWriteCSDlg()
{
}

void NSetupWriteCSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_G54G59, Ind);
	DDX_Control(pDX, IDC_G54G59, m_UCSCtrl);
}


BEGIN_MESSAGE_MAP(NSetupWriteCSDlg, CDialogEx)
END_MESSAGE_MAP()


// NSetupWriteCSDlg message handlers


void NSetupWriteCSDlg::OnOK()
{
	UpdateData();
	
	auto k = m_UCSCtrl.GetItemData(Ind);
	if(k != CB_ERR)
		((CMainFrame *)GetParent())->WriteCS_OK(int(k));

	CDialogEx::OnOK();
}


void NSetupWriteCSDlg::OnCancel()
{
	CDialogEx::OnCancel();
}

