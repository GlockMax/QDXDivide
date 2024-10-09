// NViewInterfaceSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NCMProject.h"
#include "NCMVersion.h"
#include "NViewInterfaceSettingsDlg.h"
#include "afxdialogex.h"


// NViewInterfaceSettingsDlg dialog

IMPLEMENT_DYNAMIC(NViewInterfaceSettingsDlg, CMFCPropertyPage)

NViewInterfaceSettingsDlg::NViewInterfaceSettingsDlg()
	: CMFCPropertyPage(IDD_VIEWINTERFACE_SETTINGS)
{

}

NViewInterfaceSettingsDlg::~NViewInterfaceSettingsDlg()
{
}

void NViewInterfaceSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Check(pDX, IDC_REVERSE_MW, m_Reverse);
	DDX_Control(pDX, IDC_CADCAM_NAME, m_NamesCtrl);
	CPropertyPage::DoDataExchange(pDX);
}

void NViewInterfaceSettingsDlg::OnSelchangeNamesCtrl()
{
}


BEGIN_MESSAGE_MAP(NViewInterfaceSettingsDlg, CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_CADCAM_NAME, OnSelchangeNamesCtrl)
END_MESSAGE_MAP()


// NViewInterfaceSettingsDlg message handlers


void NViewInterfaceSettingsDlg::OnOK()
{
	AfxGetApp()->WriteProfileInt(_T(""), _T("WheelReverse"), m_Reverse);
	NCM_PROJECT.GetGlobalState().SetWheelReverse(m_Reverse);
	int i = m_NamesCtrl.GetCurSel();
	if (i != CB_ERR)
	{
		CString Name;
		m_NamesCtrl.GetLBText(i, Name);
		AfxGetApp()->WriteProfileString(_T(""), _T("ViewInterfaceStyle"), Name);
		NCM_PROJECT.GetGlobalState().SetViewInterface(Name);
	}
	CMFCPropertyPage::OnOK();
}


BOOL NViewInterfaceSettingsDlg::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();

	CString ViewIntType = AfxGetApp()->GetProfileString(_T(""), _T("ViewInterfaceStyle"), _T("NCManager"));
	m_NamesCtrl.SelectString(-1, ViewIntType);
	m_Reverse = NCM_PROJECT.GetGlobalState().GetWheelReverse();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
