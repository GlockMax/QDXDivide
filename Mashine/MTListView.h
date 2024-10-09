#if !defined(AFX_MTLISTVIEW_H__5B3CE4F5_E0F7_4BE4_B172_66CBADB2DC28__INCLUDED_)
#define AFX_MTLISTVIEW_H__5B3CE4F5_E0F7_4BE4_B172_66CBADB2DC28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MTListView.h : header file
//
#include <afxcview.h>
#include "ListEditBox.h"	// Added by ClassView


/////////////////////////////////////////////////////////////////////////////
// MTListView view

class MTListView : public CListView
{
protected:
	MTListView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(MTListView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MTListView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
//	afx_msg void OnItemChenged();
//	afx_msg void OnItemSelectedInView();

// Implementation
public:
	int SelectedItem;
	HWND m_OGLViewWnd;
	HWND m_MTreeViewWnd;
protected:
	void OnItemSelectedInView();
	void OnItemChenged();
	virtual ~MTListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	ListEditBox *Edit;
	//{{AFX_MSG(MTListView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MTLISTVIEW_H__5B3CE4F5_E0F7_4BE4_B172_66CBADB2DC28__INCLUDED_)
