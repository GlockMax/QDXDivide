
// NetConfigEdit.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "NCMVersion.h"
#include "NCM.h"
#include "tinyxml.h"
#include "NCommonPaths.h"
#include "NetConfigEdit.h"
#include "NetConfigEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNetConfigEditApp

BEGIN_MESSAGE_MAP(CNetConfigEditApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CNetConfigEditApp construction

CNetConfigEditApp::CNetConfigEditApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CNetConfigEditApp object

CNetConfigEditApp theApp;


// CNetConfigEditApp initialization

BOOL CNetConfigEditApp::InitInstance()
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

// Set language
	NCMLanguage NCMLang = NCMLanguage(GetProfileInt(NCMVersion, "NCMLanguage", NL_UND));
// Load resources
//	AfxLoadLibrary(_T("ResInv.DLL"));
	switch(NCMLang)
	{
	case NL_RUS:
		AfxLoadLibrary(_T("local\\Rus\\Resources.DLL"));
		break;
	case NL_ENG:
		AfxLoadLibrary(_T("local\\Eng\\Resources.DLL"));
		break;
	}
//// Set language
//	DWORD lang = GetProfileInt(NCMVersion, _T("Language"), MAKELCID(MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),SORT_DEFAULT));
//	if(lang != 0)
//		SetThreadLocale(lang);
//
//	HMODULE hins = LoadLibraryEx("NCMLOC.DLL",NULL,LOAD_LIBRARY_AS_DATAFILE);
//	if( hins == NULL )
//	{
//		AfxMessageBox("NCMLOC.DLL not found!");
//		return FALSE;
//	}
//	AfxSetResourceHandle(hins);

	NCommonPaths CommonPaths(NCMVersion);
	CString path = CommonPaths.GetPath(CF_SETTINGS);
	TiXmlDocument ConfigXML;
	bool LoadOK = ConfigXML.LoadFile(path + "NetConfig.xml");
	if(!LoadOK)
	{
		AfxMessageBox(IDS_NETCONFIGNOTFOUND);
		return FALSE; 
	}

	TiXmlNode* Node = ConfigXML.FirstChild(_T("Config"));
	if(Node == 0)
	{
		AfxMessageBox(IDS_NETCONFIGNOTFOUND);
		return FALSE; 
	}
	NCMConfig LoadedConf = 0x0;
	TiXmlElement* FirstModule = Node->FirstChildElement(_T("Module"));
	if(FirstModule == 0)
	{
		AfxMessageBox(IDS_NETCONFIGNOTFOUND);
		return FALSE; 
	}
	TiXmlElement* Module = FirstModule;
	while(Module != 0)
	{
		int ModuleId = 0;
		if(Module->Attribute(_T("Id"), &ModuleId) == NULL)
			continue;
		LoadedConf |= 0x1 << ModuleId;
		Module = Module->NextSiblingElement(_T("Module"));
	}

	CNetConfigEditDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal(LoadedConf);

	if (nResponse == IDOK)
	{
		TiXmlElement* NewModule = new TiXmlElement(*FirstModule);
		TiXmlElement* PrevModule = FirstModule;
		NCMConfig NewConfig = dlg.GetConfig();
		while(Node->FirstChildElement())
		{
			Node->RemoveChild(Node->FirstChildElement());
		}
		for(int i = 0; i < sizeof(NewConfig) * 8; ++i)
		{
			if(!(NewConfig & 0x1 << i))
				continue;
			NewModule->SetAttribute(_T("Id"), i);
			Node->InsertEndChild(*NewModule);
		}
		delete NewModule;
		ConfigXML.SaveFile();
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
