#include "StdAfx.h"
#include "ResLocal.h"
#include "NCMDoc.h"
#include "BOpenGLWndVD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(BOpenGLWndVD, BOpenGLWnd)

BEGIN_MESSAGE_MAP(BOpenGLWndVD, BOpenGLWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_SAVE_VIEW1, OnSaveView1)
	ON_COMMAND(ID_SAVE_VIEW2, OnSaveView2)
	ON_COMMAND(ID_SAVE_VIEW3, OnSaveView3)
	ON_COMMAND(ID_SAVE_VIEW4, OnSaveView4)
	ON_COMMAND(ID_SAVE_VIEW5, OnSaveView5)
	ON_COMMAND(ID_TB_ZOOM_Y, OnTbZoomY)
	ON_COMMAND(ID_TB_SAVE, OnTbSave)
	ON_COMMAND(ID_TB_VIEW1, OnTbView1)
	ON_COMMAND(ID_TB_VIEW2, OnTbView2)
	ON_COMMAND(ID_TB_VIEW3, OnTbView3)
	ON_COMMAND(ID_TB_VIEW4, OnTbView4)
	ON_COMMAND(ID_TB_VIEW5, OnTbView5)
	ON_COMMAND(ID_TB_ZOOM_X, OnTbZoomX)
	ON_COMMAND(ID_TB_ZOOM_W, OnTbZoomW)
	ON_COMMAND(ID_TB_VIEW_RU, OnTbViewRU)
	ON_COMMAND(ID_TB_VIEW_LU, OnTbViewLU)
	ON_COMMAND(ID_TB_VIEW_RD, OnTbViewRD)
	ON_COMMAND(ID_TB_VIEW_LD, OnTbViewLD)
	ON_COMMAND(ID_TB_VIEW_L, OnTbViewL)
	ON_COMMAND(ID_TB_VIEW_R, OnTbViewR)
	ON_COMMAND(ID_TB_VIEW_B, OnTbViewB)
	ON_COMMAND(ID_TB_VIEW_F, OnTbViewF)
	ON_COMMAND(ID_TB_ZOOM_SC, OnTbZoomSc)
	ON_COMMAND(ID_TB_ZOOM_P, OnTbZoomP)
	ON_COMMAND(ID_TB_ZOOM_R, OnTbZoomR)
	ON_COMMAND(ID_TB_VIEW_ALONGTOOL, OnTbZoomTool)
	ON_COMMAND(ID_TB_ZOOM_Z, OnTbZoomZ)
	ON_COMMAND(ID_TB_VIEW_ORTHO, OnTbZoomNear)
	ON_COMMAND(ID_TB_VIEW_NORMALTOOBJECT, OnTbZoomNorm)
	ON_COMMAND(ID_TB_ZOOM_ALL, OnTbZoomAll)
	ON_COMMAND(ID_TB_REDRAW, OnTbRedraw)
	ON_COMMAND(ID_TB_RESET, OnTbReset)
	ON_COMMAND(ID_TB_VIEW_LA, OnTbViewLa)
	ON_COMMAND(ID_TB_VIEW_ZX, OnTbViewZx)
	ON_COMMAND(ID_TB_VIEW_XZ, OnTbVewXz)
	ON_COMMAND(ID_TB_VIEW_ZY, OnTbVewZy)
	ON_COMMAND(ID_TB_VIEW_XYLeft, OnTbViewXyleft)
	ON_COMMAND(ID_TB_VIEW_XYRight, OnTbViewXyright)
	ON_COMMAND(ID_TB_VIEW_T1, OnTbViewT1)
	ON_COMMAND(ID_TB_VIEW_T2, OnTbViewT2)
	ON_COMMAND(ID_TB_VIEW_T3, OnTbViewT3)
	ON_COMMAND(ID_TB_VIEW_T4, OnTbViewT4)
	ON_COMMAND(ID_TB_VIEW_PREV, OnTbViewPrev)
	ON_UPDATE_COMMAND_UI(ID_TB_VIEW_PREV, OnUpdateTbViewPrev)
	ON_COMMAND(ID_TB_VIEW_NEXT, OnTbViewNext)
	ON_UPDATE_COMMAND_UI(ID_TB_VIEW_NEXT, OnUpdateTbViewNext)
	ON_UPDATE_COMMAND_UI(ID_TB_SAVE, OnUpdateTbSave)
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

BOpenGLWndVD::BOpenGLWndVD() : BOpenGLWnd()
{
}

void BOpenGLWndVD::CloseToolBar()
{
	if (m_wndToolBar.GetSafeHwnd() != NULL)
		m_wndToolBar.ShowWindow(SW_HIDE);
	if (ViewDlg.GetSafeHwnd() != NULL)
		ViewDlg.ShowWindow(SW_HIDE);
}

int BOpenGLWndVD::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (BOpenGLWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!ViewDlg.Create(this))
	{
		TRACE0("Failed to create view dialog\n");
		return -1;      // fail to create
	}

	if (!m_wndToolBar.CreateEx(&ViewDlg, TBSTYLE_WRAPABLE, WS_CHILD | CBRS_FLOATING, CRect(0, 0, 0, 0), ID_VIEW_VIEW))
	{
		TRACE0("Failed to create view toolbar\n");
		return -1;      // fail to create
	}
	LoadTBFromIcons();
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() &
		~CBRS_HIDE_INPLACE);
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);

	CRect rect;
	m_wndToolBar.GetToolBarCtrl().SetRows(4, TRUE, rect);

	return 0;
}

void BOpenGLWndVD::ShowViewToolbar(CPoint point)
{
	ClearZoomActive();
	// Show toolbar
	ClientToScreen(&point);
	CToolBar& TB = m_wndToolBar;
	CRect rect;
	TB.GetWindowRect(&rect);
	rect.bottom = rect.Height();
	rect.right = rect.Width();
	rect.left = 0;
	rect.top = 0;
	rect += point;
	CRect t_rect;
	GetClientRect(&t_rect);
	ClientToScreen(&t_rect);
	CPoint move(max(0, rect.right - t_rect.right), max(0, rect.bottom - t_rect.bottom));
	rect -= move;
	if (rect.top < t_rect.top || rect.left < t_rect.left)
	{
		CPoint move1(min(0, rect.left - t_rect.left), min(0, rect.top - t_rect.top));
		rect -= move;
	}
	ViewDlg.GetParent()->ScreenToClient(&rect);
	TB.ShowWindow(SW_SHOWNORMAL);
	ViewDlg.MoveWindow(rect);
	ViewDlg.ShowWindow(SW_SHOWNORMAL);
}

void BOpenGLWndVD::OnTbZoomR()
{
	PrepTracker(ZoomRActive, 'A');
}
void BOpenGLWndVD::OnTbZoomX()
{
	PrepTracker(ZoomRActive, 'X');
}
void BOpenGLWndVD::OnTbZoomY()
{
	PrepTracker(ZoomRActive, 'Y');
}
void BOpenGLWndVD::OnTbZoomZ()
{
	PrepTracker(ZoomRActive, 'Z');
}

void BOpenGLWndVD::OnTbZoomNear()
{
	DoZoomNear();
}

void BOpenGLWndVD::OnTbZoomNorm()
{
	PrepZoomNorm();
}

void BOpenGLWndVD::PrepZoomNorm()
{
	ZoomNormActive = TRUE;
	SetCursorID('N');
}

void BOpenGLWndVD::SetDocModified()
{
	CDocument* pDoc = GetDocument();
	if (pDoc)
		pDoc->SetModifiedFlag();
}


void BOpenGLWndVD::OnTbZoomTool()
{
	DoZoomTool();
	SetDocModified();
}

void BOpenGLWndVD::OnTbZoomSc()
{
	PrepTracker(ZoomRActive, 'S');
}

void BOpenGLWndVD::OnTbZoomW()
{
	PrepTracker(ZoomWActive, 'W');
}

void BOpenGLWndVD::OnTbZoomP()
{
	PrepTracker(ZoomRActive, 'P');
}

void BOpenGLWndVD::OnTbRedraw()
{
	ClearZoomActive();
	Invalidate();
}

void BOpenGLWndVD::OnTbReset()
{
	DoTbReset();
	SetDocModified();
}

void BOpenGLWndVD::OnTbViewRU()
{
	DoTbViewRU();
	SetDocModified();
}

void BOpenGLWndVD::OnTbViewL()
{
	DoTbViewL();
	SetDocModified();
}

void BOpenGLWndVD::OnTbViewR()
{
	DoTbViewR();
	SetDocModified();
}

void BOpenGLWndVD::OnTbViewB()
{
	DoTbViewB();
	SetDocModified();
}

void BOpenGLWndVD::OnTbViewF()
{
	DoTbViewF();
	SetDocModified();
}

void BOpenGLWndVD::OnTbViewLU()
{
	DoTbViewLU();
	SetDocModified();
}

void BOpenGLWndVD::OnTbViewRD()
{
	DoTbViewRD();
	SetDocModified();
}

void BOpenGLWndVD::OnTbViewLD()
{
	DoTbViewLD();
	SetDocModified();
}

void BOpenGLWndVD::OnTbViewT1()
{
	DoTbViewT1();
	SetDocModified();
}

void BOpenGLWndVD::OnTbViewT2()
{
	DoTbViewT2();
	SetDocModified();
}

void BOpenGLWndVD::OnTbViewT3()
{
	DoTbViewT3();
	SetDocModified();
}

void BOpenGLWndVD::OnTbViewT4()
{
	DoTbViewT4();
	SetDocModified();
}

void BOpenGLWndVD::OnTbViewT5()
{
	DoTbViewT5();
	SetDocModified();
}

void BOpenGLWndVD::OnTbViewT6()
{
	DoTbViewT6();
	SetDocModified();
}

void BOpenGLWndVD::OnTbViewT7()
{
	DoTbViewT7();
	SetDocModified();
}

void BOpenGLWndVD::OnTbViewZx()
{
	DoTbViewZx();
	SetDocModified();
}

void BOpenGLWndVD::OnTbVewXz()
{
	DoTbVewXz();
	SetDocModified();
}

void BOpenGLWndVD::OnTbViewLa()
{
	DoTbViewLa();
	SetDocModified();
}

void BOpenGLWndVD::OnTbVewZy()
{
	DoTbVewZy();
	SetDocModified();
}

void BOpenGLWndVD::OnTbViewXyleft()
{
	DoTbViewXyleft();
	SetDocModified();
}

void BOpenGLWndVD::OnTbViewXyright()
{
	DoTbViewXyright();
	SetDocModified();
}

void BOpenGLWndVD::OnSaveView1()
{
	OnTbSave();
	OnTbView1();
}

void BOpenGLWndVD::OnSaveView2()
{
	OnTbSave();
	OnTbView2();
}

void BOpenGLWndVD::OnSaveView3()
{
	OnTbSave();
	OnTbView3();
}

void BOpenGLWndVD::OnSaveView4()
{
	OnTbSave();
	OnTbView4();
}

void BOpenGLWndVD::OnSaveView5()
{
	OnTbSave();
	OnTbView5();
}

void BOpenGLWndVD::OnTbSave()
{
	ZoomSaveActive = TRUE;
}

void BOpenGLWndVD::OnTbView1()
{
	SetSavedView(1);
}

void BOpenGLWndVD::OnTbView2()
{
	SetSavedView(2);
}

void BOpenGLWndVD::OnTbView3()
{
	SetSavedView(3);
}

void BOpenGLWndVD::OnTbView4()
{
	SetSavedView(4);
}

void BOpenGLWndVD::OnTbView5()
{
	SetSavedView(5);
}


void BOpenGLWndVD::OnTbZoomAll()
{
	DoTbZoomAll();
	SetDocModified();
}

void BOpenGLWndVD::OnUpdateTbViewPrev(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(ViewHistory.CanStepBack());
}

void BOpenGLWndVD::OnUpdateTbViewNext(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(ViewHistory.CanStepFor());
}

void BOpenGLWndVD::OnTbViewPrev()
{
	DoTbViewPrev();
	SetDocModified();
}

void BOpenGLWndVD::OnTbViewNext()
{
	DoTbViewNext();
	SetDocModified();
}

void BOpenGLWndVD::OnUpdateTbSave(CCmdUI* pCmdUI)
{
	if (ZoomSaveActive)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}

void BOpenGLWndVD::SetSavedView(int i)
{
	MakeCurrent();
	if (ZoomSaveActive)
	{
		SaveView(i);
		ZoomSaveActive = FALSE;
	}
	else
	{
		RestoreView(i);
		m_ChangeView = 2;
		Invalidate();
	}
	ClearZoomActive();
	RestoreContext();
	SetDocModified();
}

bool BOpenGLWndVD::SetCursorByCursorID()
{
	if (CursorID != 0)
	{
		HCURSOR c = AfxGetApp()->LoadCursor(CursorID);
		if (c)
			::SetCursor(c);
		return true;
	}
	return false;
}

void BOpenGLWndVD::PrepTracker(NRotTracker*& Tracker, char axes, WORD StopMessage)
{
	BOpenGLWnd::PrepTracker(Tracker, axes, StopMessage);
	SetCursorByCursorID();
}

BOOL BOpenGLWndVD::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (ZoomWActive || ZoomRActive || ZoomNormActive)
	{
		if (SetCursorByCursorID())
			return TRUE;
	}
	return BOpenGLWnd::OnSetCursor(pWnd, nHitTest, message);
}

BOOL BOpenGLWndVD::OnCmdMsg(UINT nID, int nCode, void* pExtra,
	AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (GetSafeHwnd() != nullptr)
	{
		CWnd* pOwner = GetParent();
		if (pOwner != NULL)
		{
			//TRACE(traceCmdRouting, 1, "Routing command id 0x%04X to owner window.\n", nID);

			ASSERT(pOwner != this);
			if (pOwner->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
				return TRUE;
		}
	}
	return BaseOGLWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

