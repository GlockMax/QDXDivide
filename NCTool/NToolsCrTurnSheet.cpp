// NToolsCrTurnSheet.cpp : implementation file
//

#include "stdafx.h"
#include "ResLocal.h"
#include "NToolsCrTBOut.h"
#include "NToolsCrTBCirc.h"
#include "NToolsCrTBCut.h"
#include "NToolsCrTBISO.h"
#include "NToolsCrTBThr.h"
#include "NToolsCrTurnSheet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


// NToolsCrTurnSheet

IMPLEMENT_DYNAMIC(NToolsCrTurnSheet, CPropertySheet)
NToolsCrTurnSheet::NToolsCrTurnSheet(CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(IDS_TURN_B_SHEET, pParentWnd, iSelectPage)
	, pTool(NULL)
{
	AddPage(new NToolsCrTBOut); 
	AddPage(new NToolsCrTBCirc); 
	AddPage(new NToolsCrTBCut); 
	AddPage(new NToolsCrTBISO); 
	AddPage(new NToolsCrTBThr); 
	m_psh.dwFlags |= PSH_NOAPPLYNOW ;

}


NToolsCrTurnSheet::~NToolsCrTurnSheet()
{
	for(int i = 0; i < GetPageCount(); ++i)
		delete GetPage(i);
}


BEGIN_MESSAGE_MAP(NToolsCrTurnSheet, CPropertySheet)
	ON_COMMAND(IDOK, OnOK)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// NToolsCrTurnSheet message handlers
void NToolsCrTurnSheet::OnOK()
{
	pTool = ((NToolsCrTurn *)GetActivePage())->CreateTool();
	if(!pTool)
		return;
	EndDialog( IDOK );
}
BOOL NToolsCrTurnSheet::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	CTabCtrl *Tab = CPropertySheet::GetTabControl();
	ASSERT(Tab != NULL);
	ToolPics.Create(IDB_TOOL_TURNB_PICS,24,10,RGB(255,255,255));
	Tab->SetImageList(&ToolPics);
	TCITEM tc;
	tc.mask = TCIF_IMAGE;
	for (int i = 0; i < Tab->GetItemCount(); i++)
	{
		Tab->GetItem(i, &tc);
		tc.iImage = i;
		Tab->SetItem(i, &tc);
	}
	SetActivePage(GetActiveIndex());// To redraw 
	return bResult;
}

int NToolsCrTurnSheet::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}
