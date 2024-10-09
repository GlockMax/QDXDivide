#include "stdafx.h"
#include "ConstDef.h"
#include "math.h"
#include "MBSPForest.h"
#include "NTriMesh.h"
#include "MVRDS.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

MVRDS::MVRDS(void) : SphCenter(0., 0., 0., 1.)
{
	VNum = 0;
	ENum = 0;
	TNum = 0;
	VNumMax = 0;
	ENumMax = 0;
	TNumMax = 0;
	SphRadius = -1.;
}

MVRDS::~MVRDS(void)
{
}

int MVRDS::AddVertex(int Num, int ENum)
{// returns a number of added vertex or -1 if list is full
	if(VNum == VNumMax && VNumMax < MAX_VERTEXES)
	{
		VList[VNum].SetPointNum(Num);
		VList[VNum].SetEdgeNum(ENum);
		++VNum;
		++VNumMax;
		return VNum - 1;
	}
	for(; VNum < VNumMax; ++VNum) 
		if(!VList[VNum].IsDefined())
		{
			VList[VNum].SetPointNum(Num);
			VList[VNum].SetEdgeNum(ENum);
			++VNum;
			return VNum - 1;
		}
	for(VNum = 0 ; VNum < MAX_VERTEXES; ++VNum) 
		if(!VList[VNum].IsDefined())
		{
			VList[VNum].SetPointNum(Num);
			VList[VNum].SetEdgeNum(ENum);
			++VNum;
			return VNum - 1;
		}
	return -1;// VList is full
}
int MVRDS::AddEdge(int VNum1, int VNum2)
{// returns a number of added Edge or -1 if list is full
	MEdge Edge(this);
	Edge.SetVertexes(VNum1, VNum2);
	return AddEdge(Edge);
}
int MVRDS::AddEdge(const MEdge &Edge)
{// returns a number of added Edge or -1 if list is full
	if(ENum == ENumMax && ENumMax < MAX_EDGES)
	{
		EList[ENum] = Edge;
		++ENum;
		++ENumMax;
		return ENum - 1;
	}
	for(; ENum < ENumMax; ++ENum) 
		if(!EList[ENum].IsDefined())
		{
			EList[ENum] = Edge;
			++ENum;
			return ENum - 1;
		}
	for(ENum = 0 ; ENum < MAX_EDGES; ++ENum) 
		if(!EList[ENum].IsDefined())
		{
			EList[ENum] = Edge;
			++ENum;
			return ENum - 1;
		}
	return -1;// EList is full
}
int MVRDS::AddTriFace(const MTriFace &TriFace)
{// returns a number of added TriFace or -1 if list is full
	if(TNum == TNumMax && TNumMax < MAX_TRIFACES)
	{
		TList[TNum] = TriFace;
		++TNum;
		++TNumMax;
		return TNum - 1;
	}
	for(; TNum < TNumMax; ++TNum) 
		if(!TList[TNum].IsDefined())
		{
			TList[TNum] = TriFace;
			++TNum;
			return TNum - 1;
		}
	for(TNum = 0 ; TNum < MAX_TRIFACES; ++TNum) 
		if(!TList[TNum].IsDefined())
		{
			TList[TNum] = TriFace;
			++TNum;
			return TNum - 1;
		}
	return -1;// TList is full
}

void MVRDS::Clear(void)
{
	int i;
	for( i = 0; i < VNumMax; ++i)
		VList[i] = MVertex();
	for( i = 0; i < ENumMax; ++i)
		EList[i] = MEdge();
	for( i = 0; i < TNumMax; ++i)
		TList[i] = MTriFace();
	VNum = 0;
	ENum = 0;
	TNum = 0;
	VNumMax = 0;
	ENumMax = 0;
	TNumMax = 0;
}

bool MVRDS::MakeDelaunayTri()
{// VList must contain all points
	// EList must contain single edge of convex hull
	if(VNumMax < 3 || ENumMax < 1 || SphRadius < 0.)
		return false;
	// While active edge exists
	for( int EdgeNum = GetActiveEdgeNum(); EdgeNum >= 0; EdgeNum = GetActiveEdgeNum())
	{
		const MEdge &CurEdge = GetEdge(EdgeNum);
		// Find vertex for a delaunay triangle
		double Rad = (SphRadius == 0.) ? MAX_RAD : SphRadius * 0.9;
		BPoint CircumCenter(0., 0., 0., 1.);
		int CCPointsSize = 0;
		int CCPoints[1024];
		for(int i = 0; i < VNumMax; ++i)
		{
			BPoint CurC(0., 0., 0., 1.);
			double b = CurEdge.GetSignedRad(
				*MFOREST.GetPoint(VList[i].GetPointNum()), SphCenter, SphRadius,
				CurC);
			if( b < Rad + DMIN)
			{
				if(b < Rad - DMIN)
					CCPointsSize = 0;
				Rad = b;
				CircumCenter = CurC;
				CCPoints[CCPointsSize++] = i;
			}
		}
		// Make edges and faces
		// CCPointsSize > 1 if 4 or more points are positioned on the same circle
		if(CCPointsSize < 1)// Vertex not found
		{
			EList[EdgeNum].SetLeftTriNum(MEdge::EMPTY);
			continue;
		}
		if(CCPointsSize == 1)
		{
			if(!AddNewTriFace(EdgeNum, CCPoints[0], CircumCenter, Rad))
				return false;
		}
		else
		{// 4 or more points lie on the same circle
			//Should be processed
			if(!AddNewTriFace(EdgeNum, CCPoints[0], CircumCenter, Rad))
				return false;
		}

	}
	return true;
}
const BPoint * MVRDS::GetPoint(int VertexNum)
{
	return MFOREST.GetPoint(VList[VertexNum].GetPointNum());
}

int MVRDS::AddActiveEdge(const MEdge & Edge)
{
	for(int i = 0; i < ENumMax; ++i)
	{
		if(EList[i].IsDefined())
			if(EList[i].Join(Edge))
				return i;
	}

	int k = AddEdge(Edge);
	if( k < 0)
		return k;
	EList[k].SetActive();
	return k;
}

int MVRDS::GetActiveEdgeNum(void)
{// returns first active edge number if any 
	// or -1 otherwize
	// removes active flag from returned edge
	for(int i = 0; i < ENumMax; ++i)
	{
		if(EList[i].IsActive() && EList[i].IsDefined())
		{
			EList[i].RemoveActive();
			return i;
		}
	}
	return -1;
}
int MVRDS::GetActiveTriFaceNum(void)
{// returns first active TriFace number if any 
	// or -1 otherwize
	// removes active flag from returned TriFace
	for(int i = 0; i < TNumMax; ++i)
	{
		if(TList[i].IsActive())
		{
			TList[i].RemoveActive();
			return i;
		}
	}
	return -1;
}

MEdge & MVRDS::GetEdge(int Num)
{
	return EList[Num];
}
INT_PTR MVRDS::StoreInMesh(NTriMesh &Mesh)
{
	int i = 0, k = 0;
	for(; i < TNumMax; ++i)
	{
		if(TList[i].IsDefined())
			++k;
	}
	Mesh.Triangles.resize(k);
	for(i = 0, k = 0; i < TNumMax; ++i)
	{
		if(TList[i].IsDefined())
		{
			Mesh.Triangles[k][0] = TList[i].GetPointNum(0);
			Mesh.Triangles[k][1] = TList[i].GetPointNum(1);
			Mesh.Triangles[k][2] = TList[i].GetPointNum(2);
			++k;
		}
	}
	Mesh.m_sgn = 1;
	return Mesh.Triangles.size();
}
void MVRDS::SetSphere(const BPoint &Center, double Rad)
{
	SphCenter = Center;
	SphRadius = Rad;
}

int MVRDS::MakeLimTolTri(double SphTolerance , const BPoint & SphC, double SphRad)
{
	if(SphRad <= 0.)
		return -1;
	if(SphTolerance <= DMIN)
		return -2;

	SetSphere(SphC, SphRad);
	double MaxRad = sqrt((2.*SphRadius - SphTolerance) * SphTolerance);
	double MaxLength = 2 * MaxRad;

	if(ENumMax == 1)// if EnumMax > 1 all active edges must be right oriented
	{
		// Orient start edge
		if(EList[0].GetRightTriN() != MEdge::EMPTY)
		{
			ASSERT(EList[0].GetLeftTriN() == MEdge::EMPTY);
			ASSERT(EList[0].GetRightTriN() == MEdge::NOT_DEFINED);
			EList[0].Invert();
		}
	}

	
	if(!MakeDelaunayTri())
		return -1;
#ifdef _DEBUG
	Check();
#endif
	while(1)
	{
		double MaxELength2;
		int MaxENum = GetMaxLengthEdgeNum(&MaxELength2);
		if(MaxELength2 <= 0.)
			return -1;
		if(MaxELength2 < MaxLength * MaxLength * 4)
			return 0;
		if(!RemoveEdgePatch(MaxENum))
			return 0;
#ifdef _DEBUG
		Check();
#endif
	}
	return 0;
}

int MVRDS::GetMaxLengthEdgeNum(double *pMaxLength)
{
	*pMaxLength = -1.;
	int iMaxLength = -1;
	for(int i = 0; i < ENumMax; ++i)
	{
		const MEdge & Edge = EList[i];
		if(!Edge.IsDefined())
			continue;
		if(Edge.GetRightTriN() == MEdge::EMPTY)
			continue;
		if(Edge.GetLeftTriN() == MEdge::EMPTY)
			continue;
		double Length = Edge.GetLength2();
		if(Length > *pMaxLength)
		{
			*pMaxLength = Length;
			iMaxLength = i;
		}
	}
	return iMaxLength;
}

const MVertex & MVRDS::GetVertex(int Num)
{
	return VList[Num];
}

bool MVRDS::RemoveEdgePatch(int Num)
{
	BPoint NewPoint = EList[Num].GetMiddle();
	int NewVertex = AddVertex(MFOREST.AddPoints(NewPoint));
	if(NewVertex < 0)
		return false;
	// Remove incident TriFaces and orient active edges
	int iT = EList[Num].GetLeftTriN();
	for(int i = 0; i < 3; ++i)
	{
		int iE = TList[iT].GetEdgeNum(i);
		if(iE != Num)
		{
			EList[iE].SetActive();
			if(EList[iE].GetLeftTriN() != iT)
				EList[iE].Invert();
			EList[iE].SetLeftTriNum(MEdge::NOT_DEFINED);
		}
	}
	TList[iT].Undefine();
	iT = EList[Num].GetRightTriN();
	for(int i = 0; i < 3; ++i)
	{
		int iE = TList[iT].GetEdgeNum(i);
		if(iE != Num)
		{
			EList[iE].SetActive();
			if(EList[iE].GetLeftTriN() != iT)
				EList[iE].Invert();
			EList[iE].SetLeftTriNum(MEdge::NOT_DEFINED);
		}
	}
	TList[iT].Undefine();
	// Remove divided edge
	EList[Num].Undefine();

	bool StopFlag = false;
	while(!StopFlag)
	{
		StopFlag = true;
		for(int iAE = 0; iAE < ENumMax; ++iAE)
		{
			if(!EList[iAE].IsActive())
				continue;
			int iAT = EList[iAE].GetRightTriN();
			if(iAT == MEdge::EMPTY || iAT == MEdge::NOT_DEFINED)
				continue;
			double R = TList[iAT].GetRadius();
			if((NewPoint - TList[iAT].GetCenter()).D2() > R * R)
				continue;
			StopFlag = false;
			for(int k = 0; k < 3; ++k)
			{
				int iE = TList[iAT].GetEdgeNum(k);
				if(iE != iAE)
				{
					EList[iE].SetActive();
					if(EList[iE].GetLeftTriN() != iAT)
					{
						if(EList[iE].GetRightTriN() != iAT)
							return false; // internal error
						EList[iE].Invert();
					}
					EList[iE].SetLeftTriNum(MEdge::NOT_DEFINED);
				}
			}
			TList[iAT].Undefine();
			EList[iAE].Undefine();
		}
	}
	// Patch up the hole
	int ActiveEList[MAX_EDGES];
	int ASize = 0;
	for( int EdgeNum = 0; EdgeNum < ENum; ++EdgeNum)
	{
		if(EList[EdgeNum].IsActive())
		{
			EList[EdgeNum].RemoveActive();
			ActiveEList[ASize++] = EdgeNum; // This is needed to ignore new active edges produced in AddNewTriFace
		}
	}
	for(int i = 0; i < ASize; ++i)
	{
		BPoint CircumCenter(0., 0., 0., 1.);
		double Rad = EList[ActiveEList[i]].GetSignedRad(
			*MFOREST.GetPoint(VList[NewVertex].GetPointNum()), SphCenter, SphRadius,
			CircumCenter);
		if(!AddNewTriFace(ActiveEList[i], NewVertex, CircumCenter, Rad))
			return false;
	}

	return true;
}

bool MVRDS::Check(void) const
{
	for(int iV = 0; iV < VNumMax; ++iV)
	{
		if(!VList[iV].IsDefined())
			continue;
		int iE = VList[iV].GetEdgeNum();
		if(iE >= ENumMax || iE < -1)
			return false;
		if(iE >= 0)
			if(EList[iE].GetEndVN() != iV && EList[iE].GetStartVN() != iV)
				return false;
	}
	for(int iT = 0; iT < TNumMax; ++iT)
	{
		if(!TList[iT].IsDefined())
			continue;
		for(int i = 0; i < 3; ++i)
		{
			int iE = TList[iT].GetEdgeNum(i);
			if(iE >= ENumMax || iE < -1)
				return false;
			if(iE >= 0)
				if(EList[iE].GetLeftTriN() != iT && EList[iE].GetRightTriN() != iT)
					return false;
		}
	}
	return true;
}

void MVRDS::Restart(void)
{
	// Remove all edges except the first one and all faces
	EList[0].SetActive();
	EList[0].SetLeftTriNum(MEdge::NOT_DEFINED);
	for(int i = 1; i < ENumMax; ++i)
		EList[i].Undefine();
	for(int i = 0; i < TNumMax; ++i)
		TList[i].Undefine();
}

bool MVRDS::AddNewTriFace(int EdgeNum, int NewVertex, const BPoint &CircumCenter, double Rad)
{
	MEdge &CurEdge = EList[EdgeNum];
	int NewFaceNum = AddTriFace(MTriFace());
	if(NewFaceNum < 0)
		return false;
	int NewEdge2 = AddActiveEdge(MEdge(this,
		NewVertex, CurEdge.GetEndVN(),
		MEdge::NOT_DEFINED, NewFaceNum));
	if(NewEdge2 < 0)
		return false;
	int NewEdge1 = AddActiveEdge(MEdge(this,
		CurEdge.GetStartVN(), NewVertex, 
		MEdge::NOT_DEFINED, NewFaceNum));
	if(NewEdge1 < 0)
		return false;
	CurEdge.SetLeftTriNum(NewFaceNum);
	TList[NewFaceNum].Set(this,
		EdgeNum, NewEdge2, NewEdge1,
		VList[CurEdge.GetStartVN()].GetPointNum(), 
		VList[CurEdge.GetEndVN()].GetPointNum(), 
		VList[NewVertex].GetPointNum(),
		CircumCenter, Rad);
	VList[NewVertex].SetEdgeNum(NewEdge2);
	return true;
}