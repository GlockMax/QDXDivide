// NObjDropTarget.cpp : implementation file
//

#include "stdafx.h"
#include "NCM.h"
#include "LeftView.h"
#include "NObjTreeCtrl.h"
#include "DocElements.h"
#include "NObjDropTarget.h"


// NObjDropTarget

IMPLEMENT_DYNAMIC(NObjDropTarget, COleDropTarget)


NObjDropTarget::NObjDropTarget(class CLeftView *pLV)
{
	pLeftView = pLV;
}

NObjDropTarget::~NObjDropTarget()
{
}


BEGIN_MESSAGE_MAP(NObjDropTarget, COleDropTarget)
END_MESSAGE_MAP()



// NObjDropTarget message handlers

enum DocElements NObjDropTarget::GetElemType(CWnd* pWnd, HTREEITEM & hTItem)
{
	NObjTreeCtrl *pDestTreeCtrl = (NObjTreeCtrl *)pWnd;
	// Find most parent
	for(auto WorkItem = pDestTreeCtrl->GetParentItem(hTItem); WorkItem != NULL; WorkItem = pDestTreeCtrl->GetParentItem(hTItem))
		hTItem = WorkItem;

	return (DocElements)pDestTreeCtrl->GetItemData(hTItem);
}

bool NObjDropTarget::IsItemAcceptable(CWnd* pWnd, CPoint point, enum DocElements & ItemData)
{
	NObjTreeCtrl *pDestTreeCtrl = (NObjTreeCtrl *)pWnd;
	HTREEITEM hTItem = pDestTreeCtrl->HitTest(point);
	if ( hTItem != NULL ) 
	{
		ItemData = GetElemType(pWnd, hTItem);
		if(ItemData == DocElements::NStock || ItemData == NStocksList || ItemData == NProgramList || ItemData == NProgramListAux || ItemData == NPartList || ItemData == NExObjList)
		{
			pDestTreeCtrl->SelectDropTarget(hTItem);
			return true;
		}
	}	
	return false;
}
DROPEFFECT NObjDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	NObjTreeCtrl *pDestTreeCtrl = (NObjTreeCtrl *)pWnd;
	DocElements ItemData;
	if(IsItemAcceptable(pWnd, point, ItemData))
	{
		HTREEITEM hTItem = pDestTreeCtrl->HitTest(point);
		
		return DROPEFFECT_COPY;
	}
	pDestTreeCtrl->SendMessage(TVM_SELECTITEM, TVGN_DROPHILITE,0);
	return DROPEFFECT_NONE;
}

BOOL NObjDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject,	DROPEFFECT dropEffect, CPoint point)
{
	NObjTreeCtrl *pDestTreeCtrl = (NObjTreeCtrl *)pWnd;
	pDestTreeCtrl->SendMessage(TVM_SELECTITEM, TVGN_DROPHILITE,0);

	if(dropEffect != DROPEFFECT_COPY)
		return FALSE;

	DocElements ItemData;
	if(!IsItemAcceptable(pWnd, point, ItemData))
		return FALSE;

	STGMEDIUM rSM;
    BOOL fRet= pDataObject->GetData( CF_HDROP, &rSM, 0 );

    HDROP hDropInfo = (HDROP)rSM.hGlobal;
	int nCntFiles= DragQueryFile( hDropInfo, -1, 0, 0 );
	if (pLeftView)
	{
		pLeftView->names.resize(nCntFiles);
		for ( int j = 0; j < nCntFiles; ++j )
		{
			char szBuf[MAX_PATH];
			::DragQueryFile( hDropInfo, j, szBuf, sizeof(szBuf) );
			pLeftView->names[j] = szBuf;
		}
		pLeftView->PostMessage(WM_APP_LEFT_DROP, ItemData);
	}
	pDestTreeCtrl->SendMessage(TVM_SELECTITEM, TVGN_DROPHILITE,0);
    return TRUE;
}


void NObjDropTarget::OnDragLeave(CWnd* pWnd)
{
	NObjTreeCtrl *pDestTreeCtrl = (NObjTreeCtrl *)pWnd;
	pDestTreeCtrl->SendMessage(TVM_SELECTITEM, TVGN_DROPHILITE,0);

	COleDropTarget::OnDragLeave(pWnd);
}
