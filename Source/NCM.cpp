// NCM.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
//#include "VLD.h" //  Visual Leak Detector (Version 0.9i)

#include "MainFrm.h"
#include "Splasher.h"
#include "NColor.h"
#include "NFont.h"
#include "NSurfSettings.h"
#include "NCMDoc.h"
#include "NCUnit.h"
#include "NProgram.h"
#include "NSettingsDlg.h"
#include "NCCommandLineInfo.h"
#include "LimitSingleInstance.h"
#include "TDialog.h"
#include "BConst.h"
#include "NCMbConst.h"
#include "UnitSConst.h"
#include "BSP_ModelConst.h"
#include "SurfaceConst.h"
#include "NCToolConst.h"
#include "UnitConst.h"
#include "NCMConst.h"
#include "BDebugState.h"
#include "GQuadRender.h"
#include "GAnimateCadr.h"
#include "AppLookDlg.h"
#include "NDummyView.h"
//#include "..\GUARD\Include\GrdAPI.h"
#include "..\GUARD\Lib\GrdDongle.h"
#include "GuDongleModule.h"
#include "NCMVersion.h"
#include "NNetConfigDlg.h"
#include "NDemoSettingsDlg.h"
#include "ResLocal.h"
#include "LanguageDlg.h"
#include "GQuadRenderDX5X.h"
#include "GQuadRenderRot.h"
#include "LeftView.h"
#include "NCM.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef _TIME
	int LastMonth = 4;
#endif
	//Our simple derived Splash screen class, which just uses the bitmap resource IDB_SPLASH
	class CMySplashWnd : public CSplashWnd
	{
	public:
		//Constructors / Destructors
#pragma warning(suppress: 26455)
		CMySplashWnd()
		{
			//Lets use an almost opaque splash window with some "cut-out" sections in it
			HMODULE hins = GetModuleHandle(_T("ResInv.dll"));

			SetBitmapToUse(IDB_START_PIC, hins);
			SetDropShadow();
			SetTransparent(RGB(237,28, 36), LWA_COLORKEY, 125);
		}

		DECLARE_DYNCREATE(CMySplashWnd);
	};

#pragma warning(suppress: 26409 26433 26440 26477)
	IMPLEMENT_DYNCREATE(CMySplashWnd, CSplashWnd);


/////////////////////////////////////////////////////////////////////////////
// The one and only CNCMApp object
CNCMApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CNCMApp

BEGIN_MESSAGE_MAP(CNCMApp, CWinAppEx)
	//{{AFX_MSG_MAP(CNCMApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_SETTINGS, OnSettings)
	ON_COMMAND(ID_SETTINGS_APPEARANCE, OnSettingsAppearance)
	ON_COMMAND(ID_SETTINGS_CONTROL_PANEL, OnSettingsControlPanel)
	ON_COMMAND(ID_SETTINGS_GRAPHICS, OnSettingsGraphics)
	ON_COMMAND(ID_GLOBAL_SETTINGS_SAVE, OnGlobalSettingsSave)
	ON_COMMAND(ID_SETTINGS_SAVE, OnSettingsSave)
	ON_COMMAND(ID_SETTINGS_LOAD, OnSettingsLoad)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	//ON_COMMAND(ID_FILE_NEW, OnFileNew)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinAppEx::OnFilePrintSetup)
	ON_COMMAND(ID_LANGUAGE_ENGLISH, OnLanguageEnglish)
	ON_UPDATE_COMMAND_UI(ID_LANGUAGE_KOREAN, OnUpdateLanguageKorean)
	ON_COMMAND(ID_LANGUAGE_KOREAN, OnLanguageKorean)
	ON_UPDATE_COMMAND_UI(ID_LANGUAGE_ENGLISH, OnUpdateLanguageEnglish)
	ON_COMMAND(ID_LANGUAGE_RUSSIAN, OnLanguageRussian)
	ON_UPDATE_COMMAND_UI(ID_LANGUAGE_RUSSIAN, OnUpdateLanguageRussian)
	ON_COMMAND(ID_DEBUGMODE, OnDebugmode)
	ON_COMMAND(ID_CUTFASTONOFF5X, OnCutfastonoff5x)
    ON_COMMAND(ID_CUTONOFF, OnCutonoff)
	ON_COMMAND(ID_RESULT_MODE, OnResultMode)
	ON_UPDATE_COMMAND_UI(ID_RESULT_MODE, OnUpdateResultMode)
	ON_COMMAND(ID_SMOTHNESS_MODE_F, OnSmothnessModeF)
	ON_UPDATE_COMMAND_UI(ID_SMOTHNESS_MODE_F, OnUpdateSmothnessModeF)
	ON_COMMAND(ID_SMOTHNESS_MODE, OnSmothnessMode)
	ON_UPDATE_COMMAND_UI(ID_SMOTHNESS_MODE, OnUpdateSmothnessMode)
	ON_COMMAND(ID_SMOTHNESS_MODE_RIB, OnSmothnessModeRib)
	ON_UPDATE_COMMAND_UI(ID_SMOTHNESS_MODE_RIB, OnUpdateSmothnessModeRib)
	ON_COMMAND(ID_USE_FEED, OnUseFeed)
	ON_UPDATE_COMMAND_UI(ID_USE_FEED, OnUpdateUseFeed)
	ON_COMMAND(ID_LAYOUT_PROJECT, OnLayoutProject)
	ON_UPDATE_COMMAND_UI(ID_LAYOUT_PROJECT, OnUpdateLayoutProject)
	ON_COMMAND(ID_FILE_NEW_MILL, OnFileNewMill)
	ON_COMMAND(ID_FILE_NEW_NCMPRT, OnFileNewNcmprt)
	ON_COMMAND(ID_FILE_NEW_TURN, OnFileNewTurn)
	ON_COMMAND(ID_FILE_NEW_MILLTURN, OnFileNewMillturn)
	ON_COMMAND(ID_FILE_NEW_MILL_RIB, OnFileNewMillRib)
	ON_COMMAND(ID_FILE_NEW_TURN_RIB, OnFileNewTurnRib)
	ON_COMMAND(ID_FILE_NEW_NCMPRT_RIB, OnFileNewNcmprtRib)
	ON_COMMAND(ID_FILE_NEW_MILLTURN_RIB, OnFileNewMillturnRib)
	ON_COMMAND(ID_FILE_REOPEN, OnFileReopen)
	ON_COMMAND(ID_UNIT_MM, OnUnitMm)
	ON_COMMAND(ID_UNIT_INCH, OnUnitInch)
	ON_UPDATE_COMMAND_UI(ID_UNIT_MM, OnUpdateUnitMm)
	ON_UPDATE_COMMAND_UI(ID_UNIT_INCH, OnUpdateUnitInch)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_OPEN_RIB, OnFileOpenRib)
	ON_COMMAND(ID_VIEW_APP_LOOK, OnViewAppLook)
	ON_COMMAND(ID_CHANGE_CUT_MODE, &CNCMApp::OnChangeCutMode)
	ON_UPDATE_COMMAND_UI(ID_CHANGE_CUT_MODE, &CNCMApp::OnUpdateChangeCutMode)
	ON_COMMAND(ID_CUT_MODE, &CNCMApp::OnCutMode)
	ON_COMMAND(ID_FAST_CUT_MODE5X, &CNCMApp::OnFastCutMode5X)
	ON_COMMAND(ID_DONTCUT_MODE, &CNCMApp::OnDontcutMode)
	ON_UPDATE_COMMAND_UI(ID_DONTCUT_MODE, &CNCMApp::OnUpdateDontcutMode)
	ON_COMMAND(ID_CUT_MODE_RIB, &CNCMApp::OnCutModeRib)
	ON_UPDATE_COMMAND_UI(ID_CUT_MODE_RIB, &CNCMApp::OnUpdateCutModeRib)
	ON_COMMAND(ID_USE_DX, &CNCMApp::OnUseDX)
	ON_UPDATE_COMMAND_UI(ID_USE_DX, &CNCMApp::OnUpdateUseDX)
	ON_COMMAND(ID_FILE_NEW_WELD, &CNCMApp::OnFileNewWeld)
	ON_COMMAND(ID_CHANGE_WIN, &CNCMApp::OnChangeWin)
	ON_UPDATE_COMMAND_UI(ID_CHANGE_WIN, &CNCMApp::OnUpdateChangeWin)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
#ifdef NCM_QUICK
	enum { IDD = IDD_ABOUTBOX_QUICK };
#endif
#ifndef NCM_QUICK
	enum { IDD = IDD_ABOUTBOX };
#endif
	CMFCLinkCtrl	m_HyperLink;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INITeng
}
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHLSampleDlg)
	DDX_Control(pDX, IDC_HYPERLINK, m_HyperLink);
	//}}AFX_DATA_MAP
}



BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CNCMApp construction

// The one and only CLimitSingleInstance object
CLimitSingleInstance g_SingleInstanceObj(TEXT("NCManager"));

CNCMApp::CNCMApp()
{
	// Place all significant initialization in InitInstance
	EnableHtmlHelp();
	Configuration = 0xFFFF;
	NCMComData::SetType(AT_PUNCH);
	RemoveState = false;
	AllowLoadRibbonState = true;
}

void myInvalidParameterHandler(const wchar_t* expression,
	const wchar_t* function,
	const wchar_t* file,
	unsigned int line,
	uintptr_t pReserved)
{
	wprintf(L"Invalid parameter detected in function %s."
		L" File: %s Line: %d\n", function, file, line);
	wprintf(L"Expression: %s\n", expression);
	abort();
}
/////////////////////////////////////////////////////////////////////////////
// CNCMApp initialization
BOOL CNCMApp::InitInstance()
{
	//_crtBreakAlloc = 984770;
	//_set_invalid_parameter_handler(myInvalidParameterHandler);
	AfxLoadLibrary(_T("ResInv.DLL"));
	/////////////////// ADDED TO SUPPORT THE SPLASH SCREEN//////////////////////
	CSplashFactory factory;
	if (!factory.Create(RUNTIME_CLASS(CMySplashWnd)))
	{
		AfxMessageBox(_T("Failed to create splash screen"), MB_OK | MB_ICONSTOP);
//		return FALSE;
	}
	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	//AfxEnableControlContainer();

	//EnableTaskbarInteraction(FALSE);

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
	InitContextMenuManager();
	InitShellManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	free((void*)m_pszAppName); 
	m_pszAppName=_tcsdup(_T("NCManager"));

	// Change the registry key under which our settings are stored.
	SetRegistryKey(_T("Inteks"));
	theApp.WriteProfileString(NCMVersion, _T("WindowClassName"), "");

	bool RegistryError = false;
	HKEY hkResult;
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\NCM40.EXE"), 0, KEY_QUERY_VALUE, &hkResult) 
		!= ERROR_SUCCESS)
	{
		if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\NCM38.EXE"), 0, KEY_QUERY_VALUE, &hkResult) 
			!= ERROR_SUCCESS)
		{
			RegistryError = true;
		}
	}

	unsigned char buf[256];
	unsigned long s = 256;
	if(RegQueryValueEx(hkResult, _T("Path"), NULL, NULL, buf, &s)
		!= ERROR_SUCCESS)
	{
		RegistryError = true;
	}
	WriteProfileString(NCMVersion, _T("MainPath"), CString(buf));
	s = 256;
	if (RegQueryValueEx(hkResult, _T("DataPath"), NULL, NULL, buf, &s)
		!= ERROR_SUCCESS)
	{
		RegistryError = true;
	}
	
	WriteProfileString(NCMVersion, _T("DataPath"), CString(buf));

	// Set interface type
	CString IntType = GetProfileString(_T(""), _T("InterfaceStyle"), _T("Menu"));
	NCM_PROJECT.GetGlobalState().SetRibbonInterface(IntType == _T("Ribbon"));

	// Set viewpoint interface rules
	CString ViewIntType = GetProfileString(_T(""), _T("ViewInterfaceStyle"), _T("NCManager"));
	NCM_PROJECT.GetGlobalState().SetViewInterface(ViewIntType);
	NCM_PROJECT.GetGlobalState().SetWheelReverse(GetProfileInt(_T(""), _T("WheelReverse"), FALSE));

// Set language
//	DWORD lang = GetProfileInt(NCMVersion, "Language", MAKELCID(MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),SORT_DEFAULT));
	NCMLanguage NCMLang = NCMLanguage(GetProfileInt(NCMVersion, "NCMLanguage", NL_UND));
	LCID DefLang = GetThreadLocale();
	if (NCMLang == NL_UND)
	{// Language is not defined
		if(DefLang == MAKELCID(MAKELANGID(LANG_RUSSIAN,SUBLANG_DEFAULT),SORT_DEFAULT))
			NCMLang = NL_RUS;
		else
		{
			LanguageDlg Dlg;
			Dlg.DoModal();
			switch(Dlg.Lang)
			{
			case 0:
				NCMLang = NL_ENG;
				break;
			case 1:
				NCMLang = NL_RUS;
				break;
			case 2:
				NCMLang = NL_KOR;
				break;
			default:
				NCMLang = NL_ENG;
				break;
			}
		}
		WriteProfileInt(NCMVersion, "NCMLanguage", NCMLang);
	}

// Load resources
	switch(NCMLang)
	{
	case NL_RUS:
		if (DefLang != MAKELCID(MAKELANGID(LANG_RUSSIAN, SUBLANG_DEFAULT), SORT_DEFAULT))
			SetThreadLocale(MAKELCID(MAKELANGID(LANG_RUSSIAN, SUBLANG_DEFAULT), SORT_DEFAULT));
//		SetThreadUILanguage(MAKELCID(MAKELANGID(LANG_RUSSIAN, SUBLANG_DEFAULT), SORT_DEFAULT));
		AfxLoadLibrary(_T("local\\Rus\\Resources.DLL"));
		NCM_PROJECT.ErrConfFile = CString(_T("ErrConfMesRu.txt"));
		break;
	case NL_KOR:
		AfxLoadLibrary(_T("local\\Kor\\Resources.DLL"));
		NCM_PROJECT.ErrConfFile = CString(_T("ErrConfMesCo.txt"));
		break;
	case NL_ENG:
	default:
		AfxLoadLibrary(_T("local\\Eng\\Resources.DLL"));
		NCM_PROJECT.ErrConfFile = CString(_T("ErrConfMesEn.txt"));
		break;
	}
	if(RegistryError)
	{
			AfxMessageBox(IDS_SETUP_FAULT);
			exit(1);
	}
// Set help file name
	CString ExeName = m_pszExeName;
	CString HelpFileName;
	HelpFileName.LoadString(NCM_PROJECT.GetGlobalState().GetRibbonInterface() ? IDS_HELP_NAME_RIBBON : IDS_HELP_NAME);
	auto bbb = NCM_PROJECT.GetGlobalState().GetRibbonInterface();
	CString strHelpFile = m_pszHelpFilePath;
	strHelpFile = strHelpFile.Left(strHelpFile.GetLength() - ExeName.GetLength() - 4) + HelpFileName;// -4 means ".CHM" length
	free((void*)m_pszHelpFilePath);
	m_pszHelpFilePath = _tcsdup(strHelpFile);
	//CFileStatus FileStatus;
	//if (!CFile::GetStatus(m_pszHelpFilePath, FileStatus))
	//{
	//	CString Message("Help file not found:\r\n");
	//	AfxMessageBox(Message+m_pszHelpFilePath);
	//}



	// Model mode
	GetGlobalState().SetModelMode(NGlobalState::ModelModes(GetProfileInt(NCMVersion, _T("ModelMode"), NGlobalState::BSP)));
	// Mesurement units
	int m = GetProfileInt(NCMVersion, _T("MmInch"), -1);
	if(m < 0)
	{
		if(AfxMessageBox(IDS_UNIT_SETUP, MB_YESNO) == IDYES)
			BConst::State = M_MM;
		else
			BConst::State = M_INCH;
		WriteProfileInt(NCMVersion, _T("MmInch"), BConst::State);
	}
	else
			BConst::State = NMetrics(m);

	if(BConst::State == M_INCH)
	{// Scale length constants
		double s = BConst::GetScale();
		NCMbConst::Scale(s);
		UnitSConst::Scale(s);
		BSP_ModelConst::Scale(s);
		NCToolConst::Scale(s);
		SurfaceConst::Scale(s);
		NCMConst::Scale(s);
		UnitConst::Scale(s);
	}

	CString filename;
	filename.LoadString(IDS_DEFIN_FILENAME);
	NCM_PROJECT.LoadDefaults(filename);
	//filename.LoadString(IDS_DEFPARAMETERS_FILENAME);
	//name = NCM_PROJECT.GetSettingsPath() + filename;
	//bool loadOkayEnv = NCM_PROJECT.EnvParams.LoadFile(name.GetBuffer());
#ifdef NC_DEMO
	{
		NTiParams& LocPar = NCM_PROJECT.Defaults;
		CString Key(_T("Defaults/Demo/StartDialog@Enable"));
		if (LocPar.GetBool(Key, true))
		{
			NDemoSettingsDlg Dlg;
			if (Dlg.DoModal() != IDOK)
			{
				filename = _T("Safe") + filename;
				CString name = NCM_PROJECT.GetSettingsPath() + filename;
				NCM_PROJECT.Defaults.LoadFile(name.GetBuffer());
			}
		}
	}
#endif
	//	if(!loadOkay)
//		AfxMessageBox(IDS_SET_FOPEN);

	// Parse command line for standard shell commands, DDE, file open
	NCCommandLineInfo cmdInfo;
	// Remove next line to ignore command string parameters
	ParseCommandLine(cmdInfo);
	//if (g_SingleInstanceObj.IsAnotherInstanceRunning())
	//{ /* активизируем окно работающего экземпляра */

 //       HWND hOther = NULL;
 //       EnumWindows(searcher, (LPARAM)&hOther);

 //       if ( hOther != NULL )
 //       { /* помещаем окно поверх других */
	//		::SetForegroundWindow( hOther );
 //           if ( IsIconic( hOther ) )
 //           { /* разворачиваем окно */
 //               ::ShowWindow( hOther, SW_RESTORE );
 //           } 
 //			if(cmdInfo.m_nShellCommand == CCommandLineInfo::FileOpen)
	//		{/* сохраняем имя для использования в OnFileReopen */
	//			WriteProfileString(NCMVersion, _T("ReopenFileName"), cmdInfo.m_strFileName);
	//			::PostMessage( hOther, WM_COMMAND, ID_FILE_REOPEN, 0);
	//		}
 //       } 
	//	else
	//		AfxMessageBox(IDS_MES_STARTED);

	//	return FALSE; // прекращаем запуск
	//}



#ifdef _TIME
		CTime t = CTime::GetCurrentTime();
		if(t.GetMonth() > LastMonth)
		{
			AfxMessageBox(IDS_SETUP_FAULT);
			exit(1);
		}
#endif

	LoadStdProfileSettings(16);  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

#ifndef NC_DEMO
#ifndef NCM_EDU
#ifndef NCM_QUICK
	if(ReadGuard(true) != GrdE_OK)
		return FALSE;
#endif
#endif
#endif

	NCSingleDocTemplate* pDocTemplate;
	pDocTemplate = new NCSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CNCMDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(NDummyView));
//	pDocTemplate->SetContainerInfo(IDR_CNTR_INPLACE);
	AddDocTemplate(pDocTemplate);

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

// Dispatch commands specified on the command line
	bool FileOpen = false;
	if(cmdInfo.m_nShellCommand == CCommandLineInfo::FileOpen)
	{
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNew;
		FileOpen = true;
	}
	if (cmdInfo.m_bStartInHostMode)
	{
		this->m_nCmdShow = SW_SHOWMINIMIZED;
		NCM_PROJECT.GetGlobalState().SetHostMode(true);
	}
	else
		this->m_nCmdShow = SW_HIDE; // May be removed
	m_bDeferShowOnFirstWindowPlacementLoad = TRUE;
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	m_nCmdShow = SW_HIDE; // this command must be here to avoid flash!
	WINDOWPLACEMENT wp;
	wp.length = sizeof(WINDOWPLACEMENT);
	m_pMainWnd->GetWindowPlacement(&wp);
	if (cmdInfo.m_bStartInHostMode)
		wp.showCmd = SW_SHOWMINIMIZED;
	//m_pMainWnd->ShowWindow(SW_HIDE);
//	((CMainFrame *)m_pMainWnd)->LoadLayout();

// Get CDocument	
	POSITION pos = pDocTemplate->GetFirstDocPosition();
	CDocument *pDoc = pDocTemplate->GetNextDoc(pos);
	CNCMDoc* pNCMDoc = (CNCMDoc*)pDoc;
	ASSERT(pNCMDoc);
	NTiParams& Par = NCM_PROJECT.Defaults;
	// Special cases
	NColor* c;
	if (NCM_PROJECT.Colors.Lookup("ОсевоеСечениеЗаготовок", c))
	{
		if (*c != GQuadRenderRot::AxialSectionColorStock)
		{
			pNCMDoc->GetStocksList().SetAxialSectionColor(*c);// For a future use
			GQuadRenderRot::AxialSectionColorStock = *c;
		}
	}
	if (NCM_PROJECT.Colors.Lookup("ОсевоеСечениеДеталей", c))
	{
		if (*c != GQuadRenderRot::AxialSectionColorPart)
		{
			pNCMDoc->NGetPartList()->SetAxialSectionColor(*c);// For a future use
			GQuadRenderRot::AxialSectionColorPart = *c;
		}
	}
	// END:Special cases
	CString Key = "Defaults/Program/Fragment";
	pNCMDoc->NGetProgramList()->WormAfter = int(Par.GetDouble((Key + "@FORWARD").GetBuffer(), 1.));
	pNCMDoc->NGetProgramList()->WormBefore = int(Par.GetDouble((Key + "@BACKWARD").GetBuffer(), 16.));
	pNCMDoc->SetAutoUpdateProg(Par.GetBool(_T("Defaults/Commands/UpdateProgramWithReset@Enable"), false));
	pNCMDoc->SetLoadLayoutProject(Par.GetBool(_T("Defaults/Commands/LoadLayoutFromTheProject@Enable"), false));
	pNCMDoc->UpdateAllViews(NULL, N_SETTINGS | N_SELECTTEXT | N_STATE);

	// The one and only window has been initialized, so show and update it.
	ChangeConfigByRegistry();
	((CMainFrame *)m_pMainWnd)->SyncLayoutWConfig(GetConfig());
	m_pMainWnd->UpdateWindow();
	wp.showCmd = SW_HIDE;
	m_pMainWnd->SetWindowPlacement(&wp);
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Save window class name
	char WindowClassName[256];
	::GetClassName(m_pMainWnd->m_hWnd, WindowClassName, 256);
	theApp.WriteProfileString(NCMVersion, _T("WindowClassName"), WindowClassName);
	::SetCursor(::LoadCursor(NULL, IDC_ARROW));
	// Check OpenGL diag
	if (BaseRender::GetOGLDiagCode() != 0)
		if(Par.GetBool(_T("Defaults/Render/Diag@Enable"), true))
			AfxMessageBox(IDS_MES_CHECK_OGL_DIAG);

	if (cmdInfo.m_bUnitOpen || cmdInfo.m_bProgrammOpen)
	{
		auto Mask = N_LEFT | N_TEXT | N_OGL | (cmdInfo.m_bUnitOpen ? N_UNIT_CHANGED : 0);
		pNCMDoc->UpdateAllViews(NULL, Mask);
	}
	// Restore CmdShow from registry or set it to SW_SHOWMAXIMIZED
	CRect dumRect;
	int dumFlags;
	int CmdShow = SW_HIDE;
	CWinAppEx::LoadWindowPlacement(dumRect, dumFlags, CmdShow); 
	if (CmdShow != SW_SHOWNORMAL)
		CmdShow = SW_SHOWMAXIMIZED;
	m_pMainWnd->SetActiveWindow();
	pDoc->SetModifiedFlag(FALSE);
	if (cmdInfo.m_bPrtOpen
		&& !cmdInfo.m_strPrtName.IsEmpty())
	{
		pNCMDoc->SetSilent(true);
		if (!OpenDocumentFile(cmdInfo.m_strPrtName))
			return FALSE;
		pNCMDoc->SetSilent(false);
	}
	else if (FileOpen)
	{
		if (!OpenDocumentFile(cmdInfo.m_strFileName))
			return FALSE;
	}
	else
	{
#ifdef NC_DEMO
		bool DemoLoaded = false;
		CString DemoPath = GetDemoPath();
		CString Key = "Defaults/Demo/DemoProject@Name";
		CString FileName = Par.GetString(Key, _T(""));
		if (!DemoPath.IsEmpty() && !FileName.IsEmpty())
			DemoLoaded = (NULL != OpenDocumentFile(DemoPath + FileName));
		if (!DemoLoaded)
#endif
		{
			// Load prt file
			if (GetConfig() & CM_VerifierM || GetConfig() & CM_VerifierM5X)
				OnFileNewMill();
			else if (GetConfig() & CM_VerifierMT)
				OnFileNewMillturn();
			else if (GetConfig() & CM_VerifierT)
				OnFileNewTurn();
			else if (GetConfig() & CM_VerifierW)
				OnFileNewWeld();
			else
				OnFileNewMill();
		}
	}
	pDoc->SetModifiedFlag(FALSE);

	CString strMes;
	if (cmdInfo.m_bUnitOpen
		&& !cmdInfo.m_strUnitName.IsEmpty())
	{
		if (pNCMDoc->GetUnit()->SetDef(cmdInfo.m_strUnitName) >= 0)
		{
			if (static_cast<CMainFrame*>(m_pMainWnd) != nullptr)
				static_cast<CMainFrame*>(m_pMainWnd)->SetNCUnitName(pNCMDoc->GetUnit()->GetConfFileName());
		}

		pDoc->SetModifiedFlag();
	}
	if (cmdInfo.m_bProgrammOpen
		&& cmdInfo.m_strProgramsName.GetSize() > 0)
	{
		for (int i = 0; i < cmdInfo.m_strProgramsName.GetSize(); i++)
		{
			NProgram* pP = pNCMDoc->LoadNewProgram(cmdInfo.m_strProgramsName.GetAt(i));
			if (pP == nullptr)
			{
				strMes.LoadString(IDS_OPENPR_ERR);
				AfxMessageBox(strMes);
			}
			else
				pNCMDoc->NAddMainProgram(cmdInfo.m_strProgramsName.GetAt(i), pP);
		}
		pNCMDoc->RegeneratePrograms();
		pDoc->SetModifiedFlag();
	}
	if (m_pMainWnd != nullptr)
	{
		OGLViewStyle Style = (OGLViewStyle)GetProfileInt(m_strRegSection, _T("MainViewStyle"), OG_Stock);
		if (!(theApp.GetConfig() & CM_Machine))
			Style = OG_Stock;
		switch (Style)
		{
		case OG_Mach:
			((CMainFrame*)m_pMainWnd)->OnWinMach();
			break;
		case OG_Stock:
		default:
			((CMainFrame*)m_pMainWnd)->OnWinStock();
			break;
		}
		((CMainFrame*)m_pMainWnd)->SetMainPane();
	}
	CAppLookDlg dlg(TRUE, m_pMainWnd);
	dlg.DoModal();
	pNCMDoc->UpdateAllViews(NULL, N_SETTINGS);
	m_pMainWnd->ShowWindow(CmdShow);
	((CMainFrame*)m_pMainWnd)->StartView();
//	this->m_bLoadWindowPlacement = FALSE;
	return TRUE;
}



// App command to run the dialog
void CNCMApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CNCMApp message handlers


int CNCMApp::ExitInstance() 
{
	MFOREST.ClearAll();

#ifdef _TIME
	CTime t = CTime::GetCurrentTime();
	if(t.GetMonth() > LastMonth)
	{

	}
#endif
	RegCloseKey(HKEY_LOCAL_MACHINE);

	NCM_PROJECT.ReleaseLicenses();
	ReadGuard(false, true);
	BaseOGLWnd::Finish();
//	ControlBarCleanUp();
	
	AfxOleTerm(FALSE);
	delete CMFCVisualManager::GetInstance();
	ControlBarCleanUp(); // To remove afxkeyboardmanager memory leaks
	return CWinAppEx::ExitInstance();
}

BOOL CNCMApp::LoadWindowPlacement(CRect& rectNormalPosition, int& nFflags, int& nShowCmd)
{
	auto res = CWinAppEx::LoadWindowPlacement(rectNormalPosition, nFflags, nShowCmd);
//	nShowCmd = SW_HIDE;
	return res;
}

void CNCMApp::OnSettings() 
{
	NSettingsDlg SettDlg;
	SettDlg.DoModal();
}

void CNCMApp::OnSettingsAppearance() 
{
	NSettingsDlg SettDlg;
	SettDlg.SetActivePage(&SettDlg.AppearSettDlg);
	SettDlg.DoModal();
}

void CNCMApp::OnSettingsControlPanel() 
{
	NSettingsDlg SettDlg;
	SettDlg.SetActivePage(&SettDlg.StateSettDlg);
	SettDlg.DoModal();
}

void CNCMApp::OnSettingsGraphics() 
{
	NSettingsDlg SettDlg;
	SettDlg.SetActivePage(&SettDlg.GraphSettDlg);
	SettDlg.DoModal();
}

void CNCMApp::OnGlobalSettingsSave()
{
	NCM_PROJECT.StoreDIParams2XML();
	NCM_PROJECT.StoreMColors2XML();
	CString filename;
	filename.LoadString(IDS_DEFIN_FILENAME);
	CString name = NCM_PROJECT.GetSettingsPath() + filename;
	bool saveOkay = NCM_PROJECT.Defaults.SaveFile(name.GetBuffer());
	if (!saveOkay)
		AfxMessageBox(IDS_BSP_SAVE_ERR);
}

void CNCMApp::OnSettingsSave()
{
	CNCMDoc* pDoc = (CNCMDoc*)((CFrameWnd*)m_pMainWnd)->GetActiveDocument();
	if (pDoc == nullptr)
		return;
	CString filter, ext;
	filter.LoadString(IDS_SETTINGS_FILTER);
	ext.LoadString(IDS_SETTINGS_EXT);
	CFileDialog fd(FALSE, ext, pDoc->GetDFNFileName(), OFN_HIDEREADONLY, filter);
	CString path = NCM_PROJECT.GetSettingsPath();
	fd.m_ofn.lpstrInitialDir = path;
	if (fd.DoModal() != IDOK)
		return;
	CString filename = fd.GetPathName();
	int txtpos = filename.ReverseFind('\\');
	CString ShortFileName = filename.Right(filename.GetLength() - txtpos - 1);
	pDoc->SetDFNFileName(ShortFileName);

	CStdioFile f(filename,
		CFile::typeText | CFile::modeWrite | CFile::modeCreate);

	// StateView elements
	NStateUnitPane* pUnitPane = (NStateUnitPane*)((CMainFrame*)m_pMainWnd)->GetUnitPane();
	CStringArray& VisElements = pUnitPane->GetVisElemList();
	for (int i = 0; i < VisElements.GetSize(); i++)
		f.WriteString(VisElements[i] + "\n");
	f.WriteString("NCM Part end\n");
	// AnalyseView elements
	NStateAnalysePane* pAnalysePane = (NStateAnalysePane*)((CMainFrame*)m_pMainWnd)->GetAnalysePane();
	CStringArray& VisAnalyseElements = pAnalysePane->GetVisElemList();
	for (int i = 0; i < VisAnalyseElements.GetSize(); i++)
		f.WriteString(VisAnalyseElements[i] + "\n");
	f.WriteString("NCM Part end\n");
	f.Close();
}

void CNCMApp::OnSettingsLoad()
{
	CNCMDoc* pDoc = (CNCMDoc*)((CFrameWnd*)m_pMainWnd)->GetActiveDocument();
	if (pDoc == nullptr)
		return;
	CString filter, ext;
	filter.LoadString(IDS_SETTINGS_FILTER);
	ext.LoadString(IDS_SETTINGS_EXT);
	CFileDialog fd(TRUE, ext, pDoc->GetDFNFileName(), OFN_HIDEREADONLY, filter);
	CString path = NCM_PROJECT.GetSettingsPath();
	fd.m_ofn.lpstrInitialDir = path;
	if (fd.DoModal() != IDOK)
		return;
	CString filename = fd.GetPathName();
	int txtpos = filename.ReverseFind('\\');
	CString ShortFileName = filename.Right(filename.GetLength() - txtpos - 1);
	pDoc->SetDFNFileName(ShortFileName);
	pDoc->LoadSettings();
	pDoc->UpdateAllViews(NULL, N_SETTINGS);
}

void CNCMApp::DoWaitCursor(int nCode)
{
	CFrameWnd *pFW;
	CWnd *pW = GetMainWnd();
	if(pW)
	{
		if(pW->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
		{
			pFW = (CFrameWnd *)pW;
			switch(nCode)
			{
			case -1:
				pFW->SetMessageText(AFX_IDS_IDLEMESSAGE);
				NCM_PROJECT.GetProgressPercent().Reset();
				break;
			case 1:
				NCM_PROJECT.GetProgressPercent().ClearStr();;
				//break is not required
			case 0:
				pFW->SetMessageText(NCM_PROJECT.GetProgressPercent().FillCurStr());
				break;
			}
		}
	}
	CWinAppEx::DoWaitCursor(nCode);
}

void CNCMApp::OnLanguageEnglish()
{
	NCMLanguage NCMLang = NCMLanguage(GetProfileInt(NCMVersion, "NCMLanguage", NL_UND));
	if(NCMLang == NL_ENG)
		return;
	SetRemoveState();
	AfxMessageBox(IDS_LANG_CHANGE,MB_OK);
//	WriteProfileInt(NCMVersion, "Language", MAKELCID(MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US),SORT_DEFAULT));
	WriteProfileInt(NCMVersion, "NCMLanguage", NL_ENG);
}

void CNCMApp::OnUpdateLanguageEnglish(CCmdUI *pCmdUI)
{
	NCMLanguage NCMLang = NCMLanguage(GetProfileInt(NCMVersion, "NCMLanguage", NL_UND));
	pCmdUI->SetCheck(NCMLang == NL_ENG);
}

void CNCMApp::OnLanguageKorean()
{
	NCMLanguage NCMLang = NCMLanguage(GetProfileInt(NCMVersion, "NCMLanguage", NL_UND));
	if(NCMLang == NL_KOR)
		return;
	SetRemoveState();
	AfxMessageBox(IDS_LANG_CHANGE,MB_OK);
//	WriteProfileInt(NCMVersion, "Language", MAKELCID(MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US),SORT_DEFAULT));
	WriteProfileInt(NCMVersion, "NCMLanguage", NL_KOR);
}

void CNCMApp::OnUpdateLanguageKorean(CCmdUI *pCmdUI)
{
	NCMLanguage NCMLang = NCMLanguage(GetProfileInt(NCMVersion, "NCMLanguage", NL_UND));
	pCmdUI->SetCheck(NCMLang == NL_KOR);
}

void CNCMApp::OnLanguageRussian()
{
	NCMLanguage NCMLang = NCMLanguage(GetProfileInt(NCMVersion, "NCMLanguage", NL_UND));
	if(NCMLang == NL_RUS)
		return;
	SetRemoveState();
	AfxMessageBox(IDS_LANG_CHANGE,MB_OK);
//	WriteProfileInt(NCMVersion, "Language", MAKELCID(MAKELANGID(LANG_RUSSIAN,SUBLANG_DEFAULT),SORT_DEFAULT));
	WriteProfileInt(NCMVersion, "NCMLanguage", NL_RUS);
}

void CNCMApp::OnUpdateLanguageRussian(CCmdUI *pCmdUI)
{
	NCMLanguage NCMLang = NCMLanguage(GetProfileInt(NCMVersion, "NCMLanguage", NL_UND));
	pCmdUI->SetCheck(NCMLang == NL_RUS);
}

bool CNCMApp::IsInCuttingMode(void)
{
	return GetGlobalState().GetCuttingMode();
}
bool CNCMApp::IsInAnimateMode(void)
{
	return GetGlobalState().GetAnimateMode();
}

bool CNCMApp::IsInResultMode(void)
{
	return GetGlobalState().GetResultMode();
}

void CNCMApp::OnCutonoff()
{
	CNCMDoc *pDoc = (CNCMDoc *)((CFrameWnd *)m_pMainWnd)->GetActiveDocument();
	if(!(GetConfig() & CM_VerifierM))
	{
		pDoc->CutEnabled = false;
		return;
	}
	pDoc->CutEnabled = !pDoc->CutEnabled;
}

void CNCMApp::OnCutfastonoff5x()
{
	CNCMDoc *pDoc = (CNCMDoc *)((CFrameWnd *)m_pMainWnd)->GetActiveDocument();
	if(!(GetConfig() & CM_VerifierM5X))
	{
		pDoc->FastCut5xEnabled = false;
		return;
	}
	pDoc->FastCut5xEnabled = !pDoc->FastCut5xEnabled;
}

void CNCMApp::OnDebugmode()
{
	
	TDialog dlg;
	dlg.m_CellSize=BDebugState::Get().CellSize;
	dlg.m_DXNumber=BDebugState::Get().DXNumber;
	dlg.m_DebugMode=BDebugState::Get().GetDebugMode();
	dlg.m_Smooth = GBodyRender::GetSmooth();
	dlg.m_DrawFaces=BDebugState::Get().DrawFaces;
	dlg.m_MultiThread = BDebugState::Get().MultiThread;
	dlg.m_Step = BDebugState::Get().Step;
	dlg.m_Maxl = BDebugState::Get().Maxl;//!!!
	dlg.m_Dich = BDebugState::Get().Dich;//!!!
	dlg.m_UseSlicedTree = BDebugState::Get().UseSlicedTree;
	dlg.m_SurfelVis = BDebugState::Get().SurfelVis;
	dlg.m_MarchCubes = BDebugState::Get().MarchCubes;
	dlg.m_Async = BDebugState::Get().Async;
	if(dlg.DoModal() == IDOK)
	{
		BDebugState::Get().CellSize = dlg.m_CellSize;
		BDebugState::Get().DXNumber = dlg.m_DXNumber;
		BDebugState::Get().SetDebugMode(dlg.m_DebugMode == TRUE);
		BDebugState::Get().DrawFaces = (dlg.m_DrawFaces == TRUE);
		BDebugState::Get().MultiThread = (dlg.m_MultiThread == TRUE);
		NCMProject::GetProject().GetGlobalState().SetMultiThreadDX(BDebugState::Get().MultiThread);
		GBodyRender::SetSmooth(dlg.m_Smooth == TRUE);
		BDebugState::Get().Step = dlg.m_Step;
		BDebugState::Get().Maxl = dlg.m_Maxl;//!!!
		BDebugState::Get().Dich = dlg.m_Dich;//!!!
		BDebugState::Get().UseSlicedTree = (dlg.m_UseSlicedTree == TRUE);
		BDebugState::Get().SurfelVis = (dlg.m_SurfelVis == TRUE);
		BDebugState::Get().MarchCubes = (dlg.m_MarchCubes == TRUE);
		BDebugState::Get().Async = (dlg.m_Async == TRUE);
	}
}


void CNCMApp::OnResultMode()
{
	if (!CanResultModeBeEnabled())
		return;
	NCM_PROJECT.GetGlobalState().SetResultMode();
	((CMainFrame *)m_pMainWnd)->ShowResultDlg();
}

void CNCMApp::OnUpdateResultMode(CCmdUI *pCmdUI)
{
	if( GetGlobalState().GetResultMode())
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
		pCmdUI->Enable(CanResultModeBeEnabled());
	}
}

bool CNCMApp::CanResultModeBeEnabled()
{
	return !((CMainFrame *)m_pMainWnd)->IsCuttingInProgress();
}

void CNCMApp::OnSmothnessMode()
{
	if (!CanSmothnessModeBeEnabled())
		return;
	GetGlobalState().SetAnimateMode();
	NSpeedPar::Extern.SetTrackFeed(false);
	((CMainFrame *)m_pMainWnd)->ShowPauseDlg();
}

void CNCMApp::OnSmothnessModeRib()
{
	GetGlobalState().SetAnimateMode();
	if (NSpeedPar::Extern.IsTrackFeed())
		((CMainFrame *)m_pMainWnd)->ShowPauseFDlg();
	else
		((CMainFrame *)m_pMainWnd)->ShowPauseDlg();
}

void CNCMApp::OnUseFeed()
{
	NSpeedPar::Extern.SetTrackFeed(!NSpeedPar::Extern.IsTrackFeed());
	if (GetGlobalState().GetAnimateMode())
		OnSmothnessModeRib();
}

void CNCMApp::OnLayoutProject()
{
	CNCMDoc* pDoc = (CNCMDoc*)((CMainFrame*)m_pMainWnd)->GetActiveDocument();

	pDoc->SetLoadLayoutProject(!pDoc->GetLoadLayoutProject());
}

void CNCMApp::OnUpdateSmothnessMode(CCmdUI *pCmdUI)
{
	if(IsInAnimateMode() && !NSpeedPar::Extern.IsTrackFeed())
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
		pCmdUI->Enable(CanSmothnessModeBeEnabled());
	}
}

bool CNCMApp::CanSmothnessModeBeEnabled()
{
	return !((CMainFrame *)m_pMainWnd)->IsCuttingInProgress();
}

void CNCMApp::OnUpdateSmothnessModeRib(CCmdUI *pCmdUI)
{
	if (IsInAnimateMode())
	{
		pCmdUI->SetCheck(IsInAnimateMode());
	}
	else
	{
		pCmdUI->SetCheck(0);
		BOOL Enable = !((CMainFrame *)m_pMainWnd)->IsCuttingInProgress();
		pCmdUI->Enable(Enable);
	}
}

void CNCMApp::OnUpdateUseFeed(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(NSpeedPar::Extern.IsTrackFeed());
	BOOL Enable = !((CMainFrame *)m_pMainWnd)->IsCuttingInProgress();
	pCmdUI->Enable(Enable);
}

void CNCMApp::OnUpdateLayoutProject(CCmdUI* pCmdUI)
{
	CNCMDoc* pDoc = (CNCMDoc*)((CMainFrame*)m_pMainWnd)->GetActiveDocument();

	pCmdUI->SetCheck(pDoc->GetLoadLayoutProject());
}

void CNCMApp::OnSmothnessModeF()
{
	if (!CanSmothnessModeFBeEnabled())
		return;
	GetGlobalState().SetAnimateMode();
	NSpeedPar::Extern.SetTrackFeed(true);
	((CMainFrame *)m_pMainWnd)->ShowPauseFDlg();
}

void CNCMApp::OnUpdateSmothnessModeF(CCmdUI *pCmdUI)
{
	if(IsInAnimateMode() && NSpeedPar::Extern.IsTrackFeed())
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
		pCmdUI->Enable(CanSmothnessModeFBeEnabled());
	}
}

bool CNCMApp::CanSmothnessModeFBeEnabled()
{
	return !((CMainFrame *)m_pMainWnd)->IsCuttingInProgress();
}

void CNCMApp::StockWarnMessage(void)
{
	AfxMessageBox(IDS_STOCK_WARN);
}

void CNCMApp::ProgErrMessage(void)
{
	AfxMessageBox(IDS_ERRANDCUT_WARN);
}

void CNCMApp::OnUpdateRecentFileMenu(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
	CWinAppEx::OnUpdateRecentFileMenu(pCmdUI);
	pCmdUI->Enable(FALSE);//!((CMainFrame *)m_pMainWnd)->IsCuttingInProgress());
}
CDocument* CNCMApp::OpenDocumentFile(LPCTSTR lpszFileName)
{
	if(m_pMainWnd)
	{
		((CMainFrame *)m_pMainWnd)->OnReset();
		// Process .ncmj file
		CString FileName(lpszFileName);
		int ind = FileName.ReverseFind('.');
		if(ind > 0)
			if(FileName.Right(FileName.GetLength() - ind - 1) == _T("ncmj"))
			{
				CNCMDoc *pDoc = (CNCMDoc *)((CFrameWnd *)m_pMainWnd)->GetActiveDocument();
				pDoc->ReadJobFile(lpszFileName);
				((CMainFrame *)m_pMainWnd)->OnUpdateFrameTitle(TRUE);
				pDoc->UpdateAllViews(NULL, N_LEFT | N_STATE | N_SETTINGS | N_UNIT_CHANGED | N_TOOLCHANGED);
				return pDoc;
			}
		if (((CMainFrame*)m_pMainWnd)->GetSafeHwnd())
		{
			((CMainFrame*)m_pMainWnd)->CloseCreateSheet();
			((CMainFrame*)m_pMainWnd)->Send2PropPane(NULL);
		}
		((CMainFrame*)m_pMainWnd)->DeleteAddOGLPanes();
		((NReportPane*)((CMainFrame*)m_pMainWnd)->GetReportPane())->Clear(true, true);
		((NTextView*)((CMainFrame*)m_pMainWnd)->GetTextView())->ClearCollisionMarkers();
	}

	CNCMDoc *pDoc = (CNCMDoc *)CWinAppEx::OpenDocumentFile(lpszFileName);
	if (pDoc != nullptr)
	{
		CString Path = pDoc->GetPathName();
		if (Path.Right(6) != _T("NCMPrt"))
			WriteProfileString(NCMVersion, _T("LastOpenedPath"), Path);
	}
	return pDoc;
}

void CNCMApp::OnFileNew()
{
	if(PrepFileNew())
		CWinAppEx::OnFileNew();
}

void CNCMApp::OnFileOpen()
{
#ifdef NC_DEMO
	
	// prompt the user (with all document templates)
	CString newName;
//	if (!DoPromptFileName(newName, AFX_IDS_OPENFILE,
//	  OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, TRUE, NULL))
//		return; // open cancelled
	CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL, 0);

	CString title;
	VERIFY(title.LoadString(AFX_IDS_OPENFILE));

	dlgFile.m_ofn.Flags |= OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;

	dlgFile.m_ofn.nMaxCustFilter = 1;
	CString strFilter;
	VERIFY(strFilter.LoadString(IDR_DEMOFILTER));
	strFilter.Replace('|',(TCHAR)'\0');
	strFilter += (TCHAR)'\0';// На случай ошибки в IDR_DEMOFILTER
	dlgFile.m_ofn.lpstrFilter = strFilter;

	dlgFile.m_ofn.lpstrFile = newName.GetBuffer(_MAX_PATH);

	dlgFile.m_ofn.lpstrInitialDir = GetDemoPath();

	INT_PTR nResult = dlgFile.DoModal();
	newName.ReleaseBuffer();
	if(nResult == IDOK)
		OpenDocumentFile(newName);
		// if returns NULL, the user has already been alerted
#endif
#ifndef NC_DEMO
	CNCMDoc *pDoc = (CNCMDoc *)((CMainFrame *)m_pMainWnd)->GetActiveDocument();
	CString Path = pDoc->GetPathName();
	if(Path.Right(6) == _T("NCMPrt"))
		Path.Empty();
	if(Path.IsEmpty())
		Path = GetProfileString(NCMVersion, _T("LastOpenedPath"), _T(""));
	if(Path.IsEmpty())
		Path = NCM_PROJECT.GetDemoPath();
	Path = Path.Left(Path.ReverseFind('\\') + 1);
	CString Filter;
	VERIFY(Filter.LoadString(IDS_NCMPROJ_FILTER));
	Filter.Replace('|',(TCHAR)'\0');
	Filter += (TCHAR)'\0';// На случай ошибки в IDR_DEMOFILTER
	CFileDialog FileDialog(TRUE);
	FileDialog.m_ofn.lpstrInitialDir = Path;
	FileDialog.m_ofn.lpstrFilter = Filter;
	if(FileDialog.DoModal() == IDOK)
	{
		CString FileName = FileDialog.GetPathName();
		OpenDocumentFile(FileName);
		WriteProfileString(NCMVersion, _T("LastOpenedPath"), FileName);
	}
#endif
}

void CNCMApp::OnFileOpenRib()
{
	if (!m_pMainWnd)
		return;
	((CMainFrame*)m_pMainWnd)->PostMessage(WM_COMMAND, ID_FILE_OPEN);// to prevent ribbon change while in CMFCRibbonBar::OnLButtonUp
}

#define	CryptPU		0x8566783lu							// Random value for access codes security                              
#define	CryptRD		0x17d49c84u							// Random value for access codes security                              
bool CNCMApp::FindDongle(CGrdDongle *pGrdDongle, TGrdFindInfo &GrdFindInfo, DWORD dwPublic, DWORD dwPrivRD, int &nRet)
{
	uint32_t	dwID;											// Dongle ID                               
	nRet = pGrdDongle->Open(
		// parameters for call GrdSetAccessCodes()
		dwPublic + CryptPU, 								// Must be already specified
		dwPrivRD + CryptRD,									// Must be already specified
		0 ); 								// == 0 if not needed
		// Create Grd protected object
//		HANDLE hGrd = GrdCreateHandle( NULL, GrdCHM_MultiThread, NULL );
	if(nRet != GrdE_OK)
	{
		return false;
	}
	nRet = pGrdDongle->SetFindMode(GrdFMR_Local, // Try to find local
		GrdFM_SN, // Type
		0, 0, 31161, 0, 0, 0,
		GrdFMM_ALL,
		GrdFMI_ALL ); 
	if(nRet != GrdE_OK)
	{
		return false;
	}
	nRet = pGrdDongle->Find( GrdF_First, &dwID, &GrdFindInfo );
	if(nRet != GrdE_OK || GrdFindInfo.wType & GrdDT_LAN || GrdFindInfo.wGP != 0)
	{
		nRet = pGrdDongle->SetFindMode(GrdFMR_Remote,// Try to find remote
			GrdFM_SN,
			0, 0, 31161, 0, 0, 0,
			GrdFMM_ALL,
			GrdFMI_ALL ); 
		if(nRet != GrdE_OK)
		{
			return false;
		}
		nRet = pGrdDongle->Find( GrdF_First, &dwID, &GrdFindInfo );
		if(nRet != GrdE_OK)
		{
			//char ErrMsg[100];
			//pGrdDongle->FormatMessage( GrdLng_ENG, ErrMsg, 99);
			return false;
		}
	}
	return true;
}

int CNCMApp::ReadGuard(bool StartCall, bool Clear)
{
	return GrdE_OK; //!!!_!!!
// GUARD
// Definitions

// Data variables 
DWORD	dwPublic1 = 0x717C1CBC-CryptPU;				// Must be encoded    Для INTECSO         
DWORD	dwPrivRD1 = 0xF9AF78F6-CryptRD;				// Must be encoded 
DWORD	dwPublic2 = 0x70AEA9AA-CryptPU;				// Must be encoded    Для CAO         
DWORD	dwPrivRD2 = 0xB0ABF238-CryptRD;				// Must be encoded             

int		nRet;											// Return code  
TGrdFindInfo GrdFindInfo; 
//----------------------------------------------------------------------------------------------
	static CGrdDongle *pGrdDongle = NULL;
	if(Clear)
	{
		delete pGrdDongle;
		pGrdDongle = NULL;
		return -1;
	}
		
	if(!pGrdDongle)
	{
		pGrdDongle = new CGrdDongle(GrdFMR_Local + GrdFMR_Remote, 0, 0, 0, 0, 0, 0, 0,
			GrdFMM_ALL, GrdFMI_ALL, GrdRCS_ProgramData, _T("NCManager\\"));		// parameters for call GrdStartup() & GrdSetFindMode
		GuDongleModule::DongleVersion = 0; // Old one
		if(!FindDongle(pGrdDongle, GrdFindInfo, dwPublic1, dwPrivRD1, nRet))
		{
			GuDongleModule::DongleVersion = 1; // New one
			if(!FindDongle(pGrdDongle, GrdFindInfo, dwPublic2, dwPrivRD2, nRet))
			{
				AfxMessageBox(IDS_KEYNOTFOUND);
				return nRet;
			}
		}
		nRet = pGrdDongle->Login();
		if(nRet != GrdE_OK)
		{
			AfxMessageBox(IDS_KEYNOTFOUND);
			return nRet;
		}
		if(GrdFindInfo.byVer < 50) //!!!_ ВЕРСИЯ _!!!
		{
			AfxMessageBox(IDS_VERWRONG);
			return -1;
		}
		//time code
		if (GrdFindInfo.wType & GrdDT_RTC)// Support of Real Time Clock
		{
			TGrdSystemTime Res;
			int Code = pGrdDongle->PI_GetTimeLimit(01, &Res);
			if (Code != GrdE_OK)
			{
				AfxMessageBox(IDS_KEYNOTFOUND);
				return -1;
			}
			if (StartCall && Res.wYear == 0 && Res.wMonth == 0 && Res.wDay < 28)
			{
				CString Mes;
				Mes.LoadString(IDS_GRDMES_TIMELIM);
				Mes.Format(Mes, Res.wDay);
				AfxMessageBox(Mes);
				if (Res.wDay == 0 && Res.wHour == 0 && Res.wMinute == 0)
				{
					AfxMessageBox(IDS_KEYNOTFOUND);
					return -1;
				}
			}
		}
		//
		Configuration = GrdFindInfo.wMask;
		DWORD Floating;
		pGrdDongle->GetInfo(GrdGIL_Remote, &Floating, sizeof(Floating));
		NCM_PROJECT.SetFloating(Floating == 1);

		static constexpr int PostUserIDAddr = 34;
		unsigned __int16 UserID;
		pGrdDongle->Read(PostUserIDAddr, 2, &UserID);
		// Debug
		CString mes;
		mes.Format("Значение по адресу %d: %d", PostUserIDAddr, UserID);
		AfxMessageBox(mes);
		// END:Debug
		NCM_PROJECT.SetPostUserID(UserID);

		if(NCM_PROJECT.IsLicenseFloating())
		{
			TruncateLicense();
		}
		if(GrdFindInfo.wGP > 1)// Floating license with different number of licenses for one module
		{
			NCMConfig Locked(0x0);
			for(int i = 0; i < sizeof(Configuration) * 8; ++i)
			{
				ConfigMask Flag = ConfigMask(0x1 << i);
				if(Configuration & Flag)
					if(!LoginLicense(Flag))
						Locked |= Flag;
			}
			if(Locked)
			{
				NNetConfigDlg Dlg;
				Dlg.DoModal(Configuration, Locked);
				return -1;
			}
		}
	}
	else
	{
		NCMConfig cfg;
		int	nRet = pGrdDongle->Read(4, 2, &cfg);
		if(nRet != GrdE_OK || (cfg & Configuration) != Configuration)
		{
			AfxMessageBox(IDS_KEYNOTFOUND);
			return nRet;
		}
	}

// END GUARD
	return GrdE_OK;
}

NCMConfig CNCMApp::GetConfig(void)
{
	static int counter = 0;
	if(++counter == 4980)
	{
#ifndef NC_DEMO
#ifndef NCM_EDU
#ifndef NCM_QUICK
		if(ReadGuard(false) != GrdE_OK)
			exit(3);
#endif
#endif
#endif
		counter = 0;
	}
	return Configuration;
}

bool CNCMApp::ChangeType(NCMApplicationType NewType)
{
	if(NCMComData::GetType() == NewType)
		return true;

	NCMComData::SetType(NewType);
	CString MainTitle;
#ifdef NCM_QUICK
	MainTitle.LoadString(IDS_QUICKTITLE);
#endif
#ifndef NCM_QUICK
	MainTitle.LoadString(IDS_COMMONTITLE);
	MainTitle = MainTitle + _T(" ") + NCMVersion + NCMVerSuffix;
#endif
	CString ModToleranceKey;
	if(m_pMainWnd)
	{
		switch(NCMComData::GetType())
		{
		case AT_MILL:
			ModToleranceKey = _T("ModTolerance");
			((CMainFrame *)m_pMainWnd)->ChangeMainMenu(IDR_MAINFRAME_MILL, IDR_RIBBON_MILL);
			((CMainFrame *)m_pMainWnd)->SetTitle(MainTitle + " Mill");
			break;
		case AT_PUNCH:
			ModToleranceKey = _T("ModTolerancePunch");
			((CMainFrame *)m_pMainWnd)->ChangeMainMenu(IDR_MAINFRAME, IDR_RIBBON);
			((CMainFrame *)m_pMainWnd)->SetTitle(MainTitle + " Punch");
			break;
		case AT_TURN:
			ModToleranceKey = _T("ModToleranceTurn");
			((CMainFrame *)m_pMainWnd)->ChangeMainMenu(IDR_MAINFRAME, IDR_RIBBON);
			((CMainFrame *)m_pMainWnd)->SetTitle(MainTitle + " Turn");
			break;
		case AT_MILL_TURN:
			ModToleranceKey = _T("ModToleranceMillTurn");
			((CMainFrame *)m_pMainWnd)->ChangeMainMenu(IDR_MAINFRAME, IDR_RIBBON_MILLTURN);
			((CMainFrame *)m_pMainWnd)->SetTitle(MainTitle + " Mill-Turn");
			break;
		case AT_WELD:
			ModToleranceKey = _T("ModToleranceWeld");
			((CMainFrame *)m_pMainWnd)->ChangeMainMenu(IDR_MAINFRAME_WELD, IDR_RIBBON_WELD);
			((CMainFrame *)m_pMainWnd)->SetTitle(MainTitle + " EBW");
			break;
		default:
			ModToleranceKey = _T("ModTolerance");
			((CMainFrame *)m_pMainWnd)->ChangeMainMenu(IDR_MAINFRAME, IDR_RIBBON);
			((CMainFrame *)m_pMainWnd)->SetTitle(MainTitle);
			break;
		}
		double *pDouble;
		if(NCM_PROJECT.DoubleParams.Lookup(ModToleranceKey, pDouble))
		{
			NSurfSettings::SetTolerance(*pDouble);
		}
		((CMainFrame *)m_pMainWnd)->AppTypeChanged(NCMComData::GetType());
	}
	return true;
}

bool CNCMApp::PrepFileNew(void)
{
	if(m_pMainWnd)
	{
		if(((CMainFrame *)m_pMainWnd)->IsCuttingInProgress())
		{
			AfxMessageBox(IDS_CANT_OPEN);
			return false;
		}
		NSpeedPar::Extern.SetTrackFeed(false);
//		((CMainFrame *)m_pMainWnd)->ShowPauseDlg();
		((CMainFrame *)m_pMainWnd)->OnReset();
	}
	return true;
}

void CNCMApp::OnFileNewMill()
{
	if(!PrepFileNew())
		return;

	OpenDocumentFile(NCM_PROJECT.GetPrototypesPath() + _T("Mill.NCMPrt"));
}

void CNCMApp::OnFileNewNcmprt()
{
	if (!PrepFileNew())
		return;

	CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL, 0);

	CString fileName;
	CString title;
	title.LoadString(IDS_LOAD_NCMPRT);

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

	const INT_PTR nResult = dlgFile.DoModal();
	fileName.ReleaseBuffer();
	if (nResult == IDOK)
		OpenDocumentFile(fileName);
}

void CNCMApp::OnFileNewTurn()
{
	if(!PrepFileNew())
		return;

	OpenDocumentFile(NCM_PROJECT.GetPrototypesPath() + _T("Turn.NCMPrt"));
}

void CNCMApp::OnFileNewMillturnRib()
{
	if (!m_pMainWnd)
		return;
	((CMainFrame *)m_pMainWnd)->PostMessage(WM_COMMAND, ID_FILE_NEW_MILLTURN);// to prevent ribbon change while in CMFCRibbonBar::OnLButtonUp
}

void CNCMApp::OnFileNewMillRib()
{
	if (!m_pMainWnd)
		return;
	((CMainFrame *)m_pMainWnd)->PostMessage(WM_COMMAND, ID_FILE_NEW_MILL);// to prevent ribbon change while in CMFCRibbonBar::OnLButtonUp
}

void CNCMApp::OnFileNewTurnRib()
{
	if (!m_pMainWnd)
		return;
	((CMainFrame *)m_pMainWnd)->PostMessage(WM_COMMAND, ID_FILE_NEW_TURN);// to prevent ribbon change while in CMFCRibbonBar::OnLButtonUp
}

void CNCMApp::OnFileNewNcmprtRib()
{
	if (!m_pMainWnd)
		return;
	((CMainFrame*)m_pMainWnd)->PostMessage(WM_COMMAND, ID_FILE_NEW_NCMPRT);// to prevent ribbon change while in CMFCRibbonBar::OnLButtonUp
}

void CNCMApp::OnFileNewMillturn()
{
	if(!PrepFileNew())
		return;

	OpenDocumentFile(NCM_PROJECT.GetPrototypesPath() + _T("MillTurn.NCMPrt"));
}

bool CNCMApp::IsVerifEnabled(void)
{
	return (GetConfig() & GetVerifFlag()) != 0;
}

BOOL CALLBACK CNCMApp::searcher(HWND hWnd, LPARAM lParam)
{
    DWORD_PTR result;
    LRESULT ok = ::SendMessageTimeout(hWnd, WM_APP_UNIQUE, 0, 0,
                                      SMTO_BLOCK | SMTO_ABORTIFHUNG, 200,
                                      &result);
    if(ok == 0)
    return TRUE; // игнорируем это окно и продолжаем

    if(result == WM_APP_UNIQUE)
    { /* нашли */
        HWND * target = (HWND *)lParam;
        *target = hWnd;
        return FALSE; // заканчиваем поиск
    } /* нашли */

    return TRUE; // продолжаем поиск
} // CMyApp::searcher


void CNCMApp::OnFileReopen()
{
	OpenDocumentFile(GetProfileString(NCMVersion,"ReopenFileName"));
	WriteProfileString(NCMVersion, "ReopenFileName", "");
}

void CNCMApp::OnUnitMm()
{
	if(BConst::State == M_MM)
		return;
	AfxMessageBox(IDS_UNIT_CHANGE, MB_OK);
	BConst::State = M_MM;
	WriteProfileInt(NCMVersion, "MmInch", BConst::State);
}

void CNCMApp::OnUnitInch()
{
	if(BConst::State == M_INCH)
		return;
	AfxMessageBox(IDS_UNIT_CHANGE, MB_OK);
	BConst::State = M_INCH;
	WriteProfileInt(NCMVersion, "MmInch", BConst::State);
}

void CNCMApp::OnUpdateUnitMm(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(BConst::State == M_MM);
}

void CNCMApp::OnUpdateUnitInch(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(BConst::State == M_INCH);
}

CString CNCMApp::GetDemoPath(void)
{
	CString Path = NCM_PROJECT.GetDemoPath();
	if(!Path.IsEmpty())
	{
		if(BConst::State == M_MM)
			Path += _T("Millimeter\\");
		else
			Path += _T("Inch\\");
	}
	return Path;
}
void CNCMApp::OnViewAppLook()
{
	CAppLookDlg dlg(FALSE, m_pMainWnd);
	dlg.DoModal();
}

void CNCMApp::OnChangeCutMode()
{
	if(!m_pMainWnd)
		return;
	if(!((CMainFrame *)m_pMainWnd)->CheckCutting())
		return;

	CNCMDoc *pDoc = (CNCMDoc *)((CMainFrame *)m_pMainWnd)->GetActiveDocument();
	pDoc->Measure.DeActivate();

	if(IsInCuttingMode())
		OnDontcutMode();
	else switch(GetGlobalState().GetModelMode())
	{
	case NGlobalState::DX:
		OnFastCutMode5X();
		break;
	case NGlobalState::BSP:
		OnCutMode();
		break;
	}
}
void CNCMApp::OnUpdateChangeCutMode(CCmdUI *pCmdUI)
{
	if(!m_pMainWnd)
		return;
	((CMainFrame *)m_pMainWnd)->SetModeImage();
}
void CNCMApp::OnChangeWin()
{
	if(!m_pMainWnd)
		return;

	((CMainFrame *)m_pMainWnd)->ChangeWin();
}
void CNCMApp::OnUpdateChangeWin(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(theApp.GetConfig() & CM_Machine);
	if(!(theApp.GetConfig() & CM_Machine))
		return;
	if(!m_pMainWnd)
		return;
	((CMainFrame *)m_pMainWnd)->SetChangeViewImage();
}
void CNCMApp::OnCutMode()
{
	StartSlowCutMode();
}

bool CNCMApp::StartSlowCutMode()
{
	if (!((CMainFrame*)m_pMainWnd)->CanCutModeBeEnabled())
		return false;
	if (((CLeftView*)((CMainFrame*)m_pMainWnd)->GetLeftPane())->IsPosDlgActive())
	{
		CNCMDoc* pDoc = (CNCMDoc*)((CMainFrame*)m_pMainWnd)->GetActiveDocument();
		if (pDoc->IsStockPosChecked())
		{
			AfxMessageBox(IDS_COMP_POS_CUTTING);
			return false;
		}
	}
	CNCMDoc* pDoc = (CNCMDoc*)((CMainFrame*)m_pMainWnd)->GetActiveDocument();
	if (!IsVerifEnabled() && !pDoc->IsCollWOStock())
		return false;
	if (pDoc->IsThereAnyErrProgr())
	{
		ProgErrMessage();
		return false;
	}
	pDoc->Measure.DeActivate();
	if (pDoc->IsThereAnyStock())
	{
		if (pDoc->IsThereAnyStockImpExpDX())
		{
			AfxMessageBox(IDS_MES_MFR_DX);
			return false;
		}
		CStocksCollection* list = pDoc->NGetStockList();
		list->SetWFrame(false);
		for each (const auto & el in *list)
		{
			const CString& key = el.first;
			class NStock* pS = static_cast<class NStock*>(el.second);
			if (pS)
			{
				pS->WFrame = false;
				if (GetGlobalState().GetModelMode() != NGlobalState::BSP)
					pS->RemoveMillBody();
			}
		}
	}
	else
	{
		if(!pDoc->IsCollWOStock())
		{
			StockWarnMessage();
			return false;
		}
	}
	GetGlobalState().SetCuttingMode(true);
	if (GetGlobalState().GetModelMode() != NGlobalState::BSP)
	{
		if (pDoc->IsThereAnyStock())
			pDoc->GetStock(0)->Changed();
		SetBSPParams();
		((CMainFrame*)m_pMainWnd)->OnReset();
	}
	((CMainFrame*)m_pMainWnd)->SetModeImage();
	pDoc->UpdateAllViews(NULL, N_OGL);
	pDoc->SetModifiedFlag();// for snaps
	return true;
}

void CNCMApp::OnFastCutMode5X()
{
	StartFastCutMode();
}

bool CNCMApp::StartFastCutMode()
{
	if (!((CMainFrame*)m_pMainWnd)->CanFastCut5xBeEnabled())
		return false;
	if (((CLeftView*)((CMainFrame*)m_pMainWnd)->GetLeftPane())->IsPosDlgActive())
	{
		CNCMDoc* pDoc = (CNCMDoc*)((CMainFrame*)m_pMainWnd)->GetActiveDocument();
		if (pDoc->IsStockPosChecked())
		{
			AfxMessageBox(IDS_COMP_POS_CUTTING);
			return false;
		}
	}
	CNCMDoc* pDoc = (CNCMDoc*)((CMainFrame*)m_pMainWnd)->GetActiveDocument();
	if (!IsVerifEnabled() && !pDoc->IsCollWOStock())
		return false;
	if (pDoc->IsThereAnyErrProgr())
	{
		ProgErrMessage();
		return false;
	}

	pDoc->Measure.DeActivate();
	if (pDoc->IsThereAnyStock())
	{
		CStocksCollection* list = pDoc->NGetStockList();
		list->SetWFrame(false);
		for each (const auto & el in *list)
		{
			class NStock* pS = static_cast<class NStock*>(el.second);
			if (pS)
			{
				pS->WFrame = false;
				if (GetGlobalState().GetModelMode() != NGlobalState::DX)
					pS->RemoveMillBody();
			}
		}
	}
	else
	{
		if (!pDoc->IsCollWOStock())
		{
			StockWarnMessage();
			return false;
		}
	}
	GetGlobalState().SetCuttingMode(true);
	if (GetGlobalState().GetModelMode() != NGlobalState::DX)
	{
		SetDXParams();
		((CMainFrame*)m_pMainWnd)->OnReset();
		if (pDoc->IsThereAnyStock())
			pDoc->GetStock(0)->Changed();
	}
	((CMainFrame*)m_pMainWnd)->SetModeImage();
	pDoc->UpdateAllViews(NULL, N_OGL);
	return true;
}

void CNCMApp::OnDontcutMode()
{
	if (!CanDontcutModeBeEnabled())
		return;
	if(!m_pMainWnd)
		return;
	if(!((CMainFrame *)m_pMainWnd)->CheckCutting())
		return;
	CNCMDoc *pDoc = (CNCMDoc *)((CMainFrame *)m_pMainWnd)->GetActiveDocument();
	pDoc->Measure.DeActivate();
	pDoc->Dimensions.DeActivate();
	
	if(pDoc->IsThereAnyStock())
	{
		pDoc->SetWFrameToAllStocks(true);
		pDoc->SetTransparentToAllStocks(false);
	}
	GetGlobalState().SetCuttingMode( false );
	((CMainFrame *)m_pMainWnd)->SetModeImage();
	pDoc->UpdateAllViews(NULL, N_OGL);
}

void CNCMApp::OnCutModeRib()
{
	StartCutMode();
}

bool CNCMApp::StartCutMode()
{
	if (IsInCuttingMode())
		return true;
	if (!m_pMainWnd)
		return false;
	if (!((CMainFrame*)m_pMainWnd)->CheckCutting())
		return false;

	CNCMDoc* pDoc = (CNCMDoc*)((CMainFrame*)m_pMainWnd)->GetActiveDocument();
	pDoc->Measure.DeActivate();

	switch (GetGlobalState().GetModelMode())
	{
	case NGlobalState::DX:
		return StartFastCutMode();
	case NGlobalState::BSP:
	default:
		return StartSlowCutMode();
	}
	return false;
}

void CNCMApp::OnUseDX()
{
	CWaitCursor w;
	if (!m_pMainWnd)
		return;
	if (!((CMainFrame *)m_pMainWnd)->CheckCutting())
		return;

	CNCMDoc *pDoc = (CNCMDoc *)((CMainFrame *)m_pMainWnd)->GetActiveDocument();
	pDoc->Measure.DeActivate();
	if (pDoc->IsThereAnyStock())
	{
		bool WasInCuttingMode = IsInCuttingMode();

		switch (GetGlobalState().GetModelMode())
		{
		case NGlobalState::BSP:
		default:
			OnFastCutMode5X();
			break;
		case NGlobalState::DX:
			OnCutMode();
			break;
		}

		if (!WasInCuttingMode)
			OnDontcutMode();
	}
	else
	{
		switch (GetGlobalState().GetModelMode())
		{
		case NGlobalState::BSP:
		default:
			SetDXParams();
			break;
		case NGlobalState::DX:
			SetBSPParams();
			break;
		}

	}
}

void CNCMApp::OnUpdateDontcutMode(CCmdUI *pCmdUI)
{
	if (!IsInCuttingMode())
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
		pCmdUI->Enable(CanDontcutModeBeEnabled());
	}
}

bool CNCMApp::CanDontcutModeBeEnabled()
{
	return !((CMainFrame *)m_pMainWnd)->IsCuttingInProgress();
}

void CNCMApp::OnUpdateCutModeRib(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(IsInCuttingMode());
	CNCMDoc *pDoc = (CNCMDoc *)((CMainFrame *)m_pMainWnd)->GetActiveDocument();
	BOOL Enable = !((CMainFrame *)m_pMainWnd)->IsCuttingInProgress();
	Enable &= (pDoc->FastCut5xEnabled || pDoc->CutEnabled);
	pCmdUI->Enable(Enable);
}

void CNCMApp::OnUpdateUseDX(CCmdUI *pCmdUI)
{
	switch (GetGlobalState().GetModelMode())
	{
	case NGlobalState::BSP:
		pCmdUI->SetCheck(0);
		break;
	case NGlobalState::DX:
		pCmdUI->SetCheck(1);
		break;
	}
	BOOL Enable = !((CMainFrame *)m_pMainWnd)->IsCuttingInProgress();
	if (Enable)
	{
		CNCMDoc *pDoc = (CNCMDoc *)((CMainFrame *)m_pMainWnd)->GetActiveDocument();
		if (!pDoc->IsFastCut5xEnabled() || !pDoc->IsCutEnabled())
		{
			Enable = FALSE;
		}
	}
	pCmdUI->Enable(Enable);
}


void CNCMApp::OnFileNewWeld()
{
	if(!PrepFileNew())
		return;

	OpenDocumentFile(NCM_PROJECT.GetPrototypesPath() + _T("Weld.NCMPrt"));
}
bool CNCMApp::LoginLicense(ConfigMask Module)
{
#ifndef NC_DEMO
#ifndef NCM_EDU
#ifndef NCM_QUICK
	return NCM_PROJECT.LockLicense(GetModuleNum(Module));
#endif
#endif
#endif
	return true;
}
bool CNCMApp::LogoutLicense(ConfigMask Module)
{
#ifndef NC_DEMO
#ifndef NCM_EDU
#ifndef NCM_QUICK
	return NCM_PROJECT.UnlockLicense(GetModuleNum(Module));
#endif
#endif
#endif
	return true;
}
int CNCMApp::GetVerifFlag()
{
	switch(NCMComData::GetType())
	{
	case AT_MILL:
		return CM_VerifierM | CM_VerifierM5X;
	case AT_MILL_TURN:
		return CM_VerifierMT;
	case AT_TURN:
		return CM_VerifierT;
	case AT_WELD:
		return CM_VerifierW;
	default:
		return (ConfigMask)0;
	}
}
int CNCMApp::GetModuleNum(ConfigMask Module)
{
	// Returns the number of the first non zero bit
	int b = Module;
	for(int i = 0; i < MODULES_NUM; ++i)
	{
		if(b & 0x01)
			return i;
		b = b >> 1;
	}
	return 0;
}
bool CNCMApp::TruncateLicense(void)
{
	CString path = NCM_PROJECT.GetSettingsPath();
	TiXmlDocument ConfigXML;
	bool LoadOK = ConfigXML.LoadFile(path + _T("NetConfig.xml"));
	if(!LoadOK)
		return false; // Don't truncate license
	TiXmlNode* Node = ConfigXML.FirstChild(_T("Config"));
	if(Node == 0)
		return false; // Don't truncate license
	NCMConfig LoadedConf = 0x0;
	TiXmlElement* Module = Node->FirstChildElement(_T("Module"));
	if(Module == 0)
		return false; // Don't truncate license
	while(Module != 0)
	{
		int ModuleId = 0;
		if(Module->Attribute(_T("Id"), &ModuleId) == NULL)
			continue;
		LoadedConf |= 0x1 << ModuleId;
		Module = Module->NextSiblingElement(_T("Module"));
	}
	Configuration &= LoadedConf;
	return false;
}

bool CNCMApp::SetEditMachMode(void)
{
	if(!((CMainFrame *)m_pMainWnd)->CheckCutting())
		return false;
	CNCMDoc *pDoc = (CNCMDoc *)((CMainFrame *)m_pMainWnd)->GetActiveDocument();

	OnDontcutMode();
	((CMainFrame *)m_pMainWnd)->OnReset();
	pDoc->UpdateAllViews(NULL, N_OGL | N_STATE | N_LEFT_ATTR | N_ANALYSE );

	pDoc->MachToolEditor.Activate(pDoc->GetMachTool(), *((MTEditTreePane *)((CMainFrame *)m_pMainWnd)->GetMTEditTreePane()), *((MTEditPane *)((CMainFrame *)m_pMainWnd)->GetMTEditPane()),NCM_PROJECT.GetMachinesPath());
	GetGlobalState().SetEditMachMode(true);

	return true;
}

bool CNCMApp::StopEditMachMode(void)
{
	CNCMDoc *pDoc = (CNCMDoc *)((CMainFrame *)m_pMainWnd)->GetActiveDocument();
	if(pDoc->MachToolEditor.Deactivate())
	{
		GetGlobalState().SetEditMachMode(false);
		if(pDoc->MachToolEditor.IsMachineChanged())
			pDoc->RegeneratePrograms();
		pDoc->UpdateAllViews(NULL, N_LEFT | N_OGL | N_PROGRAMCHANGED);
		return true;
	}//if
	else
		return false;
}

void CNCMApp::SetImportMode(void)
{
	CNCMDoc *pDoc = (CNCMDoc *)((CMainFrame *)m_pMainWnd)->GetActiveDocument();
	if (!pDoc->SaveModified())
		return;
	pDoc->SetModifiedFlag(FALSE);
	pDoc->PrepClipboard();
	((CMainFrame *)m_pMainWnd)->StartImportMode();
	// Find path
	CString Path = NCM_PROJECT.GetToolsPath();
	if(Path.IsEmpty())
		OnFileOpen();
	else
	{
		CString Filter;
		VERIFY(Filter.LoadString(IDS_NCMPROJ_FILTER));
		Filter.Replace('|',(TCHAR)'\0');
		Filter += (TCHAR)'\0';// На случай ошибки в IDR_DEMOFILTER
		CFileDialog FileDialog(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_DONTADDTORECENT);
		FileDialog.m_ofn.lpstrInitialDir = Path;
		FileDialog.m_ofn.lpstrFilter = Filter;
		if(FileDialog.DoModal() == IDOK)
			OpenDocumentFile(FileDialog.GetPathName());
	}
}


CString CNCMApp::GetEDUPassword()
{
	CString Answer;
	HKEY hkResult;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\NCM40.EXE"), 0, KEY_QUERY_VALUE, &hkResult)
		== ERROR_SUCCESS)
	{
		unsigned char buf[256];
		unsigned long s = 256;
		if (RegQueryValueEx(hkResult, _T("Password"), NULL, NULL, buf, &s)
			== ERROR_SUCCESS)
		{
			Answer = CString(buf);
		}
	}
	if (Answer.IsEmpty())
		Answer = theApp.GetProfileString(NCMVersion, "Password", "Empty");
	return Answer;
}

void CNCMApp::ChangeConfigByRegistry()
{
	UINT RegVal = GetProfileInt(NCMVersion, "Configuration", 0xffffffff);
	CString Config = _T("65535");
#ifdef NCM_EDU
	Config = GetEDUPassword().Right(5);
#endif
	UINT EDUVal = atoi(Config);
	Configuration &= RegVal & EDUVal;
}

void CNCMApp::SetDXParams()
{
	NTiParams &Par = NCM_PROJECT.Defaults;
	CString Key(_T("Defaults/Stock/Mill/DXSize"));
	CString SubKeyMax(_T("@DXMax"));
	CString SubKeyMin(_T("@DXMin"));
	int Max = GQuadRenderDX5X::CellsNumScale * int(Par.GetDouble((Key + SubKeyMax).GetBuffer(), 10));
	int Min = GQuadRenderDX5X::CellsNumScale * int(Par.GetDouble((Key + SubKeyMin).GetBuffer(), 1));
	int New = GQuadRenderDX5X::GetWholeCellsNum();
	New = max(Min, New);
	New = min(Max, New);
	GQuadRenderDX5X::SetWholeCellsNum(New);
	GetGlobalState().SetModelMode(NGlobalState::DX);
	WriteProfileInt(NCMVersion, "ModelMode", NGlobalState::DX);
}

void CNCMApp::SetBSPParams()
{
	GetGlobalState().SetModelMode(NGlobalState::BSP);
	WriteProfileInt(NCMVersion, "ModelMode", NGlobalState::BSP);
}

BOOL CNCMApp::LoadState(LPCTSTR lpszSectionName, CFrameImpl* pFrameImpl)
{
	bool LoadLayoutFromTheProject = false;
	if (m_pMainWnd != nullptr)
	{
		OGLViewStyle Style = (OGLViewStyle)GetProfileInt(m_strRegSection, _T("MainViewStyle"), OG_Stock);
		if (!(theApp.GetConfig() & CM_Machine))
			Style = OG_Stock;
		switch (Style)
		{
		case OG_Mach:
			((CMainFrame*)m_pMainWnd)->OnWinMach();
			break;
		case OG_Stock:
		default:
			((CMainFrame*)m_pMainWnd)->OnWinStock();
			break;
		}
		((CMainFrame*)m_pMainWnd)->SetMainPane();
	}
	else
	{
#ifdef NC_DEMO
		NTiParams& Par = NCM_PROJECT.Defaults;
		LoadLayoutFromTheProject = Par.GetBool(_T("Defaults/Commands/LoadLayoutFromTheProject@Enable"), false);
#endif
	}
	if (LoadLayoutFromTheProject)
		return TRUE;
	else
		return CWinAppEx::LoadState(lpszSectionName, pFrameImpl);
}

BOOL CNCMApp::SaveState(LPCTSTR lpszSectionName, CFrameImpl* pFrameImpl)
{
	
	if (m_pMainWnd != nullptr)
		WriteProfileInt(m_strRegSection, _T("MainViewStyle"), ((CMainFrame*)m_pMainWnd)->IsMainViewStock() ? OG_Stock : OG_Mach);

	return CWinAppEx::SaveState(lpszSectionName, pFrameImpl);
}

bool CNCMApp::SaveParams4Rest()
{
	((CMainFrame*)m_pMainWnd)->OnReset();
	Params4Rest.CuttingMode = GetGlobalState().GetCuttingMode();
	if (!Params4Rest.CuttingMode)
	{
		if (!StartCutMode())
			return false;
	}
	Params4Rest.AnimMode = !NCM_PROJECT.GetGlobalState().GetResultMode();
	if (Params4Rest.AnimMode)
	{
		if (!CanResultModeBeEnabled())
			return false;
		NCM_PROJECT.GetGlobalState().SetResultMode();
		((CMainFrame*)m_pMainWnd)->ShowResultDlg();
	}
	CNCMDoc* pDoc = (CNCMDoc*)((CFrameWnd*)m_pMainWnd)->GetActiveDocument();
	Params4Rest.MSActive = pDoc->MileStones.IsActive();
	Params4Rest.Defined = true;
	Params4Rest.Started = true;
	return true;
}

bool CNCMApp::RestoreParams4Rest()
{
	if (!Params4Rest.Defined)
		return false;
	Params4Rest.Defined = false;
	if (Params4Rest.MSActive)
	{
		CNCMDoc* pDoc = (CNCMDoc*)((CFrameWnd*)m_pMainWnd)->GetActiveDocument();
		pDoc->MileStones.Activate();
	}

	if (Params4Rest.AnimMode)
	{
		if (!CanSmothnessModeBeEnabled())
			return false;
		GetGlobalState().SetAnimateMode();
		NSpeedPar::Extern.SetTrackFeed(false);
		((CMainFrame*)m_pMainWnd)->ShowPauseDlg();
	}

	if (!Params4Rest.CuttingMode)
	{
		OnDontcutMode();
	}
	Params4Rest.Started = false;
	return true;
}

bool CNCMApp::StartFullCollisionCheck()
{
	if (!SaveParams4Rest())
		return false;

	// switch collision detection on
	CNCMDoc* pDoc = (CNCMDoc*)((CFrameWnd*)m_pMainWnd)->GetActiveDocument();
	auto& CollMan = pDoc->GetCollManager();
	CollMan.SetToolStockActive(true);
	CollMan.SetMainWnd(AfxGetMainWnd());
	CollMan.SetCheckType(1, pDoc->GetMachTool());

	pDoc->MileStones.DeActivate();
	((CMainFrame*)m_pMainWnd)->OnRunToBreakpoint();

	return true;
}

bool CNCMApp::StopFullCollisionCheck()
{
	// switch collision detection off
	CNCMDoc* pDoc = (CNCMDoc*)((CFrameWnd*)m_pMainWnd)->GetActiveDocument();
	auto& CollMan = pDoc->GetCollManager();
	CollMan.SetToolStockActive(false);
	CollMan.SetCheckType(2, pDoc->GetMachTool());
	CollMan.StopCreating();

	pDoc->RegenerateStocks();// to make sharp edges deleted while collision check executes

	if (!RestoreParams4Rest())
		return false;
	return true;
}

bool CNCMApp::StopCuttingWRest()
{
	CNCMDoc* pDoc = (CNCMDoc*)((CFrameWnd*)m_pMainWnd)->GetActiveDocument();
	if (pDoc->GetCollManager().IsActive())
		return theApp.StopFullCollisionCheck();
	else
		return RestoreParams4Rest();
}

BOOL CNCMApp::OnIdle(LONG lCount) // return TRUE if more idle processing
{
	if (m_pMainWnd != nullptr)
	{
		CNCMDoc *pDoc = (CNCMDoc *)((CFrameWnd *)m_pMainWnd)->GetActiveDocument();
		if (pDoc != nullptr && !((CMainFrame *)m_pMainWnd)->IsCuttingInProgress())
		{
			BCurve::Storage.ClearExt();
		}
	}
	return CWinAppEx::OnIdle(lCount);
}

long CNCMApp::RegCloneBranch(HKEY hkeyDestRoot, HKEY hkeySrcRoot)
{
	long result = ERROR_SUCCESS;
	DWORD index;
	DWORD subkeys, maxkeyname, values, maxvaluename, maxdata, type;
	LPSTR lpName = NULL, lpData = NULL;

	/* get information, so that we know how much memory to allocate */
	result = RegQueryInfoKey(hkeySrcRoot, NULL, NULL, NULL, &subkeys, &maxkeyname,
		NULL, &values, &maxvaluename, &maxdata, NULL, NULL);
	if (result != ERROR_SUCCESS)
		return result;
	/* in Windows NT/2000/XP, the name lengths do not include the '\0' terminator */
	maxkeyname++;
	maxvaluename++;

	/* allocate buffers, one for data and one for value & class names */
	if (maxvaluename > maxkeyname)
		maxkeyname = maxvaluename;
	ASSERT(maxkeyname > 0);
	lpName = (LPSTR)malloc(maxkeyname);
	if (lpName == NULL) {
		result = ERROR_NOT_ENOUGH_MEMORY;
		goto error_exit;
	} /* if */
	if (maxdata > 0) {
		lpData = (LPSTR)malloc(maxdata);
		if (lpData == NULL) {
			result = ERROR_NOT_ENOUGH_MEMORY;
			goto error_exit;
		} /* if */
	}
	else {
		lpData = NULL;
	} /* if */

	/* first walk through the values */
	for (index = 0; index < values; index++) {
		DWORD namesize = maxkeyname;
		DWORD datasize = maxdata;
		ASSERT(lpData != NULL);
		result = RegEnumValue(hkeySrcRoot, index, lpName, &namesize, NULL, &type, (LPBYTE)lpData, &datasize);
		ASSERT(result != ERROR_MORE_DATA);
		if (result != ERROR_SUCCESS)
			goto error_exit;
		result = RegSetValueEx(hkeyDestRoot, lpName, 0L, type, (LPBYTE)lpData, datasize);
		if (result != ERROR_SUCCESS)
			goto error_exit;
	} /* for */
	/* no longer need the data block */
	if (lpData != NULL) {
		free(lpData);
		lpData = NULL;
	} /* if */

	/* no walk through all subkeys, and recursively call this function to copy the tree */
	for (index = 0; index < subkeys; index++) {
		DWORD namesize = maxkeyname;
		HKEY hkeySrc;
		HKEY hkeyDest;
		auto r = RegEnumKeyEx(hkeySrcRoot, index, lpName, &namesize, NULL, NULL, NULL, NULL);
		result = RegOpenKeyEx(hkeySrcRoot, lpName, 0L, KEY_READ, &hkeySrc);
		if (result != ERROR_SUCCESS)
			goto error_exit;
		result = RegCreateKeyEx(hkeyDestRoot, lpName, 0L, NULL, REG_OPTION_NON_VOLATILE,
			KEY_WRITE, NULL, &hkeyDest, NULL);
		if (result != ERROR_SUCCESS)
			goto error_exit;
		RegCloneBranch(hkeyDest, hkeySrc);
		RegCloseKey(hkeySrc);
		RegCloseKey(hkeyDest);
	} /* for */

	ASSERT(lpName != NULL);
	free(lpName);
	lpName = NULL;

	return ERROR_SUCCESS;

error_exit:
	if (lpName != NULL)
		free(lpName);
	if (lpData != NULL)
		free(lpData);
	return result;
}

long CNCMApp::RegSaveBranch(HKEY hkeySrcRoot, CArchive& ar)
{
	long result = ERROR_SUCCESS;
	DWORD subkeys, maxkeyname, values, maxvaluename, maxdata, type;
	LPSTR lpName = NULL, lpData = NULL;

	/* get information, so that we know how much memory to allocate */
	result = RegQueryInfoKey(hkeySrcRoot, NULL, NULL, NULL, &subkeys, &maxkeyname,
		NULL, &values, &maxvaluename, &maxdata, NULL, NULL);
	if (result != ERROR_SUCCESS)
		return result;
	/* in Windows the name lengths do not include the '\0' terminator */
	maxkeyname++;
	maxvaluename++;

	/* allocate buffers, one for data and one for value & class names */
	if (maxvaluename > maxkeyname)
		maxkeyname = maxvaluename;
	ASSERT(maxkeyname > 0);
	lpName = (LPSTR)malloc(maxkeyname);
	if (lpName == NULL)
	{
		result = ERROR_NOT_ENOUGH_MEMORY;
		goto error_exit;
	} /* if */
	SerializeElements(ar, &maxdata, 1);
	if (maxdata > 0)
	{
		lpData = (LPSTR)malloc(maxdata);
		if (lpData == NULL) 
		{
			result = ERROR_NOT_ENOUGH_MEMORY;
			goto error_exit;
		} /* if */
	}
	else
	{
		lpData = NULL;
	} /* if */

	SerializeElements(ar, &values, 1);
	/* first walk through the values */
	for (DWORD index = 0; index < values; index++)
	{
		DWORD namesize = maxkeyname;
		DWORD datasize = maxdata;
		ASSERT(lpData != NULL);
		result = RegEnumValue(hkeySrcRoot, index, lpName, &namesize, NULL, &type, (LPBYTE)lpData, &datasize);
		ASSERT(result != ERROR_MORE_DATA);
		if (result != ERROR_SUCCESS)
			goto error_exit;
		CString name(lpName);
		SerializeElements(ar, &name, 1);
		SerializeElements(ar, &type, 1);
		SerializeElements(ar, &datasize, 1);
		SerializeElements(ar, lpData, datasize);
		//result = RegSetValueEx(hkeyDestRoot, lpName, 0L, type, (LPBYTE)lpData, datasize);
		//if (result != ERROR_SUCCESS)
		//	goto error_exit;
	} /* for */
	/* no longer need the data block */
	if (lpData != NULL) {
		free(lpData);
		lpData = NULL;
	} /* if */

	/* no walk through all subkeys, and recursively call this function to copy the tree */
	SerializeElements(ar, &subkeys, 1);
	for (DWORD index = 0; index < subkeys; index++) 
	{
		DWORD namesize = maxkeyname;
		HKEY hkeySrc;
		auto r = RegEnumKeyEx(hkeySrcRoot, index, lpName, &namesize, NULL, NULL, NULL, NULL);
		result = RegOpenKeyEx(hkeySrcRoot, lpName, 0L, KEY_READ, &hkeySrc);
		if (result != ERROR_SUCCESS)
			goto error_exit;
		CString name(lpName);
		SerializeElements(ar, &name, 1);
		RegSaveBranch(hkeySrc, ar);
		RegCloseKey(hkeySrc);
	} /* for */

	ASSERT(lpName != NULL);
	free(lpName);
	lpName = NULL;

	return ERROR_SUCCESS;

error_exit:
	if (lpName != NULL)
		free(lpName);
	if (lpData != NULL)
		free(lpData);
	return result;
}

long CNCMApp::RegLoadBranch(HKEY hkeyDestRoot, CArchive& ar)
{
	long result = ERROR_SUCCESS;
	LPSTR lpData = NULL;
	DWORD maxdata;
	SerializeElements(ar, &maxdata, 1);
	if (maxdata > 0) 
	{
		lpData = (LPSTR)malloc(maxdata);
		if (lpData == NULL) 
		{
			result = ERROR_NOT_ENOUGH_MEMORY;
			goto error_exit;
		} /* if */
	}
	else {
		lpData = NULL;
	} /* if */

	DWORD values;
	SerializeElements(ar, &values, 1);
	/* first walk through the values */
	for (DWORD index = 0; index < values; index++) 
	{
		DWORD datasize;
		CString name;
		DWORD type;
		SerializeElements(ar, &name, 1);
		SerializeElements(ar, &type, 1);
		SerializeElements(ar, &datasize, 1);
		if (datasize > maxdata)
			return -1;
		SerializeElements(ar, lpData, datasize);
		result = RegSetValueEx(hkeyDestRoot, name, 0L, type, (LPBYTE)lpData, datasize);
		if (result != ERROR_SUCCESS)
			goto error_exit;
	} /* for */
	/* no longer need the data block */
	if (lpData != NULL) 
	{
		free(lpData);
		lpData = NULL;
	} /* if */

	/* no walk through all subkeys, and recursively call this function to copy the tree */
	DWORD subkeys;
	SerializeElements(ar, &subkeys, 1);
	for (DWORD index = 0; index < subkeys; index++)
	{
		HKEY hkeyDest;
		CString name;
		SerializeElements(ar, &name, 1);
		result = RegCreateKeyEx(hkeyDestRoot, name, 0L, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hkeyDest, NULL);
		if (result != ERROR_SUCCESS)
			goto error_exit;
		result = RegLoadBranch(hkeyDest, ar);
		if (result != ERROR_SUCCESS)
			goto error_exit;
		RegCloseKey(hkeyDest);
	} /* for */

	return ERROR_SUCCESS;

error_exit:
	if (lpData != NULL)
		free(lpData);
	return result;
}

void CNCMApp::NParams4Coll::Reset()
{
	AnimMode = false;
	CuttingMode = false;
	MSActive = false;
	Defined = false;
	Started = false;
}
