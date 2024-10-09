#pragma once
#include "NViewDlg.h"
#include "BOpenGLWnd.h"

class BOpenGLWndVD : public BOpenGLWnd
{
protected:
	DECLARE_DYNCREATE(BOpenGLWndVD)
	BOpenGLWndVD();
	CToolBar m_wndToolBar;
    NViewDlg ViewDlg;

    void CloseToolBar() override;
    void ShowViewToolbar(CPoint point) override;
	virtual void LoadTBFromIcons(void) {};
	afx_msg void OnTbZoomY();
	afx_msg void OnTbSave();
	afx_msg void OnTbView1();
	afx_msg void OnTbView2();
	afx_msg void OnTbView3();
	afx_msg void OnTbView4();
	afx_msg void OnTbView5();
	afx_msg void OnTbZoomX();
	afx_msg void OnTbZoomW();
	afx_msg void OnSaveView1();
	afx_msg void OnSaveView2();
	afx_msg void OnSaveView3();
	afx_msg void OnSaveView4();
	afx_msg void OnSaveView5();
	afx_msg void OnTbZoomSc();
	afx_msg void OnTbZoomP();
	afx_msg void OnTbZoomR();
	afx_msg void OnTbZoomZ();
	afx_msg void OnTbZoomNear();
	afx_msg void OnTbZoomNorm();
	afx_msg void OnTbZoomTool();
	afx_msg void OnTbRedraw();
	afx_msg void OnTbViewRU();
	afx_msg void OnTbViewL();
	afx_msg void OnTbViewR();
	afx_msg void OnTbViewB();
	afx_msg void OnTbViewF();
	afx_msg void OnTbReset();
	afx_msg void OnTbZoomAll();
	afx_msg void OnTbViewPrev();
	afx_msg void OnTbViewNext();
	afx_msg void OnTbViewLU();
	afx_msg void OnTbViewRD();
	afx_msg void OnTbViewLD();
	afx_msg void OnTbViewT1();
	afx_msg void OnTbViewT2();
	afx_msg void OnTbViewT3();
	afx_msg void OnTbViewT4();
	afx_msg void OnTbViewT5();
	afx_msg void OnTbViewT6();
	afx_msg void OnTbViewT7();
	afx_msg void OnTbViewZx();
	afx_msg void OnTbVewXz();
	afx_msg void OnTbViewLa();
	afx_msg void OnTbVewZy();
	afx_msg void OnTbViewXyleft();
	afx_msg void OnTbViewXyright();
	afx_msg void OnUpdateTbViewPrev(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTbViewNext(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTbSave(CCmdUI* pCmdUI);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	bool IsZoomFuncActive() const { return ZoomRActive || ZoomSaveActive || ZoomWActive || ZoomNormActive; }
	bool IsViewInterfaceActive() const { return IsZoomFuncActive() || GetKeyState(VK_CONTROL) >> 1 || GetKeyState(VK_SHIFT) >> 1 || GetKeyState(VK_MENU) >> 1; } // >>1 is used to ignore the low-order bit 
	void SetSavedView(int i);
	bool SetCursorByCursorID();
	void PrepTracker(NRotTracker*& Tracker, char axes, WORD StopMessage = WM_LBUTTONUP) override;
	void PrepZoomNorm();
	void SetDocModified();

public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo) override;
	DECLARE_MESSAGE_MAP()
};

