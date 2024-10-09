#pragma once
#include "NOpenglWnd.h"

class NOGLMashWnd : public NOpenGLWnd
{
public:
	NOGLMashWnd(void);
	~NOGLMashWnd(void);
	void GetMouseCoord(const CPoint &point, double *wz, double * x, double * y, double * z) override;
	RPickID GetPointOnModel(CPoint iCPoint, BPoint& oPoint, BPoint& oNormal) override;
	void DoZoomTool() override;
protected:
	BBox GetSceneGabar(bool VisibleOnly) override;
	DECLARE_DYNCREATE(NOGLMashWnd)
	void NUpdate(const CRect *ClipRect = NULL) override;
	void DrawScene(RenderMode GlobRMode) override;
public:
	//{{AFX_MSG(NOGLMashWnd)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
protected:
	BMatr ToolMatr;
public:
	void DrawScene4Pick(int Type) override;
	const BPoint GetRotBase(void) const override;
};
