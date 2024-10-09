// NUnitCompDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ResInv.h"
#include "NUnitCompDlg.h"
#include "NMapIntToDouble.h"
#include "..\source\GridCtrl\GridCellNumeric.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int CALLBACK MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
   // lParamSort contains a pointer to the list view control.
   // The lParam of an item is just its index.
   CListCtrl* pListCtrl = (CListCtrl*) lParamSort;
   CString    strItem1 = pListCtrl->GetItemText(int(lParam1), 0);
   CString    strItem2 = pListCtrl->GetItemText(int(lParam2), 0);

   CString num1 = strItem1;
   num1.TrimLeft(_T("HD "));
   CString num2 = strItem2;
   num2.TrimLeft(_T("HD "));
   
   int f = strcmp(num1,num2);
   
   if(f != 0)
	   return f;

   return strcmp(strItem1,strItem2);
}


/////////////////////////////////////////////////////////////////////////////
// NUnitCompDlg property page
static DWORD aHelpIDs[]=
{
	IDC_LIST1,						HIDC_LIST1,
	IDC_COMP_KEY,					HIDC_COMP_KEY,
	IDC_COMP_VAL,					HIDC_COMP_VAL,
	IDC_COMP_NULL,					HIDC_COMP_NULL,
	0,0
};

IMPLEMENT_DYNCREATE(NUnitCompDlg, CPropertyPage)

NUnitCompDlg::NUnitCompDlg() : CPropertyPage(NUnitCompDlg::IDD)
{
}

NUnitCompDlg::~NUnitCompDlg()
{
}

void NUnitCompDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NUnitCompDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NUnitCompDlg, CPropertyPage)
	//{{AFX_MSG_MAP(NUnitCompDlg)
	ON_BN_CLICKED(IDC_COMP_NULL, OnCompNull)
	ON_BN_CLICKED(IDC_COMP_DEL, OnCompDel)
	ON_WM_HELPINFO()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NUnitCompDlg message handlers



void NUnitCompDlg::SetComp(NMapIntToDouble* ipDComp,NMapIntToDouble* ipHComp)
{
	pDComp = ipDComp;
	pHComp = ipHComp;
	POSITION pos;
	int key,i;
	double val;
	CString skey,sval;
	if (pDComp != nullptr)
	{
		for (pos = pDComp->GetStartPosition(), i = 0; pos != NULL; i++)
		{
			pDComp->GetNextAssoc(pos, key, val);
			if (key >= 1000)
				continue;
			skey.Format(_T("D %d"), key);
			Comp[skey] = val;
		}
	}
	if (pHComp != nullptr)
	{
		for (pos = pHComp->GetStartPosition(), i = 0; pos != NULL; i++)
		{
			pHComp->GetNextAssoc(pos, key, val);
			skey.Format(_T("H %d"), key);
			Comp[skey] = val;
		}
	}
}

BOOL NUnitCompDlg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CRect rect;
	GetDlgItem(IDC_STATIC)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	
	m_D_list.Create(rect, this, IDD);
	m_D_list.SetDefaultCellType(RUNTIME_CLASS(CGridCellNumeric));
	m_D_list.SetDefCellMargin(2);
	m_D_list.SetColumnCount(2);
	m_D_list.SetRowCount(int(Comp.GetCount() + 1));
	m_D_list.SetFixedRowCount(1);
	m_D_list.SetFixedColumnCount(1);
	m_D_list.SetAutoSizeStyle(GVS_BOTH);
	m_D_list.SetCellType(0, 1, RUNTIME_CLASS(CGridCell));
	m_D_list.SetCellType(0, 0, RUNTIME_CLASS(CGridCell));
	CString tmpstr;
	tmpstr.LoadString(IDS_COMP_N);
	m_D_list.SetItemText(0, 0, tmpstr);
	tmpstr.LoadString(IDS_COMP_VAL);
	m_D_list.SetItemText(0, 1, tmpstr);

	for (int i = 1; i < Comp.GetCount() + 1; ++i)
	{
		CGridCellNumeric *pCell = (CGridCellNumeric *)m_D_list.GetCell(i, 1);
		pCell->SetFlags(CGridCellNumeric::Negative | CGridCellNumeric::Math);
	}


	POSITION pos;
	CString key;
	int i;
	double val;
	CString skey,sval;
	for (pos = Comp.GetStartPosition(), i = 1; pos != NULL; i++)
	{
		Comp.GetNextAssoc(pos,key,val);
		sval.Format("%lf",val);
		m_D_list.SetCellType(i, 0, RUNTIME_CLASS(CGridCell));
		m_D_list.SetItemText(i, 0, key);
		m_D_list.SetItemText(i, 1, sval);
	}
	// Sort the list view items using my callback procedure.
	m_D_list.SortItems(0, true);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void NUnitCompDlg::OnOK() 
{
	UpdateData();
	CString key,id;
	for (int i = 1; i < m_D_list.GetRowCount() + 1; i++)
	{
		key = m_D_list.GetItemText(i,0);
		id = key.SpanExcluding(" ");
		key.Delete(0,id.GetLength()+1);
		if(id == _T("D"))
			if(pDComp != nullptr)
				(*pDComp)[atoi(key)] = atof(m_D_list.GetItemText(i,1));
		if(id == _T("H"))
			if (pHComp != nullptr)
				(*pHComp)[atoi(key)] = atof(m_D_list.GetItemText(i,1));
	}
	
	CPropertyPage::OnOK();
}

void NUnitCompDlg::OnCompNull() 
{
	CString sval;
	sval.Format("%lf", 0.0);
	for (int i = 1; i < m_D_list.GetRowCount() + 1; i++)
		m_D_list.SetItemText(i, 1, sval);
	m_D_list.RedrawWindow();
}

void NUnitCompDlg::ClearCompArrays(void)
{
	if(pDComp != nullptr)
		pDComp->RemoveAll();
	if(pHComp != nullptr)
		pHComp->RemoveAll();
}

void NUnitCompDlg::OnCompDel()
{
	ClearCompArrays();
	m_D_list.DeleteNonFixedRows();
	m_D_list.RedrawWindow();
	UpdateData(FALSE);
}