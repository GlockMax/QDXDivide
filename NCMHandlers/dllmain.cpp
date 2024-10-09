// dllmain.cpp : Implementation of DllMain.

#include "stdafx.h"
#include "framework.h"
#include "NCMVersion.h"
#include "NCMProject.h"
#include "resource.h"
#include "NCMHandlers_i.h"
#include "dllmain.h"
#include "xdlldata.h"

CNCMHandlersModule _AtlModule;

class CNCMHandlersApp : public CWinApp
{
public:

// Overrides
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CNCMHandlersApp, CWinApp)
END_MESSAGE_MAP()

CNCMHandlersApp theApp;

BOOL CNCMHandlersApp::InitInstance()
{
	if (!PrxDllMain(m_hInstance, DLL_PROCESS_ATTACH, nullptr))
		return FALSE;

	free((void*)m_pszAppName);
	m_pszAppName = _tcsdup(_T("NCManager"));

	// Change the registry key under which our settings are stored.
	SetRegistryKey(_T("Inteks"));

	NCM_PROJECT.GetGlobalState().SetViewInterface(CStringA(_T("NCManager")));
	return CWinApp::InitInstance();
}

int CNCMHandlersApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}
