// NUnitNameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NUnitNameDlg.h"
#include "direct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NUnitNameDlg dialog
static DWORD aHelpIDs[]=
{
	IDC_UNIT_NAME,					HIDC_UNIT_NAME,
	0,0
};


NUnitNameDlg::NUnitNameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(NUnitNameDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(NUnitNameDlg)
	m_name = _T("");
	//}}AFX_DATA_INIT
}


void NUnitNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NUnitNameDlg)
	DDX_Control(pDX, IDC_UNIT_NAME, m_ListBox);
	DDX_LBString(pDX, IDC_UNIT_NAME, m_name);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NUnitNameDlg, CDialog)
	//{{AFX_MSG_MAP(NUnitNameDlg)
	ON_LBN_DBLCLK(IDC_UNIT_NAME, OnDblclkUnitName)
	ON_WM_HELPINFO()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NUnitNameDlg message handlers

BOOL NUnitNameDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	int drive = _getdrive();
	char oldpath[512];
	_getcwd(oldpath,512);

	CString fn = m_path + "\\*.sm*";
	DlgDirList(fn.GetBuffer(0),IDC_UNIT_NAME,0,DDL_READWRITE);
	int ind = m_ListBox.FindStringExact(-1,m_name);
	if(ind != LB_ERR)
		m_ListBox.SetCurSel(ind);

	_chdrive(drive);
	_chdir(oldpath);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT_PTR NUnitNameDlg::DoModal(const CString &path, const CString &name)
{
	m_path = path;
	m_name = name;
	return CDialog::DoModal();
}

void NUnitNameDlg::OnDblclkUnitName() 
{
	CDialog::OnOK();	
}
/*
BOOL NUnitNameDlg::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	if (pHelpInfo->iContextType == HELPINFO_WINDOW)
		::WinHelp((HWND) pHelpInfo->hItemHandle,
		AfxGetApp()->m_pszHelpFilePath, HELP_WM_HELP, (DWORD) aHelpIDs);

	return TRUE;
}
*/
/*
void NUnitNameDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	::WinHelp((HWND) *pWnd,	AfxGetApp()->m_pszHelpFilePath, 
	HELP_CONTEXTMENU, (DWORD) aHelpIDs);		
}
*/