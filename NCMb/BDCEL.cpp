#include "..\Common\BDCEL.h"
#include "stdafx.h"
#include "GL\GL.h"
#include "ConstDef.h"
#include <map>
#include "SimpleCloud.h"
#include "DgmOctree.h"
#include "..\compbase\ReferenceCloud.h"
#include "BDCEL.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

const double BDCEL::PointTol = MIND * 10.;

using namespace CCLib;

BDCEL::BDCEL(void) : Edges(BEdges)
{
	EdgesNum = 0;
	FacesNum = 0;
	BorderEdges = 0;
	pC = NULL;
	SOURCE_VERTEX_SIZE = 6;
	SetSmoothAngle(15.);
	TRMMode = false;
	TripleEdgesNum = 0;
}

BDCEL::~BDCEL(void)
{
}

double BDCEL::CalcDist(int Ind) const
{
	const float *pData = GetBasePtrByStorNum(Ind) + 3;
	return CALC_D(pData[0], pData[1], pData[2]);
}

int BDCEL::compare_points_by_d( int a, int b )
{
	double da = CalcDist(a);
	double db = CalcDist(b);
	if(da < db)
		return -1;
	return (da == db) ? 0 : 1;
}
int BDCEL::compare_edges_by_SVn( int a, int b )
{
	int Inda = a;
	int Indb = b;
	int SVna = BufEdges[Inda][0];
	int SVnb = BufEdges[Indb][0];
	if(SVna < SVnb)
		return -1;
	return (SVna == SVnb) ? 0 : 1;
}
int BDCEL::compare_edges_by_EVn( int a, int b )
{
	int Inda = a;
	int Indb = b;
	int EVna = BufEdges[BufEdges[Inda][1]][0];
	int EVnb = BufEdges[BufEdges[Indb][1]][0];
	if(EVna < EVnb)
		return -1;
	return (EVna == EVnb) ? 0 : 1;
}
int BDCEL::compare_edges_by_2Vn( int a, int b )
{
	// Сравнение по двум полям: меньший номер вершины; больший номер вершины
	// при равенстве обоих полей меньше тот, у которого меньше его индекс (не обязательно). 
	// Последнее сделано для случая, когда тела в СТЛ расположены последовательно, т.е. не перемешаны
	if(a == b) 
		return 0;
	int Inda = a;
	int Indb = b;
	int MinA = GetESVn(Inda);
	int MaxA = GetEEVn(Inda);
	if(MinA > MaxA) { int buf = MinA; MinA = MaxA; MaxA = buf;}
	int MinB = GetESVn(Indb);
	int MaxB = GetEEVn(Indb);
	if (MinB > MaxB) { int buf = MinB; MinB = MaxB; MaxB = buf; }
	

	if(MinA < MinB)
		return -1;
	if(MinA > MinB)
		return 1;

	if(MaxA < MaxB)
		return -1;
	if(MaxA > MaxB)
		return 1;
	// edges are equal
	if (a < b)
		return -1;
	else
		return 1;
}

int BDCEL::MakeFromTriangles(const float *pCI, const unsigned int *pTriInds, size_t TrianglesNum, bool DoOrient)
{
 	if(pCI == NULL)
		return 0;
	auto NTri = TrianglesNum;
	if(NTri < 1)
		return 0;
	pC = pCI;
	std::vector<int> PNums(3 * NTri);// source point numbers array
	size_t MaxPointIndex = 0;
	// Remove degraded triangles
	size_t PointsNumber = 0;// A number of source points after MinTriH test
	for(size_t i = 0; i < 3 * NTri; i += 3)
	{
		if(MinTriH2(GetPointByNum(i), GetPointByNum(i + 1), GetPointByNum(i + 2)) > PointTol * PointTol)
		{
			for (int k = 0; k < 3; ++k)
			{
				auto b = pTriInds[i + k];
				MaxPointIndex = max(MaxPointIndex, b);
				PNums[PointsNumber++] = b;
			}
		}
	}
	for(size_t j = PointsNumber; j < 3 * NTri; PNums[j++] = -1); // Clear the rest

	auto BackSize = max(MaxPointIndex + 1, 3 * NTri);
	std::vector<int> BackNums(BackSize);// back references
	// BackNums - связь между старыми и новыми номерами точек в хранилище точек (pPC). Для каждого старого номера в хранилище хранит новый номер вершины
	for (size_t j = 0; j < BackSize; BackNums[j++] = -1);
	int PointsNum = IdentifyPointsOc(PNums, PointsNumber, BackNums);// A number of points after identification
	// Init Vertexes
	Vertexes.resize(PointsNum);
	for(int i = 0; i < GetVertexCount(); ++i)
	{
		Vertexes[i][0] = -1;
		Vertexes[i][1] = -1;
		Vertexes[i][2] = -1;
	}
	for (unsigned int ip = 0; ip < MaxPointIndex + 1; ++ip)
	{
		if(BackNums[ip] >= 0)
			if(Vertexes[BackNums[ip]][2] < 0) // Is not filled yet
				Vertexes[BackNums[ip]][2] = ip;
	}
	// Fill half edges
	Edges.resize(PointsNumber);
	int OldEdgesNum = 0;
	for (size_t ti = 0; ti < 3 * NTri; ti += 3)
	{
		if(BackNums[PIndByIInd(ti)] < 0)
			continue;
		if(MinTriH2(GetPointByNum(ti), GetPointByNum(ti + 1), GetPointByNum(ti + 2)) <= PointTol * PointTol)
			continue;
		// Orient edges
		int n0 = 0;
		int n1 = 1;
		int n2 = 2;
		BPoint N = (GetPointByNum(ti + 1) - GetPointByNum(ti)) % (GetPointByNum(ti + 2) - GetPointByNum(ti + 1));
		if(N * BDCEL::GetNormByNum(ti) < 0)
		{
			n0 = 2;
			n2 = 0;
			N = N * (-1.);
		}
		StoreNormal(ti, N);
		// Fill
		int FaceInd = OldEdgesNum / 3;

		Edges[OldEdgesNum + 0][0] = BackNums[PIndByIInd(ti + n0)];
		Edges[OldEdgesNum + 0][1] = OldEdgesNum + 1;
		Edges[OldEdgesNum + 0][2] = FaceInd;

		Edges[OldEdgesNum + 1][0] = BackNums[PIndByIInd(ti + n1)];
		Edges[OldEdgesNum + 1][1] = OldEdgesNum + 2;
		Edges[OldEdgesNum + 1][2] = FaceInd;

		Edges[OldEdgesNum + 2][0] = BackNums[PIndByIInd(ti + n2)];
		Edges[OldEdgesNum + 2][1] = OldEdgesNum + 0;
		Edges[OldEdgesNum + 2][2] = FaceInd;

		OldEdgesNum += 3;
	}

	// Sort edges
	for(int i = 0; i < OldEdgesNum; ++i)
		PNums[i] = i; // Edges nums now
	QuickSort(PNums.data(), OldEdgesNum, edges_by_2Vn);
	// Find triple edges
	TripleEdgesNum = 0;
	for(int i = 0; i < OldEdgesNum - 2; ++i)
	{
		if(GetESVn(PNums[i]) == GetEEVn(PNums[i + 1]) && GetEEVn(PNums[i]) == GetESVn(PNums[i + 1])
		|| GetESVn(PNums[i]) == GetESVn(PNums[i + 1]) && GetEEVn(PNums[i]) == GetEEVn(PNums[i + 1]))
		{
			if(GetESVn(PNums[i + 2]) == GetEEVn(PNums[i + 1]) && GetEEVn(PNums[i + 2]) == GetESVn(PNums[i + 1])
			|| GetESVn(PNums[i + 2]) == GetESVn(PNums[i + 1]) && GetEEVn(PNums[i + 2]) == GetEEVn(PNums[i + 1]))
			++TripleEdgesNum;
		}
	}

	// Calculate new size
	bool TripleEdgeExists = (TripleEdgesNum > 0);
	int NewEdgesNum = 0;
	BorderEdges = 0;
	int k = 0;
	for(; k < OldEdgesNum - 1; ++k)
	{
		bool RightEdge = GetESVn(PNums[k]) == GetEEVn(PNums[k + 1]) && GetEEVn(PNums[k]) == GetESVn(PNums[k + 1]);
		bool InvEdge = GetESVn(PNums[k]) == GetESVn(PNums[k + 1]) && GetEEVn(PNums[k]) == GetEEVn(PNums[k + 1]);
		if(RightEdge || (InvEdge && !TripleEdgeExists)) // Right pair 
			++k;
		else
			++BorderEdges;
		++NewEdgesNum;
	}
	if(k < OldEdgesNum)
	{// Last edge has no pair
		++NewEdgesNum;
		++BorderEdges;
	}
	// Faces
	// Индекс - номер точки в исходной нумерации
	// [1] - номер нормали в исходной нумерации
	FacesNum = OldEdgesNum / 3;
	Faces.resize(FacesNum + BorderEdges);
	FacesNum = 0;
	for (size_t i = 0; i < Faces.size(); ++i)
		Faces[i][0] = -1;
	for(size_t i = 0; i < 3 * NTri; i += 3)
	{
		if(MinTriH2(GetPointByNum(i), GetPointByNum(i + 1), GetPointByNum(i + 2)) <= PointTol * PointTol)
			continue;

		Faces[FacesNum++][1] = i;
	}
	EdgesNum = NewEdgesNum * 2;
	BufEdges.resize(EdgesNum + BorderEdges * 2); // New edges will be inserted because of border edges existance
	for (size_t i = 0; i < BufEdges.size(); ++i)
		MakeDummy(BufEdges, i);
	int *BENums = new int[BorderEdges];// The numbers of border edges in Edges[*][3] ordered by start vertex num
	for(int fi = 0, ti = 0, bi = 0; fi < OldEdgesNum; ++fi, ti += 2)
	{// Place edge
		// fi - from index; ti - to index, bi - border index
		BufEdges[ti][0] = Edges[PNums[fi]][0];
		BufEdges[ti][1] = Edges[PNums[fi]][1];
		BufEdges[ti][2] = Edges[PNums[fi]][2];
		BackNums[PNums[fi]] = ti;
		// Check if pair exists
		if(fi == OldEdgesNum - 1)// last edge can not have pair
		{
			MakeDummy(BufEdges, ti + 1);
			BufEdges[ti][3] = 0;
			BENums[bi++] = ti;// We temporary store the border edge number in BENums
		}
		else
		{
			bool RightEdge = GetESVn(PNums[fi]) == GetEEVn(PNums[fi + 1]) && GetEEVn(PNums[fi]) == GetESVn(PNums[fi + 1]);
			bool InvEdge = GetESVn(PNums[fi]) == GetESVn(PNums[fi + 1]) && GetEEVn(PNums[fi]) == GetEEVn(PNums[fi + 1]);
			if(RightEdge || (InvEdge && !TripleEdgeExists)) 
			{
				++fi;
				BufEdges[ti + 1][0] = Edges[PNums[fi]][0];
				BufEdges[ti + 1][1] = Edges[PNums[fi]][1];
				BufEdges[ti + 1][2] = Edges[PNums[fi]][2];
				BufEdges[ti + 1][3] = -1;
				BufEdges[ti][3] = 1;
				BackNums[PNums[fi]] = ti + 1;
			}
			else
			{
				MakeDummy(BufEdges, ti + 1);
				BufEdges[ti][3] = 0;
				BENums[bi++] = ti;// We temporary store the border edge number in BENums
			}
		}
	}
	for(int i = 0; i < EdgesNum; ++i)
	{
		if(BufEdges[i][0] < 0) // is dummy
			continue;
		BufEdges[i][1] = BackNums[BufEdges[i][1]];
	}

	bool NeedHealBorders = true;
	//if(InvEdgeExists)
	if(NeedHealBorders)
	{
		HealBorders(BENums);
	}
	delete[] BENums;
	Edges.clear();
	Edges = BufEdges;
//	BufEdges = nullptr;

	for(int i = 0; i < EdgesNum; ++i)
	{
		if(IsDummyEdge(i))
			continue;
		Vertexes[Edges[i][0]][0] = i;
	}

	// Adjust faces orientation
	// Wave for faces orientation
	if (!TripleEdgeExists && DoOrient)
		DoOrientWave();
	BorderEdges = 0;
	for (int i = 0; i < EdgesNum; ++i)
	{
		if (IsDummyEdge(i))
			continue;
		Vertexes[Edges[i][0]][0] = i;// Edge number in a vertex may be incorrect due to edge reorientation in DoOrientWave
		Vertexes[Edges[i][0]][1] = Edges[i][2];
		Faces[Edges[i][2]][0] = i;
		if (Edges[i][3] == 0)
			++BorderEdges;
	}
	return 0;
}

void BDCEL::MakeDummy(std::vector<Edge> &EBuf, int Ind)
{
	EBuf[Ind][0] = -1;
	EBuf[Ind][1] = -1;
	EBuf[Ind][2] = -1;
	EBuf[Ind][3] = -2;
}

BPoint BDCEL::GetPointByNum(size_t Num) const
{
	// Get point from pC by it's index
	// Входной индекс это номер точки в исходной нумерации, т.е. по 3 штуки от 0 до 3*NTri - 1
	return GetPointByStorNum(PIndByIInd(Num));
}
BPoint BDCEL::GetPointByStorNum(int Num) const
{
	// Get point from pC by it's index
	// Входной индекс это номер точки непосредственно в pC
	const float *pData = GetBasePtrByStorNum(Num);
	return BPoint(
		double(pData[3])
		, double(pData[4])
		, double(pData[5])
		, 1.);
}
const float * BDCEL::GetBasePtrByNum(int Num) const
{
	return GetBasePtrByStorNum(PIndByIInd(Num));
}
const float * BDCEL::GetPointPtrByStorNum(int Num) const
{
	// Get point from pC by it's index
	// Входной индекс это номер точки непосредственно в pC
	return GetBasePtrByStorNum(Num) + 3;
}
const BPoint BDCEL::GetNormByNum(int Num) const
{
	// Get normal from pC by it's index
	const float *pData = GetBasePtrByStorNum(PIndByIInd(Num));
	return BPoint(
		  double(pData[0])
		, double(pData[1])
		, double(pData[2])
		, 0.);
}
const BPoint BDCEL::GetNormByEdge(int HalfEdgeInd) const
{
	BPoint n = GetEdgeVec(HalfEdgeInd) % GetEdgeVec(Edges[HalfEdgeInd][1]);
	n = n * (1. / sqrt(n.D2()));
	return n;
}

int * BDCEL::FillEdges(bool Sharp, bool Error, bool Smooth) const
{
	int *pRet = NULL;
	bool All = Sharp && Smooth;
//	All = true;

	if(All)
	{
		int Size = EdgesNum * 2 + 1;
		pRet = new int[Size];
		pRet[0] = Size;
		for(int i = 0; i < EdgesNum; i += 2)
		{
			FillOneEdge(i, pRet + i + 1);
		}
		return pRet;
	}
	int Size = 0;
	for(int i = 0; i < EdgesNum; i += 2)
	{
		if(Error)
		{
			if(IsBorderEdge(i))
			{
				++Size;
				continue;
			}
		}
		if(Sharp)
		{
			if(Edges[i][3] == 1)
			{
				if(IsSharpEdge(i))
					++Size;
			}
		}
	}
	if(Size == 0)
		return NULL;
	Size = Size * 2 + 1;
	pRet = new int[Size];
	pRet[0] = Size;
	int k = 1;
	for(int i = 0; i < EdgesNum; i += 2)
	{
		if(Error)
		{
			if(IsBorderEdge(i))
			{
				FillOneEdge(i, pRet + k);
				k += 2;
				continue;
			}
		}
		if(Sharp)
		{
			if(Edges[i][3] == 1)
			{
				if(IsSharpEdge(i))
				{
					FillOneEdge(i, pRet + k);
					k += 2;
				}
			}
		}
	}
	return pRet;
}
void BDCEL::DrawEdges(bool Sharp, bool Error, bool Smooth) const
{
	bool All = Sharp && Smooth;
//	All = true;

	if(All)
	{
		glBegin(GL_LINES);
		for(int i = 0; i < EdgesNum; i += 2)
			DrawEdge(i);
		glEnd();
		return;
	}
	glBegin(GL_LINES);
	for(int i = 0; i < EdgesNum; i += 2)
	{
		if(Error)
		{
			if(IsBorderEdge(i))
			{
				DrawEdge(i);
				continue;
			}
		}
		if(Sharp)
		{
			if(Edges[i][3] == 1)
			{
				if(IsSharpEdge(i))
					DrawEdge(i);
			}
		}
	}
	glEnd();
}

int BDCEL::NextIncidentEdge(int StartVertexInd, int CurEdgeInd) const
{
	// Returns next edge incident to StartVertexInd
	// It is assumed that CurEdgeInd edge is incident to this vertex
	// but it is not assumed the right edges orientation (2 half edges may have same orientation)
	int IdentShift = Edges[CurEdgeInd][3];
	if (abs(IdentShift) != 1)
		return -1;
	int Ident = CurEdgeInd + IdentShift;
	int NextEdge = Edges[Ident][1];
	if (Edges[NextEdge][0] == StartVertexInd)
		return NextEdge;
	NextEdge = Edges[Ident][1];
	if (Edges[NextEdge][0] == StartVertexInd)
		return NextEdge;
	return -1;
}

int BDCEL::NextHalfEdge(int HalfEdge) const
{
	// Returns a number of next half edge with the same start point, or -1 if the edge doesn't exist
	int Ident = PairedHalfEdge(HalfEdge);
	return Ident < 0 ? -1 : Edges[Ident][1];
}

int BDCEL::PairedHalfEdge(int HalfEdge) const
{
	// Returns a number of paired half edge or -1 if the edge doesn't exist
	int IdentShift = Edges[HalfEdge][3];
	if (abs(IdentShift) != 1)
		return -1;
	return HalfEdge + IdentShift;
}

const BPoint BDCEL::GetEverNormal(int VertInd) const
{
	// Returns average normal vector
	int StartEdgeInd = Vertexes[VertInd][0];
	BPoint PrevEdge = GetEdgeVec(StartEdgeInd);
	BPoint Normal(0., 0., 0., 0.);
	for(int EI = NextHalfEdge(StartEdgeInd); EI > 0; EI = NextHalfEdge(EI))
	{
		BPoint CurEdge(GetEdgeVec(EI));
		Normal = Normal + PrevEdge % CurEdge;
		PrevEdge = CurEdge;
		if(EI == StartEdgeInd)
			break;
	}
	return Normal;
}

const BPoint BDCEL::GetEdgeVec(int HalfEdgeInd) const
{
	const float *SPoint = Get3fvByVn(GetESVn(HalfEdgeInd));
	const float *EPoint = Get3fvByVn(GetEEVn(HalfEdgeInd));

	return BPoint(
		double(EPoint[0] - SPoint[0]),
		double(EPoint[1] - SPoint[1]),
		double(EPoint[2] - SPoint[2]),
		0.);
}

double BDCEL::GetEdgeVecD2(int HalfEdgeInd) const
{
	const float *SPoint = Get3fvByVn(GetESVn(HalfEdgeInd));
	const float *EPoint = Get3fvByVn(GetEEVn(HalfEdgeInd));
	float x = EPoint[0] - SPoint[0];
	float y = EPoint[1] - SPoint[1];
	float z = EPoint[2] - SPoint[2];
	return x * x + y * y + z * z;
}

bool BDCEL::IsSharpEdge(int i) const
{
	if(IsBorderEdge(i) || IsDummyEdge(i))
		return false;
	int i1 = Faces[Edges[i][2]][1];
	int i2 = Faces[Edges[i + Edges[i][3]][2]][1];
	if(i1 < 0 || i2 < 0)
		return false;
	BPoint n0 = GetNormByNum(i1);
	BPoint n1 = GetNormByNum(i2);
	double cos = n0 * n1;
	return (cos < MIN_SMOOTH_COS);
}

bool BDCEL::IsRotZSilhoueteEdge(int i) const
{
	if (IsDummyEdge(i))
		return false;
	if (IsBorderEdge(i))
		return true;
	int i1 = Faces[Edges[i][2]][1];
	int i2 = Faces[Edges[i + Edges[i][3]][2]][1];
	if (i1 < 0 || i2 < 0)
		return false;
	BPoint nL = GetNormByNum(i1);
	BPoint nR = GetNormByNum(i2);
	BPoint vp = nL % nR;
	if (vp.D2() < MINAR * MINAR)
		return false;
	double d = GetEdgeVec(i) * vp;
	if (d < 0)
		return false;
	// Edge is convex
	const float* SPoint = Get3fvByVn(GetESVn(i));
	BPoint v(-SPoint[1], SPoint[0], 0., 0.);// velocity
	
	return (v * nL) * (v * nR) < 0 ? true : false;
}

int BDCEL::IsConvexEdge(int i) const
{// -1 concave; 0 - flat; 1 - convex; -2 - border; -3 - dummy
// i - half edge index
	if (IsDummyEdge(i))
		return -3;
	if (IsBorderEdge(i))
		return -2;
	int i1 = Faces[Edges[i][2]][1];
	int i2 = Faces[Edges[i + Edges[i][3]][2]][1];
	if (i1 < 0 || i2 < 0)
		return -2;
	BPoint vp = GetNormByNum(i1) % GetNormByNum(i2);
	if (vp.D2() < MINAR * MINAR)
		return 0;
	double d = GetEdgeVec(i) * vp;
	return d > 0. ? 1 : -1;
}

bool BDCEL::IsCreated() const
{
	return pC != NULL;
}

void BDCEL::Clear()
{
	Vertexes.clear();
	Edges.clear();
	Faces.clear();
	EdgesNum = 0;
	FacesNum = 0;
	BorderEdges = 0;
	pC = NULL;
	BufEdges.clear();
}

int BDCEL::HealTri(double Tol)
{
	// Find closed chain consisting of border edges and try to heal it
	if(BorderEdges < 3)
		return 0;
	for(int CurEdge = 0; CurEdge < EdgesNum; ++CurEdge)
	{
		if(!IsBorderEdge(CurEdge))
			continue;
		// Find second border edge
		int Edge2 = GetNextBorder(CurEdge);
		if(Edge2 < 0)
			continue;
		// Find third border edge
		int Edge3 = GetNextBorder(Edge2);
		if(Edge3 < 0)
			continue;
		if(GetESVn(CurEdge) != GetEEVn(Edge3))
			continue;
	}
	return 0;
}

int BDCEL::NextHalfEdgeCCW(int HalfEdge) const
{
	// working like BDCEL::NextHalfEdge but in counter direction
	int Next = Edges[HalfEdge][1];
	Next = Edges[Next][1];

	int IdentShift = Edges[Next][3];
	if(abs(IdentShift) != 1)
		return -1;
	return Next + IdentShift;
}

int BDCEL::GetNextBorder(int HalfEdgeInd) const
{
	// Find border edge with the start point equal to given edge end point
	// Find in CW direction
	int First = Edges[HalfEdgeInd][1];
	int Next = First;
	while(Next >= 0)
	{
		if(IsBorderEdge(Next))
		{
			if(Edges[Next][2] != Edges[HalfEdgeInd][2])
				return Next;
		}
		Next = NextHalfEdge(Next);
		if(Next == First)
			return -1;
	}
	// Find in CCW direction
	Next = First;
	while(Next >= 0)
	{
		if(IsBorderEdge(Next))
			if(Edges[Next][2] != Edges[HalfEdgeInd][2])
				return Next;
		Next = NextHalfEdgeCCW(Next);
		if(Next == First)
			return -1;
	}

	return -1;
}

void BDCEL::DrawEdge(int EdgeInd) const
{
	// We need to orient the line because some cards or drivers 
	// (e.g. ATI mobility Radeon HD 3450 driver v.8.533.0.0) don't draw the last pixel in the line rasterization
	if(IsDummyEdge(EdgeInd))
		return;
	const float *pS = Get3fvByVn(GetESVn(EdgeInd));
	const float *pE = Get3fvByVn(GetEEVn(EdgeInd));
	const float *p0, *p1;
	if(pS[0] + pS[1] + pS[2] > pE[0] + pE[1] + pE[2])
	{
		p0 = pS;
		p1 = pE;
	}
	else
	{
		p1 = pS;
		p0 = pE;
	}
	glVertex3fv(p0);
	glVertex3fv(p1);
}

bool BDCEL::IsEdgesOverlap(const BPoint &Pc, const BPoint &Pl, const BPoint &Ps)
{
	// Pc - common point, Pl - large edge point, Ps - small edge point
	
	if(Ps.LineD2(Pc, Pl) <= PointTol * PointTol)
	{
		BPoint V2(Ps - Pc);
		BPoint V1(Pc - Pl);
		if(V1 * V2 < 0)
			if(V1.D2() >= V2.D2()/* - 1.e-6*/)
				return true;
	}
	return false;
}
int BDCEL::FindE(int *BENums, int Size, int SVi, int EVi, int BEI1)
{
	int BEI2 = -1;
	int i = FindFirstE(BENums, BorderEdges, SVi);
	if(i >= 0)
	{
		for(; i < BorderEdges ; ++i)
		{
			int CurE = BENums[i];// This index is in BufEdges
			if(CurE == BEI1)
				continue;
			if(BufEdges[CurE][3] != 0)
				continue;
			if(BufEdges[BufEdges[CurE][1]][0]/* End vertex index */ == SVi)
			{
				BPoint Pc = GetPointByStorNum(Vertexes[SVi][2]);
				BPoint Ps2 = GetPointByStorNum(Vertexes[BufEdges[CurE][0]][2]);
				BPoint Pe1 = GetPointByStorNum(Vertexes[EVi][2]);
				if(IsEdgesOverlap(Pc, Pe1, Ps2))
				{
					BEI2 = CurE;// This index is in BufEdges
					break;
				}
			}
			else
				break;
		}
	}
	return BEI2;
}

int BDCEL::FindS(int *BENums, int Size, int EVi, int SVi, int BEI1)
{
	if (BufEdges.empty())
		return -1;
	int BEI2 = -1;
	int i = FindFirstS(BENums, BorderEdges, EVi);
	if(i >= 0)
	{
		for(; i < BorderEdges ; ++i)
		{
			int CurE = BENums[i];// This index is in BufEdges
			if(CurE == BEI1)
				continue;
			if(BufEdges[CurE][0] < 0 || BufEdges[CurE][3] != 0)
				continue;
			if(BufEdges[CurE][0]/* Start vertex index */ == EVi)
			{
				BPoint Pc = GetPointByStorNum(Vertexes[BufEdges[CurE][0]][2]);
				BPoint Ps1 = GetPointByStorNum(Vertexes[SVi][2]);
				BPoint Pe2 = GetPointByStorNum(Vertexes[BufEdges[BufEdges[CurE][1]][0]][2]);
				if(IsEdgesOverlap(Pc, Ps1, Pe2))
				{
					BEI2 = CurE;// This index is in BufEdges
					break;
				}
			}
			else
				break;
		}
	}
	return BEI2;
}
int BDCEL::FindFirstS(int *BENums, int Size, int EVi)
{
	if (BufEdges.empty())
		return -1;
	int F = -1;
	int L = Size - 1;
	for(int k = 0; k < Size; ++k)
	{
		int VF = BufEdges[BENums[F + 1]][0];
		if(VF == EVi)
			return F + 1;
		int M = (F + L) / 2;
		int VM = BufEdges[BENums[M]][0];
		if(VM < 0)
		{
			++F;
			if( F >= Size - 1)
				return -1;
		}
		else
		{
			if(VM >= EVi)
				L = M;
			else
				F = M;
		}
	}
	return -1;
}
int BDCEL::FindFirstE(int *BENums, int Size, int EVi)
{
	if (BufEdges.empty())
		return -1;
	int F = -1;
	int L = Size - 1;
	for(int k = 0; k < Size; ++k)
	{
		int VF = BufEdges[BENums[F + 1]][1];
		while(VF < 0)
		{
			++F;
			if( F >= Size - 1)
				return -1;
			VF = BufEdges[BENums[F + 1]][1];
		}
		VF = BufEdges[VF][0];
		if(VF == EVi)
			return F + 1;
		int M = (F + L) / 2;
		int VM = BufEdges[BENums[M]][1];
		if(VM < 0)
		{
			++F;
			if( F >= Size - 1)
				return -1;
		}
		else
		{
			VM = BufEdges[VM][0];
			if(VM >= EVi)
				L = M;
			else
				F = M;
		}
	}
	return -1;
}
void BDCEL::HealBorders(int *BENums)
{
// Border edges processing
	int NewEdgeInd = EdgesNum;
	int NewFaceInd = FacesNum;
	bool HaveErrors = true;
	for(int kk = 0; kk < BorderEdges && HaveErrors; ++kk)
	{
		HaveErrors = false;
//		qsort((void *)BENums, BorderEdges, sizeof(BENums[0]), &BDCEL::compare_edges_by_SVn);
		QuickSort(BENums, BorderEdges, edges_by_SVn);
		for(int iB = 0; iB < BorderEdges; ++iB)
		{
			int BEI1 = BENums[iB];// This index is in BufEdges
			if(BufEdges[BEI1][3] != 0)
				continue;
			bool EdgeFinished = false;
			while(!EdgeFinished)
			{
				int SVi = BufEdges[BEI1][0];// Start vertex index
				int EVi = BufEdges[BufEdges[BEI1][1]][0];// End vertex index
				// Find border edge starting in EVi
				int BEI2 = FindS(BENums, BorderEdges, EVi, SVi, BEI1);
				if(BEI2 < 0)
				{
					// Try to invert BEI1
					//BEI2 = FindS(BENums, BorderEdges, SVi, EVi, BEI1);
					if(BEI2 < 0)
					{
						EdgeFinished = true;
						continue;
					}
					else
					{
						// Invert BEI1
						const int E0 = BEI1;
						int E1 = BufEdges[E0][1];
						int E2 = BufEdges[E1][1];
						int P0 = BufEdges[E0][0];
						BufEdges[E0][0] = BufEdges[E1][0];
						BufEdges[E1][0] = BufEdges[E2][0];
						BufEdges[E2][0] = P0;
						BufEdges[E0][1] = E2;
						BufEdges[E1][1] = E0;
						BufEdges[E2][1] = E1;
						for(int l = 0; l < BorderEdges; ++l)
						{
							if(BENums[l] == E0)
								BENums[l] = E2;
							else if(BENums[l] == E1)
								BENums[l] = E0;
							else if(BENums[l] == E2)
								BENums[l] = E1;
						}
					}
				}
				// Cut edge
				// New border edge will have BEI1 index
				HaveErrors = true;
				BufEdges[BEI2][3] = 1;
				BufEdges[BEI2 + 1][0] = BufEdges[BufEdges[BEI2][1]][0];
				BufEdges[BEI2 + 1][1] = BufEdges[BEI1][1];
				BufEdges[BEI2 + 1][2] = BufEdges[BEI1][2];
				BufEdges[BEI2 + 1][3] = -1;
				if(BufEdges[BEI1][0] == BufEdges[BEI2 + 1][0])
				{
					BufEdges[BufEdges[BufEdges[BEI1][1]][1]][1] = BEI2 + 1;
					MakeDummy(BufEdges, BEI1);
					EdgeFinished = true;
					continue;
				}
				Faces[NewFaceInd][1] = Faces[BufEdges[BEI1][2]][1];

				BufEdges[NewEdgeInd][0] = BufEdges[BufEdges[BufEdges[BEI1][1]][1]][0];
				BufEdges[NewEdgeInd][1] = BEI2 + 1;
				BufEdges[NewEdgeInd][2] = BufEdges[BEI1][2];
				BufEdges[NewEdgeInd][3] = 1;
				++NewEdgeInd;
				BufEdges[NewEdgeInd][0] = BufEdges[BEI2 + 1][0];
				BufEdges[NewEdgeInd][1] = BufEdges[BufEdges[BEI1][1]][1];
				BufEdges[NewEdgeInd][2] = NewFaceInd;
				BufEdges[NewEdgeInd][3] = -1;
				BufEdges[BufEdges[BEI1][1]][1] = NewEdgeInd - 1;
				BufEdges[BEI1][1] = NewEdgeInd;
				BufEdges[BEI1][2] = NewFaceInd;
				BufEdges[BufEdges[NewEdgeInd][1]][2] = NewFaceInd;
				++NewFaceInd;
				++NewEdgeInd;
			}
		}
		// Compress BENums
		int NewBorderEdges = 0;
		for(int i = 0; i < BorderEdges; ++i)
		{
			if(BufEdges[BENums[i]][3] == 0)
				BENums[NewBorderEdges++] = BENums[i];
		}
		BorderEdges = NewBorderEdges;
		if(BorderEdges > 0)
		{
//			qsort((void *)BENums, BorderEdges, sizeof(BENums[0]), compare_edges_by_EVn);
			QuickSort(BENums, BorderEdges, edges_by_EVn);
			for(int iB = 0; iB < BorderEdges; ++iB)
			{
				int BEI1 = BENums[iB];// This index is in BufEdges
				if(BufEdges[BEI1][3] != 0)
					continue;
				bool EdgeFinished = false;
				while(!EdgeFinished)
				{
					int SVi = BufEdges[BEI1][0];// Start vertex index
					int EVi = BufEdges[BufEdges[BEI1][1]][0];// End vertex index
					// Find border edge endinging in SVi
					int BEI2 = FindE(BENums, BorderEdges, SVi, EVi, BEI1);
					if(BEI2 < 0)
					{
						//BEI2 = FindE(BENums, BorderEdges, EVi, SVi, BEI1);
						if(BEI2 < 0)
						{
							EdgeFinished = true;
							continue;
						}
						else
						{
							// Invert BEI1
							const int E0 = BEI1;
							int E1 = BufEdges[E0][1];
							int E2 = BufEdges[E1][1];
							int P0 = BufEdges[E0][0];
							BufEdges[E0][0] = BufEdges[E1][0];
							BufEdges[E1][0] = BufEdges[E2][0];
							BufEdges[E2][0] = P0;
							BufEdges[E0][1] = E2;
							BufEdges[E1][1] = E0;
							BufEdges[E2][1] = E1;
							for(int l = 0; l < BorderEdges; ++l)
							{
								if(BENums[l] == E0)
									BENums[l] = E2;
								else if(BENums[l] == E1)
									BENums[l] = E0;
								else if(BENums[l] == E2)
									BENums[l] = E1;
							}
						}
						EdgeFinished = true;
						continue;
					}
					// Cut edge
					// New border edge will have BEI1 index
					HaveErrors = true;
					BufEdges[BEI2][3] = 1;
					BufEdges[BEI2 + 1][0] = BufEdges[BEI1][0];
					BufEdges[BEI2 + 1][1] = NewEdgeInd + 1;
					BufEdges[BEI2 + 1][2] = BufEdges[BEI1][2];
					BufEdges[BEI2 + 1][3] = -1;
					BufEdges[BufEdges[BufEdges[BEI1][1]][1]][1] = BEI2 + 1;
					if(BufEdges[BufEdges[BEI1][1]][0] == BufEdges[BEI2][0])
					{
						BufEdges[BEI2 + 1][1] = BufEdges[BEI1][1];
						MakeDummy(BufEdges, BEI1);
						EdgeFinished = true;
						continue;
					}
					Faces[NewFaceInd][1] = Faces[BufEdges[BEI1][2]][1];

					BufEdges[NewEdgeInd][0] = BufEdges[BufEdges[BufEdges[BEI1][1]][1]][0];
					BufEdges[NewEdgeInd][1] = BEI1;
					BufEdges[NewEdgeInd][2] = NewFaceInd;
					BufEdges[NewEdgeInd][3] = 1;
					++NewEdgeInd;
					BufEdges[NewEdgeInd][0] = BufEdges[BEI2][0];
					BufEdges[NewEdgeInd][1] = BufEdges[BufEdges[BEI1][1]][1];
					BufEdges[NewEdgeInd][2] = BufEdges[BEI1][2];
					BufEdges[NewEdgeInd][3] = -1;
					BufEdges[BufEdges[BEI1][1]][1] = NewEdgeInd - 1;
					BufEdges[BEI1][0] = BufEdges[BEI2][0];
					BufEdges[BEI1][2] = NewFaceInd;
					BufEdges[BufEdges[BEI1][1]][2] = NewFaceInd;
					++NewFaceInd;
					++NewEdgeInd;
				}
			}
			// Compress BENums
			NewBorderEdges = 0;
			for(int i = 0; i < BorderEdges; ++i)
			{
				if(BufEdges[BENums[i]][3] == 0)
					BENums[NewBorderEdges++] = BENums[i];
			}
			BorderEdges = NewBorderEdges;

		}
	}
	FacesNum = NewFaceInd;
	EdgesNum = NewEdgeInd;
}

void BDCEL::WriteSTL(CStdioFile & f)
{
	f.SeekToBegin();
	char s[5 * 16 + 1] = "STL export                                                                      ";
	f.Write(s, 5 * 16);// title

	auto CorrEdgesNum = EdgesNum;
	for (int iE = 0; iE < EdgesNum; ++iE)
	{
		if (Edges[iE][2] < 0)
			--CorrEdgesNum;
	}

	__int32 TriNum = CorrEdgesNum / 3;
	f.Write(&TriNum, 4);
	char buf[2];
	GLfloat fbuf[4 * 3];
	buf[0] = '\x0';
	buf[1] = '\x0';
	for(int iE = 0; iE < EdgesNum; ++iE)
	{
		if(Edges[iE][2] < 0)
			continue;
		int EdgeInd = iE;
		BPoint n = GetEdgeVec(EdgeInd) % GetEdgeVec(BufEdges[EdgeInd][1]);
		n = n * (1. / sqrt(n.D2()));
		fbuf[0] = GLfloat(n.GetX());// Normal
		fbuf[1] = GLfloat(n.GetY());// Normal
		fbuf[2] = GLfloat(n.GetZ());// Normal
		for(int i = 0; i < 3; ++i)
		{
			BPoint P(GetPointByStorNum(Vertexes[GetESVn(EdgeInd)][2]));
			fbuf[i * 3 + 3] = GLfloat(P.GetX());// Vertex
			fbuf[i * 3 + 4] = GLfloat(P.GetY());// Vertex
			fbuf[i * 3 + 5] = GLfloat(P.GetZ());// Vertex
			Edges[EdgeInd][2] = -1 * (Edges[EdgeInd][2] + 2);// Mark edge
			EdgeInd = Edges[EdgeInd][1];
		}
		f.Write(fbuf, 4*3*4);
		f.Write(buf, 2);
	}
	UnmarkAll();
}

bool BDCEL::WriteSTLmOneSolid(CStdioFile& f, int StartEdge)
{
	// returns false if all solids are exported already
	if (StartEdge < 0)
		return false;

	f.SeekToBegin();
	char s[5 * 16 + 1] = "STL export                                                                      ";
	f.Write(s, 5 * 16);// title
	__int32 TriNum = 0;
	f.Write(&TriNum, 4);

	int* StackE = new int[EdgesNum / 2];
	const char buf[2] = { '\x0', '\x0' };

	int Head = -1;
	StackE[++Head] = StartEdge;
	while (Head >= 0)
	{
		int EdgeNum = StackE[Head--];
		if (Edges[EdgeNum][2] < 0)
			continue;
		GLfloat fbuf[4 * 3];

		TriNum++;
		int EdgeInd = EdgeNum;
		BPoint n = GetEdgeVec(EdgeInd) % GetEdgeVec(BufEdges[EdgeInd][1]);
		n = n * (1. / sqrt(n.D2()));
		fbuf[0] = GLfloat(n.GetX());// Normal
		fbuf[1] = GLfloat(n.GetY());// Normal
		fbuf[2] = GLfloat(n.GetZ());// Normal
		for (int i = 0; i < 3; ++i)
		{
			BPoint P(GetPointByStorNum(Vertexes[GetESVn(EdgeInd)][2]));
			fbuf[i * 3 + 3] = GLfloat(P.GetX());// Vertex
			fbuf[i * 3 + 4] = GLfloat(P.GetY());// Vertex
			fbuf[i * 3 + 5] = GLfloat(P.GetZ());// Vertex
			Edges[EdgeInd][2] = -1 * (Edges[EdgeInd][2] + 2);// Mark edge
			if (Edges[EdgeInd][3] != 0) // correspondent half edge exists
			{
				auto CorrEdge = GetCorrEdge(EdgeInd);
				if (Edges[CorrEdge][2] >= 0)
					StackE[++Head] = CorrEdge;// push correspondent half edge
			}
			EdgeInd = Edges[EdgeInd][1];
		}
		f.Write(fbuf, 4 * 3 * 4);
		f.Write(buf, 2);
	}

	delete[] StackE;

	f.SeekToBegin();
	f.Write(s, 5 * 16);// title
	f.Write(&TriNum, 4);


	return true;
}

void BDCEL::UnmarkAll()
{
	for (int iE = 0; iE < EdgesNum; ++iE)
	{
		if (Edges[iE][2] < -1)
			Edges[iE][2] = -1 * (Edges[iE][2] + 2);// UnMark edge
	}
	for (int iF = 0; iF < FacesNum; ++iF)
	{
		if (Faces[iF][0] < -1)
			Faces[iF][0] = -1 * (Faces[iF][0] + 2);// UnMark face
	}
}

void BDCEL::InvertMarks()
{
	for (int iE = 0; iE < EdgesNum; ++iE)
	{
		if (IsDummyEdge(iE))
			continue;
		Edges[iE][2] = -1 * (Edges[iE][2] + 2);// invert mark
	}
	for (int iF = 0; iF < FacesNum; ++iF)
	{
		if (IsDummyFace(iF))
			continue;
		Faces[iF][0] = -1 * (Faces[iF][0] + 2);// invert mark
	}
}

void BDCEL::MarkFaceEdges(int EdgeNum)
{
	BufEdges[EdgeNum][2] = (BufEdges[EdgeNum][2] + 2) * (-1);
	Edges[Edges[EdgeNum][1]][3] = (Edges[Edges[EdgeNum][1]][2] + 2) * (-1);
	Edges[Edges[Edges[EdgeNum][1]][1]][3] = (Edges[Edges[Edges[EdgeNum][1]][1]][2] + 2) * (-1);
}

void BDCEL::MarkEdge(int HEdgeNum)
{
	if (!IsMarkedEdge(HEdgeNum))
		Edges[HEdgeNum][2] = -1 * (Edges[HEdgeNum][2] + 2);
}

void BDCEL::MarkEdgePair(int HEdgeNum)
{
	MarkEdge(HEdgeNum);
	auto Pair = PairedHalfEdge(HEdgeNum);
	if(Pair >= 0)
		MarkEdge(Pair);
}

UINT BDCEL::FindNextSharp(size_t HEdgeNum) const
{
	// find next sharp edge if it is single 
	UINT Found = UINT_MAX;
	auto StartHEdge = PairedHalfEdge(HEdgeNum);
	for (int EI = NextHalfEdge(StartHEdge); EI >= 0 && EI != StartHEdge; EI = NextHalfEdge(EI))
	{
		if(IsMarkedEdge(EI))
			return UINT_MAX;
		if (IsSharpEdge(EI))
		{
			if(Found != UINT_MAX)// found already
				return UINT_MAX;
			Found = EI;
		}
	}
	return Found;
}

void BDCEL::MarkConnected(int ExtremeEdge)
{
	if (ExtremeEdge < 0)
		return;
	int* StackE = new int[EdgesNum / 2];
	int Head = -1;
	StackE[++Head] = ExtremeEdge;
	while (Head >= 0)
	{
		int EdgeNum = StackE[Head--];
		if (Edges[EdgeNum][2] < 0)
			continue;
		int E0 = EdgeNum;
		int E1 = Edges[E0][1];
		int E2 = Edges[E1][1];
		// Mark face
		int iF = Edges[E0][2];
		Faces[iF][0] = (Faces[iF][0] + 2) * (-1);
		// Mark triangle edges 
		Edges[E0][2] = (Edges[E0][2] + 2) * (-1);
		Edges[E1][2] = (Edges[E1][2] + 2) * (-1);
		Edges[E2][2] = (Edges[E2][2] + 2) * (-1);
		for (int i = 0; i < 3; ++i)
		{
			E1 = Edges[E0][3] + E0;
			if (E1 != E0 && Edges[E1][2] >= 0)
			{
				StackE[++Head] = E1;
			}
			E0 = Edges[E0][1];
		}
	}
	delete[] StackE;
}

void BDCEL::DoOrientWave()
{
	int *StackE = new int[EdgesNum / 2];
	bool *StackF = new bool[EdgesNum / 2];
	int ExtremeEdge = -1;
	bool IsRightOriented = true;
	while((ExtremeEdge = FindExtremeEdge(IsRightOriented)) >= 0)
	{// For each connected component

		int Head = -1;
		StackE[++Head] = ExtremeEdge;
		StackF[Head] = !IsRightOriented;
		while(Head >= 0)
		{
			int EdgeNum = StackE[Head];
			bool Invert = StackF[Head--];
			if(Edges[EdgeNum][2] < 0)
				continue;
			int E0 = EdgeNum;
			int E1 = Edges[E0][1];
			int E2 = Edges[E1][1];
			if(Invert)
			{
				int P0 = Edges[E0][0];
				Edges[E0][0] = Edges[E1][0];
				Edges[E1][0] = Edges[E2][0];
				Edges[E2][0] = P0;
				Edges[E0][1] = E2;
				Edges[E1][1] = E0;
				Edges[E2][1] = E1;
			}
			// Mark triangle edges and call myself
			Edges[E0][2] = (Edges[E0][2] + 2) * (-1);
			Edges[E1][2] = (Edges[E1][2] + 2) * (-1);
			Edges[E2][2] = (Edges[E2][2] + 2) * (-1);
			for(int i = 0; i < 3; ++i)
			{
				E1 = Edges[E0][3] + E0;
				if(E1 != E0 && Edges[E1][2] >= 0)
				{
					bool f = Edges[E0][0] == Edges[E1][0] && Edges[Edges[E0][1]][0] == Edges[Edges[E1][1]][0];
					StackE[++Head] = E1;
					StackF[Head] = f;
				}
				E0 = Edges[E0][1];
			}
		}
	}
	UnmarkAll();
	delete[] StackF;
	delete[] StackE;
}

double BDCEL::MinTriH2(int HalfEdgeInd)
{
	int E0 = HalfEdgeInd;
	int E1 = Edges[E0][1];
	int E2 = Edges[E1][1];
	BPoint P0(GetPointByStorNum(Vertexes[GetESVn(E0)][2]));
	BPoint P1(GetPointByStorNum(Vertexes[GetESVn(E1)][2]));
	BPoint P2(GetPointByStorNum(Vertexes[GetESVn(E2)][2]));
	return MinTriH2(P0, P1, P2);
}
double BDCEL::MinTriH2(const BPoint &P0, const BPoint &P1, const BPoint &P2)
{
	double H20 = P0.LineD2(P1, P2);
	double H21 = P1.LineD2(P0, P2);
	double H22 = P2.LineD2(P0, P1);
	return min(H20, min(H21, H22));
}
const float *BDCEL::Get3fvByVn(int i) const
{ 
	const float *pData = GetBasePtrByStorNum(Vertexes[i][2]);
	return pData + 3;
}

//////////////////////////////////////////////////////////////////////////
// QuickSort functions
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// QuickSortRecursive - core of algorithm, do not call it, use QuickSort,
// see below
void BDCEL::QuickSortRecursive(int *pArr, int d, int h, enum CompareType Type)
{
	int i, j;
	int str;

	i = h;
	j = d;

	str = pArr[((int) ((d + h) / 2))];

	do {
		//while (pArr[j] < str) j++;
		//while (pArr[i] > str) i--;

		while (Compare(pArr[j], str, Type) < 0) j++;
		while (Compare(pArr[i], str, Type) > 0) i--;

		if ( i >= j )
		{
			if ( i != j )
			{
				int zal;

				zal = pArr[i];
				pArr[i] = pArr[j];
				pArr[j] = zal;

			}
			i--;
			j++;
		}
	} while (j <= i);

	if (d < i) QuickSortRecursive(pArr, d, i, Type);
	if (j < h) QuickSortRecursive(pArr, j, h, Type);
}

//////////////////////////////////////////////////////////////////////////
// QuickSort - entry to algorithm
//
// T *pArr			... pointer of array to sort
// int iSize		... size of array T *pArr
// BOOL bAscending	... if bAscending == TRUE, then sort ascending,
//						otherwise descending
//
// return TRUE if no error, error can be bad parameter, call ::GetLastError()
// if QuickSort returned FALSE
BOOL BDCEL::QuickSort(int *pArr, int iSize, enum CompareType Type)
{
	BOOL rc = TRUE;

	if (iSize > 1)
	{
		try
		{
			int	low = 0, high = iSize - 1;
			QuickSortRecursive(pArr, low, high, Type);
		}
		catch (...)
		{
			::SetLastError(ERROR_INVALID_PARAMETER);
			rc = FALSE;
		}

	}
	else
	{
		::SetLastError(ERROR_INVALID_PARAMETER);
		rc = FALSE;
	}
	return rc;
}

int BDCEL::Compare(int a, int b, enum CompareType Type)
{
	switch(Type)
	{
	case points_by_d:
		return compare_points_by_d(a, b);
	case edges_by_EVn:
		return compare_edges_by_EVn(a, b);
	case edges_by_SVn:
		return compare_edges_by_SVn(a, b);
	case edges_by_2Vn:
		return compare_edges_by_2Vn(a, b);
	}
	return 0;
}

void BDCEL::FillOneEdge(int EdgeNum, int *pRet) const
{
	// We need to orient the line because some cards or drivers 
	// (e.g. ATI mobility Radeon HD 3450 driver v.8.533.0.0) don't draw the last pixel in the line rasterization
	const float *pS = Get3fvByVn(GetESVn(EdgeNum));
	const float *pE = Get3fvByVn(GetEEVn(EdgeNum));
	int i0, i1;
	if(pS[0] + pS[1] + pS[2] > pE[0] + pE[1] + pE[2])
	{
		i0 = 0;
		i1 = 1;
	}
	else
	{
		i1 = 0;
		i0 = 1;
	}
	pRet[i0] = Vertexes[GetESVn(EdgeNum)][2];
	pRet[i1] = Vertexes[GetEEVn(EdgeNum)][2];
}

const ClrPoint BDCEL::RayCasting(const BPoint& P, const BPoint& V, double MinW, BPoint& N) const
{
	// Find the intersection point nearest to P in the V direction
		// N - external normal in P (from Stl)
	if(Faces.empty())
		return BPoint(0., 0., 0., -1.);
	double MinT = 1.e12;
	PackedColor Color;
	for each(const auto& iFace in Faces)
	{
		if (iFace[0] < 0)
			continue;
		auto iEdge = iFace[0];
		BPoint n0 = GetNormByEdge(iEdge);
		ClrPoint p0(GetClrPByVn(GetESVn(iEdge)));
		iEdge = Edges[iEdge][1];
		ClrPoint p1(GetClrPByVn(GetESVn(iEdge)));
		iEdge = Edges[iEdge][1];
		ClrPoint p2(GetClrPByVn(GetESVn(iEdge)));
		p0 -= P;
		p1 -= P;
		p2 -= P;
		//Calculate normal
		BPoint n = (p1 - p0) % (p2 - p0);
		n = ((n * n0 < 0) ? -1. : 1.) * n;
		double z = n * V;
		if (fabs(z) < MINAR)
			continue;
		z = 1. / z;
		double t = p0 * n * z;
		if (t < 0. || t > MinT)
			continue;
		BPoint IntP(V * t);
		if (((p1 - p0) % (p0 - IntP)) * n > 0 && IntP.LineD2(p1, p0) > MinW * MinW)
			continue;
		if (((p2 - p1) % (p1 - IntP)) * n > 0 && IntP.LineD2(p2, p1) > MinW * MinW)
			continue;
		if (((p0 - p2) % (p2 - IntP)) * n > 0 && IntP.LineD2(p0, p2) > MinW * MinW)
			continue;
		MinT = t;
		N = n0;
		Color = p0.GetColor();
	}
	if (MinT >= 1.e11)
		return 	BPoint(0., 0., 0., -1.);

	if (V * N > 0.)
		N = N * (-1.);
	ClrPoint R = V * MinT + P;
	R.SetColor(Color);
	return R;
}

std::vector<BPoint> Normals;
std::vector<ClrPoint> BufPts;
BPoint BufV;
BPoint BufP;
bool Grater(int i, int j)
{
	double Spi = (BufPts[i] - BufP) * BufV;
	double Spj = (BufPts[j] - BufP) * BufV;
	return Spi < Spj;
}

size_t BDCEL::RayShreddingTri(const BPoint& P, const BPoint& V, double MinW, std::vector<ClrPoint>& Pts) const
{
	Pts.clear();
	if (Faces.empty())
		return 0;
	//Find all intersection points
	BufPts.clear();
	Normals.clear();
	std::vector<int> FaceIndexes;
	std::map<int, BPoint> Verts;
	for each (const auto & iFace in Faces)
	{
		if (iFace[0] < 0)
			continue;
		auto iEdge = iFace[0];
		BPoint n0 = GetNormByNum(iFace[1]);
		ClrPoint p0(GetClrPByVn(GetESVn(iEdge)));
		iEdge = Edges[iEdge][1];
		ClrPoint p1(GetClrPByVn(GetESVn(iEdge)));
		iEdge = Edges[iEdge][1];
		ClrPoint p2(GetClrPByVn(GetESVn(iEdge)));
		p0 -= P;
		p1 -= P;
		p2 -= P;
		//Calculate normal
		BPoint n = (p1 - p0) % (p2 - p0);
		n = ((n * n0 < 0) ? -1. : 1.) * n;
		double z = n * V;
		if (fabs(z) < MINAR)
			continue;
		z = 1. / z;
		double t = p0 * n * z;
		if (t < 0.)
			continue;
		BPoint IntP(V * t);
		if (((p1 - p0) % (p0 - IntP)) * n > 0 && IntP.LineD2(p1, p0) > MinW * MinW)
			continue;
		if (((p2 - p1) % (p1 - IntP)) * n > 0 && IntP.LineD2(p2, p1) > MinW * MinW)
			continue;
		if (((p0 - p2) % (p2 - IntP)) * n > 0 && IntP.LineD2(p0, p2) > MinW * MinW)
			continue;
		ClrPoint R = V * t;
		R.SetColor(p0.GetColor());
		// Vertexes processing
		iEdge = iFace[0];
		if ((R - p0).D2() < MIND * MIND)
		{
			const auto iVert = GetESVn(iEdge);
			const auto it = Verts.find(iVert);
			if (it == Verts.end())
				Verts[iVert] = p0 + P;
			continue;
		}
		iEdge = Edges[iEdge][1];
		if ((R - p1).D2() < MIND * MIND)
		{
			const auto iVert = GetESVn(iEdge);
			const auto it = Verts.find(iVert);
			if (it == Verts.end())
				Verts[iVert] = p1 + P;
			continue;
		}
		iEdge = Edges[iEdge][1];
		if ((R - p2).D2() < MIND * MIND)
		{
			const auto iVert = GetESVn(iEdge);
			const auto it = Verts.find(iVert);
			if (it == Verts.end())
				Verts[iVert] = p2 + P;
			continue;
		}
		// END:Vertexes processing
		// R is not equal nj any vertex
		R += P;
		BufPts.push_back(R);
		Normals.push_back(n);
		FaceIndexes.push_back(Edges[iEdge][2]);
	}
	// Vertexes processing
	for each (auto Elem in Verts)
	{
		// check all incident faces
		int EdgeInd = Vertexes[Elem.first][0];
		if (EdgeInd < 0)
			continue;// internal error
		if (Elem.first != Edges[EdgeInd][0])
			EdgeInd = PairedHalfEdge(EdgeInd);
		if (EdgeInd < 0)
			continue;// internal error
		int FaceInd = Edges[EdgeInd][2];// Nfl
		BPoint N = GetNormByNum(Faces[FaceInd][1]);
		double NV = N * V;
		int Sign = (NV > MINAR) ? 1 : ((NV < -MINAR) ? -1 : 0);
		bool Touch = false;
		for(int NextEdge = NextHalfEdge(EdgeInd); (NextEdge != EdgeInd) && (NextEdge != -1); NextEdge = NextHalfEdge(NextEdge))
		{
			FaceInd = Edges[NextEdge][2];// Nfl
			BPoint CN = GetNormByNum(Faces[FaceInd][1]);
			NV =  CN * V;
			int CurSign = (NV > MINAR) ? 1 : ((NV < -MINAR) ? -1 : 0);
			if (CurSign != 0)
				N = CN;
			if (CurSign * Sign < 0)
			{
				Touch = true;
				break;
			}
		};
		if (!Touch)
		{
			BufPts.push_back(GetClrPByVn(Elem.first));
			Normals.push_back(N);
		}
	}
	// END:Vertexes processing
	// sort along V
	BufV = V;
	BufP = P;
	std::vector<int> Ind;
	Ind.resize(BufPts.size());
	for (size_t i = 0; i < Ind.size(); ++i)
		Ind[i] = i;
	std::sort(Ind.begin(), Ind.end(), Grater);
	//Find proper intervals (remove extra points)
	std::vector<bool> RemoveFlags;
	RemoveFlags.resize(Ind.size());
	// mark duplicated points to remove
	for (size_t i = 0; i < Ind.size(); ++i)
	{
		auto IndCur = Ind[i];
		RemoveFlags[i] = false;
		double t = (BufPts[IndCur] - P) * V;
		bool NormSign = (Normals[IndCur] * V) > 0;
		for (size_t j = i + 1; j < Ind.size(); ++j)
		{
			if ((BufPts[Ind[j]] - P) * V - t < MIND) // points are identical
			{
				if (((Normals[Ind[j]] * V) > 0) == NormSign)// normals are identical
				{
					if (IsFacesAdjacent(FaceIndexes[IndCur], FaceIndexes[Ind[j]]))
					{
						RemoveFlags[i] = true;
						break;
					}
				}
			}
			else
				break;
		}
	}
	int Counter = 0;
	// mark extra points to remove
	for (size_t i = 0; i < Ind.size(); ++i)
	{
		if (RemoveFlags[i])
			continue;
		if (Normals[Ind[i]] * V < 0)
		{
			++Counter;
			RemoveFlags[i] = (Counter != 1);
		}
		else
		{
			RemoveFlags[i] = (Counter != 1);
			--Counter;
		}
	}
	for (size_t i = 0; i < Ind.size(); ++i)
	{
		if (!RemoveFlags[i])
			Pts.push_back(BufPts[Ind[i]]);
	}
	return Pts.size();
}

void BDCEL::SetSmoothAngle(double Angle)
{ 
//	SmoothAngle = Angle;
	MIN_SMOOTH_COS = cos(Angle * PI / 180.);
}

int BDCEL::IdentifyPoints(const float *pCI, int *PNums, size_t TrianglesNum, int *&BackNums)
{
	if (pCI == NULL)
		return 0;
	if (TrianglesNum < 1)
		return 0;
	pC = pCI;
	// Remove degraded triangles
	size_t PointsNumber = 0;// A number of source points after MinTriH test
	for (size_t i = 0; i < 3 * TrianglesNum; i += 3)
	{
		if (MinTriH2(GetPointByNum(i), GetPointByNum(i + 1), GetPointByNum(i + 2)) > PointTol * PointTol)
		{
			for (int k = 0; k < 3; ++k)
				PNums[PointsNumber++] = PNums[i + k];
		}
	}
	for (size_t j = PointsNumber; j < 3 * TrianglesNum; PNums[j++] = -1); // Clear the rest

	int MaxPointIndex = 0;
	for (size_t j = 0; j < PointsNumber; ++j)
	{
		int b = PNums[j];
		MaxPointIndex = max(MaxPointIndex, b);
	}
	BackNums = new int[MaxPointIndex + 1]; // back references
	// BackNums - связь между старыми и новыми номерами точек в хранилище точек. Для каждого старого номера хранит новый номер
	for (int j = 0; j < MaxPointIndex + 1; BackNums[j++] = -1);

	//Sort PNums

	QuickSort(PNums, int(PointsNumber), points_by_d);
	// PNums сейчас содержит номера точек в хранилище, упорядоченные вдоль прямой
	// Identify all points
	int PointsNum = 0;// A number of points after identification
	for (size_t i = 0; i < PointsNumber; ++i)
	{
		BackNums[PNums[i]] = PointsNum;
		BPoint Pi(GetPointByStorNum(PNums[i]));
		for (size_t k = i + 1; k < PointsNumber; ++k) // Find all point coincident with i'th point
		{
			if (PNums[k] < 0) // Point is deleted
				continue;
			BPoint Pk(GetPointByStorNum(PNums[k]));
			BPoint Delta(Pk - Pi);
			if (CALC_D(Delta.GetX(), Delta.GetY(), Delta.GetZ()) > PointTol)
				break;
			double d2 = Delta.D2();
			if (d2 < PointTol * PointTol)
			{
				BackNums[PNums[k]] = PointsNum;
				PNums[k] = -1;
			}
		}
		// Find first non deleted point
		for (; i < PointsNumber - 1; ++i)
			if (PNums[i + 1] >= 0)
			{
				++PointsNum;
				break;
			}

	}
	++PointsNum;
	return PointsNum;
}






int BDCEL::IdentifyPoints(const float * pCI, std::vector<int>& PNums, int PointsNumber, std::vector<int> &BackNums)
{
	//Sort PNums

	QuickSort(PNums.data(), PointsNumber, points_by_d);
	// PNums сейчас содержит номера точек в хранилище, упорядоченные вдоль прямой
	// Identify all points
	std::vector<double> Dist;
	Dist.resize(PointsNumber);
	for (int i = 0; i < PointsNumber; ++i)
	{
		const float *pP = GetPointPtrByStorNum(PNums[i]);
		Dist[i] = CALC_D(pP[0], pP[1], pP[2]);
	}
	int PointsNum = 0;// A number of points after identification
	for (int i = 0; i < PointsNumber; ++i)
	{
		BackNums[PNums[i]] = PointsNum;
		const float * Pi = GetPointPtrByStorNum(PNums[i]);
		for (int k = i + 1; k < PointsNumber; ++k) // Find all point coincident with i'th point
		{
			if (PNums[k] < 0) // Point is deleted
				continue;
			const float *Pk = GetPointPtrByStorNum(PNums[k]);
			if (Dist[k] - Dist[i] > PointTol)
				break;
			double d2 = (Pk[0] - Pi[0]) * (Pk[0] - Pi[0])
				+ (Pk[1] - Pi[1]) * (Pk[1] - Pi[1])
				+ (Pk[2] - Pi[2]) * (Pk[2] - Pi[2]);
			if (d2 < PointTol * PointTol)
			{
				BackNums[PNums[k]] = PointsNum;
				PNums[k] = -1;
			}
		}
		// Find first non deleted point
		for (; i < PointsNumber - 1; ++i)
			if (PNums[i + 1] >= 0)
			{
				++PointsNum;
				break;
			}

	}
	++PointsNum;
	return PointsNum;
}

int BDCEL::FindExtremeEdge(bool &IsRightOriented) const
{
	BPoint Dir(CX1, CY1, CZ1, 0.);
	// Find extreme edge
	int StartEdgeInd = -1;
	double MinDist = 1.e12;
	for (int i = 0; i < GetVertexCount(); ++i)
	{
		int EdgeInd = Vertexes[i][0];
		if (Edges[EdgeInd][2] >= 0) // Edge is not marked or dummy
		{
			const float * pBuf = Get3fvByVn(i);
			double Dist = CALC_D(pBuf[0], pBuf[1], pBuf[2]);
			if (Dist < MinDist)
			{
				MinDist = Dist;
				StartEdgeInd = EdgeInd;
			}
		}
	}
	if (StartEdgeInd < 0)
		return -1;
	int ExtrEdge = -1;
	double MinSP = 2.;// CurSP <= 1.
	bool First = true;
	int StartVertexInd = Edges[StartEdgeInd][0];
	for (int EI = StartEdgeInd; EI != StartEdgeInd || First; EI = NextIncidentEdge(StartVertexInd, EI))
	{	// for each incident edge
		First = false;
		if (EI < 0)
			return -1;
		BPoint CurEdge(GetEdgeVec(EI));
		double CurSP = fabs(CurEdge.Unit() * Dir);
		if (CurSP < MinSP)
		{
			MinSP = CurSP;
			ExtrEdge = EI;
		}
	}
	// Find extreme face (second edge)
	// Tangent plane normal
	BPoint ExtrEdgeVec(GetEdgeVec(ExtrEdge));
	if (ExtrEdgeVec * Dir < 0.)
	{
		ExtrEdge = GetCorrEdge(ExtrEdge);
		ExtrEdgeVec = ExtrEdgeVec * (-1.);
	}
	BPoint TPNormal((Dir % ExtrEdgeVec) % ExtrEdgeVec);

	int PrevEdge = NextHalfEdge(ExtrEdge);
	int NextEdge = NextHalfEdgeCCW(ExtrEdge);
	BPoint PrevEdgeVec(GetEdgeVec(PrevEdge));
	BPoint NextEdgeVec(GetEdgeVec(NextEdge));
	double PrevSP = fabs(PrevEdgeVec * TPNormal);
	double NextSP = fabs(NextEdgeVec * TPNormal);
	bool PrevIsSecond = PrevSP * sqrt(NextEdgeVec.D2()) > NextSP * sqrt(PrevEdgeVec.D2());
	// Check ExtrEdge orientation
	if (PrevIsSecond)
	{
		ExtrEdge = GetCorrEdge(ExtrEdge);
		IsRightOriented = (GetEdgeVec(ExtrEdge) % PrevEdgeVec) * Dir < 0.;
	}
	else
		IsRightOriented = (GetEdgeVec(GetCorrEdge(NextEdge)) % ExtrEdgeVec) * Dir < 0.;
	return ExtrEdge;
}

int BDCEL::FindUnmarkedEdge() const
{
	for (int iE = 0; iE < EdgesNum; ++iE)
	{
		if (Edges[iE][2] >= 0)
			return iE;
	}
	return -1;
}

int BDCEL::GetCorrEdge(int HalfEdgeInd) const
{
	return HalfEdgeInd + Edges[HalfEdgeInd][3];
}

const PointCoordinateType c_defaultSearchRadius = static_cast<PointCoordinateType>(MIND);
static bool TagDuplicatedVertices(const CCLib::DgmOctree::octreeCell& cell,
	void** additionalParameters,
	CCLib::NormalizedProgress* nProgress/*=0*/)
{
	GenericChunkedArray<1, int>* equivalentIndexes = static_cast<GenericChunkedArray<1, int>*>(additionalParameters[0]);

	//we look for points very near to the others (only if not yet tagged!)

	//structure for nearest neighbors search
	CCLib::DgmOctree::NearestNeighboursSphericalSearchStruct nNSS;
	nNSS.level = cell.level;
	nNSS.prepare(c_defaultSearchRadius, cell.parentOctree->getCellSize(nNSS.level));
	cell.parentOctree->getCellPos(cell.truncatedCode, cell.level, nNSS.cellPos, true);
	cell.parentOctree->computeCellCenter(nNSS.cellPos, cell.level, nNSS.cellCenter);
	//*/

	unsigned n = cell.points->size(); //number of points in the current cell

									  //we already know some of the neighbours: the points in the current cell!
	try
	{
		nNSS.pointsInNeighbourhood.resize(n);
	}
	catch (.../*const std::bad_alloc&*/) //out of memory
	{
		return false;
	}

	//init structure with cell points
	{
		CCLib::DgmOctree::NeighboursSet::iterator it = nNSS.pointsInNeighbourhood.begin();
		for (unsigned i = 0; i < n; ++i, ++it)
		{
			it->point = cell.points->getPointPersistentPtr(i);
			it->pointIndex = cell.points->getPointGlobalIndex(i);
		}
		nNSS.alreadyVisitedNeighbourhoodSize = 1;
	}

	//for each point in the cell
	for (unsigned i = 0; i < n; ++i)
	{
		int thisIndex = static_cast<int>(cell.points->getPointGlobalIndex(i));
		if (equivalentIndexes->getValue(thisIndex) < 0) //has no equivalent yet 
		{
			cell.points->getPoint(i, nNSS.queryPoint);

			//look for neighbors in a (very small) sphere
			//warning: there may be more points at the end of nNSS.pointsInNeighbourhood than the actual nearest neighbors (k)!
			unsigned k = cell.parentOctree->findNeighborsInASphereStartingFromCell(nNSS, c_defaultSearchRadius, false);

			//if there are some very close points
			if (k > 1)
			{
				for (unsigned j = 0; j < k; ++j)
				{
					//all the other points are equivalent to the query point
					const unsigned& otherIndex = nNSS.pointsInNeighbourhood[j].pointIndex;
					if (static_cast<int>(otherIndex) != thisIndex)
						equivalentIndexes->setValue(otherIndex, thisIndex);
				}
			}

			//and the query point is always root
			equivalentIndexes->setValue(thisIndex, thisIndex);
		}

		//if (nProgress && !nProgress->oneStep())
		//{
		//	return false;
		//}
	}

	return true;
}

int BDCEL::IdentifyPointsOc(const std::vector<ClrPoint>& AllPoints, std::vector<int>& EqNums)
{
	//vertices
	SimpleCloud vertices;
	auto PointsNumber = AllPoints.size();
	vertices.reserve(PointsNumber);
	for (int k = 0; k < AllPoints.size(); ++k)
	{
		const BPoint& P = AllPoints.at(k);
		vertices.addPoint(CCVector3(P.GetX(), P.GetY(), P.GetZ()));
	}

	unsigned vertCount = vertices.size();
	unsigned remainingCount = 0;
	//remove duplicated vertices
	GenericChunkedArray<1, int>* equivalentIndexes = new GenericChunkedArray < 1, int >;
	const int razValue = -1;
	if (equivalentIndexes && equivalentIndexes->resize(vertCount, true, razValue))
	{
		DgmOctree octree(&vertices);
		if (!octree.build(0))
		{
			octree.clear();
		}
		else
		{
			void* additionalParameters[] = { static_cast<void*>(equivalentIndexes) };
			unsigned result = octree.executeFunctionForAllCellsAtLevel(10,
				TagDuplicatedVertices,
				additionalParameters,
				false,
				0,
				"Tag duplicated vertices");

			octree.clear();

			if (result != 0)
			{
				for (int i = 0; i < PointsNumber; ++i)
				{
					int eqIndex = equivalentIndexes->getValue(i);
					assert(eqIndex >= 0);
					EqNums[i] = eqIndex;
				}
			}
			else
			{
				//					ccLog::Warning("[STL] Duplicated vertices removal algorithm failed?!");
			}
		}
	}
	else
	{
		//			ccLog::Warning("[STL] Not enough memory: couldn't removed duplicated vertices!");
	}

	if (equivalentIndexes)
		equivalentIndexes->release();
	equivalentIndexes = 0;


	return remainingCount;
}

int BDCEL::IdentifyPointsOc(std::vector<int>& PNums, size_t PointsNumber, std::vector<int>& BackNums)
{
	//vertices
	SimpleCloud vertices;
	vertices.reserve(PointsNumber);
	for (int i = 0; i < PointsNumber; ++i)
		vertices.addPoint(CCVector3(GetPointPtrByStorNum(PNums[i])));

	unsigned vertCount = vertices.size();
	unsigned remainingCount = 0;
	//remove duplicated vertices
	GenericChunkedArray<1, int>* equivalentIndexes = new GenericChunkedArray < 1, int >;
	const int razValue = -1;
	if (equivalentIndexes && equivalentIndexes->resize(vertCount, true, razValue))
	{
		DgmOctree octree(&vertices);
		if (!octree.build(0))
		{
			octree.clear();
		}
		else
		{
			void* additionalParameters[] = { static_cast<void*>(equivalentIndexes) };
			unsigned result = octree.executeFunctionForAllCellsAtLevel(10,
				TagDuplicatedVertices,
				additionalParameters,
				false,
				0,
				"Tag duplicated vertices");

			octree.clear();

			if (result != 0)
			{
				for (int i = 0; i < PointsNumber; ++i)
				{
					int eqIndex = equivalentIndexes->getValue(i);
					assert(eqIndex >= 0);
					if (eqIndex == static_cast<int>(i)) //root point
					{
						BackNums[PNums[i]] = remainingCount;
						++remainingCount;
					}
				}
				for (int i = 0; i < PointsNumber; ++i)
				{
					int eqIndex = equivalentIndexes->getValue(i);
					assert(eqIndex >= 0);
					if (eqIndex != static_cast<int>(i)) //root point
					{
						BackNums[PNums[i]] = BackNums[PNums[eqIndex]];
					}
				}
			}
			else
			{
				//					ccLog::Warning("[STL] Duplicated vertices removal algorithm failed?!");
			}
		}
	}
	else
	{
		//			ccLog::Warning("[STL] Not enough memory: couldn't removed duplicated vertices!");
	}

	if (equivalentIndexes)
		equivalentIndexes->release();
	equivalentIndexes = 0;


	return remainingCount;
}

int BDCEL::NextEdge(int StartIndex) const
{
	const Edge &CurEdge = Edges[StartIndex];
	switch (CurEdge[3])
	{ //(-1 - correspondent halfedge is previous / 0 - is absent / 1 - is next / -2 - this half edge is dummy)
	case -2:
	case -1:
	case 0:
		return StartIndex + 1;
	case 1:
		return StartIndex + 2;
	}
	return EdgesNum;// Internal error
}

void BDCEL::Divide1Edge(int EdgeInd, int PointInd)
{
	// It is assumed that half edge is not dummy
	// and point lies on the edge
	int HELeft[3]; // Half edges of original left triangle
	HELeft[0] = EdgeInd;
	HELeft[1] = Edges[HELeft[0]][1];
	HELeft[2] = Edges[HELeft[1]][1];

	int LFace = Edges[EdgeInd][2];// Original left face
	int LFaceNew = AddFace(HELeft[2], Faces[LFace][1]);// New left face
	int VertIndNew = AddVertex(EdgeInd, LFace, PointInd);

	int HalfEdgeNew = AddEdgeBorder(Edges[EdgeInd][0], -1, LFaceNew);// first part of the edge to divide
	Vertexes[Edges[EdgeInd][0]][0] = HalfEdgeNew;// this vertex may contain edge to divide. We should avoid this
	int DivEdgeLeft = AddEdge(Edges[HELeft[2]][0], VertIndNew, HELeft[2], EdgeInd, LFace, LFaceNew);// new edge on left face
	Edges[HalfEdgeNew][1] = DivEdgeLeft + 1;
	Edges[EdgeInd][0] = VertIndNew;
	Edges[HELeft[1]][1] = DivEdgeLeft;
	Edges[HELeft[2]][1] = HalfEdgeNew;
	Edges[HELeft[2]][2] = LFaceNew;

	Faces[LFace][0] = HELeft[0];

	int IdentShift = Edges[EdgeInd][3];
	if (abs(IdentShift) == 1)// EdgeInd is not border edge
	{
		int PairedHalfEdge = EdgeInd + IdentShift;
		int HERight[3]; // Half edges of original Right triangle
		HERight[0] = PairedHalfEdge;
		HERight[1] = Edges[HERight[0]][1];
		HERight[2] = Edges[HERight[1]][1];

		int RFace = Edges[HERight[0]][2];// Original right face
		int RFaceNew = AddFace(HERight[1], Faces[RFace][1]);// New right face

		Edges[HalfEdgeNew][3] = +1;// make not border
		HalfEdgeNew++;// second part of the edge to divide
		int DivEdgeRight = AddEdge(Edges[HERight[2]][0], VertIndNew, HERight[2], HalfEdgeNew, RFaceNew, RFace);// new edge on Right face
		Edges[HalfEdgeNew][0] = VertIndNew;
		Edges[HalfEdgeNew][1] = HERight[1];
		Edges[HalfEdgeNew][2] = RFaceNew;
		Edges[HalfEdgeNew][3] = -1;

		Edges[PairedHalfEdge][1] = DivEdgeRight + 1;
		Edges[HERight[1]][1] = DivEdgeRight;
		Edges[HERight[1]][2] = RFaceNew;

		Faces[RFace][0] = HERight[0];
	}
}

BPoint BDCEL::GetEdgeMidPoint(int EdgeInd) const
{
	BPoint Pb(GetPointByStorNum(Vertexes[GetESVn(EdgeInd)][2]));
	BPoint Pe(GetPointByStorNum(Vertexes[GetEEVn(EdgeInd)][2]));
	return (Pb + Pe) * 0.5;
}

int BDCEL::FindNextLongEdge(double MaxEdgeLength, int StartIndex) const
{
	for (int i = StartIndex; i < EdgesNum; i = NextEdge(i))
	{
		if (IsDummyEdge(i))
			continue;
		if (GetEdgeVecD2(i) > MaxEdgeLength * MaxEdgeLength)
			return i;
	}
	return -1;
}

int BDCEL::FindLongestEdge(double MaxEdgeLength, int StartIndex) const
{
	double d2Max = 0.;
	int iMax = -1;
	for (int i = StartIndex; i < EdgesNum; i = NextEdge(i))
	{
		if (IsDummyEdge(i))
			continue;
		double d2 = GetEdgeVecD2(i);
		if (d2Max < d2 && d2 > MaxEdgeLength * MaxEdgeLength)
		{
			d2Max = d2;
			iMax = i;
		}
	}
	return iMax;
}

int BDCEL::FindLongestEdgeLocal(double MaxEdgeLength, int StartIndex /* = 0 */) const
{
	int LongEdge = FindNextLongEdge(MaxEdgeLength, StartIndex);
	if (LongEdge < 0 && StartIndex > 0)
		LongEdge = FindNextLongEdge(MaxEdgeLength, 0);
	if (LongEdge < 0)
		return -1;
	// Find an edge longest in the both adjacent triangles simultaneously
	// this process may pass all edges in the worst case
	double d2Max = GetEdgeVecD2(LongEdge);
	bool SecondContour = false;
	for (int i = 0; i < EdgesNum; ++i)// to prevent cycling only
	{
		int EdgeMax = LongEdge;
		// Find longest edge on the triangle
		for (int i = 0, CurEdge = Edges[LongEdge][1]; i < 2 && CurEdge >= 0; ++i, CurEdge = Edges[CurEdge][1])
		{
			double d2 = GetEdgeVecD2(CurEdge);
			if (d2 > d2Max) 
			{
				d2Max = d2;
				EdgeMax = CurEdge;
			}
		}
		// Stop if edge is longest and continuation is not needed
		int NewLongEdge = PairedHalfEdge(EdgeMax);
		if (NewLongEdge < 0)
			return EdgeMax;
		if (SecondContour && EdgeMax == LongEdge)
			return EdgeMax;
		// Continue with new longest edge. It is longest on one triangle now
		SecondContour = true;
		LongEdge = NewLongEdge;
	}
	return -1; // internal error
}

int BDCEL::AddVertex(int NEdge, int NFace, int NPoint)
{
	Vertexes.resize(Vertexes.size() + 1);
	Vertexes.back()[0] = NEdge;
	Vertexes.back()[1] = NFace;
	Vertexes.back()[2] = NPoint;
	return int(Vertexes.size()) - 1;
}

int BDCEL::AddEdge(int Nv0, int Nv1, int Ne0, int Ne1, int NfLeft, int NfRight)
{
	int FirstInd = EdgesNum;
	EdgesNum += 2;
	Edges.resize(EdgesNum);
	ReplaceEdge(FirstInd, Nv0, Nv1, Ne0, Ne1, NfLeft, NfRight);
	return FirstInd;// the number of first half edge
}

void BDCEL::ReplaceEdge(int FirstInd, int Nv0, int Nv1, int Ne0, int Ne1, int NfLeft, int NfRight)
{
	Edges[FirstInd][0] = Nv0;
	Edges[FirstInd][1] = Ne1;
	Edges[FirstInd][2] = NfLeft;
	Edges[FirstInd][3] = +1;
	Edges[FirstInd + 1][0] = Nv1;
	Edges[FirstInd + 1][1] = Ne0;
	Edges[FirstInd + 1][2] = NfRight;
	Edges[FirstInd + 1][3] = -1;
}

int BDCEL::AddEdgeBorder(int Nv0, int Ne1, int NfLeft)
{
	int FirstInd = EdgesNum;
	EdgesNum += 2;
	Edges.resize(EdgesNum);
	ReplaceEdgeBorder(FirstInd, Nv0, Ne1, NfLeft);
	return FirstInd;// the number of first half edge
}

void BDCEL::ReplaceEdgeBorder(int FirstInd, int Nv0, int Ne1, int NfLeft)
{
	Edges[FirstInd][0] = Nv0;
	Edges[FirstInd][1] = Ne1;
	Edges[FirstInd][2] = NfLeft;
	MakeDummy(Edges, FirstInd + 1);
}

int BDCEL::AddFace(int NEdge, int NNormal)
{
	FacesNum++;
	Faces.resize(FacesNum);
	ReplaceFace(FacesNum - 1, NEdge, NNormal);
	return FacesNum - 1;
}

void BDCEL::ReplaceFace(int Ind, int NEdge, int NNormal)
{
	Faces[Ind][0] = NEdge;
	Faces[Ind][1] = NNormal;
}


int BDCEL::Check()
{
	for (int i = 0; i < EdgesNum; ++i)
	{
		if (IsDummyEdge(i))
			continue;
		int IdentShift = Edges[i][3];
		if (abs(IdentShift) != 1)// EdgeInd is border edge
			continue;
		int ci = GetCorrEdge(i);
		if (ci < 0)
			return 2;
		if (fabs(GetEdgeVecD2(i) - GetEdgeVecD2(ci)) > 1.e-5)
			return 1;
	}
	return 0;
}

int BDCEL::MakeFromSeqTri(const float* pCI, bool DoOrient)
{
	if (pCI == NULL)
		return 0;
	int NTri = int(pCI[0]);
	if (NTri < 1)
		return 0;
	unsigned int* PNums = new unsigned int[3 * NTri]; // source point numbers array
	for (int j = 0; j < 3 * NTri; PNums[j++] = j);
	int Res = MakeFromTriangles(pCI + 1, PNums, size_t(pCI[0]), DoOrient);
	delete[] PNums;
	return Res;
}

bool BDCEL::IsFacesAdjacent(int Face0, int Face1) const
{
	int EdgeInd = Faces[Face0][0];
	for (int i = 0; i < 3; ++i)
	{
		if (EdgeInd < 0)
			return false;
		int PairInd = PairedHalfEdge(EdgeInd);
		if (PairInd >= 0)
			if (Edges[PairInd][2] == Face1)
				return true;
		EdgeInd = Edges[EdgeInd][1];
	}
	return false;
}

ClrPoint BDCEL::GetClrPByVn(int i) const
{
	if(GetSVS() > 6)
		return ClrPoint(Get3fvByVn(i));
	const float* pF = Get3fvByVn(i);
	return ClrPoint(BPoint(pF[0], pF[1], pF[2]));
}

