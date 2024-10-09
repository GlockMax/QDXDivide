#pragma once
#include "OPENGLRENDER_API.h"
#include <deque>
#include "NCMProject.h"
#include "BMatr.h"
#include "NColor.h"
#include "BUndoRedoBuf.h"
#include "BaseOGLWnd.h"
#include "RPickID.h"
#include "BImageRGB.h"

#define ViewMatrNum   6 
/////////////////////////////////////////////////////////////////////////////
// BOpenGLWnd view
class NRotTracker;
class BPoint;
class BMatr;
class BBox;

class OPENGLRENDER_API BOpenGLWnd : public BaseOGLWnd
{
protected:
	class DequeLimited : public std::deque<BMatr>
	{
	public:
		void Push(const BMatr& _Val){ if (size() >= LimSize) pop_front(); push_back(_Val); }
		const BMatr Pop(){ if (size() == 0) return BMatr(); BMatr M = back(); pop_back(); return M; }
	protected:
		static const int LimSize = 100;
	};
public:
	class OPENGLRENDER_API NRotCenter
	{
	public:
		NRotCenter(void);
		~NRotCenter(void) {};
		BPoint RotBasePoint;
		NColor Color;
		double Rad;
		bool Visible;
	};
protected:
	friend class NRotTracker;
public:
	BOpenGLWnd();           // protected constructor used by dynamic creation
	virtual ~BOpenGLWnd();
protected:
	DECLARE_DYNCREATE(BOpenGLWnd)

// Data members
protected:
	NRotCenter RotCenter;
	int m_ChangeView;
	bool DynViewChange;
	BOOL ZoomSaveActive;
	BOOL ZoomNormActive;
	NRotTracker *ZoomRActive;
	NRotTracker *ZoomWActive;

	HDC hDCb;
	HGLRC hRCb;


	GLdouble ViewMatrix[ViewMatrNum][4][4];
	BMatr BufMatr;

	GLfloat BackCol[3];
	bool GradientEnable;
	GLfloat BackColLT[3];
	GLfloat BackColLB[3];
	GLfloat BackColRT[3];
	GLfloat BackColRB[3];
	bool SmallSize;
	class BOpenGLCtrl *pOpenGLCtrl;
	BUndoRedoBuf<BMatr, 100U> ViewHistory;
	int CursorID;

	bool(BOpenGLWnd:: *ViewActions[NGlobalState::VA_SIZE])(CPoint point, WORD StopMessage);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(BOpenGLWnd)
	//}}AFX_VIRTUAL

// Implementation
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	// Generated message map functions
	//{{AFX_MSG(BOpenGLWnd)
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
public:
	void DoTbViewRU();
	void DoTbViewL();
	void DoTbViewR();
	void DoTbViewB();
	void DoTbViewF();
	void DoTbViewPrev();
	void DoTbViewNext();
	void DoTbViewLU();
	void DoTbViewRD();
	void DoTbViewLD();
	void DoTbViewT1();
	void DoTbViewT2();
	void DoTbViewT3();
	void DoTbViewT4();
	void DoTbViewT5();
	void DoTbViewT6();
	void DoTbViewT7();
	void DoTbViewZx();
	void DoTbVewXz();
	void DoTbViewLa();
	void DoTbVewZy();
	void DoTbViewXyleft();
	void DoTbViewXyright();
	void DoTbReset();
	void DoTbViewD();
	void DoZoomNear();
	void CongVect(const BPoint& P0, const BPoint& Src, const BPoint& Dest);
	virtual void DoZoomTool();
	void DoZoomNorm(const CPoint& point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRotAngle();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual CDocument* GetDocument();
	void SetDefaultOGL(void);
	virtual void ClearBackground(void);
	void DoTbZoomAll();
	void SerializeMatrices(CArchive & ar);
	void ClearZoomActive(void);
	bool IsActive(void);
	void RestoreContext(void);
	static void GetCPointCoord(const CPoint & point, double *wz, double * x, double * y, double * z);
	virtual void GetMouseCoord(const CPoint & point, double *wz, double * x, double * y, double * z);
	virtual void GetMouseCoordGL(const CPoint & point, double *wz, double * x, double * y, double * z);
	void GetMouseRay(const CPoint & point, BPoint &P, BPoint &V);
	virtual void InitialUpdate();
	void SetChangeView(int Val){ m_ChangeView = Val;}
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void DoDefView(double eyex, double eyey, double eyez, double upx, double upy, double upz);
	void ApplyRotAngle(char Axis, double Angle);
protected:
	void GetArcPoint(double u,int nknots,float *ctlarray,float *knot,double &x,double &y,double &z);
	static BOOL IsOpenGLErr(const CStringA& str);

	void DoStdView(const CStringA& v);
	void DoZoomAll();
	virtual void CloseToolBar();
	void DoScale(double factor);
	void DoZoomR(CPoint point);
	void DoZoomW(CPoint point);

	virtual void RestoreView(int k);
	virtual void SaveView(int k);
	virtual BBox GetSceneGabar(bool VisibleOnly);

	virtual	void NUpdate(const CRect *ClipRect = NULL);
	void MakeCurrent();
public:
	void AdjustLayout();
	void DrawGLText(double x, double y, double z, char *text2draw);
	virtual const BPoint GetRotBase(void) const;
	void DrawRotCenter(void);
	void SetRotCenter(const BOpenGLWnd::NRotCenter &iRotCenter) {RotCenter = iRotCenter;}
	void GetRotCenter(BOpenGLWnd::NRotCenter &oRotCenter) const {oRotCenter = RotCenter;}
	void RotateAroundRotCenter(char Axis, double Angle);
	void MoveAlongAxis(char Axis, double Shift);
	virtual void DrawScene(enum RenderMode GlobRMode);
	void GetImage(BImageRGB& ImageRGB);
	void SetCursorID(char Axes);
	static const BPoint GetViewDir(void);

protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	BMatr GetModelMatr();
	void AppScale(short zDelta, CPoint pt);
	virtual void ShowViewToolbar(CPoint point);
	virtual void PrepTracker(NRotTracker *& Tracker, char axes, WORD StopMessage = WM_LBUTTONUP);
	bool ProcMouseMes(WORD Mes, UINT nFlags, CPoint point);
	void BSwapBuffers();
	bool PrepPan(CPoint point, WORD StopMessage);
	bool PrepScale(CPoint point, WORD StopMessage);
	bool PrepRot(CPoint point, WORD StopMessage);
	bool PrepRotX(CPoint point, WORD StopMessage);
	bool PrepRotY(CPoint point, WORD StopMessage);
	bool PrepRotZ(CPoint point, WORD StopMessage);
	bool PrepZoomW(CPoint point, WORD StopMessage);
	bool ZoomAll(CPoint point, WORD StopMessage) { DoZoomAll(); return true; }
	bool DoZoom(CPoint point, WORD StopMessage);
	bool DoEmpty(CPoint point, WORD StopMessage) { return true; }
	bool DoViewAction(WORD Message, UINT nFlags, CPoint point);
	virtual RPickID GetPointOnModel(CPoint iCPoint, BPoint& oPoint, BPoint& oNormal) { return RPickID(); };
	virtual void RecalcStockPoint(BPoint& P) const {};
	virtual void RecalcStockPointBack(BPoint& P) const {};
	virtual void GetHighPoint(class BPoint& P) const { P.Set(0., 0., 0., -1); }
};

/////////////////////////////////////////////////////////////////////////////

