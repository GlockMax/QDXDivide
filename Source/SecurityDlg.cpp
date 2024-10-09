// SecurityDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NCM.h"
#include "Security.h"
#include "securitydlg.h"


// SecurityDlg dialog

IMPLEMENT_DYNAMIC(SecurityDlg, CDialog)
SecurityDlg::SecurityDlg(CWnd* pParent /*=NULL*/)
	: CDialog(SecurityDlg::IDD, pParent)
	, m_code(_T(""))
	, m_answer(_T(""))
	, m_AllUsers(FALSE)
{
}

SecurityDlg::~SecurityDlg()
{
}

void SecurityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_code);
	DDX_Text(pDX, IDC_EDIT2, m_answer);
	DDX_Check(pDX, IDC_ALL_USERS, m_AllUsers);
	DDX_Control(pDX, IDC_ALL_USERS, m_AllUsers_Ctrl);
}


BEGIN_MESSAGE_MAP(SecurityDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// SecurityDlg message handlers

void SecurityDlg::OnBnClickedOk()
{
	UpdateData(true);
   
	bool res;
	if(Security::CheckAnswer(m_answer, &res) != Security::OK)
	{
		AfxMessageBox(IDS_MES_PASS);
		exit(0);
	}
	if( !res )
	{
		exit(0);
	}

	bool Store4AllUsers = false;
	if (m_AllUsers)
	{
		HKEY hkResult;
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\NCM40.EXE"), 0, KEY_WRITE, &hkResult)
			== ERROR_SUCCESS)
		{
//			AfxMessageBox("JJJ");
			if (RegSetValueEx(hkResult, _T("Password"), 0, REG_SZ, (unsigned char*)m_answer.GetBuffer(0), m_answer.GetLength()) == ERROR_SUCCESS)
			{
//				AfxMessageBox("JJJ");
				Store4AllUsers = true;
			}
			RegCloseKey(hkResult);
		}
	}
	if (Store4AllUsers)
		AfxGetApp()->WriteProfileString(NCMVersion, _T("Password"), CString());
	else
		AfxGetApp()->WriteProfileString(NCMVersion, _T("Password"), m_answer);

	OnOK();
}

BOOL SecurityDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	Security::Result res = Security::GetCode(&m_code);

	if(res != Security::OK)
	{
		if( res == Security::NoAdapter )
		{
			AfxMessageBox(IDS_MES_NET);
			exit(0);
		}
		AfxMessageBox(IDS_MES_SEC);
		exit(0);
	}

	HKEY hkResult;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\NCM40.EXE"), 0, KEY_WRITE, &hkResult)
		== ERROR_SUCCESS)
	{
		m_AllUsers_Ctrl.EnableWindow(TRUE);
		RegCloseKey(hkResult);
	}
	else
	{
		m_AllUsers_Ctrl.EnableWindow(FALSE);
	}

	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
