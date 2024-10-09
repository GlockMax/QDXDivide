#pragma once


// NDummyView view

class NDummyView : public CView
{
protected: // create from serialization only
	NDummyView();
	DECLARE_DYNCREATE(NDummyView)

public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view

public:
	virtual ~NDummyView();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	DECLARE_MESSAGE_MAP()
public:
	void SetPane(class NOGLPane *pMainPane);
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	NOGLPane *pPane;
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};


