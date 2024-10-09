// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include <chrono>
#include <thread>
#include "ResInv.h"
#include "ResLocal.h"
#include "ConstDef.h"
#include "NCEnums.h"
#include "NCMVersion.h"
#include "NCMProject.h"
#include "NStockSection.h"
#include "ParticularExport.h"
#include "BSpacePos.h"
#include "NCM.h"
#include "NCMDoc.h"
#include "NProgram.h"
#include "NCUnit.h"
#include "NRepDlgRepEditor.h"
#include "NRepDlgActiveReports.h"
#include "NRepDlgReportView.h"
#include "GAnimateCadrMill.h"
#include "GAnimateCadrRot.h"
#include "FAnimateDisp.h"
#include "FCutting.h"
#include "GPauseAnimate.h"
#include "NSetToolBar.h"
#include "NDummyView.h"
#include "NSetupSetBoxPointDlg.h"
#include "NSetupSetUCSOnBox.h"

#include "LeftView.h"
#include "NTextView.h"
#include "SecurityDlg.h"
#include "Security.h"

#include "FProg2BSP.h"
#include "NAnimState.h"
#include "T3DData.h"
#include "NCMVisualManager.h"
#include "NStockTurnSecDlg.h"
#include "FRange.h"
#include "DHtmlExploreDlg.h"
#include "NRemoteManager.h"
#include "GQuadRenderRot.h"
#include "GQuadRenderMill.h"
#include "NImportDlg.h"
#include "DXDirection5X.h"
#include "GQuadRenderDX5X.h"
#include "BTimeLimit.h"
#include "NSetupWriteCSDlg.h"
#include "NRotCenterDlg.h"
//#include "NRotAngles.h"
#include "DDimEntCrSheet.h"
#include "DPrimPoint.h"
#include "DPrimSegment.h"
#include "DPrimArc.h"
#include "DPrimPlane.h"
#include "DPrimAutofix.h"
#include "DPrimDimens.h"
#include "CrStockTemp.h"
#include "NMFCMenuWin.h"
#include "NToolsConCrPage.h"
#include "NToolsConCrTurnPage.h"
#include "NToolsConCrMillPage.h"
#include "NToolsConCrWeldPage.h"
#include "NToolsConCrSheet.h"
#include "NToolMill.h"
#include "QDivideDlg.h"
#include "QDXDivide.h"
#include "NPositionDlg.h"
#include "NCollidableObject.h"

#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CNCMApp theApp;

const int COLLISION_PANE_WIDTH = 20;
const int COLLISION_ANIM_NUM = 2;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_COMMAND(ID_SPEED_SLD, OnRibbonSlider)
	ON_COMMAND(ID_STEP_FOR, OnStepFor)
	ON_COMMAND(ID_STEP_BACK, OnJumpBack)
	ON_COMMAND(ID_OGL_CONFIG, OnOglConfig)
	ON_COMMAND(ID_PARTICULAR, OnParticular)
	ON_COMMAND(ID_STEP_HERE, OnRunHere)
	ON_COMMAND(ID_STEP_STOP, OnStepStop)
//	ON_UPDATE_COMMAND_UI(ID_SAVE_ENVIRONMENT_STUD, OnUpdateSaveEnvironmentStud)
	ON_UPDATE_COMMAND_UI(ID_FAST_CUT_MODE5X, OnUpdateFastCut5x)
	ON_UPDATE_COMMAND_UI(ID_CUT_MODE, OnUpdateCutMode)
	ON_COMMAND(ID_REPORTS_EDIT, OnReportsEdit)
	ON_UPDATE_COMMAND_UI(IDS_INDIC_XYZ, OnUpdateCoord)
	ON_UPDATE_COMMAND_UI(IDS_INDIC_COLL, OnUpdateCollIcon)
	ON_UPDATE_COMMAND_UI(IDS_INDIC_CADR, OnUpdateCadr)
	ON_COMMAND(IDS_INDIC_XYZ, OnStatusCoord)
	ON_COMMAND(IDS_INDIC_CADR, OnStatusCadr)
	ON_COMMAND(IDC_STEP_NUM, OnStepNum)
	ON_COMMAND(ID_PAUSE_NUM, OnPauseNum)
	ON_COMMAND(ID_HELP_FINDER, CFrameWndEx::OnHelpFinder)
	ON_COMMAND(ID_HELP, OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CFrameWndEx::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CFrameWndEx::OnHelpFinder)
	ON_COMMAND(ID_MENU_REPORTS_SELECT, OnMenuReportsSelect)
	ON_UPDATE_COMMAND_UI(ID_MENU_REPORTS_SELECT, OnUpdateMenuReportsSelect)
	ON_COMMAND(ID_MENU_REPORTS_CLEAR, OnMenuReportsClear)
	ON_COMMAND(ID_MENU_REPORTS_VIEW, OnMenuReportsView)
	ON_UPDATE_COMMAND_UI(ID_MENU_REPORTS_VIEW, OnUpdateMenuReportsView)
	ON_COMMAND(ID_WIN_MACH, OnWinMach)
	ON_COMMAND(ID_WIN_STOCK, OnWinStock)
	ON_UPDATE_COMMAND_UI(ID_WIN_STOCK, OnUpdateWinStock)
	ON_UPDATE_COMMAND_UI(ID_STEP_RUN, OnUpdateStepRun)
	ON_UPDATE_COMMAND_UI(ID_STEP_HERE, OnUpdateStepHere)
	ON_UPDATE_COMMAND_UI(ID_STEP_FOR, OnUpdateStepFor)
	ON_COMMAND(ID_ANIMATE_PAUSE, OnAnimatePause)
	ON_UPDATE_COMMAND_UI(ID_ANIMATE_PAUSE, OnUpdateAnimatePause)
	ON_COMMAND(ID_RUN_TO_BREAKPOINT, OnRunToBreakpoint)
	ON_COMMAND(ID_RESET, OnReset)
	ON_UPDATE_COMMAND_UI(ID_RUN_TO_BREAKPOINT, OnUpdateRunToBreakpoint)
	ON_COMMAND(ID_AUTO_UPDATE_PROG, OnAutoUpdateProg)
	ON_UPDATE_COMMAND_UI(ID_AUTO_UPDATE_PROG, OnUpdateAutoUpdateProg)
	ON_COMMAND(ID_STOCK_INIT, OnStockInit)
	ON_UPDATE_COMMAND_UI(ID_STEP_BACK, OnUpdateStepBack)
	ON_UPDATE_COMMAND_UI(ID_STOCK_INIT, OnUpdateStockInit)
	ON_UPDATE_COMMAND_UI(ID_RESET, OnUpdateReset)
	ON_COMMAND(ID_BREAKPOINT_FORWARD, OnBreakpointForward)
	ON_UPDATE_COMMAND_UI(ID_BREAKPOINT_FORWARD, OnUpdateBreakpointForward)
	ON_COMMAND(ID_BREAKPOINT_TABLE, OnBreakpointTable)
	ON_UPDATE_COMMAND_UI(ID_BREAKPOINT_TABLE, OnUpdateBreakpointTable)
	ON_COMMAND(ID_BREAKPOINT_BACK, OnBreakpointBack)
	ON_UPDATE_COMMAND_UI(ID_BREAKPOINT_BACK, OnUpdateBreakpointBack)
	ON_COMMAND(ID_M_BREAKPOINTADD_ON_OFF, OnMBreakpointaddOnOff)
	ON_UPDATE_COMMAND_UI(ID_M_BREAKPOINTADD_ON_OFF, OnUpdateMBreakpointaddOnOff)
	ON_COMMAND(ID_BP1, OnBp1)
	ON_UPDATE_COMMAND_UI(ID_BP1, OnUpdateBp1)
	ON_COMMAND(ID_BP2, OnBp2)
	ON_UPDATE_COMMAND_UI(ID_BP2, OnUpdateBp2)
	ON_COMMAND(ID_BP3, OnBp3)
	ON_UPDATE_COMMAND_UI(ID_BP3, OnUpdateBp3)
	ON_COMMAND(ID_BP4, OnBp4)
	ON_UPDATE_COMMAND_UI(ID_BP4, OnUpdateBp4)
	ON_COMMAND(ID_BP5, OnBp5)
	ON_UPDATE_COMMAND_UI(ID_BP5, OnUpdateBp5)
	ON_COMMAND(ID_BP6, OnBp6)
	ON_UPDATE_COMMAND_UI(ID_BP6, OnUpdateBp6)
	ON_COMMAND(ID_START_COLLCHECK, OnStartCollcheck)
	ON_UPDATE_COMMAND_UI(ID_START_COLLCHECK, OnUpdateStartCollcheck)
	ON_COMMAND(ID_MEASURE_DISTANCE, OnMeasureDistance)
	ON_UPDATE_COMMAND_UI(ID_MEASURE_DISTANCE, OnUpdateMeasureDistance)
	ON_COMMAND(ID_STOPONCOLLISION, OnStopOnColl)
	ON_UPDATE_COMMAND_UI(ID_STOPONCOLLISION, OnUpdateStopOnColl)
	ON_MESSAGE(WM_APP_GRID_EVALEXP, OnGridEvalexp)
	ON_MESSAGE(WM_APP_WIDE_EVALEXP, OnWideEvalexp)
	ON_MESSAGE(WM_APP_CUTTING_STOPPED, OnCuttingStopped)
	ON_MESSAGE(WM_APP_CUT_STATE_CHANGED, OnCutStateChanged)
	ON_MESSAGE(WM_APP_COLLCHECK_STOPPED, OnCollcheckStopped)
    ON_MESSAGE(WM_APP_COLLCHECK_ADDLOG, OnCollcheckAddlog)
    ON_MESSAGE(WM_APP_UNIT_INFO, OnUnitInfo)
	ON_MESSAGE(WM_APP_UNIQUE, OnUnique)
	ON_MESSAGE(WM_APP_MAKE_TOOL, OnMakeTool)
	ON_MESSAGE(WM_APP_STOP_TOOL, OnStopTool)
	ON_MESSAGE(WM_APP_UPDATE_PANES, OnUpdatePanes)
	ON_MESSAGE(WM_APP_STOP_IMP_MODE, OnStopImpMode)
	ON_MESSAGE(WM_APP_STOCK_VISIBLE, OnStockVisibleChange)
	ON_MESSAGE(WM_APP_REGENERATE, OnRegenerate)
    ON_MESSAGE(WM_APP_3DXINPUT, On3DxInput)
	ON_COMMAND(ID_VIEW_DEFAULT, OnViewDefault)
	ON_UPDATE_COMMAND_UI(ID_WIN_MACH, OnUpdateWinMach)
	ON_COMMAND(ID_CHECK_MI, OnCheckMi)
	ON_UPDATE_COMMAND_UI(ID_CHECK_MI, OnUpdateCheckMi)
	ON_WM_QUERYOPEN()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_ACTIVATE()
	ON_COMMAND(ID_TEST, OnTest)
	ON_WM_NCPAINT()
	ON_COMMAND(ID_STOCK_STATE, OnStockState)
	ON_UPDATE_COMMAND_UI(ID_STOCK_STATE, OnUpdateStockState)
	ON_UPDATE_COMMAND_UI(ID_STATETB0, OnUpdateStateTB0)
	ON_UPDATE_COMMAND_UI(ID_STATETB1, OnUpdateStateTB1)
	ON_UPDATE_COMMAND_UI(ID_STATETB2, OnUpdateStateTB2)
	ON_UPDATE_COMMAND_UI(ID_STATETB3, OnUpdateStateTB3)
	ON_UPDATE_COMMAND_UI(ID_STATETB4, OnUpdateStateTB4)
	ON_UPDATE_COMMAND_UI(ID_STATETB5, OnUpdateStateTB5)
	ON_UPDATE_COMMAND_UI(ID_STATETB6, OnUpdateStateTB6)
	ON_UPDATE_COMMAND_UI(ID_STATETB7, OnUpdateStateTB7)
	ON_WM_COPYDATA()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_COMMAND(ID_PAUSE_BACK, OnPauseBack)
	ON_COMMAND(ID_PAUSE_FORWARD, OnPauseForward)
	ON_COMMAND(ID_MEASURE, OnMeasure)
	ON_UPDATE_COMMAND_UI(ID_MEASURE, OnUpdateMeasure)
	ON_COMMAND(ID_PA_STOCK_SEC_VIEW, OnPaStockSecView)
	ON_COMMAND(ID_INSPECT_TOOLPATHSTATISTICS, &CMainFrame::OnInspectToolpathstatistics)
	ON_COMMAND(ID_COLLSETTINGS, &CMainFrame::OnCollsettings)
	ON_COMMAND(ID_VIEW_SECTION, OnViewSection)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SECTION, &CMainFrame::OnUpdateViewSection)
	ON_COMMAND(ID_TB_ROTATECENTERONOFF, OnRotatecenteronoff)
	ON_UPDATE_COMMAND_UI(ID_TB_ROTATECENTERONOFF, OnUpdateRotatecenteronoff)
	ON_COMMAND(ID_TB_UNDO, OnTbUndo)
	ON_UPDATE_COMMAND_UI(ID_TB_UNDO, OnUpdateTbUndo)
	ON_COMMAND(ID_TB_REDO, OnTbRedo)
	ON_UPDATE_COMMAND_UI(ID_TB_REDO, OnUpdateTbRedo)
	ON_UPDATE_COMMAND_UI(ID_COLLSETTINGS, &CMainFrame::OnUpdateCollsettings)
	ON_UPDATE_COMMAND_UI(ID_VIEW_REGEN, &CMainFrame::OnUpdateViewRegen)
	ON_COMMAND(ID_VIEW_REGEN, &CMainFrame::OnViewRegen)
	ON_COMMAND(ID_REM_CONT, &CMainFrame::OnRemCont)
	ON_UPDATE_COMMAND_UI(ID_REM_CONT, &CMainFrame::OnUpdateRemCont)
	ON_COMMAND(ID_SETUP_WRITE_0_CS, &CMainFrame::OnSetupWrite0Cs)
	ON_UPDATE_COMMAND_UI(ID_SETUP_WRITE_0_CS, &CMainFrame::OnUpdateSetupWrite0Cs)
	ON_WM_HELPINFO()
	ON_COMMAND(ID_PA_MACH_EDIT_END, &CMainFrame::OnPaMachEditEnd)
	ON_COMMAND(ID_COMPARISON, OnComparison)
	ON_COMMAND(ID_CREATE_ENT, OnCreateEnt)
	ON_UPDATE_COMMAND_UI(ID_CREATE_ENT, OnUpdateCreateEnt)
	ON_UPDATE_COMMAND_UI(ID_COMPARISON, OnUpdateComparison)
    ON_MESSAGE(WM_APP_PSLIDE, OnPSlide)
    ON_MESSAGE(WM_APP_GET_ABS_OBJ, OnGetAbsObj)
	ON_COMMAND(ID_STEP_OVER, OnJumpOver)
	ON_MESSAGE(WM_APP_MACRO_APPLY, &CMainFrame::OnMacroApply)
	ON_COMMAND(ID_SETUP_AUTOPOS, &CMainFrame::OnSetupAutops)
	ON_COMMAND(ID_SETUP_SETBOXPOINT, &CMainFrame::OnSetupSetboxpoint)
	ON_UPDATE_COMMAND_UI(ID_SETUP_SETBOXPOINT, &CMainFrame::OnUpdateSetupSetboxpoint)
	ON_COMMAND(ID_SETUP_SETUCSONBOX, &CMainFrame::OnSetupSetUCSOnbox)
	ON_UPDATE_COMMAND_UI(ID_SETUP_SETUCSONBOX, &CMainFrame::OnUpdateSetupSetUCSOnbox)
	ON_COMMAND(ID_ROTCENTER, &CMainFrame::OnRotCenter)
	ON_UPDATE_COMMAND_UI(ID_ROTCENTER, &CMainFrame::OnUpdateRotCenter)
//	ON_COMMAND(ID_DEMO_COMMENT, &CMainFrame::OnDemoComment)
	ON_COMMAND(ID_RIBBONINTERFACE, &CMainFrame::OnStyleRibboninterface)
	ON_COMMAND(ID_MENUINTERFACE, &CMainFrame::OnRbnBtnToolbarInterface)
	ON_COMMAND(ID_NCUNIT_COMBO, &CMainFrame::OnNCUnitCombo)
	ON_COMMAND(ID_MACH_COMBO, &CMainFrame::OnMachCombo)
	ON_COMMAND(ID_TOOLS_COMBO, &CMainFrame::OnToolsCombo)
	ON_COMMAND(ID_STOCK_CR, &CMainFrame::OnStockCr)
	ON_COMMAND(ID_NEW_PROJECT, &CMainFrame::OnNewProject)
	ON_COMMAND(ID_STOCK_ED, &CMainFrame::OnStockEd)
	ON_UPDATE_COMMAND_UI(ID_STOCK_ED, &CMainFrame::OnUpdateStockEd)
	ON_COMMAND(ID_O_WINDOWS, &CMainFrame::OnOWindows)
	ON_COMMAND(ID_STOCKPANE_RIB, &CMainFrame::OnStockPaneRib)
	ON_COMMAND(ID_MACHPANE_RIB, &CMainFrame::OnMachPaneRib)
	ON_UPDATE_COMMAND_UI(ID_STOCKPANE_RIB, &CMainFrame::OnUpdateStockPaneRib)
	ON_UPDATE_COMMAND_UI(ID_MACHPANE_RIB, &CMainFrame::OnUpdateMachPaneRib)
	ON_COMMAND(ID_TEXTPANE_RIB, &CMainFrame::OnTextPaneRib)
	ON_UPDATE_COMMAND_UI(ID_TEXTPANE_RIB, &CMainFrame::OnUpdateTextPaneRib)
	ON_COMMAND(ID_LEFTPANE_RIB, &CMainFrame::OnLeftPaneRib)
	ON_UPDATE_COMMAND_UI(ID_LEFTPANE_RIB, &CMainFrame::OnUpdateLeftPaneRib)
	ON_COMMAND(ID_UNITPANE_RIB, &CMainFrame::OnUnitPaneRib)
	ON_UPDATE_COMMAND_UI(ID_UNITPANE_RIB, &CMainFrame::OnUpdateUnitPaneRib)
	ON_COMMAND(ID_ANALYSEPANE_RIB, &CMainFrame::OnAnalysePaneRib)
	ON_UPDATE_COMMAND_UI(ID_ANALYSEPANE_RIB, &CMainFrame::OnUpdateAnalysePaneRib)
	ON_COMMAND(ID_GEOMPANE_RIB, &CMainFrame::OnGeomPaneRib)
	ON_UPDATE_COMMAND_UI(ID_GEOMPANE_RIB, &CMainFrame::OnUpdateGeomPaneRib)
	ON_COMMAND(ID_MESUREPANE_RIB, &CMainFrame::OnMesurePaneRib)
	ON_UPDATE_COMMAND_UI(ID_MESUREPANE_RIB, &CMainFrame::OnUpdateMesurePaneRib)
	ON_COMMAND(ID_REPORTPANE_RIB, &CMainFrame::OnReportPaneRib)
	ON_UPDATE_COMMAND_UI(ID_REPORTPANE_RIB, &CMainFrame::OnUpdateReportPaneRib)
	ON_COMMAND(ID_REGSPANE_RIB, &CMainFrame::OnRegsPaneRib)
	ON_UPDATE_COMMAND_UI(ID_REGSPANE_RIB, &CMainFrame::OnUpdateRegsPaneRib)
	ON_COMMAND(ID_PSLIDEPANE_RIB, &CMainFrame::OnSlidePaneRib)
	ON_UPDATE_COMMAND_UI(ID_PSLIDEPANE_RIB, &CMainFrame::OnUpdateSlidePaneRib)
	ON_COMMAND(ID_PROPERTIES_RIB, &CMainFrame::OnPropertiesPaneRib)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES_RIB, &CMainFrame::OnUpdatePropertiesPaneRib)
	ON_COMMAND(ID_ADDPANE_DELALL, &CMainFrame::OnAddpaneDelall)
	ON_COMMAND(ID_SET_NEXT_CHANNEL, &CMainFrame::OnSetNextChannel)
	ON_COMMAND(ID_NEW_CHANNEL, &CMainFrame::OnNewChannel)
	ON_COMMAND(ID_DELETE_CHANNEL, &CMainFrame::OnDeleteChannel)
	ON_UPDATE_COMMAND_UI(ID_DELETE_CHANNEL, &CMainFrame::OnUpdateDeleteChannel)
	ON_UPDATE_COMMAND_UI(ID_SET_NEXT_CHANNEL, &CMainFrame::OnUpdateDeleteChannel)
	ON_COMMAND(ID_FILE_SAVE_AS_NCMPRT, &CMainFrame::OnFileSaveCopyAs)
	ON_COMMAND(ID_SAVE_ENVIRONMENT, &CMainFrame::OnFileSaveAll)
#define SMPane(PaneNum)\
		ON_COMMAND(IDS_STOCK_MACH_PANE_##PaneNum, &CMainFrame::OnStockMachPane##PaneNum)\
		ON_UPDATE_COMMAND_UI(IDS_STOCK_MACH_PANE_##PaneNum, &CMainFrame::OnUpdateStockMachPane##PaneNum)

	SMPane(2)
	SMPane(3)
	SMPane(4)
	SMPane(5)
	SMPane(6)
	SMPane(7)
	SMPane(8)
	SMPane(9)
	SMPane(10)
	SMPane(11)
	SMPane(12)
	SMPane(13)
	SMPane(14)
	SMPane(15)
	SMPane(16)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_WM_UPDATEUISTATE()
	ON_WM_SETFOCUS()

	END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	IDS_INDIC_COLL,
	IDS_INDIC_CADR,
	IDS_INDIC_REL_XYZ,
	IDS_INDIC_XYZ,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction
CMainFrame::CMainFrame() :
	m_wndLeftView(*new CLeftView),
//	m_wndWSectPane(&WSectWnd, 1),
	CreateSheetRect(0, 0, 0, 0)
{
	AnimateTimerID = 0;
	PauseAnimateTimerID = 0;
	ResultTimerID = 0;
	RunAnimateTimerID = 0;
	AnimatePause = false;
	PauseAnimBack = false;
	PauseAnimFor = false;
	PauseAnimStep = 1;
	PauseAnimatePause = false;
	pPauseAnimate = nullptr;

	AnimateText = true;
	ReRenderText = true;
	MaxRunStepsNum = -1;
	flags = 0;

	pMainPane = nullptr;
	pRemoteControl = nullptr;
	pImportDlg = nullptr;
	ImportMode = false;
	pSetBoxPointDlg = nullptr;
	pSetUCSOnBoxDlg = nullptr;
	pRotCenterDlg = nullptr;
	pWriteCSDlg = nullptr;

	pDimEntCrSheet = nullptr;
    pDimStockCrDlg = nullptr;

	pCreateSheet = nullptr;
	pCreateMillPage = nullptr;
	pCreateTurnPage = nullptr;
	pCreateWeldPage = nullptr;
	pDivideDlg = nullptr;
	AnimateTimeInterval.first = MainT_UNDEF;
	AnimateTimeInterval.second = MainT_UNDEF;
}

CMainFrame::~CMainFrame()
{
	delete &m_wndLeftView;
	delete pSetBoxPointDlg;
	delete pSetUCSOnBoxDlg;
	delete pRotCenterDlg;
	delete pWriteCSDlg;

	delete pCreateSheet;
	delete pCreateMillPage;
	delete pCreateTurnPage;
	delete pCreateWeldPage;

	delete pMainPane;
	for each (auto pPane in AddOGLPanes)
		delete pPane;
//	delete pDivideDlg; delete this is in QDivideDlg::PostNcDestroy()
}

void CMainFrame::SetDefaultManager()
{
	const int AppLook = theApp.GetInt(_T("AppLook"), 6);
	const int Style = theApp.GetInt(_T("AppStyle"), 0);
	const BOOL RoundedTabs = theApp.GetInt(_T("RoundedTabs"), FALSE);

	CTabbedPane::m_StyleTabWnd = CMFCTabCtrl::STYLE_3D;

	switch (AppLook)
	{
	case 0:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		break;

	case 1:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case 2:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		break;

	case 3:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));

		if (RoundedTabs)
		{
			CTabbedPane::m_StyleTabWnd = CMFCTabCtrl::STYLE_3D_ROUNDED;
		}

		CMFCVisualManagerVS2005::m_bRoundedAutohideButtons = RoundedTabs;
		break;

	case 4:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		break;

	case 5:
		switch (Style)
		{
		case 0:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case 1:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case 2:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;

		case 3:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		break;
	case 6:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(NCMVisualManager));
		break;
	case 7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(NCMVisualManager17));
		break;
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	CString strToolBarName;
	// set the visual manager used to draw all user interface elements

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}
	m_wndMenuBar.CreateFromMenu(::LoadMenu(GetModuleHandle(_T("Resources.dll")), MAKEINTRESOURCE(IDR_MAINFRAME)), TRUE, TRUE);
	auto Style = m_wndMenuBar.GetPaneStyle();
	Style &= ~TBSTYLE_WRAPABLE;
	m_wndMenuBar.SetPaneStyle(Style | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);
	// prevent the menu bar from taking the focus on activation
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (NTextView::IsRibbonInterface())
	{
		if (!m_wndRibbonBar.Create(this))
		{
			TRACE0("Failed to create ribbonbar\n");
			return -1;      // fail to create
		}
		m_wndMenuBar.ShowPane(FALSE, FALSE, FALSE);
	}

	if (!IsRibbonCreated())
	{
		if (
			!CreateToolBar(m_wndToolBar, ID_VIEW_TOOLBAR) ||
			!CreateToolBar(m_modeToolBar, ID_VIEW_MODES) ||
			!CreateToolBar(m_navigToolBar, ID_VIEW_NAVIGATION) ||
			!CreateToolBar(m_collisionsToolBar, ID_VIEW_COLLISIONS) ||
			!CreateToolBar(m_NBreakpointToolBar, ID_VIEW_BREAKPOINT) ||
			!CreateToolBar(m_inspectToolBar, ID_VIEW_TOOLS) ||
			!CreateToolBar(m_windowToolBar, ID_VIEW_WIN) ||
			!CreateToolBar(m_wndDlgBar.DlgBar, ID_VIEW_NUMBER)
			)
		{
			TRACE0("Failed to create dialogbar\n");
			return -1;		// fail to create
		}
	}
	if (
		!CreateToolBar(m_indicatorsToolBar, ID_VIEW_STATE) ||
		!CreateToolBar(m_viewToolBar, ID_VIEW_VIEW) 
		)
	{
		TRACE0("Failed to create dialogbar\n");
		return -1;		// fail to create
	}
	//	CMFCToolBar::m_bDontScaleImages = TRUE;
	AFX_GLOBAL_DATA* pGlobData = GetGlobalData();
	pGlobData->EnableRibbonImageScale(FALSE);
	CMFCToolBar::m_dblLargeImageRatio = 1.5;
	CMFCToolBar::SetMenuSizes(CSize(22, 22), CSize(16, 16));
	CMFCToolBar::SetSizes(CSize(18, 18), CSize(16, 16));
	CMFCToolBar::SetLargeIcons(TRUE);
	CMFCToolBar::m_bExtCharTranslation = TRUE;// For Alt-key access to menu


	// Allow user-defined toolbars operations:
//	InitUserToolbars(nullptr, uiFirstUserToolBarId, uiLastUserToolBarId);

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Enable toolbar and docking window menu replacement
	CString strCustomize;
	BOOL bNameValid = strCustomize.LoadString(IDS_CUSTOMIZE);
	ASSERT(bNameValid);
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_TOOLBARS_MENU);

	// enable quick (Alt+drag) toolbar customization
	CMFCToolBar::EnableQuickCustomization();

	//if (CMFCToolBar::GetUserImages() == nullptr)
	//{
	//	// load user-defined toolbar images
	//	if (m_UserImages.Load(_T(".\\UserImages.bmp")))
	//	{
	//		m_UserImages.SetImageSize(CSize(16, 16), FALSE);
	//		CMFCToolBar::SetUserImages(&m_UserImages);
	//	}
	//}

	// enable menu personalization (most-recently used commands)
	// TODO: define your own basic commands, ensuring that each pulldown menu has at least one basic command.
	//CList<UINT, UINT> lstBasicCommands;

	//lstBasicCommands.AddTail(ID_FILE_NEW);
	//lstBasicCommands.AddTail(ID_FILE_OPEN);
	//lstBasicCommands.AddTail(ID_FILE_SAVE);
	//lstBasicCommands.AddTail(ID_FILE_NEW_MILL);
	//lstBasicCommands.AddTail(ID_EDIT_UNDO);
	//lstBasicCommands.AddTail(ID_TB_RESET);
	//lstBasicCommands.AddTail(ID_RESET);
	//lstBasicCommands.AddTail(ID_MEASURE);
	//lstBasicCommands.AddTail(ID_MENU_REPORTS_SELECT);
	//lstBasicCommands.AddTail(ID_SETTINGS);
	//lstBasicCommands.AddTail(ID_WIN_STOCK);
	//lstBasicCommands.AddTail(ID_HELP_FINDER);

	//CMFCToolBar::SetBasicCommands(lstBasicCommands);
	

	// Set defaults
	NSpeedPar::Extern.SetDefaults(NCM_PROJECT.Defaults);
	GQuadRenderRot::SetDefaults(NCM_PROJECT.Defaults);
	GQuadRenderMill::SetDefaults(NCM_PROJECT.Defaults);
	NObject::SetSelectAttrs(BSelectAttrs(NCM_PROJECT.Defaults));
	NCollidableObject::SetDefaults(NCM_PROJECT.Defaults);
	// Create panes

	CString ViewTitle;
	bNameValid = ViewTitle.LoadString(ID_TEXTPANE);
	ASSERT(bNameValid);
	if (!m_wndTextView.Create(ViewTitle, this, CRect(0, 0, 350, 400), TRUE, ID_TEXTPANE, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Text Pane window\n");
		return FALSE; // failed to create
	}

	bNameValid = ViewTitle.LoadString(ID_LEFTPANE);
	ASSERT(bNameValid);
	if (!m_wndLeftView.Create(ViewTitle, this, CRect(0, 0, 200, 200), TRUE, ID_LEFTPANE, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Left Pane window\n");
		return FALSE; // failed to create
	}

	bNameValid = ViewTitle.LoadString(ID_UNITPANE);
	ASSERT(bNameValid);
	if (!m_wndUnitPane.Create(ViewTitle, this, CRect(0, 0, 200, 200), TRUE, ID_UNITPANE, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Unit Pane window\n");
		return FALSE; // failed to create
	}

	bNameValid = ViewTitle.LoadString(ID_ANALYSEPANE);
	ASSERT(bNameValid);
	if (!m_wndAnalysePane.Create(ViewTitle, this, CRect(0, 0, 200, 200), TRUE, ID_ANALYSEPANE, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Analyse Pane window\n");
		return FALSE; // failed to create
	}

	bNameValid = ViewTitle.LoadString(ID_MTEDITPANE);
	ASSERT(bNameValid);
	if (!m_wndMTEditPane.Create(ViewTitle, this, CRect(0, 0, 200, 200), TRUE, ID_MTEDITPANE, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create MTEdit Pane window\n");
		return FALSE; // failed to create
	}

	bNameValid = ViewTitle.LoadString(ID_MTEDITTREE);
	ASSERT(bNameValid);
	if (!m_wndMTEditTreePane.Create(ViewTitle, this, CRect(0, 0, 200, 200), TRUE, ID_MTEDITTREE, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create MTEditTree Pane window\n");
		return FALSE; // failed to create
	}

	bNameValid = ViewTitle.LoadString(ID_GEOMPANE);
	ASSERT(bNameValid);
	if (!m_wndGeomPane.Create(ViewTitle, this, CRect(0, 0, 200, 200), TRUE, ID_GEOMPANE, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Geom Pane window\n");
		return FALSE; // failed to create
	}

	bNameValid = ViewTitle.LoadString(ID_MESUREPANE);
	ASSERT(bNameValid);
	if (!m_wndMesurePane.Create(ViewTitle, this, CRect(0, 0, 200, 200), TRUE, ID_MESUREPANE, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Measure Pane window\n");
		return FALSE; // failed to create
	}

	bNameValid = ViewTitle.LoadString(ID_REGSPANE);
	ASSERT(bNameValid);
	if (!m_wndRegsPane.Create(ViewTitle, this, CRect(0, 0, 200, 200), TRUE, ID_REGSPANE, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Regs Pane window\n");
		return FALSE; // failed to create
	}

	bNameValid = ViewTitle.LoadString(ID_REPORTPANE);
	ASSERT(bNameValid);
	if (!m_wndReportPane.Create(ViewTitle, this, CRect(0, 0, 200, 200), TRUE, ID_REPORTPANE, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |  WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Report Pane window\n");
		return FALSE; // failed to create
	}

	pMainPane = NOGLPane::MakeOGLPane(true, 0, this, false, false);

	//bNameValid = ViewTitle.LoadString(ID_WSECTPANE);
	//ASSERT(bNameValid);
	//if (!m_wndWSectPane.Create(ViewTitle, this, CRect(0, 0, 0, 0), TRUE, ID_WSECTPANE, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("Failed to create OpenGL Pane window\n");
	//	return FALSE; // failed to create
	//}

	bNameValid = ViewTitle.LoadString(ID_PSLIDEPANE);
	ASSERT(bNameValid);
	if (!m_wndPSlidePane.Create(ViewTitle, this, CRect(0, 0, 200
		, SLIDER_HEIGHT + TOP_GAP + BOTTOM_GAP + GetSystemMetrics(SM_CYSMCAPTION) + GetSystemMetrics(SM_CYSIZEFRAME) * 2), FALSE
		, ID_PSLIDEPANE, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create PSlide Pane window\n");
		return FALSE; // failed to create
	}

	bNameValid = ViewTitle.LoadString(ID_PROPERTIES);//propEditor
	if (!m_wndPropPane.Create(ViewTitle, this, CRect(0, 0, 200, 200), TRUE, ID_PROPERTIES, WS_CHILD | WS_VISIBLE |WS_CLIPSIBLINGS|WS_CLIPCHILDREN|CBRS_RIGHT|CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Properties window\n");
		return FALSE; // failed to create
	}
	m_wndPropPane.Clear();
	m_wndLeftView.SetPropPane(&m_wndPropPane);

	if (!m_wndErrorsCaption.Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, this, IDS_ERRORS_CAPTION, -1, TRUE))
	{
		TRACE0("Failed to create caption bar\n");
		return FALSE;
	}
	CString CaptionText, CaptionButton, CaptionButtonToolTip;
	bNameValid = CaptionText.LoadString(IDS_ERRORS_CAPTION);
	m_wndErrorsCaption.m_clrBarText = RGB(192, 0, 0);
	//m_wndErrorsCaption.m_clrBarBackground = RGB(0, 0, 0);

	//m_wndErrorsCaption.SetFlatBorder(FALSE);
	//m_wndErrorsCaption.SetMargin(1);
	CaptionButton.LoadString(IDS_ERRORS_LIST);
	CaptionButtonToolTip.LoadString(IDS_ERRORS_LIST_TOOLTIP);
	m_wndErrorsCaption.SetButton(_T(CaptionButton), ID_ERRORS_CAPTION_BUTTON, CMFCCaptionBar::ALIGN_CENTER, FALSE);
	m_wndErrorsCaption.SetButtonToolTip(_T(CaptionButtonToolTip));
	m_wndErrorsCaption.SetText(CaptionText, CMFCCaptionBar::ALIGN_CENTER);

	//m_wndErrorsCaption.SetBitmap(IDB_AXIS, RGB(255, 255, 255), FALSE, CMFCCaptionBar::ALIGN_LEFT);
	//m_wndErrorsCaption.SetImageToolTip(_T("Important"), _T("Please take a look at MSOffice2007Demo source code to learn how to create advanced user interface in minutes."));



	CDockablePane::m_bHideInAutoHideMode = TRUE;// Added to ensure dinamic visualisation when in autohide mode (IsVisible() returns true)
	LoadTBFromIcons();
	PlaceToolBars();
	PlaceViewPanes();
	for (int i = 0; i < NOGLPane::GetFixedPanesNum(); ++i)
	{
		NOGLPane* pPane = NOGLPane::MakeOGLPane(false, i + 1, this);
//		pPane->ShowPane(FALSE, FALSE, FALSE);
		DockPane(pPane);
		AddOGLPanes.push_back(pPane);
	}
	for (int i = NOGLPane::GetFixedPanesNum(); i < NOGLPane::GetMaxWinNum(); ++i)
	{
		NOGLPane* pPane = NOGLPane::MakeOGLPane(false, i + 1, this, true, false);
		DockPane(pPane);
		pPane->FloatPane(GetNewPaneRect(), DM_UNKNOWN, false);
		AddOGLPanes.push_back(pPane);
	}
//	RecalcLayout();

	SetDockingWindowIcons();

//	SetDefaultManager();
	// got from here: https://helgeklein.com/blog/how-to-enable-drag-and-drop-for-an-elevated-mfc-application-on-vistawindows-7/
	ChangeWindowMessageFilter(WM_DROPFILES, MSGFLT_ADD);
	ChangeWindowMessageFilter(WM_COPYDATA, MSGFLT_ADD);
	ChangeWindowMessageFilter(0x0049, MSGFLT_ADD);
	return 0;
}
void CMainFrame::SetDockingWindowIcons()
{
	HMODULE hins = GetModuleHandle(_T("ResInv.dll") );
	if( hins == nullptr )
		return ;
	HICON hGeomViewIcon = (HICON) ::LoadImage(hins, MAKEINTRESOURCE(IDI_GEOM), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndGeomPane.SetIcon(hGeomViewIcon, FALSE);

	HICON hUnitViewIcon = (HICON) ::LoadImage(hins, MAKEINTRESOURCE(IDI_UNIT), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndUnitPane.SetIcon(hUnitViewIcon, FALSE);

	HICON hAnalyseViewIcon = (HICON) ::LoadImage(hins, MAKEINTRESOURCE(IDI_PROC), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndAnalysePane.SetIcon(hAnalyseViewIcon, FALSE);

	HICON hMTEditViewIcon = (HICON) ::LoadImage(hins, MAKEINTRESOURCE(IDI_MTPROP), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndMTEditPane.SetIcon(hMTEditViewIcon, FALSE);

	HICON hMTEditTreeViewIcon = (HICON) ::LoadImage(hins, MAKEINTRESOURCE(IDI_MTTREE), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndMTEditTreePane.SetIcon(hMTEditTreeViewIcon, FALSE);

	HICON hMesureViewIcon = (HICON) ::LoadImage(hins, MAKEINTRESOURCE(IDI_MEASURE), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndMesurePane.SetIcon(hMesureViewIcon, FALSE);

	HICON hRegsViewIcon = (HICON) ::LoadImage(hins, MAKEINTRESOURCE(IDI_REGS), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndRegsPane.SetIcon(hRegsViewIcon, FALSE);

	HICON hReportViewIcon = (HICON) ::LoadImage(hins, MAKEINTRESOURCE(IDI_REPORT), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndReportPane.SetIcon(hReportViewIcon, FALSE);

	HICON hTextViewIcon = (HICON) ::LoadImage(hins, MAKEINTRESOURCE(IDI_TB_NEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndTextView.SetIcon(hTextViewIcon, FALSE);

	HICON hLeftViewIcon = (HICON) ::LoadImage(hins, MAKEINTRESOURCE(IDI_LEFT), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndLeftView.SetIcon(hLeftViewIcon, FALSE);

}
bool CMainFrame::CreateToolBar(CMFCToolBar &ToolBar, int ToolBarID)
{
	if (!ToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE  | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(0,0,0,0), ToolBarID))
	{
		TRACE0("Failed to create toolbar\n");
		return false;      // fail to create
	}
	CString strToolBarName;
	bool bNameValid = (strToolBarName.LoadString(ToolBarID) != 0);
	int nNameLength = strToolBarName.GetLength() - strToolBarName.ReverseFind('\n') - 1;
	ASSERT(bNameValid);
	ToolBar.SetWindowText(strToolBarName.Right(nNameLength));
//	ToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, IDS_TOOLBAR_CUSTOMIZE);
	return true;
}
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs,
	CCreateContext* pContext)
{ 
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.EnablePaneDoubleClick(TRUE);

	SetCollisionIcon(false, true);
	CString ToolTip;
	ToolTip.LoadString(IDS_STATUSPANE0);
	m_wndStatusBar.SetTipText(0, ToolTip);
	ToolTip.LoadString(IDS_STATUSPANE1_OFF);
	m_wndStatusBar.SetTipText(1, ToolTip);
	ToolTip.LoadString(IDS_STATUSPANE2);
	m_wndStatusBar.SetTipText(2, ToolTip);
	ToolTip.LoadString(IDS_STATUSPANE3);
	m_wndStatusBar.SetTipText(3, ToolTip);
	ToolTip.LoadString(IDS_STATUSPANE4);
	m_wndStatusBar.SetTipText(4, ToolTip);

	m_wndStatusBar.SetPaneWidth(1, 0);

	// default create client will create a view if asked for it

	//Security
	AddEduTest = 0;
#ifdef NCM_EDU
	NCM_PROJECT.SetPostUserID(NCMProject::EDU_POST_USER_ID);
	CString Answer = theApp.GetEDUPassword();
	if(Answer == "Empty" || Answer.GetLength() < 3)
	{
		SecurityDlg Dlg;
		if( Dlg.DoModal() != IDOK )
			exit(0);
		Answer = Dlg.m_answer;
	}
	bool res = false;

	AddEduTest = int(Answer[0] * Answer[1] * Answer[2]);
	if(Security::CheckAnswer(Answer, &res) != Security::OK)
	{
		AfxMessageBox(IDS_MES_ERR_SEC);
		exit(0);
	}
	if(!res)
	{
		SecurityDlg Dlg;
		if( Dlg.DoModal() != IDOK )
			exit(0);
	}
#endif
#ifdef NCM_QUICK
	HK_MNG SentDongle;
	if(!SentDongle.ReadLicFile("Simulator.lic"))
	{
		AfxMessageBox(IDS_MES_ERR_SEC);
		exit(0);
	}
	theApp.Configuration = 0x0000;
	NCMConfig bit = 1 << 15; 
	for(int k = 0; k < SentDongle.lic_file->config.GetLength(); ++k)
	{
		theApp.Configuration += 
			SentDongle.lic_file->config[k] == '1' ? bit : 0;
		bit = bit >> 1;
	}
	if(SentDongle.InitKey() != 0)
	{
		bit = theApp.Configuration;
		if(theApp.ReadGuard(true) != 0)
		{
			AfxMessageBox(IDS_MES_ERR_SEC);
			exit(0);
		}
		theApp.Configuration = bit;
	}

#endif

// Initialize 3DConnexion interface
	Init3Dx();

// default create client will create a view if asked for it
	if (pContext != nullptr && pContext->m_pNewViewClass != nullptr)
	{
		CView *pView = (CView *)CreateView(pContext, AFX_IDW_PANE_FIRST);
		if (pView == nullptr)
			return FALSE;
		SetActiveView(pView);
	}
	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		| WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

	if (NCM_PROJECT.GetGlobalState().GetHostMode())
		cs.dwExStyle |= WS_EX_NOACTIVATE;
	cs.cy = int(::GetSystemMetrics(SM_CYSCREEN) * 0.8);
	cs.cx = int(::GetSystemMetrics(SM_CXSCREEN) * 0.8);

	return CFrameWndEx::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnStepFor() 
{
	static bool Busy = false;
	if(Busy)
		return;
	if(IsCuttingInProgress())
		return;
// Run specified number of steps from the current position
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	ASSERT(pDoc);
	if(!pDoc->CheckProgExist())
		return;
	if (((CLeftView*)GetLeftPane())->ClosePositionDlg())
		pDoc->UpdateAllViews(nullptr, N_LEFT);

	Busy = true;
	auto N = GetStepNum();
	if (N > 1 || N == -1)
		CWaitCursor w;
	if(theApp.IsInCuttingMode())
	{
		if (pDoc->IsThereAnyErrProgr())
		{
			CNCMApp::ProgErrMessage();
			Busy = false;
			return;
		}
		pDoc->RunNSteps(N);
		if(theApp.IsInResultMode())
		{
			pDoc->UpdateAllViews(nullptr,
				N_CADRCHANGED | N_OGL | N_SELECTTEXT);
		}
	}
	else
	{
		if(theApp.IsInResultMode())
		{
			pDoc->RunNSteps(N);
			pDoc->UpdateAllViews(nullptr,
				N_STATE | N_CADRCHANGED | N_OGL | N_SELECTTEXT);
		}
		else 
		{
			MaxRunStepsNum = N;
			if(theApp.IsInAnimateMode())
			{
				pDoc->StartAnimWalker(true);
				StartRunAnimateTimer();
			}
		}
	}
	Busy = false;
}

void CMainFrame::OnClose() 
{
	if (!CheckCutting())
		return;
	if (!CheckHosting())
		return;
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	if (!pDoc)
		return;
	if (!pDoc->MachToolEditor.CheckEditing())
		return;
	for (int ChInd = 0; ChInd < pDoc->GetChannelsCount(); ++ChInd)
	{
		auto pChannel = pDoc->GetChannel(ChInd);
	}

	CFrameWndEx::OnClose();

}

void CMainFrame::OnJumpBack() 
{
	if(IsCuttingInProgress())
		return;
	// Jump N steps back
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	ASSERT(pDoc);
	if (!pDoc->CheckProgExist())
		return;
	cadrID DestCadr = pDoc->GetIDByGlobPos(pDoc->GetGlobPos(), -GetStepNum());
	BProgram *pProg = pDoc->GetProgram(DestCadr);
	pDoc->GetUnit()->JumpToHCadrRel(pProg, DestCadr.Cadr, false);

	pDoc->UpdateAllViews(nullptr,
		N_OGL | N_STATE | N_CADRCHANGED | N_SELECTTEXT);
}

CWnd * CMainFrame::GetUnitPane()
{
	return &m_wndUnitPane;
}
CWnd * CMainFrame::GetAnalysePane()
{
	return &m_wndAnalysePane;
}
CWnd * CMainFrame::GetMTEditPane()
{
	return &m_wndMTEditPane;
}
CWnd * CMainFrame::GetMTEditTreePane()
{
	return &m_wndMTEditTreePane;
}
CWnd * CMainFrame::GetGeomPane()
{
	return &m_wndGeomPane;
}
void CMainFrame::UpdateOGLPanes(CDockablePane* pSender, LPARAM lHint, CObject* pHint)
{
	CNCMDoc* pDoc = static_cast<CNCMDoc*>(GetActiveDocument());
	if (!pDoc)
		return;

	pDoc->ShowCollided(IsRunAnimateActive());

	for each (auto pPane in AddOGLPanes)
		pPane->OnUpdate(pSender, lHint, pHint);
	pMainPane->OnUpdate(pSender, lHint, pHint);
}
void CMainFrame::DeleteAddOGLPanes()
{
	for (auto i = NOGLPane::GetFixedPanesNum(); i < AddOGLPanes.size(); ++i)
	{
		auto pPane = AddOGLPanes.at(i);
		pPane->ShowPane(FALSE, FALSE, FALSE);
		pPane->SetShowOnPaneMenu(FALSE);
	}
}
CWnd * CMainFrame::GetMainPane()
{
	return pMainPane;
}
CWnd * CMainFrame::GetMesurePane()
{
	return &m_wndMesurePane;
}
CWnd * CMainFrame::GetRegsPane()
{
	return &m_wndRegsPane;
}
CWnd * CMainFrame::GetReportPane()
{
	return &m_wndReportPane;
}
void CMainFrame::UpdateLeftView(CWnd* pSender, LPARAM lHint, CObject* pHint)
{
	return m_wndLeftView.OnUpdate(nullptr, lHint, pHint);
}


CWnd * CMainFrame::GetTextView()
{
	return &m_wndTextView;
}
CWnd * CMainFrame::GetLeftPane() const
{
	return &m_wndLeftView;
}
CWnd * CMainFrame::GetPSlidePane()
{
	return &m_wndPSlidePane;
}
CWnd * CMainFrame::GetPropPane()//propEditor
{
	return &m_wndPropPane;
}

void CMainFrame::OnUpdateCoord(CCmdUI *pCmdUI)
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
//	ASSERT(pDoc);
	if(!pDoc)
		return;
	CString str;
	str.Format((MM_INCH == M_MM) ? IDS_ST_F_MM : IDS_ST_F_IN
		, pDoc->StatusX, pDoc->StatusY, pDoc->StatusZ);
	pCmdUI->SetText(str);
}
void CMainFrame::OnUpdateCadr(CCmdUI *pCmdUI)
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
//	ASSERT(pDoc);
	if(!pDoc)
		return;
	CString str;
	CString ProgName = pDoc->NGetProgramList()->GetKey(pDoc->StatusCadr.Prog);
	ProgName.Delete(0, ProgName.ReverseFind('\\') + 1);
	str.Format("%d", pDoc->StatusCadr.Cadr + 1);
	str = str + ", " + ProgName;
	pCmdUI->SetText(str);
}

void CMainFrame::OnOglConfig() 
{
// Show OpenGL config strings
//Show MBSPForest parameters
	CString Version(glGetString(GL_VERSION ));
	CString Vendor(glGetString(GL_VENDOR ));
	CString Renderer(glGetString(GL_RENDERER));
	CString Extensions(glGetString(GL_EXTENSIONS));
	int MemSize = 0;//DXDirection5X::GetGlobSize();
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	int RendSize = 0;
	if(pDoc)
	{
		class NStock *pStock = pDoc->GetStock(0);
		if(pStock)
		{
			MBody *pStockBody = pStock->GetBody();
			if(pStockBody)
			{
				if(pStockBody->GetModelMode() == NGlobalState::DX)
				{
					GBodyRender *pBodyRender = pStock->GetRender();
					if(pBodyRender)
					{
						GQuadRenderDX5X *pBodyRenderDX5X = (GQuadRenderDX5X *)pBodyRender;
						RendSize = pBodyRenderDX5X->GetGlobSize();
					}
				}
			}
		}
	}
	CString DX5Xinfo;
	DX5Xinfo.Format(_T("DX5Xinfo: MemSize: %d, RendSize: %d"), MemSize, RendSize);
	AfxMessageBox(Version+"\n"+Vendor+"\n"+Renderer+"\n"/*+Extensions*/ + "\n"
		+ MFOREST.GetInfoStr()+"\n"+DX5Xinfo);
}

void CMainFrame::OnRunHereResult()
{
	if(IsCuttingInProgress())
		return;
	if(!theApp.IsInCuttingMode())
		return;
	CNCMDoc* pDoc = static_cast<CNCMDoc*>(GetActiveDocument());
	ASSERT(pDoc);
	if (pDoc->GetChannelsCount() > 1)
		return;
	CWaitCursor w;
	pDoc->ResetStocks();
	pDoc->ResetCutting();
	bool MS = pDoc->MileStones.IsActive();
	pDoc->MileStones.DeActivate();
	bool CS = pDoc->CheckStones.IsActive();
	pDoc->CheckStones.DeActivate();

	pDoc->GetCurChannel()->RunStepsCn(NEvalStopCrit(pDoc->GetUnit()->PrepRunToCurCadr()));

	if (MS)
		pDoc->MileStones.Activate();
	if (CS)
		pDoc->CheckStones.Activate();
	pDoc->UpdateAllViews(nullptr,N_OGL | N_STATE | N_SELECTTEXT | N_CADRCHANGED);
}

void CMainFrame::OnRunHere()
{
	CNCMDoc* pDoc = static_cast<CNCMDoc*>(GetActiveDocument());
	auto CadrsToRun = NEvalStopCrit(pDoc->GetUnit()->PrepRunToCurCadr());
	if (!theApp.SaveParams4Rest())
		return;
	pDoc->MileStones.DeActivate();
	pDoc->GetCurChannel()->RunStepsCn(CadrsToRun);
}

int CMainFrame::GetStepNum()
{
	return m_wndDlgBar.GetStepNum();
}

void CMainFrame::RunAnimateStep()
{
	if (AnimatePause || RunAnimateTimerID == 0)
		return;
	static bool IsRunning = false;
	if (IsRunning)
		return;
	IsRunning = true;
	CNCMDoc* pDoc = static_cast<CNCMDoc*>(GetActiveDocument());
	ASSERT(pDoc);
	flags = pDoc->GetUnit()->GetState()->Geom.GetType().flags;
	if (!pDoc->RunAnimateStep(MaxRunStepsNum))
		StopRunAnimate();
	pDoc->UpdateAllViews(nullptr, N_STATE | N_CADRCHANGED | N_SELECTTEXT | N_OGL, nullptr);
	IsRunning = false;
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent) 
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	ASSERT(pDoc);
	if(pDoc->GetBlockUpdateLevel() == 1)
		pDoc->SetBlockUpdateLevel(0);
	switch(nIDEvent)
	{
	case T_TEST:
		TestStep();
		break;
	case T_ANIMATE:
		theApp.CWinAppEx::OnIdle(-1);
		AnimateStepSingle();
		break;
	case T_PAUSE_ANIMATE:
		PauseAnimateStep();
		break;
	case T_RUN_ANIMATE:
		RunAnimateStep();
		break;
	case T_RESULT:
		ResultStep();
		break;
	default:
		break;
	}
	CFrameWndEx::OnTimer(nIDEvent);
}

void CMainFrame::OnStepTimer()
{
}

void CMainFrame::StopRunAnimate()
{
	KillTimer(RunAnimateTimerID);
	RunAnimateTimerID = 0;
	AnimatePause = false;
	MaxRunStepsNum = -1;
}

void CMainFrame::StepStop()
{
	if (!CanStepStopBeEnabled())
		return;
	if (theApp.IsInCuttingMode())
	{
		if (AnimatePause)
		{
			AnimatePause = false;
			RemovePauseAnim();
		}
		CNCMDoc* pDoc = static_cast<CNCMDoc*>(GetActiveDocument());
		if (pDoc != nullptr)
		{
			pDoc->StopAnimation();
			pDoc->JumpAfterStop();
		}
	}
	else
	{
		if (RunAnimateTimerID != 0)
			StopRunAnimate();
	}
}

void CMainFrame::OnStepStop()
{
	StepStop();
}

void CMainFrame::OnUpdateFastCut5x(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(CanFastCut5xBeEnabled());
	CNCMDoc* pDoc = static_cast<CNCMDoc*>(GetActiveDocument());
	if (!pDoc->IsFastCut5xEnabled())
	{
		if (pCmdUI->m_pMenu != nullptr)
			pCmdUI->m_pMenu->RemoveMenu(ID_FAST_CUT_MODE5X, MF_BYCOMMAND);
		return;
	}
	pCmdUI->SetCheck(NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::DX);
}

bool CMainFrame::CanFastCut5xBeEnabled()
{
	CNCMDoc* pDoc = static_cast<CNCMDoc*>(GetActiveDocument());
	if (!pDoc->IsFastCut5xEnabled())
	{
		return false; // Needed for ribbon
	}
	return !IsCuttingInProgress();

}

void CMainFrame::OnUpdateCutMode(CCmdUI* pCmdUI)
{
	CNCMDoc* pDoc = static_cast<CNCMDoc*>(GetActiveDocument());
	if (!pDoc->IsCutEnabled())
	{
		if (pCmdUI->m_pMenu != nullptr)
			pCmdUI->m_pMenu->RemoveMenu(ID_CUT_MODE, MF_BYCOMMAND);
		return;
	}
	pCmdUI->SetCheck(NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::BSP);
	pCmdUI->Enable(CanCutModeBeEnabled());
}

bool CMainFrame::CanCutModeBeEnabled()
{
	return !IsCuttingInProgress();
}

void CMainFrame::GetMessageString(UINT nID, CString &rMessage) const
{
	CFrameWndEx::GetMessageString( nID, rMessage);
}

void CMainFrame::OnReportsEdit() 
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
 	ASSERT(pDoc);
	CString str;
	NRepDlgRepEditor dlg;
	str.LoadString(IDS_REPORT_DEF);
	str = NCM_PROJECT.GetSettingsPath() + str;
	//dlg.pReps=pDoc->pRepArray; 
	NRepReportsArray RepArray;
	RepArray.Init(str);
	dlg.pReps=&RepArray;
	NStateUnitPane *pSView = (NStateUnitPane *)GetUnitPane();
	NMapStringToOb *pSObj = pSView->GetAllElemList();
	for each (const auto & el in *pSObj)
	{
		const CString& str = el.first;
		dlg.FildsStr.Add(str);
	}
	dlg.pUnit=pDoc->GetUnit();
	dlg.DoModal();
	CStringArray Reps;
	int i,k;
	for(i=0;i<pDoc->pRepArray->GetSize();i++)
	{
		Reps.Add((*(pDoc->pRepArray))[i].GetName());
	}
	pDoc->pRepArray->RemoveAll();
	str.LoadString(IDS_REPORT_DEF);
	str = NCM_PROJECT.GetSettingsPath() + str;
	pDoc->pRepArray->Init(str);
	for(i=0;i<Reps.GetSize();i++)
	{
		k=pDoc->pRepArray->FindReport(Reps[i]);
		if(k!=-1)
		{
			(*(pDoc->pRepArray))[k].SetMode(1);
		}
	}
	pDoc->pRepArray->RemoveExtraReps();
}

void CMainFrame::OnMenuReportsClear()
{
	CString str = NCM_PROJECT.GetSettingsPath() + "temp\\*.*";
	CFileFind FF;
	if(!FF.FindFile(str))
		return;
	while(FF.FindNextFile()>0)
	{
		if(FF.IsDots()!=TRUE)
		{
			str=FF.GetFilePath();
			remove(str);
		}
	}
	if(FF.IsDots()!=TRUE)
	{
		str=FF.GetFilePath();
		remove(str);
	}
}

void CMainFrame::OnMenuReportsView()
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	ASSERT(pDoc);
	NRepDlgReportView dlg;
	dlg.pReps=pDoc->pRepArray;
	dlg.DoModal();
}

void CMainFrame::OnUpdateMenuReportsView(CCmdUI *pCmdUI)
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	ASSERT(pDoc);
	pCmdUI->Enable(pDoc->pRepArray->GetSize() > 0);
}

void CMainFrame::OnMenuReportsSelect()
{
	NRepDlgActiveReports dlg;
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	ASSERT(pDoc);
	CString str;
	str.LoadString(IDS_REPORT_DEF);
	str = NCM_PROJECT.GetSettingsPath() + str;
	pDoc->pRepArray->Init(str);
	dlg.pRepArray=pDoc->pRepArray;
	dlg.DoModal();
	pDoc->pRepArray->RemoveExtraReps();
}

void CMainFrame::OnUpdateMenuReportsSelect(CCmdUI *pCmdUI)
{
	
}

void CMainFrame::OnWinMach()
{
	pMainPane->SetType(false);
	pMainPane->OnUpdate(NULL, N_SETTINGS, nullptr);
	SetChangeViewImage();
}


void CMainFrame::OnWinStock()
{
	pMainPane->SetType(true);
	pMainPane->OnUpdate(NULL, N_SETTINGS, nullptr);
	SetChangeViewImage();
}

void CMainFrame::OnUpdateWinStock(CCmdUI *pCmdUI)
{
	if (pMainPane != nullptr)
		pCmdUI->SetCheck(pMainPane->GetType());
}

void CMainFrame::OnUpdateStepRun(CCmdUI *pCmdUI)
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	if(!pDoc)
		return;
	pCmdUI->Enable(!pDoc->IsAnaliseActive() 
		&& !IsCuttingInProgress() && !pDoc->MachToolEditor.IsActive());
}

void CMainFrame::OnUpdateStepHere(CCmdUI *pCmdUI)
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	if(!pDoc)
		return;
	pCmdUI->Enable(!pDoc->IsAnaliseActive() 
		&& !IsCuttingInProgress() && !pDoc->MachToolEditor.IsActive());
}

void CMainFrame::OnUpdateStepFor(CCmdUI *pCmdUI)
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	if(!pDoc)
		return;
	pCmdUI->Enable(!pDoc->IsAnaliseActive() 
		&& !IsCuttingInProgress() && !pDoc->MachToolEditor.IsActive());
}


void CMainFrame::OnAnimatePause()
{
	AnimatePause = !AnimatePause;

	if(theApp.IsInCuttingMode() && !NCMComData::IsTurnEnabled() && NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::BSP)
	{
		m_wndDlgBar.SetAnimPause(AnimatePause);

		if(!AnimatePause)
			RemovePauseAnim();
		else
				m_wndDlgBar.EnableForBack();

		m_wndDlgBar.ForceRedraw();
	}
}

void CMainFrame::OnUpdateAnimatePause(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(theApp.IsInAnimateMode() && (AnimateTimerID != 0 || RunAnimateTimerID != 0));
	pCmdUI->SetCheck(AnimatePause);
}

bool CMainFrame::CanAnimateBePaused()
{
	return theApp.IsInAnimateMode() && (AnimateTimerID != 0 || RunAnimateTimerID != 0) && (!AnimatePause);
}

void CMainFrame::StartAnimateTimer(void)
{
	if (AnimateTimerID != 0)
		return;
	CNCMDoc* pDoc = static_cast<CNCMDoc*>(GetActiveDocument());
	if (!pDoc)
		return;
	pDoc->ClearAllCollisions();
	TimeStepRepeat = 100 / NSpeedPar::Extern.GetTimeStep();// 100 is a magic number. We whant the interface pause no more than 100 ms
	TimeStepRepeat = max(1, TimeStepRepeat);
	AnimateTimerID = SetTimer(T_ANIMATE, UINT(TimeStepRepeat * NSpeedPar::Extern.GetTimeStep() * 1.1), nullptr);// 1.1 is a magic number. We whant to prevent next OnTimer before previous processed
	AnimateTimeInterval.first = MainT_UNDEF;
	AnimateTimeInterval.second = MainT_UNDEF;
}

void CMainFrame::StartResultTimer(void)
{
	if (ResultTimerID != 0)
		return;
	ResultTimerID = SetTimer(T_RESULT, 40, nullptr);
}

void CMainFrame::StartRunAnimateTimer(void)
{
	CNCMDoc* pDoc = static_cast<CNCMDoc*>(GetActiveDocument());
	if (!pDoc)
		return;
	for (int ChInd = 0; ChInd < pDoc->GetChannelsCount(); ++ChInd)
	{
		auto pChannel = pDoc->GetChannel(ChInd);
		pChannel->GetProg2BSP().NeedNewPosition = false;
	}
	RunAnimateTimerID = SetTimer(T_RUN_ANIMATE, NSpeedPar::Extern.GetTimeStep(), nullptr);
}

void CMainFrame::PauseAnimateStep(void)
{
	if(!pPauseAnimate)
		return;
	if(PauseAnimateTimerID == 0)
		return;
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	static bool CadrEnded = true;
	if(CadrEnded)
	{// Prep new cadr
	 // Check if pProg in AnimateCadr is correct
		GAnimateCadr &AnimateCadr = *pPauseAnimate->GetCurAnimCadr();
		int ProgNum = AnimateCadr.GetProgNum();
		if (AnimateCadr.GetCadrIDhist().Prog != ProgNum)
			AnimateCadr.SetProg(pDoc->GetProgram(AnimateCadr.GetCadrIDhist().Prog));

		if(!pPauseAnimate->NextStep())
		{
			KillTimer(PauseAnimateTimerID);
			PauseAnimateTimerID = 0;
			PauseAnimFor = false;
			PauseAnimBack = false;
			m_wndDlgBar.ForceRedraw();
			AnimateUpdate();
			return;
		}
		//else
		//{// Set right tool
		//	const NAnimState *pAnimState = pPauseAnimate->GetCurAnimState();
		//	if(pAnimState)
		//		pDoc->GetCurChannel()->SetpActTool(pAnimState->GetTool());
		//}
	}

	CStocksCollection *sList = pDoc->NGetStockList();
	// Check if pProg in AnimateCadr is correct
	GAnimateCadr &AnimateCadr = *static_cast<GAnimateCadr*>(pDoc->GetCurChannel()->GetDynamicCadr());
	int ProgNum = AnimateCadr.GetProgNum();
	if (AnimateCadr.GetCadrIDhist().Prog != ProgNum)
		AnimateCadr.SetProg(pDoc->GetProgram(AnimateCadr.GetCadrIDhist().Prog));

	CadrEnded = !AnimateCadr.NextStep();

	AnimateUpdate();
	NDynParams Params;
	Params.SetXYZ(AnimateCadr.GetXYZ());
	Params.SetABC(AnimateCadr.GetABC());
	const auto LastTool = AnimateCadr.GetLastTool();
	if (LastTool != nullptr)
		Params.ToolPosition = LastTool->Position;
	Params.Time = AnimateCadr.GetTimeS();
	((NStateUnitPane*)GetUnitPane())->AnimateUpdateControl(Params);
}

void CMainFrame::AnimateStep(void)
{
	if (AnimateTimerID == 0)
		return;
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	ASSERT(pDoc);
	static bool AnimationWasPaused = false;
	if (!pDoc->GetDifferentBodyRotFlag())
		return;
	for (int i = 0; i < TimeStepRepeat; ++i)
	{
		auto StartTime = std::chrono::high_resolution_clock::now();
		bool OneChannelStarted = false;
		for (int k = 0; k < pDoc->GetChannelsCount(); ++k)
			OneChannelStarted |= pDoc->GetChannel(k)->GetCutting().Started;
		if (!OneChannelStarted)
			return;
		if (pDoc->IsInterrupted())
		{
			AnimatePause = false;
			AnimationWasPaused = false;
			RemovePauseAnim();
			KillTimer(AnimateTimerID);
			AnimateTimerID = 0;
			pDoc->FinishAnimation();
			pDoc->UpdateAllViews(nullptr, N_OGL | N_STATE | N_CADRCHANGED);
			pDoc->ClearAnimateQueues();
			return;
		}
		if (AnimatePause)
		{
			AnimationWasPaused = true;
			return;
		}
		//if (pDoc->GetChannel(0)->GetDynamicCadr() == nullptr)
		//	return;
		const bool IsUpdatedAnim = m_wndDlgBar.IsUpdatedAnim();
		bool GlobRenderFlag = false;
		double MinCutTime = 1.e18;
		for (int ChInd = 0; ChInd < pDoc->GetChannelsCount(); ++ChInd)
		{
			NChannel* pChannel = pDoc->GetChannel(ChInd);
			if (!pChannel->GetCutting().Started)
				continue;
			//if (pChannel->GetDynamicCadr() == nullptr)
			//	continue;
			GAnimateCadr* pAnimateCadr = static_cast<GAnimateCadr*>(pChannel->GetDynamicCadr());

			if(pAnimateCadr != nullptr)
				if (AnimationWasPaused || IsUpdatedAnim)
					pAnimateCadr->Recalc();

			bool RenderFlag = true;
			FAnimateDisp& AnimateDispCn = pChannel->GetAnimateDisp();
			if (pChannel->IsChainEnded())
			{
				if (!(AnimateDispCn.RendersQueue.IsEmpty() && pChannel->GetCutting().IsStopped()))
				{
					bool SetFlag = false;
					bool PeekFlag = false;
					while ((PeekFlag = !AnimateDispCn.RendersQueue.IsEmpty()) && !SetFlag)
					{// Set new AnimateCadr
						NAnimState* pAnimState = static_cast<NAnimState*>(AnimateDispCn.RendersQueue.Dequeue());
						AnimateDispCn.RendersQueueBack.Enqueue(pAnimState);// pAnimState will be changed in GAnimateCadr::Restart
						if (pAnimState->GetCrAnimCadr())
						{// recreate AnimateCadr
							pChannel->CrNewAnimateCadr(*pAnimState, &NSpeedPar::Extern);
							pAnimState->NULLRender();
							pAnimateCadr = static_cast<GAnimateCadr*>(pChannel->GetDynamicCadr());
						}
						if (pAnimateCadr->GetCadrIDhist().Prog != pAnimState->GetCadrIDhist().Prog)
							pAnimateCadr->SetProg(pChannel->GetProgram(pAnimState->GetCadrIDhist().Prog));
						SetFlag = pAnimateCadr->SetA(*pAnimState);

						if (!SetFlag)
						{
							GAnimateCadr* pMainAnimateCadr = static_cast<GAnimateCadr*>(pDoc->GetChannel(0)->GetDynamicCadr());
							pMainAnimateCadr->RestartFrom(*pAnimateCadr);//*pAnimState->pRender is replaced by the old values
						}
						pChannel->SetChainEnded(false);
					}
					if (!PeekFlag && !SetFlag) // we leave while cycle due to PeekFlag value
					{
						auto SleepTime = std::chrono::milliseconds(NSpeedPar::Extern.GetTimeStep()) + StartTime;
						std::this_thread::sleep_until(SleepTime);
						continue;
					}
					RenderFlag = SetFlag;
				}
			}
			GlobRenderFlag |= RenderFlag;
			if (pChannel->IsChainEnded())
			{
				if (AnimateDispCn.RendersQueue.IsEmpty() && pChannel->GetCutting().IsStopped())
				{

					pChannel->GetCutting().Started = false;
					if (pChannel->IsNeedProbe() || pChannel->IsNeedTouch())
					{
						KillTimer(AnimateTimerID);
						AnimateTimerID = 0;
						PostMessage(WM_APP_CUT_STATE_CHANGED, pChannel->GetChannelInd());
						return;
					}
					else
					{
						bool AllChannelsStopped = true;
						for (int k = 0; k < pDoc->GetChannelsCount(); ++k)
							AllChannelsStopped &= pDoc->GetChannel(k)->GetCutting().IsStopped();
						if (AllChannelsStopped)
						{
							KillTimer(AnimateTimerID);
							AnimateTimerID = 0;
							AnimateDispCn.ClearQueues();
							pDoc->GetChannel(0)->StopDynamicCadr();
						}
						if(ChInd != 0)
							pChannel->StopDynamicCadr();
						pDoc->UpdateAllViews(nullptr, N_OGL | N_STATE | N_CADRCHANGED);
					}
					if (pChannel->GetCutting().Started)
					{
						pDoc->SetCuttingEnabled(*pDoc->GetChannel(0), true);
						StartAppropTimer();
					}
					// Set machine tool to its right position
					pDoc->GetMachTool().ProcCadrStep(*pChannel->GetUnitCn(), *pChannel->GetUnitCn()->GetState());
					continue;
				}
			}
			// Check if pProg in AnimateCadr is correct
			const int ProgNum = pAnimateCadr->GetProgNum();
			if (pAnimateCadr->GetCadrIDhist().Prog != ProgNum)
				pAnimateCadr->SetProg(pChannel->GetProgram(pAnimateCadr->GetCadrIDhist().Prog));

			double CutTime = pAnimateCadr->GetTimeS();
			MinCutTime = fmin(MinCutTime, CutTime);
		}// for (int ChInd = 0; ChInd < pDoc->GetChannelsCount(); ++ChInd)
		GAnimateCadr* pMainAnimateCadr = static_cast<GAnimateCadr*>(pDoc->GetChannel(0)->GetDynamicCadr());
		if (pMainAnimateCadr != nullptr)
		{
			pDoc->ShowCollidedAnim(pMainAnimateCadr->GetCollID(), false);
			for (int ChInd = 0; ChInd < pDoc->GetChannelsCount(); ++ChInd)
			{// adjust time
				auto pChannel = pDoc->GetChannel(ChInd);
				if (!pChannel->GetCutting().Started)
					continue;
				if (pChannel->GetDynamicCadr() == nullptr)
					continue;
				GAnimateCadr& AnimateCadr = *static_cast<GAnimateCadr*>(pChannel->GetDynamicCadr());
				if (int(AnimateCadr.GetTimeS()) - int(MinCutTime) < 0.020) // int() needed to prevent problem when AnimateCadr.GetTime() - int(MinCutTime) < 0
				{
					pChannel->SetChainEnded(!AnimateCadr.NextStep());
					if (pChannel->IsChainEnded())
						pMainAnimateCadr->RestartFrom(AnimateCadr);
				}
			}
		}

		AnimationWasPaused = false;
		if (GlobRenderFlag)
		{
			if (AnimateText)
			{
				GDynamicCadr* pDynCadr = pDoc->GetCurChannel()->GetDynamicCadr();
				if (pDynCadr != nullptr)
				{
					static cadrID PrevID;
					const cadrID CurID = pDynCadr->GetCadrID();
					if (PrevID.ID != CurID.ID)
					{
						PrevID = CurID;
						pDoc->GotoCadr(pDoc->GetCurChannel(), CurID);
					}
				flags = pDynCadr->GetFlags();
				}
			}
			if (pMainAnimateCadr != nullptr)
			{
				const double CurTime = pMainAnimateCadr->GetTimeS();
				NDynParams Params;
				Params.SetXYZ(pMainAnimateCadr->GetXYZ());
				Params.SetABC(pMainAnimateCadr->GetABC());
				const auto LastTool = pMainAnimateCadr->GetLastTool();
				if (LastTool != nullptr)
					Params.ToolPosition = LastTool->Position;
				Params.Time = CurTime;
				((NStateUnitPane*)GetUnitPane())->AnimateUpdateControl(Params);
				const NCollisionID* pCollID = pMainAnimateCadr->GetCollID();
				pDoc->ShowCollidedAnim(pCollID, true);
				if (pCollID != nullptr)
				{
					if(pDoc->GetCollManager().GetStopOnColl() && pCollID->IsReason4Pause())
						ForcePause();
				}
			}
			AnimateUpdate();
			auto SleepTime = std::chrono::milliseconds(NSpeedPar::Extern.GetTimeStep()) + StartTime;
			std::this_thread::sleep_until(SleepTime);
		}
	}
}

void CMainFrame::AnimateStepSingle(void)
{
	static bool IsCalled = false;
	if (IsCalled)
		return;
	IsCalled = true;
	AnimateStep();
	IsCalled = false;
}

void CMainFrame::StartAppropTimer()
{
	switch (NCM_PROJECT.GetGlobalState().GetAnimMode())
	{
	case NGlobalState::Animation:
		StartAnimateTimer();
		break;
	case NGlobalState::Result:
		StartResultTimer();
		break;
	}
}

void CMainFrame::AnimateUpdate()
{
	static BTimeLimit TimeLimit((NSpeedPar::Extern.GetTimeStep() * 8) / 10);
	if(!TimeLimit.Check())
		return;
	CNCMDoc* pDoc = static_cast<CNCMDoc*>(GetActiveDocument());
	if (!pDoc)
		return;

	for each (auto pPane in AddOGLPanes)
		pPane->NAnimateUpdate();
	pMainPane->NAnimateUpdate();
	m_wndPSlidePane.NAnimateUpdate();
}

void CMainFrame::ResultStep(void)
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	ASSERT(pDoc);
	if( !pDoc->GetChannel(0)->GetCutting().Started )
		return;
	if (pDoc->IsInterrupted())
	{
		KillTimer(ResultTimerID);
		ResultTimerID = 0;
		pDoc->FinishAnimation();
		pDoc->UpdateAllViews(nullptr, N_OGL | N_STATE | N_CADRCHANGED);
		pDoc->ClearAnimateQueues();
		theApp.StopCuttingWRest();
		return;
	}
	if(pDoc->IsInterrupted())
	{
		pDoc->WaitAllChannels();
	}
	for (int ChInd = 0; ChInd < pDoc->GetChannelsCount(); ++ChInd)
	{
		auto pChannel = pDoc->GetChannel(ChInd);
		if (!pChannel->GetCutting().Started)
			continue;

		if (pChannel->GetCutting().IsStopped())
		{
			pChannel->GetCutting().Started = false;
			if (pChannel->IsNeedProbe() || pChannel->IsNeedTouch())
			{
				KillTimer(ResultTimerID);
				ResultTimerID = 0;
				pChannel->StopDynamicCadr();
				PostMessage(WM_APP_CUT_STATE_CHANGED, pChannel->GetChannelInd());
				return;
			}
			else
			{
				pChannel->StopDynamicCadr();
				if (pChannel->GetChannelInd() == 0)
				{
					KillTimer(ResultTimerID);
					ResultTimerID = 0;
					// Set machine tool to its right position
					pDoc->GetMachTool().ProcCadrStep(*pDoc->GetUnit(), *pDoc->GetUnit()->GetState());
					theApp.StopCuttingWRest();
				}
				pDoc->UpdateAllViews(nullptr, N_OGL | N_CADRCHANGED);
			}
		}
		if (!pChannel->GetProg2BSP().NeedNewPosition)
		{//Position is ready
			GDynamicCadr* pDynamicCadr = pChannel->GetDynamicCadr();
			if (pDynamicCadr == nullptr)
				continue;
			NAnimState AnimState(pChannel->GetProg2BSP().GetAnimState());
			if (AnimState.IsDefined())
			{
				if (pDynamicCadr->GetCadrIDhist().Prog != AnimState.GetCadrIDhist().Prog)
					pDynamicCadr->SetProg(pDoc->GetProgram(AnimState.GetCadrIDhist().Prog));
				pDynamicCadr->SetA(AnimState);
				if (ReRenderText)
					if(pChannel == pDoc->GetCurChannel())
						pDoc->GotoCadrFast(pChannel, pDynamicCadr->GetCadrID());
			}
			m_wndPSlidePane.NAnimateUpdate(N_SLIDERCHANGED);
			pChannel->GetProg2BSP().NeedNewPosition = true;
		}
	}
}

void CMainFrame::OnRunToBreakpoint()
{
	if (IsCuttingInProgress())
		StepStop();
	else
		RunToBreakpoint();
}

void CMainFrame::RunToBreakpoint()
{
	if (!CanRunToBreakpointBeEnabled())
		return;
	if (IsCuttingInProgress())
		return;
#ifdef NC_DEMO
	ProcCutHit();
#endif
	CWaitCursor w;
	CNCMDoc* pDoc = static_cast<CNCMDoc*>(GetActiveDocument());
	ASSERT(pDoc);
	if (!pDoc->CheckProgExist())
		return;
	if (((CLeftView*)GetLeftPane())->ClosePositionDlg())
		pDoc->UpdateAllViews(nullptr, N_LEFT);
	pDoc->ResetInterrupt();
	if (theApp.IsInCuttingMode())
	{
		if (pDoc->IsThereAnyErrProgr())
		{
			CNCMApp::ProgErrMessage();
			return;
		}
		pDoc->RunSteps(-1);
		pDoc->SetCurProgrTDT();
	}
	else
	{
		switch (NCM_PROJECT.GetGlobalState().GetAnimMode())
		{
		case NGlobalState::Animation:
		{
			pDoc->StartAnimWalker(false);
			StartRunAnimateTimer();
		}
		break;
		case NGlobalState::Result:
			pDoc->RunSteps(-1);
			pDoc->SetCurProgrTDT();
			pDoc->UpdateAllViews(nullptr, N_OGL | N_STATE | N_CADRCHANGED);
			break;
		}
	}
}

void CMainFrame::OnRunCollCheck()
{
}

void CMainFrame::OnReset()
{
	if (!CanResetBeEnabled())
		return;

	CWaitCursor w;
	if(RunAnimateTimerID != 0)
		StopRunAnimate();

	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	if(theApp.IsInCuttingMode())
	{
		pDoc->StopAnimation();
		if(AnimateTimerID != 0)
		{
			KillTimer(AnimateTimerID);
			AnimateTimerID = 0;
			for (int ChInd = 0; ChInd < pDoc->GetChannelsCount(); ++ChInd)
			{
				auto pChannel = pDoc->GetChannel(ChInd);
				pChannel->GetCutting().Started = false;
				pDoc->SetCuttingEnabled(*pChannel, false);
			}
		}
		if(AnimatePause)
		{
			AnimatePause = false;
			RemovePauseAnim();
		}
	}
	pDoc->Reset();
	theApp.ResetParams4Rest();
}

void CMainFrame::OnUpdateAutoUpdateProg(CCmdUI *pCmdUI)
{
	const CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	if(pDoc)
		pCmdUI->SetCheck(pDoc->GetAutoUpdateProg());
}

void CMainFrame::OnAutoUpdateProg()
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	if (pDoc)
		pDoc->SetAutoUpdateProg(!pDoc->GetAutoUpdateProg());
}

void CMainFrame::ShowPauseDlg(void)
{
	m_wndDlgBar.SetAnimDialog();
}
void CMainFrame::ShowResultDlg(void)
{
	m_wndDlgBar.SetResultDialog();
}

void CMainFrame::ShowPauseFDlg(void)
{
	m_wndDlgBar.SetAnimFDialog();
}

void CMainFrame::OnUpdateRunToBreakpoint(CCmdUI *pCmdUI)
{
	if (IsRibbonCreated())
	{
		if (m_wndRibbonBar.GetCategoryIndex(m_wndRibbonBar.GetActiveCategory()) != 0)
			return;
		m_wndRibbonBar.SetStartStopButton(IsCuttingInProgress());
		m_wndRibbonBar.UpdateStart();
	}
	else
	{
		pCmdUI->SetCheck(IsCuttingInProgress());
	}
	pCmdUI->Enable(IsCuttingInProgress() ? CanStepStopBeEnabled() : CanRunToBreakpointBeEnabled());
}

bool CMainFrame::CanRunToBreakpointBeEnabled()
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	if (!pDoc)
		return false;
	if (IsRibbonCreated())
	{
		return !pDoc->IsAnaliseActive() && !pDoc->MachToolEditor.IsActive();
	}
	else
	{
		return !pDoc->IsAnaliseActive() && !pDoc->MachToolEditor.IsActive() && !IsCuttingInProgress();
	}
}

void CMainFrame::OnStockInit()
{
	if(IsCuttingInProgress())
		return;
	CWaitCursor w;
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	pDoc->ResetStocks();

	if(pDoc->CalcVolume)
	{
		double volume = 0.0;
		if (pDoc->IsThereAnyStock())
		{
			CMapStringToNStock *list = pDoc->NGetStockList();
			for each (const auto & el in *list)
			{
				class NStock* pS = static_cast<class NStock*>(el.second);
				if (pS)
					volume += pS->GetVolume();
			}
		}
		pDoc->GetAnalyseState().CurVolume = volume;
	}

	pDoc->UpdateAllViews(nullptr, N_OGL | N_ANALYSE);
}

bool CMainFrame::IsAnimatePaused(void)
{
	return AnimatePause;
}

bool CMainFrame::IsCuttingInProgress(void)
{
	CNCMDoc* pDoc = static_cast<CNCMDoc*>(GetActiveDocument());
	if(IsAnyTimerActive())
		return true;
	for (int ChInd = 0; ChInd < pDoc->GetChannelsCount(); ++ChInd)
	{
		auto pChannel = pDoc->GetChannel(ChInd);
		if (pChannel->GetCutting().Started)
			return true;
	}
	return false;
}

bool CMainFrame::IsAnyTimerActive(void)
{
	return RunAnimateTimerID != 0 || ResultTimerID != 0 || AnimateTimerID != 0;
}

bool CMainFrame::IsComparisonActive(void)
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	if (!pDoc)
		return false;

	return pDoc->Comparison.IsActive();
}

void CMainFrame::OnUpdateStepStop(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(CanStepStopBeEnabled());
}

bool CMainFrame::CanStepStopBeEnabled()
{
	return IsCuttingInProgress() && pPauseAnimate == nullptr;
}

void CMainFrame::OnUpdateStepBack(CCmdUI *pCmdUI)
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	pCmdUI->Enable(!IsCuttingInProgress() && !pDoc->MachToolEditor.IsActive());
}

void CMainFrame::OnUpdateStockInit(CCmdUI *pCmdUI)
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	if(!pDoc)
		return;
	pCmdUI->Enable(!pDoc->IsAnaliseActive() 
		&& !IsCuttingInProgress());
}

void CMainFrame::OnUpdateReset(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(CanResetBeEnabled());
}

bool CMainFrame::CanResetBeEnabled()
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	if (!pDoc)
		return false;
	return !pDoc->IsAnaliseActive() && !pPauseAnimate;
}

void CMainFrame::OnBreakpointForward()
{
	if(IsCuttingInProgress())
		return;
// Jump forward until breakpoint
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	ASSERT(pDoc);
	if (!pDoc->CheckProgExist())
		return;
	pDoc->ClearAllCollisions();
	if(!(pDoc->MileStones.IsActive() || pDoc->MileStones.GetCount() < 1) && !pDoc->GetCurChannel()->IsProbePresent())
		pDoc->GetUnit()->JumpToProjEnd();
	else if(pDoc->MileStones.HaveToolChangeOnly() && !pDoc->GetCurChannel()->IsProbePresent())
		pDoc->JumpToNextTool();
	else
		pDoc->GetUnit()->JumpForToBreakpoint();
	pDoc->SetCurProgrTDT();
	pDoc->UpdateAllViews(nullptr,
		N_OGL | N_STATE | N_CADRCHANGED);
}

void CMainFrame::OnUpdateBreakpointForward(CCmdUI *pCmdUI)
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
//	ASSERT(pDoc);
	if(!pDoc)
		return;
	pCmdUI->Enable(!IsCuttingInProgress() && !pDoc->MachToolEditor.IsActive());
}

void CMainFrame::OnBreakpointTable()
{
	if(BreakpointDlg.DoModal() == IDOK)
	{
		CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
		pDoc->MileStones.FillFromDlg(BreakpointDlg);
	}
}

void CMainFrame::OnUpdateBreakpointTable(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!IsCuttingInProgress());
}

void CMainFrame::OnBreakpointBack()
{
	if(IsCuttingInProgress())
		return;
// Jump back to the nearest breakpoint
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	ASSERT(pDoc);
	if (!pDoc->CheckProgExist())
		return;
	if(!pDoc->MileStones.IsActive() || pDoc->MileStones.GetCount() < 1)
		pDoc->GetUnit()->JumpToProjStart();
	else if(pDoc->MileStones.HaveToolChangeOnly())
		pDoc->JumpToPrevTool();
	else
		pDoc->GetUnit()->JumpToPrevBreakpoint();
	pDoc->SetCurProgrTDT();

	pDoc->UpdateAllViews(nullptr,
		N_OGL | N_STATE | N_CADRCHANGED);
}

void CMainFrame::OnUpdateBreakpointBack(CCmdUI *pCmdUI)
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	pCmdUI->Enable(!IsCuttingInProgress() && !pDoc->MachToolEditor.IsActive());
}

bool CMainFrame::SetCurChannel(int i)
{
	CNCMDoc* pDoc = static_cast<CNCMDoc*>(GetActiveDocument());
	if (pDoc == nullptr)
		return false;
	if (pDoc->SetCurChannel(i))
	{
		NTextView* pTextView = (NTextView*)GetTextView();
		if (pTextView != nullptr)
		{
			pTextView->SaveSciDoc();// To ensire undo/redo works properly while channels change
			pTextView->SendToScentilla(SCI_SETDOCPOINTER, 0, (LPARAM)nullptr);
		}
		pDoc->UpdateAllViews(nullptr, N_UNIT_CHANGED | N_TEXT | N_LEFT | N_STATE | N_TOOLCHANGED | N_PROGRAMCHANGED);
		return true;
	}
	return false;
}

void CMainFrame::OnSetNextChannel()
{
	CNCMDoc* pDoc = static_cast<CNCMDoc*>(GetActiveDocument());
	if (pDoc == nullptr)
		return;
	SetCurChannel((pDoc->GetCurChannel()->GetChannelInd() + 1) % pDoc->GetChannelsCount());
}  

void CMainFrame::OnNewChannel()
{
	CNCMDoc* pDoc = static_cast<CNCMDoc*>(GetActiveDocument());
	if (pDoc == nullptr)
		return;
	pDoc->AddNewChannel();
	SetCurChannel(pDoc->GetChannelsCount() - 1);
}

void CMainFrame::OnDeleteChannel()
{
	CNCMDoc* pDoc = static_cast<CNCMDoc*>(GetActiveDocument());
	pDoc->DeleteChannel(pDoc->GetCurChannel()->GetChannelInd());
}

void CMainFrame::OnStockCr()
{}

void CMainFrame::OnNewProject()
{}

void CMainFrame::OnRibSave()
{}

void CMainFrame::OnStockEd()
{}

void CMainFrame::OnUpdateStockEd(CCmdUI *pCmdUI)
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	if (!pDoc)
		return;

	pCmdUI->Enable(pDoc->NGetStockList()->size() == 1);
}

void CMainFrame::OnMBreakpointaddOnOff()
{
	BreakpointDlg.v_BREAKPOINT_ON_OFF = !BreakpointDlg.v_BREAKPOINT_ON_OFF;
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	pDoc->MileStones.FillFromDlg(BreakpointDlg);
}

void CMainFrame::OnUpdateMBreakpointaddOnOff(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(BreakpointDlg.v_BREAKPOINT_ON_OFF);
}

void CMainFrame::OnBp1()
{
	BreakpointDlg.v_MiProgStartEnd = !BreakpointDlg.v_MiProgStartEnd;
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	pDoc->MileStones.FillFromDlg(BreakpointDlg);
}

void CMainFrame::OnUpdateBp1(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(BreakpointDlg.v_MiProgStartEnd);
}
void CMainFrame::OnBp2()
{
	BreakpointDlg.v_MiProgOptStop = !BreakpointDlg.v_MiProgOptStop;
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	pDoc->MileStones.FillFromDlg(BreakpointDlg);
	
}

void CMainFrame::OnUpdateBp2(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(BreakpointDlg.v_MiProgOptStop);
}
void CMainFrame::OnBp3()
{
	BreakpointDlg.v_MiToolChange = !BreakpointDlg.v_MiToolChange;
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	pDoc->MileStones.FillFromDlg(BreakpointDlg);
}

void CMainFrame::OnUpdateBp3(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(BreakpointDlg.v_MiToolChange);
}
void CMainFrame::OnBp4()
{
	BreakpointDlg.v_MiBookmark = !BreakpointDlg.v_MiBookmark;
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	pDoc->MileStones.FillFromDlg(BreakpointDlg);
}

void CMainFrame::OnUpdateBp4(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(BreakpointDlg.v_MiBookmark);
}

void CMainFrame::OnBp5()
{
	BreakpointDlg.v_MiLifeTime = !BreakpointDlg.v_MiLifeTime;
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	pDoc->MileStones.FillFromDlg(BreakpointDlg);
}

void CMainFrame::OnUpdateBp5(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(BreakpointDlg.v_MiLifeTime);
}

void CMainFrame::OnBp6()
{
	BreakpointDlg.v_MiCollision = !BreakpointDlg.v_MiCollision;
	CNCMDoc* pDoc = static_cast<CNCMDoc*>(GetActiveDocument());
	pDoc->MileStones.FillFromDlg(BreakpointDlg);
}

void CMainFrame::OnUpdateBp6(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(BreakpointDlg.v_MiCollision);
}

void CMainFrame::OnStartCollcheck()
{
	if (theApp.IsCollChecking())
		StepStop();
	else
		theApp.StartFullCollisionCheck();
	//CNCMDoc* pDoc = static_cast<CNCMDoc*>(GetActiveDocument());
	//auto& CollMan = pDoc->GetCollManager();
	//CollMan.SetToolStockActive(!CollMan.IsToolStockActive());
}

void CMainFrame::OnUpdateStartCollcheck(CCmdUI* pCmdUI)
{
	CNCMDoc* pDoc = static_cast<CNCMDoc*>(GetActiveDocument());
	BOOL Enable = CanCollisionBeEnabled() || pDoc->GetCollManager().IsToolStockActive();
	pCmdUI->Enable(Enable);
	pCmdUI->SetCheck(false);
	m_wndRibbonBar.UpdateStartCollcheck(pDoc->GetCollManager().IsCollisionFound(), pDoc->GetCollManager().GetLogIsStale(), theApp.IsCollChecking());
}

void CMainFrame::OnMeasureDistance()
{
	CNCMDoc* pDoc = static_cast<CNCMDoc*>(GetActiveDocument());
	pDoc->SetMesureDistanceActive(!pDoc->IsMesureDistanceActive());
	if (pDoc->IsMesureDistanceActive())
		m_wndGeomPane.ShowPane(TRUE, FALSE, TRUE);
	pDoc->UpdateAllViews(NULL, N_STATE | N_OGL);
}

void CMainFrame::OnUpdateMeasureDistance(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(CanMesureDistanceBeEnabled());
	CNCMDoc* pDoc = static_cast<CNCMDoc*>(GetActiveDocument());
	pCmdUI->SetCheck(pDoc->IsMesureDistanceActive());
}

void CMainFrame::OnStopOnColl()
{
	CNCMDoc* pDoc = static_cast<CNCMDoc*>(GetActiveDocument());
	pDoc->GetCollManager().SetStopOnColl(!pDoc->GetCollManager().GetStopOnColl());
}

void CMainFrame::OnUpdateStopOnColl(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsCollisionAllowed());
	CNCMDoc* pDoc = static_cast<CNCMDoc*>(GetActiveDocument());
	pCmdUI->SetCheck(pDoc->GetCollManager().GetStopOnColl());
}

void CMainFrame::LoadTBFromIcons(void)
{
	for each (auto pPane in AddOGLPanes)
	{
		if (pPane)
			if (pPane->GetSafeHwnd())
				pPane->LoadTBFromIcons();
	}
	if (pMainPane)
		pMainPane->LoadTBFromIcons();
	HMODULE hins = GetModuleHandle(_T("ResInv.dll"));
	if (hins == nullptr)
		return;
	NSetToolBar::HIns = hins;
	int BSize = IsLargeIcons() ? 24 : 16;
	NSetToolBar::BSize = BSize;
	CMFCToolBar::GetImages()->Clear();
	CMFCToolBar::GetColdImages()->Clear();
	CMFCToolBar::GetLargeColdImages()->Clear();
	CMFCToolBar::GetLargeImages()->Clear();
	CMFCToolBar::GetMenuImages()->Clear();
	CMFCToolBar::GetDisabledMenuImages()->Clear();
	CMFCToolBar::GetDisabledImages()->Clear();
	CMFCToolBar::GetLargeDisabledImages()->Clear();

	if (IsRibbonCreated())
	{
		//Индикаторы (Ribbon)
		{
			UINT IDs[8] =
			{ ID_STATETB0, ID_STATETB1, ID_STATETB2, ID_STATETB3, ID_STATETB4,
			ID_STATETB5, ID_STATETB6, ID_STATETB7 };
			NSetToolBar SetToolBar(m_indicatorsToolBar, IDs, 8);
			SetToolBar.SetButton(0, IDI_RAPIDOFF, IDI_RAPIDON);
			SetToolBar.SetButton(1, IDI_COOLON, IDI_M8OFF);
			SetToolBar.SetButton(2, IDI_SPEEDON, IDI_SPEEDOFF);
			SetToolBar.SetButton(3, IDI_PPON, IDI_PPOFF);
			SetToolBar.SetButton(4, IDI_G81ONN, IDI_G81OFF);
			SetToolBar.SetButton(5, IDI_G43ON, IDI_G43OFF);
			SetToolBar.SetButton(6, IDI_G41ON, IDI_G41OFF);
			SetToolBar.SetButton(7, IDI_TOOLON, IDI_TOOLOFF);
		}
		
		if (NCMComData::GetType() == AT_MILL)
		{//Тулбар виды Mill (Ribbon)
			UINT IDs[16] =
			{ ID_TB_ZOOM_W, ID_TB_ZOOM_SC, ID_TB_ZOOM_P, ID_TB_ZOOM_ALL,
			ID_TB_ZOOM_R, ID_TB_ZOOM_Z, ID_ROTANGLES, ID_TB_ROTATECENTERONOFF, ID_ROTCENTER, ID_TB_VIEW_PREV, ID_TB_VIEW_NEXT, ID_TB_UNDO, ID_TB_REDO, ID_TB_VIEW_ORTHO, ID_TB_VIEW_ALONGTOOL, ID_TB_VIEW_NORMALTOOBJECT};
			NSetToolBar SetToolBar(m_viewToolBar, IDs, 16);
			SetToolBar.SetButton(0, IDI_TB_ZOOM, IDI_TB_ZOOM_H);
			SetToolBar.SetButton(1, IDI_TB_ZOOMD, IDI_TB_ZOOMD_H);
			SetToolBar.SetButton(2, IDI_TB_PAN, IDI_TB_PAN_H);
			SetToolBar.SetButton(3, IDI_TB_ZALL, IDI_TB_ZALL_H);
			SetToolBar.SetButton(4, IDI_TB_DROT, IDI_TB_DROT_H);
			SetToolBar.SetButton(5, IDI_TB_ZROT, IDI_TB_ZROT_H);
			SetToolBar.SetButton(6, IDI_VIEW_ANGLEROT, IDI_VIEW_ANGLEROT);
			SetToolBar.SetButton(7, IDI_TB_ROTATECENTERONOFF, IDI_TB_ROTATECENTERONOFF);
			SetToolBar.SetButton(8, IDI_VIEW_CENTRROT, IDI_VIEW_CENTRROT);
			SetToolBar.SetButton(9, IDI_TB_LVIEW, IDI_TB_LVIEW_H);
			SetToolBar.SetButton(10, IDI_TB_NVIEW, IDI_TB_NVIEW_H);
			SetToolBar.SetButton(11, IDI_TB_UNDOPOS, IDI_TB_UNDOPOS_H);
			SetToolBar.SetButton(12, IDI_TB_REDOPOS, IDI_TB_REDOPOS_H);
			SetToolBar.SetButton(13, IDI_TB_NVIEW, IDI_TB_NVIEW_H);
			SetToolBar.SetButton(14, IDI_TB_NVIEW, IDI_TB_NVIEW_H);
			SetToolBar.SetButton(15, IDI_TB_NVIEW, IDI_TB_NVIEW_H);
		}
		if (NCMComData::IsTurnEnabled())
		{
			if (NCMComData::IsMillEnabled())
			{//Меню виды  токарно-фрезерный проект (RIBBON)
				UINT IDs[13] =
				{ ID_TB_ZOOM_W, ID_TB_ZOOM_SC, ID_TB_ZOOM_P, ID_TB_ZOOM_ALL,
				ID_TB_ZOOM_R, ID_ROTANGLES, ID_TB_ROTATECENTERONOFF, ID_ROTCENTER, ID_TB_VIEW_PREV, ID_TB_VIEW_NEXT, ID_STOCK_STATE, ID_TB_UNDO, ID_TB_REDO };
				NSetToolBar SetToolBar(m_viewToolBar, IDs, 13);
				SetToolBar.SetButton(0, IDI_TB_ZOOM, IDI_TB_ZOOM_H);
				SetToolBar.SetButton(1, IDI_TB_ZOOMD, IDI_TB_ZOOMD_H);
				SetToolBar.SetButton(2, IDI_TB_PAN, IDI_TB_PAN_H);
				SetToolBar.SetButton(3, IDI_TB_ZALL, IDI_TB_ZALL_H);
				SetToolBar.SetButton(4, IDI_TB_DROT, IDI_TB_DROT_H);
				SetToolBar.SetButton(5, IDI_VIEW_ANGLEROT, IDI_VIEW_ANGLEROT);
				SetToolBar.SetButton(6, IDI_TB_ROTATECENTERONOFF, IDI_TB_ROTATECENTERONOFF);
				SetToolBar.SetButton(7, IDI_VIEW_CENTRROT, IDI_VIEW_CENTRROT);
				SetToolBar.SetButton(8, IDI_TB_LVIEW, IDI_TB_LVIEW_H);
				SetToolBar.SetButton(9, IDI_TB_NVIEW, IDI_TB_NVIEW_H);
				SetToolBar.SetButton(10, IDI_TB_MT, IDI_TB_MT);
				SetToolBar.SetButton(11, IDI_TB_UNDOPOS, IDI_TB_UNDOPOS_H);
				SetToolBar.SetButton(12, IDI_TB_REDOPOS, IDI_TB_REDOPOS_H);
			}
			else
			{//Меню виды токарный проект (RIBBON)
				UINT IDs[13] =
				{ ID_TB_ZOOM_W, ID_TB_ZOOM_SC, ID_TB_ZOOM_P, ID_TB_ZOOM_ALL,
				ID_TB_ZOOM_R, ID_ROTANGLES, ID_TB_ROTATECENTERONOFF, ID_ROTCENTER, ID_TB_VIEW_PREV, ID_TB_VIEW_NEXT, ID_PA_STOCK_SEC_VIEW, ID_TB_UNDO, ID_TB_REDO };
				NSetToolBar SetToolBar(m_viewToolBar, IDs, 13);
				SetToolBar.SetButton(0, IDI_TB_ZOOM, IDI_TB_ZOOM_H);
				SetToolBar.SetButton(1, IDI_TB_ZOOMD, IDI_TB_ZOOMD_H);
				SetToolBar.SetButton(2, IDI_TB_PAN, IDI_TB_PAN_H);
				SetToolBar.SetButton(3, IDI_TB_ZALL, IDI_TB_ZALL_H);
				SetToolBar.SetButton(4, IDI_TB_DROT, IDI_TB_DROT_H);
				SetToolBar.SetButton(5, IDI_VIEW_ANGLEROT, IDI_VIEW_ANGLEROT);
				SetToolBar.SetButton(6, IDI_TB_ROTATECENTERONOFF, IDI_TB_ROTATECENTERONOFF);
				SetToolBar.SetButton(7, IDI_VIEW_CENTRROT, IDI_VIEW_CENTRROT);
				SetToolBar.SetButton(8, IDI_TB_LVIEW, IDI_TB_LVIEW_H);
				SetToolBar.SetButton(9, IDI_TB_NVIEW, IDI_TB_NVIEW_H);
				SetToolBar.SetButton(10, IDI_TB_TURNSECT, IDI_TB_TURNSECT);
				SetToolBar.SetButton(11, IDI_TB_UNDOPOS, IDI_TB_UNDOPOS_H);
				SetToolBar.SetButton(12, IDI_TB_REDOPOS, IDI_TB_REDOPOS_H);
			}
		}
	}
	else
	{
		int ChangeWinItem = 7; // The number of Window item
		if (NCMComData::IsTurnEnabled())
		{
			ChangeWinItem = 6;
			if (NCMComData::IsMillEnabled())
			{
				//Стандартный ToolBar
				{
					UINT IDs[3] =
					{ ID_FILE_NEW_MILLTURN, ID_FILE_OPEN, ID_FILE_SAVE };
					NSetToolBar SetToolBar(m_wndToolBar, IDs, 3);
					SetToolBar.SetButton(0, IDI_TB_NEW);
					SetToolBar.SetButton(1, IDI_TB_OPEN);
					SetToolBar.SetButton(2, IDI_TB_SAVE);
				}
				//Меню виды  токарно-фрезерный проект
				{
					UINT IDs[13] =
					{ ID_TB_ZOOM_W, ID_TB_ZOOM_SC, ID_TB_ZOOM_P, ID_TB_ZOOM_ALL,
					ID_TB_ZOOM_R, ID_ROTANGLES, ID_TB_ROTATECENTERONOFF, ID_ROTCENTER, ID_TB_VIEW_PREV, ID_TB_VIEW_NEXT, ID_STOCK_STATE, ID_TB_UNDO, ID_TB_REDO };
					NSetToolBar SetToolBar(m_viewToolBar, IDs, 13);
					SetToolBar.SetButton(0, IDI_TB_ZOOM, IDI_TB_ZOOM_H);
					SetToolBar.SetButton(1, IDI_TB_ZOOMD, IDI_TB_ZOOMD_H);
					SetToolBar.SetButton(2, IDI_TB_PAN, IDI_TB_PAN_H);
					SetToolBar.SetButton(3, IDI_TB_ZALL, IDI_TB_ZALL_H);
					SetToolBar.SetButton(4, IDI_TB_DROT, IDI_TB_DROT_H);
					SetToolBar.SetButton(5, IDI_VIEW_ANGLEROT, IDI_VIEW_ANGLEROT);
					SetToolBar.SetButton(6, IDI_TB_ROTATECENTERONOFF, IDI_TB_ROTATECENTERONOFF);
					SetToolBar.SetButton(7, IDI_VIEW_CENTRROT, IDI_VIEW_CENTRROT);
					SetToolBar.SetButton(8, IDI_TB_LVIEW, IDI_TB_LVIEW_H);
					SetToolBar.SetButton(9, IDI_TB_NVIEW, IDI_TB_NVIEW_H);
					SetToolBar.SetButton(10, IDI_TB_MT, IDI_TB_MT);
					SetToolBar.SetButton(11, IDI_TB_UNDOPOS, IDI_TB_UNDOPOS_H);
					SetToolBar.SetButton(12, IDI_TB_REDOPOS, IDI_TB_REDOPOS_H);
			}
			}
			else
			{
				//Стандартный ToolBar
				{
					UINT IDs[3] =
					{ ID_FILE_NEW_TURN, ID_FILE_OPEN, ID_FILE_SAVE };
					NSetToolBar SetToolBar(m_wndToolBar, IDs, 3);
					SetToolBar.SetButton(0, IDI_TB_NEW);
					SetToolBar.SetButton(1, IDI_TB_OPEN);
					SetToolBar.SetButton(2, IDI_TB_SAVE);
				}
				//Меню виды токарный проект
				{
					UINT IDs[13] =
					{ ID_TB_ZOOM_W, ID_TB_ZOOM_SC, ID_TB_ZOOM_P, ID_TB_ZOOM_ALL,
					ID_TB_ZOOM_R, ID_ROTANGLES, ID_TB_ROTATECENTERONOFF, ID_ROTCENTER, ID_TB_VIEW_PREV, ID_TB_VIEW_NEXT, ID_PA_STOCK_SEC_VIEW, ID_TB_UNDO, ID_TB_REDO };
					NSetToolBar SetToolBar(m_viewToolBar, IDs, 13);
					SetToolBar.SetButton(0, IDI_TB_ZOOM, IDI_TB_ZOOM_H);
					SetToolBar.SetButton(1, IDI_TB_ZOOMD, IDI_TB_ZOOMD_H);
					SetToolBar.SetButton(2, IDI_TB_PAN, IDI_TB_PAN_H);
					SetToolBar.SetButton(3, IDI_TB_ZALL, IDI_TB_ZALL_H);
					SetToolBar.SetButton(4, IDI_TB_DROT, IDI_TB_DROT_H);
					SetToolBar.SetButton(5, IDI_VIEW_ANGLEROT, IDI_VIEW_ANGLEROT);
					SetToolBar.SetButton(6, IDI_TB_ROTATECENTERONOFF, IDI_TB_ROTATECENTERONOFF);
					SetToolBar.SetButton(7, IDI_VIEW_CENTRROT, IDI_VIEW_CENTRROT);
					SetToolBar.SetButton(8, IDI_TB_LVIEW, IDI_TB_LVIEW_H);
					SetToolBar.SetButton(9, IDI_TB_NVIEW, IDI_TB_NVIEW_H);
					SetToolBar.SetButton(10, IDI_TB_TURNSECT, IDI_TB_TURNSECT);
					SetToolBar.SetButton(11, IDI_TB_UNDOPOS, IDI_TB_UNDOPOS_H);
					SetToolBar.SetButton(12, IDI_TB_REDOPOS, IDI_TB_REDOPOS_H);
				}
			}
			//Режимы симуляции
			{
				{
					UINT IDs[3] =
					{ ID_CUT_MODE, ID_DONTCUT_MODE, ID_FAST_CUT_MODE5X };
					NSetToolBar SetToolBar(m_modeToolBar, IDs, 3);
					SetToolBar.SetButton(0, IDI_TB_CUTMODE, IDI_TB_CUTMODE_H);
					SetToolBar.SetButton(1, IDI_TB_DONTCUTMODE, IDI_TB_DONTCUTMODE_H);
					SetToolBar.SetButton(2, IDI_TB_CUTMODE5X, IDI_TB_CUTMODE5X_H);
				}
				UINT IDs[5] =
				{ ID_CHANGE_CUT_MODE, ID_SEPARATOR, ID_SMOTHNESS_MODE_F, ID_SMOTHNESS_MODE,
				ID_RESULT_MODE, };
				NSetToolBar SetToolBar(m_modeToolBar, IDs, 5);
				SetToolBar.SetButton(0, IDI_TB_WIRE, IDI_TB_WIRE_H);
				SetToolBar.SetButton(2, IDI_TB_SMOTHNESSMODE, IDI_TB_SMOTHNESSMODE_H);
				SetToolBar.SetButton(3, IDI_TB_SMOTH, IDI_TB_SMOTH_H);
				SetToolBar.SetButton(4, IDI_TB_RESULT, IDI_TB_RESULT_H);
				HMENU hMenu = m_wndMenuBar.GetHMenu();
				hMenu = GetSubMenu(hMenu, 2);
				hMenu = GetSubMenu(hMenu, 2);

				m_modeToolBar.ReplaceButton(ID_CHANGE_CUT_MODE, CMFCToolBarMenuButton(ID_CHANGE_CUT_MODE, hMenu, -1));
				SetModeImage();
			}


			//Навигация 
			{
				UINT IDs[8] =
				{ ID_STEP_HERE, ID_STOCK_INIT, ID_RUN_TO_BREAKPOINT, ID_BREAKPOINT_BACK, ID_BREAKPOINT_FORWARD,
				ID_ANIMATE_PAUSE, ID_STEP_STOP, ID_RESET };
				NSetToolBar SetToolBar(m_navigToolBar, IDs, 8);
				SetToolBar.SetButton(0, IDI_TB_HERE, IDI_TB_HERE_H);
				SetToolBar.SetButton(1, IDI_TB_INIT, IDI_TB_INIT_H);
				SetToolBar.SetButton(2, IDI_TB_RUN, IDI_TB_RUN_H);
				SetToolBar.SetButton(3, IDI_TB_FB, IDI_TB_FB_H);
				SetToolBar.SetButton(4, IDI_TB_FF, IDI_TB_FF_H);
				SetToolBar.SetButton(5, IDI_TB_PAUSE, IDI_TB_PAUSE_H);
				SetToolBar.SetButton(6, IDI_TB_STOP, IDI_TB_STOP_H);
				SetToolBar.SetButton(7, IDI_TB_RESET, IDI_TB_RESET_H);
			}

			{
				UINT IDs[4] = { 0, 0, 0, 0 };
				NSetToolBar SetToolBar(m_wndDlgBar.DlgBar, IDs, 4);
				m_wndDlgBar.DlgBar.SetSliderHeight(BSize - 4);
				SetToolBar.SetButton(&m_wndDlgBar.DlgBar.ButtonB, IDI_TB_BACK_N, IDI_TB_BACK_N_H);
				SetToolBar.SetButton(&m_wndDlgBar.DlgBar.ButtonF, IDI_TB_N, IDI_TB_N_H);
				SetToolBar.SetButton(&m_wndDlgBar.DlgBar.ButtonBackP, IDI_NB, IDI_NB_H);
				SetToolBar.SetButton(&m_wndDlgBar.DlgBar.ButtonForP, IDI_TB_N, IDI_TB_N_H);
				SetToolBar.SetButton(&m_wndDlgBar.DlgBar.ButtonBack, IDI_TB_PAUSE, IDI_TB_PAUSE_H);
				SetToolBar.SetButton(&m_wndDlgBar.DlgBar.ButtonFor, IDI_TB_PAUSE, IDI_TB_PAUSE_H);
				NSpeedPar::Extern.SetTrackFeed(false);
				ShowPauseDlg();
			}
			//Точки останова
			{
				UINT IDs[7] =
				{ ID_BP1, ID_BP2, ID_BP3, ID_BP4, ID_BP5, ID_M_BREAKPOINTADD_ON_OFF, ID_BREAKPOINT_TABLE };
				NSetToolBar SetToolBar(m_NBreakpointToolBar, IDs, 7);
				SetToolBar.SetButton(0, IDI_BP1);
				SetToolBar.SetButton(1, IDI_BP2);
				SetToolBar.SetButton(2, IDI_BP3);
				SetToolBar.SetButton(3, IDI_BP4);
				SetToolBar.SetButton(4, IDI_BP5);
				SetToolBar.SetButton(5, IDI_BP_ADD);
				SetToolBar.SetButton(6, IDI_BP_ALL);
			}
			//Анализ 
			{
				//				m_inspectToolBar.ShowPane(TRUE, FALSE, TRUE);
				UINT IDs[6] =
				{ ID_MEASURE_DISTANCE, ID_MEASURE, ID_CREATE_ENT, ID_VIEW_SECTION, ID_COMPARISON, ID_INSPECT_TOOLPATHSTATISTICS };
				NSetToolBar SetToolBar(m_inspectToolBar, IDs, 6);
				SetToolBar.SetButton(0, IDI_DISTANCE, IDI_DISTANCE_H);
				SetToolBar.SetButton(1, IDI_MEASURE, IDI_MEASURE_H);
				SetToolBar.SetButton(2, IDI_SIZES, IDI_SIZES_H);
				SetToolBar.SetButton(3, IDI_SECTION, IDI_SECTION_H);
				SetToolBar.SetButton(4, IDI_COMPARE);
				SetToolBar.SetButton(5, IDI_INSP_TIME, IDI_INSP_TIME);
			}
			//Столкновения
			{
				UINT IDs[3] =
				{ ID_START_COLLCHECK, ID_STOPONCOLLISION, ID_COLLSETTINGS };
				NSetToolBar SetToolBar(m_collisionsToolBar, IDs, 4);
				SetToolBar.SetButton(0, IDI_COLLISIONS_G0);
				SetToolBar.SetButton(1, IDI_COLLISIONS_STOP);
				SetToolBar.SetButton(2, IDI_COLLISIONS_PARAMETERS);
			}
			//Индикаторы
			{
				UINT IDs[8] =
				{ ID_STATETB0, ID_STATETB1, ID_STATETB2, ID_STATETB3, ID_STATETB4,
				ID_STATETB5, ID_STATETB6, ID_STATETB7 };
				NSetToolBar SetToolBar(m_indicatorsToolBar, IDs, 8);
				SetToolBar.SetButton(0, IDI_RAPIDOFF, IDI_RAPIDON);
				SetToolBar.SetButton(1, IDI_COOLON, IDI_M8OFF);
				SetToolBar.SetButton(2, IDI_SPEEDON, IDI_SPEEDOFF);
				SetToolBar.SetButton(3, IDI_PPON, IDI_PPOFF);
				SetToolBar.SetButton(4, IDI_G81ONN, IDI_G81OFF);
				SetToolBar.SetButton(5, IDI_G43ON, IDI_G43OFF);
				SetToolBar.SetButton(6, IDI_G41ON, IDI_G41OFF);
				SetToolBar.SetButton(7, IDI_TOOLON, IDI_TOOLOFF);
			}
		}
		if (NCMComData::GetType() == AT_WELD)
		{
			ChangeWinItem = 5;
			//Анализ
			{
				m_inspectToolBar.ShowPane(TRUE, FALSE, TRUE);
				UINT IDs[5] =
				{ ID_MEASURE, ID_CREATE_ENT, ID_VIEW_SECTION, ID_INSPECT_TOOLPATHSTATISTICS, ID_COLLSETTINGS };
				NSetToolBar SetToolBar(m_inspectToolBar, IDs, 5);
				SetToolBar.SetButton(0, IDI_MEASURE, IDI_MEASURE_H);
				SetToolBar.SetButton(1, IDI_SIZES, IDI_SIZES_H);
				SetToolBar.SetButton(2, IDI_SECTION, IDI_SECTION_H);
				SetToolBar.SetButton(3, IDI_INSP_TIME, IDI_INSP_TIME);
				SetToolBar.SetButton(4, IDI_INSP_COLLISION, IDI_INSP_COLLISION_H);
			}

			//Стандартный ToolBar
			{
				UINT IDs[3] =
				{ ID_FILE_NEW_WELD, ID_FILE_OPEN, ID_FILE_SAVE };
				NSetToolBar SetToolBar(m_wndToolBar, IDs, 3);
				SetToolBar.SetButton(0, IDI_TB_NEW);
				SetToolBar.SetButton(1, IDI_TB_OPEN);
				SetToolBar.SetButton(2, IDI_TB_SAVE);
			}
			//Меню виды  
			{
				UINT IDs[11] =
				{ ID_TB_ZOOM_W, ID_TB_ZOOM_SC, ID_TB_ZOOM_P, ID_TB_ZOOM_ALL,
				ID_TB_ZOOM_R, ID_TB_ZOOM_Z, ID_ROTANGLES, ID_TB_ROTATECENTERONOFF, ID_ROTCENTER, ID_TB_VIEW_PREV, ID_TB_VIEW_NEXT };
				NSetToolBar SetToolBar(m_viewToolBar, IDs, 11);
				SetToolBar.SetButton(0, IDI_TB_ZOOM, IDI_TB_ZOOM_H);
				SetToolBar.SetButton(1, IDI_TB_ZOOMD, IDI_TB_ZOOMD_H);
				SetToolBar.SetButton(2, IDI_TB_PAN, IDI_TB_PAN_H);
				SetToolBar.SetButton(3, IDI_TB_ZALL, IDI_TB_ZALL_H);
				SetToolBar.SetButton(4, IDI_TB_DROT, IDI_TB_DROT_H);
				SetToolBar.SetButton(5, IDI_TB_ZROT, IDI_TB_ZROT_H);
				SetToolBar.SetButton(6, IDI_VIEW_ANGLEROT, IDI_VIEW_ANGLEROT);
				SetToolBar.SetButton(7, IDI_TB_ROTATECENTERONOFF, IDI_TB_ROTATECENTERONOFF);
				SetToolBar.SetButton(8, IDI_VIEW_CENTRROT, IDI_VIEW_CENTRROT);
				SetToolBar.SetButton(9, IDI_TB_LVIEW, IDI_TB_LVIEW_H);
				SetToolBar.SetButton(10, IDI_TB_NVIEW, IDI_TB_NVIEW_H);

			}
			//Режимы симуляции
			{
				UINT IDs[3] =
				{ ID_SMOTHNESS_MODE_F, ID_SMOTHNESS_MODE,
				ID_RESULT_MODE };
				NSetToolBar SetToolBar(m_modeToolBar, IDs, 3);
				SetToolBar.SetButton(0, IDI_TB_SMOTHNESSMODE, IDI_TB_SMOTHNESSMODE_H);
				SetToolBar.SetButton(1, IDI_TB_SMOTH, IDI_TB_SMOTH_H);
				SetToolBar.SetButton(2, IDI_TB_RESULT, IDI_TB_RESULT_H);
			}
			//Навигация 
			{
				UINT IDs[6] =
				{ ID_RUN_TO_BREAKPOINT, ID_BREAKPOINT_BACK, ID_BREAKPOINT_FORWARD,
				ID_ANIMATE_PAUSE, ID_STEP_STOP, ID_RESET };
				NSetToolBar SetToolBar(m_navigToolBar, IDs, 6);
				SetToolBar.SetButton(0, IDI_TB_RUN, IDI_TB_RUN_H);
				SetToolBar.SetButton(1, IDI_TB_FB, IDI_TB_FB_H);
				SetToolBar.SetButton(2, IDI_TB_FF, IDI_TB_FF_H);
				SetToolBar.SetButton(3, IDI_TB_PAUSE, IDI_TB_PAUSE_H);
				SetToolBar.SetButton(4, IDI_TB_STOP, IDI_TB_STOP_H);
				SetToolBar.SetButton(5, IDI_TB_RESET, IDI_TB_RESET_H);
			}

			{
				UINT IDs[4] = { 0, 0, 0, 0 };
				NSetToolBar SetToolBar(m_wndDlgBar.DlgBar, IDs, 4);
				m_wndDlgBar.DlgBar.SetSliderHeight(BSize - 4);
				SetToolBar.SetButton(&m_wndDlgBar.DlgBar.ButtonB, IDI_TB_BACK_N, IDI_TB_BACK_N_H);
				SetToolBar.SetButton(&m_wndDlgBar.DlgBar.ButtonF, IDI_TB_N, IDI_TB_N_H);
				SetToolBar.SetButton(&m_wndDlgBar.DlgBar.ButtonBackP, IDI_NB, IDI_NB_H);
				SetToolBar.SetButton(&m_wndDlgBar.DlgBar.ButtonForP, IDI_TB_N, IDI_TB_N_H);
				SetToolBar.SetButton(&m_wndDlgBar.DlgBar.ButtonBack, IDI_TB_PAUSE, IDI_TB_PAUSE_H);
				SetToolBar.SetButton(&m_wndDlgBar.DlgBar.ButtonFor, IDI_TB_PAUSE, IDI_TB_PAUSE_H);
				NSpeedPar::Extern.SetTrackFeed(false);
				ShowPauseDlg();
			}
			//Точки останова
			{
				UINT IDs[7] =
				{ ID_BP1, ID_BP2, ID_BP3, ID_BP4, ID_BP5, ID_M_BREAKPOINTADD_ON_OFF, ID_BREAKPOINT_TABLE };
				NSetToolBar SetToolBar(m_NBreakpointToolBar, IDs, 7);
				SetToolBar.SetButton(0, IDI_BP1);
				SetToolBar.SetButton(1, IDI_BP2);
				SetToolBar.SetButton(2, IDI_BP3);
				SetToolBar.SetButton(3, IDI_BP4);
				SetToolBar.SetButton(4, IDI_BP5);
				SetToolBar.SetButton(5, IDI_BP_ADD);
				SetToolBar.SetButton(6, IDI_BP_ALL);
			}
			//Индикаторы
			{
				UINT IDs[8] =
				{ ID_STATETB0, ID_STATETB1, ID_STATETB2, ID_STATETB3, ID_STATETB4,
				ID_STATETB5, ID_STATETB6, ID_STATETB7 };
				NSetToolBar SetToolBar(m_indicatorsToolBar, IDs, 8);
				SetToolBar.SetButton(0, IDI_RAPIDOFF, IDI_RAPIDON);
				SetToolBar.SetButton(1, IDI_COOLON, IDI_M8OFF);
				SetToolBar.SetButton(2, IDI_SPEEDON, IDI_SPEEDOFF);
				SetToolBar.SetButton(3, IDI_PPON, IDI_PPOFF);
				SetToolBar.SetButton(4, IDI_G81ONN, IDI_G81OFF);
				SetToolBar.SetButton(5, IDI_G43ON, IDI_G43OFF);
				SetToolBar.SetButton(6, IDI_G41ON, IDI_G41OFF);
				SetToolBar.SetButton(7, IDI_TOOLON, IDI_TOOLOFF);
			}
		}
		if (NCMComData::GetType() == AT_MILL)
		{
			ChangeWinItem = 6;
			//Стандартный ToolBar
			{
				UINT IDs[3] =
				{ ID_FILE_NEW_MILL, ID_FILE_OPEN, ID_FILE_SAVE };
				NSetToolBar SetToolBar(m_wndToolBar, IDs, 3);
				SetToolBar.SetButton(0, IDI_TB_NEW);
				SetToolBar.SetButton(1, IDI_TB_OPEN);
				SetToolBar.SetButton(2, IDI_TB_SAVE);
			}
			//Меню виды  
			{
				UINT IDs[13] =
				{ ID_TB_ZOOM_W, ID_TB_ZOOM_SC, ID_TB_ZOOM_P, ID_TB_ZOOM_ALL,
				ID_TB_ZOOM_R, ID_TB_ZOOM_Z, ID_ROTANGLES, ID_TB_ROTATECENTERONOFF, ID_ROTCENTER, ID_TB_VIEW_PREV, ID_TB_VIEW_NEXT, ID_TB_UNDO, ID_TB_REDO };
				NSetToolBar SetToolBar(m_viewToolBar, IDs, 13);
				SetToolBar.SetButton(0, IDI_TB_ZOOM, IDI_TB_ZOOM_H);
				SetToolBar.SetButton(1, IDI_TB_ZOOMD, IDI_TB_ZOOMD_H);
				SetToolBar.SetButton(2, IDI_TB_PAN, IDI_TB_PAN_H);
				SetToolBar.SetButton(3, IDI_TB_ZALL, IDI_TB_ZALL_H);
				SetToolBar.SetButton(4, IDI_TB_DROT, IDI_TB_DROT_H);
				SetToolBar.SetButton(5, IDI_TB_ZROT, IDI_TB_ZROT_H);
				SetToolBar.SetButton(6, IDI_VIEW_ANGLEROT, IDI_VIEW_ANGLEROT);
				SetToolBar.SetButton(7, IDI_TB_ROTATECENTERONOFF, IDI_TB_ROTATECENTERONOFF);
				SetToolBar.SetButton(8, IDI_VIEW_CENTRROT, IDI_VIEW_CENTRROT);
				SetToolBar.SetButton(9, IDI_TB_LVIEW, IDI_TB_LVIEW_H);
				SetToolBar.SetButton(10, IDI_TB_NVIEW, IDI_TB_NVIEW_H);
				SetToolBar.SetButton(11, IDI_TB_UNDOPOS, IDI_TB_UNDOPOS_H);
				SetToolBar.SetButton(12, IDI_TB_REDOPOS, IDI_TB_REDOPOS_H);
			}
			//Режимы симуляции
			{
				{
					UINT IDs[3] =
					{ ID_CUT_MODE, ID_DONTCUT_MODE, ID_FAST_CUT_MODE5X };
					NSetToolBar SetToolBar(m_modeToolBar, IDs, 3);
					SetToolBar.SetButton(0, IDI_TB_CUTMODE, IDI_TB_CUTMODE_H);
					SetToolBar.SetButton(1, IDI_TB_DONTCUTMODE, IDI_TB_DONTCUTMODE_H);
					SetToolBar.SetButton(2, IDI_TB_CUTMODE5X, IDI_TB_CUTMODE5X_H);
				}
				UINT IDs[5] =
				{ ID_CHANGE_CUT_MODE, ID_SEPARATOR, ID_SMOTHNESS_MODE_F, ID_SMOTHNESS_MODE,
				ID_RESULT_MODE };
				NSetToolBar SetToolBar(m_modeToolBar, IDs, 5);
				SetToolBar.SetButton(0, IDI_TB_WIRE, IDI_TB_WIRE_H);
				SetToolBar.SetButton(2, IDI_TB_SMOTHNESSMODE, IDI_TB_SMOTHNESSMODE_H);
				SetToolBar.SetButton(3, IDI_TB_SMOTH, IDI_TB_SMOTH_H);
				SetToolBar.SetButton(4, IDI_TB_RESULT, IDI_TB_RESULT_H);

				HMENU hMenu = m_wndMenuBar.GetHMenu();
				hMenu = GetSubMenu(hMenu, 2);
				hMenu = GetSubMenu(hMenu, 2);

				m_modeToolBar.ReplaceButton(ID_CHANGE_CUT_MODE, CMFCToolBarMenuButton(ID_CHANGE_CUT_MODE, hMenu, -1));
				SetModeImage();
			}

			//Навигация 
			{
				UINT IDs[8] =
				{ ID_STEP_HERE, ID_STOCK_INIT, ID_RUN_TO_BREAKPOINT, ID_BREAKPOINT_BACK, ID_BREAKPOINT_FORWARD,
				ID_ANIMATE_PAUSE, ID_STEP_STOP, ID_RESET };
				NSetToolBar SetToolBar(m_navigToolBar, IDs, 8);
				SetToolBar.SetButton(0, IDI_TB_HERE, IDI_TB_HERE_H);
				SetToolBar.SetButton(1, IDI_TB_INIT, IDI_TB_INIT_H);
				SetToolBar.SetButton(2, IDI_TB_RUN, IDI_TB_RUN_H);
				SetToolBar.SetButton(3, IDI_TB_FB, IDI_TB_FB_H);
				SetToolBar.SetButton(4, IDI_TB_FF, IDI_TB_FF_H);
				SetToolBar.SetButton(5, IDI_TB_PAUSE, IDI_TB_PAUSE_H);
				SetToolBar.SetButton(6, IDI_TB_STOP, IDI_TB_STOP_H);
				SetToolBar.SetButton(7, IDI_TB_RESET, IDI_TB_RESET_H);
			}

			{
				UINT IDs[4] = { 0, 0, 0, 0 };
				NSetToolBar SetToolBar(m_wndDlgBar.DlgBar, IDs, 4);
				m_wndDlgBar.DlgBar.SetSliderHeight(BSize - 4);
				SetToolBar.SetButton(&m_wndDlgBar.DlgBar.ButtonB, IDI_TB_BACK_N, IDI_TB_BACK_N_H);
				SetToolBar.SetButton(&m_wndDlgBar.DlgBar.ButtonF, IDI_TB_N, IDI_TB_N_H);
				SetToolBar.SetButton(&m_wndDlgBar.DlgBar.ButtonBackP, IDI_NB, IDI_NB_H);
				SetToolBar.SetButton(&m_wndDlgBar.DlgBar.ButtonForP, IDI_TB_N, IDI_TB_N_H);
				SetToolBar.SetButton(&m_wndDlgBar.DlgBar.ButtonBack, IDI_TB_PAUSE, IDI_TB_PAUSE_H);
				SetToolBar.SetButton(&m_wndDlgBar.DlgBar.ButtonFor, IDI_TB_PAUSE, IDI_TB_PAUSE_H);
				NSpeedPar::Extern.SetTrackFeed(false);
				ShowPauseDlg();
			}
			//Точки останова
			{
				UINT IDs[7] =
				{ ID_BP1, ID_BP2, ID_BP3, ID_BP4, ID_BP5, ID_M_BREAKPOINTADD_ON_OFF, ID_BREAKPOINT_TABLE };
				NSetToolBar SetToolBar(m_NBreakpointToolBar, IDs, 7);
				SetToolBar.SetButton(0, IDI_BP1);
				SetToolBar.SetButton(1, IDI_BP2);
				SetToolBar.SetButton(2, IDI_BP3);
				SetToolBar.SetButton(3, IDI_BP4);
				SetToolBar.SetButton(4, IDI_BP5);
				SetToolBar.SetButton(5, IDI_BP_ADD);
				SetToolBar.SetButton(6, IDI_BP_ALL);
			}
			//Анализ 
			{
			//				m_inspectToolBar.ShowPane(TRUE, FALSE, TRUE);
				UINT IDs[6] =
				{ ID_MEASURE_DISTANCE, ID_MEASURE, ID_CREATE_ENT, ID_VIEW_SECTION, ID_COMPARISON, ID_INSPECT_TOOLPATHSTATISTICS };
				NSetToolBar SetToolBar(m_inspectToolBar, IDs, 6);
				SetToolBar.SetButton(0, IDI_DISTANCE, IDI_DISTANCE_H);
				SetToolBar.SetButton(1, IDI_MEASURE, IDI_MEASURE_H);
				SetToolBar.SetButton(2, IDI_SIZES, IDI_SIZES_H);
				SetToolBar.SetButton(3, IDI_SECTION, IDI_SECTION_H);
				SetToolBar.SetButton(4, IDI_COMPARE, IDI_COMPARE);
				SetToolBar.SetButton(5, IDI_INSP_TIME, IDI_INSP_TIME);
			}
			//Индикаторы
			{
				UINT IDs[8] =
				{ ID_STATETB0, ID_STATETB1, ID_STATETB2, ID_STATETB3, ID_STATETB4,
				ID_STATETB5, ID_STATETB6, ID_STATETB7 };
				NSetToolBar SetToolBar(m_indicatorsToolBar, IDs, 8);
				SetToolBar.SetButton(0, IDI_RAPIDOFF, IDI_RAPIDON);
				SetToolBar.SetButton(1, IDI_COOLON, IDI_M8OFF);
				SetToolBar.SetButton(2, IDI_SPEEDON, IDI_SPEEDOFF);
				SetToolBar.SetButton(3, IDI_PPON, IDI_PPOFF);
				SetToolBar.SetButton(4, IDI_G81ONN, IDI_G81OFF);
				SetToolBar.SetButton(5, IDI_G43ON, IDI_G43OFF);
				SetToolBar.SetButton(6, IDI_G41ON, IDI_G41OFF);
				SetToolBar.SetButton(7, IDI_TOOLON, IDI_TOOLOFF);
			}

		}
		//Окно Станок Заготовка
		UINT IDs[1] =
		{ ID_CHANGE_WIN };
		NSetToolBar SetToolBar(m_windowToolBar, IDs, 1);
		SetToolBar.SetButton(0, IDI_TB_CUTMODE, IDI_TB_CUTMODE_H);

		HMENU hMenu = m_wndMenuBar.GetHMenu();

		hMenu = m_wndMenuBar.GetHMenu();
		hMenu = GetSubMenu(hMenu, ChangeWinItem);

		m_windowToolBar.ReplaceButton(ID_CHANGE_WIN, NMFCMenuWin(ID_CHANGE_WIN, hMenu, -1));
		SetChangeViewImage();
		m_windowToolBar.RecalcLayout();
	}

	RecalcLayout();
	RedrawWindow(nullptr, nullptr, RDW_FRAME | RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
}


//void CMainFrame::SaveLayout(void)
//{
////	theApp.WriteProfileInt(NCMVersion,"MainViewStyle", IsMainViewStock() ? OG_Stock : OG_Mach);
//}
//
//void CMainFrame::LoadLayout(void)
//{
//	//OGLViewStyle Style = (OGLViewStyle)theApp.GetProfileInt(NCMVersion, "MainViewStyle", OG_Stock);
//
//	//switch(Style)
//	//{
//	//case OG_Mach:
//	//	OnWinMach();
//	//	break;
//	//case OG_Stock:
//	//default:
//	//	OnWinStock();
//	//	break;
//	//}
//	//((NDummyView*)GetActiveView())->SetPane(pMainPane);
//}

void CMainFrame::SetMainPane(void)
{
	((NDummyView*)GetActiveView())->SetPane(pMainPane);
}

bool CMainFrame::SetMainLayout()
{
	OnWinStock();
	return true;
}

LRESULT CMainFrame::OnUnitInfo(WPARAM wParam, LPARAM lParam)
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());

	int Ret = pDoc->GetUnit()->AskStartState(4, pDoc->GetCurChannel()->GetAxes().ZeroPos);// Argument 4 must correspond to tabs order defined in NCUnitState::AskUser
	bool DoRedraw = false;
	if (Ret & ZPS_NEED_REGEN)
	{
		pDoc->RegeneratePrograms();
		pDoc->GetUnit()->Reset();
		pDoc->SetModifiedFlag();
		DoRedraw = true;
	}
	if (Ret != pDoc->GetCurChannel()->GetAxes().ZeroPos)
	{
		pDoc->GetCurChannel()->GetAxes().ZeroPos = Ret;
		DoRedraw = true;
	}
	if (DoRedraw)
		pDoc->UpdateAllViews(nullptr, N_OGL | N_STATE | N_LEFT_ATTR | N_PROGRAMCHANGED, nullptr);

	return 0;
}

LRESULT CMainFrame::OnCutStateChanged(WPARAM wParam, LPARAM lParam)
{
	int ChInd = (int)wParam;
	CNCMDoc* const pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	auto pRender = pDoc->GetChannel(ChInd)->GetDynamicCadr() == nullptr
		? pDoc->NGetStockList()->GetVisibleRendersList()
		: pDoc->GetChannel(ChInd)->GetDynamicCadr()->GetActiveRender();
	if (pRender == nullptr)
		pRender = pDoc->NGetStockList()->GetVisibleRendersList();// works for result mode
	NChannel* const pChannel = pDoc->GetChannel(ChInd);
	if (pChannel->IsNeedProbe())
	{
		pChannel->DoProbing(pRender);
	}
	else if (pChannel->IsNeedTouch())
	{
		pChannel->DoTouching(pRender);
	}
	return 0;
}

LRESULT CMainFrame::OnCuttingStopped(WPARAM wParam, LPARAM lParam)
{
	CWaitCursor w;
	int ChanInd = int(wParam);
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	pDoc->ProcCuttingStopped(ChanInd);
	return 0;
}

void CMainFrame::UpdateVolumeParams(void)
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	if(pDoc->IsThereAnyStock())
	{
		double volume = 0.0;
		CMapStringToNStock *list = pDoc->NGetStockList();
		for each (const auto & el in *list)
		{
			const CString& key = el.first;
			class NStock* pS = static_cast<class NStock*>(el.second);
			if (pS)
				volume += pS->GetVolume();
		}
		pDoc->GetAnalyseState().SetCurVolume(volume);
		((NStateAnalysePane *)GetAnalysePane())->SetAnalyseState(&pDoc->GetAnalyseState());
		((NStateAnalysePane *)GetAnalysePane())->OnUpdate(nullptr, N_ANALYSE, nullptr);
	}
	if(NCM_PROJECT.GetParticularExport())
	{
		ParticularExport *pPExport = NCM_PROJECT.GetParticularExport();
		pPExport->Write(
			((NStateParamsPane *)GetAnalysePane())->GetExportString()  +
			((NStateParamsPane *)GetUnitPane())->GetExportString());

	}
}


void CMainFrame::PlaceViewPanes()
{
	m_wndUnitPane.EnableDocking(CBRS_ALIGN_ANY);
	m_wndAnalysePane.EnableDocking(CBRS_ALIGN_ANY);
	m_wndMTEditPane.EnableDocking(CBRS_ALIGN_ANY);
	m_wndMTEditTreePane.EnableDocking(CBRS_ALIGN_ANY);
	m_wndGeomPane.EnableDocking(CBRS_ALIGN_ANY);
	m_wndMesurePane.EnableDocking(CBRS_ALIGN_ANY);
	m_wndRegsPane.EnableDocking(CBRS_ALIGN_ANY);
	m_wndTextView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndLeftView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndReportPane.EnableDocking(CBRS_ALIGN_ANY);
//	m_wndWSectPane.EnableDocking(CBRS_ALIGN_ANY);
	m_wndPSlidePane.EnableDocking(CBRS_ALIGN_ANY);
	m_wndPropPane.EnableDocking(CBRS_ALIGN_ANY);//propEditor

	EnableDocking(CBRS_ALIGN_ANY);


	DockPane(&m_wndUnitPane);
	DockPane(&m_wndTextView);

	m_wndLeftView.DockToWindow(&m_wndTextView, CBRS_ALIGN_TOP);
	m_wndUnitPane.DockToWindow(&m_wndTextView, CBRS_ALIGN_BOTTOM); 
	DockPane(&m_wndPSlidePane, AFX_IDW_DOCKBAR_BOTTOM);
	HDWP hdwp;
	m_wndPSlidePane.MovePane(CRect(0, 0, 0, GetSystemMetrics(SM_CYSMCAPTION)), TRUE, hdwp);
	m_wndPSlidePane.DockToWindow(pMainPane, CBRS_ALIGN_BOTTOM);

	DockPane(&m_wndPropPane);//propEditor
	m_wndPropPane.DockToWindow(pMainPane, CBRS_ALIGN_RIGHT);//propEditor

	CDockablePane* pTabbedBar = nullptr;
	m_wndGeomPane.AttachToTabWnd(&m_wndUnitPane, DM_SHOW, FALSE, &pTabbedBar);
	m_wndMesurePane.AttachToTabWnd(pTabbedBar, DM_SHOW, FALSE, &pTabbedBar);
	m_wndRegsPane.AttachToTabWnd(pTabbedBar, DM_SHOW, FALSE, &pTabbedBar);
	
	DockPane(&m_wndReportPane);
	DockPane(&m_wndAnalysePane);
	DockPane(&m_wndMTEditPane);
	DockPane(&m_wndMTEditTreePane);

//	m_wndWSectPane.AttachToTabWnd(&m_wndUnitPane, DM_SHOW, FALSE, &pTabbedBar);


	m_wndMTEditPane.ShowPane(FALSE, FALSE, FALSE);
	m_wndMTEditTreePane.ShowPane(FALSE, FALSE, FALSE);
	m_wndMTEditTreePane.DockToWindow(&m_wndMTEditPane, CBRS_ALIGN_TOP);

	CSettingsStoreSP regSP;
	CSettingsStore& reg = regSP.Create(FALSE, TRUE);

	if (!reg.Open(theApp.GetRegSectionPath()))
	{// Default settings
		m_wndReportPane.ShowPane(FALSE, FALSE, FALSE);
		m_wndAnalysePane.ShowPane(FALSE, FALSE, FALSE);
//		m_wndWSectPane.ShowPane(FALSE, FALSE, FALSE);		
		m_wndPropPane.ShowPane(FALSE, FALSE, FALSE);//propEditor
		m_wndPropPane.ToggleAutoHide();//propEditor 
		m_wndPropPane.Clear();//propEditor
	}
}
void CMainFrame::PlaceToolBars()
{
	if (!IsRibbonCreated())
	{
		m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
		m_modeToolBar.EnableDocking(CBRS_ALIGN_ANY);
		m_windowToolBar.EnableDocking(CBRS_ALIGN_BOTTOM | CBRS_ALIGN_TOP);
		m_navigToolBar.EnableDocking(CBRS_ALIGN_ANY);
		m_NBreakpointToolBar.EnableDocking(CBRS_ALIGN_ANY);
		m_collisionsToolBar.EnableDocking(CBRS_ALIGN_ANY);
		m_inspectToolBar.EnableDocking(CBRS_ALIGN_ANY);
		m_wndDlgBar.DlgBar.EnableDocking(CBRS_ALIGN_BOTTOM | CBRS_ALIGN_TOP);
		EnableDocking(CBRS_ALIGN_ANY);

		DockPane(&m_windowToolBar);
		DockPaneLeftOf(&m_NBreakpointToolBar, &m_windowToolBar);
		DockPaneLeftOf(&m_wndDlgBar.DlgBar, &m_NBreakpointToolBar);
		DockPaneLeftOf(&m_collisionsToolBar, &m_NBreakpointToolBar);
		if(NCMComData::GetType() == AT_WELD)
		{
			DockPaneLeftOf(&m_modeToolBar, &m_wndDlgBar.DlgBar);
		}
		else
		{
			DockPaneLeftOf(&m_inspectToolBar, &m_wndDlgBar.DlgBar);
			DockPaneLeftOf(&m_modeToolBar, &m_inspectToolBar);
			m_inspectToolBar.ShowPane(TRUE,FALSE,TRUE);
		}
		DockPaneLeftOf(&m_navigToolBar, &m_modeToolBar);
		DockPaneLeftOf(&m_wndToolBar, &m_navigToolBar);
	}

	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);

	m_viewToolBar.EnableDocking(CBRS_ALIGN_LEFT);
	m_indicatorsToolBar.EnableDocking(CBRS_ALIGN_LEFT);
	DockPane(&m_indicatorsToolBar);
	DockPaneLeftOf(&m_viewToolBar, &m_indicatorsToolBar);
	m_viewToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_indicatorsToolBar.EnableDocking(CBRS_ALIGN_ANY);
	NSpeedPar::Extern.SetTrackFeed(false);
}
void CMainFrame::OnViewDefault()
{
	theApp.SetRemoveState();
	AfxMessageBox(IDS_RELOAD, MB_OK);
}

BOOL CMainFrame::IsLargeIcons(void)
{
	return 	CMFCToolBar::IsLargeIcons();
}

void CMainFrame::OnUpdateWinMach(CCmdUI *pCmdUI)
{
	if(pMainPane != nullptr)
		pCmdUI->SetCheck(!pMainPane->GetType());
	pCmdUI->Enable(theApp.GetConfig() & CM_Machine);
}


int CMainFrame::GetPauseAnimStep()
{
	return PauseAnimStep;
}
int CMainFrame::StartStopPauseAnim(bool forward, int step)
{
	if(PauseAnimatePause)
	{
		PauseAnimatePause = false;
		PauseAnimateTimerID = SetTimer(T_PAUSE_ANIMATE, NSpeedPar::Extern.GetTimeStep(), nullptr);
		return 0;
	}
	if(IsPauseAnimActive())
	{
		KillTimer(PauseAnimateTimerID);
		PauseAnimateTimerID = 0;
		PauseAnimatePause = true;
		return 0;
	}
	PauseAnimStep = step;
	if(forward)
	{
		PauseAnimFor = true;
		PauseAnimBack = false;
	}
	else
	{
		PauseAnimFor = false;
		PauseAnimBack = true;
	}
	if(pPauseAnimate)
	{
	}
	else
	{
		CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
		pDoc->GetCurChannel()->PauseAnimWait();
		pPauseAnimate = new GPauseAnimate(
			pDoc->GetCurChannel()->GetAnimateDisp().RendersQueueBack, pDoc->GetCurChannel()->GetAnimateDisp().RendersQueue,
			(GAnimateCadr *)pDoc->GetCurChannel()->GetDynamicCadr());
		pDoc->GetCurChannel()->SetAnimateCadr(pPauseAnimate->GetCurAnimCadr());
		RedrawWindow();// To redraw StepStop and Reset 
	}
	pPauseAnimate->StartNewStep(forward, step);
	PauseAnimateTimerID = SetTimer(T_PAUSE_ANIMATE, NSpeedPar::Extern.GetTimeStep(), nullptr);
	
	return 0;
}
bool CMainFrame::IsPauseAnimActive()
{
	return PauseAnimBack || PauseAnimFor;
}
bool CMainFrame::GetPauseAnimFor()
{
	return PauseAnimFor;
}
bool CMainFrame::GetPauseAnimBack()
{
	return PauseAnimBack;
}

void CMainFrame::RemovePauseAnim(void)
{
	if(PauseAnimateTimerID != 0)
	{
		KillTimer(PauseAnimateTimerID);
		PauseAnimateTimerID = 0;
	}
	PauseAnimFor = false;
	PauseAnimBack = false;
	// Restore animate cadr
	if(pPauseAnimate)
	{
		CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
		pPauseAnimate->GoToStartPos();

		delete pPauseAnimate;
		pPauseAnimate = nullptr;
	}
	if(NSpeedPar::Extern.IsTrackFeed())
		ShowPauseFDlg();
	else
		ShowPauseDlg();
}
bool CMainFrame::IsPauseAnimatePause()
{
	return PauseAnimatePause;
}

void CMainFrame::OnCheckMi()
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	if(pDoc->CheckStones.IsActive())
		pDoc->CheckStones.DeActivate();
	else
		pDoc->CheckStones.Activate();
	pDoc->RegeneratePrograms();
	pDoc->UpdateAllViews(nullptr, N_LEFT | N_TEXT | N_OGL | N_PROGRAMCHANGED);
}

void CMainFrame::OnUpdateCheckMi(CCmdUI *pCmdUI)
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	pCmdUI->SetCheck(pDoc->CheckStones.IsActive());
}

void CMainFrame::OnTest()
{
	OnRunToBreakpoint();
	SetTimer(T_TEST, 2000, nullptr);
//	((NOGLPane *)GetMachPane())->OnVolumeTree();

	//WaitCuttingThread();
	//OnReset();
}

void CMainFrame::TestStep(void)
{
	if(IsCuttingInProgress())
		return;
	KillTimer(T_TEST);
	StepStop();
	OnReset();
	OnTest();
}

void CMainFrame::SerializeMatrices(CArchive &ar)
{
	pMainPane->SerializeMatrices(ar);
	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < 5.0))
	{
		for each (auto pPane in AddOGLPanes)
			pPane->SerializeMatrices(ar);
	}
}

bool CMainFrame::CheckCutting(void)
{
	if(IsCuttingInProgress())
	{
		AfxMessageBox(IDS_COMP_STOPCUT);
		return false;
	}
	return true;
}

bool CMainFrame::CheckHosting(void)
{
	if(NCM_PROJECT.GetGlobalState().GetHostMode())
	{
		CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
		AfxMessageBox(IDS_STOP_INOTHERWIND);
		return false;
	}
	return true;
}

void CMainFrame::OnStockState()
{
	if(IsCuttingInProgress())
		return;
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	if(!pDoc->IsThereAnyStock())
		return;
	if(NCMComData::GetType() != AT_MILL_TURN)
		return;
	if (pDoc->Measure.IsActivated())
		return;
	CString key;
	class NStock *pS = nullptr;
	BStockGrID AllStocks;
	AllStocks.Set(-1);
	pDoc->NGetStockList()->ApplyStocksState((pDoc->NGetStockList()->GetStocksState() != AT_TURN) ? AT_TURN : AT_MILL, std::vector< BStockGrID> (1, AllStocks));
	pDoc->UpdateAllViews(nullptr, N_OGL | N_LEFT);
}

void CMainFrame::ActivateFrame(int nCmdShow)
{
	//LoadTBFromIcons();

	CFrameWndEx::ActivateFrame(nCmdShow);
}

void CMainFrame::OnUpdateStockState(CCmdUI *pCmdUI)
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	if(!pDoc->IsThereAnyStock())
		return;
	if(NCMComData::GetType() != AT_MILL_TURN)
		return;
	
	pCmdUI->SetCheck(pDoc->NGetStockList()->GetStocksState() == AT_TURN);
}

LRESULT CMainFrame::OnUnique(WPARAM wParam, LPARAM lParam)
{
	return WM_APP_UNIQUE;
}

LRESULT CMainFrame::On3DxInput(WPARAM wParam, LPARAM lParam)
{
	T3DData *pData = (T3DData *)lParam;
	
	if(pData->Lt == 0. && pData->Ar == 0.)
		return 0;

	NOpenGLWnd *pOGLWnd = GetViewToHandle();
	if(pOGLWnd)
	{
		pOGLWnd->Proc3DxInput(pData);
		return 0;
	}

	return 0;
}

bool CMainFrame::Init3Dx(void)
{// 3DConnexion device 
	HMODULE m = LoadLibrary("ConnexInt.dll");
	if(!m)
		return false;
	FARPROC p = GetProcAddress(m, "?Init@T3DxInit@@SAHXZ");
	if(!p)
		return false;
	p();
	return true;
}
bool CMainFrame::UnInit3Dx(void)
{// 3DConnexion device 
	HMODULE m = GetModuleHandle("ConnexInt.dll");
	if(!m)
		return false;
	FARPROC p = GetProcAddress(m, "?UnInit@T3DxInit@@SAHXZ");
	if(!p)
		return false;
	p();
	return true;
}

void CMainFrame::PostNcDestroy()
{
	UnInit3Dx();
	BSpacePos::Clear();
	CFrameWndEx::PostNcDestroy();
}

NOpenGLWnd * CMainFrame::GetViewToHandle(void)
{
	if(pMainPane->IsPaneVisible())
		if(pMainPane->HasFocus())
			return pMainPane->GetOpenGLWnd();
	for each (auto pPane in AddOGLPanes)
	{
		if (pPane)
			if (pPane->IsPaneVisible())
				if (pPane->HasFocus())
					return pPane->GetOpenGLWnd();
	}
	return nullptr;
}

void CMainFrame::OnDispExecute()
{
	if(theApp.IsInAnimateMode())
		AnimateText = !AnimateText;
	else if(theApp.IsInResultMode())
		ReRenderText = !ReRenderText;
}

void CMainFrame::OnUpdateDispExecute(CCmdUI *pCmdUI)
{
	if(theApp.IsInAnimateMode())
	{
		pCmdUI->Enable(true);
		pCmdUI->SetCheck(AnimateText);
	}
	else if(theApp.IsInResultMode())
	{
		pCmdUI->Enable(true);
		pCmdUI->SetCheck(ReRenderText);
	}
	else 
	{
		pCmdUI->Enable(false);
		pCmdUI->SetCheck(false);
	}
}

void CMainFrame::OnUpdateStateTB0(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(flags & CU_FAST);
}

void CMainFrame::OnUpdateStateTB1(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(flags & CU_COOL);
}

void CMainFrame::OnUpdateStateTB2(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(flags & CU_SPMI);
}

void CMainFrame::OnUpdateStateTB3(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(flags & CU_SUBP);
}

void CMainFrame::OnUpdateStateTB4(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(flags & CU_CYCL);
}

void CMainFrame::OnUpdateStateTB5(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(flags & CU_LCOMP_BEG);
}

void CMainFrame::OnUpdateStateTB6(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(flags & CU_DCOM);
}

void CMainFrame::OnUpdateStateTB7(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(flags & CU_SPTU);
}


void CMainFrame::SetFlags(void)
{
	if(IsCuttingInProgress())
		return;
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	if(!pDoc)
		return;
	flags = pDoc->GetUnit()->GetState()->Geom.GetType().flags;
}

BOOL CMainFrame::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
//	ASSERT(FALSE);
	if(!pCopyDataStruct)
		return FALSE;
	m_wndLeftView.CloseSectProgDlg();


	if (pCopyDataStruct->dwData == RC_Connect)
	{
		if (pRemoteControl)
			return TRUE; // Connected already

		StepStop();
		SetRemote(pWnd);
		return TRUE;
	}
	if (pCopyDataStruct->dwData == RC_Disconnect)
	{
		if (!pRemoteControl)
			return FALSE;
		StepStop();
		ClearRemote();
		return TRUE;
	}
	if (pCopyDataStruct->dwData == RC_StopHost)
	{
		if (NCM_PROJECT.GetGlobalState().GetHostMode())
		{
			CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
			pDoc->SetModifiedFlag(FALSE);
			NCM_PROJECT.GetGlobalState().SetHostMode(FALSE);
		}
		OnClose();
		return TRUE;
	}
	if (pCopyDataStruct->dwData == RC_SubscribeProgChange)
	{
		if (pCopyDataStruct->cbData < sizeof(SubscribeStruct))
			return FALSE;
		SubscribeStruct SS = *static_cast<SubscribeStruct *>(pCopyDataStruct->lpData);
		NCM_PROJECT.GetGlobalState().SetSubStProgChange(SS);
		return TRUE;
	}
	if (pCopyDataStruct->dwData == RC_SubscribeToColl)
	{
		if (pCopyDataStruct->cbData < sizeof(SubscribeStruct))
			return FALSE;
		SubscribeStruct SS = *static_cast<SubscribeStruct *>(pCopyDataStruct->lpData);
		NCM_PROJECT.GetGlobalState().SetSubStColl(SS);
		return TRUE;
	}
	if (pCopyDataStruct->dwData == RC_UnsubscribeProgChange)
	{
		SubscribeStruct SS;
		SS.Wnd = nullptr;
		SS.Pass = 0;
		NCM_PROJECT.GetGlobalState().SetSubStProgChange(SS);
		return TRUE;
	}
	if (pCopyDataStruct->dwData == RC_SavePrograms)
	{
		if (IsCuttingInProgress())
			return FALSE;

		CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
		if (pCopyDataStruct->cbData == 0 || pCopyDataStruct->lpData == nullptr)
			pDoc->SaveAllProgramsText();
		else
		{
			CString str(static_cast<char *>(pCopyDataStruct->lpData), pCopyDataStruct->cbData);
			int Ind = str.Find(_T(";"));
			if (Ind > 0)
			{
				CString OldName = str.Left(Ind);
				str.Delete(0, Ind + 1);
				if (!pDoc->SaveProgText(OldName, str))
					return FALSE;
			}
			else
			{
				if (!pDoc->SaveProgText(str, CString()))
					return FALSE;
			}
		}
		return TRUE;
	}
	if (pCopyDataStruct->dwData == RC_ShowProgramText)
	{
		CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
		if (!pDoc->ShowProgrText(CString(static_cast<char *>(pCopyDataStruct->lpData), pCopyDataStruct->cbData)))
			return FALSE;
		return TRUE;
	}
	if (pCopyDataStruct->dwData == RC_SetProperty)
	{
		if (pCopyDataStruct->cbData < 4)
			return FALSE;
		if (IsCuttingInProgress())
			return FALSE;
		unsigned int PropNum;
		CString str(static_cast<char *>(pCopyDataStruct->lpData), pCopyDataStruct->cbData);
		CString PropNumS = str.Left(3);
		str.Delete(0, 3);
		sscanf(PropNumS, _T("%u"), &PropNum);
		CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
		switch (PropNum)
		{
		case 0: // Stocks->Properties->Tuning->Quality
		{
			int Val;
			sscanf(str, _T("%d"), &Val);
			GQuadRenderDX5X::SetWholeCellsNum(Val);
			OnReset();
			break;
		}
		case 1: // Stocks->Properties->Color
		{
			int Val;
			if (sscanf(str, _T("%u"), &Val) != 1)
				return FALSE;
			NColor Col;
			Col.SetRGB(COLORREF(Val));
			pDoc->NGetStockList()->SetColor(Col);
			break;
		}
		case 2: // Stocks->Properties->Visibility
		{
			int Val;
			if (sscanf(str, _T("%d"), &Val) != 1)
				return FALSE;
			pDoc->NGetStockList()->SetVisible(Val != 0);
			if (pDoc->NGetStockList()->GetSize() == 1)
				pDoc->NGetStockList()->GetStock(0)->SetVisible(Val != 0);
			break;
		}
		case 3: // Stocks->Properties->Edges
		{
			int Val;
			sscanf(str, _T("%d"), &Val);
			if (NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::BSP)
				pDoc->NGetStockList()->SetSharpEdges(Val != 0);
			else
				pDoc->NGetStockList()->SetSharpEdgesDX(Val != 0);
			pDoc->RegenerateStocks();
			break;
		}
		case 4: // Stocks->Properties->Edges thickness
		{
			double Val;
			if (sscanf(str, _T("%lf"), &Val) != 1)
				return FALSE;
			GQuadRender::SetEdgesWidth(Val);
			pDoc->NGetStockList()->SetEdgesWidth(Val);
			break;
		}
		case 5: // Programs->Properties->To show
		{
			int Val;
			if (sscanf(str, _T("%d"), &Val) != 1)
				return FALSE;
			pDoc->NGetProgramList()->PartialDraw = static_cast<enum TrajPart>(Val);
			break;
		}
		case 6: // Tools->Properties->Visibility
		{
			int Val;
			if (sscanf(str, _T("%d"), &Val) != 1)
				return FALSE;
			NTool::SetShapeVisibleTool(Val != 0);
			break;
		}
		case 7: // Program->Properties->Thickness
		{
			double Val;
			CString ProgName;
			if (sscanf(str, _T("%lf %s"), &Val, ProgName.GetBuffer(1024)) != 2)
				return FALSE;
			ProgName.ReleaseBuffer();

			auto pProg = pDoc->GetProgram(ProgName);
			if (pProg == nullptr)
				return FALSE;
			pProg->SetThickness(Val);
			break;
		}
		case 8: // Tool->Properties->Toolpath
		{
			int Val;
			NToolID ToolID;
			if (sscanf(str, _T("%d %s"), &Val, ToolID.GetBuffer(1024)) != 2)
				return FALSE;
			ToolID.ReleaseBuffer();
			auto pToolCom = pDoc->GetTool(ToolID);
			if (pToolCom == nullptr)
				return FALSE;
			pToolCom->SetVisible(Val != 0);
			break;
		}
		case 9: // Program->Properties->Visibility
		{
			int Val;
			CString ProgName;
			if (sscanf(str, _T("%d %s"), &Val, ProgName.GetBuffer(1024)) != 2)
				return FALSE;
			ProgName.ReleaseBuffer();

			auto pProg = pDoc->GetProgram(ProgName);
			if (pProg == nullptr)
				return FALSE;
			pProg->SetVisible(Val != 0);
			break;
		}
		case 10: // Tools->Properties->Check
		{
			int Val;
			sscanf(str, _T("%d"), &Val);
			CollisWithStock Buf = CWS_OFF;
			switch (Val)
			{
			case 0:
				Buf = CWS_OFF;
				break;
			case 1:
				Buf = CWS_CUT;
				break;
			case 2:
				Buf = CWS_ALL;
				break;
			}
			pDoc->GetCollManager().SetToolStockMode(Buf);
			break;
		}
		case 11: // Tools -> Ignore Up
		{
			int Val;
			if (sscanf(str, _T("%d"), &Val) != 1)
				return FALSE;
			NTool::SetIgnoreUpToolStockColl(Val != 0);
			break;
		}
		case 12: // Tools -> Offset XY
		{
			double Val;
			if (sscanf(str, _T("%lf"), &Val) != 1)
				return FALSE;
			NTool::SetXYOffset(Val);
			break;
		}
		case 13: // Tools -> Offset Z
		{
			double Val;
			if (sscanf(str, _T("%lf"), &Val) != 1)
				return FALSE;
			NTool::SetZOffset(Val);
			break;
		}
		default:
			return FALSE;
		}
		pDoc->UpdateAllViews(nullptr, N_OGL | N_LEFT_ATTR, nullptr);
		pDoc->SetModifiedFlag();
		return TRUE;
	}
	if (pCopyDataStruct->dwData == RC_PressButton)
	{
		SendMessage(WM_COMMAND, MAKEWPARAM(*static_cast<UINT *>(pCopyDataStruct->lpData), 0), 0);
		return TRUE;
	}

	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	if (pCopyDataStruct->dwData == RC_JobFile)
	{
		if (pCopyDataStruct->cbData <= 0)
			return FALSE;
		theApp.OpenDocumentFile(CString(static_cast<char *>(pCopyDataStruct->lpData), pCopyDataStruct->cbData));
		pDoc->SetModifiedFlag();
		pDoc->UpdateAllViews(nullptr, N_OGL | N_STATE | N_LEFT | N_UNIT_CHANGED | N_PROGRAMCHANGED);
		return TRUE;
	}
	if (pCopyDataStruct->dwData == RC_SetView)
	{
		if (pCopyDataStruct->cbData <= 0)
			return FALSE;
		CString str(static_cast<char *>(pCopyDataStruct->lpData), pCopyDataStruct->cbData);
		double eyex = 0, eyey, eyez, upx, upy, upz;
		if (sscanf(str, _T("%lf %lf %lf %lf %lf %lf"), &eyex, &eyey, &eyez, &upx, &upy, &upz) != 6)
			return FALSE;
		NOpenGLWnd *pOGLWnd = GetViewToHandle();
		if (!pOGLWnd)
			return FALSE;
		pOGLWnd->DoDefView(eyex, eyey, eyez, upx, upy, upz);
		return TRUE;
	}
	if (pCopyDataStruct->dwData == RC_ShowMainMenu)
	{
		if (pCopyDataStruct->cbData <= 0)
			return FALSE;
		CString str(static_cast<char *>(pCopyDataStruct->lpData), pCopyDataStruct->cbData);
		int Val = 0;
		if (sscanf(str, _T("%d"), &Val) != 1)
			return FALSE;
		ShowMenu(Val == 1);
		return TRUE;
	}
	if (pCopyDataStruct->dwData == RC_SetFixedCenter)
	{
		if (pCopyDataStruct->cbData <= 0)
			return FALSE;
		CString str(static_cast<char *>(pCopyDataStruct->lpData), pCopyDataStruct->cbData);
		int Val = 0;
		if (sscanf(str, _T("%d"), &Val) != 1)
			return FALSE;
		SetFixedRotCenter(Val == 1);
		return TRUE;
	}
	if (pCopyDataStruct->dwData == RC_Reset)
	{
		if (!pRemoteControl)
			return FALSE;
		StepStop();
		pDoc->DeleteProg("Remote");
		pDoc->GetUnit()->SetStartState(pRemoteControl->GetBufState());
		pDoc->GetUnit()->Reset();
		pDoc->RegeneratePrograms();
		pDoc->SetModifiedFlag();
		pDoc->UpdateAllViews(nullptr, N_OGL | N_STATE | N_LEFT | N_UNIT_CHANGED | N_PROGRAMCHANGED);
	}
	if(pCopyDataStruct->dwData == RC_Stop)
	{
		StepStop();
		pDoc->UpdateAllViews(nullptr, N_OGL | N_STATE | N_LEFT);
	}

	if(pCopyDataStruct->cbData <= 0)
		return TRUE;

	const int BUF_SIZE = 1024;
	char Command[BUF_SIZE];
	int CommandSize = min(pCopyDataStruct->cbData, BUF_SIZE - 1);
	memcpy(Command, pCopyDataStruct->lpData, CommandSize);
	Command[CommandSize] = '\0';

	if(IsCuttingInProgress())
		return FALSE;

	if(pCopyDataStruct->dwData == RC_MakePlaceString)
	{
		if(!pRemoteControl)
			return FALSE;
		CString PlaceString = pDoc->GetUnit()->MakePlaceString(Command);
		if(PlaceString.IsEmpty())
			return FALSE;

		theApp.WriteProfileString(NCMVersion, _T("PlaceString"), PlaceString);

		return TRUE;
	}

	if(pCopyDataStruct->dwData == RC_WaitPoint)
	{
		if(!pRemoteControl)
			return FALSE;
		pRemoteControl->SetWaitPoint(!pRemoteControl->IsWaitingPoint());
		pRemoteControl->SaveCommand(Command);
		return TRUE;
	}

	//pDoc->GetUnit()->RunOneCadrInStepMode(Command);
	CString text(Command);
	NProgram* pProg = pDoc->GetUnit()->MakeMainInitProg(text);// Create global init prog

	if (pProg)
	{
		pProg->InvertVisible();
		pDoc->NAddMainProgram(_T("Remote"), pProg);
		pDoc->GetUnit()->SetCurProg(_T("Remote"));
		pDoc->GetUnit()->RunProgram(pProg);
		NCUnitState Buf = *pDoc->GetUnit()->GetState();
		if (pDoc->GetUnit()->GetState()->ChangedCoord.byte == 0 && pDoc->GetUnit()->GetState()->ElectrSF != 0.)
		{// Make movement to take into account penetration depth
			pDoc->GetUnit()->GetStartState().CurX = 0.0001;
			pDoc->GetUnit()->GetStartState().MashX -= pDoc->GetUnit()->GetStartState().CurX;
			pDoc->GetUnit()->GetStartState().MashcX -= pDoc->GetUnit()->GetStartState().CurX;
		}
		pDoc->GetUnit()->Reset();
		pDoc->RegeneratePrograms();
		OnRunToBreakpoint();
		pDoc->GetUnit()->GetStartState() = Buf;
	}
	pDoc->GetUnit()->GetStartState().EndProgram = false;
	pDoc->GetUnit()->GetStartState().SetCurCadr(0);
	pDoc->GetUnit()->GetStartState().ClearCurHCadr();
	pDoc->GetUnit()->GetStartState().ClearCurHCadrAbs();
	pDoc->GetUnit()->GetStartState().SetCurProg(_T("Remote"));
	pDoc->GetUnit()->GetState()->ProgramStarted = false;
	pDoc->GetUnit()->GetState()->CadrID.Prog = 0;

	pDoc->UpdateAllViews(nullptr, N_OGL | N_LEFT | N_STATE | N_PROGRAMCHANGED);

	return TRUE;
}
void CMainFrame::RC_SendPoint(const BPoint &P, const BPoint &N)
{
	if(!pRemoteControl)
		return;

	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	CString PointString = pDoc->GetUnit()->MakePointString(P, N, pRemoteControl->GetCommand());

	if(PointString.IsEmpty())
		return;

	theApp.WriteProfileString(NCMVersion, _T("PlaceString"), PointString);
	pRemoteControl->SendPick();

}

void CMainFrame::OnViewCustomize()
{
	//------------------------------------
	// Create a customize toolbars dialog:
	//------------------------------------
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
	//CSize b = m_wndToolBar.GetButtonSize();
	//pDlgCust->EnableUserDefinedToolbars();

	//CSliderButton btnSlider (ID_SLIDER);
	//btnSlider.SetRange (0, 100);

	//pDlgCust->AddButton (_T("Edit"), btnSlider);
	pDlgCust->Create ();
}

void CMainFrame::OnStatusCadr()
{// Added to prevent disabling of the correspondent pane in status bar
}
void CMainFrame::OnStatusCoord()
{// Added to prevent disabling of the correspondent pane in status bar
}
void CMainFrame::OnRibbonSlider()
{
	m_wndDlgBar.OnSlider();
}
void CMainFrame::OnStepNum()
{// Added to prevent disabling of the correspondent pane in status bar
}
void CMainFrame::OnPauseNum()
{// Added to prevent disabling of the correspondent pane in status bar
}
void CMainFrame::OnPauseBack()
{
	StartStopPauseAnim(false, m_wndDlgBar.GetPauseNum());
}
void CMainFrame::OnPauseForward()
{
	StartStopPauseAnim(true, m_wndDlgBar.GetPauseNum());
}
void CMainFrame::OnChangeLook(BOOL bOneNoteTabs, BOOL bMDITabColors, BOOL bIsVSDotNetLook, BOOL bDockTabColors, BOOL bMDITabsVS2005Look, BOOL bActiveTabCloseButton)
{
	CMDITabInfo mdiTabParams;
	mdiTabParams.m_bTabCustomTooltips = TRUE;

	if (bMDITabsVS2005Look)
	{
		mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_VS2005;
		mdiTabParams.m_bDocumentMenu = TRUE;
	}
	else if (bOneNoteTabs)
	{
		mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE;
		mdiTabParams.m_bAutoColor = bMDITabColors;
	}


	CTabbedPane::EnableTabAutoColor(bDockTabColors);

	// Reload toolbar images:
	LoadTBFromIcons();

	CDockingManager* pDockManager = GetDockingManager();
	ASSERT_VALID(pDockManager);

	pDockManager->AdjustPaneFrames();

	RecalcLayout();
	RedrawWindow(nullptr, nullptr, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if(wParam == ID_ERRORS_CAPTION_BUTTON || wParam == IDS_INDIC_COLL)
	{
		m_wndReportPane.ShowPane(TRUE, FALSE, TRUE);
		m_wndReportPane.Slide(TRUE, TRUE);
		m_wndReportPane.SetFocus();
	}
	if(HIWORD(wParam) == 1)
	{
		if(m_wndTextView.HasFocus())
			if(m_wndTextView.SendMessage(WM_COMMAND, LOWORD(wParam)))
				return TRUE;
		if(m_wndReportPane.HasFocus())
			if(m_wndReportPane.GetCtrl().SendMessage(WM_COMMAND, LOWORD(wParam)))
				return TRUE;

	}
	return CFrameWndEx::OnCommand(wParam, lParam);
}

void CMainFrame::OnMeasure()
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	if(pDoc->Measure.IsActivated())
		return;
	if(!theApp.IsInCuttingMode())
	{
		AfxMessageBox(IDS_COMP_NOCUTTING);
		return;
	}
	if(pDoc->Dimensions.IsActivated())
	{
		pDoc->Dimensions.DeActivate();
		if(pDimEntCrSheet)
			pDimEntCrSheet->ShowWindow(SW_HIDE);
	}

	InitMeasure();

	pDoc->UpdateAllViews(nullptr, N_STATE_MEASURE | N_OGL);
}

void CMainFrame::InitMeasure()
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	class NStock *pStock = ((CMainFrame *)AfxGetMainWnd())->GetActiveStock();
	if(!pStock)
		pStock = pDoc->GetFirstVisStock();
	if(!pStock)
	{
		AfxMessageBox(IDS_COMP_NOSTOCK);
		return;
	}

	switch(NCM_PROJECT.GetGlobalState().GetModelMode())
	{
	case NGlobalState::BSP:
		if(NCMComData::GetType() == AT_TURN)
			pStock->CrBody4Mill();// Create mill stock for measuring
		else
		{
			BStockGrID AllStocks;
			AllStocks.Set(-1);
			pDoc->NGetStockList()->ApplyStocksState(AT_MILL, std::vector< BStockGrID>(1, AllStocks));
		}
		break;
	case NGlobalState::DX:
		break;
	}
	pDoc->Measure.Activate(NCM_PROJECT.GetGlobalState().GetModelMode(), this, pStock);
}

void CMainFrame::OnUpdateMeasure(CCmdUI *pCmdUI)
{
	if(!(theApp.GetConfig() & CM_Measure))
	{
		pCmdUI->Enable(0);
		return;
	}
	pCmdUI->Enable(!IsCuttingInProgress());
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	pCmdUI->SetCheck(pDoc->Measure.IsActivated());
}
void CMainFrame::OnPaStockSecView()
{
	if (!CheckCutting())
		return;
	NStockTurnSecDlg Dlg(this);
	Dlg.DoModal();
}
void CMainFrame::OnPaMachEditEnd()
{
	theApp.StopEditMachMode();
}
void CMainFrame::AppTypeChanged(NCMApplicationType NewType)
{
	LoadTBFromIcons();
	//if(NewType == AT_WELD)
	//{
	//	m_wndWSectPane.ShowPane(TRUE, FALSE, FALSE);
	//	m_wndWSectPane.SetShowOnPaneMenu(TRUE);
	//}
	//else
	//{
	//	m_wndWSectPane.ShowPane(FALSE, FALSE, FALSE);
	//	m_wndWSectPane.SetShowOnPaneMenu(FALSE);
	//}
	m_wndLeftView.InitialUpdate();
}

void CMainFrame::OnInspectToolpathstatistics()
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	ASSERT(pDoc);
	NCMData Data;
	pDoc->FillExternData(&Data);
	CString Text;
	CDHtmlExploreDlg DataDialog;
	Data.GetText(	IDS_STAT_ALL, DataDialog.strAll,
					IDS_STAT_PROGS, DataDialog.strProgs,
					IDS_STAT_TOOLS, DataDialog.strTools);
	DataDialog.DoModal();
}
void CMainFrame::SetModeImage()
{
	if (IsRibbonCreated())
		return;

	CMFCToolBarMenuButton* pButton =  DYNAMIC_DOWNCAST(CMFCToolBarMenuButton, m_modeToolBar.GetButton(0));

	if (pButton != nullptr)
	{
		pButton->m_bText = FALSE;
		pButton->m_bImage = TRUE;
		UINT uiCmd = ID_DONTCUT_MODE;
		if(theApp.IsInCuttingMode())
		{
			switch(theApp.GetGlobalState().GetModelMode())
			{
			case NGlobalState::BSP:
				uiCmd = ID_CUT_MODE;
				break;
			case NGlobalState::DX:
				uiCmd = ID_FAST_CUT_MODE5X;
				break;
			}
		}
		pButton->SetImage(GetCmdMgr()->GetCmdImage(uiCmd));
		pButton->SetMessageWnd(this);
		m_modeToolBar.RedrawWindow();
	}
}

void CMainFrame::ChangeWin()
{
	if (IsMainViewStock())
		OnWinMach();
	else
		OnWinStock();
}

void CMainFrame::SetChangeViewImage()
{
	if (IsRibbonCreated())
		return;

	CMFCToolBarMenuButton* pButton = DYNAMIC_DOWNCAST(CMFCToolBarMenuButton, m_windowToolBar.GetButton(0));

	if (pButton != nullptr)
	{
		pButton->m_bText = TRUE;
		pButton->m_bImage = FALSE;

		UINT uiCmd = 0;
		if (IsMainViewStock())
			uiCmd = ID_WIN_STOCK;
		else
			uiCmd = ID_WIN_MACH;
		HMENU hMenu = m_wndMenuBar.GetHMenu();
		if (::IsMenu(hMenu))
		{
			char Txt[256];
			GetMenuString(hMenu, uiCmd, Txt, 256, MF_BYCOMMAND);
			pButton->m_strText = Txt;
		}
		pButton->SetMessageWnd(this);
		m_windowToolBar.RedrawWindow();
	}
}

void CMainFrame::OnCollsettings()
{
	if (!CheckCutting())
		return;

	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());

	if(pDoc->GetCollManager().AskSettings())
		pDoc->UpdateAllViews(NULL, N_COLLLOG);
}


void CMainFrame::SetCollisionIcon(bool Enable, bool Found)
{
	static bool IconEnabled = false;
	static bool CollisionFound = false;

	if(Enable == IconEnabled && Found == CollisionFound)
		return;
	int COLLISION_PANE_NUM = m_wndStatusBar.CommandToIndex(IDS_INDIC_COLL);
	m_wndStatusBar.SetPaneWidth(COLLISION_PANE_NUM, COLLISION_PANE_WIDTH);
	CBitmap Bmp;
	CString ToolTip;
	if(Enable)
	{
		ToolTip.LoadString(Found ? IDS_STATUSPANE1_FOUND : IDS_STATUSPANE1_NOTFOUND);
		Bmp.LoadBitmap(Found ? IDB_COLL_FOUND : IDB_COLL_NOTFOUND);
	}
	else
	{
		ToolTip.LoadString(IDS_STATUSPANE1_OFF);
		Bmp.LoadBitmap(IDB_COLL_OFF);
	}
	m_wndStatusBar.SetPaneIcon(COLLISION_PANE_NUM, Bmp, RGB(255,255,255));
	m_wndStatusBar.SetTipText(COLLISION_PANE_NUM, ToolTip);
	IconEnabled = Enable;
	CollisionFound = Found;
}

void CMainFrame::SetCollisionAnim(bool Enable, bool Found)
{
	static bool AnimationInProgress = false;
	static bool CollisionFound = false;
	if(Enable == AnimationInProgress && Found == CollisionFound)
		return;
	if(Enable)
	{
		CImageList AnimIL;
		AnimIL.Create(IDB_COLL_ANIM, 20, 0, 0);
		m_wndStatusBar.SetPaneAnimation(COLLISION_ANIM_NUM, AnimIL, 100);
	}
	else
	{
		m_wndStatusBar.SetPaneAnimation(COLLISION_ANIM_NUM, nullptr);

	}
	AnimationInProgress = Enable;
	CollisionFound = Found;
}

void CMainFrame::OnUpdateCollIcon(CCmdUI *pCmdUI)
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	if(!pDoc)
		return;
	bool EnableIcon = false;
	bool FoundIcon = false;
	if(pDoc->GetCollManager().IsBackType())
	{
		EnableIcon = true;
		FoundIcon = pDoc->GetCollManager().IsCollisionFound();
		if(pDoc->GetCollManager().IsBackCheckInProgress())
			SetCollisionAnim(true, pDoc->GetCollManager().IsCollisionFound());
		else
			SetCollisionAnim(false, false);
	}
	else
	{
		SetCollisionAnim(false, false);
		if(pDoc->GetCollManager().IsForeType() || pDoc->GetCollManager().IsToolStockActive())
		{
			EnableIcon = true;
			FoundIcon = pDoc->GetCollManager().IsCollisionFound();
		}
	}
	if(pDoc->GetCollManager().IsToolStockActive())
		EnableIcon = true;
	//if(m_wndReportPane.GetItemCount() > 0)
	//	FoundIcon = true;

	SetCollisionIcon(EnableIcon, FoundIcon);
}
LRESULT CMainFrame::OnGridEvalexp(WPARAM Aborted, LPARAM lParam)
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	if (!pDoc)
		return 0;
	CString &SRes = *((CString *)lParam);
	auto *pUnit = pDoc->GetUnit();
	double Res;
	auto flag = pUnit->GetDValue4Dial(SRes, &Res);
	if (flag)
		SRes.Format(_T("%#.4lf"), Res);
	else
		SRes = _T("error");
	return 0;
}
LRESULT CMainFrame::OnWideEvalexp(WPARAM Aborted, LPARAM lParam)
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	if (!pDoc)
		return 0;
	CString &SRes = *((CString *)lParam);
	auto *pUnit = pDoc->GetUnit();
	double Res;
	auto flag = pUnit->GetDValue4Wide(SRes, &Res);
	if (flag)
		SRes.Format(_T("%#.4lf"), Res);
	else
		SRes = _T("error");
	return 0;
}
LRESULT CMainFrame::OnCollcheckStopped(WPARAM Aborted, LPARAM lParam)
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	if(!pDoc)
		return 0;
	pDoc->GetCollManager().CollcheckStopped(Aborted != 0);
	return 0;
}
LRESULT CMainFrame::OnCollcheckAddlog(WPARAM wParam, LPARAM lParam)
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	if(!pDoc)
		return 0;
	switch (wParam)// 0 - clear report pane, 1 - add element, 2 - add whole log
	{
	case 0:
		((NReportPane*)GetReportPane())->Clear(true, false);
		((NTextView*)GetTextView())->ClearCollisionMarkers();
		break;
	case 1:
	{
		((NReportPane*)GetReportPane())->AddCollItem((NCollLogElem*)lParam);
		((NTextView*)GetTextView())->SetCollision(*(NCollLogElem*)lParam);
		delete (NCollLogElem*)lParam;
		break;
	}
	case 2:
	{
		NCollisionLog* pLog = pDoc->GetCollManager().GetActiveLog();
		if (pLog != nullptr)
		{
			((NReportPane*)GetReportPane())->AddCollLog(*pLog, 0);
			((NTextView*)GetTextView())->SetCollisionArr();
		}
		break;
	}
	case 3:
	{
		NCollisionLog* pLog = pDoc->GetCollManager().GetForeLog();
		if (pLog != nullptr)
		{
			const NCollLogElem& Elem = *(NCollLogElem*)lParam;
			const auto CadrAbs = static_cast<cadrID>(Elem.StartPTPoint);
			int pos = pLog->FindElem(CadrAbs);
			if (pLog->AddElem(pos, Elem))
			{
				((NReportPane*)GetReportPane())->AddCollItem((NCollLogElem*)lParam);
				((NTextView*)GetTextView())->SetCollision(Elem);
			}
			delete (NCollLogElem*)lParam;
		}
		break;
	}
	}
	pDoc->UpdateAllViews(nullptr, N_COLLLOG);
	return 0;
}
bool CMainFrame::IsRunAnimateActive()
{
	return RunAnimateTimerID != 0;
}

void CMainFrame::OnViewSection()
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	NStockSection *pSection = pDoc->GetStockSection();
	if(!pSection)
	{
		pSection = pDoc->CreateStockSection(this);
	}

	if(pSection->IsActive())
	{
		return;
	}
	else
	{
		if(!pDoc->IsThereAnyStock())
		{
			AfxMessageBox(IDS_COMP_NOSTOCK);
			return;
		}
		pSection->Activate(pDoc->GetStocksGabar());
	}
}

LRESULT CMainFrame::OnRegenerate(WPARAM wParam, LPARAM lParam)
{
	CWaitCursor w;
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	if (lParam & OG_PROGRAMS)
		pDoc->RegeneratePrograms();
	if (lParam & OG_STOCKS)
	{
		pDoc->RegenerateStocks();
		ResetMeasureStock();
	}
	if (lParam & OG_PARTS)
		pDoc->RegenerateParts();
	if (lParam & OG_FIXTURES)
		pDoc->RegenerateFixtures();
	if (lParam & OG_TOOLS)
		pDoc->RegenerateTools();
	if (lParam & OG_OCTREE)
		pDoc->GetRayCastManager().Initialize(pDoc);
	if (lParam & OG_NO_GROUP)
		OnReset();
	pDoc->GetSnaps().SetRestartFlag();
	pDoc->UpdateAllViews(nullptr, wParam);
	return 0;
}

LRESULT CMainFrame::OnStockVisibleChange(WPARAM wParam, LPARAM lParam)
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	pDoc->NGetStockList()->ReCreateLists();
//	pDoc->UpdateAllViews(nullptr, N_OGL | N_LEFT_ATTR, nullptr);
	return 0;
}

LRESULT CMainFrame::OnUpdatePanes(WPARAM wParam, LPARAM lParam)
{
	UpdateOGLPanes(nullptr, wParam, nullptr);
	m_wndTextView.OnUpdate(nullptr, wParam, nullptr);
	m_wndLeftView.OnUpdate(nullptr, wParam, nullptr);
	m_wndUnitPane.OnUpdate(nullptr, wParam, nullptr);
	m_wndAnalysePane.OnUpdate(nullptr, wParam, nullptr);
	m_wndGeomPane.OnUpdate(nullptr, wParam, nullptr);
	m_wndMesurePane.OnUpdate(nullptr, wParam, nullptr);
	m_wndRegsPane.OnUpdate(nullptr, wParam, nullptr);
	m_wndReportPane.OnUpdate(nullptr, wParam, nullptr);
//	m_wndWSectPane.OnUpdate(nullptr, wParam, nullptr);
	m_wndPSlidePane.OnUpdate(nullptr, wParam, nullptr);
	GetActiveDocument()->UpdateAllViews(nullptr, wParam);
	return 0;
}

void CMainFrame::OnRotatecenteronoff()
{
	SetFixedRotCenter(!GetFixedRotCenter());
}

void CMainFrame::OnTbUndo()
{
	const auto pLeft = static_cast<CLeftView*>(GetLeftPane());
	if (pLeft != nullptr)
	{
		if (pLeft->IsPosDlgActive())
		{
			pLeft->GetPosDlg()->OnBnClickedUndo();
			return;
		}
	}
	CNCMDoc* pDoc = static_cast<CNCMDoc*>(GetActiveDocument());
	if (((CMainFrame*)AfxGetMainWnd())->IsCuttingInProgress())
		return;
	((CMainFrame*)AfxGetMainWnd())->Send2PropPane(nullptr);
	theApp.OnDontcutMode();
	pDoc->UHUndoGroup();
	if (pDoc->GetUndoHistory().IsStockChanged())
	{
		pDoc->GetUndoHistory().ResetStockChanged();
		pDoc->ResetStocks();
	}
	LPARAM Hint = N_OGL | N_LEFT;
	pDoc->UpdateAllViews(NULL, Hint, NULL);
}

void CMainFrame::OnTbRedo()
{
	const auto pLeft = static_cast<CLeftView*>(GetLeftPane());
	if (pLeft != nullptr)
	{
		if (pLeft->IsPosDlgActive())
		{
			pLeft->GetPosDlg()->OnBnClickedRedo();
			return;
		}
	}
	if (((CMainFrame*)AfxGetMainWnd())->IsCuttingInProgress())
		return;
	((CMainFrame*)AfxGetMainWnd())->Send2PropPane(nullptr);
	theApp.OnDontcutMode();
	CNCMDoc* pDoc = static_cast<CNCMDoc*>(GetActiveDocument());
	pDoc->UHDoGroup();
	if (pDoc->GetUndoHistory().IsStockChanged())
	{
		pDoc->GetUndoHistory().ResetStockChanged();
		pDoc->ResetStocks();
	}
	LPARAM Hint = N_OGL | N_LEFT;
	pDoc->UpdateAllViews(NULL, Hint, NULL);
}

void CMainFrame::OnUpdateRotatecenteronoff(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(GetFixedRotCenter());
}

void CMainFrame::OnUpdateTbUndo(CCmdUI* pCmdUI)
{
	CNCMDoc* pDoc = static_cast<CNCMDoc*>(GetActiveDocument());
	pCmdUI->Enable((pDoc->UHCanUndo() && !IsCuttingInProgress()) ? TRUE : FALSE);
}

void CMainFrame::OnUpdateTbRedo(CCmdUI* pCmdUI)
{
	CNCMDoc* pDoc = static_cast<CNCMDoc*>(GetActiveDocument());
	pCmdUI->Enable((pDoc->UHCanRedo() && !IsCuttingInProgress()) ? TRUE : FALSE);
}

void CMainFrame::OnUpdateCollsettings(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(CanCollisionBeEnabled());
}

void CMainFrame::OnUpdateViewRegen(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!IsCuttingInProgress());
}

void CMainFrame::OnParticular()
{
	if(NCM_PROJECT.GetParticularExport())
	{
		delete NCM_PROJECT.GetParticularExport();
		NCM_PROJECT.SetParticularExport(nullptr);
	}
	else
	{
		CString Title = 
 			((NStateParamsPane *)GetAnalysePane())->GetExportString(true, true) +
			((NStateParamsPane *)GetUnitPane())->GetExportString(true, true);
		NCM_PROJECT.SetParticularExport(ParticularExport::Generate(Title));
	}
}
void CMainFrame::SyncLayoutWConfig(NCMConfig Config)
{
	if (!(Config & CM_Machine))
	{
		for each (auto pPane in AddOGLPanes)
		{
			if(pPane != nullptr)
				if(pPane->GetType() == false)
					pPane->ShowPane(FALSE, FALSE, FALSE);
		}
	}

	m_wndMTEditPane.ShowPane(FALSE, FALSE, FALSE);
	m_wndMTEditTreePane.ShowPane(FALSE, FALSE, FALSE);
}
void CMainFrame::OnViewRegen()
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	pDoc->Regenerate();
}

bool CMainFrame::SetRemote(CWnd *pWnd)
{
	if(pRemoteControl)
		return true;
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	pDoc->GetUnit()->Reset();
	pRemoteControl = new NRemoteManager;
	// Save programs
	CMapStringToNProgram *pPList = pDoc->NGetProgramList();
	for(int i = 0; i < pPList->GetSize(); ++i)
	{
		CString Key;
		NProgram *pProg = nullptr;
		pPList->GetAt(i, Key, (CObject *&)pProg);
		pRemoteControl->GetBufProgList()[Key] = pProg;
	}
	int MainPC = pPList->GetMainProgCount();
	for(int i = 0; i < pPList->GetSize(); ++i)
	{
		pPList->RemoveAt(i);
	}
	//
	pRemoteControl->Store(*pDoc->GetUnit()->GetState(), pDoc->GetProgrTDT(), MainPC);
	pRemoteControl->SetWnd(pWnd->GetSafeHwnd());
	pRemoteControl->SetEditorVis(m_wndTextView.IsPaneVisible() == TRUE);
	m_wndTextView.ShowPane(FALSE, FALSE, FALSE);
	pDoc->NGetProgramList()->RemoveAll();
	pDoc->UpdateAllViews(nullptr, N_OGL | N_STATE | N_LEFT | N_UNIT_CHANGED);
	return true;
}

bool CMainFrame::IsRemoteControlActive()
{
	return pRemoteControl != nullptr;
}

void CMainFrame::ClearRemote()
{
	StepStop();
	m_wndLeftView.CloseSectProgDlg();

	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	pDoc->DeleteProg(_T("Remote"));
	NCUnitState BState;
	CString ProgName;
	int MainPC;
	pRemoteControl->Restore(BState, ProgName, MainPC);
	// Restore programs
	CMapStringToNProgram *pPList = pDoc->NGetProgramList();
	for (int i = 0; i < MainPC; ++i)
	{
		CString Key;
		NProgram *pProg = nullptr;
		pRemoteControl->GetBufProgList().GetAt(i, Key, (CObject *&)pProg);
		pPList->AddMainProg(Key, pProg);
	}
	for (int i = MainPC; i < pRemoteControl->GetBufProgList().GetSize(); ++i)
	{
		CString Key;
		NProgram *pProg = nullptr;
		pRemoteControl->GetBufProgList().GetAt(i, Key, (CObject *&)pProg);
		(*pPList)[Key] = pProg;
	}
	pRemoteControl->GetBufProgList().RemoveAll();
	//
	for(int i = 0; i < 6; ++i)
		BState.UCSs[i] = pDoc->GetUnit()->GetStartState().UCSs[i];
	pDoc->GetUnit()->SetStartState(BState);
	pDoc->SetProgrTDT(ProgName);
	pDoc->SetModifiedFlag();
	m_wndTextView.ShowPane(pRemoteControl->GetEditorVis(), FALSE, FALSE);
	pDoc->GetUnit()->Reset();
	pDoc->RegeneratePrograms();
	delete pRemoteControl;
	pRemoteControl = nullptr;
	pDoc->UpdateAllViews(nullptr, N_NEW_TEXT | N_OGL | N_STATE | N_LEFT | N_UNIT_CHANGED | N_PROGRAMCHANGED);
}


bool CMainFrame::ChangeMainMenu(int NewMenuID, int NewRibbonID)
{
	if (m_wndMenuBar.GetDefaultMenuResId() != NewMenuID)
	{
		m_wndMenuBar.SetDefaultMenuResId(NewMenuID);
		m_wndMenuBar.RestoreOriginalState();
	}
	if (IsRibbonCreated())
	{
		if (m_wndRibbonBar.GetDlgCtrlID() != NewRibbonID)
		{
			m_wndRibbonBar.EnableWindow(FALSE);
			m_wndRibbonBar.LoadFromResource(NewRibbonID);
			m_wndDlgBar.Reset(&m_wndRibbonBar);
			m_wndRibbonBar.EnableWindow(TRUE);
		}
	}
	ShowPauseDlg();
	return true;
}

void CMainFrame::OnSetupWrite0Cs()
{
	if(!pWriteCSDlg)
	{
		pWriteCSDlg = new NSetupWriteCSDlg;
		pWriteCSDlg->Create(pWriteCSDlg->IDD, this);
	}
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	ASSERT_VALID(pDoc);
	class NCUnit &Unit = *pDoc->GetUnit();
	Unit.SetUCSAutoNames();
	CComboBox &UCSCtrl = pWriteCSDlg->GetUCSCtrl();
	UCSCtrl.ResetContent();
	for (auto it = pDoc->GetUnit()->GetUsedUCSs().begin(); it != pDoc->GetUnit()->GetUsedUCSs().end(); it++)
	{
		CString sval;
		sval.Format("N %03d ", *it + 1);
		sval = sval + Unit.GetStartState().UCSs[*it].GetName();
		int i = UCSCtrl.AddString(sval);
		UCSCtrl.SetItemData(i, *it);
	}
	pWriteCSDlg->ShowWindow(SW_SHOW);
}

void CMainFrame::OnRemCont()
{
	if(pRemoteControl)
		return;
	CString Name = NCM_PROJECT.GetSettingsPath() + _T("RemoteControl.exe");
	auto Res = _spawnl(_P_NOWAIT, Name, Name, nullptr);
	if(Res < 0)
	{
		TCHAR szPath[_MAX_PATH];
		VERIFY(::GetModuleFileName(nullptr, szPath, _MAX_PATH));
		Name = szPath;
		int nIndex  = Name.ReverseFind(_T('\\'));
		if (nIndex > 0) 
			Name = Name.Left(nIndex + 1);
		else
			Name.Empty();
		Name = Name + _T("RemoteControl.exe");
		auto Res = _spawnl(_P_NOWAIT, Name, Name, nullptr);
		if(Res < 0)
			AfxMessageBox(IDS_MES_REMOTECONTROL_START);
	}
}

void CMainFrame::OnUpdateRemCont(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(pRemoteControl == nullptr);
}

void CMainFrame::OnUpdateSetupWrite0Cs(CCmdUI *pCmdUI)
{
	if(!pWriteCSDlg)
		return;
	pCmdUI->Enable(!pWriteCSDlg->IsWindowVisible());
}

void CMainFrame::OnUpdateSetupSetboxpoint(CCmdUI *pCmdUI)
{
	if(!pSetBoxPointDlg)
		return;
	pCmdUI->Enable(!pSetBoxPointDlg->IsWindowVisible());
}

void CMainFrame::OnUpdateSetupSetUCSOnbox(CCmdUI *pCmdUI)
{
	if(!pSetUCSOnBoxDlg)
		return;
	pCmdUI->Enable(!pSetUCSOnBoxDlg->IsWindowVisible());
}

void CMainFrame::OnUpdateRotCenter(CCmdUI *pCmdUI)
{
	int Check = 0;
	if(pRotCenterDlg)
		if(pRotCenterDlg->IsWindowVisible())
			Check = 1;
	pCmdUI->SetCheck(Check);
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (nCode == CN_COMMAND || nCode == CN_UPDATE_COMMAND_UI)
	{
		{
			const UINT IDs[] =
			{ ID_TB_ZOOM_W, ID_TB_ZOOM_SC, ID_TB_ZOOM_P, ID_TB_ZOOM_ALL,
			ID_TB_ZOOM_R, ID_TB_VIEW_PREV, ID_TB_VIEW_NEXT, ID_TB_ZOOM_Z
			};

			for (int i = 0; i < sizeof(IDs) / sizeof(UINT); ++i)
				if (nID == IDs[i])
				{
					NOpenGLWnd *pOGLWnd = GetViewToHandle();
					if (pOGLWnd)
						return pOGLWnd->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
				}
		}
		{
			const UINT IDs[] =
			{ ID_PA_MACH_VISIBLE, ID_PA_TOOLS_SHAPE, ID_PA_EXOBJ_VISIBLE, ID_PA_PART_VISIBLE,
			ID_PA_STOCK_VISIBLE, ID_PA_PROG_VISIBLE,
			ID_LOAD, ID_SAVE, ID_LOAD_AUX, ID_PA_TOOLS_DELUNUSED, ID_PA_TOOLS_IMPNCM, ID_PA_PROGR_CR_AUX,
			ID_PA_PROG_DELAllSUBPROG, ID_PA_PROGR_CR, ID_PA_PROG_RELO, ID_PA_PROG_DELAllPROG,
			ID_CH_STOCK_CR_BOX, ID_CH_STOCK_CR_CYL, ID_CH_STOCK_CR_CONTBOX, ID_CH_STOCK_CR_STL, ID_CH_STOCK_CR_CONE,
			ID_CH_STOCK_REPLACE_BOX, ID_CH_STOCK_REPLACE_CYL, ID_CH_STOCK_REPLACE_CONTBOX, ID_CH_STOCK_REPLACE_STL,
			ID_NCUNIT_START, ID_PA_STOCK_DELETE, ID_PA_STOCK_POS,
			ID_CH_STOCK_CR_PIPE, ID_CH_STOCK_CR_PROFILE,
			ID_CH_STOCK_REPLACE_PROFILE, ID_CH_STOCK_REPLACE_PIPE, ID_CH_STOCK_REPLACE_CONE, ID_PA_STOCK_SEC_VIEW
			};

			for (int i = 0; i < sizeof(IDs) / sizeof(UINT); ++i)
				if (nID == IDs[i])
				{
					return m_wndLeftView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
				}
		}
		{
			const UINT IDs[] =
			{ ID_EDIT_FIND, ID_FIND_TEXT, ID_MODE_PAINTER,
			ID_ADD_BOOKMARK, ID_NEXT_BOOKMARK, ID_PREV_BOOKMARK, ID_CLEARALL_BOOKMARK,
			ID_BUTTON_ADDSPACE, ID_BUTTON_REMOVESPACE, ID_BUTTON_LINENUM,
			ID_EDIT_COMMENT, ID_EDIT_UNCOMMENT,
			ID_EDIT_UNDO, ID_EDIT_REDO, ID_EDIT_UPDATE, ID_EDIT_SELECT_ALL, ID_EDIT_CUT, ID_EDIT_COPY, ID_EDIT_PASTE, ID_EDIT_RENUM,  ID_TOOLS_LIST
			};

			for (int i = 0; i < sizeof(IDs) / sizeof(UINT); ++i)
				if (nID == IDs[i])
				{
					return m_wndTextView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
				}
		}
	}

	return CFrameWndEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnComparison()
{
	CNCMDoc* pDoc = static_cast<CNCMDoc*>(GetActiveDocument());
	if(pDoc->Comparison.IsActive())
	{
		pDoc->Comparison.DeActivate();
	}
	else
	{
		if(!CheckCutting())
			return;

		if(!pDoc->IsThereAnyStock())
		{
			AfxMessageBox(IDS_COMP_NOSTOCK);
			return;
		}

		if(!theApp.IsInCuttingMode() /*|| NCM_PROJECT.GetGlobalState().GetModelMode() != NGlobalState::BSP*/)
		{
			AfxMessageBox(IDS_COMP_NOCUTTING);
			return;
		}

		if(!pDoc->IsThereAnyPart())
		{
			AfxMessageBox(IDS_COMP_NOPART);
			return;
		}
		if(!theApp.IsInCuttingMode())
		{
			AfxMessageBox(IDS_COMP_NOCUTTING);
			return;
		}
		if(NCMComData::GetType() == AT_TURN)
			return;

		BStockGrID AllStocks;
		AllStocks.Set(-1);
		pDoc->NGetStockList()->ApplyStocksState(AT_MILL, std::vector< BStockGrID>(1, AllStocks));
		class NStock *pStock = pDoc->GetFirstVisStock();
		if (pStock == nullptr)
		{
			AfxMessageBox(IDS_COMP_NOSTOCK);
			return;
		}
		CWaitCursor w;
		pDoc->Comparison.Activate(GetLeftPane(), pDoc->NGetPartList(), pStock);
	}
	pDoc->UpdateAllViews(nullptr, N_INFOTIP);
}

void CMainFrame::OnCreateEnt()
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	if(pDoc->Measure.IsActivated())
	{
		pDoc->Measure.DeActivate();
	}

	if(!CheckCutting())
		return;

	pDoc->Dimensions.Activate(pDoc);
	if (!pDimEntCrSheet)
	{
		pDimEntCrSheet = new DDimEntCrSheet(IDS_CAPTION_SIZES, this, 0);

 		CPropertyPage *pPrimPointDlg = new DPrimDimens((CNCMDoc*)GetActiveDocument());
		pDimEntCrSheet->AddPage(pPrimPointDlg);

        pPrimPointDlg = new DPrimPoint((CNCMDoc*)GetActiveDocument());
		pDimEntCrSheet->AddPage(pPrimPointDlg);

		pPrimPointDlg = new DPrimSegment((CNCMDoc*)GetActiveDocument());
        pDimEntCrSheet->AddPage(pPrimPointDlg);

		pPrimPointDlg = new DPrimArc((CNCMDoc*)GetActiveDocument());
        pDimEntCrSheet->AddPage(pPrimPointDlg);

        pPrimPointDlg = new DPrimPlane((CNCMDoc*)GetActiveDocument());
        pDimEntCrSheet->AddPage(pPrimPointDlg);

        pPrimPointDlg = new DPrimAutofix((CNCMDoc*)GetActiveDocument());
        pDimEntCrSheet->AddPage(pPrimPointDlg);


		pDimEntCrSheet->Create(this);
	}
	DPrimBase* pPrimBase = dynamic_cast<DPrimBase*>(pDimEntCrSheet->GetActivePage());
	if (pPrimBase != nullptr)
		pPrimBase->FromDialog();
	pDimEntCrSheet->ShowWindow(SW_SHOW);
}

void CMainFrame::CreateDimStock()
{
    CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());

	pDoc->Dimensions.Activate(pDoc);
    if (!pDimStockCrDlg)
    {
        pDimStockCrDlg = new CrStockTemp(pDoc);
        pDimStockCrDlg->Create(CrStockTemp::IDD, GetLeftPane());
    }
	pDimStockCrDlg->Init();
	pDimStockCrDlg->ShowWindow(SW_SHOW);
}

void CMainFrame::OnUpdateCreateEnt(CCmdUI *pCmdUI)
{
	if (!(theApp.GetConfig() & CM_Measure))
	{
		pCmdUI->Enable(0);
		return;
	}
	BOOL Enable = true;
	if(pDimEntCrSheet)
		if(pDimEntCrSheet->IsWindowVisible())
			Enable = false;
	Enable &= !IsCrDimStockVisible();
	pCmdUI->Enable(Enable);
}

bool CMainFrame::IsCrDimStockVisible()
{
	if(pDimStockCrDlg)
		if(pDimStockCrDlg->IsWindowVisible())
			return true;
	return false;
}

void CMainFrame::OnUpdateComparison(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(NCMComData::GetType() != AT_TURN);
	pCmdUI->SetCheck(IsComparisonActive());
}

LRESULT CMainFrame::OnGetAbsObj(WPARAM wParam, LPARAM lParam)
{
	return (LRESULT)((CLeftView *)GetLeftPane())->GetAbsObj();
}

LRESULT CMainFrame::OnPSlide(WPARAM wParam, LPARAM lParam)
{
	if(IsCuttingInProgress())
		return 1;
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	if(!pDoc)
		return 0;
	switch(wParam)
	{
	case SB_ENDSCROLL:
		pDoc->StopSlider((int)lParam);
		return 0;
	default:
		pDoc->SetSliderPos((int)lParam);
		AnimateUpdate();
		return 0;
	}
	return 0;
}

void CMainFrame::ShowPropPane(void)
{
	m_wndPropPane.ShowPane(TRUE, FALSE, TRUE);
	m_wndPropPane.Slide(TRUE, TRUE);
	m_wndPropPane.SetFocus();
}

void CMainFrame::ShowTextView(void)
{
	m_wndTextView.ShowPane(TRUE, FALSE, TRUE);
	m_wndTextView.SetFocus();
}

void CMainFrame::Send2PropPane(PAbstractObject *pPObj)
{
	((PropPane *)GetPropPane())->GetEditor().Put(pPObj);
}

void CMainFrame::OnChStockDivide()
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	if(pDoc->pDivide != nullptr)
	{
		delete pDoc->pDivide;
		pDoc->pDivide = nullptr;
		delete pDivideDlg;
		pDivideDlg = NULL;
		return;
	}

	if(!CheckCutting())
		return;

	class NStock *pStock = GetActiveStock();
	if(!pStock)
	{
		AfxMessageBox(IDS_COMP_NOSTOCK);
		return;
	}

	if(!theApp.IsInCuttingMode())
	{
		AfxMessageBox(IDS_COMP_NOCUTTING);
		return;
	}

	if (NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::BSP)
	{
		if (pStock->GetStockType() == AT_TURN)
			pDoc->pDivide = new QDivide(pStock->GetBodyRot(), &MBSPForest::GetActive());
		else
			pDoc->pDivide = new QDivide(pStock->GetBodyMill(), &MBSPForest::GetActive());
	}
	else // DX
	{
		pDoc->pDivide = new QDXDivide(pStock->GetBodyMill());
	}

	pDivideDlg = new QDivideDlg(this, pDoc->pDivide);
	pDivideDlg->Create(QDivideDlg::IDD);
	pDivideDlg->ShowWindow(SW_SHOW);

	pDoc->UpdateAllViews(nullptr, N_OGL);
}

void CMainFrame::OnJumpOver()
{
	if(IsCuttingInProgress())
		return;
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	ASSERT(pDoc);
	if(!pDoc->CheckProgExist())
		return;
	pDoc->GetUnit()->JumpOver();
	pDoc->UpdateAllViews(NULL, N_STATE | N_CADRCHANGED | N_OGL | N_SELECTTEXT);
}

void CMainFrame::StartImportMode()
{
	if(pImportDlg)
		pImportDlg->DestroyWindow();
	pImportDlg = new NImportDlg(this);
	ImportMode = true;
	pImportDlg->Create(pImportDlg->IDD, this);
	pImportDlg->ShowWindow(SW_SHOW);
}

LRESULT CMainFrame::OnStopImpMode(WPARAM wParam, LPARAM lParam)
{
	ImportMode = false;
	CStringArray CheckedTools;
	CLeftView *pLeftView = (CLeftView *)GetLeftPane();
	pLeftView->GetCheckedToolList(CheckedTools);
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	pDoc->SetModifiedFlag(FALSE); // Don't ask about saving
	if(wParam == IDOK)
		pDoc->CopyTools2Clipboard(CheckedTools);
	pDoc->PasteToolsFrClipboard();
	pDoc->UpdateAllViews(nullptr, N_LEFT);
	pLeftView->ExpandTools();
	return 0;
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle , CWnd* pParentWnd , CCreateContext* pContext)
{
	return CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext);
}

void CMainFrame::OnUpdateViewSection(CCmdUI *pCmdUI)
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	NStockSection *pSection = pDoc->GetStockSection();
	int Check = 0;
	if(pSection)
		if(pSection->IsActive())
			Check = 1;
	pCmdUI->SetCheck(Check);
}

void CMainFrame::OnSetupAutops()
{
	if(theApp.IsInCuttingMode())
	{
		AfxMessageBox(IDS_COMP_CUTTING);
		return;
	}
	if(IsComparisonActive())
	{
		AfxMessageBox(IDS_END_COMPARE);
		return;
	}
	
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	if(!pDoc->IsThereAnyStock())
		return;
	BBox MSGabar = pDoc->GetModifStocksGabar();
	BMatr M;
	BPoint Center = (MSGabar.GetMinPoint() + MSGabar.GetMaxPoint()) * 0.5;
	Center.SetZ(MSGabar.Zmin);
	M.Trans(Center, BPoint(0., 0., 0., 1.));

	const bool InternalGroup = pDoc->GetUndoHistory().StartGroup();
	pDoc->TransformAllStocks(M);

	CMapStringToNStock *elist = pDoc->NGetPartList();
	if(elist->GetMooveWithStock())
	{
		for each (const auto & el in *elist)
		{
			class NStock* pS = static_cast<class NStock*>(el.second);
			pDoc->TransMovObjWU(pS, M);
		}
	}
	elist = pDoc->NGetExObjList();
	if(elist->GetMooveWithStock())
	{
		for each (const auto & el in *elist)
		{
			class NStock* pS = static_cast<class NStock*>(el.second);
			pDoc->TransMovObjWU(pS, M);
		}
	}
	pDoc->GetUndoHistory().EndGroup(InternalGroup);

	pDoc->ResetStocks();
	pDoc->SetModifiedFlag();
	pDoc->UpdateAllViews(nullptr, N_OGL);

}

LRESULT CMainFrame::OnMacroApply(WPARAM wParam, LPARAM lParam)
{
	if (!CheckCutting())
		return LRESULT();
	((NTextView*)GetTextView())->ApplyMacro();
	return LRESULT();
}

void CMainFrame::OnDropFiles(HDROP hDropInfo)
{
	SetActiveWindow();      // activate us first !
	UINT nFiles = ::DragQueryFile(hDropInfo, (UINT)-1, nullptr, 0);
	
	bool CanRead = false;
	if(nFiles == 1)
	{
		TCHAR szFileName[_MAX_PATH];
		::DragQueryFile(hDropInfo, 0, szFileName, _MAX_PATH);
		CString FileName(szFileName);
		int Ind = FileName.ReverseFind('.');
		FileName.Delete(0, Ind + 1);
		FileName.MakeUpper();
		if(FileName == _T("NCM"))
			CanRead = true; 
	}
	if(CanRead)
		CFrameWndEx::OnDropFiles(hDropInfo);
	else
		::DragFinish(hDropInfo);
}

void CMainFrame::WriteCS_OK(int Ucs)
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	ASSERT(pDoc);
	pDoc->StoreCurrent2UCS(Ucs);
	pDoc->UpdateAllViews(nullptr, N_STATE);
}

void CMainFrame::OnSetupSetboxpoint()
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	ASSERT(pDoc);
	if(!pDoc->IsThereAnyStock())
	{
		AfxMessageBox(IDS_COMP_NOSTOCK);
		return;
	}
	if(!pSetBoxPointDlg)
	{
		pSetBoxPointDlg = new NSetupSetBoxPointDlg(this, pDoc);
		pSetBoxPointDlg->Create(pSetBoxPointDlg->IDD, this);
	}
	pSetBoxPointDlg->ShowWindow(SW_SHOW);
}

void CMainFrame::OnSetupSetUCSOnbox()
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	ASSERT(pDoc);
	if(!pDoc->IsThereAnyStock())
	{
		AfxMessageBox(IDS_COMP_NOSTOCK);
		return;
	}
	if(!pSetUCSOnBoxDlg)
	{
		pSetUCSOnBoxDlg = new NSetupSetUCSOnBox(this, pDoc);
		pSetUCSOnBoxDlg->Create(pSetUCSOnBoxDlg->IDD, this);
	}
	pSetUCSOnBoxDlg->ShowWindow(SW_SHOW);
}

void CMainFrame::OnRotCenter()
{
	if(!pRotCenterDlg)
	{
		pRotCenterDlg = new NRotCenterDlg(this);
		pRotCenterDlg->Create(pRotCenterDlg->IDD, this);
	}
	pRotCenterDlg->ShowWindow(SW_SHOW);
}

void CMainFrame::SetRotCenter(const BOpenGLWnd::NRotCenter &RotCenter)
{
	if(pMainPane != nullptr)
		pMainPane->SetRotCenter(RotCenter);
	for each (auto pPane in AddOGLPanes)
		pPane->SetRotCenter(RotCenter);
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	pDoc->UpdateAllViews(nullptr, N_OGL);
}

void CMainFrame::GetRotCenter(BOpenGLWnd::NRotCenter& RotCenter) const
{
	if (AddOGLPanes.empty())
		return;
	const NOpenGLWnd* pOGLWnd = AddOGLPanes.front()->GetOpenGLWnd();
	if (pOGLWnd == nullptr)
		return;
	pOGLWnd->GetRotCenter(RotCenter);
}

//void CMainFrame::OnDemoComment()
//{
//	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
//	ASSERT_VALID(pDoc);
//	NMapStringToOb *plist = pDoc->NGetProgramList();
//	POSITION pos;
//	CString key;
//	NProgram *pP;
//	for( pos = plist->GetStartPosition(); pos != nullptr; )
//	{
//		plist->GetNextAssoc( pos, key, (CObject*&)pP);
//		pP->SetComment(MakeComment(key));
//	}
//	pDoc->UpdateAllViews(nullptr, N_OGL);
//}

//const CString & CMainFrame::MakeComment(CString &Str)
//{
//	for(int i = 0; i < Str.GetLength(); ++i)
//		Str.SetAt(i, ' ');
//	return Str;
//}

class NStock * CMainFrame::GetActiveStock()
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	if (!pDoc->IsThereAnyStock())
		return nullptr;

	CLeftView *pView = (CLeftView*)GetLeftPane();
	if (!pView)
		return nullptr;

	CString key;
	class NStock *pS = nullptr;
	if (!pView->GetSelListElem(key, pS))
		return nullptr;
	if (!pS->IsRealStock())
		return nullptr;
	return pS;
}

void CMainFrame::ForcePause()
{
	if (theApp.IsInAnimateMode())
	{
		if (CanAnimateBePaused())
			OnAnimatePause();
	}
	else
		StepStop();
}

void CMainFrame::ApplyBoxDialogs(void)
{
	if(pSetBoxPointDlg)
		if(pSetBoxPointDlg->IsWindowVisible())
			pSetBoxPointDlg->OnToolApply();
	if(pSetUCSOnBoxDlg)
		if(pSetUCSOnBoxDlg->IsWindowVisible())
			pSetUCSOnBoxDlg->OnToolApply();
	ResetMeasureStock();
}

void CMainFrame::ResetMeasureStock()
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	if(!pDoc->Measure.IsActivated())
		return;
	pDoc->Measure.DeActivate();

	InitMeasure();
}

void CMainFrame::UpdateDialogData()
{
    if (pDimEntCrSheet)
    {
        CPropertyPage* page = pDimEntCrSheet->GetActivePage();
        if (dynamic_cast<DPrimPoint*>(page))
            static_cast<DPrimPoint*>(page)->UpdateDialog();
        else if (dynamic_cast<DPrimSegment*>(page))
            static_cast<DPrimSegment*>(page)->UpdateDialog();
        else if (dynamic_cast<DPrimArc*>(page))
            static_cast<DPrimArc*>(page)->UpdateDialog();
        else if (dynamic_cast<DPrimPlane*>(page))
            static_cast<DPrimPlane*>(page)->UpdateDialog();
        else if (dynamic_cast<DPrimDimens*>(page))
            static_cast<DPrimDimens*>(page)->UpdateDialog();
    }
    
    if (pDimStockCrDlg)
    {
		pDimStockCrDlg->UpdateDialog();
    }
}

void CMainFrame::ClearDimEntCrSheet(void)
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	pDoc->Dimensions.DeActivate();
    pDoc->UpdateAllViews(nullptr, N_OGL);
}

void CMainFrame::CloseDimDialogs()
{
	if(pDimEntCrSheet)
		pDimEntCrSheet->ShowWindow(SW_HIDE); 
	if(pDimStockCrDlg)
		pDimStockCrDlg->ShowWindow(SW_HIDE); 
}

void CMainFrame::OnDestroy()
{
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	if (!pDoc)
		return;

	delete pRemoteControl;
	pRemoteControl = nullptr;

	CFrameWndEx::OnDestroy();

	if (theApp.GetRemoveState())
	{
		theApp.CleanState();
	}
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_MOUSEWHEEL) // To send WM_MOUSEWHEEL to the window under the cursor
	{
		CPoint point;
		GetCursorPos(&point);

		HWND hWnd = ::WindowFromPoint(point);

		if (hWnd && pMsg->hwnd != hWnd)
		{
			::PostMessage(hWnd, pMsg->message, pMsg->wParam, pMsg->lParam);
			return(true);
		}
	}
	return CFrameWndEx::PreTranslateMessage(pMsg);
}

void CMainFrame::OnHelp()
{
	CFrameWndEx::OnHelp();
}

/*Menu item control handler to switch to ribbon interface*/
void CMainFrame::OnStyleRibboninterface()
{
	theApp.WriteProfileString(_T(""), _T("InterfaceStyle"), _T("Ribbon"));
	OnViewDefault();
}

/*Ribbon bar button click handler to switch to menu/toolbar interface*/
void CMainFrame::OnRbnBtnToolbarInterface()
{
	theApp.WriteProfileString(_T(""), _T("InterfaceStyle"), _T("Menu"));
	OnViewDefault();
}

void CMainFrame::ForceRedraw()
{
	//Forcefully redraw the frame window without changing its size and position.
	CRect rectThis;
	GetWindowRect(&rectThis);
	ShowWindow(SW_SHOWMINIMIZED);
	ShowWindow(SW_SHOWNORMAL);
	SetWindowPos(
		&wndTop,
		rectThis.left,
		rectThis.top,
		rectThis.Width() - 5,
		rectThis.Height(),
		SWP_DRAWFRAME | SWP_FRAMECHANGED | SWP_NOZORDER
		);
	SetWindowPos(
		&wndTop,
		rectThis.left,
		rectThis.top,
		rectThis.Width(),
		rectThis.Height(),
		SWP_DRAWFRAME | SWP_FRAMECHANGED | SWP_NOZORDER
		);
}

void CMainFrame::OnNCUnitCombo()
{
	if (!CheckCutting())
		return;

	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	if (!pDoc)
		return;
	if (m_wndRibbonBar.MOnNCUnitCombo())
	{
		pDoc->LoadNCUnit(m_wndRibbonBar.GetNCUnitNewName());
		SetNCUnitName(pDoc->GetUnit()->GetConfFileName());
	}
}

void CMainFrame::OnMachCombo()
{
	if (!CheckCutting())
		return;

	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	if (!pDoc)
		return;
	if (m_wndRibbonBar.MOnMachCombo())
	{
		pDoc->LoadMT(m_wndRibbonBar.GetMachNewName());
		SetMachName(m_wndRibbonBar.GetMachNewName());
	}
}

void CMainFrame::SetNCUnitName(const CString &Name)
{
	if (IsRibbonCreated())
		m_wndRibbonBar.SetNCUnitName(Name);
}

void CMainFrame::SetMachName(const CString &Name)
{
	if (IsRibbonCreated())
		m_wndRibbonBar.SetMachName(Name);
}
void CMainFrame::FocusSetText()
{
}
 
void CMainFrame::FocusKillText()
{
	if (IsRibbonCreated())
	{
		//m_wndRibbonBar.ShowContextCategories(ID_CONTEXT_TEXT, TRUE);
		//m_wndRibbonBar.RecalcLayout();
	}
}

void CMainFrame::OnOWindows()
{
	CPoint point;
	GetCursorPos(&point);
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_RIB_WINDOW));
	CMenu * pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != nullptr);
	for each (auto pPane in AddOGLPanes)
	{
		if (!pPane->AllowShowOnPaneMenu())
			break;
		CString Title;
		pPane->GetPaneName(Title);
		pPopup->InsertMenu(pPopup->GetMenuItemCount(), MF_BYPOSITION, pPane->GetID(), Title);
	}
	for (UINT ID = theApp.GetContextMenuManager()->TrackPopupMenu(*pPopup, point.x, point.y, this);
		ID != 0;
		ID = theApp.GetContextMenuManager()->TrackPopupMenu(*pPopup, point.x, point.y, this))
	{
		SendMessage(WM_COMMAND, ID);
		pPopup = menu.GetSubMenu(0);
	}

	menu.DestroyMenu(); 
}

void CMainFrame::OnStockPaneRib()
{
	for each (auto pPane in AddOGLPanes)
	{
		if (!pPane->AllowShowOnPaneMenu())
		{
			pPane->SetShowOnPaneMenu(TRUE);
			pPane->SetType(true);
			pPane->FloatPane(GetNewPaneRect(), DM_UNKNOWN, true);
			pPane->ShowPane(TRUE, FALSE, TRUE);
			pPane->GetOpenGLWnd()->OnUpdate(NULL, N_SETTINGS, nullptr);
			break;
		}
	}
}

void CMainFrame::OnMachPaneRib()
{
	for each (auto pPane in AddOGLPanes)
	{
		if (!pPane->AllowShowOnPaneMenu())
		{
			pPane->SetShowOnPaneMenu(TRUE);
			pPane->SetType(false);
			pPane->FloatPane(GetNewPaneRect(), DM_UNKNOWN, true);
			pPane->ShowPane(TRUE, FALSE, TRUE);
			pPane->GetOpenGLWnd()->OnUpdate(NULL, N_SETTINGS, nullptr);
			break;
		}
	}
}

void CMainFrame::OnUpdateStockPaneRib(CCmdUI *pCmdUI)
{
	BOOL Enable = !AddOGLPanes.back()->AllowShowOnPaneMenu();
	pCmdUI->Enable(Enable);
}

void CMainFrame::OnUpdateMachPaneRib(CCmdUI *pCmdUI)
{
	BOOL Enable = !AddOGLPanes.back()->AllowShowOnPaneMenu();
	Enable = Enable && (theApp.GetConfig() & CM_Machine);
	pCmdUI->Enable(Enable);
}

void CMainFrame::OnUpdateTextPaneRib(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndTextView.IsVisible());
}

void CMainFrame::OnTextPaneRib()
{
	BOOL Val = !m_wndTextView.IsVisible();
	m_wndTextView.ShowPane(Val, FALSE, FALSE);
}

void CMainFrame::OnUpdateLeftPaneRib(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndLeftView.IsVisible());
}

void CMainFrame::OnLeftPaneRib()
{
	BOOL Val = !m_wndLeftView.IsVisible();
	m_wndLeftView.ShowPane(Val, FALSE, FALSE);
}

void CMainFrame::OnUpdateUnitPaneRib(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndUnitPane.IsVisible());
}

void CMainFrame::OnUnitPaneRib()
{
	BOOL Val = !m_wndUnitPane.IsVisible();
	m_wndUnitPane.ShowPane(Val, FALSE, FALSE);
}

void CMainFrame::OnUpdateAnalysePaneRib(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndAnalysePane.IsVisible());
}

void CMainFrame::OnAnalysePaneRib()
{
	BOOL Val = !m_wndAnalysePane.IsVisible();
	m_wndAnalysePane.ShowPane(Val, FALSE, FALSE);
}

void CMainFrame::OnUpdateGeomPaneRib(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndGeomPane.IsVisible());
}

void CMainFrame::OnGeomPaneRib()
{
	BOOL Val = !m_wndGeomPane.IsVisible();
	m_wndGeomPane.ShowPane(Val, FALSE, FALSE);
}

void CMainFrame::OnUpdateMesurePaneRib(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndMesurePane.IsVisible());
}

void CMainFrame::OnMesurePaneRib()
{
	BOOL Val = !m_wndMesurePane.IsVisible();
	m_wndMesurePane.ShowPane(Val, FALSE, FALSE);
}

void CMainFrame::OnUpdateReportPaneRib(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndReportPane.IsVisible());
}

void CMainFrame::OnReportPaneRib()
{
	BOOL Val = !m_wndReportPane.IsVisible();
	m_wndReportPane.ShowPane(Val, FALSE, FALSE);
}

void CMainFrame::OnUpdateRegsPaneRib(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndRegsPane.IsVisible());
}

void CMainFrame::OnRegsPaneRib()
{
	BOOL Val = !m_wndRegsPane.IsVisible();
	m_wndRegsPane.ShowPane(Val, FALSE, FALSE);
}

void CMainFrame::OnUpdateSlidePaneRib(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndPSlidePane.IsVisible());
}

void CMainFrame::OnSlidePaneRib()
{
	BOOL Val = !m_wndPSlidePane.IsVisible();
	m_wndPSlidePane.ShowPane(Val, FALSE, FALSE);
}

void CMainFrame::OnUpdatePropertiesPaneRib(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropPane.IsVisible());
}

void CMainFrame::OnUpdateDeleteChannel(CCmdUI *pCmdUI)
{
	CNCMDoc* pDoc = static_cast<CNCMDoc*>(GetActiveDocument());
	ASSERT(pDoc);
	pCmdUI->Enable(pDoc->GetChannelsCount() > 1);
}

void CMainFrame::OnPropertiesPaneRib()
{
	BOOL Val = !m_wndPropPane.IsVisible();
	m_wndPropPane.ShowPane(Val, FALSE, FALSE);
}


void CMainFrame::OnAddpaneDelall()
{
	//m_dockManager.LoadState(((CWinAppEx*)AfxGetApp())->GetRegSectionPath(), IDR_MAINFRAME);
	//m_dockManager.SetDockState();
	//m_dockManager.ShowDelayShowMiniFrames(TRUE);
//	theApp.LoadState(this);

	DeleteAddOGLPanes();
}

void CMainFrame::RunNcunitStartDlg(int StartPage)
{
	if (!CheckCutting())
		return;

	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	ASSERT(pDoc);
	CString CurProg(pDoc->GetUnit()->GetState()->GetCurProg());
	int CurHCadr = pDoc->GetUnit()->GetState()->GetCurHCadr();
	int Ret = pDoc->GetUnit()->AskStartState(StartPage, pDoc->GetCurChannel()->GetAxes().ZeroPos);
	bool DoRedraw = false;
	if (Ret & ZPS_NEED_REGEN)
	{
		pDoc->RegeneratePrograms();
		pDoc->GetUnit()->Reset();
		pDoc->SetModifiedFlag();
		DoRedraw = true;
	}
	if (Ret != pDoc->GetCurChannel()->GetAxes().ZeroPos)
	{
		pDoc->GetCurChannel()->GetAxes().ZeroPos = Ret;
		DoRedraw = true;
	}
	if(DoRedraw)
		pDoc->UpdateAllViews(nullptr, N_OGL | N_STATE | N_LEFT_ATTR | N_PROGRAMCHANGED, nullptr);
}

// редактирование составного инструмента
void CMainFrame::ChConToolEdit(const NToolID &ToolConID)
{
	PrepCreateSheet();
	ConToolEdit(ToolConID);
	StartCreateSheet(IDS_TOOL_EDIT_TITLE);
}

void CMainFrame::StartCreateSheet(UINT nID)
{
	CString Title;
	Title.LoadString(nID);
	pCreateSheet->SetTitle(Title);
	pCreateSheet->Create(this, WS_SYSMENU | WS_POPUP | WS_CAPTION | DS_MODALFRAME | DS_CONTEXTHELP);
	if (!CreateSheetRect.IsRectNull())
		pCreateSheet->MoveWindow(CreateSheetRect);
	pCreateSheet->ShowWindow(SW_SHOW);
	for (int i = 0; i < pCreateSheet->GetPageCount(); ++i)
	{
		CPropertyPage* pPage = pCreateSheet->GetPage(i);
		if (pPage != nullptr)
			pPage->SetModified(TRUE);
	}
}

void CMainFrame::PrepCreateSheet()
{
	CNCMDoc* pDoc = static_cast<CNCMDoc*>(GetActiveDocument());
	ASSERT_VALID(pDoc);

	CloseCreateSheet();
	pCreateSheet = new NToolsConCrSheet;
	// Save current position and start tool. Jump to project start.
	class NCUnit& Unit = *pDoc->GetUnit();
	pCreateSheet->GetStoredCadrID() = Unit.GetState()->GetCurCadrIDEx();
	pCreateSheet->GetStoredToolID() = Unit.GetStartState().CurToolN;
	Unit.JumpToProjStart();
	pDoc->UpdateAllViews(nullptr, N_OGL | N_LEFT_ATTR | N_TOOLCHANGED);
	//

	delete pCreateMillPage;
	pCreateMillPage = new NToolsConCrMillPage;
	delete pCreateTurnPage;
	pCreateTurnPage = new NToolsConCrTurnPage;
	delete pCreateWeldPage;
	pCreateWeldPage = new NToolsConCrWeldPage;

	NMapStringToNTool* plist = pDoc->NGetToolList();
	pCreateMillPage->PrepNew(plist);
	pCreateTurnPage->PrepNew(plist);
	pCreateWeldPage->PrepNew(plist);

	if (NCMComData::IsMillEnabled())
		pCreateSheet->AddPage(pCreateMillPage);
	if (NCMComData::IsTurnEnabled())
		pCreateSheet->AddPage(pCreateTurnPage);
	if (NCMComData::IsWeldEnabled())
		pCreateSheet->AddPage(pCreateWeldPage);
}

void CMainFrame::CloseCreateSheet()
{
	if (pCreateSheet == nullptr)
		return;
	if (pCreateSheet->GetSafeHwnd() != NULL)
		pCreateSheet->DestroyWindow();
	delete pCreateSheet;
	pCreateSheet = nullptr;
}

void CMainFrame::ConToolEdit(const NToolID& ToolConID)
{
	CNCMDoc* pDoc = static_cast<CNCMDoc*>(GetActiveDocument());
	ASSERT_VALID(pDoc);
	NMapStringToNTool* plist = pDoc->NGetToolList();
	NToolCombined* pExTool;
	if (!plist->Lookup(ToolConID, pExTool))
		return;//error
	pCreateSheet->GetOldToolID() = ToolConID;// to remove old tool in OnMakeTool
	switch (pExTool->GetClass())
	{
	case AT_MILL:
		pCreateSheet->SetActivePage(pCreateMillPage);
		pExTool->FillDialog(pCreateMillPage);
		if (pExTool->num_tools <= 0)
			pExTool->NToolCombined::FillDialog(pCreateMillPage);
		break;
	case AT_TURN:
		pCreateSheet->SetActivePage(pCreateTurnPage);
		pExTool->FillDialog(pCreateTurnPage);
		if (pExTool->num_tools <= 0)
			pExTool->NToolCombined::FillDialog(pCreateTurnPage);
		break;
	case AT_WELD:
		pCreateSheet->SetActivePage(pCreateWeldPage);
		pExTool->FillDialog(pCreateWeldPage);
		break;
	default:
		return;//error
		break;
	}
	class NCUnit& Unit = *pDoc->GetUnit();
	Unit.GetStartState().CurToolN = ToolConID;
	Unit.JumpToProjStart();
	Unit.GetState()->CurToolN = ToolConID;
	pDoc->UpdateAllViews(nullptr, N_OGL | N_LEFT | N_TOOLCHANGED);
}

// создание нового составного фрезерного инструмента
NToolID CMainFrame::CreateMillTool(NToolsConCrMillPage &CreateMillPage)
{
	NToolCombinedMill *pToolCon;
	pToolCon = CreateMillPage.GetNewTool();
	if (pToolCon == nullptr)
		return NToolID();
	NToolID ToolID;
	ToolID.Set(CreateMillPage.m_pos, CreateMillPage.m_pos_turret);
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	NMapStringToNTool *plist = pDoc->NGetToolList();
	NToolCombined *pOldToolCon;
	if (plist->Lookup(pCreateSheet->GetOldToolID(), pOldToolCon))
	{
		pToolCon->SetColor(pOldToolCon->Color);
		pToolCon->SetColorPassive(pOldToolCon->ColorPassive);
		pToolCon->SetColorHolder(pOldToolCon->ColorHolder);
		pToolCon->Visible = pOldToolCon->Visible;
		delete pOldToolCon;
		plist->RemoveKey(pCreateSheet->GetOldToolID());
	}
	if (plist->Lookup(ToolID, pOldToolCon))
		delete pOldToolCon;
	(*plist)[ToolID] = pToolCon;
	// добавление оправки к нескольким инструментам,
	// кроме текущего
	if (CreateMillPage.m_men_array.GetSize() > 1)
	{
		for (int i = 0; i<pToolCon->num_tools; i++)
			if (40 <= pToolCon->GetTool(i)->GetType() &&
				pToolCon->GetTool(i)->GetType() <= 43)
			{
			NToolMill *pTool = pToolCon->GetTool(i);
			i = pToolCon->num_tools;
			NToolMill *pT = pTool->CreateCopyTool();
			for (int j = 0; j<CreateMillPage.m_men_array.GetSize(); j++)
				if (CreateMillPage.m_men_array[j] != CreateMillPage.m_pos)
				{
				ToolID.Set(CreateMillPage.m_men_array[j], CreateMillPage.m_pos_turret);
				if (!plist->Lookup(ToolID, pOldToolCon))
					continue;
				plist->RemoveKey(ToolID);
				pOldToolCon->AddTool(pT);
				pOldToolCon->Position = ToolID.GetToolNum();
				(*plist)[ToolID] = pOldToolCon;
				}
			}
	}
	pToolCon->SetGroup(OG_TOOLS);
	return ToolID;
}
// создание нового составного токарного инструмента
NToolID CMainFrame::CreateTurnTool(NToolsConCrTurnPage &CreateTurnPage)
{
	NToolCombinedTurn *pToolCon;
	pToolCon = CreateTurnPage.GetNewTool();
	if (pToolCon == nullptr)
		return NToolID();
	NToolID ToolID;
	ToolID.Set(CreateTurnPage.m_pos, CreateTurnPage.m_pos_turret);
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	NMapStringToNTool* plist = pDoc->NGetToolList();
	NToolCombined *pOldToolCon;
	if (plist->Lookup(pCreateSheet->GetOldToolID(), pOldToolCon))
	{
		pToolCon->SetColor(pOldToolCon->Color);
		pToolCon->SetColorPassive(pOldToolCon->ColorPassive);
		pToolCon->SetColorHolder(pOldToolCon->ColorHolder);
		pToolCon->Visible = pOldToolCon->Visible;
		delete pOldToolCon;
		plist->RemoveKey(pCreateSheet->GetOldToolID());
	}
	if (plist->Lookup(ToolID, pOldToolCon))
		delete pOldToolCon;

	(*plist)[ToolID] = pToolCon;
	pToolCon->SetGroup(OG_TOOLS);
	return ToolID;
}

NToolID CMainFrame::CreateWeldTool(NToolsConCrWeldPage &CreateWeldPage)
{
	NToolCombinedWeld *pToolCon;
	pToolCon = CreateWeldPage.GetNewTool();
	if (pToolCon == nullptr)
		return NToolID();
	NToolID ToolID;
	ToolID.Set(CreateWeldPage.m_pos, CreateWeldPage.m_pos_turret);
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	NMapStringToNTool* plist = pDoc->NGetToolList();
	NToolCombined *pOldToolCon;
	if (plist->Lookup(pCreateSheet->GetOldToolID(), pOldToolCon))
	{
		pToolCon->SetColor(pOldToolCon->Color);
		pToolCon->SetColorPassive(pOldToolCon->ColorPassive);
		pToolCon->SetColorHolder(pOldToolCon->ColorHolder);
		pToolCon->Visible = pOldToolCon->Visible;
		delete pOldToolCon;
		plist->RemoveKey(pCreateSheet->GetOldToolID());
	}
	if (plist->Lookup(ToolID, pOldToolCon))
		delete pOldToolCon;

	(*plist)[ToolID] = pToolCon;
	pToolCon->SetGroup(OG_TOOLS);
	return ToolID;
}

void CMainFrame::OnToolsCombo()
{
	if (!CheckCutting())
		return;

	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	if (!pDoc)
		return;
	CString Text;
	switch (m_wndRibbonBar.MOnToolsCombo(Text))
	{
	case -1:
		break;
	case 1:
		PaToolsCr();
		break;
	case 0:
		if (!Text.IsEmpty())
		{
			ChConToolEdit(NToolID().CrFromPrintStr(Text));
		}
		break;
	}
}

void CMainFrame::UpdateRibbonToolsCombo()
{
	if (!IsRibbonCreated())
		return;
	CNCMDoc *pDoc = static_cast<CNCMDoc *>(GetActiveDocument());
	ASSERT_VALID(pDoc);
	std::vector<CString> FullNames;
	pDoc->NGetToolList()->GetFullNames(FullNames);
	m_wndRibbonBar.LoadToolsCombo(FullNames);
}

LRESULT CMainFrame::OnMakeTool(WPARAM wParam, LPARAM lParam)
{
	CNCMDoc* pDoc = static_cast<CNCMDoc*>(GetActiveDocument());
	NMapStringToOb* plist = pDoc->NGetToolList();
	NToolID ToolID;
	if (pCreateMillPage->GetNewTool() != nullptr)
		ToolID = CreateMillTool(*pCreateMillPage);
	else if (pCreateTurnPage->GetNewTool() != nullptr)
		ToolID = CreateTurnTool(*pCreateTurnPage);
	else if (pCreateWeldPage->GetNewTool() != nullptr)
		ToolID = CreateWeldTool(*pCreateWeldPage);
	// Set start tool
	class NCUnit& Unit = *pDoc->GetUnit();
	//
	pDoc->SetModifiedFlag();

	ConToolEdit(ToolID);
	CString Title;
	Title.LoadString(IDS_TOOL_EDIT_TITLE);
	pCreateSheet->SetTitle(Title);
	return LRESULT();
}

LRESULT CMainFrame::OnStopTool(WPARAM wParam, LPARAM lParam)
{
	CNCMDoc* pDoc = static_cast<CNCMDoc*>(GetActiveDocument());
	// Reset Unit state
	class NCUnit& Unit = *pDoc->GetUnit();
	Unit.GetStartState().CurToolN = pCreateSheet->GetStoredToolID();
	Unit.JumpToProjStart();
	Unit.JumpForToHCadr(pCreateSheet->GetStoredCadrID());
	pCreateSheet->GetWindowRect(CreateSheetRect);
	//
	pDoc->UpdateAllViews(nullptr, N_OGL | N_LEFT | N_TOOLCHANGED);
	return LRESULT();
}

void CMainFrame::PaToolsCr()
{
	PrepCreateSheet();
	pCreateSheet->GetOldToolID().Empty();
	StartCreateSheet(IDS_TOOL_CREATE_TITLE);
}

void CMainFrame::OnSetFocus(CWnd* pOldWnd)
{
	SetForegroundWindow(); // Needed for a host mode
	CFrameWndEx::OnSetFocus(pOldWnd);
}

void CMainFrame::ShowMenu(bool Show)
{
	m_wndMenuBar.ShowPane(Show, FALSE, FALSE);
	RedrawWindow();
}

void CMainFrame::SetFixedRotCenter(bool Fixed)
{
	BOpenGLWnd::NRotCenter RotCenter;
	GetRotCenter(RotCenter);
	RotCenter.RotBasePoint.SetH(Fixed ? 1. : -1.);
	SetRotCenter(RotCenter);
}

bool CMainFrame::GetFixedRotCenter()
{
	BOpenGLWnd::NRotCenter RotCenter;
	GetRotCenter(RotCenter);
	return RotCenter.RotBasePoint.IsPoint();
}

void CMainFrame::SerializeOGLPanes(CArchive& ar)
{
	if (ar.IsLoading())
	{
		const CNCMDoc* pDoc = static_cast<CNCMDoc*>(GetActiveDocument());
		ASSERT(pDoc);
		int PanesNum;
		SerializeElements(ar, &PanesNum, 1);
		if (pDoc->GetLoadLayoutProject())
		{
			for (int i = 0; i < PanesNum; ++i)
			{
				NOGLPane* pPane = AddOGLPanes.at(i);
				pPane->SetShowOnPaneMenu(TRUE);
			}
		}
	}
	else
	{
		auto PanesNum = NOGLPane::GetFixedPanesNum();
		for (; PanesNum < AddOGLPanes.size(); ++PanesNum)
		{
			if (!AddOGLPanes.at(PanesNum)->AllowShowOnPaneMenu())
				break;
		}
		SerializeElements(ar, &PanesNum, 1);
	}
}

CRect CMainFrame::GetNewPaneRect()
{
	CRect Large;
	GetWindowRect(Large);
	
	CRect Small(-Large.Width() / 5, -Large.Height() / 5, Large.Width() / 5, Large.Height() / 5);
	Small.OffsetRect(Large.CenterPoint());
	return Small;
}

bool CMainFrame::IsMainViewStock()
{
	if (pMainPane == nullptr)
		return true;
	return pMainPane->GetType();
}

void CMainFrame::GetMainImage(BImageRGB& ImageRGB) const
{
	ImageRGB.Clear();
	if (pMainPane == nullptr)
		return;
	auto pOGLWnd = pMainPane->GetOpenGLWnd();
	if (pOGLWnd == nullptr)
		return;
	pOGLWnd->GetImage(ImageRGB);
}

bool CMainFrame::CanCollisionBeEnabled()
{
	if (!IsCollisionAllowed())
		return false;
	return !IsCuttingInProgress();
}

bool CMainFrame::IsCollisionAllowed()
{
	return (theApp.GetConfig() & CM_CollisionD) != 0;
}

bool CMainFrame::CanMesureDistanceBeEnabled()
{
	const CNCMDoc* pDoc = static_cast<CNCMDoc*>(GetActiveDocument());
	ASSERT(pDoc);
	return !(pDoc->Measure.IsActivated() || pDoc->Dimensions.IsActivated() || pDoc->Comparison.IsActive());
}


void CMainFrame::ProcCutHit()
{
	NTiParams& Par = NCM_PROJECT.Defaults;
	CString Key(_T("Defaults/Demo/StartDialog@Enable"));
	if (!Par.GetBool(Key, true))
		return;

	const BOOL ShowCutHit = theApp.GetProfileInt(NCMVersion, _T("ShowCutHit"), TRUE);
	if (!theApp.IsInCuttingMode() && ShowCutHit)
	{
		const auto Res = AfxMessageBox(IDS_CUTINDEMO, MB_YESNOCANCEL | MB_ICONQUESTION);
		switch (Res)
		{
		case IDYES:
			theApp.OnCutModeRib();
			break;
		case IDCANCEL:
			//theApp.WriteProfileInt(NCMVersion, _T("ShowCutHit"), FALSE);
			break;
		}
	}
}

void CMainFrame::StartView()
{
	((NDummyView*)GetActiveView())->SetPane(pMainPane);
	GetActiveDocument()->UpdateAllViews(nullptr, N_SETTINGS);
}

void CMainFrame::ShowErrorsCaption(BOOL Show)
{
	NTiParams& Par = NCM_PROJECT.Defaults;
	const CString Key(_T("Defaults/Editor/Errors@Caption"));
	if (!Par.GetBool(Key, true))
		Show = FALSE;

	ShowPane(&m_wndErrorsCaption, Show, FALSE, TRUE);
	RecalcLayout();
}

void CMainFrame::OnFileSaveCopyAs()
{
	CFileDialog dlgFile(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL, 0);

	CString fileName;
	CString title;
	title.LoadString(IDS_SAVE_NCMPRT);

	dlgFile.m_ofn.Flags |= OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;

	CString strFilter;
	strFilter = "Prototype: ";
	strFilter += (TCHAR)'\0';
	strFilter += (TCHAR)'*';
	strFilter += _T(".NCMPrt");
	strFilter += (TCHAR)'\0';
	CString strPath = NCM_PROJECT.GetPrototypesPath();
	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);
	dlgFile.m_ofn.lpstrInitialDir = strPath;
	// set the default extension
	CString strDefault = _T("NCMPrt");// skip the '.'
	dlgFile.m_ofn.lpstrDefExt = const_cast<LPTSTR>((LPCTSTR)strDefault);
	dlgFile.m_ofn.nFilterIndex = dlgFile.m_ofn.nMaxCustFilter + 1;  // 1 based number
	dlgFile.m_ofn.nMaxCustFilter++;

	const INT_PTR nResult = dlgFile.DoModal();
	fileName.ReleaseBuffer();
	if (nResult == IDOK)
	{
		CNCMDoc* pDoc = static_cast<CNCMDoc*>(GetActiveDocument());
		ASSERT(pDoc);
		if (!pDoc->DoSave(fileName, FALSE))
			TRACE(traceAppMsg, 0, "Warning: File save copy as failed.\n");
	}

}

void CMainFrame::OnFileSaveAll()
{
	CFileDialog dlgFile(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL, 0);

	CNCMDoc* pDoc = static_cast<CNCMDoc*>(GetActiveDocument());
	ASSERT(pDoc);
	CString fileName = pDoc->GetPathName();
	int Ind = fileName.ReverseFind('.');
	if (Ind > 0)
		fileName.Delete(Ind, fileName.GetLength() - Ind);
	CString title;
	title.LoadString(IDS_SAVE_NCMALL);

	dlgFile.m_ofn.Flags |= OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;

	CString strFilter;
	strFilter = "SaveAll: ";
	strFilter += (TCHAR)'\0';
	strFilter += (TCHAR)'*';
	strFilter += _T(".NCMAll");
	strFilter += (TCHAR)'\0';
	//	CString strPath = NCM_PROJECT.GetPrototypesPath();
	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);
	//	dlgFile.m_ofn.lpstrInitialDir = strPath;
		// set the default extension
	CString strDefault = _T("NCMAll");// skip the '.'
	dlgFile.m_ofn.lpstrDefExt = const_cast<LPTSTR>((LPCTSTR)strDefault);
	dlgFile.m_ofn.nFilterIndex = dlgFile.m_ofn.nMaxCustFilter + 1;  // 1 based number
	dlgFile.m_ofn.nMaxCustFilter++;

	const INT_PTR nResult = dlgFile.DoModal();
	fileName.ReleaseBuffer();
	if (nResult == IDOK)
	{
		if (!pDoc->DoSave(fileName, FALSE))
			TRACE(traceAppMsg, 0, "Warning: File save copy as failed.\n");
	}
}

void CMainFrame::DrawPosDlg() const
{
	CLeftView* pLeft = dynamic_cast<CLeftView*>(GetLeftPane());
	if (pLeft == nullptr)
		return;
	if (!pLeft->IsPosDlgActive())
		return;
	const auto pPosDlg = pLeft->GetPosDlg();
	if (pPosDlg == nullptr)
		return;
	pPosDlg->Draw();
}


#define SMPaneProc(PaneNum)\
void CMainFrame::OnStockMachPane##PaneNum()\
{\
	BOOL Val = !AddOGLPanes.at(##PaneNum - 2)->IsPaneVisible();\
	AddOGLPanes.at(##PaneNum - 2)->ShowPane(Val, FALSE, FALSE);\
}\
void CMainFrame::OnUpdateStockMachPane##PaneNum(CCmdUI *pCmdUI)\
{\
	pCmdUI->SetCheck(AddOGLPanes.at(##PaneNum - 2)->IsPaneVisible());\
}
SMPaneProc(2)
SMPaneProc(3)
SMPaneProc(4)
SMPaneProc(5)
SMPaneProc(6)
SMPaneProc(7)
SMPaneProc(8)
SMPaneProc(9)
SMPaneProc(10)
SMPaneProc(11)
SMPaneProc(12)
SMPaneProc(13)
SMPaneProc(14)
SMPaneProc(15)
SMPaneProc(16)
