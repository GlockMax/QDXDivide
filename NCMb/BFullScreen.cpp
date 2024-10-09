// BFullScreen.cpp : implementation file
//

#include "stdafx.h"
#include "BFullScreen.h"


// BFullScreen

BFullScreen::BFullScreen(void)
{
	Active = false;
	HMenu = NULL;
}

BFullScreen::~BFullScreen()
{
}


// BFullScreen member functions
bool BFullScreen::Activate(CFrameWnd *pP)
{
	if(Active)
		return false;

	pParent = pP;
	HMenu = pParent->GetMenu()->m_hMenu;
	pParent->SetMenu(NULL);
	pParent->DrawMenuBar();
	CControlBar* pBar = pParent->GetControlBar(ID_VIEW_STATUS_BAR);
	if (pBar)
		pParent->ShowControlBar(pBar, FALSE, FALSE);
	pParent->ModifyStyle(WS_CAPTION, 0, SWP_FRAMECHANGED);

	pParent->RedrawWindow();

	Active = true;
	return true;
}
bool BFullScreen::DeActivate(void)
{
	if(!Active)
		return false;
	::SetMenu(pParent->m_hWnd, HMenu);
//	pParent->SetMenu(CMenu);
	HMenu = NULL;
	pParent->DrawMenuBar();
	CControlBar* pBar = pParent->GetControlBar(ID_VIEW_STATUS_BAR);
	if (pBar)
		pParent->ShowControlBar(pBar, TRUE, FALSE);
	pParent->ModifyStyle(0, WS_CAPTION, SWP_FRAMECHANGED);

	pParent->RedrawWindow();

	pParent = NULL;

	Active = false;
	return true;
}
