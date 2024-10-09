// NViewToolBar.cpp : implementation file
//

#include "stdafx.h"
#include "ResLocal.h"
#include "NViewToolBar.h"


// NViewToolBar

IMPLEMENT_DYNAMIC(NViewToolBar, CToolBar)

NViewToolBar::NViewToolBar()
{

}

NViewToolBar::~NViewToolBar()
{
}


BEGIN_MESSAGE_MAP(NViewToolBar, CToolBar)
	//{{AFX_MSG_MAP(CDlgToolBar)
	ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

LRESULT NViewToolBar::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM)
{
	if (IsWindowVisible())
	{
		CFrameWnd *pParent = (CFrameWnd *)GetParent();
		if (pParent)
			OnUpdateCmdUI(pParent, (BOOL)wParam);
	}
	return CToolBar::OnIdleUpdateCmdUI(wParam, 0);
}
