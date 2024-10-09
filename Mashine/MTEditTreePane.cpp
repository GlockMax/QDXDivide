// MTEditTreePane.cpp : implementation file
//

#include "stdafx.h"
#include "ResInv.h"
#include "MTMashineTool.h"
#include "MTForwardAxis.h"
#include "MTRotaryAxis.h"
#include "MTCollisionArray.h"
#include "math.h"
#include "MTEditSaveAsDlg.h"
#include "MTEditColAxisDlg.h"
#include "MTEditArrayDlg.h"
#include "MTRelocateAxis.h"
#include "MTConfig.h"
#include "MTEditTreePane.h"

using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// MTEditTreePane

IMPLEMENT_DYNAMIC(MTEditTreePane, CDockablePane)

MTEditTreePane::MTEditTreePane()
{
	fUndo = 0;
	fRedo = 0;
	fSave = 0;
	MTEditPtr = NULL;
}

MTEditTreePane::~MTEditTreePane()
{
}


BEGIN_MESSAGE_MAP(MTEditTreePane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_TREE_TOOLBAR_EXPAND, OnToolBarButtonExpand)
	ON_COMMAND(ID_TREE_TOOLBAR_SAVE, OnToolBarButtonSave)
	ON_COMMAND(ID_TREE_TOOLBAR_LOAD_BRANCH, OnToolBarLoadBranch)
	ON_COMMAND(ID_TREE_TOOLBAR_UNDO, OnToolBarButtonUndo)
	ON_COMMAND(ID_TREE_TOOLBAR_REDO, OnToolBarButtonRedo)
	ON_COMMAND(ID_TREE_TOOLBAR_SAVEAS, OnToolBarButtonSaveAs)
	

	ON_UPDATE_COMMAND_UI(ID_TREE_TOOLBAR_UNDO, OnUpdateStateTBUndo)
	ON_UPDATE_COMMAND_UI(ID_TREE_TOOLBAR_REDO, OnUpdateStateTBRedo)
	ON_UPDATE_COMMAND_UI(ID_TREE_TOOLBAR_SAVE, OnUpdateStateTBSave)
	ON_UPDATE_COMMAND_UI(ID_TREE_TOOLBAR_SAVEAS, OnUpdateStateTBSaveAs)
	ON_WM_CONTEXTMENU()
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

void MTEditTreePane::OnPaMachEditEnd()
{
	MTEditPtr->Exit();
}

void MTEditTreePane::OnToolBarButtonExpand(void)
{
	mtTreeTree.ExpandCollapseAll();
	return;
}

void MTEditTreePane::OnToolBarButtonSave(void)
{
	MTEditPtr->ChangeSave();
}

void MTEditTreePane::OnToolBarLoadBranch(void)
{
	CString filename, filter, ext = "xml";
	filter = "*." + ext + "|*." + ext + "|";
	CFileDialog fd(TRUE, ext, filename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter);
	if (fd.DoModal() == IDOK)
		filename = fd.GetPathName();
	else
		return;
	MTEditPtr->ChangeLoadBranch(filename);
}

void MTEditTreePane::OnToolBarButtonUndo(void)
{
	MTEditPtr->Undo();
}

void MTEditTreePane::OnToolBarButtonRedo(void)
{
	MTEditPtr->Redo();
}

void MTEditTreePane::OnToolBarButtonSaveAs(void)
{
	MTEditSaveAsDlg dlg;
	if(dlg.DoModal(MachinePath, EditingMT->GetFileName()) == IDOK)
	{
		MTEditPtr->ChangeSaveAs(dlg.m_name);
		RenameRoot(dlg.m_name);
	}//if

}
void MTEditTreePane::OnShowWindow( BOOL bShow, UINT nStatus )
{

}

void MTEditTreePane::OnPressCloseButton()
{
	MTEditPtr->Exit();
	//CDockablePane::OnPressCloseButton();
}

BOOL MTEditTreePane::AllowShowOnPaneMenu() const
{
	return false;
}

void MTEditTreePane::ShowPropPane()
{
	MTEditPtr->ShowPropPane();
}//void MTEditTreePane::ShowPropPane()

void MTEditTreePane::OnUpdateStateTBUndo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(fUndo);
}//void MTEditTreePane::OnUpdateStateTBUndo(CCmdUI *pCmdUI)

void MTEditTreePane::OnUpdateStateTBRedo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(fRedo);
}//void MTEditTreePane::OnUpdateStateTBUndo(CCmdUI *pCmdUI)

void MTEditTreePane::OnUpdateStateTBSave(CCmdUI *pCmdUI)
{
	if(MTEditPtr != NULL)
	if(MTEditPtr->DEMO)
		pCmdUI->Enable(0);
	else
		pCmdUI->Enable(fSave);
}//void MTEditTreePane::OnUpdateStateTBUndo(CCmdUI *pCmdUI)

void MTEditTreePane::OnUpdateStateTBSaveAs(CCmdUI *pCmdUI)
{
	if(MTEditPtr != NULL)
	if(MTEditPtr->DEMO)
		pCmdUI->Enable(0);
	else
		pCmdUI->Enable(1);
}//void MTEditTreePane::OnUpdateStateTBSaveAs(CCmdUI *pCmdUI)

void MTEditTreePane::TBEnableUndo(bool Enable)
{
	if( Enable )
		fUndo = 1;
	else
		fUndo = 0;
}//void MTEditTreePane::TBEnableUndo(bool Enable)

void MTEditTreePane::TBEnableRedo(bool Enable)
{
	if( Enable )
		fRedo = 1;
	else
		fRedo = 0;
}//void MTEditTreePane::TBEnableRedo(bool Enable = true)

void MTEditTreePane::TBEnableSave(bool Enable)
{
	if( Enable )
		fSave = 1;
	else
		fSave = 0;
}

void MTEditTreePane::SetMachineTool(MTMachineTool *MT)
{
	EditingMT = MT;
}//void MTEditTreePane::SetMachineTool(MTMachineTool *MT)

void MTEditTreePane::SetMTEdit(MTEdit *MTE, CString path)
{
	MTEditPtr = MTE;
	MachinePath = path;
}//void MTEditTreePane::SetMTEdit(MTEdit &MTE)

//номера осей - чётные числа
//номера объектов столкновения - нечётные числа
bool MTEditTreePane::ShowMachineTool(MTMachineTool *MT, int selIndex)
{
	//Если мы попали на одну из осей stock_X, stock_Y, stock_Z, которые в дереве не отображаются,
	//Выделяем stock;
	int SelIndex = selIndex;
	if(MT->GetCurConfig().IsCompAxis(SelIndex))
		SelIndex++;
	if(MT->GetCurConfig().IsCompAxis(SelIndex))
		SelIndex++;
	if(MT->GetCurConfig().IsCompAxis(SelIndex))
		SelIndex++;

	mtTreeTree.DeleteAllItems();

	HTREEITEM hRoot; //Виртуальный конрень дерева
	hRoot = mtTreeTree.InsertItem(MT->GetFileName(), 7, 7);
	mtTreeTree.SetItemData(hRoot, (DWORD_PTR)2);

	int size = MT->GetCurConfig().GetSize();
	HTREEITEM hSelItem = 0;

	AxisStack AStack;
	AxisStackNode *ASNode;

	for (int i = 0 ; i < size; i++)
	{
		//2 До тех пор, пока стек не пуст и уровень, записанный в вершине стека больше либо равен уровню текущей
		// оси, извлекаем элемент из стека
		ASNode = AStack.Peek();			

		if(MT->GetCurConfig().IsCompAxis(i))
		{
			while( (ASNode != NULL) && ASNode->level >= MT->GetCurConfig().GetLevel(i))
			{
				ASNode = AStack.Pop();
				delete ASNode;
				ASNode = AStack.Peek();
			}//while
			continue;  
		}//if

		while( (ASNode != NULL) && ASNode->level >= MT->GetCurConfig().GetLevel(i))
		{
			ASNode = AStack.Pop();
			delete ASNode;
			ASNode = AStack.Peek();
		}//while

		//Определяем иконку для оси
		int AxisImage = 0, AxisSelImage = 0;
		GetImagesNums(MT, i, AxisImage, AxisSelImage);
		//3 Подвешиваем текущую ось в контрол дерево к элементу, считанному из вершины стека
		
		ASNode = AStack.Peek();
		HTREEITEM hCurItem = 0;
		if(ASNode == NULL)
		{
			//hCurItem = mtTreeTree.InsertItem((*MT)[i]->GetName(), AxisImage, AxisSelImage);

			hCurItem = mtTreeTree.InsertItem(MT->GetCurConfig()[i]->GetName(), AxisImage, AxisSelImage, hRoot);


			//в каждый узел дерева можно записать адрес DWORD_PTR чего-либо.
			//требуется хранить в узле информацию о том, какого типа узел - ось или объект столкновения.
			//чтобы не создавать новых переменных и не записывать сюда адрес на них, запишем тип объекта
			//прямо в адрес: 1 - ось, 0 - объект столкновения
			mtTreeTree.SetItemData(hCurItem,(DWORD_PTR)1);
			if (i == SelIndex)
				hSelItem = hCurItem;
		}//if
		else
		{
			hCurItem = mtTreeTree.InsertItem(MT->GetCurConfig()[i]->GetName(), AxisImage, AxisSelImage, ASNode->hItem);
			mtTreeTree.SetItemData(hCurItem,(DWORD_PTR)1);
			if (i == SelIndex)
				hSelItem = hCurItem;
		}//else

		//4 Отображаем объекты столкновений для оси

		HTREEITEM hColItem;
		SCollision *Col;
		int ColNum;
		CString ColString;
		ColString.LoadString(IDS_COLLISION_STRING);
		CString AxisName = MT->GetCurConfig()[i]->GetName();
		ColNum = MT->GetAxisColNum(AxisName);
		for(int k = 1 ; k <= ColNum ; k++)
		{
			Col = MT->GetAxisCol(AxisName, k);
			hColItem = mtTreeTree.InsertItem(ColString + Col->Axis2, 0, 0, hCurItem);
			mtTreeTree.SetItemData(hColItem,(DWORD_PTR)0);
		}//for

		//5 Кладём в стек текущий уровень
		ASNode = new AxisStackNode();
		ASNode->level = MT->GetCurConfig().GetLevel(i);
		ASNode->hItem = hCurItem;
		AStack.Push(ASNode);

	}//for

	//Освобождаем стек
	while(AStack.Peek() != NULL)
	{
		ASNode = AStack.Pop();
		delete ASNode;
	}//while

	mtTreeTree.ExpandAll();

	//Выделение выбранной оси
	if(hSelItem != 0)
	{
		auto d = mtTreeTree.GetItemData(hSelItem);
		if(d == 1)
			MTEditPtr->ShowAxisProp( mtTreeTree.GetItemText(hSelItem));
	}
//	mtTreeTree.SelectItem(hSelItem);

	return true;
}//bool MTEditTreePane::ShowMachineTool(MTMachineTool *MT, int selIndex)

bool MTEditTreePane::GetImagesNums(MTMachineTool* MT, int i, int& AxisImage, int& AxisSelImage)
{
	switch (MT->GetCurConfig()[i]->GetType())
	{
	case MTAxis::AT_FOR:
		AxisImage = 1;
		AxisSelImage = 1;
		break;
	case MTAxis::AT_ROT:
		AxisImage = 2;
		AxisSelImage = 2;
		break;
	case MTAxis::AT_SPIN:
		AxisImage = 9;
		AxisSelImage = 9;
		break;
	case MTAxis::AT_PART:
		AxisImage = 3;
		AxisSelImage = 3;
		break;
	case MTAxis::AT_TOOL:
		AxisImage = 4;
		AxisSelImage = 4;
		break;
	case MTAxis::AT_VTOOL:
		AxisImage = 5;
		AxisSelImage = 5;
		break;
	case MTAxis::AT_STOCK:
		AxisImage = 6;
		AxisSelImage = 6;
		break;
	case MTAxis::AT_SCHAIN:
		AxisImage = 8;
		AxisSelImage = 8;
		break;
	}//switch

	return false;
}

bool MTEditTreePane::SelectAxis(const CString& AxisName)
{
	HTREEITEM hItem = mtTreeTree.GetItemByName(NULL,AxisName);
	if(hItem == NULL)
		return FALSE;

	mtTreeTree.SelectItem(hItem);
	if(mtTreeTree.GetSelectedItem() == hItem)
		this->ShowAxisProp(AxisName);
	
	return TRUE;
}//bool MTEditTreePane::SelectAxis(CString AxisName)

bool MTEditTreePane::SelectNode(const CString& NodeName)
{
	HTREEITEM hItem = mtTreeTree.GetItemByName(NULL,NodeName);
	if(hItem == NULL)
		return FALSE;

	mtTreeTree.SelectItem(hItem);
	return TRUE;
}//bool MTEditTreePane::SelectNode(CString NodeName)

bool MTEditTreePane::SelectAndShowPropNode(const CString& NodeName)
{
	HTREEITEM hItem = mtTreeTree.GetItemByName(NULL,NodeName);
	if(hItem == NULL)
		return FALSE;
	
	mtTreeTree.SelectItem(hItem);

	auto DType = mtTreeTree.GetItemData(hItem);
	switch(DType)
	{
	case 0:  //Объект столкновения
		ShowColProp(mtTreeTree.GetAxis1ofCollision(hItem), mtTreeTree.GetAxis2ofCollision(hItem));
		break;
	case 1:  //Ось
		ShowAxisProp(NodeName);
		break;
	}//switch

	return TRUE;
}//bool MTEditTreePane::SelectAndShowPropNode(CString NodeName)

CString MTEditTreePane::GetSelectedNode()
{
	HTREEITEM hItem = mtTreeTree.GetSelectedItem();
	if(hItem == NULL)
		return CString("");

	CString res = mtTreeTree.GetItemText(hItem);

	return res;
}//CString MTEditTreePane::GetSelectedNode()

bool MTEditTreePane::RenameTreeNode(const CString &OldName, const CString &NewName)
{
	//1 Находим HTREEITEM
	HTREEITEM hItem = mtTreeTree.GetItemByName(NULL, OldName);
	if(hItem == NULL)
		return FALSE;
	//2 Переименовываем его
	mtTreeTree.SetItemText(hItem, NewName);

	//3 Переименовываем объекты столкновения, в чьих именах содержится имя оси
	HTREEITEM hCol;
	CString ColString;
	ColString.LoadString(IDS_COLLISION_STRING);
	hCol = mtTreeTree.GetItemByName(NULL, ColString+OldName);
	while(hCol != NULL)
	{
		mtTreeTree.SetItemText(hCol, ColString+NewName);
		hCol = mtTreeTree.GetItemByName(NULL, ColString+OldName);
	}//while

	return TRUE;
}//bool MTEditTreePane::RenameTreeNode(CString OldName, CString NewName)

bool MTEditTreePane::ShowAxisProp(const CString& AxisName)
{
	MTEditPtr->ShowAxisProp(AxisName);
	return TRUE;
}//bool MTEditPane::ShowAxisProp(CString AxisName)

bool MTEditTreePane::ShowColProp(const CString& Axis1, const CString& Axis2)
{
	MTEditPtr->ShowColProp(Axis1, Axis2);
	return TRUE;
}//bool MTEditPane::ShowColProp(CString Axis1, CString Axis2)

bool MTEditTreePane::IsDescendant(CString &BaseAxis, CString &Axis)
{
	return MTEditPtr->IsDescendant(BaseAxis, Axis);
}//bool MTEditTreePane::IsDescendant(CString &BaseAxis, CString &Axis)

bool MTEditTreePane::IsChangingNodeAllowed()
{
	return MTEditPtr->IsChangingNodeAllowed();
}//bool MTEditTreePane::IsChangingNodeAllowed()

void MTEditTreePane::ShowEmptyProp()
{
	MTEditPtr->ShowEmptyProp();
}//void MTEditTreePane::ShowEmptyProp()

void MTEditTreePane::GetMachinePath(CString &path, CString &name)
{
	MTEditPtr->GetMachinePath(path, name);
}//void MTEditTreePane::GetMachinePath(CString &path, CString &name)

void MTEditTreePane::MoveBranch(const CString& Axis1, const CString& Axis2)
{
	//MTEditPtr->MoveBranch(Axis1,Axis2);
	MTEditPtr->ChangeMoveBranch(Axis1, Axis2);
}//void MTEditPane:MoveBranch(CString &Axis1, CString &Axis2)

void MTEditTreePane::MoveCollision(const CString& Axis1, const CString& Axis2, const CString& NewAxis1)
{
	MTEditPtr->ChangeMoveCollision(Axis1, Axis2, NewAxis1);
}//void MTEditTreePane::MoveCol(CString &MyName1, CString &EnemyName, CString &MyName2)

void MTEditTreePane::CreateSChain(const CString& ParentName)
{
	MTEditPtr->ChangeCreateSChain(ParentName, nullptr);
}

void MTEditTreePane::CreateArray(const CString& ParentName)
{
	const MTRelocateAxis* pRelAxis = MTEditPtr->GetNearestRelAxis(ParentName);
	if (pRelAxis == nullptr)
		return;
	MTEditArrayDlg dlg;
	if (dynamic_cast<const MTRotaryAxis*>(pRelAxis) != nullptr)
		dlg.SetRotary();
	if(dlg.DoModal() == IDOK)
	{
		MTEditPtr->ChangeCreateArray(ParentName, dlg.GetCount(), dlg.GetLinearStep(), !dlg.GetRotate());
	}
}

void MTEditTreePane::CreateTrans(const CString& ParentName)
{
	MTEditPtr->ChangeCreateTrans(ParentName);
}//void MTEditTreePane::CreateTrans(CString ParentName)

void MTEditTreePane::CreateRot(const CString& ParentName)
{
	MTEditPtr->ChangeCreateRot(ParentName);
}//void MTEditTreePane::CreateRot(CString ParentName)

void MTEditTreePane::CreateSpin(const CString& ParentName)
{
	MTEditPtr->ChangeCreateSpin(ParentName);
}

void MTEditTreePane::CreatePart(const CString& ParentName)
{
	MTEditPtr->ChangeCreatePart(ParentName);
}//void MTEditTreePane::CreatePart(CString ParentName)

void MTEditTreePane::CreateParts(const CString& ParentName, vector<CString> &list)
{
	MTEditPtr->ChangeCreateParts(ParentName, list);
}//void MTEditTreePane::CreateParts(CString ParentName, vector<CString> &list)

void MTEditTreePane::CreateTool(const CString& ParentName)
{
	MTEditPtr->ChangeCreateTool(ParentName);
}//void MTEditTreePane::CreateTool(CString ParentName)

void MTEditTreePane::CreateToolVirt(const CString& ParentName)
{
	MTEditPtr->ChangeCreateToolVirt(ParentName);
}//void MTEditTreePane::CreateToolVirt(CString ParentName)

void MTEditTreePane::CreateStock(const CString& ParentName)
{
	MTEditPtr->ChangeCreateStock(ParentName);
}//void MTEditTreePane::CreateStock(CString ParentName)

void MTEditTreePane::CreateCol(const CString& ParentName)
{
	MTEditColAxisDlg Dlg;
	if(Dlg.DoModal(EditingMT, ParentName) != IDOK)
		return ;

	if(!Dlg.m_name.IsEmpty())
		MTEditPtr->ChangeCreateCol(ParentName, Dlg.m_name);
}//void MTEditTreePane::CreateCol(CString ParentName)

void MTEditTreePane::CreateX(const CString& ParentName)
{
	MTEditPtr->ChangeCreateX(ParentName);
}//void MTEditTreePane::CreateX(CString ParentName)

void MTEditTreePane::CreateY(const CString& ParentName)
{
	MTEditPtr->ChangeCreateY(ParentName);
}//void MTEditTreePane::CreateY(CString ParentName)

void MTEditTreePane::CreateZ(const CString& ParentName)
{
	MTEditPtr->ChangeCreateZ(ParentName);
}//void MTEditTreePane::CreateZ(CString ParentName)

void MTEditTreePane::CreateA(const CString& ParentName)
{
	MTEditPtr->ChangeCreateA(ParentName);
}//void MTEditTreePane::CreateA(CString ParentName)

void MTEditTreePane::CreateB(const CString& ParentName)
{
	MTEditPtr->ChangeCreateB(ParentName);
}//void MTEditTreePane::CreateB(CString ParentName)

void MTEditTreePane::CreateC(const CString& ParentName)
{
	MTEditPtr->ChangeCreateC(ParentName);
}//void MTEditTreePane::CreateC(CString ParentName)

void MTEditTreePane::DeleteBranch(const CString& AxisName)
{
	MTEditPtr->ChangeDeleteBranch(AxisName);
}//void MTEditTreePane::DeleteBranch(CString AxisName)

void MTEditTreePane::DeleteCollision(const CString& Axis1, const CString& Axis2)
{
	MTEditPtr->ChangeDeleteCollision(Axis1, Axis2);
}//void MTEditTreePane::DeleteCollision(CString Axis1, CString Axis2)

void MTEditTreePane::MoveBranchCtrl(const CString& AxisName, const CString& DestName)
{
	mtTreeTree.MoveBrahcnCtrl(AxisName, DestName);
}//void MTEditTreePane::MoveBranchCtrl(CString AxisName, CString DestName)

void MTEditTreePane::MoveCollisionCtrl(const CString& Axis1, const CString& Axis2, const CString& NewAxis1)
{
	mtTreeTree.MoveCollisionCtrl(Axis1, Axis2, NewAxis1);
}

HTREEITEM MTEditTreePane::CreateAxisCtrl(const MTAxis& Axis, const CString& ParentName)
{
	HTREEITEM hParent = 0;
	if (ParentName == _T(""))
		hParent = mtTreeTree.GetRootItem();
	else
		hParent = mtTreeTree.GetItemByName(nullptr, ParentName);
	int AxisImage = 0, AxisSelImage = 0;
	const int AxisIndex = EditingMT->GetCurConfig().GetAxisIndex(Axis.GetName());
	GetImagesNums(EditingMT, AxisIndex, AxisImage, AxisSelImage);
	HTREEITEM hAxis = mtTreeTree.InsertItem(Axis.GetName(), AxisImage, AxisSelImage, hParent);
	mtTreeTree.SetItemData(hAxis, (DWORD_PTR)1);
	return hParent;
}

void MTEditTreePane::CreateTransCtrl(const CString& AxisName, const CString& ParentName)
{
	HTREEITEM hParent;
	if(ParentName == _T(""))
		hParent = mtTreeTree.GetRootItem();
	else
		hParent = mtTreeTree.GetItemByName(NULL, ParentName);
	
	HTREEITEM hAxis = mtTreeTree.InsertItem(AxisName, 1, 1, hParent);
	mtTreeTree.SetItemData(hAxis, (DWORD_PTR)1);
}

void MTEditTreePane::CreateRotCtrl(const CString& AxisName, const CString& ParentName)
{
	HTREEITEM hParent;
	if (ParentName == _T(""))
		hParent = mtTreeTree.GetRootItem();
	else
		hParent = mtTreeTree.GetItemByName(NULL, ParentName);

	HTREEITEM hAxis = mtTreeTree.InsertItem(AxisName, 2, 2, hParent);
	mtTreeTree.SetItemData(hAxis, (DWORD_PTR)1);
}

void MTEditTreePane::CreateSpinCtrl(const CString& AxisName, const CString& ParentName)
{
	HTREEITEM hParent;
	if (ParentName == _T(""))
		hParent = mtTreeTree.GetRootItem();
	else
		hParent = mtTreeTree.GetItemByName(NULL, ParentName);

	HTREEITEM hAxis = mtTreeTree.InsertItem(AxisName, 9, 9, hParent);
	mtTreeTree.SetItemData(hAxis, (DWORD_PTR)1);
}

void MTEditTreePane::CreatePartCtrl(const CString& AxisName, const CString& ParentName)
{
	HTREEITEM hParent; 
	if(ParentName == _T(""))
		hParent = mtTreeTree.GetRootItem();
	else
		hParent = mtTreeTree.GetItemByName(NULL, ParentName);
	
	HTREEITEM hAxis = mtTreeTree.InsertItem(AxisName, 3, 3, hParent,TVI_FIRST);
	mtTreeTree.SetItemData(hAxis, (DWORD_PTR)1);
}//void MTEditTreePane::CreatePartCtrl(CString AxisName, CString ParentName);
void MTEditTreePane::CreateToolCtrl(const CString& AxisName, const CString& ParentName)
{
	HTREEITEM hParent;
	if(ParentName == _T(""))
		hParent = mtTreeTree.GetRootItem();
	else
		hParent = mtTreeTree.GetItemByName(NULL, ParentName);
	
	HTREEITEM hAxis = mtTreeTree.InsertItem(AxisName, 4, 4, hParent, TVI_FIRST);
	mtTreeTree.SetItemData(hAxis, (DWORD_PTR)1);
}//void MTEditTreePane::CreateToolCtrl(CString AxisName, CString ParentName)

void MTEditTreePane::CreateToolVirtCtrl(const CString& AxisName, const CString& ParentName)
{
	HTREEITEM hParent;
	if(ParentName == _T(""))
		hParent = mtTreeTree.GetRootItem();
	else
		hParent = mtTreeTree.GetItemByName(NULL, ParentName);
	
	HTREEITEM hAxis = mtTreeTree.InsertItem(AxisName, 5, 5, hParent,TVI_FIRST);
	mtTreeTree.SetItemData(hAxis, (DWORD_PTR)1);
}//void MTEditTreePane::CreateToolVirtCtrl(CString AxisName, CString ParentName)

void MTEditTreePane::CreateStockCtrl(const CString& AxisName, const CString& ParentName)
{
	HTREEITEM hParent;
	if(ParentName == _T(""))
		hParent = mtTreeTree.GetRootItem();
	else
		hParent = mtTreeTree.GetItemByName(NULL, ParentName);
	
	HTREEITEM hAxis = mtTreeTree.InsertItem(AxisName, 6, 6, hParent, TVI_FIRST);
	mtTreeTree.SetItemData(hAxis, (DWORD_PTR)1);
}//void MTEditTreePane::CreateStockCtrl(CString AxisName, CString ParentName)

void MTEditTreePane::CreateColCtrl(const CString& AxisName, const CString& ParentName)
{
	HTREEITEM hParent;
	if(ParentName == _T(""))
		hParent = mtTreeTree.GetRootItem();
	else
		hParent = mtTreeTree.GetItemByName(NULL, ParentName);
	
	HTREEITEM hAxis = mtTreeTree.InsertItem(AxisName, 0, 0, hParent);
	mtTreeTree.SetItemData(hAxis, (DWORD_PTR)0);
}//void MTEditTreePane::CreateColCtrl(CString AxisName, CString ParentName)

void MTEditTreePane::DeleteBranchCtrl(const CString& NodeName)
{
	HTREEITEM hItem = mtTreeTree.GetItemByName(NULL, NodeName);
	mtTreeTree.DeleteItem(hItem);
}//void MTEditTreePane::DeleteBranchCtrl(CString AxisName)

int MTEditTreePane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create views:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!mtTreeTree.Create(dwViewStyle, rectDummy, this, IDC_AXIS_TREE))
	{
		TRACE0("Failed to create Tree\n");
		return -1;      // fail to create
	}//if

	mtTreeToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_MTTREE);
	mtTreeToolBar.LoadToolBar(IDR_MTTREE, 0, 0, TRUE);
	mtTreeToolBar.SetPaneStyle(mtTreeToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	mtTreeToolBar.SetPaneStyle(mtTreeToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	mtTreeToolBar.SetOwner(this);

	//Загрузка иконок для дерева
	mtTreeViewImages.DeleteImageList();
	UINT uiBmpId = IDB_AXIS;
	CBitmap bmp;
	if( !bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("Can't load bitmap: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return 0;
	}//if
	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;
	nFlags |= ILC_COLOR24;
	mtTreeViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	mtTreeViewImages.Add(&bmp, RGB(255, 0, 0));

	mtTreeTree.SetImageList(&mtTreeViewImages, TVSIL_NORMAL);

	mtTreeToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();

	return 0;
}//int MTEditTreePane::OnCreate(LPCREATESTRUCT lpCreateStruct)

void MTEditTreePane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void MTEditTreePane::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}//if

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = mtTreeToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	mtTreeToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	mtTreeTree.SetWindowPos(NULL, rectClient.left, rectClient.top + cyTlb, rectClient.Width(), rectClient.Height() - cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
}//void MTEditTreePane::AdjustLayout()

void MTEditTreePane::OnContextMenu(CWnd* pWnd, CPoint point)
{
	mtTreeTree.OnContextMenu(pWnd, point);
	
	return;
}

void MTEditTreePane::RenameRoot(const CString &NewRoot)
{
	HTREEITEM Root = mtTreeTree.GetRootItem();
	if(!Root)
		return;
	mtTreeTree.SetItemText(Root, NewRoot);
}

bool MTEditTreePane::SaveBranch(const CString& AxisName) const
{
	CString filename, filter, ext = "xml";
	filter = "*." + ext + "|*." + ext + "|";
	CFileDialog fd(FALSE, ext, filename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter);
	if (fd.DoModal() == IDOK)
		filename = fd.GetPathName();
	else
		return false;
	return EditingMT->SaveXML4Branch("", filename, AxisName);;
}

BOOL CTreeToolBar::OnUserToolTip(CMFCToolBarButton *pButton, CString & strTTText) const
{
	switch (pButton->GetImage())
	{
	case 0:
		strTTText.LoadString(IDS_TREE_TOOLBAR_TOOLTIP0);
		return TRUE;
		break;
	case 1:
		strTTText.LoadString(IDS_TREE_TOOLBAR_TOOLTIP1);
		return TRUE;
		break;
	case 2:
		strTTText.LoadString(IDS_TREE_TOOLBAR_TOOLTIP2);
		return TRUE;
		break;
	case 3:
		strTTText.LoadString(IDS_TREE_TOOLBAR_TOOLTIP3);
		return TRUE;
		break;
	case 4:
		strTTText.LoadString(IDS_TREE_TOOLBAR_TOOLTIP4);
		return TRUE;
		break;
	case 5:
		strTTText.LoadString(IDS_TREE_TOOLBAR_TOOLTIP5);
		return TRUE;
		break;
	case 6:
		strTTText.LoadString(IDS_TREE_TOOLBAR_TOOLTIP6);
		return TRUE;
		break;
	case 7:
		strTTText.LoadString(IDS_TREE_TOOLBAR_TOOLTIP7);
		return TRUE;
		break;
	case 8:
		strTTText.LoadString(IDS_TREE_TOOLBAR_TOOLTIP8);
		return TRUE;
		break;
	default :
		return FALSE;
		break;
	}//switch
};

void MTEditTreePane::OnClose()
{

	CDockablePane::OnClose();
}
