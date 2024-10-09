#if !defined(AFX_MTTreeView_H__F8F4E750_3915_4D1C_9293_4F0399925819__INCLUDED_)
#define AFX_MTTreeView_H__F8F4E750_3915_4D1C_9293_4F0399925819__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MTTreeView.h : header file
//
#include "AfxCView.h"
#include "afxtempl.h"
#include "mtcoordinate.h"

/////////////////////////////////////////////////////////////////////////////
// MTTreeView view




class MTTreeView : public CTreeView
{
protected:
	typedef struct Node{CString Name;HTREEITEM hItem;} Node;
	typedef CArray <MTCoordinate,MTCoordinate> CooArray;
	typedef CArray<Node,Node> HIArray;
	HIArray HTree;
	MTTreeView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(MTTreeView)

// Attributes
public:

// Operations
public:
	HWND m_OGLViewWnd;
	HWND m_MTListViewWnd;
	int GetHindex(CString n);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MTTreeView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~MTTreeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	LRESULT OnSelectItemInView(WPARAM w, LPARAM l);
	CPoint TempPoint;
	CImageList TreeImageList;
	CTreeCtrl *Tree;
	//{{AFX_MSG(MTTreeView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnModelAddaxis();
	afx_msg void OnModelRemoveobject();
	afx_msg void OnModelAddblock();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnModelEditaxis();
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult);
//	afx_msg void OnSelectItemInView();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MTTreeView_H__F8F4E750_3915_4D1C_9293_4F0399925819__INCLUDED_)
