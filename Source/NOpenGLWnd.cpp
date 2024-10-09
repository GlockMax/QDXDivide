// NOpenGLWnd.cpp : implementation of the NOpenGLWnd class
//

#include "stdafx.h"
#include "ResInv.h"
#include "GLee.h"
#include "Gl\Glu.h"
#include <ppltasks.h>
#include "NCM.h"
#include "NCMProject.h"
#include "ConstDef.h"
#include "NCMDoc.h"
#include "NStockSection.h"
#include "NCUnit.h"
#include "ResLocal.h"
#include "math.h"
#include "MainFrm.h"
#include "LeftView.h"
#include "NTranspGeomDlg.h"
#include "T3DData.h"
#include "NProgramWeld.h"
#include "RDynViewChange.h"
#include "NRemoteManager.h"
#include "NOpenGLWnd.h"
#include "RRuler.h"
#include "DimObject.h"
#include "DimObjectPoint.h"
#include "DimObjectSegment.h"
#include "DimObjectArc.h"
#include "DimObjectCircle.h"
#include "DimObjectPlane.h"
#include "DimObjectDimensionLinear.h"
#include "DimObjectDimensionArc.h"
#include "DimObjectDimensionCircle.h"
#include "DimObjectDimensionAngle.h"
#include "BProgVisInterval.h"
#include "RayCastManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CNCMApp theApp;
NViewState NOpenGLWnd::NState;

const int BUFSIZE = 512;
/////////////////////////////////////////////////////////////////////////////
// NOpenGLWnd

IMPLEMENT_DYNCREATE(NOpenGLWnd, BOpenGLWndVD)

BEGIN_MESSAGE_MAP(NOpenGLWnd, BOpenGLWndVD)
	//{{AFX_MSG_MAP(NOpenGLWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_FILE_EXPORT, OnFileExport)
	ON_WM_LBUTTONDBLCLK()
	ON_UPDATE_COMMAND_UI(IDS_INDIC_REL_XYZ, OnUpdateCoordRel)
	ON_COMMAND(IDS_INDIC_REL_XYZ, OnStatusCoordRel)
	//}}AFX_MSG_MAP

    ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)

ON_COMMAND(ID_VIEW_TR_GEOM, OnViewTrGeom)
ON_WM_SETCURSOR()
ON_WM_ACTIVATE()
ON_WM_SETFOCUS()
ON_WM_KILLFOCUS()
ON_WM_PAINT()
ON_WM_LBUTTONUP()
ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()

CString NOpenGLWnd::ViewName;
/////////////////////////////////////////////////////////////////////////////
// NOpenGLWnd construction/destruction

NOpenGLWnd::NOpenGLWnd()
{
	pTrGeomDlg = NULL;
	IsLastRendTranslucent = false;

	StepsCount = 0;
	HighlightedCadrStPos = 0;
	InitCursors();
}

NOpenGLWnd::~NOpenGLWnd()
{
	if(pTrGeomDlg)
	{
		delete pTrGeomDlg;
		pTrGeomDlg = NULL;
	}
}

BOOL NOpenGLWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	return CWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// NOpenGLWnd drawing
bool NOpenGLWnd::CanRedraw(void) const
{
	return (! m_wndToolBar.IsWindowVisible());
}
void NOpenGLWnd::OnDraw(CDC* pDC)
{
	if(!IsWindowVisible())
		return;
	CRect rect;
	rect = ((CPaintDC*)pDC)->m_ps.rcPaint;
	if(rect.Width() == 0 || rect.Height() == 0)
		return;

	IsOpenGLErr("OnDraw0");

	if(! CanRedraw() )//Do not redraw while TB is active
		return;

	// Set current context
	MakeCurrent();
	
	IsOpenGLErr("OnDraw1");
	NUpdate();

	RestoreContext();
}

void NOpenGLWnd::InitialUpdate()
{
	BOpenGLWndVD::InitialUpdate();
	// Set view point defined in JOB file
	if(!ViewName.IsEmpty())
		SetViewPoint(ViewName);
}

/////////////////////////////////////////////////////////////////////////////
// NOpenGLWnd diagnostics

#ifdef _DEBUG


#endif //_DEBUG

void NOpenGLWnd::OnSize(UINT nType, int cx, int cy)
{
	CDocument* pDoc = GetDocument();
	if(pDoc)
		pDoc->SetModifiedFlag();
	BOpenGLWndVD::OnSize(nType,cx,cy);
}



/////////////////////////////////////////////////////////////////////////////
// NOpenGLWnd message handlers


void NOpenGLWnd::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint) 
{
	if (lHint & N_INFOTIP)
	{
		InfoTip.EnsureCreated(this);
		CNCMDoc* pDoc = (CNCMDoc*)GetDocument();
		if (pDoc != nullptr)
			InfoTip.Activate(pDoc->Comparison.IsActive() ? TRUE : FALSE);
	}
	if(lHint & N_OGL)
	{
		Invalidate();
	}
	if(lHint & N_NEWDOC)
	{
		RestoreView(0); // To be restored in v 3.8

		IsOpenGLErr("OnUpdate");
		lHint |= N_SETTINGS;
	}
	if(lHint & N_SETTINGS)
	{
		MakeCurrent();
		NColor *pCol;
		if(NCM_PROJECT.Colors.Lookup("’ол’од",pCol))
			NCadrGeom::FastMovCol = *pCol;
		else
			NCadrGeom::FastMovCol = NColor(0.f,0.f,1.f,1.f);
		if(NCM_PROJECT.Colors.Lookup("ѕрив€зка",pCol))
			SnapCol = *pCol;
		else
			SnapCol = NColor(0.f,1.f,0.f,1.f);

		double *pDouble,*pD1,*pD2,*pD3;
		if(NCM_PROJECT.DoubleParams.Lookup("SelectionRegion",pDouble))
			SelectionRegionSize = (GLfloat)*pDouble;
		else
			SelectionRegionSize = 5;
//////////
		CNCMDoc* pDoc = (CNCMDoc*)GetDocument();
		if (pDoc != nullptr)
		{
			for (int ChInd = 0; ChInd < pDoc->GetChannelsCount(); ++ChInd)
			{
				auto pChannel = pDoc->GetChannel(ChInd);

				if (NCM_PROJECT.DoubleParams.Lookup("Axes1Size", pD1) &&
					NCM_PROJECT.DoubleParams.Lookup("Axes2Size", pD2) &&
					NCM_PROJECT.DoubleParams.Lookup("Axes3Size", pD3))
				{
					pChannel->GetAxes().SetSizes(*pD1, *pD2, *pD3, *pD1, 0.);
				}
				else
				{
					pChannel->GetAxes().SetSizes(5., 5., 5., 5., 0.);
				}
			}
		}
//////////
		if(NCM_PROJECT.DoubleParams.Lookup("MagnFactor",pDouble))
			MagnFactor = *pDouble;
		else
			MagnFactor = 2;
//////////
		NColor *c;
		if(NCM_PROJECT.Colors.Lookup("‘он√рафическогоќкна", c))
		{BackCol[0]=c->R();BackCol[1]=c->G();BackCol[2]=c->B();}
		else // color was not found
		{BackCol[0]=0.4f;BackCol[1]=0.4f;BackCol[2]=0.4f;}

		if(NCM_PROJECT.Colors.Lookup("‘он√рафическогоќкнаЋев¬ерх",c))
		{BackColLT[0]=c->R();BackColLT[1]=c->G();BackColLT[2]=c->B();}
		else // color was not found
		{BackColLT[0]=0.4f;BackColLT[1]=0.4f;BackColLT[2]=0.4f;}
		if(NCM_PROJECT.Colors.Lookup("‘он√рафическогоќкнаЋевЌижн",c))
		{BackColLB[0]=c->R();BackColLB[1]=c->G();BackColLB[2]=c->B();}
		else // color was not found
		{BackColLB[0]=0.6f;BackColLB[1]=0.6f;BackColLB[2]=0.6f;}
		if(NCM_PROJECT.Colors.Lookup("‘он√рафическогоќкнаѕрав¬ерх",c))
		{BackColRT[0]=c->R();BackColRT[1]=c->G();BackColRT[2]=c->B();}
		else // color was not found
		{BackColRT[0]=0.4f;BackColRT[1]=0.4f;BackColRT[2]=0.4f;}
		if(NCM_PROJECT.Colors.Lookup("‘он√рафическогоќкнаѕравЌижн",c))
		{BackColRB[0]=c->R();BackColRB[1]=c->G();BackColRB[2]=c->B();}
		else // color was not found
		{BackColRB[0]=0.6f;BackColRB[1]=0.6f;BackColRB[2]=0.6f;}

		if(NCM_PROJECT.Colors.Lookup("‘он√рафическогоќкнаѕравЌижн",c))
		{BackColRB[0]=c->R();BackColRB[1]=c->G();BackColRB[2]=c->B();}
		else // color was not found
		{BackColRB[0]=0.6f;BackColRB[1]=0.6f;BackColRB[2]=0.6f;}
		int *pInt;
		if(NCM_PROJECT.IntParams.Lookup("GradientBack", pInt))
			GradientEnable = (*pInt == 1);

		//////////		
		glClearDepth(1.);
		ClearBackground();
		RestoreContext();

		Invalidate();
	}
}


void NOpenGLWnd::DrawScene(RenderMode GlobRMode)
{
}

void NOpenGLWnd::DrawDefault()
{
	// draw scene with default mode
	ClearBackground();
	DrawScene(RM_UNDEF);

	Shader::ShaderOFF();
	BOpenGLWndVD::DrawScene(RM_UNDEF);
	RRuler::Draw(IsActive(), MainText);
}
void NOpenGLWnd::DrawWithTranslucent()
{
	bool progComplete = BaseRender::TranslucentPr.isProgramComplete();

	if(!progComplete)
	{
		// if shaders fail for some reason
		DrawDefault();
		return;
	}

	CNCMDoc *pDoc = (CNCMDoc*)GetDocument();
	IsLastRendTranslucent = (pDoc->IsThereAnyTransluc() == TRUE);
	if(!IsLastRendTranslucent)
	{
		DrawDefault();
		return;
	}
// Translucent rendering
	if (!BaseRender::TranslucentPr.prepareI() )
	{
		DrawDefault();
		return;
	}

	ClearBackground();
	// Disable section
	NStockSection *pSection = pDoc->GetStockSection();
	bool SectionActive = false;
	if (pSection)
	{
		SectionActive = pSection->GetActive();
		pSection->SetActive(false);
	}
	// END:Disable section

	DrawScene(RM_SHADE);

	if (!BaseRender::TranslucentPr.prepareII(RTranslucentProg::PUT_BEFORE) )
	{
		DrawDefault();
		return;
	}
	DrawScene(RM_TRANSLUCENT);
	if ( !BaseRender::TranslucentPr.prepareII(RTranslucentProg::PUT_AFTER) )
	{
		DrawDefault();
		return;
	}

	int currId = 0;
	bool useOQ = BaseRender::TranslucentPr.getOQFlag();

	for(int pass = 1; useOQ && pass < 25; pass++)
	{
		currId = pass % 2;
		int prevId = 1 - currId;

		bool samplesRemained = false;

		if(!BaseRender::TranslucentPr.prepareIII(RTranslucentProg::PUT_BEFORE, currId, prevId, &samplesRemained))
		{
			DrawDefault();
			return;
		}
		DrawScene(RM_TRANSLUCENT);
		if(!BaseRender::TranslucentPr.prepareIII(RTranslucentProg::PUT_AFTER, currId, prevId, &samplesRemained))
		{
			DrawDefault();
			return;
		}

		// if there are already no samples to process in a scene
		if(!samplesRemained )
		{
			break;
		}
	}

	if (!BaseRender::TranslucentPr.prepareIV(currId, -1) )
	{
		DrawDefault();
		return;
	}

	Shader::ShaderOFF();
	BOpenGLWndVD::DrawScene(RM_UNDEF);
	RRuler::Draw(IsActive(), MainText);
	// Restore section
	if (pSection)
	{
		pSection->SetActive(SectionActive);
	}
	// END:Restore section
}

bool NOpenGLWnd::IsVisiblePoint(const BPoint& iPoint, double iKoef)
{
	//iPoint is in view CS

	MakeCurrent();

	BPoint viewDir = NOpenGLWnd::GetViewDir();
	GLint viewport[4];
	GLdouble projMatrix[16],modelMatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	glGetIntegerv(GL_VIEWPORT, viewport);
    // ???иногда метод говорит, что точка невидима, хоть это не так
	double xs, ys, zs;
	gluProject(iPoint.GetX(), iPoint.GetY(), iPoint.GetZ(), modelMatrix, projMatrix, viewport, &xs, &ys, &zs);

	GLfloat zMin;
	glReadPixels((int)xs, (int)ys, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &zMin);

	double x, y, z;
	gluUnProject((int)xs + 0.5, (int)ys + 0.5, (GLdouble)zMin, modelMatrix, projMatrix, viewport, &x, &y, &z);

	BPoint tmpPt(x, y, z);

	double px, py, pz;
	BaseRender::GetPixelDim(px, py, pz);

	double cos = (tmpPt - iPoint) * viewDir;

	RestoreContext();

	return cos > -px * 5. * iKoef;
}

bool NOpenGLWnd::Divide1Stock(const CPoint& point, const BMatrPair & StockMatrP)
{
	CNCMDoc *pDoc = (CNCMDoc *)GetDocument();

	if (pDoc->pDivide != nullptr)
	{
		if (ZoomRActive ||
			ZoomWActive ||
			GetKeyState(VK_CONTROL) >> 1 ||
			GetKeyState(VK_SHIFT) >> 1 ||
			GetKeyState(VK_MENU) >> 1) // >>1 is used to ignore the low-order bit 
		{
			return true;
		}
		if (!pDoc->pDivide->IsModeSelected())
			return false;
		MakeCurrent();
		BPoint View(GetViewDir());
		double wz, x, y, z;
		GetMouseCoord(point, &wz, &x, &y, &z);
		if (wz == 0.0)// Point is outside the picture
		{
		}
		else
		{
			BPoint P(x, y, z, 1.);
			bool PointOnSection = false;
			bool PointNerarerSection = false;
			NStockSection *pSection = pDoc->GetStockSection();
			P = P - View * 1000.;

			BMatr CorrMatr;
			if (NCMComData::IsTurnEnabled() && NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::BSP)
				CorrMatr.RotX(0., 0., 0., 90.);
			else
				CorrMatr.SetE();
			pDoc->pDivide->SetCorrMatr(CorrMatr);

			// Ћуч упал здесь	
			pDoc->pDivide->SetPoint(P * StockMatrP.Inv(), View * StockMatrP.Inv());
			pDoc->RegenerateStocks();
			pDoc->pDivide->Recreate();
		}
		RestoreContext();
		pDoc->UpdateAllViews(NULL, N_OGL);
	}

	return true;
}

void NOpenGLWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	MakeCurrent();

	CNCMDoc* pDoc = (CNCMDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	NState.SetBase(pDoc->StatusX, pDoc->StatusY, pDoc->StatusZ);
	if (!IsViewInterfaceActive())
	{

		NStockSection *pSection = pDoc->GetStockSection();
		if (pSection)
			if (pSection->IsWaitingPoint())
			{
				double x, y, z, wz;
				GetMouseStockCoord(point, &wz, &x, &y, &z);
				pSection->SetPoint(BPoint(x, y, z, 1.));
				return;
			}

		NRemoteManager *pRC = ((CMainFrame *)AfxGetMainWnd())->GetRemoteControl();
		if (pRC)
			if (pRC->IsWaitingPoint())
			{
				GetParent()->SetFocus();
				BPoint P, V;
				BPoint R, N;
				GetMouseRay(point, P, V);
				RecalcStockPoint(P);
				RecalcStockPoint(V);
				RPickID PickID;
				if (pDoc->GetPartsPoint(P, V, R, N, PickID) < 0)
				{
					double x, y, z, wz;
					GetMouseStockCoord(point, &wz, &x, &y, &z);
					R.Set(x, y, z, 1.);
					N.Set(0., 0., 1., 0.);
				}

				pRC->SetPickedRay(R, N);
				((CMainFrame *)AfxGetMainWnd())->RC_SendPoint(R, N);
				pDoc->UpdateAllViews(NULL, N_OGL);
				return;
			}

		pDoc->GotoStatusCadr();
	}
	BOpenGLWndVD::OnLButtonDown(nFlags, point);
}

void NOpenGLWnd::NUpdate(const CRect *ClipRect)
{
	if(!IsWindowVisible())
		return;
	BOpenGLWndVD::NUpdate();

//	IsOpenGLErr("NUpdate0");
	CNCMDoc* pDoc = (CNCMDoc*)GetDocument();
	if(!pDoc)
		return;
	if (pDoc->IsUpdateBlocked())
	{
		if (((CMainFrame*)AfxGetMainWnd())->IsAnyTimerActive())
			return;
		pDoc->SetBlockUpdateLevel(0);
	}
	
	pDoc->Gabar.Init();
// set default param
	MakeCurrent();
	SetDefaultOGL();

	DynViewChange = true;
	glMatrixMode(GL_MODELVIEW);
	BMatr M1(ViewMatrix[0]);
	if(DynViewChange && m_ChangeView > 1)
	{
		int *pInt;
		if(NCM_PROJECT.IntParams.Lookup("ViewChangeTime", pInt))
			RDynViewChange::TimePeriod = *pInt;
		RDynViewChange DVChange(BufMatr, M1);
		BMatr M;
		for(int DynInd = 0; DynInd < 1000 && DVChange.GetNextMatr(M); ++DynInd)
		{
			glLoadMatrixd(M.GetArray());
			SetZRange();
			DrawWithTranslucent();

			BSwapBuffers();
		}
	}
	m_ChangeView = 0;

	glLoadMatrixd(M1.GetArray());
	SetZRange();
	DrawWithTranslucent();

	HighlightedPoint.Draw();
	pDoc->Distance.Draw();

	BSwapBuffers();

	RestoreContext();
}

void NOpenGLWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	static CPoint PrevPoint;
	if(PrevPoint == point)
		return;
	PrevPoint = point;
	CNCMDoc* pDoc = (CNCMDoc*)GetDocument();
	if (pDoc->Comparison.IsActive())
	{
		BPoint P, V;
		BPoint oPoint, oNormal;
		GetMouseRay(point, P, V);
		RecalcStockPoint(P);
		RecalcStockPoint(V);
		CString Str;

		bool InfoOn = false;
		if (pDoc->GetStaticStockPoint(P, V, oPoint, oNormal) == 0)
			InfoOn = pDoc->Comparison.GetInfoTipText(oPoint, Str);
		InfoTip.EnsureCreated(this);
		if (InfoOn)
		{
			InfoTip.Activate(TRUE);
			InfoTip.UpdateTipText(Str, this);
		}
		else
		{
			InfoTip.UpdateTipText("", this);
		}
	}
	if (pDoc->Measure.IsActivated())
	{
		MakeCurrent();
		BPoint View(GetViewDir());
		double wz, x, y, z;
		GetMouseCoord(point, &wz, &x, &y, &z);
		if (wz == 0.0)// Point is outside the picture
			pDoc->Measure.SetSurfInformation(BPoint(0., 0., 0., 1.), 0, cadrID(-1)); //Clear surf information
		else
		{
			BPoint P(x, y, z, 1.);
			RecalcStockPoint(P);
			RecalcStockPoint(View);
			bool PointOnSection = false;
			bool PointNerarerSection = false;
			NStockSection *pSection = pDoc->GetStockSection();
			if (pSection)
			{
				if (pSection->IsActive())
				{
					if (pSection->ContainsP(P, 0.05))
					{
						PointOnSection = true;
					}
					else if (pSection->CutsOffP(P, View))
					{
						PointNerarerSection = true;
					}
				}
			}

			if (PointNerarerSection || PointOnSection )
				pDoc->Measure.SetSurfInformation(BPoint(), 0, cadrID(-1)); //Clear surf information
			else
				pDoc->Measure.SetPoint(P, View, FIND_POINT);

		}
		RestoreContext();
		pDoc->UpdateAllViews(NULL, N_STATE);
	}
	else if(pDoc->Dimensions.IsActivated())
	{
		MakeCurrent();
		if (pDoc->Dimensions.GetMouseMoveAction() == DDimensions::MMA_Snap)
		{
			RPickID pickID = DoSnap(point);
			// highlight selected dimension
			if (pickID.GetGroup() == OG_DIMENS)
				pDoc->Dimensions.SetHighlighted(pickID.GetObjName());
			else
				pDoc->Dimensions.SetHighlighted(0);
		}
		else if(pDoc->Dimensions.GetMouseMoveAction() == DDimensions::MMA_Change)
		{
			BPoint pt;
			BPoint viewDir;
			GetMouseRay(point, pt, viewDir);
			pDoc->Dimensions.PerformChanges(pt, viewDir);
		}

		pDoc->UpdateAllViews(NULL, N_OGL);

		RestoreContext();
	}
	else if (!IsZoomFuncActive())
	{
		SetCursor(0); // Default cursor
		if (pDoc->DynamicLavel > 0)
		{
			tme.cbSize = sizeof(tme);
			tme.hwndTrack = m_hWnd;
			tme.dwFlags = TME_HOVER | TME_LEAVE;
			tme.dwHoverTime = HOVER_TIME;//HOVER_DEFAULT;

			_TrackMouseEvent(&tme); // Needed for the dynamic interface
		}
		if (!(nFlags & (MK_MBUTTON | MK_LBUTTON)))
		{
			CNCMDoc* pDoc = (CNCMDoc*)GetDocument();
			ASSERT_VALID(pDoc);
			GLdouble x = 0., y = 0., z = 0.;
			MakeCurrent();
			double wz;
			bool GLPointCoord = false;
			NTiParams& Par = NCM_PROJECT.Defaults;
			if (Par.GetBool("Defaults/Render/GLPointCoord@Enable", false))
				GLPointCoord = true;
			if (GLPointCoord)
				GetMouseCoordGL(point, &wz, &x, &y, &z);
			else
				GetMouseCoord(point, &wz, &x, &y, &z);
			RestoreContext();
			pDoc->StatusX = x;
			pDoc->StatusY = y;
			pDoc->StatusZ = z;
		}
	}

	if (pDoc->Distance.IsActive())
	{
		GLint viewport[4];
		GLdouble projMatrix[16], modelMatrix[16];
		glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
		glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
		glGetIntegerv(GL_VIEWPORT, viewport);

		double x, y, z;
		gluUnProject(point.x, viewport[3] - point.y, 0.0, modelMatrix, projMatrix, viewport, &x, &y, &z);

		BPoint pt = BPoint(x, y, z, 1.);
		pDoc->Distance.SetLastPoint(pt);
		NUpdate();
	}

	BOpenGLWndVD::OnMouseMove(nFlags, point);
}

RPickID NOpenGLWnd::DoSnap(const CPoint& point)
{
	BPoint pt;
	BPoint ViewDir;
	GetMouseRay(point, pt, ViewDir);
	bool StockView = this->IsKindOf(RUNTIME_CLASS(NOGLStockWnd));
	CNCMDoc* pDoc = (CNCMDoc*)GetDocument();
	auto& Snaps = pDoc->GetSnaps();
	Snaps.EnsurePrepared(ViewDir, StockView);

	RPickID pickID = PickOct(point, true);
	if (pickID.GetGroup() == OG_GR_EMPTY)
		pickID = Pick(point, PT_DIMENSIONS | PT_UCS, Snaps.GetEps());
	// сначала вз€ть точку прив€зки
	Snaps.SetMatr(StockMatr, StockAxisMatr);
	Snaps.Snap(pt, ViewDir, StockView, pickID);
	// проверить видимость точки прив€зки
	if (Snaps.GetFound())
	{
		double koef = 1.0;
		if (pickID.GetGroup() == OG_DIMENS)
			koef = pDoc->Dimensions.GetObjects().Get(pickID.GetObjName())->GetWidth();
		Snaps.SetFound(IsVisiblePoint(Snaps.GetSnapPoint() * StockAxisMatr, koef));
	}
	return pickID;
}

void NOpenGLWnd::DrawHighlightedCadr(bool ForStock)
{
	if(HighlightedCadr.IsUndef())
		return;
	if (ForStock != HighlightedCadrFrStock)
		return;
	const int StPos = abs(HighlightedCadrStPos) - 1;
	if (StPos != 0)
	{
		const bool SkelHighl = HighlightedCadrStPos < 0;
		glPushMatrix();
		CNCMDoc* pDoc = (CNCMDoc*)GetDocument();
		MTMachineTool& Mach = pDoc->GetMachTool();
		if(SkelHighl)
			glMultMatrixd((Mach.GetStockMatr(0, true).invr() * Mach.GetStockMovedMatr(StPos, true)).GetArray());
		else
			glMultMatrixd((Mach.GetStockMatr(0, true).invr() * Mach.GetStockMatr(StPos, true)).GetArray());
	}
	glPushAttrib(GL_LINE_BIT | GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glColor4fv(SnapCol.array);
		glLineWidth(GLfloat(HighlightedThick));
		HighlightedCadr.Draw(NCM_PROJECT.GetDouble("TolLineApp", 0.01), -1.);
	glPopAttrib();
	if (StPos != 0)
		glPopMatrix();
}



void NOpenGLWnd::OnFileExport() 
{
	// export the image to external file
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT ,viewport);

	GLint PackAl;// alignment of the row in ReadPixels (4- default)
	glGetIntegerv(GL_PACK_ALIGNMENT,&PackAl);
	unsigned char *ColorBuf = new unsigned 
		char[(PackAl-(3*viewport[2])%PackAl+3*viewport[2])*viewport[3]];
	
	glReadBuffer(GL_BACK);
	glReadPixels(0,0,viewport[2],viewport[3],GL_RGB,GL_UNSIGNED_BYTE,ColorBuf);
//Call DLL
	typedef bool (*lpFunc)(unsigned char *,int,int);
	bool b;
    HINSTANCE  hLibrary;
    lpFunc Func;
    hLibrary = AfxLoadLibrary("eximage.dll"); // Load the DLL now    
    if (hLibrary != NULL)
    {
        Func = (lpFunc) GetProcAddress(hLibrary, "?NESaveFile@@YA_NPAEHH@Z");
        if (Func != NULL)
            b = ((Func)(ColorBuf,viewport[2],viewport[3]));
        else 
			AfxMessageBox(IDS_EXIMAGE_ERR);
		if(!b)
			AfxMessageBox(IDS_EXIMAGE_ERR);
	}
    else
	{
		int e = GetLastError();
		AfxMessageBox(IDS_MES_ERR_EXI);
	}
    AfxFreeLibrary(hLibrary);         // Unload DLL from memory	
//End Call DLL	
	
	delete[] ColorBuf;
}

void NOpenGLWnd::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CNCMDoc* pDoc = (CNCMDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc->JumpToStatusCadrAbs())
	{// try to find object if StatusCadrAbs is not fresh
		MakeCurrent();
		RPickID ObjID = Pick(point, PT_STFIPA | PT_MACHINE, SelectionRegionSize);
		if (ObjID.GetGroup() & (OG_STOCKS | OG_PARTS | OG_FIXTURES | OG_MACHINE | OG_TOOLS))
		{
			CLeftView* pLeftView = static_cast<CLeftView*>(((CMainFrame*)AfxGetMainWnd())->GetLeftPane());
				if (pLeftView != nullptr)
					pLeftView->OnPick(ObjID);
		}
		RestoreContext();
	}

	BOpenGLWndVD::OnLButtonDblClk(nFlags, point);
}

RPickID NOpenGLWnd::Pick(const CPoint& ipoint, int Type, GLdouble Size)
{
	GLint viewport[4];
	GLfloat projMatrix[16],modelMatrix[16];
	glMatrixMode(GL_MODELVIEW);
	glGetFloatv(GL_MODELVIEW_MATRIX ,modelMatrix);
	glGetFloatv(GL_PROJECTION_MATRIX ,projMatrix);
	glGetIntegerv(GL_VIEWPORT ,viewport);

	CPoint point(ipoint.x, viewport[3] - ipoint.y);

	// Set Selection Buffer.
	GLuint selectBuf[BUFSIZE] ;
	glSelectBuffer(BUFSIZE, selectBuf) ;

	// Turn On Selection Mode.
	glRenderMode(GL_SELECT) ;

	// Initialize Name Stack
	glInitNames() ;
	// NOTE: Depends on the Visual to push the name...

	// Set Up Picking Matrix
	glMatrixMode(GL_PROJECTION) ;
	glPushMatrix() ;
	glLoadIdentity() ;

	IsOpenGLErr("Pick0");

	gluPickMatrix( (double)point.x,
					(double)point.y,
					Size, Size,
					viewport) ;
	
	IsOpenGLErr("Pick1");
	glOrtho(-(double)viewport[2], (double)viewport[2],
		-(double)viewport[3], (double)viewport[3],
		-1e7, 1e7);

	IsOpenGLErr("Pick10");
	glMatrixMode(GL_MODELVIEW) ;

	// Re-Render Scene for Picking.
	CNCMDoc* pDoc = (CNCMDoc*)GetDocument();
	DrawScene4Pick(Type);


	glMatrixMode(GL_PROJECTION) ;
	glPopMatrix() ;

	glFlush() ; // Just in Case.

	// Turn Off Selection Mode.
	GLint hits = glRenderMode(GL_RENDER) ; 

	// OnRender requires this...
	glMatrixMode(GL_MODELVIEW) ;
	IsOpenGLErr("Pick2");

	// Find Hits
	RPickID id;

	GLuint Z1 = 0;
	int sbI = 0;
	for(int i = 0; i < hits; ++i)
	{
		GLuint cNames = selectBuf[sbI] ;
		GLuint Zi = selectBuf[sbI + 1] ;
		if(cNames > 1)
		{
			if ((Z1 == 0) || (Zi < Z1))
			{
				Z1 = Zi ;
				id.Set(cNames, &selectBuf[sbI + 3]);
			}
		}
		sbI += cNames + 3;
	}
	return id;
}

RPickID NOpenGLWnd::PickOct(const CPoint& point, bool Syncr)
{
	CNCMDoc* pDoc = (CNCMDoc*)GetDocument();
	BPoint P, V;
	GetMouseRay(point, P, V);
	BMatr Tr(StockAxisMatr.invr());
	P = P * Tr;
	V = V * Tr;
	V.Unit();
	double MaxL2 = (numeric_limits<double>::max)() * 0.5; // squared max distanse
	double x, y, z, wz;
	GetMouseCoordGL(point, &wz, &x, &y, &z);
	if (wz != 0.)
	{
		BPoint Q(x, y, z, 1.);
		Q = Q * Tr;
		MaxL2 = (P - Q).D2();
	}
	double dx, dy, dz;
	BaseRender::GetPixelDim(dx, dy, dz);

	// try to find the cadr near the cursor
	if (Syncr)
	{
		return pDoc->GetRayCastManager().PickRCMSync(P, V, SelectionRegionSize * dx, MaxL2 * 1.01, pDoc);
	}
	else
	{
		using namespace concurrency;
		auto PickTask = task<RPickID>([&]() {return pDoc->GetRayCastManager().PickRCM(P, V, SelectionRegionSize * dx, MaxL2 * 1.01, pDoc); });
		CPoint bpoint = point;
		ClientToScreen(&bpoint);
		while (!PickTask.is_done())
		{// Interrupt if mouse has moved
			CPoint NewPoint;
			GetCursorPos(&NewPoint);
			if (NewPoint != bpoint)
			{
				pDoc->GetRayCastManager().SetStopPick();
				break;
			}
			Sleep(40);
		}
		RPickID index = PickTask.get();
		return index;
	}
}

void NOpenGLWnd::OnViewOutline() 
{
	NState.VMode = Outline;
	OnUpdate(NULL,N_OGL,NULL);
}

void NOpenGLWnd::OnUpdateViewOutline(CCmdUI* pCmdUI) 
{
	CNCMDoc *pDoc = (CNCMDoc *)GetDocument();
	ASSERT(pDoc);
	pCmdUI->Enable(TRUE);
	if(NState.VMode == Outline) 
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}


bool NOpenGLWnd::DrawCadrGeom(const NCadrGeom &InGeom, double Eps, double t)
{
	return InGeom.Draw(Eps, t);
}

void NOpenGLWnd::InitCursors()
{
	CWinApp *pMyApp = AfxGetApp();
	Cursors[0] = pMyApp->LoadCursor(IDC_CUR_MAIN);

// 1-right, 2-up, 3-left, 4-down
// 1:4-G0, 5:8-G1, 9:12-G2, 13:16-G3
	Cursors[ 1] = pMyApp->LoadCursor(IDC_CUR_SEL32);
	Cursors[ 2] = pMyApp->LoadCursor(IDC_CUR_SEL31);
	Cursors[ 3] = pMyApp->LoadCursor(IDC_CUR_SEL30);
	Cursors[ 4] = pMyApp->LoadCursor(IDC_CUR_SEL29);

	Cursors[ 5] = pMyApp->LoadCursor(IDC_CUR_SEL4);
	Cursors[ 6] = pMyApp->LoadCursor(IDC_CUR_SEL3);
	Cursors[ 7] = pMyApp->LoadCursor(IDC_CUR_SEL2);
	Cursors[ 8] = pMyApp->LoadCursor(IDC_CUR_SEL1);

	Cursors[ 9] = pMyApp->LoadCursor(IDC_CUR_SEL16);
	Cursors[10] = pMyApp->LoadCursor(IDC_CUR_SEL15);
	Cursors[11] = pMyApp->LoadCursor(IDC_CUR_SEL14);
	Cursors[12] = pMyApp->LoadCursor(IDC_CUR_SEL13);

	Cursors[13] = pMyApp->LoadCursor(IDC_CUR_SEL24);
	Cursors[14] = pMyApp->LoadCursor(IDC_CUR_SEL23);
	Cursors[15] = pMyApp->LoadCursor(IDC_CUR_SEL22);
	Cursors[16] = pMyApp->LoadCursor(IDC_CUR_SEL21);

	Cursors[17] = pMyApp->LoadCursor(IDC_CUR_POINT);
}

void NOpenGLWnd::SnapCadrCursor(const CPoint& ipoint)
{
	HighlightedCadr.Set(undef, 0, 0, 0, 0, 0, 0);
	HighlightedThick = 3.;
	HighlightedPoint.Clear();
	CNCMDoc *pDoc = (CNCMDoc *)GetDocument();
	pDoc->StatusCadr.Prog = -1;//Clear status cadr
	pDoc->StatusCadr.Cadr = 0;

	RPickID index = PickOct(ipoint, false);
	//if(index.GetGroup() == OG_GR_EMPTY)
	//	index = Pick(point, PT_TRAJECTORY);
// Clear old selection and point cursor
	if(!(index.GetGroup() & (OG_PROGRAMS | OG_STOCKS)))
	{
		SetCursor(0); // Default cursor
		NUpdate();
		return;
	}
	NProgram *pProg = NULL;
	const BGeomArray *pGeomArray = NULL;// Founded geom array
	if(index.GetGroup() == OG_STOCKS)
	{// Stock section contour found
		HighlightedCadrFrStock = true;
		class NStock *pStock = pDoc->GetStock(index.GetObjName());
		if(!pStock)
			return;
		pGeomArray = pStock->GetTurnSkeleton().GetSectGeom(index.GetObjSubName0());
		if (pGeomArray)
		{
			HighlightedCadrID.Prog = 0;
			HighlightedCadrID.Cadr = index.GetObjSubName1();
		}
	}
	else
	{// Program trajectory found
		HighlightedCadrFrStock = false;
		int ChanCount = pDoc->GetChannelsCount();
		int ProgInd = index.GetObjName();
		int ChanInd = ProgInd % ChanCount;
		if (ChanInd != pDoc->GetCurChannel()->GetChannelInd())
			return;
		
		ProgInd /= ChanCount;
		const auto& ProgramList = *pDoc->GetChannel(ChanInd)->NGetProgramListCn();
		CString key;
		ProgramList.GetAt(ProgInd, key, (CObject*&)pProg);
		if (pProg)
		{
			pGeomArray = ProgramList.HaveProgTraj() ? &pProg->GetGeomArray(0) : &pProg->GetCompGeomArray(0);
			HighlightedThick = pProg->GetThickness() + 2.;

			pDoc->StatusCadrAbs.Prog = index.GetObjName();
			pDoc->StatusCadrAbs.Cadr = index.GetObjSubName0();
			pDoc->StatusCadrAbsTime = GetTickCount64();
			//pDoc->StatusCadr.Prog = index.GetObjName();
			pDoc->StatusCadr = pProg->GetCadrByHist(pDoc->StatusCadrAbs.Cadr);
			HighlightedCadrID.Prog = index.GetObjName();
			HighlightedCadrID.Cadr = index.GetObjSubName1() > 0 ? index.GetObjSubName1() : index.GetObjSubName0();
			bool Vis = pDoc->IsCadrVisible(HighlightedCadrID);
			if (!Vis)
				HighlightedCadrID.Prog = -1;
		}
	}
	if (!pGeomArray)
		return;
	NCadrGeom *pGeom = nullptr;
	if (HighlightedCadrID.Prog >= 0 && HighlightedCadrID.Cadr >= 0 && HighlightedCadrID.Cadr < unsigned int(pGeomArray->GetSize()))
		pGeom = pGeomArray->GetAt(HighlightedCadrID.Cadr);// Founded geom
	////TEST
	//if (pGeom->GetHistID() != HighlightedCadrID)
	//	Beep(1000, 1000);
	if(!pGeom)
		return;

	pDoc->SelectedPrev = *pGeom;
	pDoc->SelectedNext.Clear();
	if (pProg)
		pProg->GetToolAxisDir(HighlightedCadrID.Cadr, pDoc->CadrToolAxis);


	GLint viewport[4];
	GLdouble projMatrix[16], modelMatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	glGetIntegerv(GL_VIEWPORT, viewport);

	CPoint point(ipoint.x, viewport[3] - ipoint.y);

	NCadrGeom *pNewGeom = nullptr;
	NCadrGeom *pOldGeom = pGeom;
	if (pGeom->GetMatrNum() != 0)
	{
		pNewGeom = new NCadrGeom(*pGeom);
		pNewGeom->ApplyMatr();
		pGeom = pNewGeom;
	}
	BPoint Pb(pGeom->GetBP() * StockAxisMatr);
	GLdouble xb, yb, zb, sx, sy, sz;
	xb = Pb.GetX();
	yb = Pb.GetY();
	zb = Pb.GetZ();
	gluProject(xb, yb, zb,
		modelMatrix, projMatrix, viewport, &sx, &sy, &sz); 


// try to find the vertex near the cursor
	if(abs(point.x - (int)sx) < SelectionRegionSize && abs(point.y - (int)sy) < SelectionRegionSize)
	{
		HighlightedPoint.Set(xb, yb, zb); 
		SetCursor(21); // Point snap cursor
		CPoint pt((int)sx,viewport[3]-(int)sy);
		ClientToScreen(&pt);
		pDoc->StatusX = xb;
		pDoc->StatusY = yb;
		pDoc->StatusZ = zb;
		pDoc->SelectedNext = pDoc->SelectedPrev;
		const NCadrGeom* pFoundGeom = pGeomArray->FindPreviousGCadr(HighlightedCadrID.Cadr);
		if (pFoundGeom != nullptr)
			pDoc->SelectedPrev = *pFoundGeom;

		if(pTrGeomDlg)
			pTrGeomDlg->Update(pDoc->SelectedPrev,pDoc->SelectedNext, pDoc->CadrToolAxis);
	}
	else
	{
		BPoint Pe(pGeom->GetEP() * StockAxisMatr);
		GLdouble xe, ye, ze;
		xe = Pe.GetX();
		ye = Pe.GetY();
		ze = Pe.GetZ();
		gluProject(xe, ye, ze,
			modelMatrix, projMatrix, viewport, &xb, &yb, &zb);
		if (abs(point.x - (int)xb) < SelectionRegionSize && abs(point.y - (int)yb) < SelectionRegionSize)
		{
			HighlightedPoint.Set(xe, ye, ze);
			SetCursor(21); // Point snap cursor
			CPoint pt((int)xb, viewport[3] - (int)yb);
			ClientToScreen(&pt);
			pDoc->StatusX = xe;
			pDoc->StatusY = ye;
			pDoc->StatusZ = ze;
			for (int l = 1; HighlightedCadrID.Cadr + l < pGeomArray->GetSize(); ++l)
			{
				pGeom = pGeomArray->GetAt(HighlightedCadrID.Cadr + l);

				if (!pGeom->HaveGeom())
					continue; // Ignore non geom elements
				if (pGeom->IsCycle())
					continue; // Ignore cycles

			pDoc->SelectedNext = *pGeom;
			if(pProg)
			{
				auto ind = HighlightedCadrID.Cadr + l;
				pProg->GetToolAxisDir(ind, pDoc->CadrToolAxis);
				//Change Status Cadr
				pDoc->StatusCadr = pProg->GetCadrByHist(ind);
			}
			break;
			}
			if (pTrGeomDlg)
				pTrGeomDlg->Update(pDoc->SelectedPrev, pDoc->SelectedNext, pDoc->CadrToolAxis);
		}
	}
	delete pNewGeom;
	// Distance
	if (HighlightedPoint.IsDefined())
	{
		if (pDoc->Distance.IsActive())
		{
			double x, y, z;
			HighlightedPoint.Get(x, y, z);
			pDoc->Distance.SetSnapPoint(BPoint(x, y, z, 1.));
		}
		pDoc->UpdateAllViews(NULL, N_STATE);
		NUpdate();
		return;
	}
	if (pTrGeomDlg)
		pTrGeomDlg->Update(pDoc->SelectedPrev, pDoc->SelectedNext, pDoc->CadrToolAxis);
	HighlightedCadr = *pOldGeom;
	HighlightedCadrStPos = (index.GetStPos() + 1) * (((index.GetGroup() & OG_STOCKS) != 0) ? -1 : 1);

	pDoc->UpdateAllViews(NULL, N_STATE);
	NUpdate();

	sx-=xb; sy-=yb; sz-=zb;
	int nc = 0;
	if(sx > sy)
		if ( sy > -sx)
			nc = 3; // Right cursor
		else
			nc = 2; // Down cursor
	else
		if(sy > -sx)
			nc = 4; // Up cursor
		else
			nc = 1; // Left cursor
	if(pOldGeom->IsFast())
		nc += 0;
	else if(pOldGeom->IsLine())
		nc += 4;
	else if(pOldGeom->IsCWArc())
		nc += 8;
	else if(pOldGeom->IsCCWArc())
		nc += 12;
	else if(pOldGeom->IsRotate())
		nc += 16;
	else
		nc = 0;

	SetCursor(nc);
}

LPARAM NOpenGLWnd::OnMouseHover(WPARAM wp, LPARAM lp)
{
	if (m_wndToolBar.IsWindowVisible())
		return 0;
	if (IsZoomFuncActive())
		return 0;

	if (((CMainFrame *)AfxGetMainWnd())->IsCuttingInProgress())
		return 0;

	static bool Bisy = false;
	if (Bisy)
		return 0;
	Bisy = true;

	CPoint point(GET_X_LPARAM(lp), GET_Y_LPARAM(lp));

	IsOpenGLErr("Hover0:");

	MakeCurrent();

	SnapCadrCursor(point);
	IsOpenGLErr("Hover1:");
	Bisy = false;

	return 0;
}

void NOpenGLWnd::SetCursor(int n)
{
// 1-right, 2-up, 3-left, 4-down
// 1:4-G0, 5:8-G1, 9:12-G2, 13:16-G3
	switch(n)
	{
	case  1:
		CursorID = IDC_CUR_SEL32; break;
	case  2:
		CursorID = IDC_CUR_SEL31; break;
	case  3:
		CursorID = IDC_CUR_SEL30; break;
	case  4:
		CursorID = IDC_CUR_SEL29; break;

	case  5:
		CursorID = IDC_CUR_SEL4; break;
	case  6:
		CursorID = IDC_CUR_SEL3; break;
	case  7:
		CursorID = IDC_CUR_SEL2; break;
	case  8:
		CursorID = IDC_CUR_SEL1; break;

	case  9:
		CursorID = IDC_CUR_SEL16; break;
	case 10:
		CursorID = IDC_CUR_SEL15; break;
	case 11:
		CursorID = IDC_CUR_SEL14; break;
	case 12:
		CursorID = IDC_CUR_SEL13; break;

	case 13:
		CursorID = IDC_CUR_SEL24; break;
	case 14:
		CursorID = IDC_CUR_SEL23; break;
	case 15:
		CursorID = IDC_CUR_SEL22; break;
	case 16:
		CursorID = IDC_CUR_SEL21; break;

	case 17:
		CursorID = IDC_CUR_SEL8; break;
	case 18:
		CursorID = IDC_CUR_SEL7; break;
	case 19:
		CursorID = IDC_CUR_SEL6; break;
	case 20:
		CursorID = IDC_CUR_SEL5; break;

	case 21:
		CursorID = IDC_CUR_POINT; break;
	default:
		CursorID = 0; break;
	}

	SetCursorByCursorID();
}


void NOpenGLWnd::SetWinRasterPos0()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glRasterPos3d(-1,-1,0);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glDisable(GL_DEPTH_TEST);
}

void NOpenGLWnd::SetZRange()
{
// Set z range according to current scene gabar if defined

	BBox ScreenDim;
	BBox ObjectDim = GetSceneGabar(false);
	if(!ObjectDim.IsDefined())
		return;
	ObjectDim.ApplyLimits();
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	if(viewport[2] <= 0 || viewport[3] <= 0)
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
	glOrtho(-(double)viewport[2],(double)viewport[2],
			-(double)viewport[3],(double)viewport[3],
			-3. * dZ, 3. * dZ);

	glMatrixMode(GL_MODELVIEW);
}

void NOpenGLWnd::OnViewTrGeom()
{
	if(pTrGeomDlg)
	{
		delete pTrGeomDlg;
		pTrGeomDlg = NULL;
	}
	else
		pTrGeomDlg = new NTranspGeomDlg(this);
}

void NOpenGLWnd::GetMouseStockCoord(const CPoint &point, double *wz, double * x, double * y, double * z)
{
	GetMouseCoord(point, wz, x, y, z);
	BPoint Res(*x, *y, *z, 1.);
	Res = Res * StockMatr.invr();
	double b;
	Res.Get(*x, *y, *z, b);
}

void NOpenGLWnd::RecalcStockPoint(BPoint& P) const
{
	P = P * StockMatr.invr();
}

void NOpenGLWnd::RecalcStockPointBack(BPoint& P) const
{
	P = P * StockMatr;
}

void NOpenGLWnd::GetMouseCoord(const CPoint &point, double *wz, double * x, double * y, double * z)
{
	if(HighlightedPoint.IsDefined())
	{
		GLint viewport[4];
		GLdouble projMatrix[16],modelMatrix[16];
		glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
		glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
		glGetIntegerv(GL_VIEWPORT ,viewport);
		HighlightedPoint.Get(*x, *y, *z);
		double wxv, wyv;
		gluProject(*x, *y, *z, modelMatrix, projMatrix, viewport, &wxv, &wyv, wz);
	}
	else
	{
//		Get obj coord of point

		BPoint P, V;
		BPoint R, N;
		GetMouseRay(point, P, V);
		CNCMDoc* pDoc = (CNCMDoc*)GetDocument();
		BMatr StockMatrI = StockMatr.invr();
		RPickID PickID;
		if (pDoc->GetPartsPoint(P * StockMatrI, V * StockMatrI, R, N, PickID) >= 0)
		{
			*wz = sqrt((R - P).D2() / V.D2());
		}
		else
		{
			*wz = 0.0;
			R = P + V * 0.5;
		}
		R = R * StockMatr;
		*x = R.GetX();
		*y = R.GetY();
		*z = R.GetZ();
	}
}

void NOpenGLWnd::GetMouseCoordGL(const CPoint &point, double *wz, double * x, double * y, double * z)
{

//		Get point coord in stock CS
	GLint viewport[4];
	GLdouble projMatrix[16],modelMatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	glGetIntegerv(GL_VIEWPORT, viewport);
	if(HighlightedPoint.IsDefined())
	{
		HighlightedPoint.Get(*x, *y, *z);
		double wxv, wyv;
		gluProject(*x, *y, *z, modelMatrix, projMatrix, viewport, &wxv, &wyv, wz);
	}
	else
	{
		GLfloat sz = 0.f;
		int px = min(point.x, viewport[2] - 1);
		int py = min(point.y, viewport[3] - 1);
		px = max(1, px);
		py = max(1, py);
		glReadPixels(px, viewport[3] - py, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &sz);
		if(sz == 1.)
			sz = 0.f;
		gluUnProject(px + 0.5, viewport[3] - py + 0.5, sz, modelMatrix, projMatrix, viewport,
				x, y, z);
		*wz = sz;
	}
	BPoint P(*x, *y, *z, 1.);
	RecalcStockPoint(P);
	double h;
	P.Get(*x, *y, *z, h);
}

void NOpenGLWnd::NAnimateUpdate(void)
{
	if(!IsWindowVisible())
		return;
	if(! CanRedraw())//Do not redraw while TB is active
		return;

	NUpdate();

}

BOOL NOpenGLWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if(!m_wndToolBar.IsWindowVisible())
	{
		if (SetCursorByCursorID())
			return TRUE;
	}
	return BOpenGLWndVD::OnSetCursor(pWnd, nHitTest, message);
}

void NOpenGLWnd::LoadTBFromIcons(void)
{
	CImageList *pList = m_wndToolBar.GetToolBarCtrl().GetImageList();
	if(pList)
		ImageList_Destroy(pList->m_hImageList);

	HMODULE hins = GetModuleHandle(_T("ResInv.dll") );
	if( hins == NULL )
		return;
	int BSize = 16;

	if(NCMComData::IsTurnEnabled())
	{
	// ToolBar View for turn
		UINT IDs0[24] = 
		{ ID_TB_ZOOM_W, ID_TB_ZOOM_SC, ID_TB_ZOOM_P, ID_TB_VIEW_T1, ID_TB_VIEW_XZ, ID_TB_VIEW_T2,
		ID_TB_ZOOM_ALL, ID_TB_VIEW_CHANHGETYPE, ID_ROTANGLES, ID_TB_VIEW_XYLeft, ID_TB_VIEW_ZX, ID_TB_VIEW_XYRight,
		ID_TB_ZOOM_X, ID_TB_ZOOM_Y, ID_TB_ZOOM_Z, ID_TB_VIEW_T3, ID_TB_VIEW_ZY, ID_TB_VIEW_T4,
		ID_TB_SAVE, ID_TB_VIEW1, ID_TB_VIEW2, ID_TB_VIEW3, ID_TB_VIEW4, ID_TB_VIEW5};
		m_wndToolBar.SetButtons(IDs0, 24);
		m_wndToolBar.SetSizes(CSize(BSize + 7, BSize + 6),CSize(BSize, BSize));
		HIMAGELIST HIList;
		HIList = ImageList_Create(BSize,BSize,ILC_COLOR32 | ILC_MASK, 10, 4);
		HICON buf;
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_ZOOM), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_ZOOMD), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_PAN), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_VIEW_T1), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_VIEW_XZ), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_VIEW_T2), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_ZALL), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_VIEW_CHANHGETYPE), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_VIEW_ANGLEROT), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_VIEW_XYLeft), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_VIEW_ZX), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_VIEW_XYRight), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_ZOOM_X), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_ZOOM_Y), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_ZOOM_Z), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_VIEW_T3), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_VIEW_ZY), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_VIEW_T4), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_VIEW_SAVE), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_VIEW1), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_VIEW2), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_VIEW3), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_VIEW4), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_VIEW5), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		m_wndToolBar.GetToolBarCtrl().SendMessage(TB_SETIMAGELIST,	0, (LPARAM)HIList);
	}
	else
	{
	// ToolBar View for mill
		UINT IDs0[24] = 
		{ ID_TB_ZOOM_W, ID_TB_ZOOM_SC, ID_TB_ZOOM_P, ID_TB_VIEW_RU, ID_TB_VIEW_F, ID_TB_VIEW_LU,
		ID_TB_ZOOM_ALL, ID_TB_VIEW_CHANHGETYPE, ID_ROTANGLES, ID_TB_VIEW_R, ID_TB_RESET, ID_TB_VIEW_L,
		ID_TB_ZOOM_X, ID_TB_ZOOM_Y, ID_TB_ZOOM_Z, ID_TB_VIEW_RD, ID_TB_VIEW_B, ID_TB_VIEW_LD,
		ID_TB_SAVE, ID_TB_VIEW1, ID_TB_VIEW2, ID_TB_VIEW3, ID_TB_VIEW4, ID_TB_VIEW5};
		m_wndToolBar.SetButtons(IDs0, 24);
		m_wndToolBar.SetSizes(CSize(BSize + 7, BSize + 6),CSize(BSize, BSize));
		HIMAGELIST HIList;
		HIList = ImageList_Create(BSize,BSize,ILC_COLOR32 | ILC_MASK, 10, 4);
		HICON buf;
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_ZOOM), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_ZOOMD), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_PAN), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_VIEW_RU), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_VIEW_F), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_VIEW_LU), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_ZALL), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_VIEW_CHANHGETYPE), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_VIEW_ANGLEROT), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_VIEW_R), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_VIEW_RESET), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_VIEW_L), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_ZOOM_X), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_ZOOM_Y), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_ZOOM_Z), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_VIEW_RD), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_VIEW_B), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_VIEW_LD), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_VIEW_SAVE), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_VIEW1), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_VIEW2), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_VIEW3), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_VIEW4), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		ImageList_AddIcon(HIList, buf = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_TB_VIEW5), IMAGE_ICON, BSize, BSize, LR_DEFAULTCOLOR));
		DestroyIcon(buf);
		m_wndToolBar.GetToolBarCtrl().SendMessage(TB_SETIMAGELIST,	0, (LPARAM)HIList);
	}
}

void NOpenGLWnd::SetViewPoint(const CString & ViewName)
{
	DoStdView(ViewName);
	SaveView(0);
}

void NOpenGLWnd::Proc3DxInput(T3DData * pData)
{
	CNCMDoc* pDoc = (CNCMDoc*)GetDocument();

	BPoint StockCenter(0., 0., 0., 1.);
	if (pDoc->IsThereAnyStock())
		StockCenter = pDoc->GetStocksGabar().GetCenterPoint();

	glFinish();
	BMatr UnProj = BaseRender::GetUnprojectMatr().NormRows();
	BPoint TDir = BPoint(pData->Xt, pData->Yt, 0., 0.) * UnProj;
	BPoint RDir = BPoint(pData->Xr, pData->Yr, -pData->Zr, 0.) * UnProj;
	glTranslated(StockCenter.GetX(), StockCenter.GetY(), StockCenter.GetZ()); 
	glRotated(pData->Ar, RDir.GetX(), RDir.GetY(), RDir.GetZ()); 
	glScaled(1 + pData->Zt / 50., 1 + pData->Zt / 50., 1 + pData->Zt / 50.);
	glTranslated(-StockCenter.GetX(), -StockCenter.GetY(), -StockCenter.GetZ()); 
	glTranslated(TDir.GetX(), TDir.GetY(), TDir.GetZ()); 

	m_ChangeView = 1;

	NUpdate();
}

void NOpenGLWnd::DrawPrograms(bool Draw4Pick)
{
// Draw Program List
	CNCMDoc* pDoc = (CNCMDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc->NGetProgramList()->Visible)
		return;
	//bool HasErrors = false;
	//CMapStringToNProgram *pPList = pDoc->NGetProgramList();
	//for(int i = 0; i < pPList->GetMainProgCount(); ++i)
	//{
	//	CString Key;
	//	NProgram *pProg = NULL;
	//	pPList->GetAt(i, Key, (CObject *&)pProg);
	//	HasErrors = HasErrors || pProg->HaveErr();
	//}
	//if(HasErrors)
	//	return;

	DrawProgramsCom(Draw4Pick);
}

void NOpenGLWnd::DrawProgramsCom(bool Draw4Pick)
{
	CNCMDoc* pDoc = (CNCMDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	double main_t = -1.;
	cadrID CadrID = pDoc->GetCurCadr(main_t);
	double t = main_t;

	IsOpenGLErr("DrawProgramsCom0");
	CMapStringToNProgram &ProgramList = *pDoc->NGetProgramList();
	if(ProgramList.GetMainProgCount() <= 0)
		return;

	glPushAttrib(GL_ENABLE_BIT | GL_LINE_BIT);
	if(theApp.IsInCuttingMode())
		glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	CString ActiveProgName = ProgramList.GetKey(CadrID.Prog);
	BOOL DoDraw = TRUE;// used for swtching off non passed programs drawing
	int l = 0;
	int k = 0;
	const BProgram *pActiveProg = ProgramList.GetProg(ActiveProgName);
	int HistPos = 0;
	NToolID ActiveToolID = (pActiveProg == NULL) ? NToolID() : pActiveProg->GetToolByHist(CadrID.Cadr, &HistPos);
	glPushName(OG_PROGRAMS);
	IsOpenGLErr("DrawProgramsCom1");
	for(int i = 0; i < ProgramList.GetMainProgCount(); ++i, ++k)
	{
		CString key;
		NProgram *pProg = NULL;
		ProgramList.GetAt(i, key, (CObject *&)pProg);
		if (pProg == nullptr)
			continue;
		if(pProg->IsVisible())
		{
			glPushAttrib(GL_ENABLE_BIT);//111
			glDisable(GL_LIGHTING); //The trajectory must not be shaded

			int DrawStart = 0;
			auto DrawEnd = pProg->GetGeomArraySize();
			std::vector<BProgVisInterval> VisInt;
			NToolID tool;
			int Cs;
			pProg->GetTCHist(0, Cs, tool);
			for(int Ic = 1; Ic <= pProg->GetTCHistSize(); Ic++)
			{
				bool fl = true;
				NToolCombined* pTc = nullptr;
				const auto it = pDoc->NGetToolList()->find(tool);
				if (it != pDoc->NGetToolList()->end())
				{
					pTc = static_cast<NToolCombined*>(it->second);
					fl = (pTc->IsVisible() == TRUE);
				}

				fl &= (tool == ActiveToolID)  || (!ProgramList.GetDrawOnlyActiveToolPart());

				int Ce;
				if (Ic < pProg->GetTCHistSize())
					pProg->GetTCHist(Ic, Ce, tool);
				else
					Ce = pProg->GetGeomArraySize();
				if (fl)
				{// Add interval
					if (ProgramList.UseToolColor4Prog)
					{
						if (pTc != nullptr)
							VisInt.push_back(BProgVisInterval(Cs, Ce, true, pTc->Color));
						else
							VisInt.push_back(BProgVisInterval(Cs, Ce));
					}
					else
					{
						if (!VisInt.empty())
						{
							BProgVisInterval &LastInt = VisInt.back();
							if (Cs <= LastInt.GetEnd())
							{// Extend existing interval
								LastInt.JoinIntersected(BProgVisInterval(Cs, Ce));
							}
							else
							{// Add new interval
								VisInt.push_back(BProgVisInterval(Cs, Ce));
							}
						}
						else
						{// Add new interval
							VisInt.push_back(BProgVisInterval(Cs, Ce));
						}
					}
				}
				Cs = Ce;
			}
			if(ProgramList.PartialDraw == TrajPart::TP_PART)
			{
				if(ActiveProgName == key)
				{//pProg is current program in main unit (not in the Unit)
				// and needs partial draw
					DrawEnd = CadrID.Cadr;
				}
				if(k <= int(CadrID.Prog))
					DoDraw = TRUE;
				else
					DoDraw = FALSE;
			}// End partial draw
			else 
			{
				t = -1.;
				if(ProgramList.PartialDraw == TrajPart::TP_WORM)
				{
					if(ActiveProgName == key)
					{
						DrawEnd = CadrID.Cadr + ProgramList.WormAfter;
						DrawStart = (CadrID.Cadr < ProgramList.WormBefore) ? 0 : CadrID.Cadr - ProgramList.WormBefore;
						DoDraw = TRUE;
					}
					else
						DoDraw = FALSE;
				}//END warm draw
			}
			// Intersect DS,DE arrays with DrawStart,DrawEnd interval
			if (DoDraw)
			{
				INT_PTR b;
				for (b = 0; b < INT_PTR(VisInt.size()); ++b)
					if (VisInt[b].GetEnd() >= DrawStart)
						break;
				// Remoove first b intervals
				VisInt.erase(VisInt.begin(), VisInt.begin() + b);
				if (!VisInt.empty())
				{
					VisInt[0].GetStart() = max(VisInt[0].GetStart(), DrawStart);
					for (b = VisInt.size() - 1; b >= 0; --b)
						if (VisInt[b].GetStart() <= DrawEnd)
							break;
					// Remove last unused intervals
					VisInt.erase(VisInt.begin() + (b + 1), VisInt.end());
					if (b >= 0)
						VisInt[b].GetEnd() = min(VisInt[b].GetEnd(), int(DrawEnd));
				}
			}
			else
				VisInt.clear();
			if(!VisInt.empty() && DoDraw)
			{
				cadrID BoldID;
				BoldID = CadrID;
				double Progt = t;
				if (ActiveProgName != key)
				{
					Progt = -1.;
					BoldID.Cadr = 0;
				}
				std::set<int> PosSet;
				pDoc->GetCurChannel()->GetStPosSet(PosSet);
				if (PosSet.size() == 1 && *PosSet.begin() == 0)
				{
					glPushName(k);
					pProg->DrawInt(0, ProgramList.HaveCompTraj(), ProgramList.HaveProgTraj(), Draw4Pick, VisInt, BoldID.Cadr
						, Progt, ProgramList.UseAxes, ProgramList.DrawNodes);
					glPopName();
				}
				else
				{
					MTMachineTool& Mach = pDoc->GetMachTool();
					for each (auto StPos in PosSet)
					{
						BMatr M = Mach.GetStockMatr(StPos, true) * Mach.GetStockMatr(0, true).invr();
						glPushMatrix();
						glMultMatrixd(M.GetArray());
						glPushName(k);
						pProg->DrawInt(StPos, ProgramList.HaveCompTraj(), ProgramList.HaveProgTraj(), Draw4Pick, VisInt, BoldID.Cadr
							, Progt, ProgramList.UseAxes, ProgramList.DrawNodes);
						glPopName();
						glPopMatrix();
					}
					if (RLines::DrawWithNamesException == true)
						pDoc->DynamicLavel = 0;
				}
			}
			glPopAttrib();//111
		}
	}
	IsOpenGLErr("DrawProgramsCom2");
	glPopAttrib();
	glPopName();// OG_PROGRAMS
}

void NOpenGLWnd::DrawScene4Pick(int Type)
{
	DrawScene(RM_UNDEF);
}

void NOpenGLWnd::OnStatusCoordRel()
{// Added to prevent disabling of the correspondent pane in status bar
}

void NOpenGLWnd::OnUpdateCoordRel(CCmdUI *pCmdUI)
{
	CNCMDoc *pDoc = (CNCMDoc *)GetDocument();
	if(!pDoc)
		return;
	ASSERT(pDoc);
	CString str;
	double d = sqrt(
		(pDoc->StatusX-NState.BaseX)*(pDoc->StatusX-NState.BaseX)+
		(pDoc->StatusY-NState.BaseY)*(pDoc->StatusY-NState.BaseY)+
		(pDoc->StatusZ-NState.BaseZ)*(pDoc->StatusZ-NState.BaseZ));

	str.Format((MM_INCH == M_MM) ? IDS_ST1_F_MM : IDS_ST1_F_IN,
		pDoc->StatusX-NState.BaseX,
		pDoc->StatusY-NState.BaseY,
		pDoc->StatusZ-NState.BaseZ,
		d);
	pCmdUI->SetText(str);
}

void NOpenGLWnd::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	BOpenGLWndVD::OnActivate(nState, pWndOther, bMinimized);
}

void NOpenGLWnd::OnSetFocus(CWnd* pOldWnd)
{
	BOpenGLWndVD::OnSetFocus(pOldWnd);
	if (m_hrc == NULL || m_pDC == NULL)
		return;
	MakeCurrent();
	NUpdate();
}

void NOpenGLWnd::OnKillFocus(CWnd* pNewWnd)
{
	BOpenGLWndVD::OnKillFocus(pNewWnd);

	if(GetParent() != pNewWnd)
	{
		HighlightedCadr.Set(undef, 0, 0, 0, 0, 0, 0);
		HighlightedPoint.Clear();
		CNCMDoc *pDoc = (CNCMDoc *)GetDocument();
		if (pDoc != nullptr)
		{
			pDoc->StatusCadr.Prog = -1;//Clear status cadr
			pDoc->StatusCadr.Cadr = 0;
		}
	}

	NUpdate();
}
void NOpenGLWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if(!IsWindowVisible())
		return;
	CRect rect;
	rect = dc.m_ps.rcPaint;
	if(rect.Width() == 0 || rect.Height() == 0)
		return;

	if(! CanRedraw())//Do not redraw while TB is active
		return;

	// Set current context
	MakeCurrent();
	
	NUpdate();

	RestoreContext();
}

void NOpenGLWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	CNCMDoc *pDoc = (CNCMDoc*)GetDocument();
	if(pDoc->Dimensions.IsActivated())
	{
        BPoint pt, normal;
		RPickID pickID = GetPointOnModel(point, pt, normal);
        BPoint* pNormal = (pickID.GetGroup() != OG_NO_GROUP) ? &normal : nullptr;
        
        BPoint viewDir = GetViewDir();
        
        pDoc->Dimensions.ProcessMouseClickPoint(pt, pNormal, &viewDir, &pickID);

		if (pDoc->Dimensions.GetObjects().GetChangingID() != 0)
			SetCapture();
		else
			ReleaseCapture();
        
        ((CMainFrame*)AfxGetMainWnd())->UpdateDialogData();

        ((CNCMDoc *)GetDocument())->UpdateAllViews(NULL, N_OGL);
	}
	if (pDoc->Comparison.IsActive())
	{
		BPoint P, V;
		BPoint oPoint, oNormal;
		GetMouseRay(point, P, V);
		RecalcStockPoint(P);
		RecalcStockPoint(V);
		if (pDoc->GetStaticStockPoint(P, V, oPoint, oNormal) == 0)
		{
			pDoc->Comparison.Proc1Distace(oPoint, V);
			pDoc->UpdateAllViews(NULL, N_OGL);
		}
		else
			pDoc->Comparison.ClearDistance();
	}
	if (pDoc->Measure.IsActivated())
	{
		if (IsViewInterfaceActive())
		{
			NOpenGLWnd::OnLButtonDown(nFlags, point);
			return;
		}
		MakeCurrent();
		BPoint View(GetViewDir());
		double wz, x, y, z;
		GetMouseCoord(point, &wz, &x, &y, &z);
		if (wz == 0.0)// Point is outside the picture
			pDoc->Measure.SetDefault();
		else
		{
			BPoint P(x, y, z, 1.);
			RecalcStockPoint(P);
			RecalcStockPoint(View);
			bool PointOnSection = false;
			bool PointNerarerSection = false;
			NStockSection *pSection = pDoc->GetStockSection();
			if (pSection)
			{
				if (pSection->IsActive())
				{
					if (pSection->ContainsP(P, 0.05))
					{
						PointOnSection = true;
					}
					else if (pSection->CutsOffP(P, View))
					{
						PointNerarerSection = true;
					}
				}
			}
			else
				P = P - View * 1000.;

			if (!PointNerarerSection)
			{
				if (PointOnSection)
					pDoc->Measure.CalcManual(P, pSection->GetN());
				else
					pDoc->Measure.SetPoint(P, View, AUTO_M);
			}
			else
				pDoc->Measure.SetDefault();

		}
		RestoreContext();
		pDoc->UpdateAllViews(NULL, N_OGL | N_STATE);
	}
	if (pDoc->Distance.IsActive())
	{
		pDoc->Distance.SetClickPoint();
		pDoc->UpdateAllViews(NULL, N_STATE | N_OGL);
	}


	BOpenGLWndVD::OnLButtonUp(nFlags, point);
}

void NOpenGLWnd::OnMouseLeave()
{
	HighlightedCadr.Set(undef, 0, 0, 0, 0, 0, 0);
	HighlightedPoint.Clear();

	BOpenGLWndVD::OnMouseLeave();
}

BOOL NOpenGLWnd::PreTranslateMessage(MSG* pMsg)
{
	if(InfoTip.m_hWnd != nullptr)
		InfoTip.RelayEvent(pMsg);

	return BOpenGLWnd::PreTranslateMessage(pMsg);
}

void NOpenGLWnd::GetHighPoint(BPoint& P) const
{
	if (HighlightedCadr.IsUndef())
		BOpenGLWnd::GetHighPoint(P);
	else
		P = HighlightedCadr.GetB();
}

void NOpenGLWnd::NInfoTip::EnsureCreated(CWnd* pWnd)
{
	if (Created)
		return;
	Created = true;
	Create(pWnd, TTS_ALWAYSTIP);
	AddTool(pWnd, "");
	Activate(FALSE);
	SetDelayTime(0);
	SetDelayTime(TTDT_RESHOW, 0);
}
