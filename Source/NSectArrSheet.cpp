#include "stdafx.h"
#include "WM_APP_Messages.h"
#include "NWeldPart.h"
#include "NSectArrParams.h"
#include "NSectArrLDlg.h"
#include "NSectArrCDlg.h"
#include "NSectArrSheet.h"

IMPLEMENT_DYNAMIC(NSectArrSheet, CMFCPropertySheet)

BEGIN_MESSAGE_MAP(NSectArrSheet, CMFCPropertySheet)
	//{{AFX_MSG_MAP(NSettingsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

NSectArrSheet::NSectArrSheet(NWeldPart &iWeldPart) 
	: CMFCPropertySheet(IDS_CH_PART_WSECT),
	WeldPart(iWeldPart)
{
}

NSectArrSheet::~NSectArrSheet(void)
{
}

BOOL NSectArrSheet::OnInitDialog() 
{
	BOOL bResult = CMFCPropertySheet::OnInitDialog();
	ModifyStyleEx(WS_EX_CONTEXTHELP, 0);
	return bResult;
}

INT_PTR NSectArrSheet::DoModal() 
{
	SetLook(CMFCPropertySheet::PropSheetLook_OneNoteTabs);
	WeldPart.SetVisibleSect(true);
	NSectArrLDlg PageLine;
	NSectArrCDlg PageCircle;
	AddPage(&PageCircle);
	AddPage(&PageLine);
	if(WeldPart.GetSectParams().Type == SAT_CIRCLE)
	{
		PageCircle.SetParams(WeldPart.GetSectParams());
		PageLine.SetParams(NSectArrParams().SetDefault(SAT_LINE));
		SetActivePage(&PageCircle);
	}
	else
	{
		PageCircle.SetParams(NSectArrParams().SetDefault(SAT_CIRCLE));
		PageLine.SetParams(WeldPart.GetSectParams());
		SetActivePage(&PageLine);
	}
	INT_PTR ret = CMFCPropertySheet::DoModal();
	//if(ret == IDOK)
	//	((CNCMDoc *)((CMainFrame *)AfxGetMainWnd())->GetActiveDocument())->UpdateAllViews(NULL, N_SETTINGS | N_SELECTTEXT | N_STATE);
	return ret;
}

BOOL NSectArrSheet::OnCommand(WPARAM wParam, LPARAM lParam)
{
   if(LOWORD(wParam) == ID_APPLY_NOW || LOWORD(wParam) == IDOK)
   {
	   NSectArrDlg *pActPage = (NSectArrLDlg *)GetActivePage();
	   WeldPart.GetSectParams() = pActPage->GetParams();
	   WeldPart.MakeStockSections();
	   if(LOWORD(wParam) == ID_APPLY_NOW)
		   AfxGetMainWnd()->SendMessage(WM_APP_UPDATE_PANES, N_OGL);
   }

   return CPropertySheet::OnCommand(wParam, lParam);
}

