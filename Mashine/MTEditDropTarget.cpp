// MTEditDropTarget.cpp : implementation file
//

#include "stdafx.h"
#include "MTEditDropTarget.h"
#include "MTEditTreeCtrl.h"

#define RECT_BORDER	10

HWND MTEditDropTarget::m_shWndTreeCtrl = NULL;

// MTEditDropTarget

IMPLEMENT_DYNAMIC(MTEditDropTarget, COleDropTarget)


MTEditDropTarget::MTEditDropTarget()
{
	draging = false;
}

MTEditDropTarget::~MTEditDropTarget()
{
}


BEGIN_MESSAGE_MAP(MTEditDropTarget, COleDropTarget)
END_MESSAGE_MAP()



// MTEditDropTarget message handlers

BOOL MTEditDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject,	DROPEFFECT dropEffect, CPoint point)
{
	//Get the selected item from Source and destination Tree controls 
	m_pSourceTreeCtrl = (MTEditTreeCtrl *)CWnd::FromHandlePermanent(m_shWndTreeCtrl);
	HTREEITEM hTDropItem =m_pDestTreeCtrl->GetDropHilightItem();

	m_pDestTreeCtrl->MoveBranch(hTDropItem);


	//Remove highlighting
	m_pDestTreeCtrl->SendMessage(TVM_SELECTITEM, TVGN_DROPHILITE,0);

	draging = false;

	return TRUE;
}

DROPEFFECT MTEditDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	DROPEFFECT dropeffectRet = DROPEFFECT_MOVE;
	// Expand and highlight the item under the mouse and 
	m_pDestTreeCtrl = (MTEditTreeCtrl *)pWnd;
	HTREEITEM hTItem = m_pDestTreeCtrl->HitTest(point);
	if ( hTItem != NULL ) 
	{
		m_pDestTreeCtrl->Expand(hTItem, TVE_EXPAND);
		if ( m_pDestTreeCtrl->IsDropAllowed(hTItem))
			m_pDestTreeCtrl->SelectDropTarget(hTItem);
		else
			dropeffectRet = DROPEFFECT_NONE;
	}	
	else
		dropeffectRet = DROPEFFECT_NONE;

	// Scroll Tree control depending on mouse position
	CRect rectClient;
	pWnd->GetClientRect(&rectClient);
	pWnd->ClientToScreen(rectClient);
	pWnd->ClientToScreen(&point);
	int nScrollDir = -1;
	if ( point.y >= rectClient.bottom - RECT_BORDER)
		nScrollDir = SB_LINEDOWN;
	else
		if ( (point.y <= rectClient.top + RECT_BORDER) )
			nScrollDir = SB_LINEUP;


	if ( nScrollDir != -1 ) 
	{
		int nScrollPos = pWnd->GetScrollPos(SB_VERT);
		WPARAM wParam = MAKELONG(nScrollDir, nScrollPos);
		pWnd->SendMessage(WM_VSCROLL, wParam);
	}

	nScrollDir = -1;
	if ( point.x <= rectClient.left + RECT_BORDER )
		nScrollDir = SB_LINELEFT;
	else
		if ( point.x >= rectClient.right - RECT_BORDER)
			nScrollDir = SB_LINERIGHT;

	if ( nScrollDir != -1 ) 
	{
		int nScrollPos = pWnd->GetScrollPos(SB_VERT);
		WPARAM wParam = MAKELONG(nScrollDir, nScrollPos);
		pWnd->SendMessage(WM_HSCROLL, wParam);
	}

	return dropeffectRet;
}

DROPEFFECT MTEditDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	draging = true;
	pWndTreeCtrl->RemoveCutHighlight();
	DROPEFFECT dropeffectRet = DROPEFFECT_MOVE;
	return dropeffectRet;
}

void MTEditDropTarget::OnDragLeave(CWnd* pWnd)
{
	// Remove Highlighting 
	draging = false;
	m_pDestTreeCtrl = (MTEditTreeCtrl *)pWnd;
	m_pDestTreeCtrl->SendMessage(TVM_SELECTITEM, TVGN_DROPHILITE,0);
}
