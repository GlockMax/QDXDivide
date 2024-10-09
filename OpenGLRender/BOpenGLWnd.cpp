// BOpenGLWnd.cpp : implementation file
//
#include "stdafx.h"
#include "ConstDef.h"
#include "ResInv.h"
#include "ResLocal.h"
#include "WM_APP_messages.h"
#include "NCMProject.h"
#include "BPoint.h"
#include "BMatr.h"
#include "BBox.h"
#include "math.h"
#include "BaseRender.h"
#include "BOpenGLCtrl.h"
#include "NRotTracker.h"
#include "NRotAngles.h"
#include "RenderMode.h"
#include "BOpenGLWndVD.h"
#include "BOpenGLWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// BOpenGLWnd

IMPLEMENT_DYNCREATE(BOpenGLWnd, BaseOGLWnd)

BOpenGLWnd::BOpenGLWnd() : GradientEnable(false)
{
	ZoomSaveActive = FALSE;
	ZoomNormActive = FALSE;
	ZoomWActive = NULL;
	ZoomRActive = NULL;
	m_ChangeView = 0;
	DynViewChange = false;
	pOpenGLCtrl = nullptr;
	for(int k = 0; k < 6; ++k)
		for(int i = 0; i < 4; ++i)
		{
			for(int j = 0; j < 4; ++j)
				ViewMatrix[k][i][j] = 0.;
			ViewMatrix[k][i][i] = 1.;
		}
	ViewActions[NGlobalState::VA_ZOOM_ALL] = &BOpenGLWnd::ZoomAll;
	ViewActions[NGlobalState::VA_PREP_PAN] = &BOpenGLWnd::PrepPan;
	ViewActions[NGlobalState::VA_PREP_SCALE] = &BOpenGLWnd::PrepScale;
	ViewActions[NGlobalState::VA_PREP_ROT] = &BOpenGLWnd::PrepRot;
	ViewActions[NGlobalState::VA_PREP_ROTX] = &BOpenGLWnd::PrepRotX;
	ViewActions[NGlobalState::VA_PREP_ROTY] = &BOpenGLWnd::PrepRotY;
	ViewActions[NGlobalState::VA_PREP_ROTZ] = &BOpenGLWnd::PrepRotZ;
	ViewActions[NGlobalState::VA_PREP_ZOOMW] = &BOpenGLWnd::PrepZoomW;
	ViewActions[NGlobalState::VA_DO_ZOOM] = &BOpenGLWnd::DoZoom;
	ViewActions[NGlobalState::VA_DO_EMPTY] = &BOpenGLWnd::DoEmpty;
}

BOpenGLWnd::~BOpenGLWnd()
{
	delete pOpenGLCtrl;
}


BEGIN_MESSAGE_MAP(BOpenGLWnd, BaseOGLWnd)
	//{{AFX_MSG_MAP(BOpenGLWnd)
	ON_WM_DESTROY()
	ON_WM_CANCELMODE()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_WM_NCLBUTTONDOWN()
ON_WM_KILLFOCUS()
ON_WM_MOUSEWHEEL()
ON_WM_MBUTTONDOWN()
ON_WM_MBUTTONUP()
ON_WM_MOUSEMOVE()
ON_WM_PAINT()
ON_WM_SETFOCUS()
ON_WM_LBUTTONUP()
ON_WM_RBUTTONUP()
ON_WM_MBUTTONUP()
ON_COMMAND(ID_ROTANGLES, &OnRotAngle)
END_MESSAGE_MAP()

//void (__stdcall *glAddSwapHintR)(GLint, GLint, GLint, GLint); 
//static void _stdcall DummyAddSwapHintRect(GLint, GLint, GLint, GLint)
//{
// 
//}

/////////////////////////////////////////////////////////////////////////////
// BOpenGLWnd diagnostics

#ifdef _DEBUG
void BOpenGLWnd::AssertValid() const
{
	CWnd::AssertValid();
}

void BOpenGLWnd::Dump(CDumpContext& dc) const
{
	CWnd::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// BOpenGLWnd message handlers


//


BOOL BOpenGLWnd::IsOpenGLErr(const CStringA& str)
{
	GLenum errCode;
	if((errCode = glGetError())!=GL_NO_ERROR)
	{	
		AfxMessageBox(str+_T(":OpenGL error ")+CString(gluErrorString(errCode)));
		return TRUE;
	}
//	glFinish();
	return FALSE;

}
void BOpenGLWnd::DoDefView(double eyex, double eyey, double eyez, double upx, double upy, double upz)
{
	MakeCurrent();
	glLoadIdentity();
	gluLookAt(eyex, eyey, eyez, 0., 0., 0., upx, upy, upz);
	DoZoomAll();
	RestoreContext();
}

void BOpenGLWnd::DoStdView(const CStringA& v)
{
	GLdouble	eyex=0., eyey=0., eyez=0., 
				upx=0., upy=0., upz=1.; 
	if(v == "RU")
	{
		eyex =  1.;
		eyey = -1.5;
		eyez =  2.;
	}
	else if(v == "LU")
	{
		eyex = -1.;
		eyey = -1.5;
		eyez =  2.;
	}
	else if(v == "RD")
	{
		eyex =  1.;
		eyey =  1.5;
		eyez =  2.;
	}
	else if(v == "LD")
	{
		eyex = -1.;
		eyey =  1.5;
		eyez =  2.;
	}
	else if(v == "L")
	{
		eyex = -1.;
		eyey =  0.;
		eyez =  0.;
	}
	else if(v == "R")
	{
		eyex =	1.;
		eyey =  0.;
		eyez =  0.;
	}
	else if(v == "LA")
	{
		eyex =	0.;
		eyey =  0.;
		eyez =  1.;
		upx = -1.;
		upy = 0.;
		upz = 0.; 
	}
	else if(v == "B")
	{
		eyex =  0.;
		eyey =  1.;
		eyez =  0.;
	}
	else if(v == "F")
	{
		eyex =	0.;
		eyey = -1.;
		eyez =  0.;
	}
	else if(v == "ZX")
	{
		eyex =	0.;
		eyey =  1.;
		eyez =  0.;
		upx = 1.;
		upy = 0.;
		upz = 0.; 
	}
	else if(v == "XZL")
	{
		eyex =	0.;
		eyey =  -1.;
		eyez =  0.;
		upx = 1.;
		upy = 0.;
		upz = 0.; 
	}
	else if(v == "XZ")
	{
		eyex =	0.;
		eyey = -1.;
		eyez =  0.;
		upx = -1.;
		upy = 0.;
		upz = 0.; 
	}
	else if(v == "ZY")
	{
		eyex =	0.;
		eyey =	-1.;
		eyez =  0.;
		upx = 0.;
		upy = 0.;
		upz = 1.; 
	}
	else if(v == "XL")
	{
		eyex =	0.;
		eyey =	0.;
		eyez =  -1.;
		upx =	1.;
		upy = 0.;
		upz = 0.; 
	}
	else if(v == "XR")
	{
		eyex =	0.;
		eyey =	0.;
		eyez =	1.;
		upx =	1.;
		upy = 0.;
		upz = 0.; 
	}
	else if(v == "T1")
	{
		eyex = 1.;
		eyey = -0.5;
		eyez = 0.5;
		upx = 0.;
		upy = -1.;
		upz = 0.; 
	}
	else if (v == "T2")
	{
		eyex = 1.;
		eyey = -0.5;
		eyez = -0.5;
		upx = 0.;
		upy = -1.;
		upz = 0.;
	}
	else if (v == "T5")
	{
		eyex = -1.;
		eyey = -0.5;
		eyez = -0.5;
		upx = 0.;
		upy = -1.;
		upz = 0.;
	}
	else if (v == "T3")
	{
		eyex =	-1.;
		eyey =	-1.5;
		eyez =  0.5;
		upx = 0.;
		upy = 0.;
		upz = 1.; 
	}
	else if(v == "T4")
	{
		eyex =	1.;
		eyey = -1.5;
		eyez =  0.5;
		upx = 0.;
		upy = 0.;
		upz = 1.; 
	}
	else if (v == "T6")
	{
		eyex = 0.5;
		eyey = 1.;
		eyez = 0.5;
		upx = 1.;
		upy = 0.;
		upz = 0.;
	}
	else if (v == "T7")
	{
		eyex = 0.5;
		eyey = -1.;
		eyez = -0.5;
		upx = 1.;
		upy = 0.;
		upz = 0.;
	}

	else if (v == "XY")
	{
		eyex = 0.;
		eyey = 0.;
		eyez = 1.;
		upx = 0.;
		upy = 1.;
		upz = 0.;
	}
	else if (v == "D")
	{
		eyex = 0.;
		eyey = 0.;
		eyez = -1.;
		upx = 0.;
		upy = -1.;
		upz = 0.;
	}
	DoDefView(eyex, eyey, eyez, upx, upy, upz);
}

BMatr BOpenGLWnd::GetModelMatr()
{
	GLdouble modelMatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	return BMatr(modelMatrix);
}

void BOpenGLWnd::SaveView(int k)
{
	GLdouble modelMatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX ,modelMatrix);
	for(int i=0;i<16;i++)
		*(ViewMatrix[k][0]+i)=modelMatrix[i];
}

void BOpenGLWnd::RestoreView(int k)
{
	GLdouble modelMatrix[16];
	for(int i = 0; i < 16; ++i)
		modelMatrix[i] = *(ViewMatrix[k][0] + i);
	MakeCurrent();
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(modelMatrix);
	RestoreContext();
}

BBox BOpenGLWnd::GetSceneGabar(bool VisibleOnly)
{
	return BBox();
}

void BOpenGLWnd::DoTbZoomAll() 
{
	ClearZoomActive();	
	DoZoomAll();
}

BOOL BOpenGLWnd::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
//	return CWnd::OnEraseBkgnd(pDC);
}

void BOpenGLWnd::DoTbReset()
{
	ClearZoomActive();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	DoZoomAll();
}

void BOpenGLWnd::DoTbViewRU()
{
 	ClearZoomActive();	
	DoStdView("RU");
	m_ChangeView = 2;
	Invalidate();
}

void BOpenGLWnd::DoTbViewLU() 
{
	ClearZoomActive();	
 	DoStdView("LU");
	m_ChangeView = 2;
	Invalidate();
}

void BOpenGLWnd::DoTbViewRD() 
{
 	ClearZoomActive();	
	DoStdView("RD");
	m_ChangeView = 2;
	Invalidate();
}

void BOpenGLWnd::DoTbViewLD() 
{
 	ClearZoomActive();	
	DoStdView("LD");
	m_ChangeView = 2;
	Invalidate();
}

void BOpenGLWnd::DoTbViewL()
{
	ClearZoomActive();
	DoStdView("L");
	m_ChangeView = 2;
	Invalidate();
}

void BOpenGLWnd::DoTbViewD()
{
	ClearZoomActive();
	DoStdView("D");
	m_ChangeView = 2;
	Invalidate();
}

void BOpenGLWnd::DoZoomNear()
{
	ClearZoomActive();
	CRect rect;
	GetWindowRect(rect);
	CPoint point = rect.CenterPoint();
	double wz, x, y, z;
	GetMouseCoordGL(point, &wz, &x, &y, &z);
	BPoint P0(x, y, z, 1.);
	if (wz == 0.)
		P0.Set(0., 0., 0., 1.);
	// find nearest standard direction
	BPoint V = GetViewDir();
	BPoint Targ(0., 0., 0., -1.);
	BPoint Dirs[6] = { BPoint(0., 0., 1., 0), BPoint(0., 0., -1., 0), BPoint(0., 1., 0., 0)
		, BPoint(0., -1., 0., 0), BPoint(1., 0., 0., 0), BPoint(-1., 0., 0., 0) };
	constexpr double DAng = 30.;
	for (int i = 0; i < 6; ++i)
	{
		if (Dirs[i].Angle0_180(V) < DAng)
		{
			Targ = Dirs[i];
			break;
		}
	}
	if (Targ.IsVector())
	{
		if (ViewHistory.Add(GetModelMatr()))
			GetDocument()->SetModifiedFlag();
		CongVect(P0, V, Targ);
	}
}

void BOpenGLWnd::CongVect(const BPoint& P0, const BPoint& Src, const BPoint& Dest)
{
	MakeCurrent();
	BMatr M0, M1;
	M0.Trans(P0, BPoint(0., 0., 0., 1.));
	M1.Trans(BPoint(0., 0., 0., 1.), P0);
	BMatr M;
	M.e0cong(Dest, Src);
	glMultMatrixd((M0 * M * M1).GetArray());
	RestoreContext();
	m_ChangeView = 2;
	Invalidate();
}

void BOpenGLWnd::DoZoomTool()
{// Real methods in derived classes
}

void BOpenGLWnd::DoZoomNorm(const CPoint& point)
{
	if (!ZoomNormActive)
		return;
	ClearZoomActive();
	BPoint P0, Normal;
	RPickID PickID = GetPointOnModel(point, P0, Normal);
	if (PickID.GetSize() == 0 || PickID.GetGroup() == OG_NO_GROUP)
		return;
	if (Normal.IsVector())
	{
		RecalcStockPointBack(P0);
		RecalcStockPointBack(Normal);
		CongVect(P0, GetViewDir(), Normal * (-1.));
	}
}

void BOpenGLWnd::DoTbViewR()
{
 	ClearZoomActive();	
	DoStdView("R");
	m_ChangeView = 2;
	Invalidate();
}

void BOpenGLWnd::DoTbViewB() 
{
 	ClearZoomActive();	
	DoStdView("B");
	m_ChangeView = 2;
	Invalidate();
}

void BOpenGLWnd::DoTbViewF() 
{
	ClearZoomActive();	
 	DoStdView("F");
	m_ChangeView = 2;
	Invalidate();
}

void BOpenGLWnd::DoTbViewPrev()
{
	if (!ViewHistory.CanStepBack())
		return;
	ClearZoomActive();

	ViewHistory.StepBack();

	const BMatr& M = ViewHistory.Get();
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			ViewMatrix[0][i][j] = M.GetArray()[j + 4 * i];

	BufMatr = GetModelMatr();
	m_ChangeView = 3;
	Invalidate();
}

void BOpenGLWnd::DoTbViewNext()
{
	if (!ViewHistory.CanStepFor())
		return;
	ClearZoomActive();

	ViewHistory.StepFor();

	const BMatr& M = ViewHistory.Get();
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			ViewMatrix[0][i][j] = M.GetArray()[j + 4 * i];

	BufMatr = GetModelMatr();
	m_ChangeView = 3;
	Invalidate();
}


void BOpenGLWnd::DoZoomW(CPoint point)
{
	if (ZoomWActive == nullptr)
		return;
	ZoomWActive->TrackRubberBand(this, point, TRUE);
	MakeCurrent();
	CRect rect = ZoomWActive->m_rect;
	delete ZoomWActive;
	ZoomWActive = NULL;
	if (abs(rect.Width()) < 2 || abs(rect.Height()) < 2)
		return;
	GLdouble scale;
	CPoint p;
	
	p.x=(rect.left + rect.right) / 2;
	p.y=(rect.top + rect.bottom) / 2;

	GLdouble ax,ay,az,x,y,z;
	GLint viewport[4];
	GLdouble projMatrix[16],modelMatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	glGetIntegerv(GL_VIEWPORT, viewport);

	p.y = viewport[3] - p.y;

	gluUnProject(p.x, p.y, 0.5, modelMatrix, projMatrix, viewport, &x, &y, &z);
	glTranslated(-x, -y, -z);
	scale = min(fabs((double)(viewport[2] - viewport[0]) / rect.Width()),
				fabs((double)(viewport[3] - viewport[1]) / rect.Height()));
//	glTranslated(-x, -y, -z);
	DoScale(scale);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	gluUnProject((viewport[0] + viewport[2]) / 2, (viewport[1] + viewport[3]) / 2, 0.5,
		modelMatrix, projMatrix, viewport, &ax, &ay, &az);
	glTranslated(ax - x, ay - y, az - z);
	if(ViewHistory.Add(GetModelMatr()))
		GetDocument()->SetModifiedFlag();
	RestoreContext();

	m_ChangeView = 2;
	Invalidate();
}

void BOpenGLWnd::DoScale(double factor)
{
	if(factor > 1.)
	{// prevent extra large zooming
		GLdouble modelMatrix[16];
		glGetDoublev(GL_MODELVIEW_MATRIX ,modelMatrix);
		double d =	modelMatrix[ 0] * modelMatrix[ 0]+
					modelMatrix[ 5] * modelMatrix[ 5]+
					modelMatrix[10] * modelMatrix[10];
		if(d > MAX_SCALE * MAX_SCALE)
			return;
	}
	glScaled(factor,factor,factor);
}

void BOpenGLWnd::DoZoomR(CPoint point)
{
	if (!ZoomRActive)
		return;
	ZoomRActive->TrackRubberBand( this, point, TRUE );
	delete ZoomRActive;
	ZoomRActive = NULL;
	if (ViewHistory.Add(GetModelMatr()))
		GetDocument()->SetModifiedFlag();
}

void BOpenGLWnd::CloseToolBar()
{
}

void BOpenGLWnd::MakeCurrent()
{
	ASSERT(m_hrc);
	ASSERT(m_pDC);

	hDCb = wglGetCurrentDC();
	hRCb = wglGetCurrentContext();

	if (m_hrc != hRCb)
	{
		// Делаем контекст воспроизведения hrc текущим
		//
    	BOOL bResult = wglMakeCurrent(m_pDC->GetSafeHdc(), m_hrc);
		if (!bResult)
		{
			TRACE("wglMakeCurrent Failed %x\r\n", GetLastError() ) ;
			return ;
		}
	}
}

void BOpenGLWnd::DoZoomAll()
{
	MakeCurrent();
	GLdouble x, y, z, ax, ay, az;

	BBox ScreenDim;
	BBox ObjectDim = GetSceneGabar(true);
	if(!ObjectDim.IsDefined())
		return;
	ObjectDim.ApplyLimits();
	ScreenDim.Project(ObjectDim);

	GLdouble scale;
	CPoint p;
	
	x = (ObjectDim.Xmin + ObjectDim.Xmax) / 2;
	y = (ObjectDim.Ymin + ObjectDim.Ymax) / 2;
	z = (ObjectDim.Zmin + ObjectDim.Zmax) / 2;
	glTranslated(-x, -y, -z);

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	scale = min((double)(viewport[2] - viewport[0]) / ScreenDim.dX(),
				(double)(viewport[3] - viewport[1]) / ScreenDim.dY());
	scale *= 0.9;
	glScaled(scale, scale, scale);
	glFinish();

	GLdouble projMatrix[16],modelMatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	gluUnProject((viewport[0] + viewport[2]) / 2, (viewport[1] + viewport[3]) / 2, 0.5,
		modelMatrix, projMatrix,viewport, &ax, &ay, &az);
	glTranslated(ax - x, ay - y, az - z);

	if (ViewHistory.Add(GetModelMatr()))
		GetDocument()->SetModifiedFlag();
	RestoreContext();

	m_ChangeView = 2;
	Invalidate();
}

void BOpenGLWnd::NUpdate(const CRect *ClipRect)
{
	MakeCurrent();

	if (m_ChangeView == 1 || m_ChangeView == 2)
	{
		// Save previos matrix in BufMatr
		for(int i = 0; i < 4; ++i)
			for(int j = 0; j < 4; ++j)
				(BufMatr.GetArray())[j + 4 * i] = ViewMatrix[0][i][j];

		SaveView(0);
//		ViewHistory.Add(BMatr(ViewMatrix[0]));
	}
	RestoreView(0);
	RestoreContext();
}

void BOpenGLWnd::OnDestroy() 
{
	delete pOpenGLCtrl;
	pOpenGLCtrl = nullptr;
	BaseOGLWnd::OnDestroy();
}


void BOpenGLWnd::OnSize(UINT nType, int cx, int cy) 
{
	if(cy > 0 && cx >0)
	{
		AdjustLayout();
	}

	BaseOGLWnd::OnSize(nType, cx, cy);
}


void BOpenGLWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (ViewHistory.Add(GetModelMatr()))
		GetDocument()->SetModifiedFlag();
	CloseToolBar();
	if (pOpenGLCtrl && !SmallSize)
	{
		MakeCurrent();
		Shader::ShaderOFF();
		pOpenGLCtrl->OnLButtonDown(nFlags, point);
		RestoreContext();
	}
	ProcMouseMes(WM_LBUTTONDOWN, nFlags, point);
	
	BaseOGLWnd::OnLButtonDown(nFlags, point);
	GetParent()->SetFocus();
}

int BOpenGLWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{

	if (BaseOGLWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	pOpenGLCtrl = new BOpenGLCtrl(this);

	Init(); // initialize OpenGL

//	glText.Init(wglGetCurrentDC(), USEFONTBITMAPS, "arial.ttf");

	InitialUpdate();

	return 0;
}

void BOpenGLWnd::OnContextMenu(CWnd* pWnd, CPoint point) 
{
}

void BOpenGLWnd::GetArcPoint(double u, int nknots, float *ctlarray, float *knot, double &x, double &y, double &z)
{
	int k = (nknots - 5)/3;// the segments number
	BPoint p,p0,p1,p2,p3;
	int i = int(u*k-10e-6); // number of selected segment
	double t = u*k - i;// parameter on selected segment;
	p0.Set(ctlarray[12*i+0],ctlarray[12*i+1],ctlarray[12*i+2],ctlarray[12*i+3]);
	p1.Set(ctlarray[12*i+0+4],ctlarray[12*i+1+4],ctlarray[12*i+2+4],ctlarray[12*i+3+4]);
	p2.Set(ctlarray[12*i+0+8],ctlarray[12*i+1+8],ctlarray[12*i+2+8],ctlarray[12*i+3+8]);
	p3.Set(ctlarray[12*i+0+12],ctlarray[12*i+1+12],ctlarray[12*i+2+12],ctlarray[12*i+3+12]);
	p = p0*((1.-t)*(1.-t)*(1.-t)) + p1*(3.*(1.-t)*(1.-t)*t)
		+ p2*(3.*(1.-t)*t*t) + p3*(t*t*t);
	x=p.GetX()/p.GetH();
	y=p.GetY()/p.GetH();
	z=p.GetZ()/p.GetH();
}

void BOpenGLWnd::OnMButtonDown(UINT nFlags, CPoint point) 
{
	if (ViewHistory.Add(GetModelMatr()))
		GetDocument()->SetModifiedFlag();
	BaseOGLWnd::OnMButtonDown(nFlags, point);
	GetParent()->SetFocus();
}

void BOpenGLWnd::ClearBackground(void)
{
	IsOpenGLErr("ClearBackground0");
	Shader::ShaderOFF();
	if(!GradientEnable)
	{
		glClearColor(BackCol[0], BackCol[1], BackCol[2], 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		return;
	}
/// GRADIENT BACKGROUND
/// matrix set-up
/// we use an orthographic view, and save all matrices
///
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glClear(GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-1., 1., -1., 1., -1., 1.);


	glPushAttrib(GL_ENABLE_BIT | GL_POLYGON_BIT | GL_LIGHTING_BIT );

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	/// disable the expensive tests
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glShadeModel(GL_SMOOTH);

	glBegin(GL_QUADS);
	glColor3fv(BackColLT);
	glVertex2i(-1,  1);
	glColor3fv(BackColRT);
	glVertex2i( 1,  1);

	glColor3fv(BackColRB);
	glVertex2i( 1, -1);
	glColor3fv(BackColLB);
	glVertex2i(-1, -1);
	glEnd();
	glPopAttrib();

	glPopMatrix();

	glMatrixMode (GL_MODELVIEW);
	glPopMatrix();
	IsOpenGLErr("ClearBackground1");
}

void BOpenGLWnd::RestoreContext(void)
{
	if (m_hrc != hRCb)
		if (hDCb != NULL && hRCb != NULL)
			wglMakeCurrent(hDCb, hRCb);
}

void BOpenGLWnd::GetMouseCoord(const CPoint & point, double *wz, double * x, double * y, double * z)
{
	GetCPointCoord(point, wz, x, y, z);
}

void BOpenGLWnd::GetMouseCoordGL(const CPoint & point, double *wz, double * x, double * y, double * z)
{
	GetCPointCoord(point, wz, x, y, z);
}

void BOpenGLWnd::GetMouseRay(const CPoint & point, BPoint &P, BPoint &V)
{
	GLint viewport[4];
	GLdouble projMatrix[16],modelMatrix[16];
	glMatrixMode(GL_MODELVIEW);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	glGetIntegerv(GL_VIEWPORT, viewport);
	int px = min(point.x, viewport[2] - 1);
	int py = min(point.y, viewport[3] - 1);
	px = max(1, px);
	py = max(1, py);
	double x, y, z;
	gluUnProject(px + 0.5, viewport[3] - py + 0.5, 0., modelMatrix, projMatrix, viewport,
		&x, &y, &z);
	P.Set(x, y, z, 1.);
	gluUnProject(px + 0.5, viewport[3] - py + 0.5, 1., modelMatrix, projMatrix, viewport,
		&x, &y, &z);
	V.Set(x, y, z, 1.);
	V -= P;
}
void BOpenGLWnd::GetCPointCoord(const CPoint & point, double *wz, double * x, double * y, double * z)
{
	GLint viewport[4];
	GLdouble projMatrix[16],modelMatrix[16];
	glMatrixMode(GL_MODELVIEW);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	glGetIntegerv(GL_VIEWPORT ,viewport);
	*wz = 0.5;
	gluUnProject(point.x, viewport[3] - point.y, *wz, modelMatrix, projMatrix, viewport,
			x,y,z);
}

void BOpenGLWnd::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	SetFocus();
	CloseToolBar();

	BaseOGLWnd::OnNcLButtonDown(nHitTest, point);
}

void BOpenGLWnd::ClearZoomActive(void)
{
	CloseToolBar();
	ZoomSaveActive = FALSE;
	ZoomNormActive = FALSE;
	delete ZoomWActive;
	ZoomWActive = nullptr;
	delete ZoomRActive;
	ZoomRActive = nullptr;
	auto pViewVD = dynamic_cast<BOpenGLWndVD*>(this);
	if (pViewVD != nullptr)
		pViewVD->SetCursorID('0');
}

bool BOpenGLWnd::IsActive(void)
{
	if(GetFocus() == this)
		return true;
	return false;
}

void BOpenGLWnd::OnKillFocus(CWnd* pNewWnd)
{
	BaseOGLWnd::OnKillFocus(pNewWnd);

	CloseToolBar();
}

BOOL BOpenGLWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CloseToolBar();
	ScreenToClient(&pt);
	if (pOpenGLCtrl && !SmallSize)
	{
		MakeCurrent();
		Shader::ShaderOFF();
		BOOL Res = pOpenGLCtrl->OnMouseWheel(nFlags, zDelta, pt);
		RestoreContext();
		if (Res)
			return FALSE;
	}
	AppScale(zDelta * (NCM_PROJECT.GetGlobalState().GetWheelReverse() ? -1 : 1), pt);

	return BaseOGLWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void BOpenGLWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (ViewHistory.Add(GetModelMatr()))
		GetDocument()->SetModifiedFlag();
	ProcMouseMes(WM_RBUTTONDOWN, nFlags, point);
	BaseOGLWnd::OnRButtonDown(nFlags, point);
}

void BOpenGLWnd::OnRButtonUp(UINT nFlags, CPoint point)
{
	ProcMouseMes(WM_RBUTTONUP, nFlags, point);
	BaseOGLWnd::OnRButtonUp(nFlags, point);
}

void BOpenGLWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	if (pOpenGLCtrl && !SmallSize)
	{
		MakeCurrent();
		Shader::ShaderOFF();
		BOOL Res = pOpenGLCtrl->OnMouseMove(nFlags, point);
		RestoreContext();
		if (Res)
			return;
	}
	ProcMouseMes(WM_MOUSEMOVE, nFlags, point);

	BaseOGLWnd::OnMouseMove(nFlags, point);
}

void BOpenGLWnd::SerializeMatrices(CArchive & ar)
{
	SerializeElements(ar, ViewMatrix, ViewMatrNum);
	// Check for errors
	if(ar.IsLoading())
	{
		for(int i = 0; i < ViewMatrNum; ++i)
		{
			bool Error = false;
			for(int j = 0; j < 4; ++j)
				for(int k = 0; k < 4; ++k)
					if(ViewMatrix[i][j][k] > 1.e7)
						Error = true;
			if(Error)
			{
				for(int j = 0; j < 4; ++j)
					for(int k = 0; k < 4; ++k)
						ViewMatrix[i][j][k] = (j == k) ? 1. : 0.;
			}
		}
	}
}

void BOpenGLWnd::DoTbViewLa()
{
	ClearZoomActive();	
	DoStdView("LA");
	m_ChangeView = 2;
	Invalidate();
}

void BOpenGLWnd::InitialUpdate()
{
}

void BOpenGLWnd::DoTbViewZx()
{
 	ClearZoomActive();	
	DoStdView("ZX");
	m_ChangeView = 2;
	Invalidate();
}

void BOpenGLWnd::DoTbVewXz()
{
 	ClearZoomActive();	
	DoStdView("XZ");
	m_ChangeView = 2;
	Invalidate();
}

void BOpenGLWnd::DoTbVewZy()
{
 	ClearZoomActive();	
	DoStdView("ZY");
	m_ChangeView = 2;
	Invalidate();
}

void BOpenGLWnd::DoTbViewXyleft()
{
 	ClearZoomActive();	
	DoStdView("XL");
	m_ChangeView = 2;
	Invalidate();
}

void BOpenGLWnd::DoTbViewXyright()
{
	ClearZoomActive();	
	DoStdView("XR");
	m_ChangeView = 2;
	Invalidate();
}

void BOpenGLWnd::DoTbViewT1()
{
 	ClearZoomActive();	
	DoStdView("T1");
	m_ChangeView = 2;
	Invalidate();
}

void BOpenGLWnd::DoTbViewT2()
{
 	ClearZoomActive();	
	DoStdView("T2");
	m_ChangeView = 2;
	Invalidate();
}

void BOpenGLWnd::DoTbViewT3()
{
 	ClearZoomActive();	
	DoStdView("T3");
	m_ChangeView = 2;
	Invalidate();
}

void BOpenGLWnd::DoTbViewT4()
{
	ClearZoomActive();
	DoStdView("T4");
	m_ChangeView = 2;
	Invalidate();
}

void BOpenGLWnd::DoTbViewT5()
{
	ClearZoomActive();
	DoStdView("T5");
	m_ChangeView = 2;
	Invalidate();
}
void BOpenGLWnd::DoTbViewT6()
{
	ClearZoomActive();
	DoStdView("T6");
	m_ChangeView = 2;
	Invalidate();
}
void BOpenGLWnd::DoTbViewT7()
{
	ClearZoomActive();
	DoStdView("T7");
	m_ChangeView = 2;
	Invalidate();
}

void BOpenGLWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);
	GLint cx = rectClient.Width();
	GLint cy = rectClient.Height();
	if (cx < 0 || cy < 0)
		return;
	GLint MaxDims[2];
	glGetIntegerv(GL_MAX_VIEWPORT_DIMS, MaxDims);
	if (MaxDims[0] <= 0 || MaxDims[1] <= 0)
		return;

	SmallSize = cx < BOpenGLCtrl::GetZoneXSize() && cy < BOpenGLCtrl::GetZoneYSize();
	//Store current rendering and device contexts
	MakeCurrent();

	// Check max viewport dimensions
	cx = min(cx, MaxDims[0] - 1);
	cy = min(cy, MaxDims[1] - 1);


	glViewport(0, 0, cx, cy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-(double)cx,(double)cx,
			 -(double)cy,(double)cy,
			 -1000.,1000.);
	glMatrixMode(GL_MODELVIEW);



	NUpdate();
	IsOpenGLErr("OnSize");
	// Restore last rendering and device contexts
	RestoreContext();
}

void BOpenGLWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// Do not call CWnd::OnPaint() for painting messages
	NUpdate();
}

void BOpenGLWnd::OnSetFocus(CWnd* pOldWnd)
{
	BaseOGLWnd::OnSetFocus(pOldWnd);
}
void BOpenGLWnd::DrawGLText(double x, double y, double z, char *text2draw)
{
	MainText.Draw3D(x, y, z, text2draw);
}

BOOL BOpenGLWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	switch(((LPNMHDR)lParam)->code)
	{
	case TTN_NEEDTEXT:
		AfxGetMainWnd()->SendMessage(WM_NOTIFY, wParam, lParam);
		break;
	}

	return BaseOGLWnd::OnNotify(wParam, lParam, pResult);
}
const BPoint BOpenGLWnd::GetViewDir()
{
	return BaseRender::GetViewDir();
}

BOpenGLWnd::NRotCenter::NRotCenter()
{
	NTiParams &Par = NCM_PROJECT.Defaults;
	RotBasePoint.Set(0., 0., 0., Par.GetBool("Defaults/View/RotCenter@Fixed", false) ? 1. : -1.);
	Color.SetRGB(RGB(0, 255, 0));
	Visible = false;
	Rad = 5.;
}

const BPoint BOpenGLWnd::GetRotBase(void) const
{
	return RotCenter.RotBasePoint.IsPoint() ? RotCenter.RotBasePoint : (ZoomRActive == NULL ? BPoint(0., 0., 0., -1.) : ZoomRActive->GetBase()); 
}

void BOpenGLWnd::DrawRotCenter(void)
{
	if(ZoomRActive || RotCenter.Visible)
	{
		glPushAttrib(GL_CURRENT_BIT);
		glColor3fv(RotCenter.Color.array);
		BaseRender::DrawBall(GetRotBase(), RotCenter.Rad, true);
		glPopAttrib();
	}
}

void BOpenGLWnd::MoveAlongAxis(char Axis, double Shift)
{
	MakeCurrent();
	double dX = 0., dY = 0., dZ = 0.;
	switch (Axis)
	{
	case 'X'://Rotate around X-Axes in object coord
		dX = Shift;
		break;
	case 'Y'://Rotate around Y-Axes in object coord
		dY = Shift;
		break;
	case 'Z'://Rotate around Z-Axes in object coord
		dZ = Shift;
		break;
	default:
		break;
	}

	// Translate to original position
	glTranslated(dX, dY, dZ);

	RestoreContext();

	if (ViewHistory.Add(GetModelMatr()))
		GetDocument()->SetModifiedFlag();
	m_ChangeView = 1;
	Invalidate();
}

void BOpenGLWnd::RotateAroundRotCenter(char Axis, double Angle)
{
	BPoint Center = GetRotBase();
	
	MakeCurrent();
	// Translate to 0,0,0
	glTranslated(Center.GetX(), Center.GetY(), Center.GetZ());
	switch(Axis)
	{
	case 'X'://Rotate around X-Axes in object coord
		glRotated(Angle, 1., 0., 0.);
		break;
	case 'Y'://Rotate around Y-Axes in object coord
		glRotated(Angle, 0., 1., 0.);
		break;
	case 'Z'://Rotate around Z-Axes in object coord
		glRotated(Angle, 0., 0., 1.);
		break;
	default:
		break;
	}

// Translate to original position
	glTranslated(-Center.GetX(), -Center.GetY(), -Center.GetZ());

	RestoreContext();

	if (ViewHistory.Add(GetModelMatr()))
		GetDocument()->SetModifiedFlag();
	m_ChangeView = 2;
	Invalidate();
}

void BOpenGLWnd::DrawScene(RenderMode GlobRMode)
{
	if (pOpenGLCtrl && !SmallSize)
	{
		if (GlobRMode != RM_TRANSLUCENT)
		{
			MakeCurrent();
			Shader::ShaderOFF();
			pOpenGLCtrl->Draw();
			CPoint Point;
			GetCursorPos(&Point);
			ScreenToClient(&Point);
			pOpenGLCtrl->OnMouseMove(0, Point);
			RestoreContext();
		}
	}
}

void BOpenGLWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (pOpenGLCtrl && !SmallSize)
	{
		MakeCurrent();
		Shader::ShaderOFF();
		pOpenGLCtrl->OnLButtonUp(nFlags, point);
		RestoreContext();
	}

	ProcMouseMes(WM_LBUTTONUP, nFlags, point);
	if (ViewHistory.Add(GetModelMatr()))
		GetDocument()->SetModifiedFlag();
	BaseOGLWnd::OnLButtonUp(nFlags, point);
}


void BOpenGLWnd::OnMButtonUp(UINT nFlags, CPoint point)
{
	if (ViewHistory.Add(GetModelMatr()))
		GetDocument()->SetModifiedFlag();

	BaseOGLWnd::OnMButtonUp(nFlags, point);
}

BOOL BOpenGLWnd::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case 0x58://X
		case 0x59://Y
		case 0x5A://Z
			return TRUE;// To prevent standard sound
		}
	}

	return BaseOGLWnd::PreTranslateMessage(pMsg);
}

void BOpenGLWnd::AppScale(short zDelta, CPoint pt)
{
	double wz, bx, by, bz;
	MakeCurrent();
	GetCPointCoord(pt, &wz, &bx, &by, &bz);
	glTranslated(bx, by, bz);

	double factor = 1.;
	if (zDelta < 0)
		factor = 1.3;
	if (zDelta > 0)
		factor = 1. / 1.3;

	//	factor = pow(factor, abs(zDelta / WHEEL_DELTA)); // removed to prevent huge scale changing when cutting stopped

	DoScale(factor);

	glTranslated(-bx, -by, -bz);

	m_ChangeView = 1;
	if (ViewHistory.Add(GetModelMatr()))
		GetDocument()->SetModifiedFlag();
	Invalidate();
}

void BOpenGLWnd::ShowViewToolbar(CPoint point)
{
}

void BOpenGLWnd::PrepTracker(NRotTracker *& Tracker, char axes, WORD StopMessage)
{
	ClearZoomActive();
	Tracker = new NRotTracker(this, axes, GetRotBase(), StopMessage);
}

bool BOpenGLWnd::ProcMouseMes(WORD Mes, UINT nFlags, CPoint point)
{
	switch (Mes)
	{
	default:
		return false;
	case WM_LBUTTONDOWN:
		if (!DoViewAction(WM_LBUTTONDOWN, nFlags, point))
			DoZoom(point, WM_LBUTTONUP);

		break;
	case WM_MBUTTONDOWN:
		DoViewAction(WM_MBUTTONDOWN, nFlags, point);
		break;
	case WM_RBUTTONDOWN:
		if (!DoViewAction(WM_RBUTTONDOWN, nFlags, point))
		{
			if (nFlags == MK_RBUTTON)
			{
				ClearZoomActive();
				RedrawWindow();
			}
		}
		break;
	case WM_LBUTTONUP:
		DoViewAction(WM_LBUTTONUP, nFlags, point);
		break;
	case WM_MBUTTONUP:
		DoViewAction(WM_MBUTTONUP, nFlags, point);
		break;
	case WM_RBUTTONUP:
		if (!DoViewAction(WM_RBUTTONUP, nFlags, point))
		{
			if (nFlags == 0)
				ShowViewToolbar(point);
		}
		break;
	case WM_MOUSEMOVE:
		DoViewAction(WM_MOUSEMOVE, nFlags, point);
		break;
	}
	return true;
}

void BOpenGLWnd::BSwapBuffers()
{
	if (ZoomWActive)
	{
		ZoomWActive->SetPictureChanged();
		ZoomWActive->DrawRect();
	}
	SwapBuffers(wglGetCurrentDC());
}

bool BOpenGLWnd::PrepPan(CPoint point, WORD StopMessage)
{
	PrepTracker(ZoomRActive, 'P', StopMessage);
	DoZoomR(point);
	return true;
}

bool BOpenGLWnd::PrepScale(CPoint point, WORD StopMessage)
{
	PrepTracker(ZoomRActive, 'S', StopMessage);
	DoZoomR(point);
	return true;
}

bool BOpenGLWnd::PrepRot(CPoint point, WORD StopMessage)
{
	PrepTracker(ZoomRActive, 'A', StopMessage);
	DoZoomR(point);
	return true;
}

bool BOpenGLWnd::PrepRotX(CPoint point, WORD StopMessage)
{
	PrepTracker(ZoomRActive, 'X', StopMessage);
	DoZoomR(point);
	return true;
}

bool BOpenGLWnd::PrepRotY(CPoint point, WORD StopMessage)
{
	PrepTracker(ZoomRActive, 'Y', StopMessage);
	DoZoomR(point);
	return true;
}

bool BOpenGLWnd::PrepRotZ(CPoint point, WORD StopMessage)
{
	PrepTracker(ZoomRActive, 'Z', StopMessage);
	DoZoomR(point);
	return true;
}

bool BOpenGLWnd::PrepZoomW(CPoint point, WORD StopMessage)
{
	PrepTracker(ZoomWActive, 'W', StopMessage);
	DoZoomW(point);
	return true;
}

bool BOpenGLWnd::DoZoom(CPoint point, WORD StopMessage)
{
	if (ZoomRActive)
		DoZoomR(point);
	else if (ZoomWActive)
		DoZoomW(point);
	else if (ZoomNormActive)
		DoZoomNorm(point);
	return true;
}

bool BOpenGLWnd::DoViewAction(WORD Message, UINT nFlags, CPoint point)
{
	std::pair<int, WORD> Action = NCM_PROJECT.GetGlobalState().GetViewActionInd(Message, nFlags);
	if (Action.first >= 0)
		return (this->*ViewActions[Action.first])(point, Action.second);
	return false;
}

void BOpenGLWnd::GetImage(BImageRGB& ImageRGB)
{
	MakeCurrent();

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	GLint OldPackAl;// alignment of the row in ReadPixels (4- default)
	glGetIntegerv(GL_PACK_ALIGNMENT, &OldPackAl);
	glPixelStorei(GL_PACK_ALIGNMENT, 4);
	GLint PackAl;// alignment of the row in ReadPixels (4- default)
	glGetIntegerv(GL_PACK_ALIGNMENT, &PackAl);
	const int memSize = (PackAl - (3 * viewport[2]) % PackAl + 3 * viewport[2])* viewport[3];

	unsigned char* ColorBuf = new unsigned
		char[memSize];

	glReadBuffer(GL_BACK);
	glReadPixels(0, 0, viewport[2], viewport[3], GL_BGR_EXT, GL_UNSIGNED_BYTE, ColorBuf);
	glPixelStorei(GL_PACK_ALIGNMENT, PackAl);

	ImageRGB.Set(memSize, viewport[2], viewport[3], ColorBuf);

	RestoreContext();
}

CDocument* BOpenGLWnd::GetDocument()
{
	CFrameWnd* pMainFrame = (CFrameWnd*)AfxGetMainWnd();
	if (pMainFrame == nullptr)
		return nullptr;
	if (!pMainFrame->IsKindOf(RUNTIME_CLASS(CFrameWndEx)))
		return nullptr;
	return pMainFrame->GetActiveDocument();
}

void BOpenGLWnd::SetDefaultOGL(void)
{
	int Shiness = 64;//Temporary
	float S[4] = { 0.25f,0.25f,0.25f,1.0f };//Temporary
	float A[4] = { 1.0f,1.0f,1.0f,1.0f };//Temporary
	float LD[4] = { 0.6f,0.6f,0.6f,1.0f };//Temporary
	float LA[4] = { 0.3f,0.3f,0.3f,1.0f };//Temporary
	float LS[4] = { 0.49f,0.49f,0.49f,1.0f };//Temporary

	glDrawBuffer(GL_BACK);
	glReadBuffer(GL_BACK);// same as in DrawBuffer
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, LA);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LD);
	glLightfv(GL_LIGHT0, GL_SPECULAR, LS);

	//	float LP[4] = {0.f,0.f,100.f,1.};//Temporary
	//	glLightfv(GL_LIGHT0, GL_POSITION, LP);
	//	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05);

	if (LineSmooth)
		glEnable(GL_LINE_SMOOTH);
	else
		glDisable(GL_LINE_SMOOTH);
	//	glEnable(GL_POLYGON_SMOOTH) ;
	//	glEnable(GL_POINT_SMOOTH) ;
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glLineWidth(1.f);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Set the material color to follow the current color
//	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, S);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, A);
	//	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE ,D);
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, Shiness);

	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void BOpenGLWnd::SetCursorID(char Axes)
{
	switch (Axes)
	{
	case 'A':
		CursorID = IDC_BCUR_A;
		break;
	case 'X'://Rotate around X-Axes in object coord
		CursorID = IDC_BCUR_X;
		break;
	case 'Y'://Rotate around Y-Axes in object coord
		CursorID = IDC_BCUR_Y;
		break;
	case 'Z'://Rotate around Z-Axes in object coord
		CursorID = IDC_BCUR_Z;
		break;
	case 'S'://Scale
		CursorID = IDC_BCUR_S;
		break;
	case 'P'://Pan
		CursorID = IDC_BCUR_P;
		break;
	case 'N'://Pan
		CursorID = IDC_BCUR_P;// to change
		break;
	case 'W'://Window
		CursorID = IDC_BCUR_W;
		glAccum(GL_LOAD, 1.0);
		break;
	default:
		CursorID = 0;
		break;
	}
}

void BOpenGLWnd::OnRotAngle()
{
#ifndef SHARED_HANDLERS
	CloseToolBar();
	NRotAngles RotAngles;
	RotAngles.DoModal(this);
#endif
}

void BOpenGLWnd::ApplyRotAngle(char Axis, double Angle)
{
	RotateAroundRotCenter(Axis, Angle);
	AfxGetMainWnd()->SendMessage(WM_APP_UPDATE_PANES, N_OGL);
}



//CRect BOpenGLWnd::StartTracker(NRotTracker *& Tracker, CPoint point)
//{
//	if (!Tracker)
//		return;
//	Tracker->TrackRubberBand(this, point, TRUE);
//	CRect rect = Tracker->m_rect;
//	delete Tracker;
//	Tracker = NULL;
//	ViewHistory.Add(GetModelMatr());
//	return rect;
//}

// Возможные действия
// 1. Подготовить дин операцию (ждать нажатия клавиши мыши)
// 2. Начать и выполнить дин операцию DoZoomP DoZoomW
// 3. Вписать DoTbZoomAll
// 4. Clear
// 5. ShowViewToolBar

// Условия
// Ctrl
// Shift
// Alt
// LButton
// MButton
// RButton
// X
// Y
// Z
// ZoomWActive
// ZoomRActive
// События
// LDown
// MDown
// RDown
// LUp
// MUp
// RUp
// Move
