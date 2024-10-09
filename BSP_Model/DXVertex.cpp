#include "stdafx.h"
#include "DXSubModel3X.h"
#include "MBSPForest.h"
#include "SOrigSurf.h"
#include "MElemIDProc.h"
#include "DXVertex.h"

const DXSubModel3X *DXVertex::pDXSubModel = NULL;
DXVertex::DXVertex(void) : Point(0., 0., 0., -1.)
{
}

DXVertex::~DXVertex(void)
{
}


DXVertex::DXVertex(int I, int J, int K)
{
	SetID(I, J, K);
}

void DXVertex::SetID(int I, int J, int K)
{
	i = I;
	j = J;
	k = K;
	CalcData();
}

void DXVertex::SetModel(const DXSubModel3X &CurModel)
{
//	if(pDXSubModel == NULL)
		pDXSubModel = &CurModel;
}

double DXVertex::GetX() const
{
	if(pDXSubModel == NULL)
		return 0.;
	else
		return(i * pDXSubModel->GetXstep());
}

double DXVertex::GetY() const
{
	if(pDXSubModel==NULL)
		return 0.;
	else
		return(j * pDXSubModel->GetYstep());
}

double DXVertex::GetZ() const
{
	if(pDXSubModel == NULL)
		return 0.;
	else
		return(pDXSubModel->GetDepth(i, j, k));
}

const BPoint &DXVertex::GetNormal() const
{
	return Normal;
}

const BPoint &DXVertex::GetPoint() const
{
	return Point;
}

const BPoint DXVertex::GetPointBot() const
{
	BPoint B(Point);
	B.SetZ(pDXSubModel->GetDepth(i, j, k - 1));
	return B;
}

void DXVertex::CalcData()
{
	RenderIndex = -1;
	if(pDXSubModel == NULL)
	{
		Point.Set(0., 0., 0., 1.);
		return;
	}
	bool kEven = (k % 2 == 0);
	int kTop = kEven ? k + 1 : k;
	int kBot = kEven ? k : k - 1;
	double ZijTop = pDXSubModel->GetDepth(i, j, kTop);
	double ZijBot = pDXSubModel->GetDepth(i, j, kBot);
	if(ZijTop > ZijBot)
		Point.Set(i * pDXSubModel->GetXstep(), j * pDXSubModel->GetYstep(), kEven ? ZijBot : ZijTop, 1.);
	else
		Point.Set(0., 0., 0., -1.);
	
	ElemID ElID = pDXSubModel->GetID(i, j, k);
	PCol = MFOREST.GetIDProc()->GetColor(ElID);
	int SurfNum = MFOREST.GetIDProc()->GetSurfID(ElID);
	if(SurfNum < 0)
	{
		pSurf = NULL;
		Normal.Set(0.,0.,1.,0.);
	}
	else
	{
		pSurf = MFOREST.GetOrigSurf(SurfNum);
		if(!pSurf)
			Normal.Set(0., 0. , kEven ? -1. : 1., 0.);
		else
			Normal = pSurf->GetNormal(Point + pDXSubModel->GetGabar().GetMinPoint());
	}
}

int DXVertex::GetSurfaceID() const
{
	return MFOREST.GetIDProc()->GetSurfID(pDXSubModel->GetID(i, j, k));
}

const SOrigSurf *DXVertex::GetSurface() const
{
	return pSurf;
}

bool DXVertex::GenRenderIndex(int *pCurRenderIndex)
{// returns true if NEW index was generated
	if(RenderIndex >= 0)
		return false;
	RenderIndex = (*pCurRenderIndex)++;
	return true;
}

void DXVertex::SetEmpty(void)
{
	i = 0;
	j = 0;
	k = 0;
	RenderIndex = -1;
	pSurf = NULL;
	Point.Set(0., 0., 0., -1.);
}

