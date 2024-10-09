// ..\mashine\MTEditArrayDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MTEditArrayDlg.h"
#include "afxdialogex.h"


// MTEditArrayDlg dialog

IMPLEMENT_DYNAMIC(MTEditArrayDlg, CDialogEx)

MTEditArrayDlg::MTEditArrayDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MACHINE_ARRAY, pParent)
	, m_Count(0)
	, m_LinearStep(0)
	, m_Rotate(TRUE)
{
	Rotary = false;
}

MTEditArrayDlg::~MTEditArrayDlg()
{
}

void MTEditArrayDlg::SetRotary()
{
	Rotary = true;
}

void MTEditArrayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MT_ARRAY_COUNTER, m_Count);
	DDX_Text(pDX, IDC_MT_ARRAY_X, m_LinearStep);
	DDX_Radio(pDX, IDC_MT_ARRAY_ROTATEAXIS_NO, m_Rotate);
}


BEGIN_MESSAGE_MAP(MTEditArrayDlg, CDialogEx)
END_MESSAGE_MAP()


// MTEditArrayDlg message handlers


BOOL MTEditArrayDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if(Rotary)
		GetDlgItem(IDC_MT_ARRAY_X)->EnableWindow(FALSE);
	else
	{
		GetDlgItem(IDC_MT_ARRAY_ROTATEAXIS_NO)->EnableWindow(FALSE);
		GetDlgItem(IDC_MT_ARRAY_ROTATEAXIS_YES)->EnableWindow(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
