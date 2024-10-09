// NObjTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ResLocal.h"
#include "ResInv.h"
#include "NCMDoc.h"
#include "NCM.h"
#include "NCUnit.h"
#include "LeftView.h"
#include "MTPart.h"
#include "DocElements.h"
#include "MTConfig.h"
#include "NObjTreeCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// NObjTreeCtrl

IMPLEMENT_DYNAMIC(NObjTreeCtrl, MTreeCtrl)

BEGIN_MESSAGE_MAP(NObjTreeCtrl, MTreeCtrl)
	ON_WM_MOUSEMOVE()
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, &NObjTreeCtrl::OnTvnBegindrag)
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_NOTIFY_REFLECT(NM_CLICK, &NObjTreeCtrl::OnNMClick)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &NObjTreeCtrl::OnNMDblclk)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &NObjTreeCtrl::OnTvnSelchanged)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &NObjTreeCtrl::OnNMCustomdraw)
	ON_WM_MOUSELEAVE()
	ON_WM_KILLFOCUS()
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, &NObjTreeCtrl::OnTvnItemexpanded)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_HSCROLL()
	ON_WM_KEYDOWN()
	ON_NOTIFY(NM_CUSTOMDRAW, ID_VIEW_VIEW, &NObjTreeCtrl::OnNMCustomdrawTB)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

//      Handles TTN_NEEDTEXT message to display tooltips for the toolbar.
//      This code is based on CFrameWnd::OnToolTipText
BOOL NObjTreeCtrl::OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult)
{
	ASSERT(pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW);

	// allow top level routing frame to handle the message
	if (GetRoutingFrame() != NULL)
		return FALSE;

	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	TCHAR szFullText[256];
	CString strTipText;
	UINT_PTR nID = (UINT_PTR)pNMHDR->idFrom;
	if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
		pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
	{
		// idFrom is actually the HWND of the tool
		nID = ((UINT_PTR)(WORD)::GetDlgCtrlID((HWND)nID));
	}

	if (nID != 0) // will be zero on a separator
	{
		AfxLoadString((UINT)nID, szFullText);
		// this is the command id, not the button index
		AfxExtractSubString(strTipText, szFullText, 1, '\n');
	}
#ifndef _UNICODE
	if (pNMHDR->code == TTN_NEEDTEXTA)
		_tcsncpy_s(pTTTA->szText, (sizeof(pTTTA->szText) / sizeof(pTTTA->szText[0])),
			strTipText, _TRUNCATE);
	else
	{
		int n = MultiByteToWideChar(CP_ACP, 0, strTipText, -1, pTTTW->szText,
			sizeof(pTTTW->szText) / sizeof(pTTTW->szText[0]));
		if (n > 0)
			pTTTW->szText[n - 1] = 0;
	}
#else
	if (pNMHDR->code == TTN_NEEDTEXTA)
	{
		int n = WideCharToMultiByte(CP_ACP, 0, strTipText, -1,
			pTTTA->szText, sizeof(pTTTA->szText) / sizeof(pTTTA->szText[0]),
			NULL, NULL);
		if (n > 0)
			pTTTA->szText[n - 1] = 0;
	}
	else
		_tcsncpy_s(pTTTW->szText, (sizeof(pTTTW->szText) / sizeof(pTTTW->szText[0])),
			strTipText, _TRUNCATE);
#endif
	*pResult = 0;

	// bring the tooltip window above other popup windows
	::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0,
		SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOMOVE);

	return TRUE;    // message was handled
}

NObjTreeCtrl::NObjTreeCtrl(CLeftView *pLeftView) : ObjDropTarget(pLeftView)
{
	DragIL = NULL;
	SourceItem = NULL;
	TargetItem = NULL;
	NProgramItem = NULL;
	NProgramItemAux = NULL;
	NToolItem = NULL;
	NExObjItem = NULL;
	NPartItem = NULL;
	NStockItem = NULL;
	NStocksItem = NULL;
	NCUnitItem = NULL;
	NMachineItem = NULL;
}

NObjTreeCtrl::~NObjTreeCtrl()
{
}

BOOL NObjTreeCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = MTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != NULL);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != NULL)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	return bRes;
}

void NObjTreeCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.hwndTrack = m_hWnd;
	tme.dwFlags = TME_LEAVE;
	BOOL res = TrackMouseEvent(&tme);

	MousePos = point;
	if(nFlags & MK_LBUTTON) 
		if(DragIL != NULL)
		{
			CDC* pDC = GetDC();
			HTREEITEM htiTarget;  // handle to target item 
       // Drag the item to the current position of the mouse pointer. 
			CPoint p = point;
			ClientToScreen(&p);
			DragIL->DragMove(p);
			// Find out if the pointer is on the item. If it is, 
			// highlight the item as a drop target. 
			TargetItem = NULL;
			if ((htiTarget = HitTest(point)) != NULL)
			{ 
				HTREEITEM SourceParentItem = GetParentItem(SourceItem);
				if (SourceParentItem)
				{
					if (GetItemData(SourceParentItem) == NProgramList)
					{
						HTREEITEM ParentItem = GetParentItem(htiTarget);
						if (ParentItem)
							if (GetItemData(ParentItem) == NProgramList)
							{
								SetInsertMark(htiTarget, FALSE);
								TargetItem = htiTarget;
							}
					}
					else if (GetItemData(htiTarget) == NExObjList)
					{
						TargetItem = htiTarget;
					}
					else if (GetItemData(htiTarget) == NPartList)
					{
						TargetItem = htiTarget;
					}
					else if (GetItemData(htiTarget) == NStocksList || GetItemData(htiTarget) == NStock)
					{
						TargetItem = htiTarget;
					}
				}
				CImageList::DragShowNolock(FALSE);
				SelectDropTarget(TargetItem);
				CImageList::DragShowNolock(TRUE);
			}
		}

	MTreeCtrl::OnMouseMove(nFlags, point);
}

void NObjTreeCtrl::OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	SelectItem(NULL);// To clear PropPane
	if (!((CLeftView*)GetParent())->TvnBegindrag())
		return;
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	unsigned int flags;
	SourceItem = NULL;
	TargetItem = NULL;
	HTREEITEM CurItem = HitTest(pNMTreeView->ptDrag,&flags);
	HTREEITEM ParentItem = GetParentItem(CurItem);
	if(ParentItem != NULL) // Item has parent
	{
		switch (GetItemData(ParentItem))
		{
		case NProgramList:
		{
			UINT StateMask = GetItemState(ParentItem, TVIS_BOLD);
			if(!(StateMask & TVIS_BOLD)) // Don't drag if errors
			{
				DragIL = CreateDragImage(CurItem);
				SourceItem = CurItem;
				SetCapture(); 
			}
			break;
		}
		case NExObjList:
			DragIL = CreateDragImage(CurItem);
			DragIL->BeginDrag(0, CPoint(-15, -15));
			POINT pt = pNMTreeView->ptDrag;
			ClientToScreen(&pt);
			DragIL->DragEnter(NULL, pt);
			SourceItem = CurItem;
			SetCapture();
			break;
		default:
			break;
		}
	}

	
	*pResult = 0;
}

void NObjTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(DragIL != NULL)
	{
		delete DragIL;
		SetInsertMark(NULL);
		CImageList::DragLeave(this);
		CImageList::EndDrag();
		SelectDropTarget(NULL);
		ReleaseCapture();
		DragIL = NULL;
		if (TargetItem == NULL || SourceItem == NULL)
		{
			TargetItem = NULL;
			SourceItem = NULL;
			return;
		}
		switch (GetItemData(TargetItem))
		{
		case NPartList:
			((CLeftView*)GetParent())->MoveExObj(SourceItem, TargetItem);
			break;
		case NExObjList:
			break;
		case NStocksList:
		case NStock:
			((CLeftView*)GetParent())->MoveExObj(SourceItem, TargetItem);
			break;
		default:
			((CLeftView*)GetParent())->MoveItem(SourceItem, TargetItem);

		}
		Expand(TargetItem, TVE_EXPAND);
		SourceItem = NULL;
		TargetItem = NULL;
	}

	MTreeCtrl::OnLButtonUp(nFlags, point);
}

void NObjTreeCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	MousePos = point;
	HTREEITEM CurItem = HitTest(point);
	Select(CurItem,TVGN_CARET);

	MTreeCtrl::OnRButtonDown(nFlags, point);
}

void NObjTreeCtrl::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	((CLeftView *)GetParent())->ProcClick(MousePos);
	*pResult = 0;
}

void NObjTreeCtrl::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	HTREEITEM CurItem = GetSelectedItem();
	if(CurItem == NULL) return;
	CRect rect;
	HTREEITEM ParentItem = GetParentItem(CurItem);
	if(ParentItem != NULL) // Item has parent
	{
		switch (GetItemData(ParentItem))
		{
		case NProgramList:
		case NProgramListAux:
			GetParent()->SendMessage(WM_COMMAND, ID_CH_PROGR_TEXT);
			break;
		case NToolList:
			break;
		default:
			break;
		}
	}
	else // Item has no parent
	{
	}
	*pResult = 0;
}

void NObjTreeCtrl::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	HTREEITEM NewItem = pNMTreeView->itemNew.hItem;
	HTREEITEM OldItem = pNMTreeView->itemOld.hItem;
	HTREEITEM ParentItemNew = GetParentItem(NewItem);
	HTREEITEM ParentItemOld = GetParentItem(OldItem);
	bool ExObjListChanged = false;
	bool PartListChanged = false;
	bool StockListChanged = false;
	bool ToolListChanged = false;
	bool MachineChanged = false;
	if(ParentItemNew)
	{
		switch (GetItemData(ParentItemNew))
		{
		case NExObjList:
			ExObjListChanged = true;
			break;
		case NPartList:
			PartListChanged = true;
			break;
		case NStocksList:
			StockListChanged = true;
			break;
		case NToolList:
			ToolListChanged = true;
			break;
		case NMachine:
			MachineChanged = true;
			break;
		}
	}
	if(NewItem != NULL)
		if (GetItemData(NewItem) == NStock)
			StockListChanged = true;

	if(ParentItemOld)
	{
		switch (GetItemData(ParentItemOld))
		{
		case NExObjList:
			ExObjListChanged = true;
			break;
		case NPartList:
			PartListChanged = true;
			break;
		case NStocksList:
			StockListChanged = true;
			break;
		case NToolList:
			ToolListChanged = true;
			break;
		case NMachine:
			MachineChanged = true;
			break;
		}
	}
	if (OldItem != NULL)
		if (GetItemData(OldItem) == NStock)
			StockListChanged = true;
	((CLeftView *)GetParent())->SetSelection(ExObjListChanged, PartListChanged, StockListChanged, ToolListChanged, MachineChanged, NewItem ,ParentItemNew);
	ProcessItemPosChanged();
	*pResult = 0;
}


int NObjTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (MTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetWindowTheme(GetSafeHwnd(), L"Explorer", NULL);// all colors (and some other things) from windows explorer

	if (!ContextToolBar.CreateEx(this, TBSTYLE_WRAPABLE, WS_CHILD | CBRS_FLOATING, CRect(0, 0, 0, 0), ID_VIEW_VIEW))
	{
		TRACE0("Failed to create left view toolbar\n");
		return -1;      // fail to create
	}
	ContextToolBar.SetBarStyle(ContextToolBar.GetBarStyle() & ~CBRS_HIDE_INPLACE);
	ContextToolBar.SetBarStyle(ContextToolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_BORDER_ANY);

	for (int i = 0; i < ItemTypesNum; ++i)
	{
		if (!ItemToolbars[i].CreateEx(this, TBSTYLE_WRAPABLE, WS_CHILD | CBRS_FLOATING, CRect(0, 0, 0, 0), ID_VIEW_VIEW))
		{
			TRACE0("Failed to create left view toolbar\n");
			return -1;      // fail to create
		}
		ItemToolbars[i].SetBarStyle(ItemToolbars[i].GetBarStyle() & ~CBRS_HIDE_INPLACE);
		ItemToolbars[i].SetBarStyle(ItemToolbars[i].GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
//		ItemToolbars[i].ShowWindow(SW_SHOW);
	}

	for (size_t i = 0; i < SubItemToolBars.size(); ++i)
	{
		SubItemToolBars[i].CreateEx(this, TBSTYLE_WRAPABLE, WS_CHILD | CBRS_FLOATING, CRect(0, 0, 0, 0), ID_VIEW_VIEW);
		SubItemToolBars[i].SetBarStyle(SubItemToolBars[i].GetBarStyle() & ~CBRS_HIDE_INPLACE);
		SubItemToolBars[i].SetBarStyle(SubItemToolBars[i].GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
		SubItemToolBars[i].GetToolBarCtrl().SetImageList(nullptr);
	}

	// Register Tree control as a drop target	
	HRESULT hr = OleInitialize( 0 );
	ObjDropTarget.Register(this);

	return 0;
}

void NObjTreeCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    NMTVCUSTOMDRAW *pcd = (NMTVCUSTOMDRAW   *)pNMHDR;
	switch (pNMCD->dwDrawStage)
    {
    case CDDS_PREPAINT: 
        *pResult = CDRF_NOTIFYITEMDRAW;     
        break;

    case CDDS_ITEMPREPAINT : 
        {
			HTREEITEM hItem = (HTREEITEM)pNMCD->dwItemSpec;

			if (GetItemState(hItem, TVIS_CUT) & TVIS_CUT)
            {
				pcd->clrText = GetSysColor(COLOR_GRAYTEXT);
            }

            *pResult = CDRF_DODEFAULT;// do not set *pResult = CDRF_SKIPDEFAULT
            break;
        }
    }
//	*pResult = 0;
}

void NObjTreeCtrl::OnNMCustomdrawTB(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	NMTVCUSTOMDRAW *pcd = (NMTVCUSTOMDRAW   *)pNMHDR;
	switch (pNMCD->dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;

	case CDDS_ITEMPREPAINT:
	{

		*pResult = CDRF_DODEFAULT;
		break;
	}
	}
	//	*pResult = 0;
}

void NObjTreeCtrl::OnMouseLeave()
{
	if (IsWindow(ContextToolBar))
	{
		if (ContextToolBar.IsWindowVisible())
		{
			CPoint point;
			GetCursorPos(&point);
			CRect rect;
			ContextToolBar.GetWindowRect(rect);
			if (!rect.PtInRect(point))
				CloseContextToolBar();
		}
	}
	MTreeCtrl::OnMouseLeave();
}


void NObjTreeCtrl::OnKillFocus(CWnd* pNewWnd)
{
	MTreeCtrl::OnKillFocus(pNewWnd);

//	CloseContextToolBar();
}

BOOL NObjTreeCtrl::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
		CloseContextToolBar();

	return MTreeCtrl::PreTranslateMessage(pMsg);
}


void NObjTreeCtrl::OnTvnItemexpanded(NMHDR *pNMHDR, LRESULT *pResult)
{
	ProcessItemPosChanged();
	*pResult = 0;
}


BOOL NObjTreeCtrl::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	BOOL Res = GetParent()->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);// For toolbar commands
	
	if (Res)
		return TRUE;

	return MTreeCtrl::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void NObjTreeCtrl::CloseContextToolBar()
{
	ContextToolBar.ShowWindow(SW_HIDE);
}

void NObjTreeCtrl::ShowContextToolbar(const CPoint &point)
{
	//Show context toolbar
	CloseContextToolBar();
	int MenuID = ((CLeftView *)GetParent())->CalcContextMenuID();
	if (MenuID < 0)
		return;
	ContextToolBar.MoveWindow(CRect(0, 0, 0, 0), FALSE);
	LoadTBFromIcons(MenuID);
	auto ButtonCount = ContextToolBar.GetToolBarCtrl().GetButtonCount();
	if (ButtonCount == 0)
		return;
	int RowsNum = ButtonCount / 6 + 1;
	CRect rect;
	ContextToolBar.GetToolBarCtrl().SetRows(RowsNum, TRUE, &rect);
	CRect borders = CRect(2, 2, 2, 2);
	const int VerticalShift = 5;
	const int HorizontalShift = 2;
	ContextToolBar.GetWindowRect(rect);
	rect.bottom = rect.Height() + borders.top + borders.bottom;
	rect.right = rect.Width() + borders.left + borders.right;
	rect.left = 0;
	rect.top = 0;
	rect += point - CPoint(0, rect.Height() + VerticalShift);
	CRect t_rect;
	CPoint move(0, 0);
	GetClientRect(t_rect);
	move.x = max(0, rect.right - t_rect.right + HorizontalShift);
	move.y = max(0, rect.bottom - t_rect.bottom);
	rect -= move;
	move.x = min(0, rect.left - t_rect.left + HorizontalShift);
	move.y = 0;
	if (rect.top < t_rect.top)
		move.y = -(rect.Height() + 2 * VerticalShift);
	rect -= move;
	ContextToolBar.MoveWindow(rect);
	ContextToolBar.ShowWindow(SW_SHOWNORMAL);
}

void NObjTreeCtrl::LoadTBFromIcons(UINT MenuID)
{
	CImageList *pList = ContextToolBar.GetToolBarCtrl().SetImageList(nullptr);
	if (pList != nullptr)
	{
		pList->DeleteImageList();
		delete pList;
	}
	CNCMDoc *pDoc = ((CLeftView *)GetParent())->GetDocument();
	bool NeedCleanTB = true;
	switch (MenuID)
	{
	case IDR_PA_PROGR:
	{
		if (pDoc->IsThereAnyProgr())
		{
			UINT IDs0[] =
			{ ID_PA_PROGR_CR, ID_PA_PROG_RELO, ID_PA_PROG_DELAllPROG, ID_PA_PROG_ALLVIS
				, ID_PA_PROG_ALLUNV, ID_PA_PROG_INV, ID_LOAD_AUX, ID_PROPERTIES_ED };
			UINT IDsIcon[] =
			{ IDI_TB_NEW, IDI_CM_UPDATE, IDI_CM_DELETE, IDI_CM_VISIBLE_ON, IDI_CM_VISIBLE_OFF, IDI_CM_VISIBLE_TOGGLE, IDI_CM_LOAD, IDI_CM_PROPERTIES };
			Make1TB(ContextToolBar, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]), false);
			NeedCleanTB = false;
		}
		break;
	}
	case IDR_CH_PROGR:
	{
		UINT IDs0[] =
		{ ID_CH_PROGR_RELO, ID_CH_PROGR_TEXT, ID_DELETE, ID_CH_PROGR_DXFExport, ID_CH_PROGR_APTexport, ID_CH_PROGR_StContExport, ID_PROPERTIES_ED };
		UINT IDsIcon[] =
		{ IDI_CM_UPDATE, IDI_MACH_EDIT, IDI_CM_DELETE, IDI_CM_IMPORT_DXF, IDI_CM_IMPORT_APT, IDI_CM_IMPORT_STC, IDI_CM_PROPERTIES };
		Make1TB(ContextToolBar, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]), false);
		NeedCleanTB = false;
		break;
	}
	//case IDR_PA_UNIT:
	//{
	//	UINT IDs0[] =
	//	{ ID_NCUNIT_LOAD, ID_NCUNIT_START, ID_PROPERTIES_ED };
	//	UINT IDsIcon[] =
	//	{ IDI_TB_ZOOM, IDI_TB_ZOOMD, IDI_CM_POS };
	//	Make1TB(ContextToolBar, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]), false);
	//	break;
	//}
	case IDR_PA_MACH:
	{
		UINT IDs0[] =
		{ ID_PA_MACH_EDIT, ID_PA_MACH_TOOL_VISIBLE };
		UINT IDsIcon[] =
		{ IDI_MACH_EDIT, IDI_TB_QUICK };
		Make1TB(ContextToolBar, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]), false);
		NeedCleanTB = false;
		break;
	}
	case IDR_PA_MACH_EDIT:
	{
		UINT IDs0[] =
		{ ID_PA_MACH_EDIT_END };
		UINT IDsIcon[] =
		{ IDI_CM_DELETE };
		Make1TB(ContextToolBar, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]), false);
		NeedCleanTB = false;
		break;
	}
	//case IDR_PA_PROGR_AUX:
	//{
	//	break;
	//}
	//case IDR_PA_PROGR_WELD:
	//{
	//	break;
	//}
	case IDR_PA_TOOLS:
	case IDR_PA_TOOLS_MILLTURN:
	case IDR_PA_TOOLS_TURN:
	{
		if (pDoc->IsThereAnyTool())
		{
			UINT IDs0[] =
			{ ID_PA_TOOLS_IMPNCM, ID_PA_TOOLS_DELUNUSED, ID_PA_TOOLS_ALLVIS, ID_PA_TOOLS_ALLUNV, ID_PROPERTIES_ED };
			UINT IDsIcon[] =
			{ IDI_CM_LOAD, IDI_CM_DELETE, IDI_CM_VISIBLE_ON, IDI_CM_VISIBLE_OFF, IDI_CM_PROPERTIES };
			Make1TB(ContextToolBar, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]), false);
			NeedCleanTB = false;
		}
		break;
	}
	case IDR_CH_TOOL:
	{
		if (pDoc->IsThereAnyTool())
		{
			UINT IDs0[] =
			{ ID_CH_TOOL_CUT, ID_CH_TOOL_COPY, ID_CH_TOOL_PASTE, ID_CH_TOOL_DELETE, ID_PROPERTIES_ED };
			UINT IDsIcon[] =
			{ IDI_CM_CUT, IDI_CM_COPY, IDI_CM_PASTE, IDI_CM_DELETE, IDI_CM_PROPERTIES };
			Make1TB(ContextToolBar, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]), false);
			NeedCleanTB = false;
		}
		break;
	}
	//case IDR_CH_TOOL1:
	//{
	//	if (pDoc->IsThereAnyTool())
	//	{
	//		UINT IDs0[] =
	//		{ ID_CH_TOOL_CUT, ID_CH_TOOL_COLOR, ID_CH_TOOL_PASCOLOR, ID_CH_TOOL_WF};
	//		UINT IDsIcon[] =
	//		{ IDI_CM_CUT, IDI_CM_DELETE, IDI_CM_VISIBLE_ON, IDI_CM_VISIBLE_OFF, IDI_CM_PROPERTIES };
	//		Make1TB(ContextToolBar, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]), false);
	//	}
	//	break;
	//}
	case IDR_PA_STOCK_TEMP:
	{
		UINT IDs0[] =
		{ID_PA_STOCK_ALLVIS, ID_PA_STOCK_ALLUNV, ID_PA_STOCK_INV, ID_PROPERTIES_ED};
		UINT IDsIcon[] =
		{ IDI_CM_VISIBLE_ON, IDI_CM_VISIBLE_OFF, IDI_CM_VISIBLE_TOGGLE, IDI_CM_PROPERTIES };
		Make1TB(ContextToolBar, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]), false);
		NeedCleanTB = false;
		break;
	}
	case IDR_PA_STOCK_TURN:
	{
		UINT IDs0[] =
		{
			ID_PA_STOCK_CR_PROFILE,	ID_PA_STOCK_DELETE, 
			ID_PA_STOCK_ALLVIS, ID_PA_STOCK_ALLUNV, ID_PA_STOCK_INV, ID_PROPERTIES_ED
		};
		UINT IDsIcon[] =
		{
			IDI_CM_CONT_EX, IDI_CM_DELETE, 
			IDI_CM_VISIBLE_ON, IDI_CM_VISIBLE_OFF, IDI_CM_VISIBLE_TOGGLE, IDI_CM_PROPERTIES
		};
		Make1TB(ContextToolBar, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]), false);
		NeedCleanTB = false;
		break;
	}

	case IDR_PA_STOCK_MILLTURN:
	{
		UINT IDs0[] =
		{
			ID_PA_STOCK_CR_PIPE, ID_PA_STOCK_CR_PROFILE, ID_PA_STOCK_DELETE, ID_PA_STOCK_TEMP,
			ID_PA_STOCK_ALLVIS, ID_PA_STOCK_ALLUNV, ID_PA_STOCK_INV, ID_PROPERTIES_ED
		};
		UINT IDsIcon[] =
		{
			IDI_CM_BOX, IDI_CM_CONT_EX, IDI_CM_DELETE, IDI_STEMP_ON,
			IDI_CM_VISIBLE_ON, IDI_CM_VISIBLE_OFF, IDI_CM_VISIBLE_TOGGLE, IDI_CM_PROPERTIES
		};
		Make1TB(ContextToolBar, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]), false);
		NeedCleanTB = false;
		break;
	}

	case IDR_PA_STOCK:
	{
		UINT IDs0[] =
		{
			ID_PA_STOCK_CR_CYL, ID_PA_STOCK_CR_PIPE, ID_PA_STOCK_CR_CONTBOX, ID_PA_STOCK_CR_STL,
			ID_PA_STOCK_DELETE, ID_PA_STOCK_POS, ID_PA_STOCK_TEMP,  
			ID_PA_STOCK_ALLVIS, ID_PA_STOCK_ALLUNV, ID_PA_STOCK_INV, ID_PROPERTIES_ED
		};
		UINT IDsIcon[] =
		{ 
			IDI_CM_CYL, IDI_CM_RING, IDI_CM_CONT_EX, IDI_CM_LOAD,
			IDI_CM_DELETE, IDI_CM_POS, IDI_STEMP_ON,
			IDI_CM_VISIBLE_ON, IDI_CM_VISIBLE_OFF, IDI_CM_VISIBLE_TOGGLE, IDI_CM_PROPERTIES 
		};
		Make1TB(ContextToolBar, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]), false);
		NeedCleanTB = false;
		break;
	}

	case IDR_CH_STOCK:
	{
		if (pDoc->IsThereAnyStock())
		{
			UINT IDs0[] =
			{
				ID_CH_STOCK_REPLACE_BOX, ID_CH_STOCK_REPLACE_CYL, ID_CH_STOCK_REPLACE_PIPE,ID_CH_STOCK_REPLACE_CONTBOX, ID_CH_STOCK_REPLACE_STL,
				ID_CH_STOCK_CR_BOX, ID_CH_STOCK_CR_CYL, ID_CH_STOCK_CR_PIPE, ID_CH_STOCK_CR_CONTBOX, ID_CH_STOCK_CR_STL,
				ID_CH_STOCK_POS, ID_CH_STOCK_DELETE, ID_PA_STOCK_TEMP, ID_CH_STOCK_EXSTL, ID_PROPERTIES_ED
			};
			UINT IDsIcon[] =
			{
				IDI_BOX_EDIT, IDI_CYL_EDIT, IDI_CM_RING_EDIT, IDI_CONT_EX_EDIT, IDI_LOAD_EDIT,
				IDI_CM_BOX, IDI_CM_CYL, IDI_CM_RING, IDI_CM_CONT_EX, IDI_CM_LOAD,
				IDI_CM_POS, IDI_CM_DELETE, IDI_STEMP_ON, IDI_TB_SAVE, IDI_CM_PROPERTIES
			};
			Make1TB(ContextToolBar, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]), false);
			NeedCleanTB = false;
		}
		else
		{
			UINT IDs0[] =
			{ ID_CH_STOCK_CR_PIPE, ID_CH_STOCK_CR_CONTBOX, ID_PROPERTIES_ED };
			UINT IDsIcon[] =
			{ IDI_CM_RING, IDI_CM_CONT_EX, IDI_CM_PROPERTIES };
			Make1TB(ContextToolBar, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]), false);
			NeedCleanTB = false;
		}
		break;
	}
	case IDR_CH_STOCK_TURN:
	{
		if (pDoc->IsThereAnyStock())
		{
			UINT IDs0[] =
			{
				ID_CH_STOCK_REPLACE_CONE, ID_CH_STOCK_REPLACE_PROFILE, ID_CH_STOCK_DELETE, ID_CH_STOCK_EXSTL,
				ID_CH_STOCK_CR_CONE, ID_CH_STOCK_CR_PROFILE, ID_PROPERTIES_ED
			};
			UINT IDsIcon[] =
			{
				IDI_CYL_EDIT, IDI_CONT_EX_EDIT, IDI_CM_DELETE, IDI_TB_SAVE, 
				IDI_CM_CYL, IDI_CM_CONT_EX, IDI_CM_PROPERTIES		 
			};
			Make1TB(ContextToolBar, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]), false);
			NeedCleanTB = false;
		}
		break;
	}

	case IDR_CH_STOCK_MILLTURN:
	{
		if (pDoc->IsThereAnyStock())
		{
			UINT IDs0[] =
			{
				ID_CH_STOCK_REPLACE_CONE, ID_CH_STOCK_REPLACE_PIPE, ID_CH_STOCK_REPLACE_PROFILE, ID_CH_STOCK_DELETE, ID_PA_STOCK_TEMP, 
				ID_CH_STOCK_CR_CONE, ID_CH_STOCK_CR_PIPE, ID_CH_STOCK_CR_PROFILE, ID_CH_STOCK_EXSTL, ID_PROPERTIES_ED
			};
			UINT IDsIcon[] =
			{
				IDI_CYL_EDIT, IDI_BOX_EDIT, IDI_CONT_EX_EDIT, IDI_CM_DELETE, IDI_STEMP_ON,
				IDI_CM_CYL, IDI_CM_BOX, IDI_CM_CONT_EX, IDI_TB_SAVE, IDI_CM_PROPERTIES				 
			};
			Make1TB(ContextToolBar, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]), false);
			NeedCleanTB = false;
		}
		break;
	}
	case IDR_PA_PART:
	{
		if (!pDoc->NGetPartList()->empty())
		{
			UINT IDs0[] =
			{ ID_PA_PART_DELETE, ID_PA_PART_ALLVIS, ID_PA_PART_ALLUNV, ID_PA_PART_INV, ID_PROPERTIES_ED };
			UINT IDsIcon[] =
			{ IDI_CM_DELETE, IDI_CM_VISIBLE_ON, IDI_CM_VISIBLE_OFF, IDI_CM_VISIBLE_TOGGLE, IDI_CM_PROPERTIES };
			Make1TB(ContextToolBar, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]), false);
			NeedCleanTB = false;
		}
		break;
	}
	case IDR_PA_PART_TURN:
	case IDR_PA_PART_MILLTURN:
	{
		if (!pDoc->NGetPartList()->empty())
		{
		UINT IDs0[] =
		{ ID_PA_PART_CR_PROFILE, ID_PA_PART_DELETE, ID_PA_PART_ALLVIS, ID_PA_PART_ALLUNV, ID_PA_PART_INV, ID_PROPERTIES_ED };
		UINT IDsIcon[] =
		{ IDI_CM_CONT_EX, IDI_CM_DELETE, IDI_CM_VISIBLE_ON, IDI_CM_VISIBLE_OFF, IDI_CM_VISIBLE_TOGGLE, IDI_CM_PROPERTIES };
		Make1TB(ContextToolBar, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]), false);
		NeedCleanTB = false;
		}
		break;
	}
	case IDR_CH_PART:
	{
		UINT IDs0[] =
		{ ID_CH_PART_REPLACE, ID_CH_PART_DELETE, ID_CH_PART_EXPORTSTL, ID_PROPERTIES_ED };
		UINT IDsIcon[] =
		{ IDI_CM_REPLACE, IDI_CM_DELETE, IDI_TB_SAVE, IDI_CM_PROPERTIES };
		Make1TB(ContextToolBar, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]), false);
		NeedCleanTB = false;
		break;
	}
	case IDR_PA_EXOBJ:
	{
		if (!pDoc->NGetExObjList()->empty())
		{
			UINT IDs0[] =
			{ ID_PA_EXOBJ_DELETE, ID_PA_EXOBJ_ALLVIS, ID_PA_EXOBJ_ALLUNV, ID_PA_EXOBJ_INV, ID_PROPERTIES_ED };
			UINT IDsIcon[] =
			{ IDI_CM_DELETE, IDI_CM_VISIBLE_ON, IDI_CM_VISIBLE_OFF, IDI_CM_VISIBLE_TOGGLE, IDI_CM_PROPERTIES };
			Make1TB(ContextToolBar, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]), false);
			NeedCleanTB = false;
		}
		break;
	}
	case IDR_CH_EXOBJ:
	{
		UINT IDs0[] =
		{ ID_CH_EXOBJ_REPLACE, ID_CH_EXOBJ_DELETE, ID_CH_EXOBJ_DISASSAMBLE, ID_CH_EXOBJ_SENDTOSTOCK, ID_CH_EXOBJ_SENDTOPART, ID_CH_EXOBJ_EXPORTSTL, ID_PROPERTIES_ED };
		UINT IDsIcon[] =
		{ IDI_CM_REPLACE, IDI_CM_DELETE, IDI_CM_DISASSAMBLE, IDI_CM_SENDTOSTOCK, IDI_CM_SENDTOPART, IDI_TB_SAVE, IDI_CM_PROPERTIES };
		Make1TB(ContextToolBar, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]), false);
		NeedCleanTB = false;
		break;
	}
	//case IDR_PA_TOOLS_TURN:
	//{
	//	break;
	//}
	//case IDR_PA_TOOLS_MILLTURN:
	//{
	//	break;
	//}
	//case IDR_PA_TOOLS_WELD:
	//{
	//	break;
	//}
	//case IDR_CH_STOCK_TEMP:
	//{
	//	break;
	//}
	//case IDR_CH_STOCK:
	//{
	//	break;
	//}
	//case IDR_CH_STOCK_MILLTURN:
	//{
	//	break;
	//}
	//case IDR_CH_STOCK_TURN:
	//{
	//	break;
	//}
	//case IDR_PA_PART:
	//{
	//	break;
	//}
	//case IDR_PA_PART_TURN:
	//{
	//	break;
	//}
	//case IDR_PA_PART_MILLTURN:
	//{
	//	break;
	//}
	//case IDR_PA_STOCK_TEMP:
	//{
	//	break;
	//}
	//case IDR_PA_STOCK:
	//{
	//	break;
	//}
	//case IDR_PA_STOCK_MILLTURN:
	//{
	//	break;
	//}
	//case IDR_PA_EXOBJ:
	//{
	//	break;
	//}
	default:
		break;

	}
	if (NeedCleanTB)
	{
		// delete all existing buttons
		for (int i = ContextToolBar.GetToolBarCtrl().GetButtonCount() - 1; i >= 0; --i)
			ContextToolBar.GetToolBarCtrl().DeleteButton(i);
	}
	int BSize = 16;
	ContextToolBar.SetSizes(CSize(BSize + 7, BSize + 6), CSize(BSize, BSize));
}

void NObjTreeCtrl::ClearTB()
{
	for (int i = 0; i < ItemTypesNum; ++i)
	{
		auto pB = ItemToolbars[i].GetToolBarCtrl().SetImageList(nullptr);
		if(pB != nullptr)
		{
			pB->DeleteImageList();
			delete pB;
		}
	}

	for (size_t i = 0; i < SubItemToolBars.size(); ++i)
	{
		CToolBar &TB = SubItemToolBars[i];
		if (!IsWindow(TB.GetSafeHwnd()))
			continue;
		CToolBarCtrl &TBC = TB.GetToolBarCtrl();
		auto pB = TBC.SetImageList(nullptr);
		if (pB != nullptr)
		{
			pB->DeleteImageList();
			delete pB;
		}
	}

}

void NObjTreeCtrl::ProcessSibling(HTREEITEM Parent, size_t *pGlobalInd)
{
	for (HTREEITEM item = GetChildItem(Parent); item != NULL; item = GetNextSiblingItem(item))
	{
		if (*pGlobalInd >= SubItemToolBars.size())
			break;
		if (GetItemState(Parent, TVIS_EXPANDED) & TVIS_EXPANDED)
			SetItemLeftTB(item, SubItemToolBars[*pGlobalInd]);
		else
			SubItemToolBars[*pGlobalInd].ShowWindow(SW_HIDE);
		++(*pGlobalInd);
		ProcessSibling(item, pGlobalInd); // Recursive call
	}
}

void NObjTreeCtrl::ProcImportMode(bool IsInImportMode)
{
	HTREEITEM item;
	for (item = GetChildItem(NToolItem);
	item != NULL;
		item = GetNextSiblingItem(item))
	{
		int CheckTool = IsInImportMode ? INDEXTOSTATEIMAGEMASK(TOOL_UNCHECKED) : INDEXTOSTATEIMAGEMASK(0);
		if (CheckTool != 0)
		{// We are in the import mode
			int CurCheckTool = GetItemState(item, TVIS_STATEIMAGEMASK) & TVIS_STATEIMAGEMASK;
			if (CurCheckTool != 0)
			{// It is not first update in the import mode
				CheckTool = CurCheckTool;
			}
		}
		SetItemState(item, INDEXTOOVERLAYMASK(0) + CheckTool, TVIS_OVERLAYMASK | TVIS_STATEIMAGEMASK);
	}
	if (IsInImportMode)
		Expand(NToolItem, TVE_EXPAND);
}

void NObjTreeCtrl::ProcessItemPosChanged()
{
	if (NCUnitItem == NULL)
		return;

	size_t GlobalInd = 0;
	SetItemLeftTB(NCUnitItem, ItemToolbars[0]);

	if (NMachineItem == NULL)
		return;
	SetItemLeftTB(NMachineItem, ItemToolbars[1]);
	ProcessSibling(NMachineItem, &GlobalInd);

	if (NProgramItem == NULL)
		return;
	SetItemLeftTB(NProgramItem, ItemToolbars[2]);
	ProcessSibling(NProgramItem, &GlobalInd);

	if (NProgramItemAux != NULL)
	{
		SetItemLeftTB(NProgramItemAux, ItemToolbars[3]);
		ProcessSibling(NProgramItemAux, &GlobalInd);
	}
	else
		ItemToolbars[3].ShowWindow(SW_HIDE);

	if (NToolItem == NULL)
		return;
	SetItemLeftTB(NToolItem, ItemToolbars[4]);
	ProcessSibling(NToolItem, &GlobalInd);

	if (NStockItem != NULL)
		SetItemLeftTB(NStockItem, ItemToolbars[5]);
	else
		ItemToolbars[5].ShowWindow(SW_HIDE);

	if (NStocksItem != NULL)
	{
		SetItemLeftTB(NStocksItem, ItemToolbars[6]);
		ProcessSibling(NStocksItem, &GlobalInd);
	}
	else
		ItemToolbars[6].ShowWindow(SW_HIDE);

	if (NPartItem == NULL)
		return;
	SetItemLeftTB(NPartItem, ItemToolbars[7]);
	ProcessSibling(NPartItem, &GlobalInd);

	if (NExObjItem== NULL)
		return;
	SetItemLeftTB(NExObjItem, ItemToolbars[8]);
	ProcessSibling(NExObjItem, &GlobalInd);
}

void NObjTreeCtrl::MakeTB(CNCMDoc* pDoc)
{
	// Parent TB
	{// Unit
		auto &TB = ItemToolbars[0];
		UINT IDs0[] = { ID_NCUNIT_LOAD, ID_NCUNIT_START, ID_PROPERTIES_ED };
		UINT IDsIcon[] = { IDI_CM_LIST, IDI_CM_UNIT_START, IDI_CM_PROPERTIES };
		Make1TB(TB, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]));
	}

	{// Machine
		auto &TB = ItemToolbars[1];
		UINT IDs0[] = { ID_PA_MACH_LOAD, ID_PA_MACH_VISIBLE, ID_PROPERTIES_ED };
		UINT IDsIcon[] = { IDI_CM_LIST, IDI_CM_VISIBLE_ON, IDI_CM_PROPERTIES, IDI_CM_VISIBLE_OFF };
		Make1TB(TB, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]));
		// Changeable buttons
		const MTMachineTool &MachTool = pDoc->GetMachTool();
		MakeChangable(TB, IDs0[1], MachTool.IsVisible() ? 1 : 3);
	}

	{// Programs
		auto &TB = ItemToolbars[2];
		if (pDoc->IsThereAnyProgr())
		{// программы загружены
			UINT IDs0[] = { ID_LOAD, ID_PA_PROG_VISIBLE, ID_PA_PROG_FP_TRAJ };
			UINT IDsIcon[] = { IDI_CM_LOAD, IDI_CM_VISIBLE_ON, IDI_FTP_ON, IDI_FTP_OFF, IDI_CM_VISIBLE_OFF };
			Make1TB(TB, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]));
			MakeChangable(TB, IDs0[1], pDoc->NGetProgramList()->Visible ? 1 : 4);
			MakeChangable(TB, IDs0[2], pDoc->NGetProgramList()->PartialDraw == TrajPart::TP_WHOLE ? 2 : 3);
		}
		else
		{// нет программ
			UINT IDs0[] = { ID_LOAD, ID_PA_PROGR_CR, ID_LOAD_AUX };
			UINT IDsIcon[] = { IDI_CM_LOAD, IDI_TB_NEW, IDI_CM_LOAD };
			Make1TB(TB, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]));
		}
	}

	{// Programs aux
		auto &TB = ItemToolbars[3];
		UINT IDs0[] = { ID_LOAD_AUX, ID_PA_PROGR_CR_AUX, ID_PA_PROG_DELAllSUBPROG };
		UINT IDsIcon[] = { IDI_CM_LOAD, IDI_TB_NEW, IDI_CM_DELETE};
		Make1TB(TB, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]));
	}


	{// Tools
		auto &TB = ItemToolbars[4];
		if (pDoc->IsThereAnyTool())
		{// Есть инструменты
			switch (NCMComData::GetType())
			{
			case AT_MILL:
			{
				UINT IDs0[] = { ID_PA_TOOLS_CR, ID_PA_TOOLS_INV, ID_PA_TOOLS_SHAPE };
				UINT IDsIcon[] = { IDI_TB_NEW, IDI_CM_VISIBLE_TOGGLE, IDI_CM_VISTOOL_ON, IDI_CM_VISIBLE_OFF, IDI_CM_VISTOOL_OFF };
				Make1TB(TB, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]));
				//MakeChangable(TB, IDs0[1], pDoc->IsThereAnyVisTool() ? 1 : 3);				
				MakeChangable(TB, IDs0[2], NTool::IsShapeVisible() ? 2 : 4);
				break;
			}
			case AT_TURN:
			{
				UINT IDs0[] = { ID_PA_TOOLS_TURN, ID_PA_TOOLS_INV, ID_PA_TOOLS_SHAPE };
				UINT IDsIcon[] = { IDI_TB_NEW, IDI_CM_VISIBLE_TOGGLE, IDI_CM_VISTOOL_ON, IDI_CM_VISIBLE_OFF, IDI_CM_VISTOOL_OFF };
				Make1TB(TB, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]));
				MakeChangable(TB, IDs0[2], NTool::IsShapeVisible() ? 2 : 4);
				break;
			}
			case AT_MILL_TURN:
			{
				UINT IDs0[] = { ID_PA_TOOLS_MT, ID_PA_TOOLS_INV, ID_PA_TOOLS_SHAPE };
				UINT IDsIcon[] = { IDI_TB_NEW, IDI_CM_VISIBLE_TOGGLE, IDI_CM_VISTOOL_ON, IDI_CM_VISIBLE_OFF, IDI_CM_VISTOOL_OFF };
				Make1TB(TB, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]));
				MakeChangable(TB, IDs0[2], NTool::IsShapeVisible() ? 2 : 4);
				break;
			}
			}
		}
		else
		{// Нет инструментов
			switch (NCMComData::GetType())
			{
			default:
			case AT_MILL:
			{
				UINT IDs0[] = { ID_PA_TOOLS_IMPNCM, ID_PA_TOOLS_CR, ID_PROPERTIES_ED };
				UINT IDsIcon[] = { IDI_CM_LOAD, IDI_TB_NEW, IDI_CM_PROPERTIES };
				Make1TB(TB, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]));
				break;
			}
			case AT_TURN:
			{
				UINT IDs0[] = { ID_PA_TOOLS_IMPNCM, ID_PA_TOOLS_TURN, ID_PROPERTIES_ED };
				UINT IDsIcon[] = { IDI_CM_LOAD, IDI_TB_NEW, IDI_CM_PROPERTIES };
				Make1TB(TB, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]));
				break;
			}
			case AT_MILL_TURN:
			{
				UINT IDs0[] = { ID_PA_TOOLS_IMPNCM, ID_PA_TOOLS_MT, ID_PROPERTIES_ED };
				UINT IDsIcon[] = { IDI_CM_LOAD, IDI_TB_NEW, IDI_CM_PROPERTIES };
				Make1TB(TB, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]));
				break;
			}
			}
		}
	}

	{// Stock
		auto &TB = ItemToolbars[5];
		if (NStockItem) // NStockItem may be null in Weld project or we have more than one stock
		{
			if (pDoc->GetStock(0))
			{ // The stock is defined
				switch (NCMComData::GetType())
				{
				default:
				case AT_MILL:
				{
					UINT IDs0[] = { ID_CH_STOCK_CR_BOX, ID_PA_STOCK_VISIBLE, ID_PA_STOCK_COLOR };
					UINT IDsIcon[] = { IDI_CM_BOX, IDI_CM_VISIBLE_ON, IDI_CM_VISIBLE_OFF, IDI_CM_VISIBLE_OFF };
					Make1TB(TB, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]));
					MakeChangable(TB, IDs0[1], pDoc->NGetStockList()->IsVisible() ? 1 : 3);
					SetColorIcon(2, TB, pDoc->GetStock(0)->GetColor());			
					break;
				}
				case AT_MILL_TURN:
				case AT_TURN:
				{
					UINT IDs0[] = { ID_CH_STOCK_CR_CONE, ID_PA_STOCK_VISIBLE, ID_PA_STOCK_COLOR };
					UINT IDsIcon[] = { IDI_CM_CYL, IDI_CM_VISIBLE_ON, IDI_CM_VISIBLE_OFF, IDI_CM_VISIBLE_OFF };
					Make1TB(TB, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]));
					MakeChangable(TB, IDs0[1], pDoc->NGetStockList()->IsVisible() ? 1 : 3);
					SetColorIcon(2, TB, pDoc->GetStock(0)->GetColor());
					break;
				}
				}
			}
			else
			{ // The stock is not defined
				switch (NCMComData::GetType())
				{
				default:
				case AT_MILL:
				{
					UINT IDs0[] = { ID_CH_STOCK_CR_STL, ID_CH_STOCK_CR_BOX, ID_PA_STOCK_CR_CYL };
					UINT IDsIcon[] = { IDI_CM_LOAD, IDI_CM_BOX, IDI_CM_CYL };
					Make1TB(TB, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]));
					break;
				}
				case AT_TURN:
				{
					UINT IDs0[] = { ID_PA_STOCK_CR_CONE, ID_PA_STOCK_CR_PROFILE, ID_PROPERTIES_ED };
					UINT IDsIcon[] = { IDI_CM_CYL, IDI_CM_CONT_EX, IDI_CM_PROPERTIES };
					Make1TB(TB, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]));
					break;
				}
				case AT_MILL_TURN:
				{
					UINT IDs0[] = { ID_PA_STOCK_CR_CONE, ID_PA_STOCK_CR_PIPE, ID_PA_STOCK_CR_PROFILE };
					UINT IDsIcon[] = { IDI_CM_CYL, IDI_CM_BOX, IDI_CM_CONT_EX };
					Make1TB(TB, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]));
					break;
				}
				}		
			}
		}
	}

	{// Stocks
		auto &TB = ItemToolbars[6];
		if (NStocksItem) // NStocksItem may be NULL if we have single stock only
		{
			switch (NCMComData::GetType())
			{
			case AT_MILL:
			{
				if (pDoc->NGetStockList()->HaveTempStock())
				{//temp stocks
					UINT IDs0[] = { ID_PA_STOCK_TEMP, ID_PA_STOCK_TEMPREM, ID_PA_STOCK_VISIBLE };
					UINT IDsIcon[] = { IDI_STEMP_ON, IDI_STEMP_OFF, IDI_CM_VISIBLE_ON, IDI_CM_VISIBLE_OFF };
					Make1TB(TB, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]));
					MakeChangable(TB, IDs0[2], pDoc->NGetStockList()->IsVisible() ? 2 : 3);
				}
				else
				{
				UINT IDs0[] = { ID_CH_STOCK_CR_BOX, ID_PA_STOCK_VISIBLE, ID_PA_STOCK_COLOR };
				UINT IDsIcon[] = { IDI_CM_BOX, IDI_CM_VISIBLE_ON, IDI_CM_VISIBLE_OFF, IDI_CM_VISIBLE_OFF };
				Make1TB(TB, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]));
				MakeChangable(TB, IDs0[1], pDoc->NGetStockList()->IsVisible() ? 1 : 3);
				SetColorIcon(2, TB, pDoc->GetStock(0)->GetColor());
				}
				break;
			}
			case AT_TURN:
			{
				UINT IDs0[] = { ID_PA_STOCK_CR_CONE, ID_PA_STOCK_VISIBLE, ID_PA_STOCK_COLOR };
				UINT IDsIcon[] = { IDI_CM_CYL, IDI_CM_VISIBLE_ON, IDI_CM_VISIBLE_OFF, IDI_CM_VISIBLE_OFF };
				Make1TB(TB, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]));
				MakeChangable(TB, IDs0[1], pDoc->NGetStockList()->IsVisible() ? 1 : 3);
				SetColorIcon(2, TB, pDoc->GetStock(0)->GetColor());
				break;
			}
			case AT_MILL_TURN:
			{
				if (pDoc->NGetStockList()->HaveTempStock())
				{//temp stocks
					UINT IDs0[] = { ID_PA_STOCK_TEMP, ID_PA_STOCK_TEMPREM, ID_PA_STOCK_VISIBLE };
					UINT IDsIcon[] = { IDI_STEMP_ON, IDI_STEMP_OFF, IDI_CM_VISIBLE_ON, IDI_CM_VISIBLE_OFF };
					Make1TB(TB, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]));
					MakeChangable(TB, IDs0[1], pDoc->NGetStockList()->IsVisible() ? 2 : 3);
				}
				else
				{
					UINT IDs0[] = { ID_PA_STOCK_CR_CONE, ID_PA_STOCK_VISIBLE, ID_PA_STOCK_COLOR };
					UINT IDsIcon[] = { IDI_CM_CYL, IDI_CM_VISIBLE_ON, IDI_CM_VISIBLE_OFF, IDI_CM_VISIBLE_OFF };
					Make1TB(TB, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]));
					MakeChangable(TB, IDs0[1], pDoc->NGetStockList()->IsVisible() ? 1 : 3);
					SetColorIcon(2, TB, pDoc->GetStock(0)->GetColor());
				}
				break;
			}
			}
		}
	}

	{// Part
		auto &TB = ItemToolbars[7];
		if (!pDoc->NGetPartList()->empty())
		{
			switch (NCMComData::GetType())
			{
			case AT_MILL:
			{
				UINT IDs0[] = { ID_PA_PART_CR_STL, ID_PA_PART_VISIBLE, ID_PA_PART_POS };
				UINT IDsIcon[] = { IDI_CM_LOAD, IDI_CM_VISIBLE_ON, IDI_CM_POS, IDI_CM_VISIBLE_OFF };
				Make1TB(TB, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]));
				MakeChangable(TB, IDs0[1], pDoc->NGetPartList()->IsVisible() ? 1 : 3);
				break;
			}
			default:
			{
				UINT IDs0[] = { ID_PA_PART_CR_STL, ID_PA_PART_VISIBLE, ID_PA_PART_POS };
				UINT IDsIcon[] = { IDI_CM_LOAD, IDI_CM_VISIBLE_ON, IDI_CM_POS, IDI_CM_VISIBLE_OFF };
				Make1TB(TB, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]));
				MakeChangable(TB, IDs0[1], pDoc->NGetPartList()->IsVisible() ? 1 : 3);
				break;
			}
			}
		}
		else
		{// no part
				UINT IDs0[] = { ID_PA_PART_CR_STL, ID_PA_PART_VISIBLE, ID_PROPERTIES_ED };
				UINT IDsIcon[] = { IDI_CM_LOAD, IDI_CM_VISIBLE_ON, IDI_CM_PROPERTIES, IDI_CM_VISIBLE_OFF };
				Make1TB(TB, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]));
				MakeChangable(TB, IDs0[1], pDoc->NGetPartList()->IsVisible() ? 1 : 3);
		}
	}

	{// ExObj
		auto &TB = ItemToolbars[8];
		if (!pDoc->NGetExObjList()->empty())
		{
		UINT IDs0[] = { ID_PA_EXOBJ_CR_STL, ID_PA_EXOBJ_VISIBLE, ID_PA_EXOBJ_POS };
		UINT IDsIcon[] = { IDI_CM_LOAD, IDI_CM_VISIBLE_ON, IDI_CM_POS, IDI_CM_VISIBLE_OFF };
		Make1TB(TB, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]));
		MakeChangable(TB, IDs0[1], pDoc->NGetExObjList()->IsVisible() ? 1 : 3);
		}
		else
		{
		UINT IDs0[] = { ID_PA_EXOBJ_CR_STL, ID_PA_EXOBJ_VISIBLE, ID_PROPERTIES_ED };
		UINT IDsIcon[] = { IDI_CM_LOAD, IDI_CM_VISIBLE_ON, IDI_CM_PROPERTIES, IDI_CM_VISIBLE_OFF };
		Make1TB(TB, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]));
		MakeChangable(TB, IDs0[1], pDoc->NGetExObjList()->IsVisible() ? 1 : 3);
		}

	}
	// Child TB

	size_t GlobalInd = 0;

	{//Machine
		UINT IDs0[] = { ID_CH_MACH_VISIBLE, ID_CH_MACH_WF, ID_PROPERTIES_ED };
		UINT IDsIcon[] = { IDI_CM_VISIBLE_ON, IDI_CM_WF, IDI_CM_PROPERTIES, IDI_CM_VISIBLE_OFF, IDI_TB_INIT };
		const MTPartArray &Parts = pDoc->GetMachTool().GetParts();
		int i = 0;
		for (HTREEITEM item = GetChildItem(NMachineItem); item != NULL; item = GetNextSiblingItem(item), GlobalInd++, ++i)
		{
			if (GlobalInd >= SubItemToolBars.size())
				break;
			Make1TB(SubItemToolBars[GlobalInd], IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]));
			MakeChangable(SubItemToolBars[GlobalInd], IDs0[0], Parts[i]->IsVisible() ? 0 : 3);
			MakeChangable(SubItemToolBars[GlobalInd], IDs0[1], Parts[i]->IsWFrame() ? 1 : 4);
		}
	}

	{// Programs
		UINT IDs0[] = { ID_SAVE, ID_CH_PROGR_VISIBLE, ID_CH_PROGRAM_COLOR };
		UINT IDsIcon[] = { IDI_TB_SAVE, IDI_CM_VISIBLE_ON, IDI_CM_VISIBLE_ON, IDI_CM_VISIBLE_OFF };
		const CMapStringToNProgram *pPrL = pDoc->NGetProgramList();
		int i = 0;
		HTREEITEM item;
		for (item = GetChildItem(NProgramItem); item != NULL; item = GetNextSiblingItem(item), GlobalInd++, i++)
		{
			if (GlobalInd >= SubItemToolBars.size())
				break;
			auto &TB = SubItemToolBars[GlobalInd];
			Make1TB(TB, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]));
			NProgram *pP;
			CString key;
			CImageList *pList = TB.GetToolBarCtrl().GetImageList();
			if (pPrL->GetAt(i, key, (CObject *&)pP))
			{
				SetColorIcon(2, TB, pP->GetColor());
				MakeChangable(TB, IDs0[1], pP->IsVisible() ? 1 : 3);
			}
		}
	}

	{// Programs aux
		if (NProgramItemAux)
		{
			UINT IDs0[] = { ID_SAVE, ID_CH_PROGR_TEXT, ID_DELETE };
			UINT IDsIcon[] = { IDI_TB_SAVE, IDI_MACH_EDIT, IDI_CM_DELETE };
			const CMapStringToNProgram *pPrL = pDoc->NGetProgramList();
			int i = pPrL->GetMainProgCount();
			HTREEITEM item;
			for (item = GetChildItem(NProgramItemAux); item != NULL; item = GetNextSiblingItem(item), GlobalInd++, i++)
			{
				if (GlobalInd >= SubItemToolBars.size())
					break;
				auto &TB = SubItemToolBars[GlobalInd];
				Make1TB(TB, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]));
			}
		}
	}

	{// Tools
		UINT IDs0[] = { ID_CH_TOOL_EDIT, ID_CH_TOOL_VISIBLE, ID_CH_TOOL_COLOR };
		UINT IDsIcon[] = { IDI_MACH_EDIT, IDI_CM_VISIBLE_ON, IDI_TB_ZOOMD, IDI_CM_VISIBLE_OFF };
		UINT IDs1[] = { ID_CH_TOOL_DELETE, ID_CH_TOOL_COPY, ID_CH_TOOL_PASTE };
		UINT IDsIcon1[] = { IDI_CM_DELETE, IDI_CM_COPY, IDI_CM_PASTE };
		const NMapStringToNTool *pTlL = pDoc->NGetToolList();
		for (HTREEITEM item = GetChildItem(NToolItem); item != NULL; item = GetNextSiblingItem(item), GlobalInd++)
		{
			if (GlobalInd >= SubItemToolBars.size())
				break;
			auto &TB = SubItemToolBars[GlobalInd];
			Make1TB(TB, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]));
			CString ToolText = GetItemText(item);
			int k = ToolText.Find(" ");
			NToolID ToolID;
			ToolID.CrFromPrintStr(ToolText.Left(k));
			CImageList *pList = TB.GetToolBarCtrl().GetImageList();
			auto it = pTlL->find(ToolID);
			if (it != pTlL->end()) 
			{
				NToolCombined* pP = static_cast<NToolCombined*>(it->second);
				SetColorIcon(2, TB, pP->Color);
				MakeChangable(TB, IDs0[1], pP->Visible ? 1 : 3);
			}

			// Sub tools
			for (HTREEITEM SubChildItem = GetChildItem(item); SubChildItem != NULL; SubChildItem = GetNextSiblingItem(SubChildItem))
			{
				if (GlobalInd >= SubItemToolBars.size())
					break;
				++GlobalInd;
				Make1TB(SubItemToolBars[GlobalInd], IDs1, sizeof(IDs1) / sizeof(IDs1[0]), IDsIcon1, sizeof(IDsIcon1) / sizeof(IDsIcon1[0]));
			}
		}
	}

	{// Stocks
		if (NStocksItem)
		{
			if (pDoc->NGetStockList()->HaveTempStock())
			{
				UINT IDs0[] = { ID_PA_STOCK_TEMP, ID_CH_STOCK_VISIBLE, ID_CH_STOCK_EXSTL };
				UINT IDsIcon[] = { IDI_STEMP_ON, IDI_CM_VISIBLE_ON, IDI_TB_SAVE, IDI_CM_VISIBLE_OFF };
//				const CStocksCollection *pStL = pDoc->NGetStockList();
				int i = 0;
				HTREEITEM item;
				for (item = GetChildItem(NStocksItem); item != NULL; item = GetNextSiblingItem(item), GlobalInd++, i++)
				{
					if (GlobalInd >= SubItemToolBars.size())
						break;
					auto &TB = SubItemToolBars[GlobalInd];
					Make1TB(TB, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]));
					CString Text = GetItemText(item);
					auto it = pDoc->NGetStockList()->find(Text);
					if (it == pDoc->NGetStockList()->end())
						continue;
					class NStock* pS = static_cast<class NStock*>(it->second);
					MakeChangable(TB, IDs0[1], pS->IsVisible() ? 1 : 3);
				}
			}
			else
			{
				UINT IDs0[] = { ID_CH_STOCK_DELETE, ID_CH_STOCK_VISIBLE, ID_CH_STOCK_EXSTL };
				UINT IDsIcon[] = { IDI_CM_DELETE, IDI_CM_VISIBLE_ON, IDI_TB_SAVE, IDI_CM_VISIBLE_OFF };
//				const CStocksCollection *pStL = pDoc->NGetStockList();
				int i = 0;
				HTREEITEM item;
				for (item = GetChildItem(NStocksItem); item != NULL; item = GetNextSiblingItem(item), GlobalInd++, i++)
				{
					if (GlobalInd >= SubItemToolBars.size())
						break;
					auto &TB = SubItemToolBars[GlobalInd];
					Make1TB(TB, IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]));
					CString Text = GetItemText(item);
					auto it = pDoc->NGetStockList()->find(Text);
					if (it == pDoc->NGetStockList()->end())
						continue;
					class NStock* pS = static_cast<class NStock*>(it->second);
					MakeChangable(TB, IDs0[1], pS->IsVisible() ? 1 : 3);
				}
			}
		}
	}

	{//Parts
		UINT IDs0[] = { ID_CH_PART_POS, ID_CH_PART_VISIBLE, ID_CH_PART_COLOR };
		UINT IDsIcon[] = { IDI_CM_POS, IDI_CM_VISIBLE_ON, IDI_CM_VISIBLE_ON, IDI_CM_VISIBLE_OFF };
		for (HTREEITEM item = GetChildItem(NPartItem); item != NULL; item = GetNextSiblingItem(item), GlobalInd++)
		{
			if (GlobalInd >= SubItemToolBars.size())
				break;
			auto &TB = SubItemToolBars[GlobalInd];
			Make1TB(SubItemToolBars[GlobalInd], IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]));
			CString Text = GetItemText(item);
			auto it = pDoc->NGetPartList()->find(Text);
			if (it == pDoc->NGetPartList()->end())
				continue;
			class NStock* pS = static_cast<class NStock*>(it->second);
			SetColorIcon(2, TB, pS->GetColor());
			MakeChangable(TB, IDs0[1], pS->Visible ? 1 : 3);
		}
	}

	{//ExObj
		UINT IDs0[] = { ID_CH_EXOBJ_POS, ID_CH_EXOBJ_VISIBLE, ID_CH_EXOBJ_COLOR };
		UINT IDsIcon[] = { IDI_CM_POS, IDI_CM_VISIBLE_ON, IDI_CM_VISIBLE_ON, IDI_CM_VISIBLE_OFF };
		for (HTREEITEM item = GetChildItem(NExObjItem); item != NULL; item = GetNextSiblingItem(item), GlobalInd++)
		{
			if (GlobalInd >= SubItemToolBars.size())
				break;
			auto &TB = SubItemToolBars[GlobalInd];
			Make1TB(SubItemToolBars[GlobalInd], IDs0, sizeof(IDs0) / sizeof(IDs0[0]), IDsIcon, sizeof(IDsIcon) / sizeof(IDsIcon[0]));
			CString Text = GetItemText(item);
			auto it = pDoc->NGetExObjList()->find(Text);
			if (it == pDoc->NGetExObjList()->end())
				continue;
			class NStock* pS = static_cast<class NStock*>(it->second);
			SetColorIcon(2, TB, pS->GetColor());
			MakeChangable(TB, IDs0[1], pS->Visible ? 1 : 3);
		}
	}
	SetHighlights(pDoc);
	ProcessItemPosChanged();// Place toolbars to their right places
}

void NObjTreeCtrl::Make1TB(CToolBar & TB, UINT * IDs0, int ButtonsCount, UINT * IDsIcon, int IconsCount, bool SetBkColor/* = true*/)
{
	if (SetBkColor)
	{
		NLeftViewToolBar *pTB = dynamic_cast<NLeftViewToolBar *>(&TB);
		if(pTB != nullptr)
			pTB->SetBkColor(GetBkColor());
	}

	int BSize = 16;
	CImageList *pList = TB.GetToolBarCtrl().GetImageList();
	HMODULE hins = GetModuleHandle(_T("ResInv.dll"));
	if (hins == NULL)
		return;
	if (pList == nullptr)
	{
		pList = new CImageList();
		pList->Create(BSize, BSize, ILC_COLOR32 | ILC_MASK, IconsCount, 1);
		TB.GetToolBarCtrl().SetImageList(pList);
	}
	TB.SetButtons(IDs0, ButtonsCount);
	pList->SetImageCount(IconsCount);
	for (int i = 0; i < IconsCount; ++i)
	{
		HICON buf;
		pList->Replace(i, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDsIcon[i]), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
	}
}

void NObjTreeCtrl::MakeChangable(CToolBar &TB, int ButtonID, int NewIconInd)
{
	TBBUTTONINFO tbbi;
	tbbi.cbSize = sizeof(tbbi);
	tbbi.dwMask = TBIF_IMAGE;
	TB.GetToolBarCtrl().GetButtonInfo(ButtonID, &tbbi);
	if (NewIconInd != tbbi.iImage)
	{
		tbbi.iImage = NewIconInd;
		TB.GetToolBarCtrl().SetButtonInfo(ButtonID, &tbbi);
	}
}

bool NObjTreeCtrl::SetItemLeftTB(HTREEITEM item, CToolBar & TB)
{
	const int HorizontalShift = 2;

	CRect itemRect;
	GetItemRect(item, itemRect, TRUE);
	CRect tbRect;
	TB.GetToolBarCtrl().SetRows(1, TRUE, tbRect);
	if (tbRect.Height() == 0)
		return true;
	tbRect.bottom = tbRect.Height();
	tbRect.right = tbRect.Width();
	tbRect.left = 0;
	tbRect.top = 0;
	CPoint base;
	base.x = itemRect.left - tbRect.Width() - HorizontalShift;
	base.y = itemRect.top + (itemRect.Height() - tbRect.Height()) / 2;
	TB.MoveWindow(tbRect + base);
	TB.ShowWindow(SW_SHOW);
	return true;
}

void NObjTreeCtrl::SetColorIcon(int ind, CToolBar &TB, const NColor &col)
{
	// Create color bitmap
	COLORMAP Map;
	Map.from = RGB(0, 0, 0);
	Map.to = col.GetRGB();
	CBitmap ColorBmp, MaskBmp;
	ColorBmp.LoadMappedBitmap(IDB_BITMAP_COL, 0, &Map, 1);
	MaskBmp.LoadBitmap(IDB_BITMAP_COL);
	// Insert color bitmap into the image list
	CImageList *pList = TB.GetToolBarCtrl().GetImageList();
	if (pList == nullptr)
		return;
	if (pList->Replace(ind, &ColorBmp, &MaskBmp) < 0)
		AfxMessageBox(IDS_LEFTVIEW_IMGERR);
	else
		TB.RedrawWindow();
}



void NObjTreeCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	ShowWindow(SW_HIDE);
	MTreeCtrl::OnVScroll(nSBCode, nPos, pScrollBar);

	ProcessItemPosChanged();
	ShowWindow(SW_SHOW);
}


BOOL NObjTreeCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	ShowWindow(SW_HIDE);
	auto res = MTreeCtrl::OnMouseWheel(nFlags, zDelta, pt);
	ProcessItemPosChanged();
	ShowWindow(SW_SHOW);
	return res;
}


void NObjTreeCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	ShowWindow(SW_HIDE);
	MTreeCtrl::OnHScroll(nSBCode, nPos, pScrollBar);

	ProcessItemPosChanged();
	ShowWindow(SW_SHOW);
}


void NObjTreeCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	ShowWindow(SW_HIDE);
	MTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);

	ProcessItemPosChanged();
	ShowWindow(SW_SHOW);
}

bool NObjTreeCtrl::IsItemExpanded(HTREEITEM item) const
{
	if (item == nullptr)
		return false;

	return (GetItemState(item, TVIS_EXPANDED) & TVIS_EXPANDED) != 0;
}

void NObjTreeCtrl::SetItemExpanded(HTREEITEM item) 
{
	Expand(item, TVE_EXPAND);
}

void NObjTreeCtrl::SetHighlights(CNCMDoc * pDoc)
{
	size_t GlobalInd = 0;
	{//Unit
		if (!pDoc->GetUnit()->Defined())
			ItemToolbars[0].SetBkColorWRedraw(RGB(255, 200, 200));
	}
	{//Stock
		if (!pDoc->GetStock(0))
			ItemToolbars[5].SetBkColorWRedraw(RGB(255, 200, 200));
	}

	{//Machine
		if (!pDoc->GetUnit()->Defined())
			ItemToolbars[1].SetBkColorWRedraw(RGB(255, 200, 200));
		else if(!pDoc->GetMachTool().GetCurConfig().IsDefined())
			ItemToolbars[1].SetBkColorWRedraw(RGB(255, 200, 200));
		for (HTREEITEM item = GetChildItem(NMachineItem); item != NULL; item = GetNextSiblingItem(item), GlobalInd++)
		{
			if (GlobalInd >= SubItemToolBars.size())
				break;
		}
	}
	const CString &CurProg = pDoc->GetUnit()->GetState()->GetCurProg();
	cadrID CurCadrID = pDoc->GetUnit()->GetState()->GetCurCadrID();
	const CString &LoaProg = pDoc->GetProgrTDT();
	const CMapStringToNProgram *pPrL = pDoc->NGetProgramList();
	constexpr COLORREF MinorErrorCol = RGB(240, 230, 140);

	{// Programs
		if(GetChildItem(NProgramItem) == NULL)
			ItemToolbars[2].SetBkColorWRedraw(RGB(255, 200, 200));
		bool IsErrProg = false;
		int i = 0;
		for (HTREEITEM item = GetChildItem(NProgramItem); item != NULL; item = GetNextSiblingItem(item), GlobalInd++, i++)
		{
			if (GlobalInd >= SubItemToolBars.size())
				break;
			auto &TB = SubItemToolBars[GlobalInd];
			NProgram *pP;
			CString key;
			if (pPrL->GetAt(i, key, (CObject *&)pP))
			{
				if (pP->HaveErr())
				{
					IsErrProg = true;
					TB.SetBkColorWRedraw(RGB(255, 200, 200));
					ItemToolbars[2].SetBkColorWRedraw(RGB(255, 200, 200));
				}
				else if (pP->GetErrorHistory().GetSize() != 0)
				{
					TB.SetBkColorWRedraw(MinorErrorCol);
					ItemToolbars[2].SetBkColorWRedraw(MinorErrorCol);
					TB.SetBkColorWRedraw(MinorErrorCol);
				}
				else if (key == CurProg)
					TB.SetBkColorWRedraw(RGB(200, 255, 200));
				else
					TB.SetBkColorWRedraw(GetBkColor());

				SetItemBold(item, key == LoaProg);
				SetItemCut(item, pP->IsActive() && pPrL->IsActive());
			}
		}
		SetItemState(NProgramItem, IsErrProg ? TVIS_BOLD : ~TVIS_BOLD, TVIS_BOLD); // added 21.12.17 to prevent dragging if errors occured
	}

	{// Programs aux
		if (NProgramItemAux)
		{
			bool IsErrProg = false;
			int i = pPrL->GetMainProgCount();
			for (HTREEITEM item = GetChildItem(NProgramItemAux); item != NULL; item = GetNextSiblingItem(item), GlobalInd++, i++)
			{
				if (GlobalInd >= SubItemToolBars.size())
					break;
				auto &TB = SubItemToolBars[GlobalInd];
				NProgram *pP = nullptr;
				CString key;
				if (pPrL->GetAt(i, key, (CObject *&)pP))
				{
					if (pP != nullptr)
					{
						if (pP->HaveErr())
						{
							IsErrProg = true;
							TB.SetBkColorWRedraw(RGB(255, 200, 200));
							ItemToolbars[3].SetBkColorWRedraw(RGB(255, 200, 200));
						}
						else if (pP->GetErrorHistory().GetSize() != 0)
						{
							TB.SetBkColorWRedraw(MinorErrorCol);
							ItemToolbars[3].SetBkColorWRedraw(MinorErrorCol);
							TB.SetBkColorWRedraw(MinorErrorCol);
						}
						else if (i == CurCadrID.Prog)
							TB.SetBkColorWRedraw(RGB(200, 255, 200));
						else if (pP->IsProtected() && NCUnit::IsFileProtectionEnabled())
							TB.SetBkColorWRedraw(RGB(200, 200, 200));
						else
							TB.SetBkColorWRedraw(GetBkColor());
						SetItemBold(item, key == LoaProg);
						SetItemCut(item, pP->IsActive() && pPrL->IsActive());
					}
				}
			}
			SetItemState(NProgramItemAux, IsErrProg ? TVIS_BOLD : ~TVIS_BOLD, TVIS_BOLD); // added 21.12.17 to prevent dragging if errors occured
		}
	}
	{// Tools
		//if (GetChildItem(NToolItem) == NULL)
		//		ItemToolbars[4].SetBkColorWRedraw(RGB(255, 200, 200));
		const NMapStringToNTool *pTlL = pDoc->NGetToolList();
		for (HTREEITEM item = GetChildItem(NToolItem); item != NULL; item = GetNextSiblingItem(item), GlobalInd++)
		{
			if (GlobalInd >= SubItemToolBars.size())
				break;
			auto &TB = SubItemToolBars[GlobalInd];
			CString ToolText = GetItemText(item);
			int k = ToolText.Find(" ");
			NToolID ToolID;
			ToolID.CrFromPrintStr(ToolText.Left(k));
			NToolCombined *pToolCom = nullptr;
			auto it = pDoc->NGetToolList()->find(ToolID);
			if (it != pDoc->NGetToolList()->end())
				pToolCom = static_cast<NToolCombined*>(it->second);
			GlobalInd += pToolCom->num_tools > 1 ? pToolCom->num_tools : 0;
			COLORREF NewColor = GetBkColor();
			bool ToolHasCutting = false;
			for (int t = 0; t < pToolCom->num_tools; ++t)
			{
				NTool *pTool = pToolCom->GetTool(t);
				if (pTool != nullptr)
					if (pTool->IsCutting())
						ToolHasCutting = true;
			}
			if (!ToolHasCutting)
				NewColor = RGB(255, 200, 200);
			if (ToolID == pDoc->GetUnit()->GetState()->CurToolN)
				NewColor = RGB(200, 255, 200);
			TB.SetBkColorWRedraw(NewColor);
		}
	}
	//{//Part

	//	if (GetChildItem(NPartItem) == NULL)
	//		ItemToolbars[7].SetBkColorWRedraw(RGB(255, 200, 200));
	//}
	//{//Fix

	//	if (GetChildItem(NExObjItem) == NULL)
	//		ItemToolbars[8].SetBkColorWRedraw(RGB(255, 200, 200));
	//}

}

void NObjTreeCtrl::SetPosChecks(CNCMDoc* pDoc, bool SetChecks)
{
	SetPosChecks1Sect(pDoc, SetChecks, NPartItem, CMapStringToNStock::PrefixPart);
	SetPosChecks1Sect(pDoc, SetChecks, NExObjItem, CMapStringToNStock::PrefixFixt);
	SetPosChecks1Sect(pDoc, SetChecks, NStocksItem, CMapStringToNStock::PrefixStock);
	SetPosChecks1Sect(pDoc, SetChecks, NStockItem, CMapStringToNStock::PrefixStock);
	MakeTB(pDoc);
}

void NObjTreeCtrl::SetPosChecks1Sect(CNCMDoc* pDoc, bool SetChecks, HTREEITEM SectItem, const CString& Prefix)
{
	if (SectItem == NULL)
		return;
	bool AllChecked = true;
	bool AllUnchecked = true;
	for (HTREEITEM item = GetChildItem(SectItem); item != NULL; item = GetNextSiblingItem(item))
	{
		UINT nState = INDEXTOSTATEIMAGEMASK(0);
		if (SetChecks)
		{
			CString Text = Prefix + GetItemText(item);
			auto it = pDoc->NGetPosChecks()->find(Text);
			if (it != pDoc->NGetPosChecks()->end())
			{
				nState = INDEXTOSTATEIMAGEMASK(TOOL_CHECKED);
				AllUnchecked = false;
			}
			else
			{
				nState = INDEXTOSTATEIMAGEMASK(TOOL_UNCHECKED);
				AllChecked = false;
			}
		}
		SetItemState(item, nState, TVIS_STATEIMAGEMASK);
	}
	if (SectItem == NStockItem)
	{
		if (SetChecks)
		{
			CString Text = Prefix + GetItemText(SectItem);
			class NStock* pS = nullptr;
			auto it = pDoc->NGetPosChecks()->find(Text);
			if (it != pDoc->NGetPosChecks()->end())
				AllUnchecked = false;
			else
				AllChecked = false;
		}
	}
	UINT nState = INDEXTOSTATEIMAGEMASK(0);
	if (SetChecks)
		nState = (AllUnchecked ? INDEXTOSTATEIMAGEMASK(TOOL_UNCHECKED)
			: (AllChecked ? INDEXTOSTATEIMAGEMASK(TOOL_CHECKED) : INDEXTOSTATEIMAGEMASK(TOOL_PARTIAL)));
	SetItemState(SectItem, nState, TVIS_STATEIMAGEMASK);
}

void NObjTreeCtrl::InvertPosCheck(CMapStringToNStock* pStL, HTREEITEM CurItem, const CString& Prefix)
{
	CNCMDoc* pDoc = ((CLeftView*)GetParent())->GetDocument();
	if (pDoc == nullptr)
		return;

	CString Text = GetItemText(CurItem);
	class NStock* pS = nullptr;

	if (IsChecked(CurItem))
	{
		pDoc->NGetPosChecks()->RemoveKey(Prefix + Text);
	}
	else
	{
		BOOL Found = FALSE;
		if (CurItem == NStockItem)
		{
			CString key;
			Found = pStL->GetAt(0, key, (CObject*&)pS);
		}
		else
		{
			auto it = pStL->find(Text);
			Found = (it != pStL->end());
			if (Found)
				pS = static_cast<class NStock*>(it->second);
		}
		if(Found)
			pDoc->NGetPosChecks()->SetAt(Prefix + Text, pS);
	}
}

void NObjTreeCtrl::ProcPaPosCheck(CMapStringToNStock* pStL, HTREEITEM SectItem, const CString& Prefix)
{
	if (IsChecked(SectItem))
	{// unselect all 
		MergeStL2PosChecks(pStL, Prefix, false);
	}
	else
	{// select all
		MergeStL2PosChecks(pStL, Prefix, true);
	}
}

void NObjTreeCtrl::MergeStL2PosChecks(CMapStringToNStock* pStL, const CString& Prefix, bool Add)
{
	CNCMDoc* pDoc = ((CLeftView*)GetParent())->GetDocument();
	if (pDoc == nullptr)
		return;
	for each(const auto& el in *pStL)
	{
		CString key = el.first;
		class NStock* pS = static_cast<class NStock*>(el.second);
		if(Add)
			pDoc->NGetPosChecks()->SetAt(Prefix + key, pS);
		else
			pDoc->NGetPosChecks()->RemoveKey(Prefix + key);
	}
}

bool NObjTreeCtrl::IsChecked(HTREEITEM item) const
{
	return (GetItemState(item, TVIS_STATEIMAGEMASK) & TVIS_STATEIMAGEMASK) == INDEXTOSTATEIMAGEMASK(TOOL_CHECKED);
}

bool NObjTreeCtrl::IsPartial(HTREEITEM item) const
{
	return (GetItemState(item, TVIS_STATEIMAGEMASK) & TVIS_STATEIMAGEMASK) == INDEXTOSTATEIMAGEMASK(TOOL_PARTIAL);
}

void NObjTreeCtrl::ProcPosClick(HTREEITEM item)
{
	CNCMDoc* pDoc = ((CLeftView*)GetParent())->GetDocument();
	ASSERT_VALID(pDoc);
	HTREEITEM ParentItem = GetParentItem(item);
	if (ParentItem != NULL) /// Item has parent
	{
		switch (GetItemData(ParentItem))
		{
		case NToolList:
		{
			int ToolState = 0;
			if (IsChecked(item))
				ToolState = TOOL_UNCHECKED;
			else
				ToolState = TOOL_CHECKED;

			SetItemState(item, INDEXTOSTATEIMAGEMASK(ToolState), TVIS_STATEIMAGEMASK);
		}

		break;
		case NPartList:
			InvertPosCheck(pDoc->NGetPartList(), item, CMapStringToNStock::PrefixPart);
			break;
		case NExObjList:
			InvertPosCheck(pDoc->NGetExObjList(), item, CMapStringToNStock::PrefixFixt);
			break;
		case NStocksList:
			if(((CNCMApp*)AfxGetApp())->IsInCuttingMode())
				AfxMessageBox(IDS_COMP_CUTTING);
			else
				InvertPosCheck(pDoc->NGetStockList(), item, CMapStringToNStock::PrefixStock);
			break;
		default:
			break;
		}
	}
	else
	{/// Item has not parent
		switch (GetItemData(item))
		{
		case NPartList:
			ProcPaPosCheck(pDoc->NGetPartList(), item, CMapStringToNStock::PrefixPart);
			break;
		case NExObjList:
			ProcPaPosCheck(pDoc->NGetExObjList(), item, CMapStringToNStock::PrefixFixt);
			break;
		case NStocksList:
			if (((CNCMApp*)AfxGetApp())->IsInCuttingMode())
				AfxMessageBox(IDS_COMP_CUTTING);
			else
				ProcPaPosCheck(pDoc->NGetStockList(), item, CMapStringToNStock::PrefixStock);
			break;
		case NStock:
			if (((CNCMApp*)AfxGetApp())->IsInCuttingMode())
				AfxMessageBox(IDS_COMP_CUTTING);
			else
				InvertPosCheck(pDoc->NGetStockList(), item, CMapStringToNStock::PrefixStock);
			break;
		default:
			break;
		}
	}
}

bool NObjTreeCtrl::SetItemBold(HTREEITEM item, bool Set)
{
	if (((GetItemState(item, TVIS_BOLD) & TVIS_BOLD) != 0) == Set)
		return false;
	UINT NewState = Set ? TVIS_BOLD : ~TVIS_BOLD;
	SetItemState(item, NewState, TVIS_BOLD);

	return true;
}

bool NObjTreeCtrl::SetItemCut(HTREEITEM item, bool Set)
{
	if (((GetItemState(item, TVIS_CUT) & TVIS_CUT) == 0) == Set)
		return false;
	UINT NewState = Set ? ~TVIS_CUT : TVIS_CUT;
	SetItemState(item, NewState, TVIS_CUT);

	return false;
}

void NObjTreeCtrl::ShowAllTB(bool Show)
{
	int CmdShow = Show ? SW_SHOW : SW_HIDE;
	for (int i = 0; i < ItemTypesNum; ++i)
		ItemToolbars[i].ShowWindow(CmdShow);
	for (size_t i = 0; i < SubItemToolBars.size(); ++i)
		SubItemToolBars[i].ShowWindow(CmdShow);
}

void NObjTreeCtrl::OnDestroy()
{
	MTreeCtrl::OnDestroy();

	for (int i = 0; i < ItemTypesNum; ++i)
		delete ItemToolbars[i].GetToolBarCtrl().GetImageList();
	for (size_t i = 0; i < SubItemToolBars.size(); ++i)
		delete SubItemToolBars[i].GetToolBarCtrl().GetImageList();
	delete ContextToolBar.GetToolBarCtrl().GetImageList();
}

void NObjTreeCtrl::OnSize(UINT nType, int cx, int cy)
{
	auto WasVisible = ShowWindow(SW_HIDE);
	MTreeCtrl::OnSize(nType, cx, cy);

	ProcessItemPosChanged();
	if (WasVisible)
		ShowWindow(SW_SHOW);
}

HTREEITEM NObjTreeCtrl::SelectItemByPoint(const CPoint & point)
{
	UINT Flags;
	HTREEITEM CurItem = HitTest(point, &Flags);
	SelectItem(CurItem);
	return CurItem;
}
