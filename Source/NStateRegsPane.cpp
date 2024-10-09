// NStateRegsPane.cpp : implementation file
//

#include "stdafx.h"
#include "NCMDoc.h"
#include "NCUnit.h"
#include "NStateRegsPane.h"
#include "NColor.h"
#include "NControlPanel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NStateRegsPane

IMPLEMENT_DYNCREATE(NStateRegsPane, CDockablePane)

NStateRegsPane::NStateRegsPane()
{
}

NStateRegsPane::~NStateRegsPane()
{
}


BEGIN_MESSAGE_MAP(NStateRegsPane, CDockablePane)
	//{{AFX_MSG_MAP(NStateRegsPane)
	ON_WM_SIZE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()




/////////////////////////////////////////////////////////////////////////////
// NStateRegsPane diagnostics

#ifdef _DEBUG
void NStateRegsPane::AssertValid() const
{
	CDockablePane::AssertValid();
}

void NStateRegsPane::Dump(CDumpContext& dc) const
{
	CDockablePane::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// NStateRegsPane message handlers

void NStateRegsPane::OnUpdate(CDockablePane* pSender, LPARAM lHint, CObject* pHint) 
{
 // Update colors
	if(lHint & N_SETTINGS)
		RegsCtrl.SetSettings();
	if(lHint & N_STATE)
		UpdateRegs();
}

void NStateRegsPane::OnSize(UINT nType, int cx, int cy) 
{
	CDockablePane::OnSize(nType, cx, cy);
	
	CRect wrect;
	GetClientRect(&wrect);
	RegsCtrl.MoveWindow(wrect, TRUE);
}

int NStateRegsPane::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	RegsCtrl.Create(CRect(10, 10, 300, 100), this, ID_REGSCTRL); 
	RegsCtrl.ShowWindow(SW_SHOW);
	return 0;
}

//void NStateRegsPane::Update()
//{
//	UpdateRegs();
//	UpdateWindow();
//}

void NStateRegsPane::UpdateRegs()
{
	CNCMDoc *pDoc = (CNCMDoc *)GetDocument();
	if(!pDoc)
		return;
	RegsCtrl.Update(pDoc->GetUnit()->GetState()->GetRegisters(), pDoc->GetUnit()->GetState()->GetVarSet(), NCUnitState::VisibleRegisters);
}

CNCMDoc* NStateRegsPane::GetDocument() 
{
	CFrameWnd *pMainFrame = (CFrameWnd *)AfxGetMainWnd();
	if(!pMainFrame)
		return NULL;
	return (CNCMDoc *)pMainFrame->GetActiveDocument();
}


void NStateRegsPane::OnSetFocus(CWnd* pOldWnd) 
{
	CDockablePane::OnSetFocus(pOldWnd);

	RegsCtrl.SetFocus();
}
