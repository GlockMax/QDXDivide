// NToolsCrTurnHoldSheet.cpp : implementation file
//

#include "stdafx.h"
#include "NToolsCrTHBar.h"
#include "NToolsCrTHCyl.h"
#include "NToolsCrTurnHoldSheet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


// NToolsCrTurnHoldSheet

IMPLEMENT_DYNAMIC(NToolsCrTurnHoldSheet, CPropertySheet)
NToolsCrTurnHoldSheet::NToolsCrTurnHoldSheet(CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(IDS_TURN_H_SHEET, pParentWnd, iSelectPage)
	, pTool(NULL)
{
	AddPage(new NToolsCrTHBar); 
	AddPage(new NToolsCrTHCyl); 
	m_psh.dwFlags |= PSH_NOAPPLYNOW ;
}


NToolsCrTurnHoldSheet::~NToolsCrTurnHoldSheet()
{
	for(int i = 0; i < GetPageCount(); ++i)
		delete GetPage(i);
}


BEGIN_MESSAGE_MAP(NToolsCrTurnHoldSheet, CPropertySheet)
	ON_COMMAND(IDOK, OnOK)
END_MESSAGE_MAP()


// NToolsCrTurnHoldSheet message handlers
void NToolsCrTurnHoldSheet::OnOK()
{
	pTool = ((NToolsCrTurn *)GetActivePage())->CreateTool();
	if(!pTool)
		return;
	EndDialog( IDOK );
}