#include "stdafx.h"
#include "math.h"
#include "NCMDoc.h"
#include "NCUnit.h"
#include "NToolCombined.h"
#include "MTMashineTool.h"
#include "NProgram.h"
#include "BObjectManager.h"
#include "NToolCombinedWeld.h"
#include "NProgramWeld.h"
#include "NOGLWSectWnd.h"

IMPLEMENT_DYNCREATE(NOGLWSectWnd, NOpenGLWnd)

BEGIN_MESSAGE_MAP(NOGLWSectWnd, NOpenGLWnd)
	//{{AFX_MSG_MAP(NOGLWSectWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

NOGLWSectWnd::NOGLWSectWnd(void)
{
}

NOGLWSectWnd::~NOGLWSectWnd(void)
{
}

void NOGLWSectWnd::NUpdate(const CRect *ClipRect)
{
	NOpenGLWnd::NUpdate();
}

void NOGLWSectWnd::DrawScene(RenderMode GlobRMode)
{
	IsOpenGLErr("DrawSceneWSect");
	CNCMDoc* pDoc = (CNCMDoc *)GetDocument();
	if(!pDoc)
		return;
	if(pDoc->IsUpdateBlocked())
		return;
	const NToolCombined *pTool = pDoc->GetActiveTool();
	glPushMatrix();
// Draw sections
	MTMachineTool &Mach = pDoc->GetMachTool();
	int Turret = 0, Attach = 0;
	BPoint ToolDir(-1., 0., 0., 0.);
	if(pTool)
	{
		Turret = pTool->Turret; 
		Attach =  pTool->Attach;
//		ToolDir = ((NToolCombinedWeld *)pTool)->GetDirection();
		ToolDir = BPoint(0., 0., -1., 0.);
	}
	BMatr ToolMatr = Mach.GetStock2ToolMatr(0, Turret, Attach);
	ToolDir = ToolDir * ToolMatr;
	BPoint View(pDoc->GetToolSection().GetN());
	glMultMatrixd(CalcViewMatrix(View).GetArray());
	pDoc->DrawToolSection();
// Draw penetration
	if(NTool::IsShapeVisible())
		DrawPenetration(false);

// Draw active tool
	glMultMatrixd(ToolMatr.GetArray());
	pDoc->DrawTool(GlobRMode, pTool);
	glPopMatrix();
	
	NOpenGLWnd::DrawScene(GlobRMode);
	IsOpenGLErr("DrawSceneWSect2");

}
BBox NOGLWSectWnd::GetSceneGabar(bool VisibleOnly)
{
	CNCMDoc* pDoc = (CNCMDoc*)GetDocument();
	if(!pDoc)
		return BBox();
	ASSERT_VALID(pDoc);

	BBox Res = pDoc->GetToolSection().GetGabar();
	if(!Res.IsDefined())
		Res.Expand(100.,100.,100.);
	Res.Expand(0.,0.,0.);
	return Res;
}

void NOGLWSectWnd::DrawScene4Pick(int Type)
{
}

const BMatr NOGLWSectWnd::CalcViewMatrix(const BPoint &View)
{
	BMatr Src;
	Src.SetRow(0, BPoint(0., 0., 0., 1.));
	Src.SetRow(1, View);
	BPoint X(BPoint(0., 0., 1., 0.) % View);
	double d2 = X.D2();
	if(d2 < 1.e-12)
		X.Set(1., 0., 0., 0.);
	else
		X = X * (1. / sqrt(d2)); 

	Src.SetRow(2, X);
	Src.SetRow(3, View % X);
	BMatr Dst;
	Dst.SetRow(0, BPoint(0., 0., 0., 1.));
	Dst.SetRow(1, BPoint(0., 0., 1., 0.));
	Dst.SetRow(2, BPoint(1., 0., 0., 0.));
	Dst.SetRow(3, BPoint(0., 1., 0., 0.));
	return Src.invr() * Dst;
}

void NOGLWSectWnd::DrawPenetration(bool Draw4Pick)
{
	CNCMDoc* pDoc = (CNCMDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc->IsThereAnyProgr())
		return;
	bool HasErrors = false;
	CMapStringToNProgram *pPList = pDoc->NGetProgramList();
	for(int i = 0; i < pPList->GetMainProgCount(); ++i)
	{
		CString Key;
		NProgram *pProg = NULL;
		pPList->GetAt(i, Key, (CObject *&)pProg);
		HasErrors = HasErrors || pProg->HaveErr();
	}
	if(HasErrors)
		return;

	double main_t = -1.;
	cadrID CadrID = pDoc->GetCurCadr(main_t);
	double t = main_t;

//	glPushAttrib(GL_ENABLE_BIT | GL_LINE_BIT);
	CString ActiveProgName = pDoc->NGetProgramList()->GetKey(CadrID.Prog);
	BOOL DoDraw = TRUE;// используется для отключения рисования непройденных программ
	for(int i = 0; i < pPList->GetMainProgCount(); ++i)
	{
		CString Key;
		NProgram *pProg = NULL;
		pPList->GetAt(i, Key, (CObject *&)pProg);

		((NProgramWeld *)pProg)->DrawPenetration(CadrID.Cadr, main_t);
	}
}