// NStockCyl.cpp: implementation of the NStockCyl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BaseRender.h"
#include "ConstDef.h"
#include "NSurfSettings.h"
#include "NCMProject.h"
#include "GBodyRender.h"
#include "MQuadTree.h"
#include "NMeshArray.h"
#include "NRectMesh.h"
#include "NMakeStdMesh.h"
#include "Gl\GLu.h"
#include "math.h"
#include "SOCyl.h"
#include "SOPlane.h"
#include "DXSubModel5X.h"
#include "nstockcyl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(NStockCyl, NStock, 0)
NStockCyl::NStockCyl()
: NStock(), axis(2)
{

}

void NStockCyl::CreateCopyTo(class NStock* &stock) const
{
	if (!stock)
		stock = new NStockCyl;
	NStock::CreateCopyTo(stock);

	NStockCyl *pCyl = (NStockCyl*)stock;
	pCyl->x = this->x;
	pCyl->y = this->y;
	pCyl->z = this->z;
	pCyl->d = this->d;
	pCyl->h = this->h;
	pCyl->axis = this->axis;
}

NStockCyl::~NStockCyl()
{

}

void NStockCyl::Serialize(CArchive &ar)
{
	NStock::Serialize(ar);
	SerializeElements(ar,&x,1);
	SerializeElements(ar,&y,1);
	SerializeElements(ar,&z,1);
	SerializeElements(ar,&d,1);
	SerializeElements(ar,&h,1);
	if(!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < 2.7))
	{
		SerializeElements(ar,&axis,1);
	}
	if(ar.IsLoading())
		Create();
}

void NStockCyl::Create(double X, double Y, double Z, double D, double H, int Axis)
{
	x = X;
	y = Y;
	z = Z;
	d = D;
	h = H;
	axis = Axis;

	Create();
}

void NStockCyl::Create()
{
	NStock::Create();
	Gabar.Init();
	Gabar.Expand( -d/2., -d/2., 0.);
	Gabar.Expand( d/2., d/2., h);
	Gabar.Transform(GetTransfMatr());
}

void NStockCyl::DrawOriginal(enum RenderMode GlobRMode, enum RenderMode ExtRMode, bool DrawSurf)
{
	if(!NeedDraw(GlobRMode, ExtRMode))
		return;
	NStock::DrawOriginal(GlobRMode, ExtRMode, DrawSurf);
	if(WFrame)
		BaseRender::SetWFMode();

	glPushMatrix();
	glMultMatrixd(GetTransfMatr().GetArray());

	int SlicesNum = 50;

	bool DrawEdges = GetEdgesS() && (NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::BSP)
		|| GetEdgesDX() && (NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::DX);
	DrawEdges &= !WFrame;

	if (DrawEdges) 
	{
		glPushAttrib(GL_ENABLE_BIT);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1., 1.);
	}
	BaseRender::DrawCone(0., d * 0.5, 0., d * 0.5, h, SlicesNum, false);
	if (DrawEdges)
		glPopAttrib();
	if (DrawEdges)
	{
		if (GlobRMode == RM_TRANSLUCENT)
		{
			glPushAttrib(GL_ENABLE_BIT);
			glDisable(GL_LIGHTING);
		}
		else
			BaseRender::SetWFMode();
		glPushAttrib(GL_CURRENT_BIT);
		glColor4fv(GBodyRender::GetEdgesColor().array);
		glLineWidth(GLfloat(GBodyRender::GetEdgesWidth()));
		BaseRender::DrawCone(0., d * 0.5, 0., d * 0.5, h, SlicesNum, true);
		glPopAttrib();
		if (GlobRMode == RM_TRANSLUCENT)
			glPopAttrib();
		else
			BaseRender::RemoveWFMode();
	}
	glPopMatrix();

	if(WFrame)
		BaseRender::RemoveWFMode();
	NStock::RestDrawOriginal();
}

bool NStockCyl::CreateBody(void)
{
	BPoint DeltaMin(-ST_DEC, -ST_DEC, -ST_DEC, 0.);
	BPoint DeltaMax(ST_INC * 0.3, ST_INC * 0.3, ST_INC * 0.3, 0.);

	BMatr Tr = GetTransfMatr();
	switch (NCM_PROJECT.GetGlobalState().GetModelMode())
	{
	case NGlobalState::DX:
		{
			int DXNum = int(BDebugState::Get().DXNumber + 0.5);
			MakeGabarBoxDX5X(DXNum);

			DXSubModel5X *pBodyDX = (DXSubModel5X *)MFOREST.GetTree(BSPTNumDX);
			pBodyDX->MakeCyl(d, h, Tr);
		}
		break;
	case NGlobalState::BSP:
		{
			int Num = int(BDebugState::Get().CellSize + 0.5);
			MQuadTree *pTree = new MQuadTree(Gabar.GetMinPoint() + DeltaMin, Gabar.GetMaxPoint() + DeltaMax, Num, Num, GetStockGr(), false);
			if(BSPTNum < 0)
				BSPTNum = MFOREST.AddTree(pTree);
			else
			{
				MFOREST.DeleteTree(BSPTNum);
				MFOREST.SetTree(BSPTNum, pTree);
			}
	
			// Create meshs for standard cylinder
			MeshArr MeshArray;
			BMatr Tr = GetTransfMatr();

			SetStockType(AT_MILL);
			MFOREST.SetMark();
			NMakeStdMesh::Cylinder(MeshArray, d, h, NSurfSettings::GetTolerance());
			MFOREST.TransformFMark(Tr);
			MFOREST.RemoveMark();
			for (int l = 0; l < MeshArray.GetSize(); ++l)
			{
				pTree->StoreMesh(*MeshArray[l], MElemIDProc::ID_NULL, cadrID(-1), false);
				delete MeshArray[l];
			}

			BBox Bounds;
			Bounds.Expand(Gabar.GetMinPoint() + BPoint(-1., -1., -1., 0.));
			Bounds.Expand(Gabar.GetMaxPoint() + BPoint(+1., +1., +1., 0.));
			pTree->CreateBodyStd(Bounds, false, false);
			pTree->CrStandard(false);
		}
		break;
	}
	CreateRender();

	MillStockChanged = false;
	TurnStockChanged = false;
	return true;
}

const BMatr NStockCyl::GetTransfMatr(void) const
{
	BMatr Matr, B;
	Matr.Trans(x, y, z);
	switch(axis)
	{
	case 0:// X
		Matr = B.RotY(BPoint(0.,0.,0.,1.), 90.) * Matr;
		break;
	case 1:// Y
		Matr = B.RotX(BPoint(0.,0.,0.,1.), -90.) * Matr;
		break;
	case 2:// Z
		break;
	}

	return Matr * NMovableObj::Matr;
}
const BPoint NStockCyl::RayCastingOr(const BPoint & P, const BPoint & V, double MinW, BPoint &N) const
{
	BPoint Dir(1., 0., 0., 0.);
	switch(axis)
	{
	case 0:
		break;
	case 1:
		Dir.Set(0., 1., 0., 0.);
		break;
	case 2:
		Dir.Set(0., 0., 1., 0.);
		break;
	}
	SOCyl Cyl(BPoint(x, y, z, 1.), Dir, 0.5 * d);
	Cyl.Transform(GetTransfMatr());
	BPoint Res = P;
	bool Ref = Cyl.RefiningPoint(Res, V);
	double height = (Res - BPoint(x, y, z, 1.)) * Dir;
	if(height >= 0 && height <= h && Ref)
	{// Side surface
		N = Cyl.GetNormal(Res);
		return Res;
	}
	double base = V * Dir > 0. ? 0. : h;
	BPoint Pbase = BPoint(x, y, z, 1.) + Dir * base;
	SOPlane Plane(Dir, Pbase);
	Res = P;
	Ref = Plane.RefiningPoint(Res, V);
	if((Res - Pbase).D2() <= d * d * 0.25 && Ref)
	{
		N = Dir * ((V * Dir > 0.) ? -1. : 1.);
		return Res;
	}
	return BPoint(0., 0., 0., -1.);
}

void NStockCyl::Transformm(const BMatr & M)
{
	NMovableObj::Transformm(M);
	Create();
}

NStock::StockCrType NStockCyl::GetType() const
{
	return SCT_CYL;
}
