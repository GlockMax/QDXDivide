#pragma once
#include "NOpenglWnd.h"
#include "NSectionSet.h"

class NOGLWSectWnd : public NOpenGLWnd
{
public:
	NOGLWSectWnd(void);
	~NOGLWSectWnd(void);
protected:
	virtual BBox GetSceneGabar(bool VisibleOnly);
	DECLARE_DYNCREATE(NOGLWSectWnd)
	virtual void NUpdate(const CRect *ClipRect = NULL);
	virtual void DrawScene(RenderMode GlobRMode);
	void DrawPenetration(bool Draw4Pick);
public:
	//{{AFX_MSG(NOGLWSectWnd)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


	const BMatr CalcViewMatrix(const BPoint &View);
public:
	virtual void DrawScene4Pick(int Type);
};
