// NRotTracker.cpp: implementation of the NRotTracker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResInv.h"
#include "BOpenGLWndVD.h"
#include "BaseRender.h"
#include "NTiParams.h"
#include "NCMProject.h"
#include "math.h"
#include "NRotTracker.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NRotTracker::NRotTracker(BOpenGLWnd *pV, char axes, const BPoint &iRotBase, WORD LeftButton)
	: New0(0., 0., 0., 1.), New1(0., 0., 0., 1.)
{
	// 
	WorkMouseButton = LeftButton;
	RotBase = iRotBase;

	m_sizeMin = 1;
	m_nHandleSize = 1;
	PictureChanged = true;
	pView = pV;
	NewOne = TRUE;
	Axes = axes;
	if(Axes == 'W')
		PictureChanged = false;

	auto pViewVD = dynamic_cast<BOpenGLWndVD*>(pView);
	if (pViewVD != nullptr)
		pViewVD->SetCursorID(Axes);
}

NRotTracker::~NRotTracker()
{
	auto pViewVD = dynamic_cast<BOpenGLWndVD*>(pView);
	if (pViewVD != nullptr)
		pViewVD->SetCursorID('0');
}

void NRotTracker::OnChangedRect(const CRect &rectOld)
{
	// Clear sc
	if (Axes == 'W')
	{
		CRectTracker::OnChangedRect(rectOld);
		return;
	}
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (pView != nullptr)
		pView->MakeCurrent();
	if (NewOne)
	{
		NewOne = FALSE;

		if (RotBase.IsPoint())
		{// get base point from RotBase instead of picked point
			double h;
			RotBase.Get(bx, by, bz, h);
		}
		else
		{
			//	Get obj coord of picked point (bx, by, bz)
			double x = 0., y = 0., z = 0.;
			CPoint point(m_rect.left, m_rect.top);
			double h = 1.;
			BPoint High;
			pView->GetHighPoint(High);
			if (High.IsPoint())
			{
				High.Get(x, y, z, h);
			}
			else
			{
				double wz;
				pView->GetMouseCoordGL(point, &wz, &x, &y, &z);
				if (wz == 0.)
				{// get base point from RotBase instead of picked point
					RotBase.Get(x, y, z, h);
				}
			}
			BPoint Buf(x, y, z, 1.);
			pView->RecalcStockPointBack(Buf);
			Buf.Get(bx, by, bz, h);
		}
		if (Axes == 'P')
		{
			GLint viewport[4];
			GLdouble projMatrix[16], modelMatrix[16];
			glMatrixMode(GL_MODELVIEW);
			glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
			glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
			glGetIntegerv(GL_VIEWPORT, viewport);
			double x, y, z;
			gluUnProject(0., 0., 0., modelMatrix, projMatrix, viewport,
				&x, &y, &z);
			//	Get obj coord of x axes
			gluUnProject(1., 0., 0., modelMatrix, projMatrix, viewport,
				&xX, &yX, &zX);
			xX -= x; yX -= y; zX -= z;
			//		Get obj coord of y axes
			gluUnProject(0., 1., 0., modelMatrix, projMatrix, viewport,
				&xY, &yY, &zY);
			xY -= x; yY -= y; zY -= z;
		}

		if (pView != nullptr)
			pView->RestoreContext();
	}
	else
	{
		int dx = m_rect.right - rectOld.right;
		int dy = m_rect.bottom - rectOld.bottom;
		GLdouble angle;
		//Calculate angle for X,Y,Z rotation
		if (abs(dx) > abs(dy))
			angle = (GLdouble)dx * 0.7;
		else
			angle = (GLdouble)dy * 0.7;
		// Translate to 0,0,0
		glTranslated(bx, by, bz);
		double scale = 1.;
		switch (Axes)
		{
		case 'A':
		{
			GLint viewport[4];
			GLdouble projMatrix[16], modelMatrix[16];
			glMatrixMode(GL_MODELVIEW);
			glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
			glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
			glGetIntegerv(GL_VIEWPORT, viewport);
			double x, y, z;
			gluUnProject(0., 0., 0., modelMatrix, projMatrix, viewport,
				&x, &y, &z);
			//	Get obj coord of x axes
			gluUnProject(1., 0., 0., modelMatrix, projMatrix, viewport,
				&xX, &yX, &zX);
			xX -= x; yX -= y; zX -= z;
			//		Get obj coord of y axes
			gluUnProject(0., 1., 0., modelMatrix, projMatrix, viewport,
				&xY, &yY, &zY);
			xY -= x; yY -= y; zY -= z;
			// Rotate around x-axes
			glRotated((GLdouble)dy * 0.7, xX, yX, zX);
			// Rotate around y-axes
			glRotated((GLdouble)dx * 0.7, xY, yY, zY);
			break;
		}
		case 'X'://Rotate around X-Axes in object coord
			glRotated(angle, 1., 0., 0.);
			break;
		case 'Y'://Rotate around Y-Axes in object coord
			glRotated(angle, 0., 1., 0.);
			break;
		case 'Z'://Rotate around Z-Axes in object coord
			glRotated(angle, 0., 0., 1.);
			break;
		case 'S'://Scale
			if (dy < 0.)
				scale -= dy * 0.006;
			if (dy > 0.)
				scale /= 1. + dy * 0.006;
			if (scale <= 0.)
				scale = 1.;
			glScaled(scale, scale, scale);
			break;
		case 'P'://Pan
			glTranslated(xX * dx - xY * dy,
				yX * dx - yY * dy,
				zX * dx - zY * dy);
			break;
		default:
			break;
		}

		// Translate to original position
		glTranslated(-bx, -by, -bz);
		if (pView != nullptr)
		{
			pView->RestoreContext();
			pView->m_ChangeView = 1;
			pView->Invalidate();
		}
	}
}

void NRotTracker::DrawRect()
{
	if (PictureChanged)
	{
		glAccum(GL_LOAD, 1.0);
		PictureChanged = false;
	}
	glPushMatrix();
	glLoadIdentity();
	glPushAttrib(GL_ENABLE_BIT | GL_POLYGON_BIT | GL_COLOR_BUFFER_BIT | GL_LINE_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	Shader::ShaderOFF();
	glEnable(GL_BLEND);
	glDisable(GL_POLYGON_SMOOTH);
	glDisable(GL_LINE_SMOOTH);
	glLineWidth(1.);
	glColor4d(0.5, 0.5, 0.5, 0.5);
	glAccum(GL_RETURN, 1.0);
	glRectdv(New0.GetArray(), New1.GetArray());
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_BLEND);
	glColor4d(0., 0., 0., 1.);
	glRectdv(New0.GetArray(), New1.GetArray());
	glPopAttrib();
	glPopMatrix();

}
void NRotTracker::DrawTrackerRect(LPCRECT lpRect, CWnd *pWndClipTo, CDC *pDC, CWnd *pWnd)
{
	// Do not draw 
	if (Axes == 'W')
	{
		((BOpenGLWnd *)pWnd)->MakeCurrent();
		CRect Rect(*lpRect);
		glPushMatrix();
		glLoadIdentity();
		double wz, x, y, z;
		BOpenGLWnd::GetCPointCoord(Rect.TopLeft(), &wz, &x, &y, &z);
		New0.Set(x, y, 0., 1.);
		BOpenGLWnd::GetCPointCoord(Rect.BottomRight(), &wz, &x, &y, &z);
		New1.Set(x, y, 0., 1.);

		DrawRect();

		glPopMatrix();

		SwapBuffers(wglGetCurrentDC());
		((BOpenGLWnd *)pWnd)->RestoreContext();


	}
	//	if(Axes == 'W')
	//		CRectTracker::DrawTrackerRect(lpRect,pWndClipTo,pDC,pWnd);
}

BOOL NRotTracker::TrackRubberBand(CWnd* pWnd, CPoint point, BOOL bAllowInvert)
{
	// simply call helper function to track from bottom right handle
	m_bAllowInvert = bAllowInvert;
	m_rect.SetRect(point.x, point.y, point.x, point.y);
	return TrackHandle(hitBottomRight, pWnd, point, NULL);
}

BOOL NRotTracker::TrackHandle(int nHandle, CWnd* pWnd, CPoint point,
	CWnd* pWndClipTo)
{
	ASSERT(nHandle >= 0);
	ASSERT(nHandle <= 8);   // handle 8 is inside the rect

							// don't handle if capture already set
	if (::GetCapture() != NULL)
		return FALSE;

	AfxLockTempMaps();  // protect maps while looping

	ASSERT(!m_bFinalErase);

	// save original width & height in pixels
	int nWidth = m_rect.Width();
	int nHeight = m_rect.Height();

	// set capture to the window which received this message
	pWnd->SetCapture();
	ASSERT(pWnd == CWnd::GetCapture());
	pWnd->Invalidate();
	pWnd->UpdateWindow();
	if (pWndClipTo != NULL)
		pWndClipTo->UpdateWindow();
	CRect rectSave = m_rect;

	// find out what x/y coords we are supposed to modify
	int *px, *py;
	int xDiff, yDiff;
	GetModifyPointers(nHandle, &px, &py, &xDiff, &yDiff);
	xDiff = point.x - xDiff;
	yDiff = point.y - yDiff;

	// get DC for drawing
	CDC* pDrawDC;
	if (pWndClipTo != NULL)
	{
		// clip to arbitrary window by using adjusted Window DC
		pDrawDC = pWndClipTo->GetDCEx(NULL, DCX_CACHE);
	}
	else
	{
		// otherwise, just use normal DC
		pDrawDC = pWnd->GetDC();
	}
	ENSURE_VALID(pDrawDC);

	CRect rectOld;
	BOOL bMoved = FALSE;

	// get messages until capture lost or cancelled/accepted
	for (;;)
	{
		MSG msg;
		//		VERIFY(::GetMessage(&msg, NULL, 0, 0));
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if (CWnd::GetCapture() != pWnd)
			break;

		if (msg.message == WorkMouseButton || msg.message == WM_MOUSEMOVE)
		{

			rectOld = m_rect;
			// handle resize cases (and part of move)
			if (px != NULL)
				*px = GET_X_LPARAM(msg.lParam) - xDiff;
			if (py != NULL)
				*py = GET_Y_LPARAM(msg.lParam) - yDiff;

			// handle move case
			if (nHandle == hitMiddle)
			{
				m_rect.right = m_rect.left + nWidth;
				m_rect.bottom = m_rect.top + nHeight;
			}
			// allow caller to adjust the rectangle if necessary
			AdjustRect(nHandle, &m_rect);

			// only redraw and callback if the rect actually changed!
			m_bFinalErase = (msg.message == WorkMouseButton);
			if (!rectOld.EqualRect(&m_rect) || m_bFinalErase)
			{
				if (bMoved)
				{
					m_bErase = TRUE;
					DrawTrackerRect(&rectOld, pWndClipTo, pDrawDC, pWnd);
				}
				OnChangedRect(rectOld);
				if (msg.message != WorkMouseButton)
					bMoved = TRUE;
			}
			if (m_bFinalErase)
				goto ExitLoop;

			if (!rectOld.EqualRect(&m_rect))
			{
				m_bErase = FALSE;
				DrawTrackerRect(&m_rect, pWndClipTo, pDrawDC, pWnd);
			}
		}
		else if( msg.message == WM_KEYDOWN)
		{
			if (msg.wParam == VK_ESCAPE)
			{

				if (bMoved)
				{
					m_bErase = m_bFinalErase = TRUE;
					DrawTrackerRect(&m_rect, pWndClipTo, pDrawDC, pWnd);
				}
				m_rect = rectSave;
				goto ExitLoop;
			}
		}
		else
		{
			DispatchMessage(&msg);
		}
		//switch (msg.message)
		//{
		//	// handle movement/accept messages
		//case WorkMouseButton:
		//case WM_MOUSEMOVE:
		//	rectOld = m_rect;
		//	// handle resize cases (and part of move)
		//	if (px != NULL)
		//		*px = GET_X_LPARAM(msg.lParam) - xDiff;
		//	if (py != NULL)
		//		*py = GET_Y_LPARAM(msg.lParam) - yDiff;

		//	// handle move case
		//	if (nHandle == hitMiddle)
		//	{
		//		m_rect.right = m_rect.left + nWidth;
		//		m_rect.bottom = m_rect.top + nHeight;
		//	}
		//	// allow caller to adjust the rectangle if necessary
		//	AdjustRect(nHandle, &m_rect);

		//	// only redraw and callback if the rect actually changed!
		//	m_bFinalErase = (msg.message == WorkMouseButton);
		//	if (!rectOld.EqualRect(&m_rect) || m_bFinalErase)
		//	{
		//		if (bMoved)
		//		{
		//			m_bErase = TRUE;
		//			DrawTrackerRect(&rectOld, pWndClipTo, pDrawDC, pWnd);
		//		}
		//		OnChangedRect(rectOld);
		//		if (msg.message != WorkMouseButton)
		//			bMoved = TRUE;
		//	}
		//	if (m_bFinalErase)
		//		goto ExitLoop;

		//	if (!rectOld.EqualRect(&m_rect))
		//	{
		//		m_bErase = FALSE;
		//		DrawTrackerRect(&m_rect, pWndClipTo, pDrawDC, pWnd);
		//	}
		//	break;

		//	// handle cancel messages
		//case WM_KEYDOWN:
		//	if (msg.wParam != VK_ESCAPE)
		//		break;
		//case WM_MBUTTONDOWN:
		//	if (bMoved)
		//	{
		//		m_bErase = m_bFinalErase = TRUE;
		//		DrawTrackerRect(&m_rect, pWndClipTo, pDrawDC, pWnd);
		//	}
		//	m_rect = rectSave;
		//	goto ExitLoop;

		//	// just dispatch rest of the messages
		//default:
		//	DispatchMessage(&msg);
		//	break;
		//}
	}

ExitLoop:
	if (pWndClipTo != NULL)
		pWndClipTo->ReleaseDC(pDrawDC);
	else
		pWnd->ReleaseDC(pDrawDC);
	ReleaseCapture();

	AfxUnlockTempMaps(FALSE);

	// restore rect in case bMoved is still FALSE
	if (!bMoved)
		m_rect = rectSave;
	m_bFinalErase = FALSE;
	m_bErase = FALSE;

	// return TRUE only if rect has changed
	return !rectSave.EqualRect(&m_rect);
}


