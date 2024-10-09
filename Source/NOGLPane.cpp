// NOGLPane.cpp : implementation file
//

#include "stdafx.h"
#include "NCM.h"
#include "MainFrm.h"
#include "NOGLPane.h"
#include "NColor.h"
#include "NControlPanel.h"
#include "NOGLStockWnd.h"
#include "NOGLMashWnd.h"
#include "NOpenGLWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CNCMApp theApp;
/////////////////////////////////////////////////////////////////////////////
// NOGLPane

//IMPLEMENT_DYNCREATE(NOGLPane, CDockablePane)
int NOGLPane::WinIDs[] = { IDS_STOCK_MACH_PANE_1, IDS_STOCK_MACH_PANE_2, IDS_STOCK_MACH_PANE_3, IDS_STOCK_MACH_PANE_4
							, IDS_STOCK_MACH_PANE_5, IDS_STOCK_MACH_PANE_6, IDS_STOCK_MACH_PANE_7, IDS_STOCK_MACH_PANE_8
							, IDS_STOCK_MACH_PANE_9, IDS_STOCK_MACH_PANE_10, IDS_STOCK_MACH_PANE_11, IDS_STOCK_MACH_PANE_12
							, IDS_STOCK_MACH_PANE_13, IDS_STOCK_MACH_PANE_14, IDS_STOCK_MACH_PANE_15, IDS_STOCK_MACH_PANE_16 };

IMPLEMENT_SERIAL(NOGLPane, CDockablePane, 0)

NOGLPane::NOGLPane(NOpenGLWnd *pWnd, int WID, bool ShowOnMenu)
{
	pOpenGLWnd = pWnd;
	WndID = WID;
	ShowOnPaneMenu = ShowOnMenu;
	pOpenGLWndStock = nullptr;
	pOpenGLWndMach = nullptr;
}

NOGLPane::NOGLPane(bool StockType, int WID, bool ShowOnMenu)
{
	WndID = WID;
	ShowOnPaneMenu = ShowOnMenu;
	pOpenGLWndStock = new NOGLStockWnd;
	pOpenGLWndMach = new NOGLMashWnd;
	pOpenGLWnd = StockType ? pOpenGLWndStock : pOpenGLWndMach;
}

NOGLPane::~NOGLPane()
{
	delete pOpenGLWndStock;
	delete pOpenGLWndMach;
}


BEGIN_MESSAGE_MAP(NOGLPane, CDockablePane)
	//{{AFX_MSG_MAP(NOGLPane)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_VOLUME_TREE, OnVolumeTree)
	ON_COMMAND(ID_TB_VIEW_CHANHGETYPE, OnChangeType)
	ON_UPDATE_COMMAND_UI(ID_TB_VIEW_CHANHGETYPE, OnUpdateChangeType)
	//}}AFX_MSG_MAP
//	ON_WM_PAINT()
//	ON_WM_SHOWWINDOW()
//	ON_WM_NCPAINT()
END_MESSAGE_MAP()




/////////////////////////////////////////////////////////////////////////////
// NOGLPane diagnostics

#ifdef _DEBUG
void NOGLPane::AssertValid() const
{
	CDockablePane::AssertValid();
}

void NOGLPane::Dump(CDumpContext& dc) const
{
	CDockablePane::Dump(dc);
}
#endif //_DEBUG
#define AFX_REG_SECTION_FMT    _T("%sOGLPane-%d")
#define AFX_REG_SECTION_FMT_EX _T("%sOGLPane-%d%x")

/////////////////////////////////////////////////////////////////////////////
// NOGLPane message handlers

void NOGLPane::OnUpdate(CDockablePane* pSender, LPARAM lHint, CObject* pHint) 
{
	if (IsPaneVisible() || (lHint & N_NEWDOC))
	{
		pOpenGLWnd->OnUpdate(pSender, lHint, pHint);
	}
}

void NOGLPane::OnSize(UINT nType, int cx, int cy) 
{
	CDockablePane::OnSize(nType, cx, cy);
	
	if (pOpenGLWnd->GetSafeHwnd() != NULL)
	{
		CRect wrect;
		GetClientRect(&wrect);
		pOpenGLWnd->MoveWindow(wrect, TRUE);
	}
}

int NOGLPane::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;
	CreateFirst();
	return 0;
}

void NOGLPane::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	if(pOldWnd == this)
		return;
	if(pOpenGLWnd->m_hWnd)
		pOpenGLWnd->SetFocus();
}

void NOGLPane::OnChangeType()
{
	auto pMain = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	if (pMain != nullptr)
	{
		if (pMain->GetMainPane() == this)
		{
			if (GetType())
				pMain->OnWinMach();
			else
				pMain->OnWinStock();
			return;
		}
	}
	SetType(!GetType());
	OnUpdate(NULL, N_SETTINGS, nullptr);
}

void NOGLPane::OnUpdateChangeType(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(theApp.GetConfig() & CM_Machine);
}

BOOL NOGLPane::LoadState(LPCTSTR lpszProfileName, int nIndex, UINT uiID)
{
	if (nIndex == -1)
	{
		nIndex = GetDlgCtrlID();
	}

	CString strSection;
	if (uiID == (UINT)-1)
	{
		strSection.Format(AFX_REG_SECTION_FMT, lpszProfileName, nIndex);
	}
	else
	{
		strSection.Format(AFX_REG_SECTION_FMT_EX, lpszProfileName, nIndex, uiID);
	}

	CSettingsStoreSP regSP;
	CSettingsStore& reg = regSP.Create(FALSE, TRUE);

	if (!reg.Open(strSection))
	{
		return FALSE;
	}

	int intStockType = 1;
	if(theApp.GetConfig() & CM_Machine)
		reg.Read(_T("StockType"), intStockType);
	SetType(intStockType != 0);

	return CDockablePane::LoadState(lpszProfileName, nIndex, uiID);
}
BOOL NOGLPane::SaveState(LPCTSTR lpszProfileName, int nIndex, UINT uiID)
{
	if (nIndex == -1)
	{
		nIndex = GetDlgCtrlID();
	}

	CString strSection;
	if (uiID == (UINT)-1)
	{
		strSection.Format(AFX_REG_SECTION_FMT, lpszProfileName, nIndex);
	}
	else
	{
		strSection.Format(AFX_REG_SECTION_FMT_EX, lpszProfileName, nIndex, uiID);
	}

	CSettingsStoreSP regSP;
	CSettingsStore& reg = regSP.Create(FALSE, FALSE);

	if (reg.CreateKey(strSection))
	{
		reg.Write(_T("StockType"), GetType());
	}

	return CDockablePane::SaveState(lpszProfileName, nIndex, uiID);
}
void NOGLPane::NAnimateUpdate(void)
{
	if(IsVisible())
		pOpenGLWnd->NAnimateUpdate();
}
void NOGLPane::LoadTBFromIcons(void)
{
	if(pOpenGLWnd->GetSafeHwnd() != NULL)
		pOpenGLWnd->LoadTBFromIcons();
}
BOOL NOGLPane::AllowShowOnPaneMenu() const
{
	return ShowOnPaneMenu;
}

void NOGLPane::SerializeMatrices(CArchive & ar)
{
	pOpenGLWndStock->SerializeMatrices(ar);
	pOpenGLWndMach->SerializeMatrices(ar);
}

bool NOGLPane::HasFocus(void)
{
	return (GetFocus() == pOpenGLWnd);
}

void NOGLPane::OnVolumeTree() 
{
	const int Count = 1000;

	glPushMatrix();
	double Angle = 360. / Count;
	for(int i = 0; i < Count; ++i)
	{
		glRotated(Angle, 1., 0., 0.);
		pOpenGLWnd->SetChangeView(1);
		pOpenGLWnd->NUpdate();
		pOpenGLWnd->RedrawWindow();
	}
	glPopMatrix();

}

void NOGLPane::SetType(bool StockType)
{
	if (pOpenGLWndStock == pOpenGLWndMach)
		return;// The type of this pane can't be changed
	if (StockType == GetType())
		return;
	bool NeedCreate = pOpenGLWnd->GetSafeHwnd() != NULL;

	if (NeedCreate)
		pOpenGLWnd->DestroyWindow();

	pOpenGLWnd = StockType ? pOpenGLWndStock : pOpenGLWndMach;
	SetWindowText(MakeTitle());

	if (NeedCreate)
	{
		CreateFirst();
	}
}

const CString NOGLPane::MakeTitle()
{
	CString Prefix;
	Prefix.Format(_T("#%d "), WndID);
	CString Title;
	Title.LoadString(GetType() ? ID_STOCKPANE : ID_MACHPANE);
	return Prefix + Title;
}

bool NOGLPane::GetType() const
{
	return pOpenGLWnd == pOpenGLWndStock;
}

void NOGLPane::SetRotCenter(const BOpenGLWnd::NRotCenter& iRotCenter)
{
	if (pOpenGLWnd != nullptr)
		pOpenGLWnd->SetRotCenter(iRotCenter);
	if (pOpenGLWndStock != nullptr)
		pOpenGLWndStock->SetRotCenter(iRotCenter);
	if (pOpenGLWndMach != nullptr)
		pOpenGLWndMach->SetRotCenter(iRotCenter);
}

bool NOGLPane::CreateFirst()
{
	CRect rect;
	GetClientRect(rect);
	bool Ret = (TRUE == pOpenGLWnd->CreateEx(WS_EX_CLIENTEDGE, NULL,   //CWnd default
		NULL,   //has no name
		WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE,
		rect,
		this,   //this is the parent
		WinIDs[WndID]));
	pOpenGLWnd->OnUpdate(NULL, N_SETTINGS, NULL);
	return Ret;
}

NOGLPane* NOGLPane::MakeOGLPane(bool StockType, int WID, CWnd* pParentWnd, bool HasGripper, bool ShowOnMenu)
{
	NOGLPane* pPane = new NOGLPane(StockType, WID, ShowOnMenu);
	if (!pPane->CreateEx(WS_EX_WINDOWEDGE, pPane->MakeTitle(), pParentWnd, CRect(0, 0, 200, 200), HasGripper, WinIDs[WID]
		, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create OpenGL Pane window\n");
		return nullptr; // failed to create
	}
	pPane->ShowPane(FALSE, FALSE, FALSE);
	pPane->EnableDocking(CBRS_ALIGN_ANY);
	HMODULE hins = GetModuleHandle(_T("ResInv.dll"));
	if (hins != nullptr)
	{
		HICON hStockViewIcon = (HICON) ::LoadImage(hins, MAKEINTRESOURCE(StockType ? IDI_STOCK : IDI_MACH), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
		pPane->SetIcon(hStockViewIcon, FALSE);
	}
	return pPane;
}
