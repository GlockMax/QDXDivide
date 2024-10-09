#pragma once

#include "NCMB_API.h"
#include "math.h"
#include <vector>
#include "ClrPoint.h"
// A number of half edges must be even
// If twin half edge does not exist we place dummy half edge
//typedef int Vertex[3]; // Ne, Nf, No(number of original point)
//typedef int Edge[4]; // Half edge Nv0, Ne1, Nfl, IdentShift(-1 - correspondent halfedge is previous/0 - is absent/1 - is next/ -2 - this half edge is dummy)
//typedef int Face[2]; // Ne, Nn
class Face
{
public:
	int & operator[](int Ind) { return (data[Ind]); }
	const int operator[](int Ind) const { return (data[Ind]); }
protected:
	int data[2];// Ne, Nn
};

class Edge
{
public:
	int & operator[](int Ind) { return (data[Ind]); }
	const int operator[](int Ind) const { return (data[Ind]); }
protected:
	int data[4];// Half edge Nv0, Ne1, Nfl, IdentShift(-1 - correspondent halfedge is previous/0 - is absent/1 - is next/ -2 - this half edge is dummy)
};

class Vertex
{
public:
	int & operator[](int Ind) { return (data[Ind]); }
	const int operator[](int Ind) const { return (data[Ind]); }
protected:
	int data[3];// Ne, Nf, No(number of original point)
};

const double C1 = 1. / sqrt(14.);
const double CX1 = C1;
const double CY1 = 2. * C1;
const double CZ1 = 3. * C1;

#define CALC_D(X, Y, Z) ((X) * CX1 + (Y) * CY1 + (Z) * CZ1)

enum CompareType
{
	points_by_d,
	edges_by_SVn,
	edges_by_EVn,
	edges_by_2Vn
};
enum QR_PTR
{
	QR_EMPTY = NULL,
	QR_EMPTY_DEF = NULL + 1
};

class NCMB_API BDCEL
{ // double connected edges list
public:
	BDCEL(void);
	virtual ~BDCEL(void);
	int GetESVn(int i) const { return Edges[i][0];} 
	int GetEEVn(int i) const { return Edges[Edges[i][1]][0];}
	const float* Get3fvByVn(int i) const;
	virtual ClrPoint GetClrPByVn(int i) const;
	void DrawEdges(bool Sharp, bool Error, bool Smooth) const;
	int NextIncidentEdge(int StartVertexInd, int CurEdgeInd) const;
	int * FillEdges(bool Sharp, bool Error, bool Smooth) const;
	int NextHalfEdge(int HalfEdge) const;
	int PairedHalfEdge(int HalfEdge) const;
	int NextHalfEdgeCCW(int HalfEdge) const;
	const BPoint GetEverNormal(int VertInd) const;
	const BPoint GetEdgeVec(int HalfEdgeInd) const;
	double GetEdgeVecD2(int HalfEdgeInd) const;
	bool IsSharpEdge(int i) const;
	bool IsRotZSilhoueteEdge(int i) const;
	int IsConvexEdge(int i) const;// -1 concave; 0 - flat; 1 - convex; -2 - border
	static bool Empty(const void *in) { return (in == (void *)QR_EMPTY) || (in == (void *)QR_EMPTY_DEF); }
	static bool NotDefined(const void *in) { return (in == (void *)QR_EMPTY); }


	std::vector<Vertex>& GetVertexes(void) { return Vertexes; }
	const std::vector<Vertex>& GetVertexes(void) const { return Vertexes; }
	std::vector<Edge>& GetEdges(void) { return Edges; }
	std::vector<Edge>& GetEdges(void) const { return Edges; }
	std::vector<Face>& GetFaces(void) { return Faces; }
	const std::vector<Face>& GetFaces(void) const { return Faces; }
	int GetVertexCount(void) const { return int(Vertexes.size()); }
	int GetFacesCount(void) const { return FacesNum; }
	int GetEdgesCount(void) const { return EdgesNum; }

	virtual BPoint GetPointByNum(size_t Num) const;
	virtual BPoint GetPointByStorNum(int Num) const;
	const float * GetBasePtrByNum(int Num) const;
	const float * GetPointPtrByStorNum(int Num) const;
	bool IsCreated() const;
	bool IsBorderEdge(int HalfEdgeInd) const { return Edges[HalfEdgeInd][3] == 0;}
	bool IsDummyEdge(int HalfEdgeInd) const { return Edges[HalfEdgeInd][0] < 0; }
	bool IsMarkedEdge(int HalfEdgeInd) const { return Edges[HalfEdgeInd][2] < -1; }
	bool IsDummyFace(int FaceInd) const { return Faces[FaceInd][0] == -1; }
	bool IsMarkedFace(int FaceInd) const { return Faces[FaceInd][0] < -1; }
	int GetNextBorder(int HalfEdgeInd) const;
	int GetSVS() const { return SOURCE_VERTEX_SIZE; }
	int HealTri(double Tol);
	void DrawEdge(int EdgeInd) const;
	bool IsEdgesOverlap(const BPoint& Pc, const BPoint& Pl, const BPoint& Ps);
	static void MakeDummy(std::vector<Edge>& EBuf, int Ind);
	void HealBorders(int* BENums);
	void WriteSTL(class CStdioFile& f);
	bool WriteSTLmOneSolid(class CStdioFile& f, int StartEdge);
	void UnmarkAll();
	void InvertMarks();
	double MinTriH2(int HalfEdgeInd);
	static double MinTriH2(const BPoint& P0, const BPoint& P1, const BPoint& P2);
	void SetSmoothAngle(double Angle);
	void Clear();
	int IdentifyPoints(const float* pCI, int* PNums, size_t TrianglesNum, int*& BackNums);
	int IdentifyPoints(const float* pCI, std::vector<int>& PNums, int PointsNumber, std::vector<int>& BackNums);
	int IdentifyPointsOc(std::vector<int>& PNums, size_t PointsNumber, std::vector<int>& BackNums);
	static int IdentifyPointsOc(const std::vector<ClrPoint>& AllPoints, std::vector<int>& EqNums);
	int FindExtremeEdge(bool& IsRightOriented) const;
	int FindUnmarkedEdge() const;
	int GetCorrEdge(int HalfEdgeInd) const;
	int FindNextLongEdge(double MaxEdgeLength, int StartIndex) const;
	int FindLongestEdge(double MaxEdgeLength, int StartIndex) const;
	int FindLongestEdgeLocal(double MaxEdgeLength, int StartIndex = 0) const;
	int NextEdge(int StartIndex) const;
	void Divide1Edge(int EdgeInd, int PointInd);
	BPoint GetEdgeMidPoint(int EdgeInd) const;
	int AddVertex(int NEdge, int NFace, int NPoint);
	int AddEdge(int Nv0, int Nv1, int Ne0, int Ne1, int NfLeft, int NfRight);
	void ReplaceEdge(int Ind, int Nv0, int Nv1, int Ne0, int Ne1, int NfLeft, int NfRight);
	int AddEdgeBorder(int Nv0, int Ne1, int NfLeft);
	void ReplaceEdgeBorder(int Ind, int Nv0, int Ne1, int NfLeft);
	int AddFace(int NEdge, int NNormal);
	void ReplaceFace(int Ind, int NEdge, int NNormal);
	int Check();
	virtual const BPoint GetNormByNum(int Num) const;
	const ClrPoint RayCasting(const BPoint& P, const BPoint& V, double MinW, BPoint& N) const;
	size_t RayShreddingTri(const BPoint& P, const BPoint& V, double MinW, std::vector<ClrPoint>& Pts) const;
	void MarkConnected(int ExtremeEdge);
	int MakeFromSeqTri(const float* pCI, bool DoOrient);
	bool HaveTripleEdges() const { return TripleEdgesNum > 0; }
	bool IsFacesAdjacent(int Face0, int Face1) const;
	void MarkEdge(int HEdgeNum);
	void MarkEdgePair(int HEdgeNum);
	UINT FindNextSharp(size_t HEdgeNum) const;

protected:
	const BPoint GetNormByEdge(int HalfEdgeInd) const;
	int compare_edges_by_SVn( int a, int b );
	int compare_edges_by_EVn( int a, int b );
	int compare_points_by_d(int a, int b );
	int compare_edges_by_2Vn( int a, int b );
	int Compare(int a, int b, enum CompareType Type);
	virtual double CalcDist(int Ind) const;
	int FindFirstS(int *BENums, int Size, int EVi);	
	int FindS(int *BENums, int Size, int EVi, int SVi, int BEI1);	
	int FindE(int *BENums, int Size, int SVi, int EVi, int BEI1);	
	int FindFirstE(int *BENums, int Size, int EVi);	
	int MakeFromTriangles(const float *pCI, const unsigned int *pTriInds, size_t TrianglesNum, bool DoOrient);
	virtual void StoreNormal(int Ind, const BPoint &N){ N, Ind; }
	void QuickSortRecursive(int *pArr, int d, int h, enum CompareType Type);
	BOOL QuickSort(int *pArr, int iSize, enum CompareType Type);
	void MarkFaceEdges(int EdgeNum);
	void DoOrientWave();
	void FillOneEdge(int EdgeNum, int* pRet) const;
	virtual size_t PIndByIInd(size_t Ind) const { return Ind;} // Point index in float array by point index in triangles array
	const float* GetBasePtrByStorNum(int Num) const { return pC + Num * SOURCE_VERTEX_SIZE; }
protected:
	static const double PointTol;
	std::vector<Vertex> Vertexes;
	std::vector<Edge> &Edges;
	std::vector<Edge> BEdges;
	std::vector<Face> Faces;
	int EdgesNum;
	int FacesNum;
	int BorderEdges;

	const float *pC;
	int SOURCE_VERTEX_SIZE;
	double MIN_SMOOTH_COS;
	std::vector<Edge> BufEdges;
	bool TRMMode;
	int TripleEdgesNum;
};
