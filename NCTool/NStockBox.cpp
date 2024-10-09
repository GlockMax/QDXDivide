// NStockBox.cpp: implementation of the NStockBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GBodyRender.h"
#include "MBSPNode.h"
#include "ConstDef.h"
#include "NCMProject.h"
#include "MBSPForest.h"
#include "NMeshArray.h"
#include "NRectMesh.h"
#include "NMakeStdMesh.h"
#include "MQuadTree.h"
#include "MCubeMask.h"
#include "BaseRender.h"
#include "DXSubModel5X.h"
#include "TBladeCont.h"
#include "nstockbox.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#undef max
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(NStockBox, NStock, 0)

NStockBox::NStockBox()
: NStock()
{

}

void NStockBox::CreateCopyTo(class NStock* &stock) const
{
	if (!stock)
		stock = new NStockBox();
	NStock::CreateCopyTo(stock);

	((NStockBox*)stock)->Box = this->Box;
}

NStockBox::~NStockBox()
{
}

bool NStockBox::Create(BBox gabar)
{
	if(!gabar.IsDefined())
		return false;
	Box = gabar;
	//if (NCMComData::IsTurnEnabled())
	//	Box.Transform(BMatr().RotX(0., 0., 0., -90.));

	Create();
	return true;
}

void NStockBox::Serialize(CArchive &ar)
{
	NStock::Serialize(ar);

	if(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("3.8"))
	{
		double B[6];
		BOOL Def;
		SerializeElements(ar, B, 6);
		SerializeElements(ar, &Def, 1);
		if(Def)
		{
			Box.Expand(B[0], B[2], B[4]);
			Box.Expand(B[1], B[3], B[5]);
		}
	}
	else
		SerializeElements(ar, &Box, 1);
	if(ar.IsLoading())
		Create();
}

void NStockBox::Create()
{
	NStock::Create();
	Gabar = Box;
	Gabar.Transform(Matr);
}

void NStockBox::DrawOriginal(enum RenderMode GlobRMode, enum RenderMode ExtRMode, bool DrawSurf)
{
	if(!NeedDraw(GlobRMode, ExtRMode))
		return;
	NStock::DrawOriginal(GlobRMode, ExtRMode, DrawSurf);

	bool DrawEdges = GetEdgesS() && (NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::BSP)
		|| GetEdgesDX() && (NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::DX);
	DrawEdges &= !WFrame;
	if(WFrame)
	{
		BaseRender::SetWFMode();
	}
	glPushMatrix();	
	glMultMatrixd(Matr.GetArray());

	if (DrawEdges)
	{
		glPushAttrib(GL_ENABLE_BIT);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1., 1.);
	}
	glBegin(GL_QUADS);
	glNormal3d(-1.,0.,0.);
	glVertex3d(Box.Xmin,Box.Ymin,Box.Zmin);
	glVertex3d(Box.Xmin,Box.Ymin,Box.Zmax);
	glVertex3d(Box.Xmin,Box.Ymax,Box.Zmax);
	glVertex3d(Box.Xmin,Box.Ymax,Box.Zmin);

	glNormal3d(1.,0.,0.);
	glVertex3d(Box.Xmax,Box.Ymin,Box.Zmin);
	glVertex3d(Box.Xmax,Box.Ymax,Box.Zmin);
	glVertex3d(Box.Xmax,Box.Ymax,Box.Zmax);
	glVertex3d(Box.Xmax,Box.Ymin,Box.Zmax);

	glNormal3d(0.,0.,-1.);
	glVertex3d(Box.Xmin,Box.Ymax,Box.Zmin);
	glVertex3d(Box.Xmax,Box.Ymax,Box.Zmin);
	glVertex3d(Box.Xmax,Box.Ymin,Box.Zmin);
	glVertex3d(Box.Xmin,Box.Ymin,Box.Zmin);

	glNormal3d(0.,1.,0.);
	glVertex3d(Box.Xmin,Box.Ymax,Box.Zmax);
	glVertex3d(Box.Xmax,Box.Ymax,Box.Zmax);
	glVertex3d(Box.Xmax,Box.Ymax,Box.Zmin);
	glVertex3d(Box.Xmin,Box.Ymax,Box.Zmin);

	glNormal3d(0.,0.,1.);
	glVertex3d(Box.Xmin,Box.Ymin,Box.Zmax);
	glVertex3d(Box.Xmax,Box.Ymin,Box.Zmax);
	glVertex3d(Box.Xmax,Box.Ymax,Box.Zmax);
	glVertex3d(Box.Xmin,Box.Ymax,Box.Zmax);

	glNormal3d(0.,-1.,0.);
	glVertex3d(Box.Xmin,Box.Ymin,Box.Zmin);
	glVertex3d(Box.Xmax,Box.Ymin,Box.Zmin);
	glVertex3d(Box.Xmax,Box.Ymin,Box.Zmax);
	glVertex3d(Box.Xmin,Box.Ymin,Box.Zmax);

	glEnd();
	if (DrawEdges)
	{
		glPopAttrib();
		if (GlobRMode == RM_TRANSLUCENT)
		{
			glPushAttrib(GL_ENABLE_BIT);
			glDisable(GL_LIGHTING);
		}
		else
			BaseRender::SetWFMode();
		if (EdgesAttrs.GetEdgesWidth() > 0.01)
		{
			glPushAttrib(GL_CURRENT_BIT);
			glColor4fv(EdgesAttrs.GetEdgesColor().array);
			glLineWidth(GLfloat(EdgesAttrs.GetEdgesWidth()));
			glBegin(GL_LINE_STRIP);
			glVertex3d(Box.Xmin, Box.Ymin, Box.Zmin);
			glVertex3d(Box.Xmin, Box.Ymin, Box.Zmax);
			glVertex3d(Box.Xmin, Box.Ymax, Box.Zmax);
			glVertex3d(Box.Xmin, Box.Ymax, Box.Zmin);
			glVertex3d(Box.Xmin, Box.Ymin, Box.Zmin);
			glEnd();

			glBegin(GL_LINE_STRIP);
			glVertex3d(Box.Xmax, Box.Ymin, Box.Zmin);
			glVertex3d(Box.Xmax, Box.Ymax, Box.Zmin);
			glVertex3d(Box.Xmax, Box.Ymax, Box.Zmax);
			glVertex3d(Box.Xmax, Box.Ymin, Box.Zmax);
			glVertex3d(Box.Xmax, Box.Ymin, Box.Zmin);
			glEnd();

			glBegin(GL_LINES);
			glVertex3d(Box.Xmin, Box.Ymax, Box.Zmin);
			glVertex3d(Box.Xmax, Box.Ymax, Box.Zmin);
			glVertex3d(Box.Xmax, Box.Ymin, Box.Zmin);
			glVertex3d(Box.Xmin, Box.Ymin, Box.Zmin);
			glVertex3d(Box.Xmin, Box.Ymax, Box.Zmax);
			glVertex3d(Box.Xmax, Box.Ymax, Box.Zmax);
			glVertex3d(Box.Xmin, Box.Ymin, Box.Zmax);
			glVertex3d(Box.Xmax, Box.Ymin, Box.Zmax);
			glEnd();

			glPopAttrib();
		}
		if (GlobRMode == RM_TRANSLUCENT)
			glPopAttrib();
		else
			BaseRender::RemoveWFMode();
	}
	glPopMatrix();
	if(WFrame)
	{
		BaseRender::RemoveWFMode();
	}
	NStock::RestDrawOriginal();
}

bool NStockBox::CreateBody(void)
{
	if (NCMComData::IsTurnEnabled())
	{
		SetStockType(AT_MILL_TURN);
	}
	else
	{
		SetStockType(AT_MILL);
	}
	switch(NCM_PROJECT.GetGlobalState().GetModelMode())
	{
	case NGlobalState::DX:
		{
			int Num = int(BDebugState::Get().DXNumber + 0.5);
			MakeGabarBoxDX5X(Num);
			DXSubModel5X *pBodyDX = (DXSubModel5X *)MFOREST.GetTree(BSPTNumDX);
			if (Matr.IsE())
			{
				BBox LBox = GetGabar();
				if (NCMComData::IsTurnEnabled())
					LBox.Transform(BMatr().RotX(0., 0., 0., -90.));
				pBodyDX->MakeBox(LBox);
			}
			else
			{

				MQuadTree* pTree = MakeBSPTree(1);
				if (pTree == NULL)
					return false;

				pBodyDX->MakeFromBSP(*pTree);

				delete pTree;
				MFOREST.Compress();
			}
		}
		break;
	case NGlobalState::BSP:
		{
			int Num = int(BDebugState::Get().CellSize + 0.5);
			MQuadTree* pTree = nullptr;
			if (/*Matr.IsE()*/ false)// changed 06/03/24 for snaps work correctly with box stock
			{
				BBox NBox = Box;
				if (NCMComData::IsTurnEnabled())
					NBox.Transform(BMatr().RotX(0., 0., 0., -90.));
				pTree = new MQuadTree(NBox.GetMinPoint() + BPoint(-ST_DEC, -ST_DEC, ST_DEC, 0.), NBox.GetMaxPoint() + BPoint(ST_INC, ST_INC, ST_INC, 0.), Num, Num, GetStockGr(), false);
			}
			else
			{
				pTree = MakeBSPTree(Num);
			}
			if (BSPTNum < 0)
				BSPTNum = MFOREST.AddTree(pTree);
			else
			{
				MFOREST.DeleteTree(BSPTNum);
				MFOREST.SetTree(BSPTNum, pTree);
			}
		}
		break;
	}
	if (NCMComData::IsTurnEnabled())
	{
		// Create body for turn
		MakeTurnFromMillBox();
	}
	else
	{
		RemoveRotBody();
	}
	CreateRender();
	MillStockChanged = false;
	TurnStockChanged = false;
	return true;
}

const BPoint NStockBox::RayCastingOr(const BPoint & P, const BPoint & V, double MinW, BPoint &N) const
{
	return Box.RayCasting(P, V, MinW, N);
}

void NStockBox::Transformm(const BMatr & M)
{
	NMovableObj::Transformm(M);
	Create();
}

MQuadTree *NStockBox::MakeBSPTree(int Num, bool ForDX5X /*= false */) const
{
	BPoint DeltaMin(-ST_DEC, -ST_DEC, -ST_DEC, 0.);
	BPoint DeltaMax(ST_INC * 0.3, ST_INC * 0.3, ST_INC * 0.3, 0.);
	BBox NBox = Box;
	if (NCMComData::IsTurnEnabled())
		NBox.Transform(BMatr().RotX(0., 0., 0., -90.));
	MeshArr MeshArray;
	MFOREST.SetMark();
	NMakeStdMesh::BoxBounds(MeshArray, NBox.GetMinPoint(), NBox.GetMaxPoint());
	MFOREST.TransformFMark(Matr);
	MFOREST.RemoveMark();

	if (!NCMComData::IsTurnEnabled())
		NBox = Gabar;
	MQuadTree *pTree = new MQuadTree(NBox.GetMinPoint() + DeltaMin, NBox.GetMaxPoint() + DeltaMax, Num, Num, GetStockGr(), false);
	
	for(int l = 0; l < MeshArray.GetSize(); ++l)
	{
		pTree->StoreMesh(*MeshArray[l], MElemIDProc::ID_NULL, cadrID(-1), false);
		delete MeshArray[l];
	}

	BBox Bounds;
	Bounds.Expand(NBox.GetMinPoint() + BPoint(-1., -1., -1., 0.));
	Bounds.Expand(NBox.GetMaxPoint() + BPoint(+1., +1., +1., 0.));
	pTree->CreateBodyStd(Bounds, false, false);
	pTree->CrStandard(false);
	return pTree;
}

bool NStockBox::MakeTurnFromMillBox(void)
{
	// Box is in the "right" coordinates (rotation axis - Z) but contour in XY plane where Y is rotation axis
	// Calculate 4 points
	BBox NBox = Box;
	NBox.Transform(BMatr().RotX(0., 0., 0., -90.));
	BPoint Pv[4];
	Pv[0] = NBox.GetMinPoint();
	Pv[0].SetY(0.);
	Pv[2] = NBox.GetMaxPoint();
	Pv[2].SetY(0.);
	Pv[1] = Pv[0];
	Pv[1].SetX(Pv[2].GetX());
	Pv[3] = Pv[2];
	Pv[3].SetX(Pv[0].GetX());
	// Find largest distance from rotation axis
	double MaxD2 = 0.;
	for (int i = 0; i < 4; ++i)
	{
		MaxD2 = fmax(MaxD2, Pv[i].D2());
	}
	// Check if rotation axis is inside the box
	bool Xsignchanges = (Pv[0].GetX() * Pv[2].GetX()) <= 0.;
	bool Zsignchanges = (Pv[0].GetZ() * Pv[2].GetZ()) <= 0.;
	bool IsInside = Xsignchanges && Zsignchanges;
	// Find smallest distance from rotation axis
	double MinD2;
	if (IsInside)
		MinD2 = 0.;
	else
	{
		MinD2 = std::numeric_limits<double>::max();
		// Nearest vertex
		for (int i = 0; i < 4; ++i)
		{
			MinD2 = fmin(MinD2, Pv[i].D2());
		}
		// Nearest edge
		BPoint O(0., 0., 0., 1.);
		for (int i = 0; i < 4; ++i)
		{
			int j = (i + 1) % 4;
			BPoint V = Pv[j] - Pv[i];
			if ((V * (O - Pv[i])) * (V * (Pv[j] - O)) > 0.)
			{
				// Center projection is inside the edge
				BPoint ExtNorm = V * BMatr().RotY(0., 0., 0., 90.);
				double Sp = ExtNorm * (O - Pv[i]);
				if (Sp > 0.)
				{
					// Edge is near
					double D2 = ExtNorm.D2();
					if (D2 > MIND * MIND)
						Sp /= sqrt(D2);
					Sp *= Sp;
					MinD2 = fmin(MinD2, Sp);
				}
			}
		}
	}

	TBladeCont Cont;
	Cont.Clear();
	TBladeEdge *pEdge = new TBladeEdge;
	pEdge->SetAttr(BE_CUT);
	BPoint StartP(NBox.GetMinPoint());
	StartP.SetZ(0);
	StartP.SetX(sqrt(MinD2));
	BPoint P(StartP);
	pEdge->SetPoint(P);
	Cont.AddEdge(pEdge);
	pEdge = new TBladeEdge(*pEdge);
	P.SetX(sqrt(MaxD2));
	pEdge->SetPoint(P);
	Cont.AddEdge(pEdge);
	pEdge = new TBladeEdge(*pEdge);
	P.SetY(NBox.GetMaxPoint().GetY());
	pEdge->SetPoint(P);
	Cont.AddEdge(pEdge);
	pEdge = new TBladeEdge(*pEdge);
	P.SetX(StartP.GetX());
	pEdge->SetPoint(P);
	Cont.AddEdge(pEdge);
	pEdge = new TBladeEdge(*pEdge);
	pEdge->SetPoint(StartP);
	Cont.AddEdge(pEdge);

	MakeRot(Cont);

	MFOREST.Compress();

	return true;
}

NStock::StockCrType NStockBox::GetType() const
{
	return SCT_BOX;
}
