#pragma once
#include "StdAfx.h"
#include "DXFinalCycle.h"
#include "DXBound.h"
//#include "DXMap.h"
#include "DXBigCell5X.h"
#include "BSP_MODEL_API.h"

// My numbering
//                            Nodes
//                         6              7
//                          ===============
//                         /|            /|
//                        / |           / |
//                       /  |          /  |
//                    4 /   |        5/   |
//                     ===============    |
//                     |    |        |    |
//                     |    |2       |    |3
//                     |    =========|=====
//                     |   /         |   /
//                     |  /          |  /
//                     | /           | /
//                     |/            |/
//                     ===============
//                    0               1
//                           Edges
//                                 6       
//                          ===============
//                         /|            /|
//                       7/ |           / |
//                       /  |          /5 |
//                      / 10|         /   |11
//                     ===============    |
//                     |    |  4     |    |
//                     |    |       2|    | 
//                     |    =========|=====
//                    8|   /         |9  /
//                     | 3/          |  / 
//                     | /           | / 1
//                     |/            |/
//                     ===============
//                            0       
//                               Bounds
//                                         
//                     Z    ===============
//                     ^   /|            /|
//                     |  / |   5       / |
//                     | /  |     3    /  |
//                     |/   |         /   | 
//                     ===============    |
//                     |  4 |        |  2 |
//                     |    |        |    | 
//                     |    =========|=====  0 - bottom
//                     |   /   1     |   /   5 - top
//                     |  /      0   |  / 
//                     | /           | /  
//                     |/            |/
//                     ====================> X
//                                    
//

// Marching cubes numbering
//                            Nodes
//                         4              5
//                          ===============
//                         /|            /|
//                        / |           / |
//                       /  |          /  |
//                    7 /   |        6/   |
//                     ===============    |
//                     |    |        |    |
//                     |    |0       |    |1
//                     |    =========|=====
//                     |   /         |   /
//                     |  /          |  /
//                     | /           | /
//                     |/            |/
//                     ===============
//                    3               2
//                           Edges
//                                 4       
//                          ===============
//                         /|            /|
//                       7/ |           / |
//                       /  |          /5 |
//                      /  8|         /   |9
//                     ===============    |
//                     |    |  6     |    |
//                     |    |       0|    | 
//                     |    =========|=====
//                   11|   /         |10  /
//                     | 3/          |  / 
//                     | /           | / 1
//                     |/            |/
//                     ===============
//                            2       
//

class DXCellInd;

class DXCell5X
{    

	friend class GQuadRenderDX5X;
	friend class DXMap;

	static const int EdgeFace[12][2];
	static const int EdgeVert[12][2];
	static const int contTable[256][32];

	typedef int FEP[6][13];
	// FaceEdgeP[3][13];// [*][0] - a points count, [1]-[4] - indexes in BUF, [5]-[8] - IDs, 9 - 12 - edges numbers inside this cell
	// We work with top 3 faces only (2, 3, 5 faces(0, 1, 2 indexes in FaceEdgeP array), (all edges except 0, 3, 8)
	static constexpr int PC = 0; // index of a points count
	static constexpr int IiB = 1; // start index of indexes in BUF
	static constexpr int ID = 5; // start index of IDs
	static constexpr int EN = 9; // start index of edges numbers inside this cell

	enum FillFaceResult
	{
		FF_4PMAX,
		FF_2PMAX,
		FF_0MAX,
		FF_FAILED,
	};

public:
	DXCell5X::DXCell5X(void) {}

	DXCell5X::~DXCell5X(void){}

	void InitEdge(int IDEdgeCell, unsigned int Edge) { Edges[IDEdgeCell] = Edge; }
	void Init(DXBigCell5X *ipBigCell) { pBigCell = ipBigCell; }

	DXCell5X & Clear(void);
	const class DXEdge *GetEdge(int Num) const;// получить ребро
	const class DXEdge *GetEdge(const DXFinalCycle::Elem &El) const { return GetEdge(El.Number);}
	const DXBound *GetBound(int Num) const; // получить грань
	int GetBufNum(const DXFinalCycle::Elem &El, bool FirstNormal) const;
	int GetBufNum(const DXFinalCycle::Elem &El) const;
	static bool IntersectionPoint(double *outi, double *outj, float *BUFBase0, float *BUFBase1, const int BufShifts[5]);
	int MakeTriangles(const DXCellInd &Key, const DXMap &Map);
	int RecoverWhenOnSurface(const DXCellInd &Key, const DXMap &Map);
	const DXCell5X & operator = (const DXCell5X &In) { memcpy(this, &In, sizeof(In));  return *this; }
protected:
	FillFaceResult FillFaceEdgeP(FEP FaceEdgeP, bool Check = false) const;
	void MakeAdditPoints(const DXCellInd &Key, const FEP FaceEdgeP);
	void StoreAdditPoint(const BPoint &BoundP, int FaceNum, const int *EdgeP);
	bool FindBoundPoint(int LocalFaceNum, const int *EdgeP, BPoint &BoundP);
	int MakeFinalCycle(const FEP FaceEdgeP);
	int MakeFinalCycle4(FEP FaceEdgeP);
	int MakeFinalCycleMC(); // Marching cubes
	bool CheckPointOnFace(float * Base, double Val[3], int LocalFaceNum, int EdgeNum);
	void Store0();
	void Store1(const DXCellInd& Key);
	void Store2(const DXCellInd& Key);
	void Store3(const DXCellInd& Key);
	void Store4(const DXCellInd& Key);
	void Store5(const DXCellInd& Key);
	void Store6(const DXCellInd& Key);
	void Store4_1Fan(int First, int Stop, const DXCellInd& Key);
	void StoreSingleFan(const DXCellInd &Key);
	void StoreSingleFan(const BPoint &NP, const DXCellInd& Key);
	void StoreOneMainEdge(int Count, int AddPoints[12], double AngleSin2[12], const DXCellInd& Key);

	bool EnsureInCell(const DXCellInd &Key, class BPoint &P);
	bool IsInCell(const DXCellInd &Key, class BPoint &P);
	static void StoreNPC(float *BUF, ElemID ID, DXBigCell5X::GroupData &GData);
	void StoreNPC(float *BUF, const BPoint &P, ElemID ID);
	void StoreNPC(float *BUF, const BPoint &P, const BPoint &N);
	void StoreNPC(float *BUF, float *P, const BPoint &N);
	void EmptyFEP(FEP iFEP) const { memset(iFEP, 0, sizeof(FEP)); }
	static int CalcWholePointsNum(const FEP FaceEdgeP);

protected:
	unsigned int Edges[12]; // массив ребер €чейки. Stores index in MapEdges
	unsigned int Bounds[6]; // массив граней €чейки. Stores index in MapBounds
	DXBigCell5X *pBigCell;
};

