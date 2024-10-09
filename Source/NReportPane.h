
#pragma once

#include "NReportCtrl.h"
// NReportPane.h : header file
//


class NReportPane : public CDockablePane
{
protected:
	DECLARE_DYNCREATE(NReportPane)
	CNCMDoc* GetDocument();

public:
	NReportPane();
	virtual ~NReportPane();
	void AddCollItem(const NCollLogElem* pCollLogElem);
	void AddErrorItem(int HistPos, const NProgError* pErrorLogElem);
	void AddCollLog(const NCollisionLog& Log, int StartInd);
	void Clear(bool Collisions, bool Errors);
	int GetItemCount(void) const;
	void RedrawCaption();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NReportPane)
	public:
	virtual void OnUpdate(CDockablePane* pSender, LPARAM lHint, CObject* pHint);
	protected:
	//}}AFX_VIRTUAL

// Implementation
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	NReportCtrl ReportCtrl;
	//{{AFX_MSG(NReportPane)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg LRESULT OnReportClear(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReportRunto(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnItemChanged(WPARAM wParam, LPARAM lParam);
	bool HasFocus(void);
	const auto& GetCtrl() const { return ReportCtrl; }
	auto& GetCtrlW() { return ReportCtrl; }
	virtual BOOL LoadState(LPCTSTR lpszProfileName = NULL, int nIndex = -1, UINT uiID = (UINT)-1);
	virtual BOOL SaveState(LPCTSTR lpszProfileName = NULL, int nIndex = -1, UINT uiID = (UINT)-1);
protected:
	void ReportRunto();
protected:
	bool Blocked;
	static constexpr int MAX_LINES = 150;
};

