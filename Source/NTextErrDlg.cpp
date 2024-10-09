// NTextErrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NTextErrDlg.h"

bool NTextErrDlg::Busy = false;

// NTextErrDlg dialog

IMPLEMENT_DYNAMIC(NTextErrDlg, CDialog)
NTextErrDlg::NTextErrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(NTextErrDlg::IDD, pParent)
{
	Busy = true;
}

NTextErrDlg::~NTextErrDlg()
{
	Busy = false;
}

void NTextErrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LINE1, m_Line1);
	DDX_Control(pDX, IDC_LINE2, m_Line2);
}


BEGIN_MESSAGE_MAP(NTextErrDlg, CDialog)
	ON_BN_CLICKED(IDYES, OnBnClickedYes)
	ON_BN_CLICKED(IDNO, OnBnClickedNo)
END_MESSAGE_MAP()


// NTextErrDlg message handlers

BOOL NTextErrDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Line1.SetTextColor(RGB(255,0,0));
	m_Line2.SetTextColor(RGB(0,0,255));
	m_Line1.InsertItem(0,message1);
	m_Line2.InsertItem(0,message2);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void NTextErrDlg::SetMes(const CString & mes1, const CString & mes2)
{
	message1 = mes1.SpanExcluding("\r\n");
	message2 = mes2.SpanExcluding("\r\n");
}

void NTextErrDlg::OnBnClickedYes()
{
	EndDialog(IDYES);
}

void NTextErrDlg::OnBnClickedNo()
{
	EndDialog(IDNO);
}
