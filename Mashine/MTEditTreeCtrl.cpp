#pragma once

#include "stdafx.h"
#include "DocElements.h"
#include "NOCImport.h"
#include "MTEditTreeCtrl.h"
#include "MTEditTreePane.h"
#include "MTEditStlNamesDlg.h"
#include "BFileDialogWPath.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// MTEditTreeCtrl

MTEditTreeCtrl::MTEditTreeCtrl()
{
	hDraged = NULL;
	hContextMenu = NULL;

	m_CTreeDropTarget.pWndTreeCtrl = this;
}

MTEditTreeCtrl::~MTEditTreeCtrl()
{
}

void MTEditTreeCtrl::ExpandCollapseAll()
{
	HTREEITEM hRoot = GetRootItem();
	/// Имеет смысл выполнять сворачивание/разворачивание дерева только если у корня есть потомки
	if(ItemHasChildren(hRoot))
	{
		Expand(hRoot, TVE_EXPAND);
		HTREEITEM hChild;
		hChild = GetChildItem(hRoot);
		HTREEITEM hCur = hChild;
		for( ;
			 !ItemHasChildren(hCur);
			 hCur = GetNextItem(hCur, TVGN_NEXT));
		{
			if(!hCur)
				return;
		}//for

		hChild = hCur;

		/// Если первый потомок корня развернут, свернуть всё дерево до первого уровня
		if( GetItemState( hChild, TVIS_EXPANDED) & TVIS_EXPANDED )
		{
			while (hChild)
			{
				Expand(hChild, TVE_COLLAPSE);
				hChild = GetNextItem(hChild, TVGN_NEXT);
			}//while
		}//if
		/// Иначе, развернуть всё дерево
		else
		{
			ExpandCollapseAllInternal(hRoot, TVE_EXPAND);
		}//else
	}//if
}//void MTEditTreeCtrl::ExpandAll()

void MTEditTreeCtrl::ExpandAll()
{
	HTREEITEM hRoot = GetRootItem();
	ExpandCollapseAllInternal(hRoot, TVE_EXPAND);
}//void MTEditTreeCtrl::ExpandAll()

void MTEditTreeCtrl::ExpandCollapseAllInternal(HTREEITEM hItem, UINT nCode)
{
	HTREEITEM hChild;

	if(ItemHasChildren(hItem))
	{
		Expand(hItem, nCode);
		hChild = GetChildItem(hItem);

		while(hChild)
		{
			ExpandCollapseAllInternal(hChild, nCode);
			hChild = GetNextItem(hChild, TVGN_NEXT);
		}
	}
}//void MTEditTreeCtrl::ExpandAllInternal(HTREEITEM hItem, UINT nCode)


BEGIN_MESSAGE_MAP(MTEditTreeCtrl, CTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &MTEditTreeCtrl::OnTvnSelchanged)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
	ON_NOTIFY_REFLECT(TVN_SELCHANGING, &MTEditTreeCtrl::OnTvnSelchanging)
	ON_COMMAND(IDR_MTEDIT_SHOWPROP, OnMTEditShowProp)
	ON_COMMAND(IDR_MTEDIT_CUT, OnCMCut)
	ON_COMMAND(IDR_MTEDIT_PASTE, OnCMPaste)
	ON_COMMAND(IDR_MTEDIT_ADD_TRANS, OnCMCreateTrans)
	ON_COMMAND(IDR_MTEDIT_CREATE_CHAIN_MECHANISM, OnCMCreateSChain)
	ON_COMMAND(IDR_MTEDIT_ADD_ROT, OnCMCreateRot)
	ON_COMMAND(IDR_MTEDIT_CREATE_AXIS_ARRAY, OnCMCreateArray)
	ON_COMMAND(IDR_MTEDIT_ADD_PARTS, OnCMCreateParts)
	ON_COMMAND(IDR_MTEDIT_ADD_3DTOSTL, OnCMCreateParts3D)
	ON_COMMAND(IDR_MTEDIT_ADD_TOOL, OnCMCreateTool)
	ON_COMMAND(IDR_MTEDIT_ADD_VIRTTOOL, OnCMCreateToolVirt)
	ON_COMMAND(IDR_MTEDIT_ADD_STOCK, OnCMCreateStock)
	ON_COMMAND(IDR_MTEDIT_ADD_COLLISION, OnCMCreateCol)
	ON_COMMAND(IDR_MTEDIT_DELETE, OnCMDeleteBranch)
	ON_COMMAND(IDR_MTEDIT_SAVE_BRANCH, OnCMSaveBranch)
	ON_COMMAND(IDR_MTEDIT_ADD_X, OnCMCreateX)
	ON_COMMAND(IDR_MTEDIT_ADD_Y, OnCMCreateY)
	ON_COMMAND(IDR_MTEDIT_ADD_Z, OnCMCreateZ)
	ON_COMMAND(IDR_MTEDIT_ADD_A, OnCMCreateA)
	ON_COMMAND(IDR_MTEDIT_ADD_B, OnCMCreateB)
	ON_COMMAND(IDR_MTEDIT_ADD_C, OnCMCreateC)
	ON_COMMAND(IDR_MTEDIT_ADD_SPIN, OnCMCreateSpin)
	ON_WM_CONTEXTMENU()
	ON_WM_KEYDOWN()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MTEditTreeCtrl message handlers

BOOL MTEditTreeCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
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
void MTEditTreeCtrl::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	if(m_CTreeDropTarget.draging)
	{
		*pResult = 0;
		return;
	}//if
	
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	
	HTREEITEM currItem = GetSelectedItem();
	CString name = GetItemText(currItem);

	//1 - Определяем тип объекта
	DWORD_PTR DType = GetItemData(currItem);

	ASSERT(DType == 1 || DType ==0 || DType == 2);

	CWnd *Parent = GetParent();
	
	if(DType == 1)
	{
		//Ось
		
		((MTEditTreePane*)Parent)->ShowAxisProp(name);
	}//if
	if(DType == 0)
	{
		//Объект столкновения
		//1 - Определяем имя оси (текст родительского узла)
		HTREEITEM axisItem = GetParentItem(currItem);
		CString myName;
		myName = GetItemText(axisItem);

		//2 - Определяем имя оси для контроля столкновения
		CString axisName;

		CString ColString;
		ColString.LoadString(IDS_COLLISION_STRING);

		axisName = name.Mid(ColString.GetLength());

		//3 - Показываем свойства объекта столкновения

		((MTEditTreePane*)Parent)->ShowColProp(myName, axisName);
	}//else
	if(DType == 2) //корневой узел
	{
		((MTEditTreePane*)Parent)->ShowEmptyProp();
	}//if

	*pResult = 0;
}

//Дерево не поддерживает поиск по имени ни в на уровне WinApi, ни в MFC
//http://support.microsoft.com/kb/155895
HTREEITEM MTEditTreeCtrl::GetItemByName(HTREEITEM hItem, LPCTSTR szItemName) const
{
#define MAXTEXTLEN 100
	HWND hWnd = this->GetSafeHwnd();
    // If hItem is NULL, start search from root item.
    if (hItem == NULL)
        hItem = (HTREEITEM)SendMessage(TVM_GETNEXTITEM, TVGN_ROOT, 0);
    while (hItem != NULL)
    {
        char szBuffer[MAXTEXTLEN+1];
        TV_ITEM item;

        item.hItem = hItem;
        item.mask = TVIF_TEXT | TVIF_CHILDREN;
        item.pszText = szBuffer;
        item.cchTextMax = MAXTEXTLEN;   //Вот этот параметр не даст возникнуть переполнению буфера
        SendMessage(TVM_GETITEM, 0, (LPARAM)&item);

        // Did we find it?
        if (lstrcmp(szBuffer, szItemName) == 0)
            return hItem;

        // Check whether we have child items.
        if (item.cChildren)
        {
            // Recursively traverse child items.
            HTREEITEM hItemFound, hItemChild;

            hItemChild = (HTREEITEM)SendMessage(TVM_GETNEXTITEM,
                                                TVGN_CHILD, (LPARAM)hItem);
            hItemFound = GetItemByName(hItemChild, szItemName);

            // Did we find it?
            if (hItemFound != NULL)
                return hItemFound;
        }

        // Go to next sibling item.
        hItem = (HTREEITEM)SendMessage(TVM_GETNEXTITEM,
                                       TVGN_NEXT, (LPARAM)hItem);
    }

    // Not found.
    return NULL;
}

CString MTEditTreeCtrl::GetAxis1ofCollision(HTREEITEM hItem)
{
	//Определяем имя оси (текст родительского узла)
	HTREEITEM axisItem = GetParentItem(hItem);
	CString Axis1;
	Axis1 = GetItemText(axisItem);

	return Axis1;
}//CString& MTEditTreeCtrl::GetAxis1ofCollision(HTREEITEM hItem)

CString MTEditTreeCtrl::GetAxis2ofCollision(HTREEITEM hItem)
{
	CString Axis2;
	CString Col = GetItemText(hItem);

	CString ColString;
	ColString.LoadString(IDS_COLLISION_STRING);

	Axis2 = Col.Mid(ColString.GetLength());

	return Axis2;
}//CString& MTEditTreeCtrl::GetAxis2ofCollision(HTREEITEM hItem)

void MTEditTreeCtrl::InsertCollision(HTREEITEM hParent, const CString &Axis2)
{
	CString ColString;
	ColString.LoadString(IDS_COLLISION_STRING);
	ColString = ColString + Axis2;
	HTREEITEM Col = InsertItem(ColString,0,0,hParent,TVI_FIRST);
	SetItemData(Col, (DWORD_PTR)0);
}//void MTEditTreeCtrl::InsertCollision(HTREEITEM hParent, CString &Axis2)

void MTEditTreeCtrl::MoveBranch(HTREEITEM hDest)
{
	MoveBranch(hDraged, hDest);
	return;

	ASSERT(hDest != NULL);

	// 1 - обрабатываем перемещение объекта столкновения
	CWnd *Parent = GetParent();

	if(GetItemData(hDraged) == 0) //перетаскиваем объект столкновения
	{
		
		CString Axis1,Axis2,NewAxis1;
		Axis1 = GetAxis1ofCollision(hDraged);
		Axis2 = GetAxis2ofCollision(hDraged);

		NewAxis1 = GetItemText(hDest);
							
		((MTEditTreePane*)Parent)->MoveCollision(Axis1, Axis2, NewAxis1);
		return;
	}//if
	else    //Перетаскиваем ветвь
	{
		// 2 - Вызываем функцию перемещения ветви

			// 1 - Перемещение в корень дерева
		if(hDest == GetRootItem())
		{
			((MTEditTreePane*)Parent)->MoveBranch(GetItemText(hDraged),CString(""));
		}

			// 2 - Общий случай
		else
			((MTEditTreePane*)Parent)->MoveBranch(GetItemText(hDraged),GetItemText(hDest));
	}//else

}//void MTEditTreeCtrl::MoveBranch(HTREEITEM hBase, HTREEITEM hDest)

void MTEditTreeCtrl::MoveBranch(HTREEITEM hSrc, HTREEITEM hDest)
{
	ASSERT(hDest != NULL);

	// 1 - обрабатываем перемещение объекта столкновения
	CWnd *Parent = GetParent();

	if(GetItemData(hSrc) == 0) //перетаскиваем объект столкновения
	{
		
		CString Axis1,Axis2,NewAxis1;
		Axis1 = GetAxis1ofCollision(hDraged);
		Axis2 = GetAxis2ofCollision(hDraged);

		NewAxis1 = GetItemText(hDest);
							
		((MTEditTreePane*)Parent)->MoveCollision(Axis1, Axis2, NewAxis1);
		return;
	}//if
	else    //Перетаскиваем ветвь
	{
		// 2 - Вызываем функцию перемещения ветви

			// 1 - Перемещение в корень дерева
		if(hDest == GetRootItem())
		{
			((MTEditTreePane*)Parent)->MoveBranch(GetItemText(hSrc),CString(""));
		}

			// 2 - Общий случай
		else
			((MTEditTreePane*)Parent)->MoveBranch(GetItemText(hSrc),GetItemText(hDest));
	}//else

}//void MTEditTreeCtrl::MoveBranch(HTREEITEM hBase, HTREEITEM hDest)

void MTEditTreeCtrl::MoveBrahcnCtrl(const CString& AxisName, const CString& DestName)
{
	HTREEITEM hAxis, hDest;
	hAxis = GetItemByName(NULL, AxisName);
	hDest = GetItemByName(NULL, DestName);
	
	if(DestName == _T(""))
		hDest = GetRootItem();

	int BranchAmount = GetBranchAmount(hAxis);

	TNode *Branch = new TNode [BranchAmount];

	int pos = 0;
	GetBranch(Branch, pos, hAxis);
	Branch[0].Parent = GetItemText(hDest);
		// 2 - Удаляем ветвь
	DeleteItem(hAxis);
		// 3 - Подвешиваем ветвь в новое место
	for(int i = 0; i < BranchAmount; i++)
	{
		HTREEITEM hParent = GetItemByName(NULL, Branch[i].Parent);
		CString sp = GetItemText(hParent);
		HTREEITEM hItem;
		if(Branch[i].Image == 3 || Branch[i].Image == 4 || Branch[i].Image == 5 || Branch[i].Image == 6) //Part nodes first
			hItem = InsertItem(Branch[i].Name, Branch[i].Image, Branch[i].SelImage, hParent, TVI_FIRST);
		else
			hItem = InsertItem(Branch[i].Name, Branch[i].Image, Branch[i].SelImage, hParent);
		CString ch = GetItemText(hItem);
		SetItemData(hItem, Branch[i].Data);

	}//for

	delete [] Branch;


}//void MTEditTreeCtrl::MoveBrahcnCtrl(CString AxisName, CString DestName)

void MTEditTreeCtrl::MoveCollisionCtrl(const CString& Axis1, const CString& Axis2, const CString& NewAxis1)
{
	CString ColString;
	ColString.LoadString(IDS_COLLISION_STRING);
	HTREEITEM hAxis1 = GetItemByName(NULL, Axis1);
	ASSERT(hAxis1 != NULL);

	CString sCol = ColString+Axis2;

	HTREEITEM hCol = GetItemByName(hAxis1, sCol);
	ASSERT(hCol != NULL);

	HTREEITEM hNewAxis1 = GetItemByName(NULL, NewAxis1);
	ASSERT(hNewAxis1 != NULL);

	InsertCollision(hNewAxis1, Axis2);
	DeleteItem(hCol);
	return;
}//void MTEditTreeCtrl::MoveCollisionCtrl(CString Axis1, CString Axis2, CString NewAxis1) 

int MTEditTreeCtrl::GetBranchAmount(HTREEITEM hItem)
{
	int amount = 1;

	HTREEITEM hChild = GetNextItem(hItem,TVGN_CHILD);
	if(hChild == NULL)
		return amount;

	while(hChild != NULL)
	{
		amount = amount + GetBranchAmount(hChild);
		hChild = GetNextSiblingItem(hChild);
	}//while

	return amount;
}//int MTEditTreeCtrl::GetBranchAmount(HTREEITEM hItem)

void MTEditTreeCtrl::GetBranch(TNode * Branch, int & pos, HTREEITEM hBase)
{
	//Записываем себя в массив
	Branch[pos].Name = GetItemText(hBase);
	Branch[pos].Parent = GetItemText(GetParentItem(hBase));
	Branch[pos].Data = GetItemData(hBase);
	int image, sel_image;
	GetItemImage(hBase, image, sel_image);
	Branch[pos].Image = image;
	Branch[pos].SelImage = sel_image;

	pos++;

	HTREEITEM hChild = GetNextItem(hBase, TVGN_CHILD);
	while(hChild != NULL)
	{
		GetBranch(Branch, pos, hChild);
		hChild = GetNextSiblingItem(hChild);
	}//while

	return;
}//void MTEditTreeCtrl::GetBranch(TNode * Branch, int pos, HTREEITEM hBase)

void MTEditTreeCtrl::ShowItemProp(HTREEITEM hItem)
{
	CString name = GetItemText(hItem);

	//1 - Определяем тип объекта
	DWORD_PTR DType = GetItemData(hItem);

	ASSERT(DType == 1 || DType ==0 || DType == 2);

	CWnd *Parent = GetParent();
	
	if(DType == 1)
	{
		//Ось
		
		((MTEditTreePane*)Parent)->ShowAxisProp(name);
	}//if
	if(DType == 0)
	{
		//Объект столкновения
		//1 - Определяем имя оси (текст родительского узла)
		HTREEITEM axisItem = GetParentItem(hItem);
		CString myName;
		myName = GetItemText(axisItem);

		//2 - Определяем имя оси для контроля столкновения
		CString axisName;

		CString ColString;
		ColString.LoadString(IDS_COLLISION_STRING);

		axisName = name.Mid(ColString.GetLength());

		//3 - Показываем свойства объекта столкновения

		((MTEditTreePane*)Parent)->ShowColProp(myName, axisName);
	}//if
	if(DType == 2)
	{
		((MTEditTreePane*)Parent)->ShowEmptyProp();
	}//if
}//void MTEditTreeCtrl::ShowItemProp(HTREEITEM hItem)

HTREEITEM MTEditTreeCtrl::GetCutNode(HTREEITEM hNode)
{
	HTREEITEM hItem;
	HTREEITEM hRes;
	// 1 - если NULL, начинаем поиск с корня
	if( hNode == NULL)
		hItem = GetRootItem();
	else
		hItem = hNode;
	// 2 - проверяем узел на соответствие
	if (TVIS_CUT & GetItemState(hItem, TVIS_CUT))
		return hItem;
	// 3 - находим ребенка
	HTREEITEM hChild = GetNextItem(hItem, TVGN_CHILD);
	if(hChild != NULL)
	{
	// 4 - Вызываем функцию для всех непосредственных потомков
		while(hChild != NULL)
		{
			hRes = GetCutNode(hChild);
			if(hRes != NULL)
				return hRes;
			hChild = GetNextSiblingItem(hChild);
		}//while
	}//if

	// 5 - неудача, возвращаем NULL
	return NULL;
}//HTREEITEM MTEditTreeCtrl::GetCutNode(HTREEITEM hNode)

void MTEditTreeCtrl::RemoveCutHighlight()
{
	HTREEITEM hItem = GetCutNode();
	SetItemState(hItem, !TVIS_CUT, TVIS_CUT);
}//void MTEditTreeCtrl::RemoveCutHighlight()

void MTEditTreeCtrl::OnMTEditShowProp()
{
	CWnd *Parent = GetParent();
	((MTEditTreePane*)Parent)->ShowPropPane();
	
	ShowItemProp(hContextMenu);

	hContextMenu = NULL;
}//void MTEditTreeCtrl::OnMTEditShowProp()

void MTEditTreeCtrl::OnCMPaste()
{
	HTREEITEM hItem = GetCutNode();
	if(hItem == NULL)
		return;
	if(hItem == hContextMenu)
	{
		RemoveCutHighlight();
		return;
	}//if
	if(IsMoveAllowed(hItem, hContextMenu))
	{
		RemoveCutHighlight();
		MoveBranch(hItem, hContextMenu);		
	}//if
}//void MTEditTreeCtrl::OnCMPaste()

void MTEditTreeCtrl::OnCMCut()
{
	RemoveCutHighlight();
	SetItemState(hContextMenu, TVIS_CUT, TVIS_CUT);
}//void MTEditTreeCtrl::OnCMCut()

void MTEditTreeCtrl::OnCMCreateTrans()
{
	CWnd *Parent = GetParent();
	switch(GetItemData(hContextMenu))
	{
	case 0:
		return;
		break;
	case 1:
		((MTEditTreePane*)Parent)->CreateTrans(GetItemText(hContextMenu));
		break;
	case 2:
		((MTEditTreePane*)Parent)->CreateTrans(_T(""));
		break;
	}//switch
}
void MTEditTreeCtrl::OnCMCreateSChain()
{
	CWnd* Parent = GetParent();
	switch (GetItemData(hContextMenu))
	{
	case 0:
		return;
		break;
	case 1:
		((MTEditTreePane*)Parent)->CreateSChain(GetItemText(hContextMenu));
		break;
	case 2:
		((MTEditTreePane*)Parent)->CreateSChain(_T(""));
		break;
	}//switch
}
void MTEditTreeCtrl::OnCMCreateArray()
{
	CWnd* Parent = GetParent();
	switch (GetItemData(hContextMenu))
	{
	case 0:
		return;
		break;
	case 1:
		((MTEditTreePane*)Parent)->CreateArray(GetItemText(hContextMenu));
		break;
	case 2:
		return;
		break;
	}//switch
}
//void MTEditTreeCtrl::OnCMCreateTrans()

void MTEditTreeCtrl::OnCMCreateRot()
{
	CWnd *Parent = GetParent();
	switch(GetItemData(hContextMenu))
	{
	case 0:
		return;
		break;
	case 1:
		((MTEditTreePane*)Parent)->CreateRot(GetItemText(hContextMenu));
		break;
	case 2:
		((MTEditTreePane*)Parent)->CreateRot(_T(""));
		break;
	}//switch
}//void MTEditTreeCtrl::OnCMCreateRot()
void MTEditTreeCtrl::OnCMCreatePart()
{
	CWnd *Parent = GetParent();
	switch(GetItemData(hContextMenu))
	{
	case 0:
		return;
		break;
	case 1:
		((MTEditTreePane*)Parent)->CreatePart(GetItemText(hContextMenu));
		break;
	case 2:
		((MTEditTreePane*)Parent)->CreatePart(_T(""));
		break;
	}//switch
}//void MTEditTreeCtrl::OnCMCreatePart()

void MTEditTreeCtrl::OnCMCreateTool()
{
	CWnd *Parent = GetParent();
	switch(GetItemData(hContextMenu))
	{
	case 0:
		return;
		break;
	case 1:
		((MTEditTreePane*)Parent)->CreateTool(GetItemText(hContextMenu));
		break;
	case 2:
		((MTEditTreePane*)Parent)->CreateTool(_T(""));
		break;
	}//switch
}//void MTEditTreeCtrl::OnCMCreateTool()

void MTEditTreeCtrl::OnCMCreateToolVirt()
{
	CWnd *Parent = GetParent();
	switch(GetItemData(hContextMenu))
	{
	case 0:
		return;
		break;
	case 1:
		((MTEditTreePane*)Parent)->CreateToolVirt(GetItemText(hContextMenu));
		break;
	case 2:
		((MTEditTreePane*)Parent)->CreateToolVirt(_T(""));
		break;
	}//switch
}//void MTEditTreeCtrl::OnCMCreateToolVirt()

void MTEditTreeCtrl::OnCMCreateStock()
{
	CWnd *Parent = GetParent();
	switch(GetItemData(hContextMenu))
	{
	case 0:
		return;
		break;
	case 1:
		((MTEditTreePane*)Parent)->CreateStock(GetItemText(hContextMenu));
		break;
	case 2:
		((MTEditTreePane*)Parent)->CreateStock(_T(""));
		break;
	}//switch
}//void MTEditTreeCtrl::OnCMCreateStock()

void MTEditTreeCtrl::OnCMCreateCol()
{
	CWnd *Parent = GetParent();
	switch(GetItemData(hContextMenu))
	{
	case 0:
		return;
		break;
	case 1:
		((MTEditTreePane*)Parent)->CreateCol(GetItemText(hContextMenu));
		break;
	case 2:
		return;
		break;
	}//switch
}//void MTEditTreeCtrl::OnCMCreateCol()

void MTEditTreeCtrl::OnCMCreateX()
{
	CWnd *Parent = GetParent();
	switch(GetItemData(hContextMenu))
	{
	case 0:
		return;
		break;
	case 1:
		((MTEditTreePane*)Parent)->CreateX(GetItemText(hContextMenu));
		break;
	case 2:
		((MTEditTreePane*)Parent)->CreateX(_T(""));
		break;
	}//switch
}//void MTEditTreeCtrl::OnCMCreateX()

void MTEditTreeCtrl::OnCMCreateY()
{
	CWnd *Parent = GetParent();
	switch(GetItemData(hContextMenu))
	{
	case 0:
		return;
		break;
	case 1:
		((MTEditTreePane*)Parent)->CreateY(GetItemText(hContextMenu));
		break;
	case 2:
		((MTEditTreePane*)Parent)->CreateY(_T(""));
		break;
	}//switch
}//void MTEditTreeCtrl::OnCMCreateY()

void MTEditTreeCtrl::OnCMCreateZ()
{
	CWnd *Parent = GetParent();
	switch(GetItemData(hContextMenu))
	{
	case 0:
		return;
		break;
	case 1:
		((MTEditTreePane*)Parent)->CreateZ(GetItemText(hContextMenu));
		break;
	case 2:
		((MTEditTreePane*)Parent)->CreateZ(_T(""));
		break;
	}//switch
}//void MTEditTreeCtrl::OnCMCreateZ()

void MTEditTreeCtrl::OnCMCreateA()
{
	CWnd *Parent = GetParent();
	switch(GetItemData(hContextMenu))
	{
	case 0:
		return;
		break;
	case 1:
		((MTEditTreePane*)Parent)->CreateA(GetItemText(hContextMenu));
		break;
	case 2:
		((MTEditTreePane*)Parent)->CreateA(_T(""));
		break;
	}//switch
}//void MTEditTreeCtrl::OnCMCreateA()

void MTEditTreeCtrl::OnCMCreateB()
{
	CWnd *Parent = GetParent();
	switch(GetItemData(hContextMenu))
	{
	case 0:
		return;
		break;
	case 1:
		((MTEditTreePane*)Parent)->CreateB(GetItemText(hContextMenu));
		break;
	case 2:
		((MTEditTreePane*)Parent)->CreateB(_T(""));
		break;
	}//switch
}//void MTEditTreeCtrl::OnCMCreateB()

void MTEditTreeCtrl::OnCMCreateC()
{
	CWnd* Parent = GetParent();
	switch (GetItemData(hContextMenu))
	{
	case 0:
		return;
		break;
	case 1:
		((MTEditTreePane*)Parent)->CreateC(GetItemText(hContextMenu));
		break;
	case 2:
		((MTEditTreePane*)Parent)->CreateC(_T(""));
		break;
	}//switch
}

void MTEditTreeCtrl::OnCMCreateSpin()
{
	CWnd* Parent = GetParent();
	switch (GetItemData(hContextMenu))
	{
	case 0:
		return;
		break;
	case 1:
		((MTEditTreePane*)Parent)->CreateSpin(GetItemText(hContextMenu));
		break;
	case 2:
		((MTEditTreePane*)Parent)->CreateSpin(_T(""));
		break;
	}//switch
}

void MTEditTreeCtrl::OnCMSaveBranch()
{
	const MTEditTreePane* Parent = static_cast<MTEditTreePane*>(GetParent());
	if (Parent == nullptr)
		return;

	CString AxisName;
	if (GetItemData(hContextMenu) != 2) // root item
		AxisName = GetItemText(hContextMenu);
	Parent->SaveBranch(AxisName);
}

void MTEditTreeCtrl::OnCMDeleteBranch()
{
	CWnd *Parent = GetParent();
	switch(GetItemData(hContextMenu))
	{
	case 0:  //collision
		((MTEditTreePane*)Parent)->DeleteCollision(GetAxis1ofCollision(hContextMenu), GetAxis2ofCollision(hContextMenu));
		break;
	case 1: //axis		
		((MTEditTreePane*)Parent)->DeleteBranch(GetItemText(hContextMenu));
		break;
	}//switch
}

void MTEditTreeCtrl::OnCMCreateParts()
{
	MTEditStlNamesDlg dlg;
	CString MachinePath, MachineName;
	CWnd *Parent = GetParent();
	((MTEditTreePane*)Parent)->GetMachinePath(MachinePath, MachineName);
	if(dlg.DoModal(MachinePath + MachineName + "\\")== IDOK)
	{
		CString AxisName;
		switch(GetItemData(hContextMenu))
		{
		case 0:
			return;
			break;
		case 1:
			AxisName = GetItemText(hContextMenu);
			break;
		case 2:
			AxisName = _T("");
			break;
		}
		((MTEditTreePane*)Parent)->CreateParts(AxisName, dlg.stringList);
	}
}

void MTEditTreeCtrl::OnCMCreateParts3D()
{
	CString MachinePath, MachineName;
	CWnd* Parent = GetParent();
	((MTEditTreePane*)Parent)->GetMachinePath(MachinePath, MachineName);
	CString filter;
	filter.LoadString(IDS_PART_FILTER);
	CFileDialog fd(TRUE, (LPCSTR)0, (LPCSTR)0, 6UL, filter);
	CString InitPath = MachinePath + MachineName + '\\';
	// Update the m_ofn variable
	fd.UpdateOFNFromShellDialog();
	fd.m_ofn.lpstrInitialDir = InitPath;
	fd.m_ofn.Flags = fd.m_ofn.Flags | OFN_ALLOWMULTISELECT;
	CHAR buf[4096] = "";
	fd.m_ofn.lpstrFile = buf;
	fd.m_ofn.nMaxFile = 4096;
	fd.ApplyOFNToShellDialog();
	if (fd.DoModal() != IDOK)
		return;
	std::vector<CString> names;
	for (POSITION pos = fd.GetStartPosition(); pos != NULL;)
	{
		names.push_back(fd.GetNextPathName(pos));
	}
	CString AxisName;
	switch (GetItemData(hContextMenu))
	{
	case 0:
		return;
		break;
	case 1:
		AxisName = GetItemText(hContextMenu);
		break;
	case 2:
		AxisName = _T("");
		break;
	}
	for (auto it = names.begin(); it != names.end(); ++it)
	{
		const StockFileType StockType = MTEdit::Conv2STL(*it, NMachine, InitPath);// *it can be changed here
		if (StockType == SFT_UNKNOWN)
		{
			it->Empty();
			continue;// ignore file if error while convertion
		}
	}
	((MTEditTreePane*)Parent)->CreateParts(AxisName, names);
}


bool MTEditTreeCtrl::IsDropAllowed(HTREEITEM htiTarget)
{
	return IsMoveAllowed(hDraged, htiTarget);
}//bool MTEditTreeCtrl::IsDropAllowed(HTREEITEM hItem)

bool MTEditTreeCtrl::IsMoveAllowed(HTREEITEM hSrc, HTREEITEM hDest)
{
	/// Обрабатываем перетаскивание корня
	if(hSrc == GetRootItem())
		return false;

	if( GetNextItem(hSrc, TVGN_PARENT) == hDest)
		return false;

	/// Обрабатываем объекты столкновений
	if(GetItemData(hDest) == 0) // перетаскиваем на объект столкновения
		return false;
	
	if(GetItemData(hSrc) == 0)  // перетаскиваем объект столкновения
	{
		if(hDest == GetRootItem())  // нельзя делать объект столкновения корнем
			return false;   
		CString Axis1,Axis2,NewAxis1;
		Axis1 = GetAxis1ofCollision(hSrc);
		Axis2 = GetAxis2ofCollision(hSrc);

		NewAxis1 = GetItemText(hDest);
		/// Проверяем, не перетаскиваем ли мы на ту ось, с которой контролируем
		/// столкновение
		if(NewAxis1 == Axis2)
			return false;

		return true;
	}//if
	//else  ///Перетаскиваем ось // commented out 15.07.20
	//{
	//	int a, b;
	//	GetItemImage(hDest, a, b);
	//	if ( a == 4 || a == 5 || a == 6 )  // tool, virttool, stock
	//		return false;
	//}//else
	/// Если наводим на себя, то отменяем перетаскивание
	if(hSrc == hDest)
	{
		return false;
	}//if
	/// Проверяем, не перетаскиваем ли мы в собственное поддерево
	CWnd *Parent = GetParent();
	if (((MTEditTreePane*)Parent)->IsDescendant(GetItemText(hSrc),GetItemText(hDest)))
		return false;

	return true;
}//bool MTEditTreeCtrl::IsMoveAllowed(HTREEITEM hSrc, HTREEITEM hDest)

int MTEditTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	SetWindowTheme(GetSafeHwnd(), L"Explorer", NULL);// all colors (and some other things) from windows explorer
	// Register Tree control as a drop target	
	m_CTreeDropTarget.Register(this);
	return 0;
}

void MTEditTreeCtrl::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	HTREEITEM hTSelItem = pNMTreeView->itemNew.hItem;

	hDraged = hTSelItem;
	// Highlight selected item
	SelectItem(hTSelItem);

	TreeView_SelectDropTarget(m_hWnd,NULL);

	COleDataSource *poleSourceObj = new COleDataSource ;
	MTEditDropTarget::m_shWndTreeCtrl = m_hWnd;
	// Begin Drag operation
	DROPEFFECT dropeffect = poleSourceObj->DoDragDrop();

	delete poleSourceObj;
	*pResult = 0;
}
void MTEditTreeCtrl::OnTvnSelchanging(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	HTREEITEM hSelItem = pNMTreeView->itemNew.hItem;
	CWnd *Parent = GetParent();
	if( ! ((MTEditTreePane*)Parent)->IsChangingNodeAllowed())
	{
		*pResult = 1;
		return;
	}//if
	
	*pResult = 0;
}

void MTEditTreeCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	/// Получаем элемент, на котором открыто контекстное меню
	CPoint clientpt = point;
	ScreenToClient(&clientpt);

	hContextMenu = HitTest(clientpt);
	/// Если попытались отктыть контекстное меню на пустом месте, ничего не делаем
	if(hContextMenu == NULL)
		return;
	
	/// Показываем свойства элемента
	this->SelectItem(hContextMenu);

	/// Загружаем меню из ресурсов
	CMenu Menu, *hPopupMenu;
	Menu.LoadMenu(IDR_PA_MACH_EDIT_TREE_AXIS);
	hPopupMenu = Menu.GetSubMenu(0);

	/// Если не было ничего вырезано, выключаем пункт "Вставить"
	HTREEITEM hItem = GetCutNode();
	if(hItem == NULL)
		hPopupMenu->EnableMenuItem(IDR_MTEDIT_PASTE, MF_GRAYED);
	else 
	/// Если что-то было вырезано, проверяем, можно ли перемещать сюда
	if(!IsMoveAllowed(hItem, hContextMenu))
	/// Если нет, выключаем пункт "Вставить"	
		hPopupMenu->EnableMenuItem(IDR_MTEDIT_PASTE, MF_GRAYED);

	switch (GetItemData(hContextMenu))
	{
	case 0:
		/// Для объектов столкновения выключаем пункт "Вставить" и все пункты "Добавить"
		hPopupMenu->EnableMenuItem(IDR_MTEDIT_PASTE, MF_GRAYED);
		hPopupMenu->EnableMenuItem(IDR_MTEDIT_ADD_X, MF_GRAYED);
		hPopupMenu->EnableMenuItem(IDR_MTEDIT_ADD_Y, MF_GRAYED);
		hPopupMenu->EnableMenuItem(IDR_MTEDIT_ADD_Z, MF_GRAYED);
		hPopupMenu->EnableMenuItem(IDR_MTEDIT_ADD_A, MF_GRAYED);
		hPopupMenu->EnableMenuItem(IDR_MTEDIT_ADD_B, MF_GRAYED);
		hPopupMenu->EnableMenuItem(IDR_MTEDIT_ADD_C, MF_GRAYED);
		hPopupMenu->EnableMenuItem(IDR_MTEDIT_ADD_TRANS, MF_GRAYED);
		hPopupMenu->EnableMenuItem(IDR_MTEDIT_ADD_ROT, MF_GRAYED);
//		hPopupMenu->EnableMenuItem(IDR_MTEDIT_ADD_PART, MF_GRAYED);
		hPopupMenu->EnableMenuItem(IDR_MTEDIT_ADD_PARTS, MF_GRAYED);
		hPopupMenu->EnableMenuItem(IDR_MTEDIT_ADD_3DTOSTL, MF_GRAYED);
		hPopupMenu->EnableMenuItem(IDR_MTEDIT_ADD_TOOL, MF_GRAYED);
		hPopupMenu->EnableMenuItem(IDR_MTEDIT_ADD_VIRTTOOL, MF_GRAYED);
		hPopupMenu->EnableMenuItem(IDR_MTEDIT_ADD_STOCK, MF_GRAYED);
		hPopupMenu->EnableMenuItem(IDR_MTEDIT_ADD_COLLISION, MF_GRAYED);
		break;
	case 1: //axis
		break;
	case 2: //root
		/// Для корня выклчаем пункты "вырезать" и "удалить"
		hPopupMenu->EnableMenuItem(IDR_MTEDIT_CUT, MF_GRAYED);
		hPopupMenu->EnableMenuItem(IDR_MTEDIT_DELETE, MF_GRAYED);
		break;
	}//switch
	/// Для tool, virttool, stock выключаем пункты "Добавить"
	int a,b;
	GetItemImage(hContextMenu, a, b);
	if(a == 4 || a == 5 || a== 6) //tool, virttool, stock
	{
		hPopupMenu->EnableMenuItem(IDR_MTEDIT_ADD_X, MF_GRAYED);
		hPopupMenu->EnableMenuItem(IDR_MTEDIT_ADD_Y, MF_GRAYED);
		hPopupMenu->EnableMenuItem(IDR_MTEDIT_ADD_Z, MF_GRAYED);
		hPopupMenu->EnableMenuItem(IDR_MTEDIT_ADD_A, MF_GRAYED);
		hPopupMenu->EnableMenuItem(IDR_MTEDIT_ADD_B, MF_GRAYED);
		hPopupMenu->EnableMenuItem(IDR_MTEDIT_ADD_C, MF_GRAYED);
		hPopupMenu->EnableMenuItem(IDR_MTEDIT_ADD_TRANS, MF_GRAYED);
		hPopupMenu->EnableMenuItem(IDR_MTEDIT_ADD_ROT, MF_GRAYED);
//		hPopupMenu->EnableMenuItem(IDR_MTEDIT_ADD_PART, MF_GRAYED);
		hPopupMenu->EnableMenuItem(IDR_MTEDIT_ADD_PARTS, MF_GRAYED);
		hPopupMenu->EnableMenuItem(IDR_MTEDIT_ADD_3DTOSTL, MF_GRAYED);
		hPopupMenu->EnableMenuItem(IDR_MTEDIT_ADD_TOOL, MF_GRAYED);
		hPopupMenu->EnableMenuItem(IDR_MTEDIT_ADD_VIRTTOOL, MF_GRAYED);
		hPopupMenu->EnableMenuItem(IDR_MTEDIT_ADD_STOCK, MF_GRAYED);
		hPopupMenu->EnableMenuItem(IDR_MTEDIT_ADD_COLLISION, MF_GRAYED);
	}//if

	/// Показываем контекстное меню
	//if(hContextMenu != NULL)
	//	hPopupMenu->TrackPopupMenu(NULL,point.x, point.y, GetParent());

	((CWinAppEx*)AfxGetApp())->GetContextMenuManager()->ShowPopupMenu(hPopupMenu->m_hMenu, point.x, point.y, this, TRUE);
	//auto ID = ((CWinAppEx*)AfxGetApp())->GetContextMenuManager()->TrackPopupMenu(hPopupMenu->m_hMenu, point.x, point.y, this);
	//if(ID != 0)
	//	SendMessage(WM_COMMAND, ID);
	/// Удаляем объект меню
	Menu.DestroyMenu();
}

void MTEditTreeCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	/// Если нажали delete
	if(nChar == VK_DELETE)
	{
		/// Кладём в hContextmenu удаляемый узел
		hContextMenu = GetSelectedItem();
		/// Если он не пуст, вызываем обработку удаления ветви
		if( hContextMenu != NULL)
			OnCMDeleteBranch();
	}//if
	CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}
