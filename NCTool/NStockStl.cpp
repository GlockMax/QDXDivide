// NStockStl.cpp: implementation of the NStockStl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConstDef.h"
#include "NCMProject.h"
#include "MQuadTree.h"
#include "math.h"
#include "BaseRender.h"
#include "SOPlane.h"
#include "DXSubModel5X.h"
#include "DXFOut.h"
#include "NStockStl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(NStockStl, NStock, 0)

NStockStl::NStockStl()
: NStock()
{
	IsCreated = false;
}

void NStockStl::CreateCopyTo(class NStock* &stock) const
{
	if(!stock)
		stock = new NStockStl();
	NStock::CreateCopyTo(stock);

	NStockStl *pStl = (NStockStl*)stock;
	this->Stl.MakeCopy(pStl->Stl);
}

NStockStl::~NStockStl()
{
	Deleted();
}

void NStockStl::Create()
{
	NStock::Create();
	

	IsCreated = Stl.CreateEx(Stl.GetFileName());
	if(IsCreated)
		Stl.Transform(Matr);
	Recalc();
}



void NStockStl::Serialize(CArchive &ar)
{
	NStock::Serialize(ar);
	Stl.Serialize(ar, NCM_PROJECT.GetInFileVersion());

	if(ar.IsLoading())
	{
		Stl.Transform(Matr);
		Recalc();
	}
	IsCreated = (Gabar.IsDefined() == TRUE);
}

void NStockStl::CreateEx(const CString &name)
{
	IsCreated = Stl.CreateEx(name);
	if(IsCreated)
		Stl.Transform(Matr);
	Recalc();
}

void NStockStl::MoveBase(double x, double y, double z)
{
	Stl.MoveBase(x, y, z);
	NStock::Create();
	IsCreated = (Stl.GetTriangles() != NULL);
	if(IsCreated)
	{
		Matr.Trans(x, y, z);
		Stl.Transform(Matr);
	}
	Recalc();
}



CString NStockStl::GetFileName() const
{
	return Stl.GetFileName();
}

const BPoint & NStockStl::GetBase()
{
	return Stl.GetBase();
}

void NStockStl::DrawOriginal(enum RenderMode GlobRMode, enum RenderMode ExtRMode, bool DrawSurf)
{
	if(!NeedDraw(GlobRMode, ExtRMode))
		return;
	NStock::DrawOriginal(GlobRMode, ExtRMode, DrawSurf);
	BOOL SharpEdges = SharpEdgesS;
	if (IsRealStock())
	{
		switch (NCM_PROJECT.GetGlobalState().GetModelMode())
		{
		case NGlobalState::DX:
			SharpEdges = SharpEdgesDX;
			break;
		case NGlobalState::BSP:
			SharpEdges = SharpEdgesS;
			break;
		}
	}
	Stl.DrawOriginal(GlobRMode, ExtRMode, WFrame == TRUE, SharpEdges == TRUE, DrawSurf, DrawSmoothSurf);
	NStock::RestDrawOriginal();
}
bool NStockStl::CreateBody(void)
{
//Create quad tree
	const GLfloat *Triangles = Stl.GetTriangles();
	if(Triangles == NULL)
		return false;
	switch(NCM_PROJECT.GetGlobalState().GetModelMode())
	{
	case NGlobalState::DX:
		{
			int DXNum = int(BDebugState::Get().DXNumber + 0.5);
			BBox LBox = GetGabar();
			LBox.Offset(+0.001, +0.001, +0.001);
			MakeBoxDX5X(DXNum, LBox);

			DXSubModel5X *pBodyDX = (DXSubModel5X *)MFOREST.GetTree(BSPTNumDX);
			for(int TriInd = 0; TriInd < int(Triangles[0]); ++TriInd)
			{
				int i = TriInd * 18 + 1;
				BPoint pn(Triangles[i], Triangles[i+1], Triangles[i+2],0.);
				pn = pn * (1. / sqrt(pn.D2()));
				BPoint p0(Triangles[i+3], Triangles[i+4], Triangles[i+5], 1.);
				BPoint p1(Triangles[i+9], Triangles[i+10], Triangles[i+11], 1.);
				BPoint p2(Triangles[i+15], Triangles[i+16], Triangles[i+17], 1.);
				ElemID ElID = ID_EMPTY + SURF_NUM_SHIFT;
				MElemIDProc::SetPCn(&ElID, STOCK_COLOR_NUM);
				MElemIDProc *pProc = MFOREST.GetIDProc();
				int SurfNum;
				float N[3] = {float(pn.GetX()), float(pn.GetY()), float(pn.GetZ())};
				pProc->SetNormalToSurfID(SurfNum, N);
				MElemIDProc::SetSurfID(&ElID, SurfNum);
				pBodyDX->RastTriangle(p0, p1, p2, pn, DX_ID(ElID, cadrID(-1)));
			}
			pBodyDX->Heal();
		}
		break;
	case NGlobalState::BSP:
		{
			int Num = int(BDebugState::Get().CellSize + 0.5);
			MQuadTree *pTree = new MQuadTree(Gabar.GetMinPoint() + BPoint(-ST_DEC, -ST_DEC, -ST_DEC, 0.), Gabar.GetMaxPoint() + BPoint(ST_INC, ST_INC, ST_INC, 0.), Num, Num, GetStockGr(), false);
			if(BSPTNum < 0)
				BSPTNum = MFOREST.AddTree(pTree);
			else
			{
				MFOREST.DeleteTree(BSPTNum);
				MFOREST.SetTree(BSPTNum, pTree);
			}
//			pTree->MakeFromTriangles(int(Triangles[0]), &Triangles[1]);
			if (!Stl.GetDCEL().IsCreated())
				Stl.GetDCEL().MakeFromStl(Stl);
			pTree->MakeFromBDCEL(Stl.GetDCEL());
		}
		break;
	}
	CreateRenderMill();

	MillStockChanged = false;
	TurnStockChanged = false;
	return true;
}

const GLfloat * NStockStl::GetTriangles(void) const
{
	return Stl.GetTriangles();
}

void NStockStl::Transformm(const BMatr & M)
{
	Stl.Transform(M);
	Recalc();
	NMovableObj::Transformm(M);
}

void NStockStl::WriteSTL(CStdioFile & f)
{
	Stl.WriteSTL(f);
}

GLfloat *NStockStl::MakeOneSection(const BPoint &P, const BPoint &V, bool Trimed) const
{
	return Stl.MakeOneSection(P, V, Trimed);
}

void NStockStl::DrawSelected(void) const
{
	glPushAttrib(GL_LINE_BIT | GL_COLOR_BUFFER_BIT);
	auto& Attrs = GetSelectAttrs();
	glLineWidth(GLfloat(Attrs.GetBoxWidth()));
	if (Attrs.GetBox().GetA() == 0.)
		glColor3fv(GetColor().array);
	else
		glColor3fv(Attrs.GetBox().array);
	Stl.DrawSelected();
	glPopAttrib();
}

const BPoint NStockStl::RayCastingOr(const BPoint & P, const BPoint & V, double MinW, BPoint &N) const
{
	return Stl.RayCasting(P, V, MinW, N);
}

bool NStockStl::FindSegmentsWithStl(SnapRopes& oSegments, std::vector<BMatr>& oMatrsInvr)
{
    const auto Res = Stl.FindSegments(oSegments);
	if (Res)
	{
		const auto Rest = oSegments.size() - oMatrsInvr.size();
		oMatrsInvr.reserve(oMatrsInvr.size() + Rest);
		for (int i = 0; i < Rest; ++i)
			oMatrsInvr.push_back(BMatr().SetE());
	}
	return Res;
}

void NStockStl::SetDrawSmoothSurf(bool SmoothSurf)
{
	NStock::SetDrawSmoothSurf(SmoothSurf);
	Stl.SetSmoothSurf(SmoothSurf);
}

void NStockStl::SetRMode(RenderMode Mode)
{
	NStock::SetRMode(Mode);
	Stl.SetRMode(Mode);
}

NStock::StockCrType NStockStl::GetType() const
{
	return SCT_STL;
}

void NStockStl::SetEdgesAttrs(const BEdgesAttrs & Attr)
{
	NPassiveObject::SetEdgesAttrs(Attr);
	Stl.SetEdgesAttrs(Attr);
}

void NStockStl::Recalc()
{
	Gabar = Stl.GetGabar();
}