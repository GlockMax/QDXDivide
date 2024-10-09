#include "StdAfx.h"
#include "NCMHDoc.h"
#include "RDynViewChange.h"
#include "NCMHWnd.h"

void NCMHWnd::NUpdate(const CRect* ClipRect)
{
	if (!IsWindowVisible())
		return;
	BOpenGLWnd::NUpdate();

	CNCMHDoc* pDoc = (CNCMHDoc*)GetDocument();
	if (!pDoc)
		return;

//	pDoc->Gabar.Init();
	// set default param
//	MakeCurrent();
	SetDefaultOGL();

	glMatrixMode(GL_MODELVIEW);
	BMatr M1(ViewMatrix[0]);
	if (DynViewChange && m_ChangeView > 1)
	{
		RDynViewChange DVChange(BufMatr, M1);
		BMatr M;
		for (int DynInd = 0; DynInd < 1000 && DVChange.GetNextMatr(M); ++DynInd)
		{
			glLoadMatrixd(M.GetArray());
			SetZRange();
			DrawScene(RM_UNDEF);

			BSwapBuffers();
		}
	}
	m_ChangeView = 0;

	glLoadMatrixd(M1.GetArray());
	SetZRange();
	DrawScene(RM_UNDEF);

	BSwapBuffers();

//	RestoreContext();
}

void NCMHWnd::SetZRange()
{
	// Set z range according to current scene gabar if defined

	BBox ScreenDim;
	BBox ObjectDim = GetSceneGabar(false);
	if (!ObjectDim.IsDefined())
		return;
	ObjectDim.ApplyLimits();
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	if (viewport[2] <= 0 || viewport[3] <= 0)
		return;
	GLdouble modelMatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	ScreenDim.Project(ObjectDim, BMatr(modelMatrix));
	double MidZ = 0.5 * (ScreenDim.Zmin + ScreenDim.Zmax);
	double dZ = max(100., ScreenDim.Zmax - ScreenDim.Zmin);// 100. limit is to prevent UCS symbols from z-clipping

	glLoadIdentity();
	glTranslated(0., 0., -MidZ);
	glMultMatrixd(modelMatrix);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-(double)viewport[2], (double)viewport[2],
		-(double)viewport[3], (double)viewport[3],
		-3. * dZ, 3. * dZ);

	glMatrixMode(GL_MODELVIEW);
}

BBox NCMHWnd::GetSceneGabar(bool VisibleOnly)
{
	CNCMHDoc* pDoc = (CNCMHDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	BBox Res;

	auto* list = &pDoc->GetStocksList();
	for (int i = 0; i <list->GetSize(); ++i)
	{
		NStock* pS = list->GetStock(i);
		if (pS != nullptr)
		{
			if (!(VisibleOnly && !pS->Visible))
				Res.Expand(pS->GetGabar());
		}
	}
	Res.Expand(pDoc->NGetExObjList()->GetGabar());
	auto pEl = pDoc->GetpElems();
	for (int i = 0; i < pDoc->GetElemsSize(); ++i)
		Res.Expand(pEl[i].x, pEl[i].y, pEl[i].z);
	if (!Res.IsDefined())
	{
		Res.Expand(100., 100., 20.);
		Res.Expand(0., 0., 0.);
	}
	else
		Res.Offset(1., 1., 1.);
	return Res;
}

void NCMHWnd::GetMouseCoord(const CPoint& point, double* wz, double* x, double* y, double* z)
{
	//		Get obj coord of point
	GLint viewport[4];
	GLdouble projMatrix[16], modelMatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	glGetIntegerv(GL_VIEWPORT, viewport);
	GLfloat sz;
	int px = min(point.x, viewport[2] - 1);
	int py = min(point.y, viewport[3] - 1);
	px = max(1, px);
	py = max(1, py);
	glReadPixels(px, viewport[3] - py, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &sz);
	if (sz == 1.)
		sz = 0.f;
	gluUnProject(px + 0.5, viewport[3] - py + 0.5, sz, modelMatrix, projMatrix, viewport,
		x, y, z);
	*wz = sz;
}

void NCMHWnd::DrawScene(RenderMode GlobRMode)
{
	CNCMHDoc* pDoc = (CNCMHDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	DynViewChange = true;
	if (pDoc->IsJustLoaded())
	{
		DoDefView(1., 1.5, 2., 0., 0., 1.);
		pDoc->ResetJustLoaded();
		DynViewChange = false;
	}
	ClearBackground();

	DrawRotCenter();

	// Draw Stock
	pDoc->GetStocksList().DrawStocksOld(GlobRMode, 7, FALSE, true);
	// Draw Fixture
	pDoc->NGetExObjList()->Draw(GlobRMode, true);
	//Draw trajectory sample
	RLines::LineVertex* pElems = pDoc->GetpElems();
	if (pElems != nullptr)
	{
		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(3, GL_UNSIGNED_BYTE, sizeof(RLines::LineVertex), ((char*)pElems) + sizeof(GLfloat) * 3);
		glVertexPointer(3, GL_FLOAT, sizeof(RLines::LineVertex), ((char*)pElems));
		glShadeModel(GL_FLAT);
		glDrawArrays(GL_LINE_STRIP, 0, pDoc->GetElemsSize());
		glPopClientAttrib();
		glPopAttrib();
	}
	BOpenGLWnd::DrawScene(GlobRMode);
}
BEGIN_MESSAGE_MAP(NCMHWnd, BOpenGLWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()


CDocument* NCMHWnd::GetDocument()
{
	return static_cast<CView*>(GetParent())->GetDocument();
}

void NCMHWnd::ClearBackground(void)
{
	glClearColor(0.7f, 0.7f, 0.7f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void NCMHWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if (!IsWindowVisible())
		return;
	CRect rect;
	rect = dc.m_ps.rcPaint;
	if (rect.Width() == 0 || rect.Height() == 0)
		return;

	// Set current context
//	MakeCurrent();

	NUpdate();

//	RestoreContext();
}

