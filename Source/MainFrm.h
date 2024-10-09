// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "WM_APP_messages.h"

class CLeftView;
class FAnimateDisp;
class GPauseAnimate;

#include "NBreakpointDlg.h"
//#include "NSpeedDialogBarEx.h"
#include "NTextView.h"
#include "NStateUnitPane.h"
#include "NStateGeomPane.h"
#include "NStateRegsPane.h"
#include "NReportPane.h"
#include "NStateMesurePane.h"
#include "NOGLMashWnd.h"
#include "NOGLStockWnd.h"
#include "NOGLWSectWnd.h"
#include "NOGLPane.h"
#include "NStateAnalysePane.h"
#include "MTEdit.h"
#include "MTEditPane.h"
#include "MTEditTreePane.h"
#include "RemoteCommands.h"
#include "NPSlidePane.h"
#include "NRibbonBar.h"
#include "NSpeedControls.h"
#include "NPropPane.h"//PropEditor

class CMainFrame : public CFrameWndEx
{
	friend class NDummyView;
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
protected:

	class NRemoteManager *pRemoteControl;

public:
	NBreakpointDlg BreakpointDlg;
	int AddEduTest;
protected:
	NTextView m_wndTextView;
	CLeftView &m_wndLeftView;
	NStateUnitPane m_wndUnitPane;
	NStateAnalysePane m_wndAnalysePane;
	MTEditPane m_wndMTEditPane;
	MTEditTreePane m_wndMTEditTreePane;
	NStateGeomPane m_wndGeomPane;
	NStateMesurePane m_wndMesurePane;
	NStateRegsPane m_wndRegsPane;
	NReportPane m_wndReportPane;

	std::vector<NOGLPane*> AddOGLPanes;
	NOGLPane m_wndWSectPane;
	NPSlidePane m_wndPSlidePane;
	NPropPane    m_wndPropPane; //PropEditor
	NOGLPane *pMainPane;
	CRect CreateSheetRect;
	class NToolsConCrSheet* pCreateSheet;
	class NToolsConCrMillPage* pCreateMillPage;
	class NToolsConCrTurnPage* pCreateTurnPage;
	class NToolsConCrWeldPage* pCreateWeldPage;
	bool ImportMode;
	class NImportDlg *pImportDlg;
	class NSetupSetBoxPointDlg *pSetBoxPointDlg;
	class NSetupSetUCSOnBox *pSetUCSOnBoxDlg;
	class NSetupWriteCSDlg *pWriteCSDlg;
	class NRotCenterDlg *pRotCenterDlg;
	class QDivideDlg* pDivideDlg;
	int MaxRunStepsNum;
	class DDimEntCrSheet *pDimEntCrSheet;
    class CrStockTemp *pDimStockCrDlg;
	CMFCCaptionBar m_wndErrorsCaption;
	// Operations
public:
	void OnChangeLook(BOOL bOneNoteTabs, BOOL bMDITabColors, BOOL bIsVSDotNetLook, BOOL bDockTabColors, BOOL bMDITabsVS2005Look, BOOL bActiveTabCloseButton);
	void CloseDimDialogs();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
//	virtual void RecalcLayout(BOOL bNotify = TRUE);
	//}}AFX_VIRTUAL

// Implementation
public:
	void UpdateLeftView(CWnd* pSender, LPARAM lHint, CObject* pHint);
	CWnd * GetUnitPane();
	CWnd * GetAnalysePane();
	CWnd * GetMTEditPane();
	CWnd * GetMTEditTreePane();
	CWnd * GetGeomPane();
	CWnd * GetMesurePane();
	CWnd * GetRegsPane();
	CWnd * GetReportPane();
	CWnd * GetTextView();
	CWnd * GetMainPane();
	void UpdateOGLPanes(CDockablePane* pSender, LPARAM lHint, CObject* pHint);
	void DeleteAddOGLPanes();
	CWnd * GetLeftPane() const;
	CWnd * GetPSlidePane();
	CWnd * GetPropPane();

	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  
	CMFCMenuBar m_wndMenuBar;
	NRibbonBar m_wndRibbonBar;
	CMFCToolBar	m_viewToolBar;
	CMFCStatusBar m_wndStatusBar;
	CMFCToolBar	m_wndToolBar;
	CMFCToolBar	m_modeToolBar;
	CMFCToolBar m_navigToolBar;
	CMFCToolBar m_collisionsToolBar;
	CMFCToolBar m_NBreakpointToolBar;
	CMFCToolBar m_inspectToolBar;
	CMFCToolBar m_indicatorsToolBar;
	CMFCToolBar m_windowToolBar;
	NSpeedControls m_wndDlgBar;

	bool PauseAnimBack;
	bool PauseAnimFor;
	bool PauseAnimatePause;
	int PauseAnimStep;
	UINT_PTR AnimateTimerID;
	UINT_PTR PauseAnimateTimerID;
	UINT_PTR ResultTimerID;
	UINT_PTR RunAnimateTimerID;
	std::pair<MainTime, MainTime> AnimateTimeInterval;
	bool AnimatePause;
	GPauseAnimate *pPauseAnimate;
	bool AnimateText;
	bool ReRenderText;
	int TimeStepRepeat = 5;
	_int16 flags;
	DECLARE_MESSAGE_MAP()
public:
	void StartAnimateTimer(void);
	void StartResultTimer(void);
	void StartRunAnimateTimer(void);
	void StopRunAnimate();
	void AnimateStep(void);
	void AnimateStepSingle(void);
	void StartAppropTimer();
	void PauseAnimateStep(void);
	void ResultStep(void);
	void ShowPauseDlg(void);
	void ShowResultDlg(void);
	void ShowPauseFDlg(void);
	bool IsAnimatePaused(void);
	bool CanAnimateBePaused(void);
	bool IsCuttingInProgress(void);
	bool IsAnyTimerActive(void);
	bool IsComparisonActive(void);
	void LoadTBFromIcons(void);
	void PlaceToolBars(void);
	void PlaceViewPanes(void);
	//void SaveLayout(void);
	//void LoadLayout(void);
	void SetMainPane(void);
	bool SetMainLayout();
	BOOL IsLargeIcons(void);
	int GetPauseAnimStep();
	int StartStopPauseAnim(bool forward, int step);
	bool IsPauseAnimActive();
	bool GetPauseAnimBack();
	bool GetPauseAnimFor();
	void RemovePauseAnim(void);
	bool IsPauseAnimatePause();
	void SetDockingWindowIcons(void);
	void SetModeImage(void);
	void SetChangeViewImage(void);
	void ChangeWin(void);
	void AnimateUpdate();
public:
	void TestStep(void);
	void SerializeMatrices(CArchive &ar);
	bool CheckCutting(void);
	bool CheckHosting(void);
	virtual void ActivateFrame(int nCmdShow = -1);
//	int CutProgFragments(RunStepsStopReason StopReason);
	void SetFlags(void);
	bool CreateToolBar(CMFCToolBar &ToolBar, int ToolBarID);
	NOpenGLWnd * GetViewToHandle(void);
	void SetCollisionIcon(bool Enable, bool Found);
	void SetCollisionAnim(bool Enable, bool Found);
	bool IsRunAnimateActive();
	void UpdateVolumeParams(void);
	void SyncLayoutWConfig(NCMConfig Config);
	class NStock * GetActiveStock();
	void ForcePause();
public:
	afx_msg void OnReportsEdit();
	afx_msg void OnUpdateCollIcon(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCoord(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCadr(CCmdUI *pCmdUI);
	afx_msg void OnAnimatePause();
	afx_msg void OnUpdateAnimatePause(CCmdUI *pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnStepFor();
	afx_msg void OnClose();
	afx_msg void OnJumpBack();
	afx_msg void OnOglConfig();
	afx_msg void OnRunHere();
	afx_msg void OnRunHereResult();
	afx_msg void OnStepTimer();
	void StepStop();
	afx_msg void OnStepStop();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
//	afx_msg void OnUpdateSaveEnvironmentStud(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFastCut5x(CCmdUI* pCmdUI);
	bool CanFastCut5xBeEnabled();
	afx_msg void OnUpdateCutMode(CCmdUI* pCmdUI);
	bool CanCutModeBeEnabled();
	afx_msg void OnUpdateStepRun(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStepHere(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStepFor(CCmdUI *pCmdUI);
	afx_msg void OnRunToBreakpoint();
	void RunToBreakpoint();
	afx_msg void OnRunCollCheck();
	afx_msg void OnReset();
	afx_msg void OnUpdateAutoUpdateProg(CCmdUI *pCmdUI);
	afx_msg void OnAutoUpdateProg();
	afx_msg void OnUpdateRunToBreakpoint(CCmdUI *pCmdUI);
	bool CanRunToBreakpointBeEnabled();
	afx_msg void OnStockInit();
	afx_msg void OnUpdateStepStop(CCmdUI *pCmdUI);
	bool CanStepStopBeEnabled();
	afx_msg void OnUpdateStepBack(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStockInit(CCmdUI *pCmdUI);
	afx_msg void OnUpdateReset(CCmdUI *pCmdUI);
	bool CanResetBeEnabled();
	afx_msg void OnBreakpointForward();
	afx_msg void OnUpdateBreakpointForward(CCmdUI *pCmdUI);
	afx_msg void OnBreakpointTable();
	afx_msg void OnUpdateBreakpointTable(CCmdUI *pCmdUI);
	afx_msg void OnBreakpointBack();
	afx_msg void OnUpdateBreakpointBack(CCmdUI *pCmdUI);
	afx_msg void OnMBreakpointaddOnOff();
	afx_msg void OnUpdateMBreakpointaddOnOff(CCmdUI *pCmdUI);
	afx_msg void OnBp1();
	afx_msg void OnUpdateBp1(CCmdUI *pCmdUI);
	afx_msg void OnBp2();
	afx_msg void OnUpdateBp2(CCmdUI *pCmdUI);
	afx_msg void OnBp3();
	afx_msg void OnUpdateBp3(CCmdUI *pCmdUI);
	afx_msg void OnBp4();
	afx_msg void OnUpdateBp4(CCmdUI *pCmdUI);
	afx_msg void OnBp5();
	afx_msg void OnUpdateBp5(CCmdUI *pCmdUI);
	afx_msg void OnBp6();
	afx_msg void OnUpdateBp6(CCmdUI *pCmdUI);
	afx_msg void OnStartCollcheck();
	afx_msg void OnUpdateStartCollcheck(CCmdUI *pCmdUI);
	afx_msg void OnMeasureDistance();
	afx_msg void OnUpdateMeasureDistance(CCmdUI *pCmdUI);
	afx_msg void OnStopOnColl();
	afx_msg void OnUpdateStopOnColl(CCmdUI *pCmdUI);
	afx_msg void OnStatusCadr();
	afx_msg void OnStepNum();
	afx_msg void OnPauseNum();
	afx_msg void OnStatusCoord();
	afx_msg void OnMenuReportsClear();
	afx_msg void OnMenuReportsView();
	afx_msg void OnUpdateMenuReportsView(CCmdUI *pCmdUI);
	afx_msg void OnMenuReportsSelect();
	afx_msg void OnUpdateMenuReportsSelect(CCmdUI *pCmdUI);
	afx_msg void OnWinMach();
	afx_msg void OnWinStock();
	afx_msg void OnUpdateWinStock(CCmdUI *pCmdUI);
	afx_msg LRESULT OnCuttingStopped(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCutStateChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPSlide(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetAbsObj(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGridEvalexp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWideEvalexp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCollcheckStopped(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCollcheckAddlog(WPARAM wParam, LPARAM lParam);// wParam: 0 - clear report pane, 1 - add element, 2 - add whole log
	afx_msg LRESULT OnUnitInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUnique(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMakeTool(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnStopTool(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdatePanes(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRegenerate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnStockVisibleChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMacroApply(WPARAM wParam, LPARAM lParam);
	afx_msg void OnViewDefault();
	afx_msg void OnUpdateWinMach(CCmdUI *pCmdUI);
	afx_msg void OnTest();
	afx_msg void OnUpdateStockState(CCmdUI *pCmdUI);
	afx_msg void OnStockState();
//	afx_msg void OnNMReleasedcaptureSld(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT On3DxInput(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCheckMi();
	afx_msg void OnUpdateCheckMi(CCmdUI *pCmdUI);
	//afx_msg void OnNMReleasedcaptureWfrSlider(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnNMReleasedcaptureRerendSld(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnNMReleasedcaptureAnimSld(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnNMReleasedcaptureAnimFSld(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDispExecute();
	afx_msg void OnUpdateDispExecute(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStateTB0(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStateTB1(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStateTB2(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStateTB3(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStateTB4(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStateTB5(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStateTB6(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStateTB7(CCmdUI *pCmdUI);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnViewCustomize();
	afx_msg void OnPauseBack();
	afx_msg void OnPauseForward();
	afx_msg void OnMeasure();
	afx_msg void OnUpdateMeasure(CCmdUI *pCmdUI);
	afx_msg void OnPaStockSecView();
	afx_msg void OnPaMachEditEnd();
	afx_msg void OnViewSection();
	afx_msg void OnRotatecenteronoff();
	afx_msg void OnTbUndo();
	afx_msg void OnTbRedo();
	afx_msg void OnUpdateRotatecenteronoff(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTbUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTbRedo(CCmdUI* pCmdUI);
	afx_msg void OnViewRegen(void);
	afx_msg void OnInspectToolpathstatistics();
	afx_msg void OnCollsettings();
	afx_msg void OnUpdateCollsettings(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewRegen(CCmdUI *pCmdUI);
	afx_msg void OnParticular();
	afx_msg void OnRemCont();
	afx_msg void OnUpdateRemCont(CCmdUI *pCmdUI);
	afx_msg void OnComparison();
	afx_msg void OnCreateEnt();
	afx_msg void OnUpdateCreateEnt(CCmdUI *pCmdUI);
	afx_msg void OnUpdateComparison(CCmdUI *pCmdUI);
	afx_msg void OnChStockDivide();
	afx_msg void OnJumpOver();
	afx_msg LRESULT OnStopImpMode(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUpdateViewSection(CCmdUI *pCmdUI);
	afx_msg void OnSetupAutops();
	afx_msg void OnSetupWrite0Cs();
	afx_msg void OnSetupSetboxpoint();
	afx_msg void OnUpdateSetupWrite0Cs(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSetupSetboxpoint(CCmdUI *pCmdUI);
	afx_msg void OnSetupSetUCSOnbox();
	afx_msg void OnUpdateSetupSetUCSOnbox(CCmdUI *pCmdUI);
	afx_msg void OnUpdateRotCenter(CCmdUI *pCmdUI);
	afx_msg void OnRotCenter();
//	afx_msg void OnDemoComment();
	afx_msg void OnHelp();
	afx_msg void OnStyleRibboninterface();
	afx_msg void OnRbnBtnToolbarInterface();
	afx_msg void OnRibbonSlider();
	afx_msg void OnMachCombo();
	afx_msg void OnNCUnitCombo();
	afx_msg void OnToolsCombo();
	afx_msg void OnOWindows();
	afx_msg void OnStockPaneRib();
	afx_msg void OnMachPaneRib();
	afx_msg void OnUpdateStockPaneRib(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMachPaneRib(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTextPaneRib(CCmdUI *pCmdUI);
	afx_msg void OnTextPaneRib();
	afx_msg void OnUpdateLeftPaneRib(CCmdUI *pCmdUI);
	afx_msg void OnLeftPaneRib();
	afx_msg void OnUpdateUnitPaneRib(CCmdUI *pCmdUI);
	afx_msg void OnUnitPaneRib();
	afx_msg void OnUpdateAnalysePaneRib(CCmdUI *pCmdUI);
	afx_msg void OnAnalysePaneRib();
	afx_msg void OnUpdateGeomPaneRib(CCmdUI *pCmdUI);
	afx_msg void OnGeomPaneRib();
	afx_msg void OnUpdateMesurePaneRib(CCmdUI *pCmdUI);
	afx_msg void OnMesurePaneRib();
	afx_msg void OnUpdateReportPaneRib(CCmdUI *pCmdUI);
	afx_msg void OnReportPaneRib();
	afx_msg void OnUpdateRegsPaneRib(CCmdUI *pCmdUI);
	afx_msg void OnRegsPaneRib();
	afx_msg void OnUpdateSlidePaneRib(CCmdUI *pCmdUI);
	afx_msg void OnSlidePaneRib();
	afx_msg void OnUpdatePropertiesPaneRib(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDeleteChannel(CCmdUI *pCmdUI);
	afx_msg void OnPropertiesPaneRib();
	afx_msg void OnAddpaneDelall();
	afx_msg void OnSetNextChannel();
	afx_msg void OnNewChannel();
	afx_msg void OnDeleteChannel();
	afx_msg void OnStockCr();
	afx_msg void OnNewProject();
	afx_msg void OnRibSave();
	afx_msg void OnStockEd();
	afx_msg void OnUpdateStockEd(CCmdUI *pCmdUI);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnDestroy();
	afx_msg void OnFileSaveCopyAs();
	afx_msg void OnFileSaveAll();
#define SMPaneAfx(PaneNum)\
	afx_msg void OnStockMachPane##PaneNum();\
	afx_msg void OnUpdateStockMachPane##PaneNum(CCmdUI* pCmdUI);
	SMPaneAfx(2);
	SMPaneAfx(3);
	SMPaneAfx(4);
	SMPaneAfx(5);
	SMPaneAfx(6);
	SMPaneAfx(7);
	SMPaneAfx(8);
	SMPaneAfx(9);
	SMPaneAfx(10);
	SMPaneAfx(11);
	SMPaneAfx(12);
	SMPaneAfx(13);
	SMPaneAfx(14);
	SMPaneAfx(15);
	SMPaneAfx(16);
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void PostNcDestroy();
	virtual void GetMessageString(UINT nID, CString& rMessage) const;
	static bool Init3Dx(void);
	static bool UnInit3Dx(void);
	int GetStepNum();
	void RunAnimateStep();
	void ProcCutHit();
	void SetDefaultManager();
public:
	void StartView();
	void ShowErrorsCaption(BOOL Show);
	void AppTypeChanged(NCMApplicationType NewType);
	bool SetRemote(CWnd *pWnd);
	bool IsRemoteControlActive();
	void ClearRemote();
	bool ChangeMainMenu(int NewMenuID, int NewRibbonID);
	class NRemoteManager *GetRemoteControl(void) { return pRemoteControl;}
	void RC_SendPoint(const BPoint &P, const BPoint &N);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	void ShowPropPane(void);
	void ShowTextView(void);
	void Send2PropPane(PAbstractObject *pPObj);
	void StartImportMode(void);
	bool IsInImportMode(void) const { return ImportMode;}
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	void WriteCS_OK(int Ucs);
	void SetRotCenter(const BOpenGLWnd::NRotCenter &RotCenter);
	void GetRotCenter(BOpenGLWnd::NRotCenter& RotCenter) const;
//	const CString & MakeComment(CString &Str);
	void ClearDimEntCrSheet(void);
    void UpdateDialogData();
	void ApplyBoxDialogs(void);
	void ResetMeasureStock(void);
	void InitMeasure(void);
    void CreateDimStock();
	bool IsCrDimStockVisible();
	BOOL PreTranslateMessage(MSG* pMsg);
	bool IsRibbonCreated() { return IsWindow(m_wndRibbonBar.m_hWnd) == TRUE; }
	void ForceRedraw();
	void SetNCUnitName(const CString &Name);
	void SetMachName(const CString &Name);
	void FocusSetText();
	void FocusKillText();
	void RunNcunitStartDlg(int StartPage);
	void ChConToolEdit(const NToolID& ToolConID);
	void StartCreateSheet(UINT nID);
	void PrepCreateSheet();
	void CloseCreateSheet();
	void ConToolEdit(const NToolID& ToolConID);
	NToolID CreateMillTool(class NToolsConCrMillPage &CreateMillPage);
	NToolID CreateTurnTool(class NToolsConCrTurnPage &CreateTurnPage);
	NToolID CreateWeldTool(class NToolsConCrWeldPage &CreateWeldPage);
	void UpdateRibbonToolsCombo();
	void PaToolsCr();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	void ShowMenu(bool Show);
	void SetFixedRotCenter(bool Fixed);
	bool GetFixedRotCenter();
	void SerializeOGLPanes(CArchive& ar);
	CRect GetNewPaneRect();
	bool IsMainViewStock();
	void GetMainImage(BImageRGB& ImageRGB) const;
	bool CanCollisionBeEnabled();
	bool IsCollisionAllowed();
	bool CanMesureDistanceBeEnabled();
	void DrawPosDlg() const;
	bool SetCurChannel(int i);
};
