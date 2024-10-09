// DlgProxy.cpp : implementation file
//

#include "stdafx.h"
#include "NCMFileInfo.h"
#include "DlgProxy.h"
#include "NCMFileInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNCMFileInfoDlgAutoProxy

IMPLEMENT_DYNCREATE(CNCMFileInfoDlgAutoProxy, CCmdTarget)

CNCMFileInfoDlgAutoProxy::CNCMFileInfoDlgAutoProxy()
{
	EnableAutomation();
	
	// To keep the application running as long as an automation 
	//	object is active, the constructor calls AfxOleLockApp.
	AfxOleLockApp();

	// Get access to the dialog through the application's
	//  main window pointer.  Set the proxy's internal pointer
	//  to point to the dialog, and set the dialog's back pointer to
	//  this proxy.
	ASSERT (AfxGetApp()->m_pMainWnd != NULL);
	ASSERT_VALID (AfxGetApp()->m_pMainWnd);
	ASSERT_KINDOF(CNCMFileInfoDlg, AfxGetApp()->m_pMainWnd);
	m_pDialog = (CNCMFileInfoDlg*) AfxGetApp()->m_pMainWnd;
	m_pDialog->m_pAutoProxy = this;
}

CNCMFileInfoDlgAutoProxy::~CNCMFileInfoDlgAutoProxy()
{
	// To terminate the application when all objects created with
	// 	with automation, the destructor calls AfxOleUnlockApp.
	//  Among other things, this will destroy the main dialog
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CNCMFileInfoDlgAutoProxy::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CNCMFileInfoDlgAutoProxy, CCmdTarget)
	//{{AFX_MSG_MAP(CNCMFileInfoDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CNCMFileInfoDlgAutoProxy, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CNCMFileInfoDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_INCMFileInfo to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {679A9CC9-C2D0-11D5-B0AC-0050BF4A8273}
static const IID IID_INCMFileInfo =
{ 0x679a9cc9, 0xc2d0, 0x11d5, { 0xb0, 0xac, 0x0, 0x50, 0xbf, 0x4a, 0x82, 0x73 } };

BEGIN_INTERFACE_MAP(CNCMFileInfoDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CNCMFileInfoDlgAutoProxy, IID_INCMFileInfo, Dispatch)
END_INTERFACE_MAP()

// The IMPLEMENT_OLECREATE2 macro is defined in StdAfx.h of this project
// {679A9CC7-C2D0-11D5-B0AC-0050BF4A8273}
IMPLEMENT_OLECREATE2(CNCMFileInfoDlgAutoProxy, "NCMFileInfo.Application", 0x679a9cc7, 0xc2d0, 0x11d5, 0xb0, 0xac, 0x0, 0x50, 0xbf, 0x4a, 0x82, 0x73)

/////////////////////////////////////////////////////////////////////////////
// CNCMFileInfoDlgAutoProxy message handlers
