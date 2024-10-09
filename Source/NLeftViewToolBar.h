#pragma once

// NLeftViewToolBar

class NLeftViewToolBarBase : public CToolBar
{
	DECLARE_DYNAMIC(NLeftViewToolBarBase)

public:
	NLeftViewToolBarBase() {};
	NLeftViewToolBarBase(const NLeftViewToolBarBase & IN) : CToolBar(IN) {  }
	afx_msg void OnTbnHotItemChange(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
};

class NLeftViewToolBar : public NLeftViewToolBarBase
{
	DECLARE_DYNAMIC(NLeftViewToolBar)

public:
	NLeftViewToolBar();
	NLeftViewToolBar(const NLeftViewToolBar & IN) : NLeftViewToolBarBase(IN) {  }
	virtual ~NLeftViewToolBar();

protected:
	// Generated message map functions
	//{{AFX_MSG(CDlgToolBar)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	COLORREF BkColor;
public:
	void SetBkColor(COLORREF Color = 0) { BkColor = Color; }
	bool SetBkColorWRedraw(COLORREF Color);
};


