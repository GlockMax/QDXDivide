// NNetConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NCM.h"
#include "NNetConfigDlg.h"


// NNetConfigDlg dialog

IMPLEMENT_DYNAMIC(NNetConfigDlg, CDialog)

NNetConfigDlg::NNetConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(NNetConfigDlg::IDD, pParent)
{

}

NNetConfigDlg::~NNetConfigDlg()
{
}

void NNetConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MODULES, m_Modules);
	DDX_Control(pDX, IDC_MODULES_STATUS, m_Status);
}


BEGIN_MESSAGE_MAP(NNetConfigDlg, CDialog)
END_MESSAGE_MAP()


// NNetConfigDlg message handlers

BOOL NNetConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString Busy;
	Busy.LoadString(IDS_MODULES_BUSY);
	CString Free;
	Free.LoadString(IDS_MODULES_FREE);
	CString Delim(_T("\n"));
	CString Modules;
	Modules.LoadString(IDS_MODULES);

	for(int i = 0; Modules.GetLength() > 1; ++i)
	{
		CString Line = Modules.SpanExcluding(Delim);
		if((0x1 << i) & Config)
		{
			m_Modules.AddString(Line);
			m_Status.AddString(((0x1 << i) & Locked) ?
				Busy : Free);
		}
		Modules.Delete(0, Line.GetLength() + 1);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

INT_PTR NNetConfigDlg::DoModal(NCMConfig Conf, NCMConfig Lock)
{
	Config = Conf;
	Locked = Lock;

	return CDialog::DoModal();
}
