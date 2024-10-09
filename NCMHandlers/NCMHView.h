
// NCMView.h : interface of the CNCMView class
//

#pragma once
#include "NCMHWnd.h"
class CNCMHView : public CView
{
protected: // create from serialization only
	CNCMHView() noexcept;
	DECLARE_DYNCREATE(CNCMHView)

public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view

public:
	virtual ~CNCMHView();
public:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct); 
protected:
	NCMHWnd OpenGLWnd;
};
