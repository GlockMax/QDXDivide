// MTEditSaveAsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MTEditSaveAsDlg.h"
#include "direct.h"

// MTEditSaveAsDlg dialog

IMPLEMENT_DYNAMIC(MTEditSaveAsDlg, CDialog)

MTEditSaveAsDlg::MTEditSaveAsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(MTEditSaveAsDlg::IDD, pParent)
{
	m_name = _T("");
}

MTEditSaveAsDlg::~MTEditSaveAsDlg()
{
}

void MTEditSaveAsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MACHINE_NAME, m_Edit);
	DDX_Control(pDX, IDC_MACHINE_NAMES, m_ListBox);
}


BEGIN_MESSAGE_MAP(MTEditSaveAsDlg, CDialog)
	ON_BN_CLICKED(IDOK, &MTEditSaveAsDlg::OnBnClickedOk)
	ON_LBN_DBLCLK(IDC_MACHINE_NAMES, OnDblclkMachineName)
	ON_LBN_SELCHANGE(IDC_MACHINE_NAMES, &MTEditSaveAsDlg::OnLbnSelchangeMachineNames)
END_MESSAGE_MAP()

INT_PTR MTEditSaveAsDlg::DoModal(const CString &path, const CString &name)
{
	m_path = path;
	m_name = name;
	return CDialog::DoModal();
}

// MTEditSaveAsDlg message handlers

BOOL MTEditSaveAsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	int drive = _getdrive();
	char oldpath[512];
	_getcwd(oldpath,512);

	CString fn = m_path;
	DlgDirList(fn.GetBuffer(0),IDC_MACHINE_NAMES,0,DDL_DIRECTORY);
	m_ListBox.DeleteString(0);
	int ind = m_ListBox.FindStringExact(-1,"["+m_name+"]");
	if(ind != LB_ERR)
		m_ListBox.SetCurSel(ind);

	_chdrive(drive);
	_chdir(oldpath);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void MTEditSaveAsDlg::OnBnClickedOk()
{
	
	CString sName;
	m_Edit.GetWindowText(sName);
	if(sName == CString(_T("")))
	{
		CString sError;sError.LoadString(IDS_ERROR_EMPTY_NAME);
		AfxMessageBox(sError);
		return;
	}//if
	// Проверка, нет ли такого имени
	int size = m_ListBox.GetCount();
	for( int i = 0; i < size ; i++)
	{
		CString sItem;
		m_ListBox.GetText(i, sItem);
		sItem = sItem.Mid(1);
		sItem = sItem.Mid(0,sItem.GetLength()-1);
		if(sItem == sName)
		{
			CString sReplace; sReplace.LoadString(IDS_EDITOR_REPLACE_MACHINE);
			if(AfxMessageBox(sReplace,MB_YESNO) == IDNO)
				return;
		}//if
	}//for
	m_name = sName;
	OnOK();
}

void MTEditSaveAsDlg::OnDblclkMachineName()
{
	int size = m_ListBox.GetCount();
	for(int i = 0; i < size; i++)
	{
		if(m_ListBox.GetSel(i))
		{
			CString sItem;
			m_ListBox.GetText(i, sItem);
			sItem = sItem.Mid(1);
			sItem = sItem.Mid(0,sItem.GetLength()-1);
			m_Edit.SetWindowText(sItem);
		}//if
	}//for
}//void MTEditSaveAsDlg::OnDblclkMachineName()
void MTEditSaveAsDlg::OnLbnSelchangeMachineNames()
{
	
	int size = m_ListBox.GetCount();
	for(int i = 0; i < size; i++)
	{
		if(m_ListBox.GetSel(i))
		{
			CString sItem;
			m_ListBox.GetText(i, sItem);
			sItem = sItem.Mid(1);
			sItem = sItem.Mid(0,sItem.GetLength()-1);
			m_Edit.SetWindowText(sItem);
		}//if
	}//for
}
