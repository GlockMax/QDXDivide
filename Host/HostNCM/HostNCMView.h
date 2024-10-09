
// HostNCMView.h : interface of the CHostNCMView class
//

#pragma once


class CHostNCMView : public CView
{
protected: // create from serialization only
	CHostNCMView();
	DECLARE_DYNCREATE(CHostNCMView)

// Attributes
public:
	CHostNCMDoc* GetDocument() const;

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
	virtual ~CHostNCMView();
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
public:
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcmanagerSenddata();
	afx_msg void OnNcmanagerNcmsubscribetoprogramchanges();
	afx_msg void OnNcmanagerNcmunsubscribefromprogramchanges();
	afx_msg void OnNcmanagerNcmblankallprograms();
	afx_msg void OnNcmanagerNcmunblankallprograms();
	afx_msg void OnNcmanagerNcmshowprogramtext();
	afx_msg void OnNcmanagerNcmsaveprogram();
	afx_msg void OnNcmanagerNcmreset();
	afx_msg void OnNcmanagerNcmstartexecution();
	afx_msg void OnNcmanagerNcmstopexecution();
	afx_msg void OnNcmanagerNcmsetmodecut();
	afx_msg void OnNcmanagerNcmsetmodedontcut();
	afx_msg void OnNcmanagerNcmsetmodefastcut();
	afx_msg void OnNcmanagerNcmsettmsmoothly();
	afx_msg void OnNcmanagerNcmsettmfeedrate();
	afx_msg void OnNcmanagerNcmsettmresult();
	afx_msg void OnNcmanagerNcmviewzoomall();
	afx_msg void OnNcmanagerNcmviewrotatex();
	afx_msg void OnNcmanagerNcmviewrotatey();
	afx_msg void OnNcmanagerNcmviewrotatez();
	afx_msg void OnNcmanagerNcmviewpan();
	afx_msg void OnNcmanagerNcmviewscale();
	afx_msg void OnNcmanagerNcmviewzoomw();
	afx_msg void OnNcmanagerNcmviewsetviewtop();
	afx_msg void OnNcmanagerNcmsetstockcolour();
	afx_msg void OnNcmanagerNcmsetstockvisibility();
	afx_msg void OnNcmanagerNcmsetstockedges();
	afx_msg void OnNcmanagerNcmsetstockedgesthickness();
	afx_msg void OnNcmanagerNcmsetprogramsshow();
	afx_msg void OnNcmanagerNcmsetprogramthickness();
	afx_msg void OnNcmanagerNcmsettoolsvisibility();
	afx_msg void OnNcmanagerNcmsetstockquality();
	afx_msg void OnNcmanagerNcmsettooltoolpath();
	afx_msg void OnNcmanagerNcmremoveallsubprograms();
	afx_msg void OnNcmanagerNcmremovesubprogram();
};

#ifndef _DEBUG  // debug version in HostNCMView.cpp
inline CHostNCMDoc* CHostNCMView::GetDocument() const
   { return reinterpret_cast<CHostNCMDoc*>(m_pDocument); }
#endif

