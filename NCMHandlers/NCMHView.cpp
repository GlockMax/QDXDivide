
// NCMView.cpp : implementation of the CNCMView class
//

#include "stdafx.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "NCM.h"
#endif

#include "NCMHDoc.h"
#include "NCMHView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNCMView

IMPLEMENT_DYNCREATE(CNCMHView, CView)

BEGIN_MESSAGE_MAP(CNCMHView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
ON_WM_LBUTTONDOWN()
ON_WM_MOUSEMOVE()
ON_WM_MBUTTONDOWN()
ON_WM_MBUTTONUP()
END_MESSAGE_MAP()

// CNCMView construction/destruction

CNCMHView::CNCMHView() noexcept
{
}

CNCMHView::~CNCMHView()
{
}


// CNCMView drawing

void CNCMHView::OnDraw(CDC* /*pDC*/)
{
	OpenGLWnd.NUpdate();
}

// CNCMView message handlers


void CNCMHView::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);
	OpenGLWnd.SetFocus();
}

int CNCMHView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rect;
	GetClientRect(rect);
	bool Ret = (TRUE == OpenGLWnd.CreateEx(WS_EX_CLIENTEDGE, NULL,   //CWnd default
		NULL,   //has no name
		WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE,
		rect,
		this,   //this is the parent
		IDS_STOCK_MACH_PANE_1));
	return 0;
}



void CNCMHView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	CRect rect;
	GetClientRect(&rect);
	if (OpenGLWnd.GetSafeHwnd())
		OpenGLWnd.MoveWindow(rect);
}

