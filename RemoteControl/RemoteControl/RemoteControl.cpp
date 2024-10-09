
// RemoteControl.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "NCMVersion.h"
#include "RemoteControl.h"
#include "RemoteControlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
enum NCMLanguage
{
	NL_UND,
	NL_RUS,
	NL_ENG,
	NL_KOR
};


// CRemoteControlApp

BEGIN_MESSAGE_MAP(CRemoteControlApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CRemoteControlApp construction

CRemoteControlApp::CRemoteControlApp()
{
	EnableHtmlHelp();
	Version = NCMVersion;
}


// The one and only CRemoteControlApp object

CRemoteControlApp theApp;


// CRemoteControlApp initialization

BOOL CRemoteControlApp::InitInstance()
{
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

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Inteks"));
	m_pszProfileName = _tcsdup(_T("NCManager"));
	CString Path = GetProfileString(Version, _T("MainPath"));

// Set language
	NCMLanguage NCMLang = NCMLanguage(GetProfileInt(NCMVersion, "NCMLanguage", NL_UND));
	LCID DefLang = GetThreadLocale();
	if (NCMLang == NL_UND)
	{// Language is not defined
		NCMLang = NL_RUS;
	}
	switch (NCMLang)
	{
	case NL_RUS:
		if (DefLang != MAKELCID(MAKELANGID(LANG_RUSSIAN, SUBLANG_DEFAULT), SORT_DEFAULT))
			SetThreadUILanguage(MAKELCID(MAKELANGID(LANG_RUSSIAN, SUBLANG_DEFAULT), SORT_DEFAULT));
		break;
	default:
		SetThreadUILanguage(MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT));
		break;
	}

	CRemoteControlDlg dlg;
	m_pMainWnd = &dlg;

	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
