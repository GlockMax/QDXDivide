#if !defined(AFX_NSTATEVIEW_H__FB643E21_42DE_11D3_B384_605D07C10000__INCLUDED_)
#define AFX_NSTATEVIEW_H__FB643E21_42DE_11D3_B384_605D07C10000__INCLUDED_

#include "NCUnitState.h"	// Added by ClassView
#include "NStateGeomCtrl.h"	// Added by ClassView
#include "NStateUnitCtrl.h"
#include "NStateMeasureCtrl.h"
#include "NStateRegsCtrl.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NStateView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// NStateView view
class NCUnitState;

class NStateView : public CDockablePane
{
protected:
	DECLARE_DYNCREATE(NStateView)
	CNCMDoc* GetDocument();

// Attributes
public:
	NMapStringToOb * GetAllElemList();

// Operations
public:
	NStateView();
	virtual ~NStateView();
	void GetUnvisElemList(CStringArray & UnvisElemList);
	CStringArray & GetVisElemList();
	void Update();
	NStateUnitCtrl lc;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NStateView)
	public:
	virtual void OnInitialUpdate();
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	virtual void OnUpdate(CDockablePane* pSender, LPARAM lHint, CObject* pHint);
	protected:
	//}}AFX_VIRTUAL

// Implementation
protected:
	HMODULE hins;
	CFont m_Font;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
//	CToolTipCtrl ToolTips;
	NStateGeomCtrl GeomCtrl;
	NStateMeasureCtrl MeasureCtrl;
	NStateRegsCtrl RegsCtrl;
	int ActiveControlNum; // 0-GeomCtrl; 1-lc; 2-measure
	void UpdateGeom();
	void UpdateUnit();
	void UpdateMeasure();
	void UpdateRegs();
	CTabCtrl * m_pTabCtrl;
	void CreateStateView();
	void DeleteElements();
	CStringArray VisElemList;
	NMapStringToOb AllElemList;
	void ResetElements();
	NCUnitState * pUnitState;
//	CImageList ImageList;
	COleDropTarget DropTarget;
	//{{AFX_MSG(NStateView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnHideItem();
	afx_msg void OnStateViewSettings();
	//}}AFX_MSG
	afx_msg void OnTabSelChanging(NMHDR* pnmhdr, LRESULT* pResult);
	afx_msg void OnTabSelChange(NMHDR* pnmhdr, LRESULT* pResult);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
//	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NSTATEVIEW_H__FB643E21_42DE_11D3_B384_605D07C10000__INCLUDED_)
