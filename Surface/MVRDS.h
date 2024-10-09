#pragma once

#include "MVertex.h"
#include "MEdge.h"
#include "MTriFace.h"

class NTriMesh;

const int MAX_VERTEXES = 256;
const int MAX_EDGES = 2048;
const int MAX_TRIFACES = 2048;
const double MAX_RAD = 1.e12;

class MVRDS
{
public:
	MVRDS(void);
	~MVRDS(void);
protected:
	MVertex VList[MAX_VERTEXES];
	MEdge EList[MAX_EDGES];
	MTriFace TList[MAX_TRIFACES];
	int VNum;
	int ENum;
	int TNum;
	int VNumMax;
	int ENumMax;
	int TNumMax;
	BPoint SphCenter;
	double SphRadius;
public:
	int AddVertex(int Num, int ENum = -1);
	int AddEdge(int VNum1, int VNum2);
	int AddEdge(const MEdge &Edge);
	int AddTriFace(const MTriFace &TriFace);
	void Clear(void);
	bool MakeDelaunayTri();
	const BPoint *GetPoint(int VertexNum);
	int AddActiveEdge(const MEdge & Edge);
	int GetActiveEdgeNum(void);
	int GetActiveTriFaceNum(void);
	MEdge & GetEdge(int Num);
	INT_PTR StoreInMesh(NTriMesh &Mesh);
	void SetSphere(const BPoint &Center, double Rad);
	int MakeLimTolTri(double SphTolerance , const BPoint & SphC, double SphRad);
	int GetMaxLengthEdgeNum(double *pMaxLength);
	const MVertex & GetVertex(int Num);
	double GetSphRadius() const { return SphRadius; }
	const BPoint & GetSphCenter() const { return SphCenter; }
	bool RemoveEdgePatch(int Num);
	bool Check(void) const;
	void Restart(void);
	bool AddNewTriFace(int EdgeNum, int NewVertex, const BPoint &CircumCenter, double Rad);
};
