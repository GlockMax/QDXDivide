
// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "Integration.h"
#include "gl/gl.h"
#include "gl/glu.h"
#include "..\NCManagerAPI\NCM_API.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()

NCM_API Ncm;

// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
		int Shiness = 64;//Temporary
		float S[4] = {0.4f,0.4f,0.4f,1.0f};//Temporary
		float A[4] = {1.0f,1.0f,1.0f,1.0f};//Temporary
		float LD[4] = {1.f,1.f,1.f,1.0f};//Temporary
		float LA[4] = {0.2f,0.2f,0.2f,1.0f};//Temporary
		float LS[4] = {0.6f,0.6f,0.6f,1.0f};//Temporary

	glDrawBuffer(GL_BACK);
	glReadBuffer(GL_BACK);// same as in DrawBuffer
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);

	glEnable(GL_LIGHTING) ; 
	glEnable(GL_LIGHT0) ;
	glLightfv(GL_LIGHT0, GL_AMBIENT, LA);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LD);
	glLightfv(GL_LIGHT0, GL_SPECULAR, LS);

//	float LP[4] = {0.f,0.f,100.f,1.};//Temporary
//	glLightfv(GL_LIGHT0, GL_POSITION, LP);
//	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05);

		glEnable(GL_LINE_SMOOTH);
//	glEnable(GL_POLYGON_SMOOTH) ;
//	glEnable(GL_POINT_SMOOTH) ;
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); 
	glLineWidth(1.f);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Set the material color to follow the current color
//	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_COLOR_MATERIAL) ;
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE) ;
//TEMPORARY
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,S);
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,A);
//	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE ,D);
	glMateriali(GL_FRONT_AND_BACK,GL_SHININESS ,Shiness);

	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	CPaintDC dc(this); // device context for painting
	
	static BOOL     bBusy = FALSE;
	static GLfloat  wAngleY = 10.0f;
	static GLfloat  wAngleX = 1.0f;
	static GLfloat  wAngleZ = 5.0f;

	if(bBusy)
		return;
	bBusy = TRUE;

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

		glTranslatef(0.0f, 0.0f, -m_fRadius);
		glRotatef(wAngleX, 1.0f, 0.0f, 0.0f);
		glRotatef(wAngleY, 0.0f, 1.0f, 0.0f);
		glRotatef(wAngleZ, 0.0f, 0.0f, 1.0f);

		wAngleX += 1.0f;
		wAngleY += 10.0f;
		wAngleZ += 5.0f;

	Ncm.renderStock();

		//glBegin(GL_QUAD_STRIP);
		//	glColor3f(1.0f, 0.0f, 1.0f);
		//	glVertex3f(-0.5f, 0.5f, 0.5f);

		//	glColor3f(1.0f, 0.0f, 0.0f);
		//	glVertex3f(-0.5f, -0.5f, 0.5f);

		//	glColor3f(1.0f, 1.0f, 1.0f);
		//	glVertex3f(0.5f, 0.5f, 0.5f);

		//	glColor3f(1.0f, 1.0f, 0.0f);
		//	glVertex3f(0.5f, -0.5f, 0.5f);

		//	glColor3f(0.0f, 1.0f, 1.0f);
		//	glVertex3f(0.5f, 0.5f, -0.5f);

		//	glColor3f(0.0f, 1.0f, 0.0f);
		//	glVertex3f(0.5f, -0.5f, -0.5f);

		//	glColor3f(0.0f, 0.0f, 1.0f);
		//	glVertex3f(-0.5f, 0.5f, -0.5f);

		//	glColor3f(0.0f, 0.0f, 0.0f);
		//	glVertex3f(-0.5f, -0.5f,  -0.5f);

		//	glColor3f(1.0f, 0.0f, 1.0f);
		//	glVertex3f(-0.5f, 0.5f, 0.5f);

		//	glColor3f(1.0f, 0.0f, 0.0f);
		//	glVertex3f(-0.5f, -0.5f, 0.5f);

		//glEnd();

		//glBegin(GL_QUADS);
		//	glColor3f(1.0f, 0.0f, 1.0f);
		//	glVertex3f(-0.5f, 0.5f, 0.5f);

		//	glColor3f(1.0f, 1.0f, 1.0f);
		//	glVertex3f(0.5f, 0.5f, 0.5f);

		//	glColor3f(0.0f, 1.0f, 1.0f);
		//	glVertex3f(0.5f, 0.5f, -0.5f);

		//	glColor3f(0.0f, 0.0f, 1.0f);
		//	glVertex3f(-0.5f, 0.5f, -0.5f);
		//glEnd();

		//glBegin(GL_QUADS);
		//	glColor3f(1.0f, 0.0f, 0.0f);
		//	glVertex3f(-0.5f, -0.5f, 0.5f);

		//	glColor3f(1.0f, 1.0f, 0.0f);
		//	glVertex3f(0.5f, -0.5f, 0.5f);

		//	glColor3f(0.0f, 1.0f, 0.0f);
		//	glVertex3f(0.5f, -0.5f, -0.5f);

		//	glColor3f(0.0f, 0.0f, 0.0f);
		//	glVertex3f(-0.5f, -0.5f,  -0.5f);
		//glEnd();

	glPopMatrix();

	glFinish();
	SwapBuffers(wglGetCurrentDC());

	bBusy = FALSE;
}

void CChildView::Init()
{
	PIXELFORMATDESCRIPTOR pfd;
	int         n;
	HGLRC       hrc;
	GLfloat     fMaxObjSize, fAspect;
	GLfloat     fNearPlane, fFarPlane;

	m_pDC = new CClientDC(this);

	ASSERT(m_pDC != NULL);

	if (!bSetupPixelFormat())
		return;

	n = ::GetPixelFormat(m_pDC->GetSafeHdc());
	::DescribePixelFormat(m_pDC->GetSafeHdc(), n, sizeof(pfd), &pfd);

	//CreateRGBPalette();

	hrc = wglCreateContext(m_pDC->GetSafeHdc());
	wglMakeCurrent(m_pDC->GetSafeHdc(), hrc);

	GetClientRect(&m_oldRect);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	if (m_oldRect.bottom)
		fAspect = (GLfloat)m_oldRect.right/m_oldRect.bottom;
	else    // don't divide by zero, not that we should ever run into that...
		fAspect = 1.0f;
	fNearPlane = 3.0f;
	fFarPlane = 7.0f;
	fMaxObjSize = 3.0f;
	m_fRadius = fNearPlane + fMaxObjSize / 2.0f;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, fAspect, fNearPlane, fFarPlane);
	glMatrixMode(GL_MODELVIEW);
}

BOOL CChildView::bSetupPixelFormat()
{
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
		1,                              // version number
		PFD_DRAW_TO_WINDOW |            // support window
		  PFD_SUPPORT_OPENGL |          // support OpenGL
		  PFD_DOUBLEBUFFER,             // double buffered
		PFD_TYPE_RGBA,                  // RGBA type
		24,                             // 24-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		32,                             // 32-bit z-buffer
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
	};
	int pixelformat;

	if ( (pixelformat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd)) == 0 )
	{
		MessageBox("ChoosePixelFormat failed");
		return FALSE;
	}

	if (SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd) == FALSE)
	{
		MessageBox("SetPixelFormat failed");
		return FALSE;
	}

	return TRUE;
}

unsigned char CChildView::ComponentFromIndex(int i, UINT nbits, UINT shift)
{
	return 0;
	//unsigned char val;

	//val = (unsigned char) (i >> shift);
	//switch (nbits)
	//{

	//case 1:
	//	val &= 0x1;
	//	return oneto8[val];
	//case 2:
	//	val &= 0x3;
	//	return twoto8[val];
	//case 3:
	//	val &= 0x7;
	//	return threeto8[val];

	//default:
	//	return 0;
	//}
}

#pragma warning(disable : 4244)
void CChildView::CreateRGBPalette()
{
	//PIXELFORMATDESCRIPTOR pfd;
	//LOGPALETTE *pPal;
	//int n, i;

	//n = ::GetPixelFormat(m_pDC->GetSafeHdc());
	//::DescribePixelFormat(m_pDC->GetSafeHdc(), n, sizeof(pfd), &pfd);

	//if (pfd.dwFlags & PFD_NEED_PALETTE)
	//{
	//	n = 1 << pfd.cColorBits;
	//	pPal = (PLOGPALETTE) new char[sizeof(LOGPALETTE) + n * sizeof(PALETTEENTRY)];

	//	ASSERT(pPal != NULL);

	//	pPal->palVersion = 0x300;
	//	pPal->palNumEntries = n;
	//	for (i=0; i<n; i++)
	//	{
	//		pPal->palPalEntry[i].peRed =
	//				ComponentFromIndex(i, pfd.cRedBits, pfd.cRedShift);
	//		pPal->palPalEntry[i].peGreen =
	//				ComponentFromIndex(i, pfd.cGreenBits, pfd.cGreenShift);
	//		pPal->palPalEntry[i].peBlue =
	//				ComponentFromIndex(i, pfd.cBlueBits, pfd.cBlueShift);
	//		pPal->palPalEntry[i].peFlags = 0;
	//	}

	//	/* fix up the palette to include the default GDI palette */
	//	if ((pfd.cColorBits == 8)                           &&
	//		(pfd.cRedBits   == 3) && (pfd.cRedShift   == 0) &&
	//		(pfd.cGreenBits == 3) && (pfd.cGreenShift == 3) &&
	//		(pfd.cBlueBits  == 2) && (pfd.cBlueShift  == 6)
	//	   )
	//	{
	//		for (i = 1 ; i <= 12 ; i++)
	//			pPal->palPalEntry[defaultOverride[i]] = defaultPalEntry[i];
	//	}

	//	m_cPalette.CreatePalette(pPal);
	//	delete [] pPal;

	//	m_pOldPalette = m_pDC->SelectPalette(&m_cPalette, FALSE);
	//	m_pDC->RealizePalette();
	//}
}


BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	Init(); // initialize OpenGL
	double min[3] = {-1., -1., -1.};
	double max[3] = {1., 1., 1.};
	Ncm.setStock(min, max);
	char * contour = {"G1Z0R0.2G1Z2R0.4"};
	NCM_API::TOOLID Tool = Ncm.newCustomMillingTool(contour);
	double Bp[3] = {0., 0., 0.};
	double Bn[3] = {0., 0., 1.};
	double Ep[3] = {0.5, 0., 0.};
	double En[3] = {0., 0., 1.};
	double Ca[3] = {0., 0., 1.};
	Ncm.removeToolTrace(Tool, Bp, Bn, Ep, En, Ca, false, false, false);
	return 0;
}


void CChildView::OnDestroy()
{
	HGLRC   hrc;

	hrc = ::wglGetCurrentContext();

	::wglMakeCurrent(NULL,  NULL);

	if (hrc)
		::wglDeleteContext(hrc);


	if (m_pDC)
		delete m_pDC;

	CWnd::OnDestroy();

}


void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if(cy > 0)
	{
		glViewport(0, 0, cx, cy);

		if((m_oldRect.right > cx) || (m_oldRect.bottom > cy))
			RedrawWindow();

		m_oldRect.right = cx;
		m_oldRect.bottom = cy;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0f, (GLdouble)cx/cy, 3.0f, 7.0f);
		glMatrixMode(GL_MODELVIEW);
	}
}
