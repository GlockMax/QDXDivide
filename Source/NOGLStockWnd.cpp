#include "stdafx.h"
#include "NCM.h"
#include "NCMDoc.h"
#include "NStockSection.h"
#include "NCUnit.h"
#include "ResLocal.h"
#include "MainFrm.h"
#include "MTMashineTool.h"
#include "NProgram.h"
#include "NToolCombined.h"
#include "MQuadTreeRot.h"
#include "GDynamicCadr.h"
#include "BObjectManager.h"
#include "NRemoteManager.h"
#include "MTConfig.h"
#include "NOGLStockWnd.h"

IMPLEMENT_DYNCREATE(NOGLStockWnd, NOpenGLWnd)

BEGIN_MESSAGE_MAP(NOGLStockWnd, NOpenGLWnd)
	//{{AFX_MSG_MAP(NOGLStockWnd)
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

extern CNCMApp theApp;

NOGLStockWnd::NOGLStockWnd(void)
{
}

NOGLStockWnd::~NOGLStockWnd(void)
{
}

void NOGLStockWnd::NUpdate(const CRect *ClipRect)
{

	if(!((CPane *)GetParent())->IsPaneVisible())
		return;
	//HighlightedCadr.Set(undef,0,0,0,0,0,0);
	//HighlightedPoint.Clear();
	NOpenGLWnd::NUpdate();
}

void NOGLStockWnd::DrawScene(RenderMode GlobRMode)
{
	MakeCurrent();
	IsOpenGLErr("DrawSceneStock");
	CNCMDoc* pDoc = (CNCMDoc*)GetDocument();
	if (!pDoc)
		return;
	if (pDoc->IsUpdateBlocked())
		return;
	if (!pDoc->GetUnit())
		return;

	DrawRotCenter();

	pDoc->SetClipPlane();

	MTMachineTool& Mach = pDoc->GetMachTool();
	StockMatr = Mach.GetStockMovedMatr(0, true) * Mach.GetStockMatr(0, true).invr();
	glPushMatrix();
	glMultMatrixd(StockMatr.GetArray());
	// Draw comparision
	pDoc->Comparison.Draw();
	// Draw Stock
	IsOpenGLErr("StockView0");
	pDoc->DrawStockWSect(GlobRMode, true);
	DrawHighlightedCadr(true);
	// Draw Part
	IsOpenGLErr("StockView01");
	pDoc->DrawParts(GlobRMode, true);
	// Draw Fixture
	IsOpenGLErr("StockView02");
	pDoc->DrawFixture(GlobRMode, true);
	IsOpenGLErr("StockView03");
	// Draw position dialog
	if (GlobRMode != RM_TRANSLUCENT)
		((CMainFrame*)AfxGetMainWnd())->DrawPosDlg();
	glPopMatrix();
	//Draw sections
	if (GlobRMode != RM_TRANSLUCENT)
	{
		if (NCMComData::GetType() == AT_WELD)
		{
			pDoc->DrawToolSection();
		}
	}
	// Draw pick arrow
	IsOpenGLErr("StockView04");
	if (GlobRMode != RM_TRANSLUCENT)
	{
		NRemoteManager* pRC = ((CMainFrame*)AfxGetMainWnd())->GetRemoteControl();
		if (pRC)
			if (pRC->IsWaitingPoint() && GlobRMode != RM_TRANSLUCENT)
			{
				Shader::ShaderOFF();
				pRC->GetPickArrow().Draw(true);
			}
	}
	// for all channels
	const auto pBufChannel = pDoc->GetCurChannel();
	for (int CInd = 0; pDoc->SetNextChannel(CInd);)
	{
		const NToolCombined* pActTool = pDoc->GetActiveTool();
		int Tur = 0;
		int Att = 0;
		if (pActTool)
		{
			Tur = pActTool->Turret;
			Att = pActTool->Attach;
		}
		// Draw Program List!!
		if (GlobRMode != RM_TRANSLUCENT)
		{
			Shader::ShaderOFF();
			DrawPrograms(false);
			if (pBufChannel == pDoc->GetCurChannel())
				DrawHighlightedCadr(false);
			// Draw Axis
			double Brightness = IsActive() ? ActiveBrightness : InactiveBrightness;
			pDoc->GetCurChannel()->DrawAxes(true, Brightness, Tur, Att);
			Shader::RestoreState();
		}
		IsOpenGLErr("StockView1");
		// Draw active tool
		if (pActTool)
		{
			glPushMatrix();
			BMatr S2TMatr = Mach.GetStock2ToolMatr(0, Tur, Att);
			glMultMatrixd(S2TMatr.GetArray());
			pDoc->DrawTool(GlobRMode, pActTool);
			glPopMatrix();
		}
	}
	pDoc->RemoveClipPlane();
	if (GlobRMode != RM_TRANSLUCENT)
	{
		Shader::ShaderOFF();
		if (pDoc->GetSnaps().GetFound())
		{
			glPushAttrib(GL_ENABLE_BIT);
			glDepthMask(false);
			pDoc->GetSnaps().DrawLastPoint();// snaps in stock CS
			glDepthMask(true);
			glPopAttrib();
		}
		//Draw dimensions
		pDoc->Dimensions.Draw(true);
		Shader::RestoreState();
	}

	//Draw measure
	glPushMatrix();
	glMultMatrixd(StockMatr.GetArray());
	if (GlobRMode != RM_TRANSLUCENT)
	{
		Shader::ShaderOFF();
		pDoc->Measure.Draw();
	}
	IsOpenGLErr("StockView2");
	glPopMatrix();
	NOpenGLWnd::DrawScene(GlobRMode);
	RestoreContext();
}


BBox NOGLStockWnd::GetSceneGabar(bool VisibleOnly)
{
	CNCMDoc* pDoc = (CNCMDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	BBox Res;
	CMapStringToNProgram *pPList = pDoc->NGetProgramList();
	if (!(VisibleOnly && !pPList->Visible))
	{
		for(int j = 0; j < pPList->GetMainProgCount(); ++j)
		{
			CString Key;
			NProgram *pProg = NULL;
			pPList->GetAt(j, Key, (CObject *&)pProg);
			if(pProg)
				if (!(VisibleOnly && !pProg->IsVisible()))
					Res.Expand(pProg->GetGabar());
		}
	}
	for each(const auto& el in *pDoc->NGetToolList())
	{
		NToolID key = el.first;
		NToolCombined* pTool = static_cast<NToolCombined*>(el.second);
		if ((!(VisibleOnly && !NTool::IsShapeVisible())
			&& pDoc->GetUnit()->GetState()->CurToolN == key.GetToolStr()
			&& pDoc->GetUnit()->GetState()->CurTurret == key.GetTurretNum()))
		{
			MTMachineTool &Mach = pDoc->GetMachTool();
			BBox Gabar = pTool->GetGabar();
			if(Mach.GetCurConfig().IsDefined())
			{
				BMatr M = Mach.GetStock2ToolMatr(0, pTool->Turret, pTool->Attach);
				Gabar.Transform(M);
			}
			Res.Expand(Gabar);
		}
	}
	if(pDoc->IsThereAnyStock())
	{
		auto list = pDoc->NGetStockList();
		const auto GetG = theApp.IsInCuttingMode() ? &NStock::GetGabarRender : &NStock::GetGabar;
		for each (const auto & el in *list)
		{
			CString key = el.first;
			NStock* pS = static_cast<NStock*>(el.second);
			if(pS)
			{
				if (!pS->Visible)
					continue;
				BBox Tmp = (pS->*GetG)();
				Tmp.Transform(StockMatr);
				Res.Expand(Tmp);
			}					
		}
	}
	for each (const auto & el in *pDoc->NGetPartList())
	{
		CString key = el.first;
		NStock* pS = static_cast<NStock*>(el.second);
		if(pS->IsVisible())
			Res.Expand(pS->GetGabar());
	}

	if (pDoc->IsThereAnyStock())
	{
		CMapStringToNStock *list = pDoc->NGetStockList();
		if (!(VisibleOnly && !list->IsVisible()))
		{
			for each (const auto & el in *list)
			{
				CString key = el.first;
				NStock* pS = static_cast<NStock*>(el.second);
				if (pS != nullptr)
				{
					if (!(VisibleOnly && !pS->Visible))
						Res.Expand(pS->GetGabar());
				}
			}
		}
	}
	for each (const auto & el in *pDoc->NGetPartList())
	{
		NStock* pS = static_cast<NStock*>(el.second);
		if(pS != nullptr)
			if (!(VisibleOnly && !pS->IsVisible()))
				Res.Expand(pS->GetGabar());
	}

	for each (const auto & el in *pDoc->NGetExObjList())
	{
		NStock* pS = static_cast<NStock*>(el.second);
		if(pS != nullptr)
			if(pS->IsVisible())
				Res.Expand(pS->GetGabar());
	}

    if (pDoc->Dimensions.IsActivatedForDraw())
        Res.Expand(pDoc->Dimensions.GetGabar());

	if(!Res.IsDefined())
	{
		Res.Expand(100.,100.,20.);
		Res.Expand(0.,0.,0.);
	}
	else
		Res.Offset(1.,1.,1.);
//	Res.Transform(StockMatr);

	return Res;
}

void NOGLStockWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (Divide1Stock(point, BMatrPair()))
		NOpenGLWnd::OnLButtonDown(nFlags, point);
}


void NOGLStockWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	
	
	NOpenGLWnd::OnMouseMove(nFlags, point);
}


void NOGLStockWnd::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint) 
{
	NOpenGLWnd::OnUpdate(pSender, lHint, pHint);
}

void NOGLStockWnd::DrawScene4Pick(int Type)
{
	MakeCurrent();
	BaseRender::PhShader.Disable();
	IsOpenGLErr("DrawSceneStockP");
	CNCMDoc* pDoc = (CNCMDoc*)GetDocument();
	if(!pDoc)
		return;
	if(pDoc->IsUpdateBlocked())
		return;
	if(!pDoc->GetUnit())
		return;
	MTMachineTool& Mach = pDoc->GetMachTool();
	Mach.MakeActual();
	BBox StockGabar = pDoc->GetModifStocksGabar();
	const NToolCombined* pActTool = pDoc->GetActiveTool();
	// Draw active tool
	const NMapStringToNTool& TList = *pDoc->NGetToolList();
	if (Type & PT_MACHINE && pActTool)
	{
		glPushName(OG_TOOLS);
		const int TurretNum = pActTool->Turret;
		const int AttachNum = pActTool->Attach;
		BMatr S2TMatr = Mach.GetStock2ToolMatr(0, TurretNum, AttachNum);
		glPushMatrix();
		glMultMatrixd(S2TMatr.GetArray());
		glPushName(TList.GetIndex(pActTool));
		pDoc->DrawTool(RM_UNDEF, pActTool);
		glPopName();
		glPopMatrix();
		glPopName();
	}
	StockMatr = Mach.GetStockMovedMatr(0, true) * Mach.GetStockMatr(0, true).invr();
	glPushMatrix();
	glMultMatrixd(StockMatr.GetArray());
	// Draw Stock
	if((pDoc->NGetStockList()->GetStocksState() == AT_TURN && Type != PT_DIMENSIONS) || Type & PT_STFIPA)
		pDoc->DrawStockWSect(RM_UNDEF, true);
	if (Type & PT_STFIPA)
	{
// Draw Part
		pDoc->DrawParts(RM_UNDEF, true);
// Draw Fixture
		pDoc->DrawFixture(RM_UNDEF, true);
	}
	glPopMatrix();
	if (Type & PT_UCS)
	{
		// Draw axes
		int Tur = 0;
		int Att = 0;
		if (pActTool)
		{
			Tur = pActTool->Turret;
			Att = pActTool->Attach;
		}
		glPushMatrix();
		Shader::ShaderOFF();
		glMultMatrixd(StockAxisMatr.GetArray());
		double Brightness = IsActive() ? ActiveBrightness : InactiveBrightness;
		pDoc->GetCurChannel()->DrawAxes(false, Brightness, Tur, Att);
		Shader::RestoreState();
		glPopMatrix();
	}
	if (Type & PT_DIMENSIONS)
		pDoc->Dimensions.Draw();
	BaseRender::PhShader.Enable();
	RestoreContext();
}

void NOGLStockWnd::DoZoomTool()
{
	ClearZoomActive();

	CNCMDoc* pDoc = (CNCMDoc*)GetDocument();
	if (!pDoc)
		return;
	MTMachineTool& Mach = pDoc->GetMachTool();
	const NToolCombined* pActTool = pDoc->GetActiveTool();
	if (pActTool == nullptr)
		return;
	BMatr S2TMatr = Mach.GetStock2ToolMatr(0, pActTool->Turret, pActTool->Attach);
	BPoint ToolDir = BPoint(0., 0., -1., 0.) * S2TMatr;
	if (ViewHistory.Add(GetModelMatr()))
		GetDocument()->SetModifiedFlag();
	CongVect(BPoint(0., 0., 0., 1.), GetViewDir(), ToolDir);
	DoZoomAll();
}

RPickID NOGLStockWnd::GetPointOnModel(CPoint iCPoint, BPoint& oPoint, BPoint& oNormal)
{
	// output points in stock coordinate system
	RPickID Result;
	GetParent()->SetFocus();
	BPoint P, V;
	BPoint R, N;
	GetMouseRay(iCPoint, P, V);
	RecalcStockPoint(P);
	RecalcStockPoint(V);
	CNCMDoc* pDoc = (CNCMDoc*)GetDocument();
	pDoc->GetPartsPoint(P, V, oPoint, oNormal, Result);
	if (Result.GetSize() == 0)
    {
		GLint viewport[4];
		GLdouble projMatrix[16], modelMatrix[16];
		glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
		glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
		glGetIntegerv(GL_VIEWPORT, viewport);

		double x, y, z;
		gluUnProject(iCPoint.x, viewport[3] - iCPoint.y, 0.0, modelMatrix, projMatrix, viewport, &x, &y, &z);
		oPoint.SetX(x);
		oPoint.SetY(y);
		oPoint.SetZ(z);
		oPoint.SetH(1.0);
		Result.Set1(OG_NO_GROUP);
		return Result;
	}

    return Result;
}



void NOGLStockWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	NOpenGLWnd::OnLButtonUp(nFlags, point);
}
