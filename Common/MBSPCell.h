#pragma once
#include "MACHINE_API.h"
#include "BDCEL_BSP.h"
#include "BLinesDCEL.h"

class MSideV
{
	friend class MBSPCell;
protected:
	UINT SEdgesInd;// index of incident half edge for bound points in LinesDCEL vector. This index must point to the half edge with start point equal to pP
	const float* pP;
public:
	MSideV(const float* p, UINT ind) { pP = p;  SEdgesInd = ind;}
};

class MACHINE_API MBSPCell
{
public:
	MBSPCell() {}
	MBSPCell(int iix, int iiy, double iXmin, double iXmax, double iYmin, double iYmax);
	~MBSPCell() {}
	void DrawSharpEdges() const;
	size_t Size() const { return LinesDCEL.Size(); }
	void FillFromBDCEL(BDCEL& El, bool Sharp, bool Error);
	void ReplaceEdge(const BDCEL& El, size_t OutInd, size_t DcelInd);
	void ProcEndPoint(UINT LinesInd);
	void SetShift(UINT Val) { Shift = Val; }
	void Bind(bool XShift, MBSPCell* pNextCell, BLinesDCEL& Result) const;
public:
	int ix;
	int iy;
	BLinesDCEL LinesDCEL;
	std::vector<MSideV> Sides[4];// 0 - xmin 1 - ymin 2 - xmax 3 - ymax
	double Xmin, Xmax, Ymin, Ymax;
	UINT Shift;
	UINT YIndex;
};

