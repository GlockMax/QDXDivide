// NStateSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NMapStringToOb.h"
#include "NStateParamsPane.h"
#include "NStateSettingsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NStateSettingsDlg property page

IMPLEMENT_DYNCREATE(NStateSettingsDlg, CMFCPropertyPage)

NStateSettingsDlg::NStateSettingsDlg() : CMFCPropertyPage(NStateSettingsDlg::IDD)
{
	//{{AFX_DATA_INIT(NStateSettingsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	pParamsPane = NULL;
}

NStateSettingsDlg::~NStateSettingsDlg()
{
}

void NStateSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NStateSettingsDlg)
	DDX_Control(pDX, IDC_VIS_ELEM_LIST, VisElemListBox);
	DDX_Control(pDX, IDC_UNVIS_ELEM_LIST, UnvisElemListBox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NStateSettingsDlg, CMFCPropertyPage)
	//{{AFX_MSG_MAP(NStateSettingsDlg)
	ON_BN_CLICKED(IDC_ADD_ELEM, OnAddElem)
	ON_BN_CLICKED(IDC_DEL_ELEM, OnDelElem)
	ON_BN_CLICKED(IDC_UP, OnUp)
	ON_BN_CLICKED(IDC_DOWN, OnDown)
	ON_WM_HELPINFO()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NStateSettingsDlg message handlers

BOOL NStateSettingsDlg::OnInitDialog() 
{

	if(!pParamsPane)
		return FALSE;

	CMFCPropertyPage::OnInitDialog();
	CStringArray & VisElements = pParamsPane->GetVisElemList();
	CStringArray UnvisElements;
	pParamsPane->GetUnvisElemList(UnvisElements);
	for(int i = 0; i < VisElements.GetSize(); i++)
		VisElemListBox.AddString(VisElements[i]);
	VisElemListBox.AddString("");
	for(int i = 0; i < UnvisElements.GetSize(); i++)
		UnvisElemListBox.AddString(UnvisElements[i]);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void NStateSettingsDlg::OnAddElem() 
{
	
	int SelCount = UnvisElemListBox.GetSelCount();
	if(SelCount == LB_ERR || SelCount == 0)
		return;
	int *pSelIndexArray = new int [SelCount];
	UnvisElemListBox.GetSelItems(SelCount, pSelIndexArray);
	CString ItemText;

	int InsertIndex = VisElemListBox.GetCount() - 1;
	for(int i = 0; i <= InsertIndex; i++)
		if(VisElemListBox.GetSel(i) > 0)
		{
			InsertIndex = i;
			break;
		}

	for(int i = 0; i < SelCount; i++)
	{
		UnvisElemListBox.GetText(pSelIndexArray[i], ItemText);
		VisElemListBox.InsertString(InsertIndex++,ItemText);
	}
	for(int i = SelCount - 1; i >= 0; i--)
		UnvisElemListBox.DeleteString(pSelIndexArray[i]);
	delete[] pSelIndexArray;
}

void NStateSettingsDlg::OnDelElem() 
{
	int SelCount = VisElemListBox.GetSelCount();
	if(SelCount == LB_ERR || SelCount == 0)
		return;
	int *pSelIndexArray = new int [SelCount];
	VisElemListBox.GetSelItems(SelCount, pSelIndexArray);
	CString ItemText;
	for(int i = 0; i < SelCount; i++)
	{
		VisElemListBox.GetText(pSelIndexArray[i], ItemText);
		if(ItemText == "")
			pSelIndexArray[i] = -1;
		else
			UnvisElemListBox.AddString(ItemText);
	}
	for(int i = SelCount - 1; i >= 0; i--)
		if(pSelIndexArray[i] >= 0)
			VisElemListBox.DeleteString(pSelIndexArray[i]);
	delete[] pSelIndexArray;
}

void NStateSettingsDlg::OnUp() 
{
	
	int SelCount = VisElemListBox.GetSelCount();
	if(SelCount == LB_ERR || SelCount == 0)
		return;
	int *pSelIndexArray = new int [SelCount];
	VisElemListBox.GetSelItems(SelCount, pSelIndexArray);
	CStringArray ItemTextArray;
	ItemTextArray.SetSize(SelCount);
	CString ItemText;
	for(int i = 0; i < SelCount; i++)
	{
		VisElemListBox.GetText(pSelIndexArray[i], ItemText);
		ItemTextArray.SetAt(i, ItemText);
	}
	for(int i = SelCount - 1; i >= 0; i--)
		if(ItemTextArray[i] != "" && pSelIndexArray[i] != 0)
			VisElemListBox.DeleteString(pSelIndexArray[i]);
	for(int i = 0; i < SelCount; i++)
		if(ItemTextArray[i] != "" && pSelIndexArray[i] != 0)
		{
			VisElemListBox.InsertString(pSelIndexArray[i] - 1, ItemTextArray[i]);
			VisElemListBox.SetSel(pSelIndexArray[i] - 1);
		}
	delete[] pSelIndexArray;
}

void NStateSettingsDlg::OnDown() 
{
	
	int SelCount = VisElemListBox.GetSelCount();
	if(SelCount == LB_ERR || SelCount == 0)
		return;
	int LastItemIndex = VisElemListBox.GetCount() - 2;
	int *pSelIndexArray = new int [SelCount];
	VisElemListBox.GetSelItems(SelCount, pSelIndexArray);
	CStringArray ItemTextArray;
	ItemTextArray.SetSize(SelCount);
	CString ItemText;
	for(int i = 0; i < SelCount; i++)
	{
		VisElemListBox.GetText(pSelIndexArray[i], ItemText);
		ItemTextArray.SetAt(i, ItemText);
	}
	for(int i = SelCount - 1; i >= 0; i--)
		if(ItemTextArray[i] != "" && pSelIndexArray[i] != LastItemIndex)
			VisElemListBox.DeleteString(pSelIndexArray[i]);
	for(int i = 0; i < SelCount; i++)
		if(ItemTextArray[i] != "" && pSelIndexArray[i] != LastItemIndex)
		{
			VisElemListBox.InsertString(pSelIndexArray[i] + 1, ItemTextArray[i]);
			VisElemListBox.SetSel(pSelIndexArray[i] + 1);
		}
	delete[] pSelIndexArray;
}

void NStateSettingsDlg::OnOK() 
{
	if(!pParamsPane)
		return;

	CStringArray & VisElements = pParamsPane->GetVisElemList();
	if(VisElements.GetSize() != 0)
	{
		VisElements.RemoveAll();
		ASSERT(VisElements.GetSize() == 0);
	}
	CString ElemName;
	for(int i = 0; i < VisElemListBox.GetCount() - 1; i++)
	{
		VisElemListBox.GetText(i, ElemName);
		VisElements.Add(ElemName);
	}		
	
	CMFCPropertyPage::OnOK();
}

void NStateSettingsDlg::SetParamsPane(NStateParamsPane &ParamsPane)
{
	pParamsPane = &ParamsPane;
}
