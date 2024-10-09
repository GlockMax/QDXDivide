// MTEditStlNameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resinv.h"
#include "MTEditStlNameDlg.h"
#include "direct.h"

// MTEditStlNameDlg dialog

IMPLEMENT_DYNAMIC(MTEditStlNameDlg, CDialog)

MTEditStlNameDlg::MTEditStlNameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(MTEditStlNameDlg::IDD, pParent)
{
	m_name = _T("");
}

MTEditStlNameDlg::~MTEditStlNameDlg()
{
}

void MTEditStlNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NUnitNameDlg)
	DDX_Control(pDX, IDC_STL_NAME, m_ListBox);
	DDX_LBString(pDX, IDC_STL_NAME, m_name);
}


BEGIN_MESSAGE_MAP(MTEditStlNameDlg, CDialog)
	ON_LBN_DBLCLK(IDC_STL_NAME, OnDblclkStlName)
END_MESSAGE_MAP()


// MTEditStlNameDlg message handlers


INT_PTR MTEditStlNameDlg::DoModal(const CString &path, const CString &name)
{
	m_path = path;
	m_name = name;
	return CDialog::DoModal();
}//int MTEditStlNameDlg::DoModal(const CString &path, const CString &name)

BOOL MTEditStlNameDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	int drive = _getdrive();
	char oldpath[512];
	_getcwd(oldpath,512);

	CString fn = m_path + "\\*.stl";
	DlgDirList(fn.GetBuffer(0),IDC_STL_NAME,0,DDL_READWRITE);
	CString str;
	str.LoadString(IDS_NO_STL_FILE);
	m_ListBox.AddString(str);
	int ind = m_ListBox.FindStringExact(0,m_name);
	if(ind != LB_ERR)
		m_ListBox.SetCurSel(ind);
	else
		m_ListBox.SetCurSel(0);

	_chdrive(drive);
	_chdir(oldpath);

	return TRUE;   // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void MTEditStlNameDlg::OnDblclkStlName()
{
	OnOK();
}//void MTEditStlNameDlg:OnActivatenDblclkStlName()



CCustomDlgProp::CCustomDlgProp(const CString& strName, const COleVariant& varValue , const CString & strDescr) : CMFCPropertyGridProperty(strName, varValue, strDescr)
{
	MachinePath = _T("");
}

void CCustomDlgProp::SetMachinePath(const CString &path)
{
	MachinePath = path;
}//void CCustomDlgProp::SetMachinePath(const CString &path)

void CCustomDlgProp::OnClickButton(CPoint point)
{
	MTEditStlNameDlg stl;
	if(stl.DoModal(MachinePath, GetValue()) != IDOK)
		return ;
	CString res = stl.m_name;
	SetValue(res);
}//void CCustomDlgProp::OnClickButton(CPoint point)


void MTEditStlNameDlg::OnOK()
{
	CDialog::OnOK();
	if (m_ListBox.GetCurSel() == 0)
		m_name.Empty();
}
