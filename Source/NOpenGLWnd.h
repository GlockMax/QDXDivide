// NOpenGLWnd.h : interface of the NOpenGLWnd class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "NViewState.h"
#include "RPickID.h"


#include "BBox.h"
#include "cadrID.h"
#include "NColor.h"
#include "BOpenGLWndVD.h"
#include "NCadrGeom.h"
#include "ntoolcombined.h"
#include "OGLCursor.h"

const int HOVER_TIME = 400;
const int CursorSize = 32;

class CNCMDoc;
class NCadrGeom;
class NToolCombined;
class BMatr;
class NTranspGeomDlg;
class T3DData;
class BMatrPair;

const double ActiveBrightness = 1.;
const double InactiveBrightness = 0.6;

class NOpenGLWnd : public BOpenGLWndVD
{
	friend class NOGLPane;
	class NInfoTip : public CMFCToolTipCtrl
	{
	public:
		NInfoTip() { Created = false; }
		void EnsureCreated(CWnd* pWnd);
	protected:
		bool Created;
	};
protected:
	enum PickType
	{
		PT_TRAJECTORY = 1,
		PT_DIMENSIONS = 2,
		PT_MACHINE = 4,
		PT_STFIPA = 8,
		PT_UCS = 16,
		PT_ALL = 31
	};
public:
	static CString ViewName;
protected: 
protected: // create from serialization only
	DECLARE_DYNCREATE(NOpenGLWnd)

// Attributes
public:
	NOpenGLWnd();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NOpenGLWnd)
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	BOOL PreCreateWindow(CREATESTRUCT& cs) override;
	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
protected:
	virtual void InitialUpdate() override; // called first time after construct
	//}}AFX_VIRTUAL
// Data members
protected:
	TRACKMOUSEEVENT tme;
	HCURSOR Cursors[64];
	HCURSOR CurrentCursor;
	double MagnFactor;
	GLfloat SelectionRegionSize;
	NColor SnapCol;
	int StepsCount;
	static NViewState NState;
	NTranspGeomDlg *pTrGeomDlg;// Transparent dialog for the geom properties
	bool IsLastRendTranslucent;
// Implementation
public:
	static bool DrawCadrGeom(const NCadrGeom &Geom, double Eps, double t = -1.);
	virtual ~NOpenGLWnd();
	virtual void DrawScene(RenderMode GlobRMode);
	virtual void NUpdate(const CRect *ClipRect = NULL);

	void DrawWithTranslucent();
	void DrawDefault();

protected:
	void SetZRange();
	int HighlightedCadrStPos;// real stock position is abs(HighlightedCadrStPos) - 1. if HighlightedCadrStPos < 0 - skeleton
	cadrID HighlightedCadrID;
	NCadrGeom HighlightedCadr;
	bool HighlightedCadrFrStock;
	double HighlightedThick;
	OGLCursor HighlightedPoint;
	void SetWinRasterPos0();
	void SetCursor(int n);
	RPickID Pick(const CPoint& point, int Type, GLdouble Size);
	RPickID PickOct(const CPoint& point, bool Syncr);
	void SnapCadrCursor(const CPoint& point);
    const BMatr& GetStockMatr() const { return StockMatr; }
	void InitCursors();
	//{{AFX_MSG(NOpenGLWnd)
	afx_msg void OnStatusCoordRel();
	afx_msg void OnUpdateCoordRel(CCmdUI *pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnFileExport();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnViewOutline();
	afx_msg void OnUpdateViewOutline(CCmdUI* pCmdUI);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	//}}AFX_MSG
    afx_msg LPARAM  OnMouseHover(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnViewTrGeom();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnPaint();
	afx_msg void OnMouseLeave();
protected:
	BMatr StockMatr;// Stock moved matrix 0. Used for stock and others
	BMatr StockAxisMatr;// Stock matrix 0. Used for programs
	NInfoTip InfoTip;
public:
	void LoadTBFromIcons(void) override;
	void GetMouseCoord(const CPoint &point, double *wz, double * x, double * y, double * z) override;
	void GetMouseCoordGL(const CPoint &point, double *wz, double * x, double * y, double * z) override;
	virtual void GetMouseStockCoord(const CPoint &point, double *wz, double * x, double * y, double * z);
	void RecalcStockPoint(BPoint& P) const override;
	void RecalcStockPointBack(BPoint& P) const override;
	void NAnimateUpdate(void);
	void SetViewPoint(const CString & ViewName);
	void Proc3DxInput(T3DData * pData);
	virtual void DrawPrograms(bool Draw4Pick);
	virtual void DrawScene4Pick(int Type);
	void DrawProgramsCom(bool Draw4Pick = false);
	void DrawHighlightedCadr(bool ForStock);
	bool CanRedraw(void) const;
    bool IsVisiblePoint(const BPoint& iPoint, double iKoef);
	bool Divide1Stock(const CPoint& point, const BMatrPair &StockMatrP);
	RPickID DoSnap(const CPoint& point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void GetHighPoint(class BPoint& P) const override;

};



