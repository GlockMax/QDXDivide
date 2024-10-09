// NStateParamsPane.cpp : implementation file
//

#include "stdafx.h"
#include "NCMProject.h"
#include "NCMDoc.h"
#include "NColor.h"
#include "NFont.h"
#include "NSettingsDlg.h"
#include "NControlPanel.h"
#include "NStateParamsPane.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define AFX_REG_SECTION_FMT    _T("%sParamsPane-%d")
#define AFX_REG_SECTION_FMT_EX _T("%sParamsPane-%d%x")
/////////////////////////////////////////////////////////////////////////////
// NStateParamsPane

IMPLEMENT_DYNCREATE(NStateParamsPane, CDockablePane)

NStateParamsPane::NStateParamsPane()
{
}

NStateParamsPane::~NStateParamsPane()
{
	DeleteElements();
}


BEGIN_MESSAGE_MAP(NStateParamsPane, CDockablePane)
	//{{AFX_MSG_MAP(NStateParamsPane)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_COMMAND(ID_HIDE_ITEM, OnHideItem)
	ON_COMMAND(ID_STATE_VIEW_SETTINGS, OnStateViewSettings)
	//}}AFX_MSG_MAP
	ON_WM_CONTEXTMENU()
	ON_WM_SETFOCUS()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()




/////////////////////////////////////////////////////////////////////////////
// NStateParamsPane diagnostics

#ifdef _DEBUG
void NStateParamsPane::AssertValid() const
{
	CDockablePane::AssertValid();
}

void NStateParamsPane::Dump(CDumpContext& dc) const
{
	CDockablePane::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// NStateParamsPane message handlers

void NStateParamsPane::OnUpdate(CDockablePane* pSender, LPARAM lHint, CObject* pHint) 
{
 // Update colors
	if(lHint & N_SETTINGS)
	{
		lc.SetSettings();
		CreateStateView();
	}
}

void NStateParamsPane::OnSize(UINT nType, int cx, int cy) 
{
	CDockablePane::OnSize(nType, cx, cy);
	
	lc.MoveWindow(0, 0, cx, cy, TRUE);
	// Coloumn1 width fits space
	lc.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
	lc.RedrawWindow();
}

int NStateParamsPane::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rect;
	GetClientRect(&rect);

	lc.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_SHOWSELALWAYS,
		rect, this, ID_LISTCTRL);
	lc.SetExtendedStyle(lc.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	CString tmpstr;
	tmpstr.LoadString(IDS_STATE_NAME);
	lc.InsertColumn(0,tmpstr,LVCFMT_LEFT,105,0);
	tmpstr.LoadString(IDS_STATE_VALUE);
	lc.InsertColumn(1,tmpstr,LVCFMT_LEFT,105,1);
	tmpstr.LoadString(IDS_STATE_NOTE);
	lc.InsertColumn(2,tmpstr,LVCFMT_LEFT,15,2);

	return 0;
}

void NStateParamsPane::Update()
{
	OnUpdate(NULL,N_STATE,NULL);
	UpdateWindow();
}

void NStateParamsPane::ResetElements()
{
}

void NStateParamsPane::DeleteElements()
{
	if(AllElemList.empty())
		return;
	for each(auto el in AllElemList)
	{ 
		NStateElem* pElem = static_cast<NStateElem*>((void*)el.second);
		delete pElem;
	}
	AllElemList.RemoveAll();
	ASSERT(AllElemList.empty());
}

CStringArray & NStateParamsPane::GetVisElemList()
{
	return VisElemList;
}

void NStateParamsPane::GetUnvisElemList(CStringArray &UnvisElemList)
{
	auto AllElemCount = AllElemList.GetSize();
	auto VisElemCount = VisElemList.GetSize();
	CString ElemName;
	CObject * pObject;
	for(int i = 0; i < AllElemCount; i++)
	{
		AllElemList.GetAt(i, ElemName, pObject);
		int j = 0;
		for (; j < VisElemCount; j++)
			if(ElemName == VisElemList[j])
				break;
		if(j == VisElemCount)
			UnvisElemList.Add(ElemName);
	}
}

void NStateParamsPane::OnHideItem() 
{
	// Hide selected item
	int SelCount = lc.GetSelectedCount();
	if( SelCount == 0)
		return;
	int * pIndexArray = new int [SelCount];
	POSITION pos = lc.GetFirstSelectedItemPosition();
	for(int i = 0; pos != NULL; i++)
		pIndexArray[i] = lc.GetNextSelectedItem(pos);
	for(int i = SelCount - 1; i >= 0; i--)
		VisElemList.RemoveAt(pIndexArray[i]);
	delete[] pIndexArray;
	OnUpdate(NULL, N_SETTINGS, NULL);
}

void NStateParamsPane::OnStateViewSettings() 
{
	NSettingsDlg SettDlg;
	SettDlg.StateSettDlg.SetParamsPane(*this);
	SettDlg.SetActivePage(&SettDlg.StateSettDlg);
	SettDlg.DoModal();
	//NSettingsDlg SettDlg;
	//SettDlg.SetActivePage(&SettDlg.AppearSettDlg);
	//NAppearSettingsDlg::DefaultElement = CString(_T("ТекстПанелиКонтроля"));
	//SettDlg.DoModal();
}

void NStateParamsPane::CreateStateView()
{
	lc.DeleteAllItems();
	// Update values
	auto VisElemCount = VisElemList.GetSize();
	if(VisElemCount == 0)
		return;
	for(int i = 0, j = 0; i < VisElemCount; ++i)
	{
		CString ElemName = VisElemList[i];
		auto it = AllElemList.find(ElemName);
		if (it == AllElemList.end())
			continue;
		NStateElem* pElem = static_cast<NStateElem*>((void*)it->second);
		lc.InsertItem(j, ElemName);
		lc.SetItemData(j, 0);
		lc.SetItemText(j, 1, pElem->GetValueString());
		lc.SetItemText(j, 2, pElem->GetNoteString());
		++j;
	}
}
NMapStringToOb * NStateParamsPane::GetAllElemList()
{
	return &AllElemList;
}

void NStateParamsPane::UpdateControl()
{

// update values on the lc control

	auto VisElemCount = VisElemList.GetSize();
	CString ElemName;
	CString value, note;
	for(int i = 0,j = 0; i < VisElemCount && j < lc.GetItemCount(); i++)
	{
		ElemName = VisElemList[i];
		auto it = AllElemList.find(ElemName);
		if (it == AllElemList.end())
			continue;
		NStateElem* pElem = static_cast<NStateElem*>((void*)it->second);
		value = pElem->GetValueString();
		note = pElem->GetNoteString();
		if(value == lc.GetItemText(j,1))
		{
			if(lc.GetSafeHwnd())// To prevent assertion
				if(lc.GetItemData(j) != 0)
					lc.SetItemData(j, 0);
		}
		else
		{
			lc.SetItemData(j, RGB(255, 0, 0));
			lc.SetItemText(j, 1, value);
		}

		if(note != lc.GetItemText(j,2))
			lc.SetItemText(j, 2, note);
		j++;
	}
	RedrawWindow();
}

CNCMDoc* NStateParamsPane::GetDocument() 
{
	CFrameWnd *pMainFrame = (CFrameWnd *)AfxGetMainWnd();
	if(!pMainFrame)
		return NULL;
	return (CNCMDoc *)pMainFrame->GetActiveDocument();
}

void NStateParamsPane::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	UINT idMenu = lc.GetSelectedCount() == 0 ? IDR_STATE_VIEW_NOITEM : IDR_STATE_VIEW_ITEM;
	VERIFY(menu.LoadMenu(idMenu));
	((CWinAppEx *)AfxGetApp())->GetContextMenuManager()->ShowPopupMenu(*menu.GetSubMenu(0), point.x, point.y, this, TRUE);
	menu.DestroyMenu();
}

void NStateParamsPane::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	lc.SetFocus();
}

CString NStateParamsPane::GetExportString(bool Title, bool VisibleOnly, CStringArray *pElemList/* = NULL*/)
{
	CString String;
	CString Delim(";");
	CStringArray *pList2Process = pElemList;
	if(!pList2Process && VisibleOnly)
		pList2Process = &VisElemList;

	if(pList2Process)
	{
		auto VisElemCount = pList2Process->GetSize();
		for(int i = 0; i < VisElemCount; i++)
		{
			CString ElemName;
			CString value, note;
			ElemName = pList2Process->GetAt(i);
			auto it = AllElemList.find(ElemName);
			if (it == AllElemList.end())
				continue;
			NStateElem* pElem = static_cast<NStateElem*>((void*)it->second);
			if(Title)
				String += ElemName + Delim;
			else
				String += pElem->GetValueString() + Delim;
		}
	}
	else
	{
		for each (auto el in AllElemList)
		{
			NStateElem* pElem = static_cast<NStateElem*>((void*)el.second);
			CString ElemName = el.first;
			if(Title)
				String += ElemName + Delim;
			else
				String += pElem->GetValueString() + Delim;
		}
	}
	return String;
}
BOOL NStateParamsPane::LoadState(LPCTSTR lpszProfileName, int nIndex, UINT uiID)
{
	//CString strProfileName = ::AFXGetRegPath(strControlBarProfile, lpszProfileName);

	if (nIndex == -1)
	{
		nIndex = GetDlgCtrlID();
	}

	CString strSection;
	if (uiID == (UINT) -1)
	{
		strSection.Format(AFX_REG_SECTION_FMT, lpszProfileName, nIndex);
	}
	else
	{
		strSection.Format(AFX_REG_SECTION_FMT_EX, lpszProfileName, nIndex, uiID);
	}

	CSettingsStoreSP regSP;
	CSettingsStore& reg = regSP.Create(FALSE, TRUE);

	if (!reg.Open(strSection))
	{
		return FALSE;
	}

	int Width;
	reg.Read(_T("Width0"), Width);
	lc.SetColumnWidth(0, Width);
	reg.Read(_T("Width1"), Width);
	lc.SetColumnWidth(1, Width);

	return CDockablePane::LoadState(lpszProfileName, nIndex, uiID);
}
BOOL NStateParamsPane::SaveState(LPCTSTR lpszProfileName, int nIndex, UINT uiID)
{

	//CString strProfileName = ::AFXGetRegPath(strBaseControlBarProfile, lpszProfileName);

	if (nIndex == -1)
	{
		nIndex = GetDlgCtrlID();
	}

	CString strSection;
	if (uiID == (UINT) -1)
	{
		strSection.Format(AFX_REG_SECTION_FMT, lpszProfileName, nIndex);
	}
	else
	{
		strSection.Format(AFX_REG_SECTION_FMT_EX, lpszProfileName, nIndex, uiID);
	}

	CSettingsStoreSP regSP;
	CSettingsStore& reg = regSP.Create(FALSE, FALSE);

	if (reg.CreateKey(strSection))
	{
		reg.Write(_T("Width0"), lc.GetColumnWidth(0));
		reg.Write(_T("Width1"), lc.GetColumnWidth(1));
	}

	return CDockablePane::SaveState(lpszProfileName, nIndex, uiID);
}

void NStateParamsPane::OnShowWindow( BOOL bShow, UINT nStatus )
{
	if(bShow)
		UpdateControl();
}
