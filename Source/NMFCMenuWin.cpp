// NMFCMenuWin.cpp : implementation file
//

#include "stdafx.h"
#include "NCM.h"
#include "NMFCMenuWin.h"

IMPLEMENT_DYNCREATE(NMFCMenuWin, CMFCToolBarMenuButton)
// NMFCMenuWin

NMFCMenuWin::NMFCMenuWin()
{
}

NMFCMenuWin::~NMFCMenuWin()
{
}

SIZE NMFCMenuWin::OnCalculateSize(CDC* pDC, const CSize& sizeDefault, BOOL bHorz)
{
	SIZE size = CMFCToolBarMenuButton::OnCalculateSize(pDC, sizeDefault, bHorz);
	size.cx = BUTTON_NWIDTH;
	return size;
}
// NMFCMenuWin member functions
