
#include "stdafx.h"
#include "MainFrm.h"
#include "NCUnitTreePane.h"
#include "Resource.h"
#include "NCMControl.h"
#include "NCUnitDef.h"
#include <vector>
#include <set>
#include <algorithm>
#include "NCSelectXmlDlg.h"
#include "NCEditor2Doc.h"

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNCUnitTreePane::CNCUnitTreePane()
{
	ShowPresenceOnly = false;
	ShowAlphaSort = false;
	ShowGroups = false;
	ExpandState = 2;
}

CNCUnitTreePane::~CNCUnitTreePane()
{
}

BEGIN_MESSAGE_MAP(CNCUnitTreePane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CLASS_ADD_MEMBER_FUNCTION, OnClassAddMemberFunction)
	ON_COMMAND(ID_CLASS_ADD_MEMBER_VARIABLE, OnClassAddMemberVariable)
	ON_COMMAND(ID_CLASS_DEFINITION, OnClassDefinition)
	ON_COMMAND(ID_CLASS_PROPERTIES, OnClassProperties)
	ON_COMMAND(ID_NEW_FOLDER, OnChangeXML)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_UPDATE_COMMAND_UI(ID_TREE_TB_EXPAND, &CNCUnitTreePane::OnUpdateExpand)
	ON_UPDATE_COMMAND_UI(ID_TREE_TB_PRES, &CNCUnitTreePane::OnUpdatePres)
	ON_UPDATE_COMMAND_UI(ID_TREE_TB_ABC, &CNCUnitTreePane::OnUpdateABC)
	ON_UPDATE_COMMAND_UI(ID_TREE_TB_GROUP, &CNCUnitTreePane::OnUpdateGroup)
	ON_COMMAND(ID_TREE_TB_EXPAND, &CNCUnitTreePane::OnTBExpand)
	ON_COMMAND(ID_TREE_TB_PRES, &CNCUnitTreePane::OnTBPres)
	ON_COMMAND(ID_TREE_TB_ABC, &CNCUnitTreePane::OnTBABC)
	ON_COMMAND(ID_TREE_TB_GROUP, &CNCUnitTreePane::OnTBGroup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNCUnitTreePane message handlers

int CNCUnitTreePane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create views:
	const DWORD dwViewStyle = TVS_SHOWSELALWAYS | WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndTreeView.Create(dwViewStyle, rectDummy, this, 2))
	{
		TRACE0("Failed to create Class View\n");
		return -1;      // fail to create
	}

	// Load images:
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
	m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* Is locked */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// Fill in some static tree view data (dummy code, nothing magic here)
	//FillClassView();

	return 0;
}

void CNCUnitTreePane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CNCUnitTreePane::FillClassView()
{
	HTREEITEM hRoot = m_wndTreeView.InsertItem(_T("FakeApp classes"), 0, 0);
	m_wndTreeView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	HTREEITEM hClass = m_wndTreeView.InsertItem(_T("CFakeAboutDlg"), 1, 1, hRoot);
	m_wndTreeView.InsertItem(_T("CFakeAboutDlg()"), 3, 3, hClass);

	m_wndTreeView.Expand(hRoot, TVE_EXPAND);

	hClass = m_wndTreeView.InsertItem(_T("CFakeApp"), 1, 1, hRoot);
	m_wndTreeView.InsertItem(_T("CFakeApp()"), 3, 3, hClass);
	m_wndTreeView.InsertItem(_T("InitInstance()"), 3, 3, hClass);
	m_wndTreeView.InsertItem(_T("OnAppAbout()"), 3, 3, hClass);

	hClass = m_wndTreeView.InsertItem(_T("CFakeAppDoc"), 1, 1, hRoot);
	m_wndTreeView.InsertItem(_T("CFakeAppDoc()"), 4, 4, hClass);
	m_wndTreeView.InsertItem(_T("~CFakeAppDoc()"), 3, 3, hClass);
	m_wndTreeView.InsertItem(_T("OnNewDocument()"), 3, 3, hClass);

	hClass = m_wndTreeView.InsertItem(_T("CFakeAppView"), 1, 1, hRoot);
	m_wndTreeView.InsertItem(_T("CFakeAppView()"), 4, 4, hClass);
	m_wndTreeView.InsertItem(_T("~CFakeAppView()"), 3, 3, hClass);
	m_wndTreeView.InsertItem(_T("GetDocument()"), 3, 3, hClass);
	m_wndTreeView.Expand(hClass, TVE_EXPAND);

	hClass = m_wndTreeView.InsertItem(_T("CFakeAppFrame"), 1, 1, hRoot);
	m_wndTreeView.InsertItem(_T("CFakeAppFrame()"), 3, 3, hClass);
	m_wndTreeView.InsertItem(_T("~CFakeAppFrame()"), 3, 3, hClass);
	m_wndTreeView.InsertItem(_T("m_wndMenuBar"), 6, 6, hClass);
	m_wndTreeView.InsertItem(_T("m_wndToolBar"), 6, 6, hClass);
	m_wndTreeView.InsertItem(_T("m_wndStatusBar"), 6, 6, hClass);

	hClass = m_wndTreeView.InsertItem(_T("Globals"), 2, 2, hRoot);
	m_wndTreeView.InsertItem(_T("theFakeApp"), 5, 5, hClass);
	m_wndTreeView.Expand(hClass, TVE_EXPAND);
}

void CNCUnitTreePane::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndTreeView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		//CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// Select clicked item:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	CMenu menu;
	menu.LoadMenu(IDR_TREE_CMENU);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}
}

void CNCUnitTreePane::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndTreeView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL CNCUnitTreePane::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}
void CNCUnitTreePane::ShowNCUnitDef(CNCUnitDef * unitDef, CNCTreeCtrlSnapshot *snapshot)
{
	m_unitDef = unitDef;
	ShowNCUnitDefInternal(m_unitDef, snapshot);

}//void CNCUnitTreePane::ShowNCUnitDef(CNCUnitDef * unitDef, CNCTreeCtrlSnapshot *snapshot)

void CNCUnitTreePane::ShowNCUnitDefInternal(CNCUnitDef * unitDef, CNCTreeCtrlSnapshot *snapshot)
{
	class local
	{
	public:
		/// Получаем множество групп для секции
		static bool GetGroups(IN CNCUnitDef &unitDef, IN int SectNumber, OUT set<CString> &groups)
		{
			for(unsigned int i = 0; i < unitDef.sectionList[SectNumber].defList.size(); i ++)
			{
				groups.insert(unitDef.sectionList[SectNumber].defList[i].def.group);
			}//for

			return true;
		};//static bool GetGroups(IN NCCUnitDef &unitDef, OUT vector<CString> groups)
		static void InsertDef(IN CNCUnitTreeCtrl &treectrl, HTREEITEM &hParent, Def &def, CString section, bool showpresenceonly)
		{
			if(showpresenceonly && !def.presence)
				return;

			/// 1 - Определяем иконку элемента
			int image = 0;
			if(def.presence)
				image = 1;
			else
				image = 2;

			if(def.known == false)
				image = 4;
			
			/// 2 - Вставляем элемент в дерево
			HTREEITEM hChild = treectrl.InsertItem(def.defWord, image, image, hParent);

			/// 3 - Определяем цвет текста элемента
			COLORREF color;
			if(def.presence)
				color = RGB(0,30,0);
			else
				color = RGB(140,140,140);

			if(def.known == false)
				color = RGB(100,0,0);

			/// 4 - Устанавливаем цвет элемента
			treectrl.SetItemColor(hChild, color);

			/// 5 - Добавляем структуру данных к элементу
			NCTreeItemData *pData = new NCTreeItemData();
			pData->type = I_DEFINITION;
			pData->description = def.briefDescr;
			pData->def.sectName = section;
			pData->def.known = def.known;
			treectrl.SetItemData(hChild, (DWORD_PTR)pData);
		};//static void InsertItem(IN CNCUnitTreeCtrl &treectrl, Def &def, bool showpresenceonly)
	};//class local

	/// Очищаем дерево
	m_wndTreeView.DeleteAllItems();

	/// Перебираем секции
	for(unsigned int i = 0; i < unitDef->sectionList.size(); i++)
	{
		/// Добавляем в дерево элемент секции
		HTREEITEM hSect = m_wndTreeView.InsertItem(unitDef->sectionList[i].name,0,0);
		m_wndTreeView.SetItemColor(hSect, RGB(0,0,0)); 

		/** Добавление информации к каждому элементу в дереве
		*/
		NCTreeItemData *pData = new NCTreeItemData();
		pData->type = I_SECTION;
		pData->description = unitDef->sectionList[i].briefDescr;
		m_wndTreeView.SetItemData(hSect,(DWORD_PTR)pData);

		/// Секции бывают 2 видов: обычные и Word Replacement,
		/// обрабатываем эти случаи отдельно
		if(unitDef->sectionList[i].name != _T("Word Replacement"))
		{
			/// Section with common definitions

			/// Существует 2 варианта отображения стойки с деревом:
			/// С группировкой определений по группам и без группировки
			if(ShowGroups)
			{
				/// Показываем сгруппированные определения

				/// Получаем список групп
				set <CString> groups;
				local::GetGroups(*unitDef, i, groups);
				
				/// Перебираем все группы
				set<CString>::iterator it;
				for(it = groups.begin(); it != groups.end(); it++)
				{
					/// Добавляем в дерево элемент группы
					HTREEITEM hGroup = m_wndTreeView.InsertItem(*it,3,3,hSect);
					NCTreeItemData *pData = new NCTreeItemData();
					pData->type = I_GROUP;
					CString str; str.LoadString(IDS_GROUP);
					pData->description = str;
					for(unsigned int j = 0; j < unitDef->sectionList[i].groupList.size(); j++)
					{
						if(*it == unitDef->sectionList[i].groupList[j].name)
							pData->description = unitDef->sectionList[i].groupList[j].briefDescr;
					}//for
					pData->group.sectName = unitDef->sectionList[i].name;
					m_wndTreeView.SetItemData(hGroup, (DWORD_PTR)pData);
					m_wndTreeView.SetItemColor(hGroup, RGB(0,60,0));
					/// Добавляем определения.

					/// Делаем копию вектора с определениями
					vector<Definition> tempV;
					tempV = unitDef->sectionList[i].defList;

					/// Если установлен флаг ShowAlphaSort, сортируем вектор по алфавиту
					if(ShowAlphaSort)
						sort(tempV.begin(), tempV.end());

					/// Перебираем все определения секции
					for(unsigned int j = 0; j < tempV.size(); j++)
					{
						/// Если определение принадлежит группе, отображаем его
						if(tempV[j].def.group == *it)
						{
							local::InsertDef(m_wndTreeView, hGroup, tempV[j].def, unitDef->sectionList[i].name, ShowPresenceOnly); 
							//HTREEITEM hChild;
							///// 2 варианта отображения определения: 
							///// для добавленного в стойку и для недобавленного
							//if(tempV[j].def.presence)
							//{
							//	/// Иконка номер 1
							//	hChild = m_wndTreeView.InsertItem(tempV[j].def.defWord,1,1,hGroup);
							//	/// Чёрный цвет
							//	m_wndTreeView.SetItemColor(hChild,RGB(0,30,0));
							//	NCTreeItemData *pData = new NCTreeItemData();
							//	pData->type = I_DEFINITION;
							//	pData->description = tempV[j].def.briefDescr;
							//	pData->def.sectName = unitDef->sectionList[i].name;
							//	pData->def.known = tempV[j].def.known;
							//	m_wndTreeView.SetItemData(hChild, (DWORD_PTR)pData);
							//}//if
							//else
							//{
							//	/// Недобавленные определения отображаем только если
							//	/// Не установлен флаг ShowPresenceOnly
							//	if(!ShowPresenceOnly)
							//	{
							//		/// Иконка номер 2
							//		hChild = m_wndTreeView.InsertItem(tempV[j].def.defWord,2,2,hGroup);
							//		/// Серый цвет
							//		m_wndTreeView.SetItemColor(hChild,RGB(140,140,140));
							//		NCTreeItemData *pData = new NCTreeItemData();
							//		pData->type = I_DEFINITION;
							//		pData->description = tempV[j].def.briefDescr;
							//		pData->def.sectName = unitDef->sectionList[i].name;
							//		pData->def.known = tempV[j].def.known;
							//		m_wndTreeView.SetItemData(hChild, (DWORD_PTR)pData);
							//	}//if
							//}//else
						}//if
					}//for
				}//for
			}//if
			else
			{
				/// Не показываем группы

				/// Делаем копию вектора с определениями
				vector<Definition> tempV;
				tempV = unitDef->sectionList[i].defList;

				/// Если установлен флаг ShowAlphaSort, сортируем вектор по алфавиту
				if(ShowAlphaSort)
					sort(tempV.begin(), tempV.end());

				for(unsigned int j = 0; j < tempV.size(); j++)
				{
					if(tempV[j].GetType() == D_COMMON)
					{
						local::InsertDef(m_wndTreeView, hSect, tempV[j].def, unitDef->sectionList[i].name, ShowPresenceOnly); 
						//if(tempV[j].def.presence)
						//{
						//	hChild = m_wndTreeView.InsertItem(tempV[j].def.defWord,1,1,hSect);
						//	m_wndTreeView.SetItemColor(hChild,RGB(0,30,0));
						//	NCTreeItemData *pData = new NCTreeItemData();
						//	pData->type = I_DEFINITION;
						//	pData->description = tempV[j].def.briefDescr;
						//	pData->def.sectName = unitDef->sectionList[i].name;
						//	m_wndTreeView.SetItemData(hChild, (DWORD_PTR)pData);
						//}//if
						//else
						//{
						//	if(!ShowPresenceOnly)
						//	{
						//		hChild = m_wndTreeView.InsertItem(tempV[j].def.defWord,2,2,hSect);
						//		m_wndTreeView.SetItemColor(hChild,RGB(140,140,140));
						//		NCTreeItemData *pData = new NCTreeItemData();
						//		pData->type = I_DEFINITION;
						//		pData->description = tempV[j].def.briefDescr;
						//		pData->def.sectName = unitDef->sectionList[i].name;
						//		m_wndTreeView.SetItemData(hChild, (DWORD_PTR)pData);
						//	}//if
						//}//else

					}//if
					else
						ASSERT(FALSE);
				}//for
			}//else
		}//if
		else
		{
			/// Word Replacement
			for(unsigned int j = 0; j < unitDef->sectionList[i].defList.size(); j++)
			{
				if(unitDef->sectionList[i].defList[j].GetType() == D_WREPL)
				{
					HTREEITEM hChild;
					hChild = m_wndTreeView.InsertItem(_T("Replace"),3,3,hSect);

					m_wndTreeView.SetItemColor(hChild, RGB(0,0,30));

					NCTreeItemData *pData = new NCTreeItemData();
					pData->type = I_WREPL;
					pData->description = _T("Word Replacement");
					pData->wrep.srcStr = unitDef->sectionList[i].defList[j].wRep.srcString;
					pData->wrep.dstStr = unitDef->sectionList[i].defList[j].wRep.destString;
					m_wndTreeView.SetItemData(hChild, (DWORD_PTR)pData);
				}//if
				else
				{
					ASSERT(false);
				}//else
			}//for
		}//else
	}//for
	if(snapshot != NULL)
		ApplySnapshot(snapshot);
}//void CNCUnitTreePane::ShowNCUnitDef(CNCUnitDef * unitDef)

void CNCUnitTreePane::RebuildTree()
{
	CNCTreeCtrlSnapshot snapshot;
	GetTreeCtrlSnapshot(snapshot);
	ShowNCUnitDefInternal(m_unitDef, &snapshot);
}//void CNCUnitTreePane::ReBuildTree()

void CNCUnitTreePane::GetTreeCtrlSnapshot(CNCTreeCtrlSnapshot &snapshot)
{
	class local
	{
	public:
		/// На вход передается первый элемент среди соседей, которые обрабатываются в данном вызове функции
		static void processChilds(CNCUnitTreeCtrl &treeCtrl, HTREEITEM hItem, vector<CNCTreeCtrlSnapshot::NodeStatus> &nodeList)
		{
			/// Перебираем всех соседей элемента
			for( HTREEITEM hCurItem = hItem;
				 hCurItem != NULL ;
				 hCurItem = treeCtrl.GetNextItem(hCurItem, TVGN_NEXT))
			{
				/// Если у элемента есть хотя бы один ребенок, вызываем функцию для него
				HTREEITEM hChild = treeCtrl.GetNextItem(hCurItem, TVGN_CHILD);
				if(hChild != NULL)
					processChilds(treeCtrl, hChild, nodeList);

				CNCTreeCtrlSnapshot::NodeStatus nodeStatus;
				CNCTreeCtrlSnapshot::Node node;

				NCTreeItemData data = *(NCTreeItemData*)treeCtrl.GetItemData(hCurItem);

				switch(data.type)
				{
				case I_SECTION:
					node.type = NT_SECTION;
					node.Sect.sectName = treeCtrl.GetItemText(hCurItem);
					break;
				case I_DEFINITION:
					{
						node.type = NT_DEFINITION;
						/// Имя определения содержится в тексте элемента дерева
						node.Def.defName = treeCtrl.GetItemText(hCurItem);
						/// Получаем имя секции
						NCTreeItemData *pData = (NCTreeItemData*)treeCtrl.GetItemData(hCurItem);
						node.Def.sectName = pData->def.sectName;
					}
					break;
				case I_WREPL:
					{
						node.type = NT_WREPLACEMENT;
						NCTreeItemData Data = *(NCTreeItemData*)treeCtrl.GetItemData(hCurItem);
						node.WRepl.srcString = Data.wrep.srcStr;
						node.WRepl.dstString = Data.wrep.dstStr;
					}
					break;
				case I_GROUP:
					node.type = NT_GROUP;
					node.Group.groupName = treeCtrl.GetItemText(hCurItem);
					NCTreeItemData *pData = (NCTreeItemData*)treeCtrl.GetItemData(hCurItem);
					node.Group.sectName = pData->group.sectName;
					break;
				}//switch

				nodeStatus.node = node;
				if(treeCtrl.GetItemState( hCurItem, TVIS_EXPANDED) & TVIS_EXPANDED)
					nodeStatus.Expanded = true;
				else
					nodeStatus.Expanded = false;
				if(treeCtrl.GetSelectedItem() == hCurItem)
					nodeStatus.Selected = true;
				else
					nodeStatus.Selected = false;

				nodeList.push_back(nodeStatus);
				
			}//for
		};
	};
	///test
	//m_wndTreeView.GetNextItem(hItem, TVGN_CHILD)
	////////
	
	/// Очищаем вектор
	while(!snapshot.snapshot.empty())
		snapshot.snapshot.pop_back();

	HTREEITEM hFirstItem = m_wndTreeView.GetRootItem();
	/// Вызываем локальную рекурсивную функцию для первого элемента 0 уровня
	local::processChilds(m_wndTreeView, hFirstItem, snapshot.snapshot);
	
}//void CNCUnitTreePane::GetTreeCtrlSnapshot(CNCTreeCtrlSnapshot &snapthot)


CNCEditor2Doc * CNCUnitTreePane::GetDocument()
{
	CFrameWnd *Parent = GetParentFrame();
	return (CNCEditor2Doc*)((CMainFrame*)Parent)->GetActiveDocument();
}//CNCEditor2Doc * CNCUnitTreePane::GetDocument()

void CNCUnitTreePane::SetInternalSelect(bool IS)
{
	m_wndTreeView.InternalSelect = IS;
}//void CNCEditor2Doc::SetInternalSelect(bool IS)

void CNCUnitTreePane::ApplySnapshot(CNCTreeCtrlSnapshot *snapshot)
{
	class local
	{
	public:
		/// На вход передается первый элемент среди соседей, которые обрабатываются в данном вызове функции
		static void processChilds(CNCUnitTreeCtrl &treeCtrl, HTREEITEM hItem, CNCTreeCtrlSnapshot *snapshot)
		{
			for( HTREEITEM hCurItem = hItem;
				 hCurItem != NULL ;
				 hCurItem = treeCtrl.GetNextItem(hCurItem, TVGN_NEXT))
			{
				HTREEITEM hChild = treeCtrl.GetNextItem(hCurItem, TVGN_CHILD);
				if(hChild != NULL)
					processChilds(treeCtrl, hChild, snapshot);

				NCTreeItemData iData = *(NCTreeItemData*)treeCtrl.GetItemData(hCurItem);
				switch(iData.type)
				{
				case I_SECTION:
					{
						CString sectName = treeCtrl.GetItemText(hCurItem);
						if(snapshot->GetSectSelected(sectName))
							treeCtrl.Select(hCurItem, TVGN_CARET);
						if(snapshot->GetSectExpanded(sectName))
							treeCtrl.Expand(hCurItem, TVE_EXPAND);
						else
							treeCtrl.Expand(hCurItem, TVE_COLLAPSE);
					}
					break;
				case I_DEFINITION:
					{
						CString defName = treeCtrl.GetItemText(hCurItem);
						NCTreeItemData tiData = *(NCTreeItemData*)treeCtrl.GetItemData(hCurItem);
						CString sectName = tiData.def.sectName;
						if(snapshot->GetDefSelected(sectName, defName))
							treeCtrl.Select(hCurItem, TVGN_CARET);
					}
					break;
				case I_WREPL:
					{
					}
					break;
				case I_GROUP:
					{
						CString groupName = treeCtrl.GetItemText(hCurItem);
						NCTreeItemData tiData = *(NCTreeItemData*)treeCtrl.GetItemData(hCurItem);
						CString sectName = tiData.group.sectName;
						if(snapshot->GetGroupSelected(sectName, groupName))
							treeCtrl.Select(hCurItem, TVGN_CARET);
						if(snapshot->GetGroupExpanded(sectName, groupName))
							treeCtrl.Expand(hCurItem, TVE_EXPAND);
						else
							treeCtrl.Expand(hCurItem, TVE_COLLAPSE);
					}
					break;
				}//switch
			}//for
		};
	};
	
	HTREEITEM hRoot = m_wndTreeView.GetRootItem();
	local::processChilds(m_wndTreeView, hRoot, snapshot);
}//void CNCUnitTreePane::ApplySnapshot(CNCTreeCtrlSnapshot *snapshot)

void CNCUnitTreePane::Expand()
{
	/// Если активен режим ShowGroups, параметр ExpandState имеет 3 состояния
	if(ShowGroups)
	{
		if(ExpandState == 0)
		{
			ExpandState = 1;

			HTREEITEM hRoot = m_wndTreeView.GetRootItem();
			for(HTREEITEM hCur = hRoot;
				hCur != NULL;
				hCur = m_wndTreeView.GetNextItem(hCur, TVGN_NEXT))
			{
				m_wndTreeView.Expand(hCur, TVE_EXPAND);

				HTREEITEM hChild = m_wndTreeView.GetChildItem(hCur);
				if(hChild != NULL)
				{
					for(HTREEITEM hCur2 = hChild;
						hCur2 != NULL;
						hCur2 = m_wndTreeView.GetNextItem(hCur2, TVGN_NEXT))
					{
						m_wndTreeView.Expand(hCur2, TVE_COLLAPSE);
					}//for
				}//if
			}//for
		}//if
		else
		if(ExpandState == 1)
		{
			ExpandState = 2;

			HTREEITEM hRoot = m_wndTreeView.GetRootItem();
			for(HTREEITEM hCur = hRoot;
				hCur != NULL;
				hCur = m_wndTreeView.GetNextItem(hCur, TVGN_NEXT))
			{
				m_wndTreeView.Expand(hCur, TVE_EXPAND);
				
				HTREEITEM hChild = m_wndTreeView.GetChildItem(hCur);
				if(hChild != NULL)
				{
					for(HTREEITEM hCur2 = hChild;
						hCur2 != NULL;
						hCur2 = m_wndTreeView.GetNextItem(hCur2, TVGN_NEXT))
					{
						m_wndTreeView.Expand(hCur2, TVE_EXPAND);
					}//for
				}//if
			}//for
		}//if
		else
		if(ExpandState == 2)
		{
			ExpandState = 0;

			HTREEITEM hRoot = m_wndTreeView.GetRootItem();
			for(HTREEITEM hCur = hRoot;
				hCur != NULL;
				hCur = m_wndTreeView.GetNextItem(hCur, TVGN_NEXT))
			{
				
				HTREEITEM hChild = m_wndTreeView.GetChildItem(hCur);
				if(hChild != NULL)
				{
					for(HTREEITEM hCur2 = hChild;
						hCur2 != NULL;
						hCur2 = m_wndTreeView.GetNextItem(hCur2, TVGN_NEXT))
					{
						m_wndTreeView.Expand(hCur2, TVE_COLLAPSE);
					}//for
				}//if

				m_wndTreeView.Expand(hCur, TVE_COLLAPSE);
			}//for
		}//if
	}//if
	/// Если не активен режим ShowGroups, 2 состояния
	else
	{
		if(ExpandState == 0)
		{
			ExpandState = 1;

			HTREEITEM hRoot = m_wndTreeView.GetRootItem();
			for(HTREEITEM hCur = hRoot;
				hCur != NULL;
				hCur = m_wndTreeView.GetNextItem(hCur, TVGN_NEXT))
				m_wndTreeView.Expand(hCur, TVE_EXPAND);
		}//if
		else
		if(ExpandState == 1 || ExpandState == 2)
		{
			ExpandState = 0;

			HTREEITEM hRoot = m_wndTreeView.GetRootItem();
			for(HTREEITEM hCur = hRoot;
				hCur != NULL;
				hCur = m_wndTreeView.GetNextItem(hCur, TVGN_NEXT))
				m_wndTreeView.Expand(hCur, TVE_COLLAPSE);
		}//if
	}//else
}//void CNCUnitTreePane::Expand()

void CNCUnitTreePane::OnClassAddMemberFunction()
{
}

void CNCUnitTreePane::OnClassAddMemberVariable()
{
	// TODO: Add your command handler code here
}

void CNCUnitTreePane::OnClassDefinition()
{
	// TODO: Add your command handler code here
}

void CNCUnitTreePane::OnClassProperties()
{
	// TODO: Add your command handler code here
}

void CNCUnitTreePane::OnChangeXML()
{
	CNCSelectXmlDlg dlg;
	CString curFile = GetDocument()->GetCurFile();
	CString path = theApp.workPath;
	m_wndTreeView.InternalSelect = true;
	if(dlg.DoModal(path,curFile) == IDOK)
	{
		GetDocument()->ReloadXML(dlg.xmlName);
	}//if
	m_wndTreeView.InternalSelect = false;
}

void CNCUnitTreePane::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_wndTreeView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CNCUnitTreePane::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndTreeView.SetFocus();
}

void CNCUnitTreePane::OnChangeVisualStyle()
{
	m_ClassViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("Can't load bitmap: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_ClassViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_ClassViewImages.Add(&bmp, RGB(255, 0, 0));

	m_wndTreeView.SetImageList(&m_ClassViewImages, TVSIL_NORMAL);

	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT_24 : IDR_SORT, 0, 0, TRUE /* Locked */);
}

void CNCUnitTreePane::OnTBExpand()
{
	Expand();
}//void CNCUnitTreePane::OnTBExpand()

void CNCUnitTreePane::OnTBPres()
{
	ShowPresenceOnly = !ShowPresenceOnly;
	RebuildTree();
}//void CNCUnitTreePane::OnTBPres()

void CNCUnitTreePane::OnTBABC()
{
	ShowAlphaSort = !ShowAlphaSort;
	RebuildTree();
}//void CNCUnitTreePane::OnTBABC()

void CNCUnitTreePane::OnTBGroup()
{
	ShowGroups = !ShowGroups;
	RebuildTree();
}//void CNCUnitTreePane::OnTBGroup()

void CNCUnitTreePane::OnUpdateExpand(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(ExpandState == 1 || ExpandState == 2);
}//void CNCUnitTreePane::OnUpdateExpand(CCmdUI *pCmdUI)

void CNCUnitTreePane::OnUpdatePres(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(ShowPresenceOnly);
}//void CNCUnitTreePane::OnUpdatePres(CCmdUI *pCmdUI)

void CNCUnitTreePane::OnUpdateABC(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(ShowAlphaSort);
}//void CNCUnitTreePane::OnUpdateABC(CCmdUI *pCmdUI)

void CNCUnitTreePane::OnUpdateGroup(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(ShowGroups);
}//void CNCUnitTreePane::OnUpdateGroup(CCmdUI *pCmdUI)

BOOL CNCUnitTreePane::DestroyWindow()
{
	m_wndTreeView.DestroyWindow();
	return CDockablePane::DestroyWindow();
}