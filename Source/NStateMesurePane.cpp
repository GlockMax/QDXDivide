// NStateMesurePane.cpp : implementation file
//

#include "stdafx.h"
#include "NCMProject.h"
#include "NCMDoc.h"
#include "NCUnit.h"
#include "NStateMesurePane.h"
#include "NColor.h"
#include "NFont.h"
#include "NControlPanel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NStateMesurePane

IMPLEMENT_DYNCREATE(NStateMesurePane, CDockablePane)

NStateMesurePane::NStateMesurePane()
{
}

NStateMesurePane::~NStateMesurePane()
{
}


BEGIN_MESSAGE_MAP(NStateMesurePane, CDockablePane)
	//{{AFX_MSG_MAP(NStateMesurePane)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




/////////////////////////////////////////////////////////////////////////////
// NStateMesurePane diagnostics

#ifdef _DEBUG
void NStateMesurePane::AssertValid() const
{
	CDockablePane::AssertValid();
}

void NStateMesurePane::Dump(CDumpContext& dc) const
{
	CDockablePane::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// NStateMesurePane message handlers

void NStateMesurePane::OnUpdate(CDockablePane* pSender, LPARAM lHint, CObject* pHint) 
{
 // Update colors
	if(lHint & N_SETTINGS)
	{
		MeasureCtrl.SetSettings();
		return;
	}
	if(lHint & N_STATE_MEASURE)
	{
		ShowPane(TRUE, FALSE, TRUE);
		lHint |= N_STATE;
	}
	if(lHint & N_STATE)
	{
		if (IsPaneVisible())
			UpdateMeasure();
		return;
	}
}

void NStateMesurePane::OnSize(UINT nType, int cx, int cy) 
{
	CDockablePane::OnSize(nType, cx, cy);
	
	CRect wrect;
	GetClientRect(&wrect);
	MeasureCtrl.MoveWindow(wrect, TRUE);
}

int NStateMesurePane::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	MeasureCtrl.Create( WS_TABSTOP | WS_CHILD | WS_BORDER | WS_VISIBLE 
   | TVS_DISABLEDRAGDROP,   CRect(0, 0, 0, 0), this, ID_MEASURECTRL);
	
	return 0;
}

//void NStateMesurePane::Update()
//{
//	UpdateMeasure();
//	UpdateWindow();
//}

void NStateMesurePane::UpdateMeasure()
{
	CNCMDoc *pDoc = (CNCMDoc *)GetDocument();
	if(!pDoc)
		return;
	MeasureCtrl.SetMeasure(&pDoc->Measure);
	MeasureCtrl.Update();
}

CNCMDoc* NStateMesurePane::GetDocument() 
{
	CFrameWnd *pMainFrame = (CFrameWnd *)AfxGetMainWnd();
	if(!pMainFrame)
		return NULL;
	return (CNCMDoc *)pMainFrame->GetActiveDocument();
}

void NStateMesurePane::OnSetFocus(CWnd* pOldWnd) 
{
	CDockablePane::OnSetFocus(pOldWnd);

	MeasureCtrl.SetFocus();
}

