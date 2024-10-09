// DlgEditorSettings.cpp : implementation file
//

#include "stdafx.h"
//#include "Editor.h"
#include "DlgEditorSettings.h"
#include ".\dlgeditorsettings.h"


// DlgEditorSettings dialog

IMPLEMENT_DYNAMIC(DlgEditorSettings, CDialog)
DlgEditorSettings::DlgEditorSettings(CWnd* pParent /*=NULL*/)
	: CDialog(DlgEditorSettings::IDD, pParent)
	, EditDescription(_T(""))
{
}

DlgEditorSettings::~DlgEditorSettings()
{
}

void DlgEditorSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_COLOR, m_ColorCtrl);
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, EditDescription);
}


BEGIN_MESSAGE_MAP(DlgEditorSettings, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_COLOR, OnBnClickedButtonColor)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_LBN_SELCHANGE(IDC_LIST_STYLES, OnLbnSelchangeListStyles)
END_MESSAGE_MAP()


// DlgEditorSettings message handlers

void DlgEditorSettings::OnBnClickedButtonColor()
{
	CColorDialog dlg(CurColor,0,this);
	if(dlg.DoModal()!= IDOK)
		return;
	CurColor = dlg.GetColor();
	StylesArray[CurStyleNum]->color.red=GetRValue(CurColor);
	StylesArray[CurStyleNum]->color.green=GetGValue(CurColor);
	StylesArray[CurStyleNum]->color.blue=GetBValue(CurColor);
	UpdateColorButton();
}

void DlgEditorSettings::Renew(void)
{
	EditDescription = StylesArray[CurStyleNum]->description;
	CurColor = RGB(StylesArray[CurStyleNum]->color.red,StylesArray[CurStyleNum]->color.green,
		StylesArray[CurStyleNum]->color.blue);
	UpdateColorButton();
	UpdateData(FALSE);
}

void DlgEditorSettings::UpdateColorButton(void)
{
	COLORMAP Map;
	Map.from = RGB(255, 255, 255);
	Map.to = CurColor;
	ColorButtonBmp.DeleteObject();
	ColorButtonBmp.LoadMappedBitmap(IDB_COLOR_BUTTON_BMP, 0, &Map, 1);
	m_ColorCtrl.SetBitmap(HBITMAP(ColorButtonBmp));
}

BOOL DlgEditorSettings::OnInitDialog()
{
	CDialog::OnInitDialog();

	StylesArray.Add(&pConfig->StyleCmdNumber);
	StylesArray.Add(&pConfig->StyleComment);
	StylesArray.Add(&pConfig->StyleDigit);
	StylesArray.Add(&pConfig->StyleGCode);
	StylesArray.Add(&pConfig->StyleMCode);
	StylesArray.Add(&pConfig->StyleOther);
	StylesArray.Add(&pConfig->StyleReg);
	StylesArray.Add(&pConfig->StyleText);
	
	pListStyles = (CListBox*)GetDlgItem(IDC_LIST_STYLES);
	for(int i=0;i<StylesArray.GetSize();i++)
		pListStyles->AddString(StylesArray[i]->short_name);
	CurStyleNum = 0;
	pListStyles->SetCurSel(CurStyleNum);

	CButton *pButton = (CButton*)GetDlgItem(IDC_CHECK_CMDCOLOR);
	if(pConfig->fCmdNumber)
		pButton->SetCheck(BST_CHECKED);
	else
		pButton->SetCheck(BST_UNCHECKED);
	pButton = (CButton*)GetDlgItem(IDC_CHECK_NUMSTRING);
	if(pConfig->fEnableNumString)
		pButton->SetCheck(BST_CHECKED);
	else
		pButton->SetCheck(BST_UNCHECKED);
	Renew();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void DlgEditorSettings::OnBnClickedOk()
{
	CButton *pButton = (CButton*)GetDlgItem(IDC_CHECK_CMDCOLOR);
	if(BST_CHECKED == pButton->GetCheck())
		pConfig->fCmdNumber = true;
	else
		pConfig->fCmdNumber = false;
	pButton = (CButton*)GetDlgItem(IDC_CHECK_NUMSTRING);
	if(BST_CHECKED == pButton->GetCheck())
		pConfig->fEnableNumString = true;
	else
		pConfig->fEnableNumString = false;
	OnOK();
}

void DlgEditorSettings::OnLbnSelchangeListStyles()
{
	CurStyleNum = pListStyles->GetCurSel();
	Renew();
}
