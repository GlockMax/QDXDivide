#include "StdAfx.h"
#include "direct.h"
#include "nmachnamedlg.h"

NMachNameDlg::NMachNameDlg(void)
{
}

NMachNameDlg::~NMachNameDlg(void)
{
}
CString NMachNameDlg::GetName(void)
{
	CString tmp = m_name;
	tmp.Delete(0);
	tmp.Delete(tmp.GetLength()-1);
	return tmp;
}

BOOL NMachNameDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString title;
	title.LoadString(IDS_MT_TITLE);
	CDialog::SetWindowText(title);

	int drive = _getdrive();
	char oldpath[512];
	_getcwd(oldpath, 512);

	CString fn = m_path;
	DlgDirList(fn.GetBuffer(0), IDC_UNIT_NAME, 0, DDL_DIRECTORY);
	m_ListBox.DeleteString(0);
	int ind = m_ListBox.FindStringExact(-1, _T("[") + m_name + _T("]"));
	if(ind != LB_ERR)
		m_ListBox.SetCurSel(ind);

	_chdrive(drive);
	_chdir(oldpath);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
