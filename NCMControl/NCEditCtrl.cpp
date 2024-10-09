#include "StdAfx.h"
#include "NCEditCtrl.h"

CNCEditCtrl::CNCEditCtrl(void)
{
}

CNCEditCtrl::~CNCEditCtrl(void)
{
}
BEGIN_MESSAGE_MAP(CNCEditCtrl, CEdit)
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

void CNCEditCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: Add your message handler code here
}
