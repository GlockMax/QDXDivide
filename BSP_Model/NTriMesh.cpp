#include "stdafx.h"
#include "BBox.h"
#include "MBody.h"
#include "MFace.h"
#include "DXSubModel5X.h"
#include "NTriMesh.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

NTriMesh::NTriMesh(void)
{
}

NTriMesh::~NTriMesh(void)
{
	NParamMesh::~NParamMesh();
}
void NTriMesh::StoreMesh(MBody & Body, ElemID ElID, cadrID CaID, bool ToAux) const
{
	// Process single body

	MElemIDProc::SetSurfID(&ElID,m_iSurf);
	MFace Face;
	for(int i = 0; i < Triangles.size(); ++i)
	{
		int n = MFOREST.CalcNormalSafe(
			Triangles[i][0], Triangles[i][1], Triangles[i][2], m_sgn); 
		if (n < 0)
			continue;
		Face.SetPlane(MPlane(n, Triangles[i][0]));
		Face.SetArraySize(3);
		Face.SetPointAt(0, Triangles[i][0]);
		Face.SetPointAt(1, Triangles[i][1]);
		Face.SetPointAt(2, Triangles[i][2]);
		Body.StoreFace(&Face, ElID, CaID, ToAux);
	}
}
void NTriMesh::StoreMesh2Aux(class DXSubModel5X &Mod5, ElemID ElID, cadrID CaID) const
{
	MElemIDProc::SetSurfID(&ElID,m_iSurf);
	MFace Face;
	for (int d = 0; d < 3; ++d)
	{
		DXDirection3X &Dir = Mod5.GetAuxDir(d);
		for (int i = 0; i < Triangles.size(); ++i)
		{
			int n = MFOREST.CalcNormal(
				Triangles[i][0], Triangles[i][1], Triangles[i][2], m_sgn);
			Face.SetPlane(MPlane(n, Triangles[i][0]));
			Face.SetArraySize(3);
			Face.SetPointAt(0, Triangles[i][0]);
			Face.SetPointAt(1, Triangles[i][1]);
			Face.SetPointAt(2, Triangles[i][2]);
			Dir.StoreFace(&Face, DX_ID(ElID, CaID));
		}
	}
}
const BBox & NTriMesh::Expand(BBox & Gab) const
{
	for(int i = 0 ; i < Triangles.size(); ++i)
	{
		Gab.Expand(*MFOREST.GetPoint(Triangles[i][0]));
		Gab.Expand(*MFOREST.GetPoint(Triangles[i][1]));
		Gab.Expand(*MFOREST.GetPoint(Triangles[i][2]));
	}
	return Gab;
}

int NTriMesh::AddTri(int i0, int i1, int i2)
{
	int Size = int(Triangles.size());
	if(i0 == i1 || i0 == i2 || i1 == i2)
		return Size;
	Triangles.resize(Size + 1);
	Triangles[Size][0] = i0;
	Triangles[Size][1] = i1;
	Triangles[Size][2] = i2;
	return Size + 1;
}

void NTriMesh::Invert(void)
{
	for(int i = 0; i < Triangles.size(); ++i)
	{
		int b = Triangles[i][0];
		Triangles[i][0] = Triangles[i][1];
		Triangles[i][1] = b;
	}
}

NParamMesh * NTriMesh::CopyTransform(CopyMap &Map) const
{
	NTriMesh *pNewMesh = new NTriMesh(*this);
	FillCopy(*pNewMesh, Map);
	return nullptr;
}
