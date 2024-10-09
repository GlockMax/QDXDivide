
// NCEditor2.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
//#include "vld.h"
#include "afxwinappex.h"
#include "NCMControl.h"
#include "MainFrm.h"

#include "NCEditor2Doc.h"
#include "NCEditor2View.h"

#include "NCMVersion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNCEditor2App

BEGIN_MESSAGE_MAP(CNCEditor2App, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CNCEditor2App::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CNCEditor2App construction

CNCEditor2App::CNCEditor2App()
{
	m_bHiColorIcons = TRUE;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CNCEditor2App object

CNCEditor2App theApp;


// CNCEditor2App initialization

BOOL CNCEditor2App::InitInstance()
{
	//BOOL asdfdadf = SetThreadLocale(MAKELCID(MAKELANGID(LANG_RUSSIAN,SUBLANG_DEFAULT),SORT_DEFAULT));

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Inteks\\NCManager"));

	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)

	workPath = GetProfileString(_T("Settings"),_T("Working Path"));
	
	/// —читываем из реестра код €зыка 
	/// 1 - открываем раздел реестра
	HKEY hkResult;
	CString regPath = _T("Software\\Inteks\\NCManager\\") + NCMVersion;
	LONG res;
	res = RegOpenKeyEx(HKEY_CURRENT_USER, regPath, 0, KEY_QUERY_VALUE, &hkResult);
	if(res != ERROR_SUCCESS)
	{
		AfxMessageBox(IDS_SETUP_FAULT);
		exit(1);
	}
	/// 2 - —читываем значение DWORD из параметра Language
	DWORD dLang, dLen;
	if(RegQueryValueEx(hkResult, _T("Language"), NULL, NULL, (LPBYTE) &dLang, &dLen) != ERROR_SUCCESS)
	{
		dLang = 0;
	}//if
	
	/// 3 - —читываем путь, куда установлен NCManager
	CSettingsStore *m_pRegistry = new CSettingsStore(FALSE,TRUE);
	if( m_pRegistry &&
		m_pRegistry->Open(regPath))
	{
		if(m_pRegistry->Read(_T("MainPath"),workPath) == 0)
		{
			AfxMessageBox(IDS_SETUP_FAULT);
			exit(1);
		}//if
		m_pRegistry->Close();
	}//if
	else
	{
		AfxMessageBox(IDS_SETUP_FAULT);
		exit(1);
	}//else

	delete m_pRegistry;


	/// 4 - мен€ем €зык
	if(dLang != 0)
		SetThreadLocale(dLang);

	//InitContextMenuManager();

	//InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CNCEditor2Doc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CNCEditor2View));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	return TRUE;
}

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CNCEditor2App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CNCEditor2App customization load/save methods

void CNCEditor2App::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CNCEditor2App::LoadCustomState()
{
}

void CNCEditor2App::SaveCustomState()
{
}

// CNCEditor2App message handlers




void CNCEditor2App::OnEditUndo()
{
	// TODO: Add your command handler code here
}

int CNCEditor2App::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class

	return CWinAppEx::ExitInstance();
}
