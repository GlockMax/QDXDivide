// NCSelectXmlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NCMControl.h"
#include "NCSelectXmlDlg.h"
#include "direct.h"


// CNCSelectXmlDlg dialog

IMPLEMENT_DYNAMIC(CNCSelectXmlDlg, CDialog)

CNCSelectXmlDlg::CNCSelectXmlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNCSelectXmlDlg::IDD, pParent)
{

}

CNCSelectXmlDlg::~CNCSelectXmlDlg()
{
}

void CNCSelectXmlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_XML_NAME, m_ListBox);
	DDX_LBString(pDX, IDC_XML_NAME, xmlName);
}


BEGIN_MESSAGE_MAP(CNCSelectXmlDlg, CDialog)
END_MESSAGE_MAP()


// CNCSelectXmlDlg message handlers

void CNCSelectXmlDlg::OnOK()
{
	UpdateData(1);

	CDialog::OnOK();
}

BOOL CNCSelectXmlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	int drive = _getdrive();
	char oldpath[512];
	_getcwd(oldpath,512);

	CString fn = workPath + "Settings\\*.xml";
	DlgDirList(fn.GetBuffer(0),IDC_XML_NAME,0,DDL_READWRITE);
	int ind = m_ListBox.FindStringExact(0,curName);
	if(ind != LB_ERR)
		m_ListBox.SetCurSel(ind);
	else
		m_ListBox.SetCurSel(0);

	_chdrive(drive);
	_chdir(oldpath);

	return TRUE;   // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

INT_PTR CNCSelectXmlDlg::DoModal(CString path, CString curname)
{
	curName = curname;
	workPath = path;

	return CDialog::DoModal();
}