#pragma once
#include <vector>
#include "BBox.h"
#include "BPoint.h"
#include "MElemIDProc.h"
#include "BStockGrID.h"
#include "CadrID.h"

class CadrData
{
	friend class DXSubModel;
	friend class DXSubModel5X;
public:
	CadrData() { Last = false; }
	~CadrData(){}
	void MakeLast(DX_TOOLTYPE iDXType) { Last = true; DXType = iDXType; }
	void MakeEmpty() { Last = false; }
	void MakeEmtyMask()
	{
		Last = false;
		CylRight = true;
		IsPlane = false;
		R = 0.;
		DXType = TT_NO;
		FirstCadr = false;
		LastCadr = false;
		CylID1 = ID_EMPTY;
		CylID2 = ID_EMPTY;
		CylIDb = ID_EMPTY;
		CylIDt = ID_EMPTY;
		DiskIDt = ID_EMPTY;
		SphID1 = ID_EMPTY;
		SphID2 = ID_EMPTY;
		Plane1ID = ID_EMPTY;
		Plane2ID = ID_EMPTY;
		HistID = cadrID(-1);
	}
	bool IsLast() const { return Last; }
	DX_TOOLTYPE GetDXType() const { return DXType; }
	void SetStockPos(const std::vector<BStockGrID>& Groups) { StockGroups = Groups; }
	auto& GetStockGroups() const { return StockGroups; }
protected:
	bool Last;
	bool CylRight;
	bool IsPlane;
	double R;
	DX_TOOLTYPE DXType;
	bool FirstCadr;
	bool LastCadr;
	BBox Bounds;
	BPoint P0;
	BPoint P1;
	BPoint N;
	BPoint N1;
	BPoint P0Up;
	BPoint P1Up;
	ElemID CylID1;
	ElemID CylID2;
	ElemID CylIDb; // bottom cylinder
	ElemID CylIDt; // top cylinder
	ElemID DiskIDt; // top disk on P0Up
	ElemID SphID1;
	ElemID SphID2;
	ElemID Plane1ID;
	ElemID Plane2ID;
	std::vector<BStockGrID> StockGroups;
	cadrID HistID;
};
