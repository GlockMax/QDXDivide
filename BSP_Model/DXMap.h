#pragma once

#include <vector>
#include "DXMemoryCell5X.h"
#include "GQuadRenderDX5X.h"
#include "DXCell5X.h"
#include "DXEdge.h"

class DXCellInd // Indexes of a cell
{
	friend class DXMap;
public:
	DXCellInd() {}
	DXCellInd(int ix, int iy, int iz) { Ix = ix; Iy = iy; Iz = iz; }
	~DXCellInd() {}
	DXCellInd(const DXCellInd& In) { *this = In; }
	const DXCellInd &ShiftX(int i) { Ix += i; return *this;}
	const DXCellInd &ShiftY(int i) { Iy += i; return *this;}
	const DXCellInd &ShiftZ(int i) { Iz += i; return *this;}
public:
	int Ix, Iy, Iz;
};

class DXSearchPair
{
	friend class DXMap;
public:
	DXSearchPair(void);
	~DXSearchPair(void) {}
	bool StepUp(void);
	void Init(DXMemID StartID);
	void SetMemory(class DXMemoryCell5X *pMem);

protected:
	DXMemID DownID;
	DXMemID UpID;
	int DownZInd;
	int UpZInd;
	const class DXMemoryCell5X *pMemory;
	static const int HugeZInd = 32000;
};

class DXMap
{
	friend class DXCell5X;
public:
	DXMap();
	~DXMap();
	void Set(const DXSubModel5X& Body5X, int B0, int B1);
	void AddEdgeZ(const DXCellInd &NumCell, int Shift, int NumEdge, unsigned int Edge, DXBigCell5X *pBigCell);
	void AddEdgeY(const DXCellInd &NumCell, int Shift, int NumEdge, unsigned int Edge, DXBigCell5X *pBigCell);
	void AddEdgeX(const DXCellInd &NumCell, int Shift, int NumEdge, unsigned int Edge, DXBigCell5X *pBigCell);
	void InitZFast(int NextI, int NextJ);
	void InitYFast(int NextI);
	void NewInitYFast(int NextI);
	void InitXFast(int NextI);
	void InitFast(int Size);
	void InitFastZ();
	void InitFastY();
	void InitFastX();
	void FinishFast(void);
	DXCell5X * GetCell(const DXCellInd &CellInd) const;
	DXMemoryCell5X &GetMemory(void) { return MemCell;}
	DXMemID GetMemID(int i, int j) const { return RectID[ i * nY + j];}
	void MakeTrianglesBigColumn() const;
	int SetEdge(const float *Coord, ElemID Surf, unsigned int &Edge, bool i, unsigned int Degraded, DXBigCell5X *pBigCell) const;
	int MakeNewEdge(unsigned int Dir, const float *Coord, const BPoint &Normal, unsigned int &Edge, DXCell5X *pCell) const;

protected:
	DXCell5X& GetCellFast(const DXCellInd& CellInd, DXMemID& RectIDPos, DXSearchPair& SearchPair);
	DXCell5X& GetCellSlow(const DXCellInd& CellInd, DXMemID& RectIDPos);

protected:
	DXMemoryCell5X MemCell;
	DXMemID *RectID;
	int startX;
	int startY;
	int nX;
	int nY;
	int nZ;
	double DeltaZ;
	int nBig[3];
	static constexpr int START_SIZE = 256;
private:
	DXSearchPair *BufLines[2];
};
