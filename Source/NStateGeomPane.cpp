// NStateGeomPane.cpp : implementation file
//

#include "stdafx.h"
#include "NCMProject.h"
#include "NFont.h"
#include "NCMDoc.h"
#include "NCUnit.h"
#include "NStateGeomPane.h"
#include "NColor.h"
#include "NControlPanel.h"
#include "NStateGeomCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NStateGeomPane

IMPLEMENT_DYNCREATE(NStateGeomPane, CDockablePane)

NStateGeomPane::NStateGeomPane()
{
}

NStateGeomPane::~NStateGeomPane()
{
}


BEGIN_MESSAGE_MAP(NStateGeomPane, CDockablePane)
	//{{AFX_MSG_MAP(NStateGeomPane)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
//	ON_WM_ACTIVATE()
END_MESSAGE_MAP()




/////////////////////////////////////////////////////////////////////////////
// NStateGeomPane diagnostics

#ifdef _DEBUG
void NStateGeomPane::AssertValid() const
{
	CDockablePane::AssertValid();
}

void NStateGeomPane::Dump(CDumpContext& dc) const
{
	CDockablePane::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// NStateGeomPane message handlers

void NStateGeomPane::OnUpdate(CDockablePane* pSender, LPARAM lHint, CObject* pHint) 
{
 // Update colors
	if(lHint & N_SETTINGS)
	{
		GeomCtrl.SetSettings();
		return;
	}
	if(lHint & N_STATE)
	{
		UpdateGeom();
		return;
	}
}

void NStateGeomPane::OnSize(UINT nType, int cx, int cy) 
{
	CDockablePane::OnSize(nType, cx, cy);
	
	CRect wrect;
	GetClientRect(&wrect);
	GeomCtrl.MoveWindow(wrect, TRUE);
}

int NStateGeomPane::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rect;
	GetClientRect(&rect);


	GeomCtrl.Create( WS_TABSTOP | WS_CHILD | WS_BORDER | WS_VISIBLE 
   | TVS_DISABLEDRAGDROP,   CRect(10, 10, 300, 100), this, ID_GEOMCTRL);
	
	return 0;
}

void NStateGeomPane::Update()
{
	OnUpdate(NULL,N_STATE,NULL);
	UpdateWindow();
}


void NStateGeomPane::UpdateGeom()
{
	CNCMDoc *pDoc = (CNCMDoc *)GetDocument();
	if(!pDoc)
		return;
	if (pDoc->Distance.IsActive())
		GeomCtrl.UpdateDist(pDoc->Distance.GetBase(), pDoc->Distance.GetSnap(), pDoc->CadrToolAxis);
	else
		GeomCtrl.Update(pDoc->SelectedPrev, pDoc->SelectedNext, pDoc->CadrToolAxis);

}
CNCMDoc* NStateGeomPane::GetDocument() 
{
	CFrameWnd *pMainFrame = (CFrameWnd *)AfxGetMainWnd();
	if(!pMainFrame)
		return NULL;
	return (CNCMDoc *)pMainFrame->GetActiveDocument();
}

void NStateGeomPane::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	GeomCtrl.SetFocus();
}

