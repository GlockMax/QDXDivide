// NPropPane.cpp : implementation file
//

#include "stdafx.h"
#include "MainFrm.h"
#include "NPropPane.h"


// NPropPane

IMPLEMENT_DYNAMIC(NPropPane, PropPane)

NPropPane::NPropPane()
{

}

NPropPane::~NPropPane()
{
}


BEGIN_MESSAGE_MAP(NPropPane, PropPane)
END_MESSAGE_MAP()

BOOL NPropPane::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if(message == WM_IDLEUPDATECMDUI)
	{
		CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
		if(pMain)
			if(::IsWindow(m_hWnd) && ::IsWindow(m_wndPropList))
			{
				m_wndPropList.SetEnabled(!pMain->IsCuttingInProgress() && !pMain->IsComparisonActive());
			}
	}
	return PropPane::OnWndMsg(message, wParam, lParam, pResult);
}
