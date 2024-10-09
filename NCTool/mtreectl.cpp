// MyTreeCtrl.cpp : implementation file
//

// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "mtreectl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyTreeCtrl

CMyTreeCtrl::CMyTreeCtrl()
{
	m_bDragging = FALSE;
	m_pimagelist = NULL;
	m_pParent = NULL;
}

void CMyTreeCtrl::SetParentWnd(CWnd *pParent)
{
	m_pParent = pParent;
}

CMyTreeCtrl::~CMyTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CMyTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnEndLabelEdit)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBeginDrag)
	ON_NOTIFY_REFLECT(TVN_BEGINRDRAG, OnBeginDrag)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyTreeCtrl message handlers
void CMyTreeCtrl::SetNewStyle(long lStyleMask, BOOL bSetBits)
{
	auto lStyleOld = GetWindowLongPtr(m_hWnd, GWL_STYLE);
	lStyleOld &= ~lStyleMask;
	if (bSetBits)
		lStyleOld |= lStyleMask;

	SetWindowLongPtr(m_hWnd, GWL_STYLE, lStyleOld);
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
}

void CMyTreeCtrl::OnEndLabelEdit(LPNMHDR pnmhdr, LRESULT *pLResult)
{
	TV_DISPINFO     *ptvinfo;

	ptvinfo = (TV_DISPINFO *)pnmhdr;
	if (ptvinfo->item.pszText != NULL)
	{
		ptvinfo->item.mask = TVIF_TEXT;
		SetItem(&ptvinfo->item);
	}
	*pLResult = TRUE;
}

void CMyTreeCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	HTREEITEM           hitem;
	UINT                flags;

	if (m_bDragging)
	{
		ASSERT(m_pimagelist != NULL);
		m_pimagelist->DragMove(point);
		if ((hitem = HitTest(point, &flags)) != NULL)
		{
			m_pimagelist->DragLeave(this);
			SelectDropTarget(hitem);
			m_hitemDrop = hitem;
			m_pimagelist->DragEnter(this, point);
		}
	}

	CTreeCtrl::OnMouseMove(nFlags, point);
}

BOOL CMyTreeCtrl::IsChildNodeOf(HTREEITEM hitemChild, HTREEITEM hitemSuspectedParent)
{
	do
	{
		if (hitemChild == hitemSuspectedParent)
			break;
	}
	while ((hitemChild = GetParentItem(hitemChild)) != NULL);

	return (hitemChild != NULL);
}


BOOL CMyTreeCtrl::TransferItem(HTREEITEM hitemDrag, HTREEITEM hitemDrop)
{
	HTREEITEM	hNewItem = NULL, hParentItem;
	int			nImage, nSelImage;
	RECT		rStart, rEnd;

	hParentItem = GetParentItem(hitemDrop);

	GetItemRect(hitemDrag, &rStart, TRUE);
	GetItemRect(hitemDrop, &rEnd, TRUE);

	if(rStart.top < rEnd.top)
		hNewItem = hitemDrop;
	else
		hNewItem = GetPrevSiblingItem(hitemDrop);

	//если добавление в корень первого уровня
	if (hParentItem == NULL)
		hParentItem = hitemDrop;

	if (hNewItem != NULL)
		hNewItem = InsertItem(GetItemText(hitemDrag), hParentItem, hNewItem);
	else
		hNewItem = InsertItem(GetItemText(hitemDrag), hParentItem, TVI_FIRST);

	GetItemImage(hitemDrag, nImage, nSelImage);
	SetItemData(hNewItem, GetItemData(hitemDrag));
	SetCheck(hNewItem, GetCheck(hitemDrag));
	SetItemImage(hNewItem, nImage, nImage);

	m_hNewDragDropItem = hNewItem;
	return TRUE;
}

void CMyTreeCtrl::OnButtonUp()
{
	if (m_bDragging)
	{
		ASSERT(m_pimagelist != NULL);
		m_pimagelist->DragLeave(this);
		m_pimagelist->EndDrag();
		delete m_pimagelist;
		m_pimagelist = NULL;

		if (m_hitemDrag != m_hitemDrop && 
			!IsChildNodeOf(m_hitemDrop, m_hitemDrag) &&
			GetParentItem(m_hitemDrag) != m_hitemDrop)
		{
			if (GetParentItem(m_hitemDrag) != GetParentItem(m_hitemDrop))
			{
				TransferItem(m_hitemDrag, m_hitemDrop);
				DeleteItem(m_hitemDrag);
			}
		}
		else
			MessageBeep(0);

		ReleaseCapture();
		SelectDropTarget(NULL);
		if (m_pParent)
			m_pParent->SendMessage(WM_MYTREE_LBUP);
		m_bDragging = FALSE;
	}
}

void CMyTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	OnButtonUp();
	CTreeCtrl::OnLButtonUp(nFlags, point);
}

void CMyTreeCtrl::OnRButtonUp(UINT nFlags, CPoint point)
{
	OnButtonUp();
	CTreeCtrl::OnRButtonUp(nFlags, point);
}

void CMyTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CTreeCtrl::OnLButtonDown(nFlags, point);
	m_point =  point;
	if (m_pParent)
		m_pParent->SendMessage(WM_MYTREE_LBDOWN);
}

void CMyTreeCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	CTreeCtrl::OnRButtonDown(nFlags, point);
	m_point =  point;
	if (m_pParent)
		m_pParent->SendMessage(WM_MYTREE_RBDOWN);
}

void CMyTreeCtrl::OnBeginDrag(LPNMHDR pnmhdr, LRESULT *pLResult)
{
	CPoint      ptAction;
	UINT        nFlags;

	GetCursorPos(&ptAction);
	ScreenToClient(&ptAction);
	ASSERT(!m_bDragging);
	m_hitemDrag = HitTest(ptAction, &nFlags);

/*	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pnmhdr;
	
	m_hitemDrag = HitTest(pNMTreeView->ptDrag,&nFlags);*/
	HTREEITEM	hParentItem;
	hParentItem = GetParentItem(m_hitemDrag);
	if (!hParentItem || hParentItem  == TVI_ROOT || GetItemText(hParentItem) == "")
		return;

	m_bDragging = TRUE;
	m_hitemDrop = NULL;

	ASSERT(m_pimagelist == NULL);
	m_pimagelist = CreateDragImage(m_hitemDrag);  // get the image list for dragging
	m_pimagelist->DragShowNolock(TRUE);
	m_pimagelist->SetDragCursorImage(0, CPoint(0, 0));
	m_pimagelist->BeginDrag(0, CPoint(0,0));
	m_pimagelist->DragMove(ptAction);
	m_pimagelist->DragEnter(this, ptAction);
	SetCapture();

	if (m_pParent)
		m_pParent->SendMessage(WM_MYTREE_BEGINGRAG);
}

