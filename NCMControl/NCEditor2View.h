
// NCEditor2View.h : interface of the CNCEditor2View class
//


#pragma once


class CNCEditor2View : public CEditView
{
protected: // create from serialization only
	CNCEditor2View();
	DECLARE_DYNCREATE(CNCEditor2View)

// Attributes
public:
	CNCEditor2Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CNCEditor2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in NCEditor2View.cpp
inline CNCEditor2Doc* CNCEditor2View::GetDocument() const
   { return reinterpret_cast<CNCEditor2Doc*>(m_pDocument); }
#endif

