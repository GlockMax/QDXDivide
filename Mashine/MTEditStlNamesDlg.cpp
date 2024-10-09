// MTEditStlNameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resinv.h"
#include "MTEditStlNamesDlg.h"
#include "direct.h"

using namespace std;

// MTEditStlNamesDlg dialog

IMPLEMENT_DYNAMIC(MTEditStlNamesDlg, CDialog)

MTEditStlNamesDlg::MTEditStlNamesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(MTEditStlNamesDlg::IDD, pParent)
{
}

MTEditStlNamesDlg::~MTEditStlNamesDlg()
{
}

void MTEditStlNamesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NUnitNameDlg)
	DDX_Control(pDX, IDC_STL_NAME, m_ListBox);
}


BEGIN_MESSAGE_MAP(MTEditStlNamesDlg, CDialog)
//	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// MTEditStlNameDlg message handlers


INT_PTR MTEditStlNamesDlg::DoModal(const CString &path)
{
	m_path = path;
	return CDialog::DoModal();
}//int MTEditStlNameDlg::DoModal(const CString &path, const CString &name)

BOOL MTEditStlNamesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	int drive = _getdrive();
	char oldpath[512];
	_getcwd(oldpath,512);

	CString fn = m_path + "\\*.stl";
	CString str;
	str.LoadString(IDS_NO_STL_FILE);
	DlgDirList(fn.GetBuffer(0),IDC_STL_NAME,0,DDL_READWRITE);
	m_ListBox.AddString(str);

	_chdrive(drive);
	_chdir(oldpath);

	return TRUE;   // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void MTEditStlNamesDlg::OnOK()
{
	int selCount = m_ListBox.GetSelCount(); 
	if (selCount == 0)
	{
		CString sError; sError.LoadString(IDS_ERROR_EMPTY_STL_LIST);
		AfxMessageBox(sError);
		return;
	}//if
	for(int i = 0; i < m_ListBox.GetCount(); i++)
	{
		if( m_ListBox.GetSel(i))
		{
			CString str;
			m_ListBox.GetText(i, str);
			stringList.push_back(str);
		}//if
	}//for

	CDialog::OnOK();
}

