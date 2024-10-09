// TestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Test.h"
#include "TestDlg.h"


// TestDlg dialog

IMPLEMENT_DYNAMIC(TestDlg, CDialog)
TestDlg::TestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(TestDlg::IDD, pParent)
	, t1(FALSE)
	, t2(FALSE)
{
}

TestDlg::~TestDlg()
{
}

void TestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_T2, t1);
	DDX_Check(pDX, IDC_T3, t2);
	DDX_Check(pDX, IDC_T4, t3);
	DDX_Check(pDX, IDC_T5, t4);
	DDX_Check(pDX, IDC_T6, t5);
	DDX_Check(pDX, IDC_T7, t6);
}


BEGIN_MESSAGE_MAP(TestDlg, CDialog)
//	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// TestDlg message handlers

void TestDlg::OnOK()
{
	UpdateData();
	test a;
	if(t1)
		a.test01();
	if(t2)
		a.test02();
	if(t3)
		a.test03();
	if(t4)
		a.test04();
	if(t5)
		a.test05();
	if(t6)
		a.test06();
	pTree = a.pRes;
	CDialog::OnOK();
}
