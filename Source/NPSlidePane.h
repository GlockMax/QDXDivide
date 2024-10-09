#pragma once

// NPSlidePane
constexpr int SLIDER_HEIGHT = 24;
constexpr int TOP_GAP = 6;
constexpr int BOTTOM_GAP = 0;

class NPSlidePane : public CDockablePane
{
	DECLARE_DYNAMIC(NPSlidePane)

public:
	NPSlidePane();
	virtual ~NPSlidePane();
	void OnUpdate(CDockablePane* pSender, LPARAM lHint, CObject* pHint);
	void NAnimateUpdate(LPARAM lHint = N_CADRCHANGED);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void AdjustLayout();
	void RecalcLayout();
	void SetSlider(int NewVal);
protected:
	void Scroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
protected:
	CSliderCtrl SliderCtrl;
	class CNCMDoc *NPSlidePane::GetDocument() const; 
	CRect rectClient;
	int PSlideRange;
	int SliderPos;
	DWORD Orient;
	bool IsSliding;
	int BarsNum;
	int *BarsSizes;
	static const int MaxTools = 256;
	NColor Colors[MaxTools];
	int Positions[MaxTools];
	int Marks[MaxTools];

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};


