#include "stdafx.h"
#include "math.h"
#include "NCM.h"
#include "MainFrm.h"
#include "NCMDoc.h"
#include "NCUnit.h"
#include "NToolCombined.h"
#include "MTMashineTool.h"
#include "NProgram.h"
#include "NOMScaner.h"
#include "NCMProject.h"
#include "NRemoteManager.h"
#include "NStockSection.h"
#include "NRotTracker.h"
#include "NToolCombinedTurn.h"
#include "MTConfig.h"
#include "NOGLMashWnd.h"

IMPLEMENT_DYNCREATE(NOGLMashWnd, NOpenGLWnd)

BEGIN_MESSAGE_MAP(NOGLMashWnd, NOpenGLWnd)
	//{{AFX_MSG_MAP(NOGLMashWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

extern CNCMApp theApp;

NOGLMashWnd::NOGLMashWnd(void)
{
}

NOGLMashWnd::~NOGLMashWnd(void)
{
}

void NOGLMashWnd::NUpdate(const CRect *ClipRect)
{
	NOpenGLWnd::NUpdate();
}

void NOGLMashWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(Divide1Stock(point, BMatrPair(StockMatr)))
		NOpenGLWnd::OnLButtonDown(nFlags, point);
}

void NOGLMashWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	NOpenGLWnd::OnLButtonDblClk(nFlags, point);
	if(NCM_PROJECT.GetGlobalState().GetEditMode() == NGlobalState::Edit_Mach)
	{
		MakeCurrent();
		RPickID ObjID = Pick(point, PT_MACHINE, SelectionRegionSize);
		if(ObjID.GetGroup() == OG_MACHINE)
		{
			CNCMDoc* pDoc = (CNCMDoc*)GetDocument();
			pDoc->MachToolEditor.OnPick(ObjID.GetObjName());
		}
		RestoreContext();
	}
}
void NOGLMashWnd::DrawScene(RenderMode GlobRMode)
{
	MakeCurrent();
	IsOpenGLErr("DrawSceneMachWnd");
	CNCMDoc* pDoc = (CNCMDoc*)GetDocument();
	if(!pDoc)
		return;
	if(pDoc->IsUpdateBlocked())
		return;
	if(!pDoc->GetUnit())
		return;
	
//DrawMashineTool
	MTMachineTool &Mach = pDoc->GetMachTool();
	//NStock *pStock = pDoc->GetStock();
	BBox StockGabar = pDoc->GetModifStocksGabar();
	for (int CInd = 0; pDoc->SetNextChannel(CInd);)
	{
		const NToolCombined* pActTool = pDoc->GetActiveTool();
		if (pActTool)
		{
			Mach.SetComp(*pDoc->GetUnit(), pActTool->GetGabar().Zmax, pActTool->GetCompVect(), pActTool->Turret, pActTool->Attach, StockGabar);
		}
		else
		{
			Mach.SetComp(*pDoc->GetUnit(), 0., BPoint(0., 0., 0., 0.), 0, 0, StockGabar);
		}
	}
	StockMatr = Mach.GetStockMovedMatr(0, true);
	const NToolCombined *pActTool = pDoc->GetActiveTool();
	Mach.SetDrawParams(true, true, true);
	if(NCM_PROJECT.GetGlobalState().GetEditMode() != NGlobalState::Edit_no)
	{
		switch(NCM_PROJECT.GetGlobalState().GetEditMode())
		{
		case NGlobalState::Edit_Mach:
			Mach.SetState(BMTLinkage().SetIndW1(), pDoc->MachToolEditor.GetEditMachState());
			Mach.MakeActual();
			break;
		case NGlobalState::Edit_Extern:
			//if(((CMainFrame *)AfxGetMainWnd())->pRemoteControl
			//	Mach.SetState(pDoc->MachToolEditor.GetEditMachState());
			break;
		}
	}
	
	glPushMatrix();
	glMultMatrixd(StockMatr.GetArray());// For SetClipPlane works properly
	pDoc->SetClipPlane();
	glPopMatrix();

	if (pDoc->GetSectionActive())
		if(pDoc->GetStockSection()->NeedCutAll())
			pDoc->GetStockSection()->EnableClipPlane();
	Mach.Draw(GlobRMode, true);
	IsOpenGLErr("Debug2");
	if (pDoc->GetSectionActive())
		pDoc->GetStockSection()->DisableClipPlane();

	DrawRotCenter();
	glPushMatrix();
	glMultMatrixd(StockMatr.GetArray());
// Draw comparision
	pDoc->Comparison.Draw();
// Draw Stock
	IsOpenGLErr("DrawStockWSect0");
	pDoc->DrawStockWSect(GlobRMode, true);
	DrawHighlightedCadr(true);
	IsOpenGLErr("DrawStockWSect1");
	// Draw Part
	pDoc->DrawParts(GlobRMode, true);
	// Draw Fixture
	pDoc->DrawFixture(GlobRMode, true);
	// Draw position dialog
	if (GlobRMode != RM_TRANSLUCENT)
		((CMainFrame*)AfxGetMainWnd())->DrawPosDlg();
	//Draw sections
	if(GlobRMode != RM_TRANSLUCENT)
	{
		if(NCMComData::GetType() == AT_WELD)
		{
			pDoc->DrawToolSection();
		}
	}
// Draw pick arrow
	if(GlobRMode != RM_TRANSLUCENT)
	{
		NRemoteManager *pRC = ((CMainFrame *)AfxGetMainWnd())->GetRemoteControl();
		if(pRC)
			if(pRC->IsWaitingPoint() && GlobRMode != RM_TRANSLUCENT)
			{
				Shader::ShaderOFF();
				pRC->GetPickArrow().Draw(true);
			}
	}
	
//Draw measure
	if (GlobRMode != RM_TRANSLUCENT)
	{
		Shader::ShaderOFF();
		pDoc->Measure.Draw();
	}
	glPopMatrix();// StockMatr

	// Draw active tool
	NMapStringToNTool& TList = *pDoc->NGetToolList();
	CString SelectedKey = TList.GetSelectedKey();
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
		Mach.GetToolAndStockMovedMatr(ToolMatr, StockMatr, 0, Tur, Att, true);
		// Draw Program List
		if (GlobRMode != RM_TRANSLUCENT)
		{
			glPushMatrix();
			Shader::ShaderOFF();
			StockAxisMatr = Mach.GetStockMatr(0, true);
			glMultMatrixd(StockAxisMatr.GetArray());
			DrawPrograms(false);
			if(pBufChannel == pDoc->GetCurChannel())
				DrawHighlightedCadr(false);
			// Draw axes
			double Brightness = IsActive() ? ActiveBrightness : InactiveBrightness;
			pDoc->GetCurChannel()->DrawAxes(false, Brightness, Tur, Att);
			Shader::RestoreState();
			glPopMatrix();
		}

		// Draw active tool
		if (pActTool)
		{
			IsOpenGLErr("Debug1");
			glPushMatrix();
			glMultMatrixd(ToolMatr.GetArray());
			pDoc->DrawTool(GlobRMode, pActTool);
			if (!SelectedKey.IsEmpty())
			{
				NToolID ToolID;
				ToolID.Set(pActTool->Position, pActTool->Turret);
				if (ToolID == SelectedKey)
					pActTool->DrawGabar(GlobRMode);
			}
			glPopMatrix();
		}
	}
	// Draw other tools
 //	if(GlobRMode != RM_TRANSLUCENT)
	{
		if(Mach.GetDrawAllTools() && Mach.IsVisible())
		{
			for each (const auto & el in TList)
			{
				CString key = el.first;
				NToolCombined* pTool = static_cast<NToolCombined*>(el.second);;
				glPushMatrix();
				if(pTool && pTool != pActTool)
				{
					BOOL BufVis = NTool::IsShapeVisible();
					if(!BufVis)
						NTool::FlipShapeDraw();
					bool Found;
					BMatr PlaceM = pTool->GetCompMatr() * Mach.GetToolPlaceMatr(pTool->Position, pTool->Turret, pTool->Attach, &Found);
					glMultMatrixd(PlaceM.GetArray());
					if (Found)
					{
						pDoc->DrawTool(GlobRMode, pTool);
						if (key == SelectedKey)
							pTool->DrawGabar(GlobRMode);

					}
					if(!BufVis)
						NTool::FlipShapeDraw();
				}
				glPopMatrix();
			}
		}
	}
	pDoc->RemoveClipPlane();
	if (GlobRMode != RM_TRANSLUCENT)
	{
		glPushMatrix();
		Shader::ShaderOFF();
		glMultMatrixd(StockAxisMatr.GetArray());
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
		glPopMatrix();
	}

	NOpenGLWnd::DrawScene(GlobRMode);

	IsOpenGLErr("MashView2");
	RestoreContext();
}
BBox NOGLMashWnd::GetSceneGabar(bool VisibleOnly)
{
	if(!GetDocument())
		return BBox();
	CNCMDoc* pDoc = (CNCMDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	BBox Res;
	MTMachineTool &Mach = pDoc->GetMachTool();
	if(Mach.GetCurConfig().IsDefined())
	{
		CMapStringToNProgram *pPList = pDoc->NGetProgramList();
		if (!(VisibleOnly && !pPList->Visible))
		{
			for (int i = 0; i < pPList->GetMainProgCount(); ++i)
			{
				CString Key;
				NProgram *pProg = NULL;
				pPList->GetAt(i, Key, (CObject *&)pProg);
				if (!(VisibleOnly && !pProg->IsVisible()))
				{
					BBox B = pProg->GetGabar();
					B.Transform(Mach.GetStockMatr(0, true));// to change
					Res.Expand(B);
				}
			}
		}
		for each (const auto & el in *pDoc->NGetToolList())
		{
			NToolID key = el.first;
			NToolCombined* pTool = static_cast<NToolCombined*>(el.second);
			if ((!(VisibleOnly && !NTool::IsShapeVisible())
				&& pDoc->GetUnit()->GetState()->CurToolN == key.GetToolStr()
				&& pDoc->GetUnit()->GetState()->CurTurret == key.GetTurretNum()))
			{
				BBox ToolGabar = pTool->GetGabar();
				ToolGabar.Transform(Mach.GetToolMatr(pTool->Turret, pTool->Attach, true));
				Res.Expand(ToolGabar);
			}
		}
		BMatr StMatr = Mach.GetStockMovedMatr(0, true);
		BBox StockG;
		if(pDoc->IsThereAnyStock())
		{
			CMapStringToNStock *list = pDoc->NGetStockList();
			if (!(VisibleOnly && !list->IsVisible()))
			{
				const auto GetG = theApp.IsInCuttingMode() ? &NStock::GetGabarRender : &NStock::GetGabar;
				for each (const auto & el in *list)
				{
					CString key = el.first;
					NStock* pS = static_cast<NStock*>(el.second);;
					if (pS != nullptr)
					{
						if (!(VisibleOnly && !pS->Visible))
						{
							BBox G = (pS->*GetG)();
							int StPos = pS->GetStockGr().Get();
							if (StPos == 0)
								G.Transform(StMatr);
							else
								G.Transform(Mach.GetStockMovedMatr(StPos, true));
							StockG.Expand(G);
						}
					}
				}
			}
		}
		BBox TmpBox;
		for each (const auto & el in *pDoc->NGetPartList())
		{
			CString key = el.first;
			NStock* pS = static_cast<NStock*>(el.second);;
			if (pS != nullptr)
				if (!(VisibleOnly && !pS->IsVisible()))
					TmpBox.Expand(pS->GetGabar());
		}

		for each (const auto & el in *pDoc->NGetExObjList())
			{
				CString key = el.first;
				NStock* pS = static_cast<NStock*>(el.second);;
				if (!(VisibleOnly && !pS->IsVisible()))
				TmpBox.Expand(pS->GetGabar());
		}
		TmpBox.Transform(StMatr);
		Res.Expand(TmpBox);
		Res.Expand(StockG);

		if (!(VisibleOnly && !Mach.IsVisible()))
			Res.Expand(Mach.GetGabar());
		// virtual tools
		if (Mach.GetDrawAllTools() && Mach.IsVisible())
		{
			for each (const auto & el in *pDoc->NGetToolList())
			{
				NToolID key = el.first;
				NToolCombined* pTool = static_cast<NToolCombined*>(el.second);
				if (pTool)
				{
					bool Found;
					BMatr PlaceM = pTool->GetCompMatr() * Mach.GetToolPlaceMatr(pTool->Position, pTool->Turret, pTool->Attach, &Found);
					if (Found)
					{
						if (pTool->IsKindOf(RUNTIME_CLASS(NToolCombinedTurn)))
							PlaceM = BMatr().RotY(0., 0., 0., -90.) * BMatr().RotZ(0., 0., 0., -90.) * PlaceM;
						BBox Box = pTool->GetGabar();
						Box.Transform(PlaceM);
						Res.Expand(Box);
					}
				}
			}
		}
	}
    
    if (pDoc->Dimensions.IsActivatedForDraw())
        Res.Expand(pDoc->Dimensions.GetGabar());

	if(!Res.IsDefined())
	{
		Res.Expand(100.,100.,100.);
		Res.Expand(0.,0.,0.);
	}
	return Res;
}

void NOGLMashWnd::DrawScene4Pick(int Type)
{
	MakeCurrent();
	BaseRender::PhShader.Disable();
	IsOpenGLErr("DrawSceneMachWnd");
	CNCMDoc* pDoc = (CNCMDoc*)GetDocument();
	if(!pDoc)
		return;
	if(pDoc->IsUpdateBlocked())
		return;
	if(!pDoc->GetUnit())
		return;

    //DrawMachineTool
	MTMachineTool &Mach = pDoc->GetMachTool();
	BBox StockGabar = pDoc->GetModifStocksGabar();
	const NToolCombined *pActTool = pDoc->GetActiveTool();
	if(pActTool)
		Mach.SetComp(*pDoc->GetUnit(), pActTool->GetGabar().Zmax, pActTool->GetCompVect(), pActTool->Turret, pActTool->Attach, StockGabar);
	else
		Mach.SetComp(*pDoc->GetUnit(), 0., BPoint(0., 0., 0., 0.), 0, 0, StockGabar);
	Mach.MakeActual();
	if (Type & PT_MACHINE || Type == PT_DIMENSIONS)
	{
		Mach.SetDrawParams(true, true, true);
		Mach.Draw(RM_UNDEF, (Type == PT_DIMENSIONS) ? false : true);
	}
// Draw active tool
	NMapStringToNTool& TList = *pDoc->NGetToolList();
	if ((Type & PT_MACHINE) && pActTool)
	{
		glPushName(OG_TOOLS);
		glPushMatrix();
		glMultMatrixd(ToolMatr.GetArray());
		glPushName(TList.GetIndex(pActTool));
		pDoc->DrawTool(RM_UNDEF, pActTool);
		glPopName();
		glPopMatrix();
		glPopName();
	}
// Draw other tools
	if ((Type & PT_MACHINE) && Mach.GetDrawAllTools() && Mach.IsVisible())
	{
		glPushName(OG_TOOLS);
		for each (const auto & el in TList)
		{
			NToolID key = el.first;
			NToolCombined* pTool = static_cast<NToolCombined*>(el.second);
			glPushMatrix();
			if (pTool && pTool != pActTool)
			{
				const BOOL BufVis = NTool::IsShapeVisible();
				if (!BufVis)
					NTool::FlipShapeDraw();
				bool Found;
				BMatr PlaceM = pTool->GetCompMatr() * Mach.GetToolPlaceMatr(pTool->Position, pTool->Turret, pTool->Attach, &Found);
				glMultMatrixd(PlaceM.GetArray());
				if (Found)
				{
					glPushName(TList.GetIndex(key));
					pDoc->DrawTool(RM_UNDEF, pTool);
					glPopName();
				}
				if (!BufVis)
					NTool::FlipShapeDraw();
			}
			glPopMatrix();
		}
		glPopName();
	}

	StockMatr = Mach.GetStockMovedMatr(0, true);
	glPushMatrix();
	glMultMatrixd(StockMatr.GetArray());
	// Draw Dimensions
	if(Type & PT_DIMENSIONS)
		pDoc->Dimensions.Draw();
	// Draw Stock
	if (pDoc->NGetStockList()->GetStocksState() == AT_TURN || Type & PT_STFIPA)
		pDoc->DrawStockWSect(RM_UNDEF, (Type == PT_DIMENSIONS) ? false : true);
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

const BPoint NOGLMashWnd::GetRotBase(void) const
{
	return RotCenter.RotBasePoint.IsPoint() ? RotCenter.RotBasePoint * StockMatr : (ZoomRActive == NULL ? BPoint(0., 0., 0., -1.) : ZoomRActive->GetBase());
}

void NOGLMashWnd::GetMouseCoord(const CPoint &point, double *wz, double * x, double * y, double * z)
{
	MakeCurrent();
	if (HighlightedPoint.IsDefined())
	{
		GLint viewport[4];
		GLdouble projMatrix[16], modelMatrix[16];
		glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
		glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
		glGetIntegerv(GL_VIEWPORT, viewport);
		HighlightedPoint.Get(*x, *y, *z);
		double wxv, wyv;
		gluProject(*x, *y, *z, modelMatrix, projMatrix, viewport, &wxv, &wyv, wz);
		return;
	}
	BPoint R(0., 0., 0., 1.), N;
	RPickID PickID = GetPointOnModel(point, R, N);
	if (PickID.GetSize() == 0 || PickID.GetGroup() == OG_NO_GROUP)
		*wz = 0.0;
	else
		*wz = 0.6;
	if(*wz != 0.)
		R *= StockMatr;// GetPointOnModel returns R & N in stock CS
	*x = R.GetX();
	*y = R.GetY();
	*z = R.GetZ();
}

RPickID NOGLMashWnd::GetPointOnModel(CPoint iCPoint, BPoint& oPoint, BPoint& oNormal)
{
	// output points in stock coordinate system
	RPickID Result;
	if(GetFocus() != this)
		GetParent()->SetFocus();
	BPoint P, V;
	BPoint R, N;
	GetMouseRay(iCPoint, P, V);
	CNCMDoc* pDoc = (CNCMDoc*)GetDocument();
    std::vector<BPoint> pts, dirs;
	RPickID Result0;
    pDoc->GetMachPoint(P, V, oPoint, oNormal, Result0);
    if (Result0.GetSize() != 0)
    {
        pts.push_back(oPoint * StockMatr.invr());
        dirs.push_back(oNormal * StockMatr.invr());
    }
	RecalcStockPoint(P);
	RecalcStockPoint(V);
	RPickID Result1;
	pDoc->GetPartsPoint(P, V, oPoint, oNormal, Result1);
	if (Result1.GetSize() != 0)
	{
		if (pts.size() == 0)
			Result0 = Result1;
		pts.push_back(oPoint);
        dirs.push_back(oNormal);
    }
    
    if (pts.empty())
    {
        GLint viewport[4];
		GLdouble projMatrix[16],modelMatrix[16];
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
    else if (pts.size() == 2)
    {
        double cos = GetViewDir() * (pts[1] - pts[0]);
        if (cos < 0.0)
        {
            oPoint = pts[1];
            oNormal = dirs[1];
			Result = Result1;
        }
        else
        {
            oPoint = pts[0];
            oNormal = dirs[0];
			Result = Result0;
		}
    }
    else
    {
        oPoint = pts[0];
        oNormal = dirs[0];
		Result = Result0;
	}

    return Result;
}

void NOGLMashWnd::DoZoomTool()
{
	ClearZoomActive();

	CNCMDoc* pDoc = (CNCMDoc*)GetDocument();
	if (!pDoc)
		return;
	MTMachineTool& Mach = pDoc->GetMachTool();
	const NToolCombined* pActTool = pDoc->GetActiveTool();
	if (pActTool == nullptr)
		return;
	BMatr S2TMatr = Mach.GetToolMatr(0, pActTool->Turret, pActTool->Attach);
	BPoint ToolDir = BPoint(0., 0., -1., 0.) * S2TMatr;
	if (ViewHistory.Add(GetModelMatr()))
		GetDocument()->SetModifiedFlag();
	CongVect(BPoint(0., 0., 0., 1.), GetViewDir(), ToolDir);
	DoZoomAll();
}
