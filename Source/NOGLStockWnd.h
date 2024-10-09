#pragma once
#include "NSectionSet.h"
#include "NOpenGLWnd.h"

class NOGLStockWnd : public NOpenGLWnd
{
public:
	NOGLStockWnd(void);
	~NOGLStockWnd(void);
protected:
	DECLARE_DYNCREATE(NOGLStockWnd)
	void NUpdate(const CRect *ClipRect = NULL) override;
	void DrawScene(RenderMode GlobRMode) override;
	void DrawScene4Pick(int Type) override;
	void DoZoomTool() override;
	BBox GetSceneGabar(bool VisibleOnly) override;
public:
	//{{AFX_MSG(NOGLStockWnd)
	//}}AFX_MSG
 	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
protected:
	void OnUpdate(CWnd* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/) override;
public:
	RPickID GetPointOnModel(CPoint iCPoint, BPoint& oPoint, BPoint& oNormal) override;
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
