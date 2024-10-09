// NPSlidePane.cpp : implementation file
//

#include "stdafx.h"
#include "NCMDoc.h"
#include "WM_APP_Messages.h"
#include "SliderButton.h"
#include "NPSlidePane.h"

// NPSlidePane

IMPLEMENT_DYNAMIC(NPSlidePane, CDockablePane)

NPSlidePane::NPSlidePane()
{
	SliderPos = 0;
	PSlideRange = 0;
	Orient = TBS_HORZ;
	IsSliding = false;
	BarsNum = 0;
	BarsSizes = NULL;
}

NPSlidePane::~NPSlidePane()
{
}

void NPSlidePane::OnUpdate(CDockablePane* pSender, LPARAM lHint, CObject* pHint) 
{
	if((lHint & N_SLIDERCHANGED) && !IsSliding)
	{
		SetSlider(GetDocument()->GetGlobPos());
	}
	if((lHint & N_CADRCHANGED) && !IsSliding)
	{
		SetSlider(GetDocument()->GetGlobPos());
	}
	if(lHint & (N_TOOLCHANGED | N_PROGRAMCHANGED))
	{
		CNCMDoc *pDoc = GetDocument();
		BarsNum = pDoc->GetToolsPosCol(MaxTools, Colors, Positions) - 1;
		PSlideRange = Positions[BarsNum];
		SliderCtrl.SetRange(0, PSlideRange);
		AdjustLayout();
		Invalidate();
	}
}

CNCMDoc *NPSlidePane::GetDocument() const
{
	CFrameWnd *pMainFrame = (CFrameWnd *)AfxGetMainWnd();
	if(!pMainFrame)
		return NULL;
	return (CNCMDoc *)pMainFrame->GetActiveDocument();
}

BEGIN_MESSAGE_MAP(NPSlidePane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_SIZING()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// NPSlidePane message handlers



int NPSlidePane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	if(!SliderCtrl.Create(WS_CHILD | WS_VISIBLE | TBS_HORZ | TBS_TOP | TBS_NOTICKS | TBS_NOTIFYBEFOREMOVE ,
		CRect(0, 0, 200, 200), this, IDR_PSLIDER))
		return -1;

	Orient = GetCurrentAlignment();

	AdjustLayout();
	return 0;
}

void NPSlidePane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	AdjustLayout();
	Invalidate();
}
void NPSlidePane::RecalcLayout()
{
	CDockablePane::RecalcLayout();
}

void NPSlidePane::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	GetClientRect(rectClient);
	CRect Rect;
	GetWindowRect(Rect);
	DWORD NewOrient = (rectClient.Width() > rectClient.Height()) ? TBS_HORZ : TBS_VERT;

	if(Orient != NewOrient)
	{
		SliderCtrl.ModifyStyle(TBS_VERT, NewOrient);
		Orient = NewOrient;
	}
	switch(Orient)
	{
		case TBS_HORZ:
			SliderCtrl.SetWindowPos(NULL, 0, TOP_GAP, rectClient.Width(), SLIDER_HEIGHT, SWP_NOACTIVATE | SWP_NOZORDER);
			SetMinSize(CSize(20, TOP_GAP + BOTTOM_GAP + SLIDER_HEIGHT - rectClient.Height() + Rect.Height()));
			break;
		case TBS_VERT:
			SliderCtrl.SetWindowPos(NULL, TOP_GAP, 0, SLIDER_HEIGHT, rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
			SetMinSize(CSize(TOP_GAP + BOTTOM_GAP + SLIDER_HEIGHT - rectClient.Width() + Rect.Width(), 20));
			break;
	}
	// Prepare colored bars
	CRect rectThumb;
	SliderCtrl.GetThumbRect(rectThumb);
	bool IsHor = (Orient == TBS_HORZ);
	int Thumb = (IsHor ? rectThumb.Width() : rectThumb.Height()) / 2;
	if(BarsNum > 0)
	{
		for(int i = 0; i <= BarsNum; ++i)
		{
			SliderCtrl.	SetPos(Positions[i]);
			SliderCtrl.GetThumbRect(rectThumb);
			Marks[i] = (IsHor ? rectThumb.left : rectThumb.top) + Thumb;
		}
		--Marks[BarsNum];
	}
	SliderCtrl.SetPos(SliderPos);
}

void NPSlidePane::SetSlider(int NewVal)
{
	SliderPos = NewVal;
	SliderCtrl.SetPos(NewVal);
}

void NPSlidePane::NAnimateUpdate(LPARAM lHint)
{
	OnUpdate(NULL, lHint, NULL);
}

void NPSlidePane::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// Clear window
	GetClientRect(rectClient);
	dc.FillSolidRect(rectClient, GetSysColor(COLOR_WINDOW));
	// Draw colored bars
	bool IsHor = (Orient == TBS_HORZ);
	for(int i = 0; i < BarsNum; ++i)
	{
		int x, y, cx, cy;
		x = IsHor ? Marks[i] : 1;
		y = IsHor ? 1 : Marks[i];
		cx = IsHor ? Marks[i + 1] - Marks[i] : TOP_GAP - 2;
		cy = IsHor ? TOP_GAP - 2 : Marks[i + 1] - Marks[i];
		dc.FillSolidRect(x, y, cx, cy, Colors[i + 1].GetRGB());
	}
	for(int i = 0; i <= BarsNum; ++i)
	{
		int x, y, cx, cy;
		x = IsHor ? Marks[i] : 0;
		y = IsHor ? 0 : Marks[i];
		cx = IsHor ? 1 : TOP_GAP;
		cy = IsHor ? TOP_GAP : 1;
		dc.FillSolidRect(x, y, cx, cy, RGB(0, 0, 0));
	}
}

void NPSlidePane::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	Scroll(nSBCode, nPos, pScrollBar);
	CDockablePane::OnHScroll(nSBCode, nPos, pScrollBar);
}
void NPSlidePane::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	Scroll(nSBCode, nPos, pScrollBar);
	CDockablePane::OnVScroll(nSBCode, nPos, pScrollBar);
}
void NPSlidePane::Scroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if(nSBCode == TB_PAGEDOWN || nSBCode == TB_PAGEUP)
	{
		SetSlider(SliderPos);
		return;
	}
	int NewPos = SliderCtrl.GetPos();
	if(AfxGetMainWnd()->SendMessage(WM_APP_PSLIDE, nSBCode, (LPARAM)NewPos) != 0)
	{
		SetSlider(SliderPos);
		return;
	}
	IsSliding = (nSBCode == TB_ENDTRACK) ? false : true;

	SliderPos = NewPos;
}

