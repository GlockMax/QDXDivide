
#include "stdafx.h"
#include "NCUnitTreeCtrl.h"
#include "NCUnitTreePane.h"
#include "NCEditor2Doc.h"
#include "resource.h"
#include "NCAddWReplD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNCUnitTreeCtrl

CNCUnitTreeCtrl::CNCUnitTreeCtrl()
{
	InternalSelect = false;
	DragIL = NULL;
	hPopup = NULL;
}

CNCUnitTreeCtrl::~CNCUnitTreeCtrl()
{
}

BEGIN_MESSAGE_MAP(CNCUnitTreeCtrl, CTreeCtrlColor)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CNCUnitTreeCtrl::OnTvnSelchanged)
	ON_WM_LBUTTONDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_CONTEXTMENU()
	ON_NOTIFY_REFLECT(NM_RCLICK, &CNCUnitTreeCtrl::OnNMRClick)
	ON_COMMAND(ID_ADD_WREPL, &CNCUnitTreeCtrl::OnAddWrepl)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, &CNCUnitTreeCtrl::OnTvnBegindrag)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_DEL_WREPL, &CNCUnitTreeCtrl::OnDelWrepl)
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CNCUnitTreeCtrl, CTreeCtrlColor)

/////////////////////////////////////////////////////////////////////////////
// CNCUnitTreeCtrl message handlers

BOOL CNCUnitTreeCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != NULL);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != NULL)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	return bRes;
}

BOOL CNCUnitTreeCtrl::DeleteAllItems()
{
	class local
	{
	public:
		static void FreeItemData(CTreeCtrl *tree, HTREEITEM item)
		{
			/// Освобождаем память в узле
			NCTreeItemData * iData;
			iData = (NCTreeItemData*)tree->GetItemData(item);
			if(iData != NULL)
			{
				delete iData;
				iData = NULL;
			}//if

			/// Освобождаем память в потомках
			HTREEITEM hChild = tree->GetChildItem(item);
			if(hChild != NULL)
				FreeItemData(tree, hChild);

			/// Освобождаем память в соседях
			HTREEITEM hSibling = tree->GetNextSiblingItem(item);

			if(hSibling != NULL)
				FreeItemData(tree,hSibling);
		};
	};
	/// Освобождаем память из под динамических данных
	HTREEITEM hRoot = GetRootItem();
	if(hRoot != NULL)
		local::FreeItemData(this, GetRootItem());

	/// Вызываем удаление всех элементов из родительского класса
	return CTreeCtrl::DeleteAllItems();
}//BOOL CNCUnitTreeCtrl::DeleteAllItems()

void CNCUnitTreeCtrl::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	if(InternalSelect)
		return;

	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	HTREEITEM hSelItem = pNMTreeView->itemNew.hItem;
	NCTreeItemData *pData = (NCTreeItemData*)GetItemData(hSelItem);

	CWnd *Parent = GetParent();

	if(pData != NULL)
	switch (pData->type)
	{
	case I_SECTION:
		((CNCUnitTreePane*)Parent)->GetDocument()->ShowSectionProp(GetItemText(hSelItem));
		break;
	case I_DEFINITION:
		{
			NCTreeItemData *pData;
			pData = (NCTreeItemData*)GetItemData(hSelItem);
			if(pData->def.known)
				((CNCUnitTreePane*)Parent)->GetDocument()->ShowDefinitionProp(pData->def.sectName,GetItemText(hSelItem));
			else
				((CNCUnitTreePane*)Parent)->GetDocument()->ShowDefinitionUnknownProp(pData->def.sectName,GetItemText(hSelItem));
			break;
		}
	case I_WREPL:
		{
			CString srcString = pData->wrep.srcStr;
			CString dstString = pData->wrep.dstStr;
			((CNCUnitTreePane*)Parent)->GetDocument()->ShowWReplProp(srcString, dstString);
		}
		break;
	case I_GROUP:
		{
			NCTreeItemData *pData;
			pData = (NCTreeItemData*)GetItemData(hSelItem);
			((CNCUnitTreePane*)Parent)->GetDocument()->ShowGroupProp(pData->group.sectName, GetItemText(hSelItem));
		}
		break;
	}//switch
	
	//((CNCUnitTreePane*)Parent)->GetDocument()->test();

	*pResult = 0;
}

void CNCUnitTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CTreeCtrlColor::OnLButtonDown(nFlags, point);

	UINT flags;
	HTREEITEM hItem = HitTest(point, &flags);
	if(hItem == NULL)
		return;

	if(flags == TVHT_ONITEMICON)
	{
		NCTreeItemData Data = *(NCTreeItemData*)GetItemData(hItem);
		if(Data.type == I_DEFINITION)
		{
			if(Data.def.known == false)
				return;

			CString defName = GetItemText(hItem);
			CString sectName = Data.def.sectName;
			CWnd * Parent = GetParent();
			((CNCUnitTreePane*)Parent)->GetDocument()->ChangeDefPresence(sectName,defName);
		}//if
	}//if

}

void CNCUnitTreeCtrl::OnKillFocus(CWnd* pNewWnd)
{
	CTreeCtrlColor::OnKillFocus(pNewWnd);

	// TODO: Add your message handler code here
}

/// http://support.microsoft.com/kb/222905
void CNCUnitTreeCtrl::OnContextMenu(CWnd* pWnd, CPoint ptMousePos)
{
	/// if Shift-F10
	if (ptMousePos.x == -1 && ptMousePos.y == -1)
		ptMousePos = (CPoint) GetMessagePos();

	ScreenToClient(&ptMousePos);

	UINT uFlags;
	
	hPopup = HitTest( ptMousePos, &uFlags );

	if( hPopup == NULL )
		return;
	
	m_hActiveItem = hPopup;

	CMenu menu;
	CMenu* pPopup;

	// the font popup is stored in a resource
	menu.LoadMenu(IDR_TREE_CMENU);
	pPopup = menu.GetSubMenu(0);

	NCTreeItemData *pData = (NCTreeItemData*)GetItemData(hPopup);
	if(pData->type != I_WREPL)
		pPopup->EnableMenuItem(ID_DEL_WREPL, MF_GRAYED);
	ClientToScreen(&ptMousePos);
	pPopup->TrackPopupMenu( TPM_LEFTALIGN, ptMousePos.x, ptMousePos.y, this );
}
/// http://support.microsoft.com/kb/222905
void CNCUnitTreeCtrl::OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// Send WM_CONTEXTMENU to self
	SendMessage(WM_CONTEXTMENU, (WPARAM) m_hWnd, GetMessagePos());
	*pResult = 1;
}

void CNCUnitTreeCtrl::OnAddWrepl()
{
	CNCAddWReplD dlg;
	if(dlg.DoModal() == IDOK)
	{
		CWnd * Parent = GetParent();
		((CNCUnitTreePane*)Parent)->GetDocument()->AddWRepl(dlg.srcString, dlg.dstString);
	}//if
}

void CNCUnitTreeCtrl::OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	unsigned int flags;
	HTREEITEM CurItem = HitTest(pNMTreeView->ptDrag, &flags);
	HTREEITEM hParent = GetParentItem(CurItem);
	if(hParent != NULL)
	{
		NCTreeItemData *pData = (NCTreeItemData*)GetItemData(hParent);
		if(pData->type == I_SECTION)
		{
			CString str = GetItemText(hParent);
			if(str == _T("Word Replacement"))
			{
				DragIL = CreateDragImage(CurItem);
				hSource = CurItem;
				SetCapture();
			}//if
		}//if
	}//if

	*pResult = 0;
}

void CNCUnitTreeCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	MousePos = point;
	if(nFlags & MK_LBUTTON)
		if(DragIL != NULL)
		{
			HTREEITEM htiTarget; /// handle to target item

			if((htiTarget = HitTest(point)) != NULL)
			{
				HTREEITEM hParent = GetParentItem(htiTarget);
				if(hParent != NULL)
				{
					NCTreeItemData *pData = (NCTreeItemData*)GetItemData(hParent);
					if(pData->type == I_SECTION)
					{
						CString str = GetItemText(hParent);
						if(str == _T("Word Replacement"))
						{
							SetInsertMark(htiTarget, FALSE);
							hTarget = htiTarget;
						}//if
					}//if
				}//if
			}//if
		}//if

	CTreeCtrlColor::OnMouseMove(nFlags, point);
}

void CNCUnitTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(DragIL != NULL)
	{
		delete DragIL;
		SetInsertMark(NULL);
		/// move...
		if(hSource != hTarget)
		{
			/// 1 - Получение srcsrcString, srcdstString, dstsrcString, dstdstString
			NCTreeItemData *pSrcData = (NCTreeItemData*)GetItemData(hSource);	
			ASSERT(pSrcData->type == I_WREPL);
			
			NCTreeItemData *pDstData = (NCTreeItemData*)GetItemData(hTarget);			
			ASSERT(pDstData->type == I_WREPL);

			CString srcsrcString = pSrcData->wrep.srcStr;
			CString srcdstString = pSrcData->wrep.dstStr;
			CString dstsrcString = pDstData->wrep.srcStr;
			CString dstdstString = pDstData->wrep.dstStr;
			/// 2 - Перемещение wrepl
			CWnd * Parent = GetParent();
			((CNCUnitTreePane*)Parent)->GetDocument()->MoveWRepl(srcsrcString, srcdstString, dstsrcString, dstdstString);
		}//if
		hSource = NULL;
		hTarget = NULL;
		ReleaseCapture();
	}//if
	DragIL = NULL;

	CTreeCtrlColor::OnLButtonUp(nFlags, point);
}

BOOL CNCUnitTreeCtrl::DestroyWindow()
{
	DeleteAllItems();

	return CTreeCtrlColor::DestroyWindow();
}

void CNCUnitTreeCtrl::OnDelWrepl()
{
	NCTreeItemData *pData = (NCTreeItemData*)GetItemData(hPopup);
	ASSERT(pData->type == I_WREPL);
	if(pData->type != I_WREPL)
		return;

	CWnd * Parent = GetParent();
	((CNCUnitTreePane*)Parent)->GetDocument()->DelWRepl(pData->wrep.srcStr, pData->wrep.dstStr);
}
