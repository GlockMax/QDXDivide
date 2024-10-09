// NReportCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "WM_APP_messages.h"
#include "ResLocal.h"
#include "cadrID.h"
#include "NReportCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(NReportCtrl, MListCtrl)

NReportCtrl::NReportCtrl()
{
}

NReportCtrl::~NReportCtrl()
{
}

BEGIN_MESSAGE_MAP(NReportCtrl, MListCtrl)
	//{{AFX_MSG_MAP(NReportCtrl)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(NM_DBLCLK, &NReportCtrl::OnNMDblclk)
	ON_NOTIFY_REFLECT_EX(LVN_ITEMCHANGED, OnListItemChanged)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_REPORT_RUNTO, &NReportCtrl::OnReportRunto)
	ON_COMMAND(ID_REPORT_CLEAR, &NReportCtrl::OnReportClear)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

int NReportCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (MListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	CString tmpstr;
	tmpstr.LoadString(IDS_ERR_NAME);
	InsertColumn(0, tmpstr, LVCFMT_LEFT, 25, 0);
	tmpstr.LoadString(IDS_ERR_PROG);
	InsertColumn(1, tmpstr, LVCFMT_RIGHT, 25, 1);
	tmpstr.LoadString(IDS_ERR_BLOCK);
	InsertColumn(2, tmpstr, LVCFMT_RIGHT, 25, 2);
	tmpstr.LoadString(IDS_ERR_EXEC);
	InsertColumn(3, tmpstr, LVCFMT_RIGHT, 25, 2);
	tmpstr.LoadString(IDS_ERR_REM);
	InsertColumn(4, tmpstr, LVCFMT_LEFT, 25, 2);
	CRect Rect;
	GetClientRect(&Rect);
	int cx = Rect.Width();
	SetColumnWidth(0, cx / 5);
	SetColumnWidth(1, cx / 5);
	SetColumnWidth(2, cx / 5);
	SetColumnWidth(3, cx / 5);
	SetColumnWidth(4, cx / 5);
	return 0;
}

void NReportCtrl::Update(const double *Regs)
{
	RedrawWindow();
}


void NReportCtrl::OnSize(UINT nType, int cx, int cy)
{
	MListCtrl::OnSize(nType, cx, cy);

}
void NReportCtrl::AddCollItem(UINT StrID, DWORD_PTR Data, const CString &ProgName, cadrID LocID
	, int StepNum, const CString& CollNote)
{
	CString Str;
	Str.LoadString(StrID);
	AddCollItem(Str, Data, ProgName, LocID, StepNum, CollNote);
}
void NReportCtrl::AddCollItem(UINT StrID, DWORD_PTR Data, const CString &ProgName, cadrID LocID
	, int StepNum, int StartLineNum, int StartStepNum, const CString& CollNote)
{
	int NewInd = GetItemCount();
	CString Str;
	Str.LoadString(StrID);
	InsertItem(NewInd, Str);//, CollImageNum);
	SetItemData(NewInd, Data);
	SetItemText(NewInd, 1, ProgName);
	cadrID ID = static_cast<unsigned int>(Data);
	CString NumStr;
	NumStr.Format("%d.%d : %d", LocID.Prog, StartLineNum + 1, LocID.Cadr + 1);
	SetItemText(NewInd, 2, NumStr);
	NumStr.Format("%d.%d : %d", ID.Prog, StartStepNum, StepNum);
	SetItemText(NewInd, 3, NumStr);
	SetItemText(NewInd, 4, CollNote);

//	Sort(3, TRUE);

	if(!GetFirstSelectedItemPosition())
		EnsureVisible(NewInd, FALSE); // Scroll to the last position only if no items are selected
}

void NReportCtrl::AddCollItem(const CString& Str, DWORD_PTR Data, const CString& ProgName, cadrID LocID
	, int StepNum, const CString& CollNote)
{
	int NewInd = GetItemCount();
	InsertItem(NewInd, Str);//, CollImageNum);
	SetItemData(NewInd, Data);
	SetItemText(NewInd, 1, ProgName);
	const cadrID ID = static_cast<unsigned int>(Data);
	CString NumStr;
	NumStr.Format("%d.%d", LocID.Prog, LocID.Cadr + 1);
	SetItemText(NewInd, 2, NumStr);
	NumStr.Format("%d.%d", ID.Prog, StepNum);
	SetItemText(NewInd, 3, NumStr);
	SetItemText(NewInd, 4, CollNote);

//	Sort(3, TRUE);

	if (!GetFirstSelectedItemPosition())
		EnsureVisible(NewInd, FALSE); // Scroll to the last position only if no items are selected
}

void NReportCtrl::AddErrorItem(const CString& Str, DWORD_PTR Data, cadrID LocID, const CString& ProgName, const CString& Prim)
{
	cadrID ID;
	ID.ID = static_cast<unsigned int>(Data);
	int NewInd = GetItemCount();
	InsertItem(NewInd, Str);//, CollImageNum);
	SetItemData(NewInd, Data);
	SetItemText(NewInd, 1, ProgName);
	CString NumStr;
	NumStr.Format("%d.%d", LocID.Prog, LocID.Cadr + 1);
	SetItemText(NewInd, 2, NumStr);
	NumStr.Format("%d.%d", ID.Prog, ID.Cadr);
	SetItemText(NewInd, 3, NumStr);
	SetItemText(NewInd, 4, Prim);


	if (!GetFirstSelectedItemPosition())
		EnsureVisible(NewInd, FALSE); // Scroll to the last position only if no items are selected
}

void NReportCtrl::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	GetParent()->SendMessage(WM_LBUTTONDBLCLK);
}

BOOL NReportCtrl::OnListItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;
	NM_LISTVIEW* pState = (NM_LISTVIEW*)pNMHDR;
	if (pState->uChanged & LVIF_STATE && pState->uNewState & LVIS_SELECTED)
		GetParent()->SendMessage(WM_APP_REP_ITEM_CHANGE);
	return FALSE;
}

DWORD_PTR NReportCtrl::GetDataFromSelectedItem() const
{
	POSITION pos = GetFirstSelectedItemPosition();
	if(!pos)
		return 0;
	int NItem = GetNextSelectedItem(pos);
	return GetItemData(NItem);
}

CString NReportCtrl::GetTextFromSelectedItem(int FieldInd) const
{
	POSITION pos = GetFirstSelectedItemPosition();
	if (!pos)
		return 0;
	int NItem = GetNextSelectedItem(pos);
	return GetItemText(NItem, FieldInd);
}

COLORREF NReportCtrl::OnGetCellTextColor(int nRow, int nColumn)
{
	const CString Text = GetItemText(nRow, 0);
	if (Text.GetLength() < 1)
		return MListCtrl::OnGetCellTextColor(nRow, nColumn);
	switch (atoi(Text.Left(1)))
	{
	case 0:
		return MListCtrl::OnGetCellTextColor(nRow, nColumn);
	case 1:
		return RGB(192, 0, 0);
	case 2:
		return RGB(0, 192, 0);
	case 3:
		return RGB(0, 0, 192);
	default:
		return MListCtrl::OnGetCellTextColor(nRow, nColumn);
	}
	return MListCtrl::OnGetCellTextColor(nRow, nColumn);
}
void NReportCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	auto POS = GetFirstSelectedItemPosition();

	CMenu menu;
	UINT idMenu = IDR_REPORT_CONT;
	VERIFY(menu.LoadMenu(idMenu));
	auto pSub = menu.GetSubMenu(0);
	if (POS == NULL)
		pSub->RemoveMenu(ID_REPORT_RUNTO, MF_BYCOMMAND);
	((CWinAppEx *)AfxGetApp())->GetContextMenuManager()->ShowPopupMenu(pSub->GetSafeHmenu(), point.x, point.y, this, TRUE);
	menu.DestroyMenu();
}

void NReportCtrl::OnReportRunto()
{
	GetParent()->SendMessage(WM_APP_REPORT_RUNTO);
}

void NReportCtrl::OnReportClear()
{
	GetParent()->SendMessage(WM_APP_REPORT_CLEAR);
}

int NReportCtrl::OnCompareItems(LPARAM lParam1, LPARAM lParam2, int iColumn)
{
	if (lParam1 > lParam2)
		return -1;
	else if (lParam1 < lParam2)
		return 1;
	else
		return 0;
}

void NReportCtrl::Sort(int iColumn, BOOL bAscending, BOOL bAdd)
{
	GetHeaderCtrl().SetSortColumn(iColumn, bAscending, bAdd);
	m_iSortedColumn = iColumn;
	m_bAscending = bAscending;
	SortItemsEx(CompareFunc, (LPARAM)this);
}

int NReportCtrl::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	NReportCtrl* pCtrl = (NReportCtrl*)lParamSort;
	CString    strItem1 = pCtrl->GetItemText(static_cast<int>(lParam1), pCtrl->m_iSortedColumn);
	CString    strItem2 = pCtrl->GetItemText(static_cast<int>(lParam2), pCtrl->m_iSortedColumn);
	switch (pCtrl->m_iSortedColumn)
	{
	case 0:
	{
		int num1 = atoi(strItem1.SpanExcluding(":"));
		int num2 = atoi(strItem2.SpanExcluding(":"));
		return (num1 > num2 ? 1 : (num1 == num2 ? 0 : -1))  * (pCtrl->m_bAscending ? 1 : -1);
	}
	case 1:
	{
		return strItem1.Compare(strItem2) * (pCtrl->m_bAscending ? 1 : -1);
	}
	case 2:
	case 3:
	{
		int ind1 = strItem1.Find('.');
		if (ind1 == -1)
			ind1 = strItem1.GetLength();
		const int	num11 = atoi(strItem1.Left(ind1));
		int num12 = atoi(strItem1.Right(strItem1.GetLength() - ind1 - 1));
		int ind2 = strItem2.Find('.');
		if (ind2 == -1)
			ind2 = strItem2.GetLength();
		const int	num21 = atoi(strItem2.Left(ind2));
		const int num22 = atoi(strItem2.Right(strItem2.GetLength() - ind2 - 1));
		const int res1 = (num11 > num21 ? 1 : (num11 == num21 ? 0 : -1)) * (pCtrl->m_bAscending ? 1 : -1);
		return res1 != 0 ? res1 : (num12 > num22 ? 1 : (num12 == num22 ? 0 : -1)) * (pCtrl->m_bAscending ? 1 : -1);
	}
	}
	return strItem1.Compare(strItem2) * (pCtrl->m_bAscending ? 1 : -1);
}

void NReportCtrl::DeleteMessages(const std::set<int>& Inds)
{
	SetRedraw(FALSE);
	for (int i = GetItemCount(); i >= 0; --i)
	{
		int Num = atoi(GetItemText(i, 0).SpanExcluding(":"));
		if (Inds.find(Num) != Inds.end())
			DeleteItem(i);
	}
	SetRedraw(TRUE);
}

void NReportCtrl::SortBySettings()
{
	SortItemsEx(CompareFunc, (LPARAM)this);
}

