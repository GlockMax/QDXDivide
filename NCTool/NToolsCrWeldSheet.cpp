// NToolsCrWeldSheet.cpp : implementation file
//

#include "stdafx.h"
#include "ResLocal.h"
#include "NToolsCrWeldPage.h"
#include "NToolsCrWeldSheet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


// NToolsCrWeldSheet

IMPLEMENT_DYNAMIC(NToolsCrWeldSheet, CPropertySheet)
NToolsCrWeldSheet::NToolsCrWeldSheet(CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(IDS_BEAM, pParentWnd, iSelectPage)
	, pTool(NULL)
{
	AddPage(new NToolsCrWeldPage); 
	m_psh.dwFlags |= PSH_NOAPPLYNOW ;

}


NToolsCrWeldSheet::~NToolsCrWeldSheet()
{
	for(int i = 0; i < GetPageCount(); ++i)
		delete GetPage(i);
}


BEGIN_MESSAGE_MAP(NToolsCrWeldSheet, CPropertySheet)
	ON_COMMAND(IDOK, OnOK)
	ON_COMMAND(IDCANCEL, OnCancel)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// NToolsCrWeldSheet message handlers
void NToolsCrWeldSheet::OnOK()
{
	pTool = ((NToolsCrWeldPage *)GetActivePage())->CreateTool();
	if(!pTool)
		return;
	//int IsErr = pTool->GenInit();	
	//if (IsErr != 0)
	//{
	//}
	EndDialog( IDOK );
}
void NToolsCrWeldSheet::OnCancel()
{
	pTool = ((NToolsCrWeldPage *)GetActivePage())->CreateTool();
	delete pTool;
	pTool = NULL;
	EndDialog( IDCANCEL );
}
BOOL NToolsCrWeldSheet::OnInitDialog()
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

int NToolsCrWeldSheet::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}
