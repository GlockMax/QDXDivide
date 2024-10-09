#include "stdafx.h"
#include "NCMProject.h"
#include "math.h"
#include "MBody.h"
#include "MBSPTree.h"
#include "MBSPNode.h"
#include "NStockSectionDlg.h"
#include "GQuadRenderRot.h"
#include "nstocksection.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

NStockSection::NStockSection(CWnd *pParent) :
N(1., 0., 0., 0.), P(0., 0., 0., 1.)
{
	Active = false;
	WaitPoint = false;
	FillSection = true;
	CutAll = true;
	pDlg = new NStockSectionDlg(pParent, this);
	pDlg->Create(pDlg->IDD, pParent);
	NColor *c;
	if(NCM_PROJECT.Colors.Lookup("Сечение",c))
		Color=*c;
	else
		Color.SetRGB(RGB(0,0,255));
}

NStockSection::~NStockSection(void)
{
	delete pDlg;
}

void NStockSection::RemoveClipPlane(void)
{
	LocalCut.CloseCPlane();
}

void NStockSection::SetClipPlane(bool ClipNear)
{
	GLint viewport[4];
	GLdouble projMatrix[16],modelMatrix[16];
	glMatrixMode(GL_MODELVIEW);
	glGetDoublev(GL_MODELVIEW_MATRIX ,modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX ,projMatrix);
	glGetIntegerv(GL_VIEWPORT ,viewport);
	// check N direction in window space to ensure its direction to the viewer
	GLdouble x,y,zs,ze;
	gluProject(P.GetX(),P.GetY(),P.GetZ(),modelMatrix, projMatrix,viewport,&x,&y,&zs); 
	gluProject(P.GetX()+N.GetX(),P.GetY()+N.GetY(),P.GetZ()+N.GetZ(),modelMatrix, projMatrix,viewport,&x,&y,&ze); 
	double sign = ((ze < zs) && ClipNear) ? -1. : 1.;
	GLdouble equation[4] = { sign * N.GetX(), sign * N.GetY(), sign * N.GetZ(), -sign * (N*P)};
	LocalCut.SetCPlane(equation);
}

void NStockSection::EnableClipPlane(void)
{
	glEnable(GL_CLIP_PLANE5);
}

void NStockSection::DisableClipPlane(void)
{
	glDisable(GL_CLIP_PLANE5);
}

void NStockSection::DrawClipPlane(void)
{
	if (NeedFillSection())
		LocalCut.DrawCPlane(GetColor());
	LocalCut.EndDrawCPlane();
}

void NStockSection::Activate(const BBox &Gabar)
{
	// Change stock section view parameters
	GQuadRenderRot::SetFull();

	Active = true;
	StockGabar = Gabar;
	CalcVMinMax();
	pDlg->ResetSlider();
	pDlg->InvalidateView();
	pDlg->ShowWindow(SW_SHOW);
	NColor *c;
	if(NCM_PROJECT.Colors.Lookup("Сечение",c))
		Color=*c;
	else
		Color.SetRGB(RGB(0,0,255));
}

void NStockSection::Deactivate(void)
{
	if(::IsWindow(pDlg->m_hWnd))
	{
		// Change stock section view parameters
		GQuadRenderRot::ResetStored();

		pDlg->ShowWindow(SW_HIDE);
	}
	else
		return;// To prevent error while exiting application 
	Active = false;
}

bool NStockSection::IsActive(void)
{
	return Active;
}

bool NStockSection::IsWaitingPoint(void)
{
	return Active & WaitPoint;
}

void NStockSection::SetPoint(const BPoint & InP)
{
	P = InP;
	pDlg->m_Dist = P * N;
	pDlg->DistChanged();
}

void NStockSection::SetPlane(int PlNum)
{
	switch(PlNum)
	{
	case 0://XY
		N.Set(0.,0.,1.,0.);
		Vmin = StockGabar.Zmin;
		Vmax = StockGabar.Zmax;
		break;
	case 1://ZX
		N.Set(0.,1.,0.,0.);
		Vmin = StockGabar.Ymin;
		Vmax = StockGabar.Ymax;
		break;
	case 2://YZ
		N.Set(1.,0.,0.,0.);
		Vmin = StockGabar.Xmin;
		Vmax = StockGabar.Xmax;
		break;
	case 3://Screen
		{
			GLint viewport[4];
			GLdouble projMatrix[16],modelMatrix[16];
			glMatrixMode(GL_MODELVIEW);
			glGetDoublev(GL_MODELVIEW_MATRIX ,modelMatrix);
			glGetDoublev(GL_PROJECTION_MATRIX ,projMatrix);
			glGetIntegerv(GL_VIEWPORT ,viewport);
			GLdouble x,y,z,xe,ye,ze;
			gluUnProject(0.,0.,0.,modelMatrix, projMatrix,viewport,&x,&y,&z); 
			gluUnProject(0.,0.,1.,modelMatrix, projMatrix,viewport,&xe,&ye,&ze); 
			N.Set(xe-x,ye-y,ze-z,0.);
			N = N * (sqrt(1./N.D2()));
			CalcVMinMax();
		}
		break;
	}
	Vmin = Vmin * 1.01 + Vmax * (-0.01);
	Vmax = Vmax * 1.01 + Vmin * (-0.01);
}
void NStockSection::CalcVMinMax()
{
	double Vvalues[8];
	Vvalues[0] = BPoint(StockGabar.Xmin,StockGabar.Ymin,StockGabar.Zmin,1)*N;
	Vvalues[1] = BPoint(StockGabar.Xmax,StockGabar.Ymin,StockGabar.Zmin,1)*N;
	Vvalues[2] = BPoint(StockGabar.Xmin,StockGabar.Ymax,StockGabar.Zmin,1)*N;
	Vvalues[3] = BPoint(StockGabar.Xmax,StockGabar.Ymax,StockGabar.Zmin,1)*N;
	Vvalues[4] = BPoint(StockGabar.Xmin,StockGabar.Ymin,StockGabar.Zmax,1)*N;
	Vvalues[5] = BPoint(StockGabar.Xmax,StockGabar.Ymin,StockGabar.Zmax,1)*N;
	Vvalues[6] = BPoint(StockGabar.Xmin,StockGabar.Ymax,StockGabar.Zmax,1)*N;
	Vvalues[7] = BPoint(StockGabar.Xmax,StockGabar.Ymax,StockGabar.Zmax,1)*N;
	Vmin = Vmax = Vvalues[0];
	for(int i = 1; i < 8; ++i)
	{
		Vmin = min(Vmin, Vvalues[i]);
		Vmax = max(Vmax, Vvalues[i]);
	}
	Vmin = Vmin * 1.01 + Vmax * (-0.01);
	Vmax = Vmax * 1.01 + Vmin * (-0.01);
}
bool NStockSection::NeedFillSection(void)
{
	return FillSection;
}
bool NStockSection::NeedCutAll(void)
{
	return CutAll;
}
bool NStockSection::ContainsP(const BPoint &iP, double Eps)
{// True if the iP lies on the section plane
	return fabs((iP - P) * N) < Eps;
}

bool NStockSection::CutsOffP(const BPoint &iP, const BPoint &View)
{ // true if iP is nearer to the viewer than the section plane
	double Sign = (View * N > 0) ? 1. : -1.;
	return ((iP - P) * N) * Sign < 0.;
}

void NStockSection::BeginTest1(void)
{
	LocalCut.BeginTest1();
}

void NStockSection::BeginTest2(void)
{
	LocalCut.BeginTest2();
}

void NStockSection::EndTest(void)
{
	LocalCut.EndTest();
}