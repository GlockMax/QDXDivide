// NAppearSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Mainfrm.h"
#include "NCMDoc.h"
#include "NColor.h"
#include "ResInv.h"
#include "NAppearSettingsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NAppearSettingsDlg property page
static DWORD aHelpIDs[]=
{
	IDC_APPEARANCE_ELEMENT,		HIDC_APPEARANCE_ELEMENT,
	IDC_COLOR,					HIDC_COLOR,
	0,0
};

IMPLEMENT_DYNCREATE(NAppearSettingsDlg, CMFCPropertyPage)

NAppearSettingsDlg::NAppearSettingsDlg() : CMFCPropertyPage(NAppearSettingsDlg::IDD)
, m_Grad(false)
{
	//{{AFX_DATA_INIT(NAppearSettingsDlg)
	m_Element = _T("");
	//}}AFX_DATA_INIT
	HDC hDCScreen = ::GetDC(NULL);
	ASSERT(hDCScreen != NULL);
	cyPixelsPerInch = GetDeviceCaps(hDCScreen, LOGPIXELSY);
	::ReleaseDC(NULL, hDCScreen);
}

NAppearSettingsDlg::~NAppearSettingsDlg()
{
	// Delete buffers
	POSITION pos;
	CString key;
	NColor *c;
	for(pos = ColorsBuffer.GetStartPosition(); pos !=NULL;)
	{
		ColorsBuffer.GetNextAssoc(pos,key,c);
		delete c;
	}
	ColorsBuffer.RemoveAll();
}

void NAppearSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NAppearSettingsDlg)
	DDX_Control(pDX, IDC_COLOR, m_ColorCtrl);
	DDX_Control(pDX, IDC_APPEARANCE_PICTURE, m_PictureCtrl);
	DDX_Control(pDX, IDC_APPEARANCE_ELEMENT, m_ElementCtrl);
	DDX_CBString(pDX, IDC_APPEARANCE_ELEMENT, m_Element);
	DDX_Check(pDX, IDC_CHECK2, m_Grad);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NAppearSettingsDlg, CMFCPropertyPage)
	//{{AFX_MSG_MAP(NAppearSettingsDlg)
	ON_CBN_SELCHANGE(IDC_APPEARANCE_ELEMENT, OnSelchangeAppearanceElement)
	ON_BN_CLICKED(IDC_COLOR, OnColor)
	ON_WM_HELPINFO()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NAppearSettingsDlg message handlers
CString NAppearSettingsDlg::DefaultElement("???");

BOOL NAppearSettingsDlg::OnInitDialog() 
{
	CMFCPropertyPage::OnInitDialog();
	
	if(DefaultElement == "???")
		DefaultElement = NCM_PROJECT.TrBaseCur["ФонПанелиОбъектов"];
	POSITION pos;
	CString key;
	NColor *pColor, *pColorBuffer;
	// fill buffers and fill elements' combo-box
	for(pos = NCM_PROJECT.Colors.GetStartPosition(); pos !=NULL;)
	{
		NCM_PROJECT.Colors.GetNextAssoc(pos, key, pColor);
		pColorBuffer = new NColor;
		*pColorBuffer = *pColor;
		ColorsBuffer[key] = pColorBuffer;
		m_ElementCtrl.AddString(NCM_PROJECT.TrBaseCur[key]);
	}
	// set element to last used value
	m_Element = DefaultElement;

	m_Grad = *NCM_PROJECT.IntParams["GradientBack"];

	ElementChanged();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void NAppearSettingsDlg::OnOK() 
{
	DefaultElement = m_Element;	
	POSITION pos;
	CString key;
	NColor *pColor, *pColorBuffer;
	// fill settings from buffers
	for(pos = NCM_PROJECT.Colors.GetStartPosition(); pos !=NULL;)
	{
		NCM_PROJECT.Colors.GetNextAssoc(pos, key, pColor);
		if(ColorsBuffer.Lookup(key, pColorBuffer))
			*pColor = *pColorBuffer;
	}
	int* pInt;
	if(NCM_PROJECT.IntParams.Lookup("GradientBack", pInt))
		*pInt = m_Grad;
	((CNCMDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument())->ApplyAppearSettings();

	CMFCPropertyPage::OnOK();
}

void NAppearSettingsDlg::OnSelchangeAppearanceElement() 
{
	UpdateData();
	ElementChanged();
}

// Call when new element is selected.
void NAppearSettingsDlg::ElementChanged()
{
	// Update color button
	NColor *pColor;
	CString BufElement = NCM_PROJECT.TrCurBase[m_Element];
	ColorsBuffer.Lookup(BufElement, pColor);
	UpdateColorButton(pColor->GetRGB());
	UpdateData(FALSE);
}

void NAppearSettingsDlg::OnColor() 
{
	NColor *pCol;
	ColorsBuffer.Lookup(NCM_PROJECT.TrCurBase[m_Element], pCol);
	COLORREF clrInit = pCol->GetRGB();
	CColorDialog ColorDialog(clrInit, 0, this);
	ColorDialog.m_cc.Flags |= CC_FULLOPEN;
	if(ColorDialog.DoModal() != IDOK)
		return;
	COLORREF NewColor = ColorDialog.GetColor();
	pCol->SetRGB(NewColor);
	UpdateColorButton(NewColor);
}

void NAppearSettingsDlg::UpdateColorButton(COLORREF NewColor)
{
	COLORMAP Map;
	Map.from = RGB(255, 255, 255);
	Map.to = NewColor;
	ColorButtonBmp.DeleteObject();
	ColorButtonBmp.LoadMappedBitmap(IDB_COLOR_BUTTON_BMP, 0, &Map, 1);
	m_ColorCtrl.SetBitmap(HBITMAP(ColorButtonBmp));
}

BOOL NAppearSettingsDlg::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.dwExStyle |= WS_EX_CONTEXTHELP;
	
	return CMFCPropertyPage::PreCreateWindow(cs);
}
