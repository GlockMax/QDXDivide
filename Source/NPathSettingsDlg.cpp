// NPathSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ConstDef.h"
#include "NPathSettingsDlg.h"
#include "NCMProject.h"
#include "ResLocal.h"
#include "NSurfSettings.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NPathSettingsDlg property page

IMPLEMENT_DYNCREATE(NPathSettingsDlg, CMFCPropertyPage)

NPathSettingsDlg::NPathSettingsDlg()	
	: CMFCPropertyPage(NPathSettingsDlg::IDD),
	pGroupPath(NULL)
{
	//{{AFX_DATA_INIT(NPathSettingsDlg)
	//}}AFX_DATA_INIT
}

NPathSettingsDlg::~NPathSettingsDlg()
{
}

void NPathSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROPLIST_LOCATION, m_PropListLocation);
}


BEGIN_MESSAGE_MAP(NPathSettingsDlg, CMFCPropertyPage)
	//{{AFX_MSG_MAP(NPathSettingsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NPathSettingsDlg message handlers

BOOL NPathSettingsDlg::OnInitDialog() 
{
	CMFCPropertyPage::OnInitDialog();

	NCommonPaths &CommonPaths = NCM_PROJECT.GetCommonPaths();

	CRect rectPropList;
	m_PropListLocation.GetClientRect(&rectPropList);
	m_PropListLocation.MapWindowPoints(this, &rectPropList);
	
	m_PropList.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, rectPropList, this, (UINT)-1);

	m_PropList.EnableHeaderCtrl(FALSE);
	m_PropList.EnableDescriptionArea();
	m_PropList.SetVSDotNetLook(TRUE);
	m_PropList.MarkModifiedProperties(TRUE);

	m_PropList.SetAlphabeticMode(FALSE);
	m_PropList.SetShowDragContext(TRUE);

	CString ParName;
	CString DescName;
	ParName.LoadString(IDS_PREF_PATH);
	pGroupPath = new CMFCPropertyGridProperty(ParName);
	NCustomDlgProp *pProp;

	ParName.LoadString(IDS_PREF_UNITS);
	DescName.LoadString(IDS_PREF_UNITS_DESC);
	pProp = new NCustomDlgProp(ParName, COleVariant(CommonPaths.GetPath(CF_UNITS)), DescName);
	pProp->SetData(CF_UNITS);
	pGroupPath->AddSubItem(pProp);

	ParName.LoadString(IDS_PREF_MACHINES);
	DescName.LoadString(IDS_PREF_MACHINES_DESC);
	pProp = new NCustomDlgProp(ParName, COleVariant(CommonPaths.GetPath(CF_MACHINES)), DescName);
	pProp->SetData(CF_MACHINES);
	pGroupPath->AddSubItem(pProp);

	ParName.LoadString(IDS_PREF_SETTINGS);
	DescName.LoadString(IDS_PREF_SETTINGS_DESC);
	pProp = new NCustomDlgProp(ParName, COleVariant(CommonPaths.GetPath(CF_SETTINGS)), DescName);
	pProp->SetData(CF_SETTINGS);
	pGroupPath->AddSubItem(pProp);

	ParName.LoadString(IDS_PREF_PROTOTYPES);
	DescName.LoadString(IDS_PREF_PROTOTYPES_DESC);
	pProp = new NCustomDlgProp(ParName, COleVariant(CommonPaths.GetPath(CF_PROTOTYPES)), DescName);
	pProp->SetData(CF_PROTOTYPES);
	pGroupPath->AddSubItem(pProp);

	ParName.LoadString(IDS_PREF_NCM);
	DescName.LoadString(IDS_PREF_NCM_DESC);
	pProp = new NCustomDlgProp(ParName, COleVariant(CommonPaths.GetPath(CF_TOOLS)), DescName);
	pProp->SetData(CF_TOOLS);
	pGroupPath->AddSubItem(pProp);

	m_PropList.AddProperty(pGroupPath);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void NPathSettingsDlg::OnOK() 
{
	if(!pGroupPath)
		return;

	AfxMessageBox(IDS_RELOAD, MB_OK);

	for(int i = 0; i < pGroupPath->GetSubItemsCount(); ++i)
	{
		CMFCPropertyGridProperty *pProp = pGroupPath->GetSubItem(i);
		NCM_PROJECT.GetCommonPaths().SetPath(static_cast<CommonFolder>(pProp->GetData()), pProp->GetValue());
	}
	CMFCPropertyPage::OnOK();
}





NCustomDlgProp::NCustomDlgProp(const CString& strName, const COleVariant& varValue, const CString& strDesc) :
	CMFCPropertyGridProperty(strName, varValue, strDesc)
{
}

void NCustomDlgProp::OnClickButton(CPoint /*point*/)
{
	CString Folder = GetValue();
	if(((CWinAppEx *)AfxGetApp())->GetShellManager()->BrowseForFolder(Folder, NULL, Folder))
	{
		SetValue(Folder + "\\");
	}
}
