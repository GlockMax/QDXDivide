// NLeftViewToolBar.cpp : implementation file
//

#include "stdafx.h"
#include "ResLocal.h"
#include "LeftView.h"
#include "NLeftViewToolBar.h"

// NLeftViewToolBar

IMPLEMENT_DYNAMIC(NLeftViewToolBarBase, CToolBar)
BEGIN_MESSAGE_MAP(NLeftViewToolBarBase, CToolBar)
	//{{AFX_MSG_MAP(CDlgToolBar)
	ON_NOTIFY_REFLECT(TBN_HOTITEMCHANGE, &NLeftViewToolBarBase::OnTbnHotItemChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
IMPLEMENT_DYNAMIC(NLeftViewToolBar, NLeftViewToolBarBase)

BEGIN_MESSAGE_MAP(NLeftViewToolBar, NLeftViewToolBarBase)
	//{{AFX_MSG_MAP(CDlgToolBar)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void NLeftViewToolBarBase::OnTbnHotItemChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTBHOTITEM pNMTBHI = reinterpret_cast<LPNMTBHOTITEM>(pNMHDR);
	if (pNMTBHI->dwFlags & HICF_LEAVING)
		static_cast<CFrameWnd *>(AfxGetMainWnd())->SetMessageText(AFX_IDS_IDLEMESSAGE);
	else
	{
		TCHAR szFullText[256];
		CString cstStatusText;
		AfxLoadString((UINT)pNMTBHI->idNew, szFullText);
		AfxExtractSubString(cstStatusText, szFullText, 0, '\n');
		static_cast<CFrameWnd *>(AfxGetMainWnd())->SetMessageText(cstStatusText);
	}
	*pResult = 0;
}

NLeftViewToolBar::NLeftViewToolBar() : BkColor(0)
{

}

NLeftViewToolBar::~NLeftViewToolBar()
{
}

BOOL NLeftViewToolBar::OnEraseBkgnd(CDC* pDC)
{
	// Set brush to desired background color
	CBrush backBrush(BkColor);

	// Save old brush
	CBrush* pOldBrush = pDC->SelectObject(&backBrush);

	CRect rect;
	pDC->GetClipBox(&rect);     // Erase the area needed

	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(),
		PATCOPY);

	pDC->SelectObject(pOldBrush);

	return TRUE;
}

void NLeftViewToolBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	CLeftView *pLeftView = static_cast<CLeftView *>(GetParent()->GetParent());
	if (pLeftView)
	{
		CPoint b = point;
		ClientToScreen(&b);
		pLeftView->GetTreeCtrl().ScreenToClient(&b);
		pLeftView->GetTreeCtrl().SelectItemByPoint(b);
	}
	CToolBar::OnLButtonDown(nFlags, point);
}

bool NLeftViewToolBar::SetBkColorWRedraw(COLORREF Color)
{
	if (Color == BkColor)
		return false;
	BkColor = Color;
	RedrawWindow();
	return true;
}
