#pragma once
#include "DXDirection.h"
#include "CadrID.h"

class BPoint;

class BSP_MODEL_API DXDirection3X : public DXDirection
{
	friend class DXDirection5X;
protected:
	int LayersNum;
	DX_DEPTH **Depth;
	DX_ID **IDs;
public:
	DXDirection3X(int ILayersNum = 2);
	~DXDirection3X(void);
	bool Init(const DXSubModel *pSubModel, DX_Dir, int s0, int s1, double St0, double St1, int s0Big, int s1Big, double v0, double v1, double vv);
	void ClearAllValues(double Val, double Shift);
	DX_DEPTH *GetColumnD(int Layer, int i0) const { return Depth[Layer] + n1 * i0;}
	DX_DEPTH DepthEl(int Layer, int i0, int i1) const { return GetColumnD(Layer, i0)[i1];}
	DX_DEPTH &DepthEl(int Layer, int i0, int i1) { return GetColumnD(Layer, i0)[i1];}
	DX_ID *GetColumnI(int Layer, int i0) const { return IDs[Layer] + n1 * i0;}
	DX_ID IDsEl(int Layer, int i0, int i1) const { return GetColumnI(Layer, i0)[i1];}
	DX_ID &IDsEl(int Layer, int i0, int i1) { return GetColumnI(Layer, i0)[i1];}
	bool IsDefined() const;
	void MarkUsedOrig(class BDynBitArray & Used);
	void InvertDepth(int Layer);
	void Clear(void);
	int StoreFace(const class MFace * pFace, DX_ID PairID);
	DX_ID InsRect(const BPoint & P0, const BPoint & P1, const BPoint & P2, const BPoint & P3, const BPoint & N, bool FindMax, int Layer, DX_ID PairID);
	DX_ID RastTriangle(const BPoint & P0, const BPoint & P1, const BPoint & P2, const BPoint & N, bool FindMax, int Layer, DX_ID PairID);
	DX_ID RastCylinder(const BPoint & P0, const BPoint & P1, const BPoint & Q0, const BPoint & Q1, double R, DX_ID PairID);
	//Растеризатор наклонного сегмента (Inclined Cylinder)
	DX_ID RastICylinder(const BPoint & P0, const BPoint & P1, const BPoint & N, double R, bool IsPlane, DX_ID PairID, bool flat_disk, DX_ID FPairID);
	// Raster half cyclinder
	DX_ID RastHCylinder(const BPoint & pP0, const BPoint & pP1, const BPoint &ArcCntr, double R, DX_ID PairID, bool flat_disk, DX_ID FPairID);
	DX_ID RastCylSegment(const BPoint & pP0, const BPoint & pP1, const BPoint &vA, const BPoint &vB, double R, DX_ID PairID, bool flat_disk, DX_ID FPairID);
//	DX_ID RastSphSegment(const BPoint & P0, const BPoint & P1, const BPoint & Q0, const BPoint & Q1, double R, ElemID ElID);
	DX_ID RastSphSegment(const BPoint & P0, int NPlanes, const BPoint * P, const BPoint * N, double R, DX_ID PairID);
	bool Merge(DXDirection3X & In, int &i0, int &i1);
	void Move(DXDirection3X & In);
	void MakeFromBSP(const class MBSPNode *pTree);
	bool Check() const;
};

