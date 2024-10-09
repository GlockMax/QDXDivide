// NRibbonBar.cpp : implementation file
//

#include "stdafx.h"
#include "ResInv.h"
#include "ResLocal.h"
#include "NCM.h"
#include "NRibbonBar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

extern CNCMApp theApp;
// NRibbonBar

IMPLEMENT_DYNAMIC(NRibbonBar, CMFCRibbonBar)

NRibbonBar::NRibbonBar()
{
	Initialize();
}

NRibbonBar::~NRibbonBar()
{
}


BEGIN_MESSAGE_MAP(NRibbonBar, CMFCRibbonBar)
//	ON_WM_LBUTTONUP()
ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

void NRibbonBar::Initialize()
{
	IsStartButtonActive = false;
	//	PrevCommandID = 0;
	pMachCombo = nullptr;
	pNCUnitCombo = nullptr;
	pToolsCombo = nullptr;
}

BOOL NRibbonBar::LoadState(LPCTSTR lpszProfileName, int nIndex, UINT uiID)
{
	if (!theApp.AllowLoadRibbonState)
		return TRUE;
	return CMFCRibbonBar::LoadState(lpszProfileName, nIndex, uiID);
}

BOOL NRibbonBar::SaveState(LPCTSTR lpszProfileName, int nIndex, UINT uiID)
{
	if (!theApp.AllowLoadRibbonState)
		return TRUE;
	return CMFCRibbonBar::SaveState(lpszProfileName, nIndex, uiID);
}

BOOL NRibbonBar::LoadFromResource(UINT uiXMLResID, LPCTSTR lpszResType, HINSTANCE hInstance)
{
	//Save category
	int CategoryIndex = -1;
	auto pActiveCategory = GetActiveCategory();
	if(pActiveCategory != nullptr)
		CategoryIndex = GetCategoryIndex(pActiveCategory);

	// Save minimized state
	bool IsMinimized = (GetHideFlags() & AFX_RIBBONBAR_HIDE_ELEMENTS);

	Initialize();
	if (GetCategoryCount() > 1)
	{
		SaveState(theApp.GetRegSectionPath());
		RemoveAllCategories();
		RemoveAllFromTabs();
	}

	CMFCRibbonBar::LoadFromResource(uiXMLResID, lpszResType, hInstance);

	// Restore category
	if (CategoryIndex >= 0)
	{
		if (!(GetHideFlags() & AFX_RIBBONBAR_HIDE_ELEMENTS)) // Ribbon is not minimized
		{
			auto pNewCategory = GetCategory(CategoryIndex);
				if (pNewCategory != nullptr)
					SetActiveCategory(pNewCategory);
		}
	}

	// Mid align all edit controls with same ID
	CArray<CMFCRibbonBaseElement *, CMFCRibbonBaseElement *> Elements;
	GetElementsByID(IDC_STEP_NUM, Elements);
	for (int i = 0; i < Elements.GetSize(); ++i)
	{
		CMFCRibbonEdit *pEdit = static_cast<CMFCRibbonEdit *>(Elements[i]);
		if (pEdit)
			pEdit->SetTextAlign(ES_CENTER);// Нет в XML
	}

	// Combo boxes
	pNCUnitCombo = static_cast<CMFCRibbonComboBox *>(FindByID(ID_NCUNIT_COMBO, FALSE, TRUE));
	if (pNCUnitCombo != nullptr)
	{
		LoadNCUnitCombo();
		pNCUnitCombo->SelectItem(NCUnitName);
	}

	pMachCombo = static_cast<CMFCRibbonComboBox *>(FindByID(ID_MACH_COMBO, FALSE, TRUE));
	if (pMachCombo != nullptr)
	{
		LoadMachCombo();
		pMachCombo->SelectItem(MachName);
	}
	pToolsCombo = static_cast<CMFCRibbonComboBox *>(FindByID(ID_TOOLS_COMBO, FALSE, TRUE));
	if (pToolsCombo != nullptr)
	{
		pToolsCombo->SelectItem(-1);
	}

	//Work ribbon
	CMFCRibbonCategory *pCategory = GetCategory(0);
	if (pCategory)
	{
		CMFCRibbonPanel *pPanel = pCategory->GetPanel(1);
		if (pPanel)
		{
			pPanel->SetCenterColumnVert(TRUE);// Есть в XML
			// Make buttons group 
			CMFCRibbonButtonsGroup *pGroup = new CMFCRibbonButtonsGroup();

			if (pGroup)
			{
				Add2Group(*pGroup, *pPanel, ID_ANIMATE_PAUSE);
				CMFCRibbonSeparator *pSeparator = new CMFCRibbonSeparator();
				pGroup->AddButton(pSeparator);
				Add2Group(*pGroup, *pPanel, ID_BREAKPOINT_BACK);
				pSeparator = new CMFCRibbonSeparator();
				pGroup->AddButton(pSeparator);
				Add2Group(*pGroup, *pPanel, ID_BREAKPOINT_FORWARD);
				pSeparator = new CMFCRibbonSeparator();
				pGroup->AddButton(pSeparator);
				Add2Group(*pGroup, *pPanel, ID_STEP_BACK);
				Add2Group(*pGroup, *pPanel, IDC_STEP_NUM);
				Add2Group(*pGroup, *pPanel, ID_STEP_FOR);

				pPanel->Insert(pGroup, 1);// Есть в XML
			}
			// Make buttons group Slaider
			pGroup = new CMFCRibbonButtonsGroup();
			if (pGroup)
			{
				Add2Group(*pGroup, *pPanel, ID_SPEED_SLD);
				CMFCRibbonSeparator *pSeparator = new CMFCRibbonSeparator();
				pGroup->AddButton(pSeparator);
				Add2Group(*pGroup, *pPanel, ID_VIEW_REGEN);
				pPanel->Insert(pGroup, 2);// Есть в XML
			}
		}
		pPanel = pCategory->GetPanel(2);//Stop points
		pPanel->EnableLaunchButton(ID_BREAKPOINT_TABLE);// Есть в XML
		pPanel = pCategory->GetPanel(3);//Window
		pPanel->EnableLaunchButton(ID_O_WINDOWS);
	}

	UINT RibbonID = AFX_IDW_RIBBON_BAR;
	switch (uiXMLResID)
	{
	case IDR_RIBBON:
		RibbonID = AFX_IDW_RIBBON_BAR;
		break;
	case IDR_RIBBON_MILL:
		RibbonID = AFX_IDW_RIBBON_BAR + 1;
		break;
	case IDR_RIBBON_MILLTURN:
		RibbonID = AFX_IDW_RIBBON_BAR + 2;
		break;
	case IDR_RIBBON_WELD:
		RibbonID = AFX_IDW_RIBBON_BAR + 3;
		break;
	}
	SetDlgCtrlID(RibbonID);
	LoadState(theApp.GetRegSectionPath());
	// Restore minimized state
	if (IsMinimized != (GetHideFlags() & AFX_RIBBONBAR_HIDE_ELEMENTS))
		ToggleMimimizeState();

	ForceRecalcLayout();

	return TRUE;
}

void NRibbonBar::SetStartStopButton(bool Stop)
{
	IsStartButtonActive = !Stop;
}

int NRibbonBar::MOnToolsCombo(CString &Text)
{
	// Returns -1 if error 0 if an element selected and +1 - if last element selected
	// Returns true if an item has been selected
	if (pToolsCombo == nullptr)
		return -1;
	auto CurSel = pToolsCombo->GetCurSel();
	if (CurSel == pToolsCombo->GetCount() - 1)
		return 1;
	Text = pToolsCombo->GetItem(CurSel);
	return 0;
}

bool NRibbonBar::MOnNCUnitCombo()
{
	// Returns true if an item has been selected
	if (pNCUnitCombo == nullptr)
		return false;
	auto CurSel = pNCUnitCombo->GetCurSel();
	if (CurSel == pNCUnitCombo->GetCount() - 1)
	{
		LoadNCUnitCombo();
		return false;
	}
	return true;
}

bool NRibbonBar::MOnMachCombo()
{
	// Returns true an item has been selected
	if (pMachCombo == nullptr)
		return false;
	auto CurSel = pMachCombo->GetCurSel();
	if (CurSel == pMachCombo->GetCount() - 1)
	{
		LoadMachCombo();
		return false;
	}
	return true;
}

void NRibbonBar::LoadToolsCombo(const std::vector<CString> &FullNames)
{
	if (pToolsCombo == nullptr)
		return;

	auto CurSel = pToolsCombo->GetCurSel();
	pToolsCombo->RemoveAllItems();
	for (size_t i = 0; i < FullNames.size(); ++i)
		pToolsCombo->AddItem(FullNames[i]);
	CString AddTool;
	AddTool.LoadString(IDS_TOOL_CREATE_TITLE);
	pToolsCombo->AddItem(AddTool);
	if (CurSel >= int(FullNames.size()))
		CurSel = 0;
	pToolsCombo->SelectItem(CurSel);
}

void NRibbonBar::LoadNCUnitCombo()
{
	if (pNCUnitCombo == nullptr)
		return;

	CArray<CString> Names;
	NCM_PROJECT.ReadDirFiles(NCM_PROJECT.GetUnitsPath(), _T("*.sm*"), Names);
	
	pNCUnitCombo->RemoveAllItems();
	for (int i = 0; i < Names.GetSize(); ++i)
		pNCUnitCombo->AddItem(Names[i]);
	pNCUnitCombo->AddItem(_T("...Refresh..."));
}

void NRibbonBar::LoadMachCombo()
{
	if (pMachCombo == nullptr)
		return;

	CArray<CString> Names;
	NCM_PROJECT.ReadDirFiles(NCM_PROJECT.GetMachinesPath(), _T("*.*"), Names);
	
	pMachCombo->RemoveAllItems();
	for (int i = 0; i < Names.GetSize(); ++i)
		pMachCombo->AddItem(Names[i]);
	pMachCombo->AddItem(_T("...Refresh..."));
}

void NRibbonBar::SetNCUnitName(const CString &NewName)
{
	if (NCUnitName == NewName)
		return;

	NCUnitName = NewName;

	if (pNCUnitCombo == nullptr)
		return;

	pNCUnitCombo->SelectItem(NCUnitName);
}

void NRibbonBar::SetMachName(const CString &NewName)
{
	if (MachName == NewName)
		return;

	MachName = NewName;

	if (pMachCombo == nullptr)
		return;

	pMachCombo->SelectItem(MachName);
}

const CString NRibbonBar::GetToolsSelection()
{
	return pToolsCombo->GetText();
}

const CString NRibbonBar::GetNCUnitNewName()
{
	const CString &ExName = pNCUnitCombo->GetItem(pNCUnitCombo->GetCurSel());
	return ExName;
}

const CString NRibbonBar::GetMachNewName()
{
	const CString &ExName = pMachCombo->GetItem(pMachCombo->GetCurSel());
	return ExName;
}


bool NRibbonBar::Add2Group(CMFCRibbonButtonsGroup & Group, CMFCRibbonPanel & Panel, UINT ElementID)
{
	CMFCRibbonBaseElement *pElement = Panel.FindByID(ElementID);
	if (pElement == nullptr)
		return false;
	auto Index = Panel.GetIndex(pElement);
	Panel.Remove(Index, FALSE);
	Group.AddButton(pElement);
	return true;
}

BOOL NRibbonBar::PreTranslateMessage(MSG* pMsg)
{
	return CMFCRibbonBar::PreTranslateMessage(pMsg);
}

void NRibbonBar::OnLButtonUp(UINT nFlags, CPoint point)
{
	CMFCRibbonBar::OnLButtonUp(nFlags, point);
}

bool NRibbonBar::IsMinimized() const
{
	return m_dwHideFlags == AFX_RIBBONBAR_HIDE_ELEMENTS;
}

void NRibbonBar::UpdateStartCollcheck(bool CollFound, bool LogIsStale, bool CheckingIsRunning)
{
	CMFCRibbonButton* pStartColl = static_cast<CMFCRibbonButton*>(FindByID(ID_START_COLLCHECK, FALSE, TRUE));
	if (pStartColl != nullptr)
	{
		const int NewData = CheckingIsRunning ? (CollFound ? 1 : 2) : (LogIsStale ? 3 : (CollFound ? 1 : 0));// 1 - red; 2 - green; 3 - yellow
		constexpr int StopInd = 7;
		constexpr int StartInd = 19;
		constexpr int GreyStartInd = 21;
		const int NewInd = CheckingIsRunning ? StopInd : (LogIsStale ? StartInd : GreyStartInd);
		if(pStartColl->GetImageIndex(TRUE) != NewInd || pStartColl->GetData() != NewData)
		{ 
			pStartColl->SetData(NewData);
			pStartColl->SetImageIndex(NewInd , TRUE);
			pStartColl->Redraw();
			int DescID = 0;
			if (CheckingIsRunning)
				DescID = IDS_TOOL_DIAM;// Выполняется проверка. Можно остановить.
			else if (LogIsStale)
				DescID = IDS_TOOL_EDIT_TITLE;// Проверка не выполнена. Можно начать.
			else if(CollFound)
				DescID = IDS_TOOL_DLINA;// Проверка выполнена. Есть столкновения. Можно начать проверку заново.
			else
				DescID = IDS_TOOL_CREATE_TITLE;// Проверка выполнена. Нет столкновений. Можно начать проверку заново.
			CString Desc;
			Desc.LoadString(DescID);
			pStartColl->SetDescription(Desc);
		}
	}
}

void NRibbonBar::UpdateStart()
{
	CMFCRibbonButton* pSt = static_cast<CMFCRibbonButton*>(FindByID(ID_RUN_TO_BREAKPOINT, FALSE, TRUE));
	if (pSt != nullptr)
	{
		constexpr int StopInd = 7;
		constexpr int StartInd = 6;
		const int NewInd = IsStartButtonActive ? StartInd : StopInd;
		if (pSt->GetImageIndex(TRUE) != NewInd)
		{
			int DescID = 0, TextID = 0;
			if (IsStartButtonActive)
			{
				DescID = ID_RUN_TO_BREAKPOINT;
				TextID = IDS_ANGLE_A;
			}
			else
			{
				DescID = ID_STEP_STOP;
				TextID = IDS_ANGLE_B;
			}
			CString Desc;
			Desc.LoadString(DescID);
			Desc = Desc.SpanExcluding(_T("\n"));
			CString Text;
			Text.LoadString(TextID);

			CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arButtons;
			GetElementsByID(ID_RUN_TO_BREAKPOINT, arButtons);
			for (int i = 0; i < arButtons.GetSize(); ++i)
			{
				CMFCRibbonButton* pStart = static_cast<CMFCRibbonButton*>(arButtons[i]);
				pStart->SetImageIndex(NewInd, TRUE);
				pStart->SetImageIndex(NewInd, FALSE);
				pStart->SetDescription(Desc);
				pStart->SetText(Text);
				pStart->Redraw();
			}
		}
	}
}
