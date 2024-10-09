// NTextRenumDlg.cpp : implementation file
//

#include "stdafx.h"
#include ".\ntextrenumdlg.h"


// NTextRenumDlg dialog

IMPLEMENT_DYNAMIC(NTextRenumDlg, CDialog)
NTextRenumDlg::NTextRenumDlg(CWnd* pParent /*=NULL*/)
	: CDialog(NTextRenumDlg::IDD, pParent)
	, m_StartNum(5)
	, m_Step(5)
	, m_MaxNum(9995)
	, m_Width(0)
	, m_LeadZero(false)
	, m_AddBlank(FALSE)
	, m_RemoveNumbers(FALSE)
	, m_Sym("N")
{
}

NTextRenumDlg::~NTextRenumDlg()
{
}

void NTextRenumDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_StartNum);
	DDV_MinMaxInt(pDX, m_StartNum, 0, 1000000);
	DDX_Text(pDX, IDC_EDIT3, m_Step);
	DDV_MinMaxInt(pDX, m_Step, 0, 1000000);
	DDX_Text(pDX, IDC_EDIT5, m_MaxNum);
	DDV_MinMaxInt(pDX, m_MaxNum, 0, 1000000);
	DDX_Text(pDX, IDC_EDIT6, m_Width);
	DDV_MinMaxInt(pDX, m_Width, 0, 1000000);
	DDX_Check(pDX, IDC_CHECK2, m_LeadZero);
	DDX_Check(pDX, IDC_CHECK3, m_AddBlank);
	DDX_Check(pDX, IDC_CHECK4, m_RemoveNumbers);
	DDX_Text(pDX, IDC_EDIT2, m_Sym);
	DDX_Control(pDX, IDC_EDIT1, m_edit1);
	DDX_Control(pDX, IDC_EDIT3, m_edit3);
	DDX_Control(pDX, IDC_EDIT5, m_edit5);
	DDX_Control(pDX, IDC_EDIT6, m_edit6);
	DDX_Control(pDX, IDC_CHECK2, m_check2);
	DDX_Control(pDX, IDC_CHECK3, m_check3);
	DDX_Control(pDX, IDC_CHECK4, m_check4);
}


BEGIN_MESSAGE_MAP(NTextRenumDlg, CDialog)
	ON_BN_CLICKED(IDC_CHECK4, OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_CHECK2, OnBnClickedCheck2)
END_MESSAGE_MAP()


// NTextRenumDlg message handlers

void NTextRenumDlg::OnBnClickedCheck4()
{
	m_check2.EnableWindow(!m_check4.GetCheck());
	m_check3.EnableWindow(!m_check4.GetCheck());
	m_edit1.EnableWindow(!m_check4.GetCheck());
	m_edit3.EnableWindow(!m_check4.GetCheck());
	m_edit5.EnableWindow(!m_check4.GetCheck());
	m_edit6.EnableWindow(!m_check4.GetCheck() && m_check2.GetCheck());
}

void NTextRenumDlg::OnBnClickedCheck2()
{
	m_edit6.EnableWindow(!m_check4.GetCheck() && m_check2.GetCheck());
}

BOOL NTextRenumDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	OnBnClickedCheck4();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
