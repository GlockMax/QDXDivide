// NDummyView.cpp : implementation file
//

#include "stdafx.h"
#include "NOpenGLWnd.h"
#include "NOGLPane.h"
#include "NCMDoc.h"
#include "NDummyView.h"


// NDummyView

IMPLEMENT_DYNCREATE(NDummyView, CView)

BEGIN_MESSAGE_MAP(NDummyView, CView)
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

NDummyView::NDummyView()
{
	pPane = NULL;
}

NDummyView::~NDummyView()
{
}


void NDummyView::OnDraw(CDC* /*pDC*/)
{
}


void NDummyView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	CFrameWndEx *pMainFrame = (CFrameWndEx *)GetParentFrame();
	if(!pMainFrame)
		return;
	CRect rect;
	GetClientRect(&rect);
	ClientToScreen(&rect);
	pMainFrame->ScreenToClient(&rect);

	if(pPane)
		if(pPane->GetSafeHwnd())
			pPane->MoveWindow(rect);
}

void NDummyView::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);

	//if(pPane)
	//	pPane->SetFocus();
}

BOOL NDummyView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
#ifdef NC_DEMO
	if(nID == ID_FILE_SAVE_AS || nID == ID_SAVE_ENVIRONMENT || nID == ID_FILE_SAVE_AS_NCMPRT)
		return FALSE;
#endif
	if ((nID == ID_FILE_SAVE
		|| nID == ID_FILE_SAVE_AS || nID == ID_FILE_SAVE_AS_NCMPRT) && nCode == CN_COMMAND && pHandlerInfo == NULL)
	{
		CNCMDoc* pDoc = (CNCMDoc*)GetDocument();
		if (pDoc->IsNCMAll())
		{
			nID = ID_SAVE_ENVIRONMENT;
			AfxMessageBox(IDS_SAVE_NCMALL_MES);
		}
		return CView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	}
	// let the pane have first crack at the command
	if(pPane)
		if(pPane->GetOpenGLWnd())
		{
			if(pPane->GetOpenGLWnd()->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
				return TRUE;
		}

	return CView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void NDummyView::SetPane(class NOGLPane *pMainPane)
{
	pPane = pMainPane;
	if(pPane)
	{
		CRect rect;
		GetClientRect(&rect);
		ClientToScreen(&rect);
		AfxGetMainWnd()->ScreenToClient(&rect);

		pPane->MoveWindow(rect);
		pPane->ShowPane(TRUE, TRUE, TRUE);
		pPane->OnUpdate(NULL, N_SETTINGS, NULL);
	}
}

