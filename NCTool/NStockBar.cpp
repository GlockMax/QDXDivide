#include "stdafx.h"
#include "BaseRender.h"
#include "GLee.h"
#include "gl\glu.h"
#include "BDebugState.h"
#include "ConstDef.h"
#include "MQuadTree.h"
#include "GBodyRender.h"
#include "NMeshArray.h"
#include "NMakeStdMesh.h"
#include "NRectMesh.h"
#include "NParamMesh.h"
#include "MBSPInter.h"
#include "SOPlane.h"
#include "MElemIDProc.h"
#include "math.h"
#include "MQuadTreeSliced.h"
#include "NSurfSettings.h"
#include "DXSubModel5X.h"
#include "nstockbar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(NStockBar, NStock, 0)

NStockBar::NStockBar(void) 
	: BaseCenter(0., 0., 0., 1.)
{
	PolyNum = 0;// A number of polygon edges (circle if 0)
	Axis = 2;
	//double ExtRad0 = 0., ExtRad1 = 0.;//circumcircle rad for external contour
	//double IntRad0 = 0., IntRad1 = 0.;//circumcircle rad for internal contour
	//double Length = 0.;// Along Y axis
	//double RotAngle = 0.;// Rotation angle around Y axis

}

void NStockBar::CreateCopyTo(class NStock* &stock) const
{
	if (!stock)
		stock = new NStockBar();
	NStock::CreateCopyTo(stock);
	
	NStockBar *pStockBar = (NStockBar*)stock;

	pStockBar->PolyNum = this->PolyNum;
	pStockBar->ExtRad0 = this->ExtRad0;
	pStockBar->ExtRad1 = this->ExtRad1;
	pStockBar->IntRad0 = this->IntRad0;
	pStockBar->IntRad1 = this->IntRad1;
	pStockBar->Length = this->Length;
	pStockBar->RotAngle = this->RotAngle;
	pStockBar->BaseCenter = this->BaseCenter;
}

NStockBar::~NStockBar(void)
{
}

void NStockBar::DrawOriginal(enum RenderMode GlobRMode, enum RenderMode ExtRMode, bool DrawSurf)
{
	if(!NeedDraw(GlobRMode, ExtRMode))
		return;
	NStock::DrawOriginal(GlobRMode, ExtRMode, DrawSurf);
	if(WFrame)
	{
		BaseRender::SetWFMode();
	}

	glPushMatrix();
	glMultMatrixd(GetTransfMatr().GetArray());
	if(PolyNum == 0)
		DrawConePipe(GlobRMode);
	else
		DrawPolyPipe(GlobRMode);

	if(WFrame)
	{
		BaseRender::RemoveWFMode();
	}
	glPopMatrix();
	NStock::RestDrawOriginal();
}
void NStockBar::DrawConePipe(enum RenderMode GlobRMode)
{
	int SlicesNum = 50;

	bool DrawEdges = GetEdgesS() && (NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::BSP)
		|| GetEdgesDX() && (NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::DX);
	DrawEdges &= !WFrame;


	glPushMatrix();
	if (DrawEdges)
	{
		glPushAttrib(GL_ENABLE_BIT);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1., 1.);
	}
	if (NCMComData::IsTurnEnabled())
	{
		glTranslated(0., 0., BaseCenter.GetZ());
	}
	else
	{
		glTranslated(0., BaseCenter.GetZ(), 0.);
		glRotated(-90., 1., 0., 0.);
	}
	BaseRender::DrawCone(IntRad0, ExtRad0, IntRad1, ExtRad1, Length, SlicesNum, false);
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

		if (EdgesAttrs.GetEdgesWidth() > 0.01)
		{
			glPushAttrib(GL_CURRENT_BIT | GL_LINE_BIT);
			glColor4fv(EdgesAttrs.GetEdgesColor().array);
			glLineWidth(GLfloat(EdgesAttrs.GetEdgesWidth()));
			BaseRender::DrawCone(IntRad0, ExtRad0, IntRad1, ExtRad1, Length, SlicesNum, true);
			glPopAttrib();
		}
		if (GlobRMode == RM_TRANSLUCENT)
			glPopAttrib();
		else
			BaseRender::RemoveWFMode();
	}
	glPopMatrix();
}

void NStockBar::DrawPolyPipe(enum RenderMode GlobRMode)
{
	glPushMatrix();
	if (NCMComData::IsTurnEnabled())
	{
		glTranslated(0., 0., BaseCenter.GetZ());
		glRotated(90., 1., 0., 0.);
	}
	else
	{
		glTranslated(0., BaseCenter.GetZ(), 0.);
	}
	glRotated(-RotAngle, 0., 1., 0.);

	BPoint Points[5];
	BPoint Normals[5];
	int PointsNum = 0;

	FillPN(&PointsNum, Points, Normals);

	double HalfAngleDeg = 180. / PolyNum;
	BMatr M;
	M.RotY(BPoint(0.,0.,0.,1.), -2. * HalfAngleDeg);

	bool DrawEdges = GetEdgesS() && (NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::BSP)
		|| GetEdgesDX() && (NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::DX);


	if (DrawEdges)
	{
		glPushAttrib(GL_ENABLE_BIT);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1., 1.);
	}
	for(int i = 0; i < PolyNum; ++i)
	{
		glRotated(2. * HalfAngleDeg, 0., 1., 0.);
		glBegin(GL_QUADS);
		for(int k = 0; k < PointsNum - 1; ++k)
		{
			BPoint P0(Points[k]), P1(Points[k + 1]);
			BPoint N(Normals[k]);
			glNormal3dv(N.GetArray());
			glVertex3dv(P0.GetArray());
			glVertex3dv(P1.GetArray());
			BPoint P2(P1 * M);
			BPoint P3(P0 * M);
			glVertex3dv(P2.GetArray());
			glVertex3dv(P3.GetArray());
		}
		glEnd();
	}
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
		if (EdgesAttrs.GetEdgesWidth() > 0.01)
		{
			glPushAttrib(GL_CURRENT_BIT);
			glColor4fv(EdgesAttrs.GetEdgesColor().array);
			glLineWidth(GLfloat(EdgesAttrs.GetEdgesWidth()));
			for (int i = 0; i < PolyNum; ++i)
			{
				glRotated(2. * HalfAngleDeg, 0., 1., 0.);
				glBegin(GL_LINES);
				for (int k = PointsNum - 2; k > 0; k -= 2)
				{
					BPoint P0(Points[k]), P1(Points[k - 1]);
					glVertex3dv(P0.GetArray());
					glVertex3dv(P1.GetArray());
					BPoint P2(P1 * M);
					BPoint P3(P0 * M);
					glVertex3dv(P1.GetArray());
					glVertex3dv(P2.GetArray());
					glVertex3dv(P0.GetArray());
					glVertex3dv(P3.GetArray());
				}
				glEnd();
			}
			glPopAttrib();
		}
		if (GlobRMode == RM_TRANSLUCENT)
			glPopAttrib();
		else
			BaseRender::RemoveWFMode();
	}
	glPopMatrix();
}

void NStockBar::Serialize(CArchive &ar)
{
	NStock::Serialize(ar);
	SerializeElements(ar,&ExtRad0,1);
	SerializeElements(ar,&ExtRad1,1);
	SerializeElements(ar,&IntRad0,1);
	SerializeElements(ar,&IntRad1,1);
	SerializeElements(ar,&Length,1);
	SerializeElements(ar,&PolyNum,1);
	SerializeElements(ar,&RotAngle,1);
	SerializeElements(ar,&BaseCenter,1);
	if(ar.IsLoading())
		Create();
}
void NStockBar::Create()
{
	NStock::Create();
	Gabar.Init();
	double MaxRad = max(ExtRad0, ExtRad1);
	BPoint MinPoint(-MaxRad, -MaxRad, BaseCenter.GetZ(), 0.);
	BPoint MaxPoint(MaxRad, MaxRad, Length + BaseCenter.GetZ(), 0.);
	if (!NCMComData::IsTurnEnabled())
	{
		BMatr R;
		R.RotX(0., 0., 0., -90.);
		MinPoint = MinPoint * R;
		MaxPoint = MaxPoint * R;
	}
	Gabar.Expand(MinPoint);
	Gabar.Expand(MaxPoint);
	Gabar.Transform(GetTransfMatr());
}
void NStockBar::Create( int iPolyNum, double iExtRad0, double iExtRad1
		, double iIntRad0, double iIntRad1
		, double iLength
		, double iRotAngle
		, const BPoint &iBaseCenter
		, int iAxis)
{
	PolyNum = iPolyNum;
	ExtRad0 = iExtRad0 * 0.5;
	ExtRad1 = iExtRad1 * 0.5;
	IntRad0 = iIntRad0 * 0.5;
	IntRad1 = iIntRad1 * 0.5;
	Length = iLength;
	RotAngle = iRotAngle;
	BaseCenter = iBaseCenter;
	Axis = iAxis;
	Create();
}
bool NStockBar::CreateBody(void)
{
	int Num = (NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::BSP) ?
		int(BDebugState::Get().CellSize + 0.5) :
		int(BDebugState::Get().DXNumber + 0.5);
	BPoint DeltaMin(-ST_DEC, -ST_DEC, -ST_DEC, 0.);
	BPoint DeltaMax(ST_INC, ST_INC, ST_INC, 0.);
	// Create meshs for bar stock cylinder
	MeshArr MeshArray;

	BPoint Pn(Gabar.GetMinPoint());
	BPoint Px(Gabar.GetMaxPoint());
	BPoint MinPointRot((Pn.GetX() + Px.GetX()) * 0.5, Pn.GetZ() + DeltaMin.GetZ() * 10., -1., 1.);
	BPoint MaxPointRot(Px.GetX() + DeltaMax.GetX() * 10., Px.GetZ() + DeltaMax.GetZ() * 10., 1., 1.);
	if (NCMComData::IsTurnEnabled())
	{
		// Create body for turn

		MeshArray.RemoveAll();

		double CosHalfAngle = 1.;
		double HalfAngle = 0.;
		if (PolyNum != 0)
		{
			HalfAngle = PI / PolyNum;
			CosHalfAngle = cos(HalfAngle);
		}

		BPoint Cont[4];
		Cont[0].Set(IntRad0 * CosHalfAngle, BaseCenter.GetZ(), -2., 1.);
		Cont[1].Set(ExtRad0, BaseCenter.GetZ(), -2., 1.);
		Cont[2].Set(ExtRad1, Length + BaseCenter.GetZ(), -2., 1.);
		Cont[3].Set(IntRad1 * CosHalfAngle, Length + BaseCenter.GetZ(), -2., 1.);
		NMakeStdMesh::BoxBounds(MeshArray
			, Cont
			, 4
			, 4.);

		int Num1 = BDebugState::Get().UseSlicedTree ? 1 : Num;

		MQuadTreeRot *pTreeRot = new MQuadTreeRot(MinPointRot, MaxPointRot, Num, Num1, GetStockGr(), false);// SlicedTree


		for (int l = 0; l < MeshArray.GetSize(); ++l)
		{
			pTreeRot->StoreMesh(*MeshArray[l], MElemIDProc::ID_NULL, cadrID(-1), false);
			delete MeshArray[l];
		}
		BBox Bounds;
		Bounds.Expand(MinPointRot);
		Bounds.Expand(MaxPointRot);
		pTreeRot->CreateBodyStd(Bounds, true, false);
		pTreeRot->Invert(false);
		pTreeRot->CrStandard(false);

		if (BSPTNumRot < 0)
			BSPTNumRot = MFOREST.AddTree(pTreeRot);
		else
		{
			MFOREST.DeleteTree(BSPTNumRot);
			MFOREST.SetTree(BSPTNumRot, pTreeRot);
		}
		SetStockType(AT_TURN);
		CreateRenderRot();
	}
	else
	{
		RemoveRotBody();
		SetStockType(AT_MILL);
	}
	if((NCMComData::IsMillEnabled() ||	PolyNum > 0) && NCMComData::IsTurnEnabled())
	{// Mill-turn project
		SetStockType(AT_MILL_TURN);
		// Create body for mill
		BBox Box;
		Box.Expand(BPoint(-MaxPointRot.GetX()
			, MinPointRot.GetY()
			, -MaxPointRot.GetX()
			, 1.));
		Box.Expand(BPoint(MaxPointRot.GetX() + 1.
			, MaxPointRot.GetY()
			, MaxPointRot.GetX() + 1.
			, 1.));

		switch (NCM_PROJECT.GetGlobalState().GetModelMode())
		{
			case NGlobalState::DX:
			{
				MakeBoxDX5X(Num, Box);
				break;
			}
			case NGlobalState::BSP:
			{
				MBody *pNewBody = new MQuadTree(Box.GetMinPoint(), Box.GetMaxPoint(), Num, Num, GetStockGr(), false);

				if (BSPTNum < 0)
					BSPTNum = MFOREST.AddTree(pNewBody);
				else
				{
					MFOREST.DeleteTree(BSPTNum);
					MFOREST.SetTree(BSPTNum, pNewBody);
				}
				break;
			}
		}

		if (PolyNum == 0)
		{
			if (NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::DX)
			{
				DXSubModel5X *pBody = (DXSubModel5X *)MFOREST.GetTree(BSPTNumDX);
				if (pBody)
					pBody->Reset();
			}
			TurnStockChanged = true;
			IntersectBodyes();
		}
		else
			MakePolyPipe();
		CreateRenderMill();
	}
	else if (!NCMComData::IsTurnEnabled())
	{// Mill project
		double MaxRad = max(ExtRad0, ExtRad1);
		bool PolyNumChanged = false;
		if (PolyNum == 0)
		{
			PolyNumChanged = true;
			PolyNum = 2 * NSurfSettings::GetNumAproxLine(MaxRad, 360.);
		}
		SetStockType(AT_MILL);
		// Create body for mill
		BPoint DeltaMin(-ST_DEC, -ST_DEC, -ST_DEC, 0.);
		BPoint DeltaMax(ST_INC, ST_INC, ST_INC, 0.);
		BBox Box;
		Box.Expand(Gabar.GetMinPoint() + DeltaMin);
		Box.Expand(Gabar.GetMaxPoint() + DeltaMax);
		switch (NCM_PROJECT.GetGlobalState().GetModelMode())
		{
		case NGlobalState::DX:
		{
			int DXNum = int(BDebugState::Get().DXNumber + 0.5);
			MakeBoxDX5X(DXNum, Box);
			break;
		}
		case NGlobalState::BSP:
		{
			MBody *pNewBody = new MQuadTree(Box.GetMinPoint(), Box.GetMaxPoint(), Num, Num, GetStockGr(), false);

			if (BSPTNum < 0)
				BSPTNum = MFOREST.AddTree(pNewBody);
			else
			{
				MFOREST.DeleteTree(BSPTNum);
				MFOREST.SetTree(BSPTNum, pNewBody);
			}
			break;
		}
		}

		if (PolyNum == 0)
		{
			if (NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::DX)
			{
				DXSubModel5X *pBody = (DXSubModel5X *)MFOREST.GetTree(BSPTNumDX);
				if (pBody)
					pBody->Reset();
			}
			TurnStockChanged = true;
			IntersectBodyes();
		}
		else
			MakePolyPipe();

		if (PolyNumChanged)
			PolyNum = 0;

		CreateRenderMill();
	}
	else
	{
		RemoveMillBody();
		SetStockType(AT_TURN);
	}
	MillStockChanged = false;
	TurnStockChanged = false;
	return true;
}

bool NStockBar::MakePolyPipe(void)
{
	MQuadBody *pBody = (MQuadBody *)GetBodyMill();
	if (!pBody)
		return false;
	BBox Bounds;
	Bounds.Expand(pBody->GetMin());
	Bounds.Expand(pBody->GetMax());
	MeshArr MeshArray;
	MFOREST.SetMark();
	MakePolyPipeMesh(MeshArray);
	MFOREST.TransformFMark(GetTransfMatr());
	MFOREST.RemoveMark();
	switch (NCM_PROJECT.GetGlobalState().GetModelMode())
	{
		case NGlobalState::DX:
		{
			((DXSubModel5X *)pBody)->SetStoreInA(false);
			for (int k = 0; k < MeshArray.GetSize(); ++k)
			{
				if (!MeshArray[k])
					continue;
				MeshArray[k]->StoreMesh(*pBody, MeshArray[k]->GetMeshID(), cadrID(-1), false);
				delete MeshArray[k];
			}
			((DXSubModel5X *)pBody)->GetDirectionA(0).SetWindow(Bounds);
			((DXSubModel5X *)pBody)->GetDirectionA(1).SetWindow(Bounds);
			((DXSubModel5X *)pBody)->GetDirectionA(2).SetWindow(Bounds);
			((DXSubModel5X *)pBody)->SetStoreInA(true);
			break;
		}
		case NGlobalState::BSP:
		{

			MQuadTree *pMillTree = (MQuadTree *)pBody;
			if (!pMillTree)
				return false;
			for (int k = 0; k < MeshArray.GetSize(); ++k)
			{
				if (!MeshArray[k])
					continue;
				MeshArray[k]->StoreMesh(*pMillTree, MeshArray[k]->GetMeshID(), cadrID(-1), true);
				delete MeshArray[k];
			}
			pMillTree->CreateBodyStd(Bounds, false, true);
			MBSPInter Inter;
			pMillTree->SafeMerge(Inter);
			break;
		}
	}
	return true;
}
bool NStockBar::MakePolyPipeMesh(MeshArr &MeshArray)
{
	if(PolyNum <= 0)
		return false;

	int PointsNum = 0;
	BPoint Points[5];
	BPoint Normals[5];

	FillPN(&PointsNum, Points, Normals);
	BMatr Shift;
	Shift.Trans(BPoint(0.,0.,0.,1.), BPoint(0., BaseCenter.GetZ(), 0., 1.));
	for(int l = 0; l < PointsNum; ++l)
		Points[l] = Points[l] * Shift;


	double AngleDeg = 360. / PolyNum;

	NRectMesh PointMesh;
	if(IntRad0 > 0.)
	{
		PointMesh.SetSize(PolyNum + 1, PointsNum);
		for(int i = 0; i < PolyNum; ++i)
		{
			BMatr M;
			M.RotY(BPoint(0.,0.,0.,1.), -(RotAngle + i * AngleDeg));
			for(int k = 0; k < PointsNum; ++k)
				PointMesh.m_arr[i][k] = MFOREST.AddPoints(Points[k] * M);
		}
		for(int k = 0; k < PointsNum; ++k)
			PointMesh.m_arr[PolyNum][k] = PointMesh.m_arr[0][k];
			

		for(int l = 0; l < PolyNum; ++l)
			for(int m = 0; m < PointsNum - 1; ++m)
			{
				NRectMesh *pMesh = new NRectMesh;
				pMesh->m_CadrNum = -1;
				pMesh->m_sgn = 1;
				pMesh->m_IsDeg = 0;
				pMesh->SetMeshID(MElemIDProc::ID_NULL);
				pMesh->m_iSurf = MFOREST.AddOrigSurf(
					new SOPlane(
						Normals[m] * BMatr().RotY(BPoint(0.,0.,0.,1.), -(RotAngle + l * AngleDeg)) * -1.
						, *MFOREST.GetPoint(PointMesh.m_arr[l][m])));
				pMesh->SetSize(2, 2);
				pMesh->m_arr[1][0] = PointMesh.m_arr[l][m];
				pMesh->m_arr[0][0] = PointMesh.m_arr[l + 1][m];
				pMesh->m_arr[1][1] = PointMesh.m_arr[l][m + 1];
				pMesh->m_arr[0][1] = PointMesh.m_arr[l + 1][m + 1];
				MeshArray.Add(pMesh);
			}
	}
	else
	{
		PointMesh.SetSize(PolyNum + 1, 2);
		for(int i = 0; i < PolyNum; ++i)
		{
			BMatr M;
			M.RotY(BPoint(0.,0.,0.,1.), -(RotAngle + i * AngleDeg));
			for(int k = 0; k < 2; ++k)
				PointMesh.m_arr[i][k] = MFOREST.AddPoints(Points[k + 1] * M);
		}
		for(int k = 0; k < 2; ++k)
			PointMesh.m_arr[PolyNum][k] = PointMesh.m_arr[0][k];
		for(int l = 0; l < PolyNum; ++l)
			for(int m = 0; m < 1; ++m)
			{
				NRectMesh *pMesh = new NRectMesh;
				pMesh->m_CadrNum = -1;
				pMesh->m_sgn = 1;
				pMesh->m_IsDeg = 0;
				pMesh->SetMeshID(MElemIDProc::ID_NULL);
				pMesh->m_iSurf = MFOREST.AddOrigSurf(
					new SOPlane(
						Normals[m + 1] * BMatr().RotY(BPoint(0.,0.,0.,1.), -(RotAngle + l * AngleDeg)) * -1.
						, *MFOREST.GetPoint(PointMesh.m_arr[l][m])));
				pMesh->SetSize(2, 2);
				pMesh->m_arr[1][0] = PointMesh.m_arr[l][m];
				pMesh->m_arr[0][0] = PointMesh.m_arr[l + 1][m];
				pMesh->m_arr[1][1] = PointMesh.m_arr[l][m + 1];
				pMesh->m_arr[0][1] = PointMesh.m_arr[l + 1][m + 1];
				MeshArray.Add(pMesh);
			}
		{
			NRectMesh *pMesh = new NRectMesh;
			pMesh->m_CadrNum = -1;
			pMesh->m_sgn = 1;
			pMesh->m_IsDeg = 0;
			pMesh->SetMeshID(MElemIDProc::ID_NULL);
			pMesh->m_iSurf = MFOREST.AddOrigSurf(
				new SOPlane(BPoint(0., 1., 0., 0.), *MFOREST.GetPoint(PointMesh.m_arr[0][0])));
			pMesh->SetSize(1, PolyNum);
			for(int p = 0; p < PolyNum; ++p)
				pMesh->m_arr[0][PolyNum - p - 1] = PointMesh.m_arr[p][0];
			MeshArray.Add(pMesh);
		}
		{
			NRectMesh *pMesh = new NRectMesh;
			pMesh->m_CadrNum = -1;
			pMesh->m_sgn = 1;
			pMesh->m_IsDeg = 0;
			pMesh->SetMeshID(MElemIDProc::ID_NULL);
			pMesh->m_iSurf = MFOREST.AddOrigSurf(
				new SOPlane(BPoint(0., -1., 0., 0.), *MFOREST.GetPoint(PointMesh.m_arr[0][1])));
			pMesh->SetSize(1, PolyNum);
			for(int p = 0; p < PolyNum; ++p)
				pMesh->m_arr[0][p] = PointMesh.m_arr[p][1];
			MeshArray.Add(pMesh);
		}
	}
	return true;
}

bool NStockBar::FillPN(int * pPointsNum, BPoint * Points, BPoint * Normals)
{
	double HalfAngleDeg = 180. / PolyNum;
	double HalfAngle = PI / PolyNum;
	double xE0 = ExtRad0 * cos(HalfAngle);
	double zE0 = ExtRad0 * sin(HalfAngle);
	double xE1 = ExtRad1 * cos(HalfAngle);
	double zE1 = ExtRad1 * sin(HalfAngle);

	double xI0 = IntRad0 * cos(HalfAngle);
	double zI0 = IntRad0 * sin(HalfAngle);
	double xI1 = IntRad1 * cos(HalfAngle);
	double zI1 = IntRad1 * sin(HalfAngle);

	if(IntRad0 > 0.)
	{
		*pPointsNum = 5;
		Points[0].Set(xI1, Length, -zI1, 1.);
		Normals[0].Set(-Length, xI1 - xI0, 0., 0.);
		Normals[0] = Normals[0] * (1. / sqrt(Normals[0].D2()));
		Points[1].Set(xI0, 0., -zI0, 1.);
		Normals[1].Set(0., -1., 0., 0.);
		Points[2].Set(xE0, 0., -zE0, 1.);
		Normals[2].Set(Length, xE0 - xE1, 0., 0.);
		Normals[2] = Normals[2] * (1. / sqrt(Normals[2].D2()));
		Points[3].Set(xE1, Length, -zE1, 1.);
		Normals[3].Set(0., 1., 0., 0.);
		Points[4].Set(xI1, Length, -zI1, 1.);
		Normals[4].Set(1.,0.,0.,0.);// Dummy
	}
	else
	{
		*pPointsNum = 4;
		Points[0].Set(0., 0., 0., 1.);
		Normals[0].Set(0., -1., 0., 0.);
		Points[1].Set(xE0, 0., -zE0, 1.);
		Normals[1].Set(Length, xE0 - xE1, 0., 0.);
		Normals[1] = Normals[1] * (1. / sqrt(Normals[1].D2()));
		Points[2].Set(xE1, Length, -zE1, 1.);
		Normals[2].Set(0., 1., 0., 0.);
		Points[3].Set(0., Length, 0., 1.);
		Normals[3].Set(1.,0.,0.,0.);// Dummy
	}
	return true;
}

bool NStockBar::IsCone(void)
{
	return PolyNum == 0;
}

const BPoint NStockBar::RayCastingOr(const BPoint & P, const BPoint & V, double MinW, BPoint &N) const 
{
	return BPoint(0., 0., 0., -1.);
}

NStock::StockCrType NStockBar::GetType() const
{
	if (PolyNum != 0)
		return SCT_BAR;
	else
		return SCT_BAR_SM;
}

const BMatr NStockBar::GetTransfMatr(void) const
{
	if (NCMComData::IsTurnEnabled())
		return NMovableObj::Matr;

	BMatr Matr, B;
	Matr.Trans(BPoint(0., 0., BaseCenter.GetZ(), 1.), BaseCenter);
	switch (Axis)
	{
	case 0:// X
		Matr = B.RotZ(BPoint(0., 0., 0., 1.), -90.) * Matr;
		break;
	case 1:// Y
		break;
	case 2:// Z
		Matr = B.RotX(BPoint(0., 0., 0., 1.), 90.) * Matr;
		break;
	}

	return Matr * NMovableObj::Matr;
}

void NStockBar::Transformm(const BMatr & M)
{
	NMovableObj::Transformm(M);
	Create();
}