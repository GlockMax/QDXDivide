// NTranspGeomDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NTranspGeomDlg.h"

// NTranspGeomDlg dialog

IMPLEMENT_DYNAMIC(NTranspGeomDlg, CDialog)
NTranspGeomDlg::NTranspGeomDlg(CWnd* pParent /*=NULL*/)
	: CDialog(NTranspGeomDlg::IDD, pParent)
	, BasePoint(0)
{
	Create(IDD,pParent);
}

NTranspGeomDlg::~NTranspGeomDlg()
{
	DestroyWindow();
}

void NTranspGeomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(NTranspGeomDlg, CDialog)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

BOOL NTranspGeomDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

// Set WS_EX_LAYERED on this window 
	SetWindowLong(m_hWnd, GWL_EXSTYLE,
       GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED );
// Set Back Color as a transparent color and 50% transparency
//For WIN98	remove next statement
	SetLayeredWindowAttributes( GetSysColor(COLOR_3DFACE), (255 * 50) / 100, LWA_COLORKEY | LWA_ALPHA );

	Ctrl.SubclassDlgItem(IDC_TREE_GEOM,this);
// Calculate window height required to fit all items
	int ih = 18 * Ctrl.GetItemHeight();
	CRect Rect;
	GetWindowRect(&Rect);

// Adjust dialog position to the m_pParentWnd window
	int left = Rect.left, top = Rect.top;
	CRect PrRect;
	m_pParentWnd->GetWindowRect(&PrRect);
	int dx = left + Rect.Width() - PrRect.left - PrRect.Width();
	if( dx > 0 )
		left -= dx;
	dx = PrRect.left - left;
	if( PrRect.left > left )
		left = PrRect.left;
	int dy = top + ih - PrRect.top - PrRect.Height();
	if( dy > 0 )
		top -= dy;
	dy = PrRect.top - top;
	if( PrRect.top > top )
		top = PrRect.top;
	MoveWindow(left,top,Rect.Width(),ih);
	Ctrl.MoveWindow(0,0,Rect.Width(),ih);
	Ctrl.Update(NCadrGeom(), NCadrGeom(), BPoint(0., 0., 1., 0.));


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void NTranspGeomDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
	ClientToScreen(&point);
	BasePoint = point;

	CDialog::OnLButtonDown(nFlags, point);
}

void NTranspGeomDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();

	CDialog::OnLButtonUp(nFlags, point);
}

void NTranspGeomDlg::OnMouseMove(UINT nFlags, CPoint point)
{
// Move window
	if(MK_LBUTTON & nFlags)
	{
		ClientToScreen(&point);
		CPoint pt = point - BasePoint;
		CRect Rect;
		GetWindowRect(&Rect);
		CRect MainRect;
		GetParent()->GetWindowRect(&MainRect);
		if(MainRect.PtInRect(Rect.CenterPoint() + pt)) 
			Rect += pt;
		MoveWindow(Rect,FALSE);
		BasePoint = point;
	}

	CDialog::OnMouseMove(nFlags, point);
}

void NTranspGeomDlg::Update(const NCadrGeom & Prev, const NCadrGeom & Next, const BPoint &ToolAxis)
{
	Ctrl.Update(Prev, Next, ToolAxis);
}
