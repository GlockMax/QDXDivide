
// NCEditor2View.cpp : implementation of the CNCEditor2View class
//

#include "stdafx.h"
#include "NCMControl.h"

#include "NCEditor2Doc.h"
#include "NCEditor2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNCEditor2View

IMPLEMENT_DYNCREATE(CNCEditor2View, CEditView)

BEGIN_MESSAGE_MAP(CNCEditor2View, CEditView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CNCEditor2View::OnFilePrintPreview)
//	ON_WM_INITMENUPOPUP()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

// CNCEditor2View construction/destruction

CNCEditor2View::CNCEditor2View()
{
	// TODO: add construction code here

}

CNCEditor2View::~CNCEditor2View()
{
}

BOOL CNCEditor2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CEditView::PreCreateWindow(cs);
}

// CNCEditor2View drawing

void CNCEditor2View::OnDraw(CDC* pDC)
{
	CNCEditor2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CNCEditor2View printing


void CNCEditor2View::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CNCEditor2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CNCEditor2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CNCEditor2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CNCEditor2View::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CNCEditor2View::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CNCEditor2View diagnostics

#ifdef _DEBUG
void CNCEditor2View::AssertValid() const
{
	CEditView::AssertValid();
}

void CNCEditor2View::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CNCEditor2Doc* CNCEditor2View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNCEditor2Doc)));
	return (CNCEditor2Doc*)m_pDocument;
}
#endif //_DEBUG
