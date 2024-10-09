// NSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NSettingsDlg.h"
#include "Mainfrm.h"
#include "NCMDoc.h"
#include "NCUnit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NSettingsDlg

IMPLEMENT_DYNAMIC(NSettingsDlg, CMFCPropertySheet)

BEGIN_MESSAGE_MAP(NSettingsDlg, CMFCPropertySheet)
	//{{AFX_MSG_MAP(NSettingsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NSettingsDlg message handlers

NSettingsDlg::NSettingsDlg()
	:CMFCPropertySheet(IDS_SETTINGS, NULL, 0)
{
	m_psh.dwFlags |= PSH_NOAPPLYNOW;

	CWnd *pSV = ((CMainFrame *)AfxGetMainWnd())->GetUnitPane();
	ASSERT(pSV->IsKindOf(RUNTIME_CLASS(NStateUnitPane)));
	StateSettDlg.SetParamsPane(*(NStateUnitPane *)pSV);


	AddPage(&AppearSettDlg);
	AddPage(&StateSettDlg);
	AddPage(&GraphSettDlg); 
	AddPage(&PathSettDlg);
	AddPage(&ViewInterfaceSettingsDlg);
}

NSettingsDlg::~NSettingsDlg()
{
	//delete pAppearSettDlg;
	//delete pStateSettDlg;
	//delete pGraphSettDlg;
}

BOOL NSettingsDlg::OnInitDialog() 
{
	BOOL bResult = CMFCPropertySheet::OnInitDialog();
	ModifyStyleEx(0, WS_EX_CONTEXTHELP);
	return bResult;
}

INT_PTR NSettingsDlg::DoModal() 
{
	SetLook(CMFCPropertySheet::PropSheetLook_OneNoteTabs);
	INT_PTR ret = CMFCPropertySheet::DoModal();
	if(ret == IDOK)
		((CNCMDoc *)((CMainFrame *)AfxGetMainWnd())->GetActiveDocument())->UpdateAllViews(NULL, N_SETTINGS | N_SELECTTEXT | N_STATE);
	return ret;
}

