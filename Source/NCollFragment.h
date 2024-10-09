#pragma once
#include "NCadrGeomArray.h"

class NCollFragment
{
public:
	NCollFragment(class NCollManagerOrd& iCollManager, BBox SGabar);
	~NCollFragment();
	void Set(const class NToolCombined* pTool, const BProgram* pProg, int ProgNum, class NMachUnitPair* pMU);
	void Reset(const CArray<NCadrGeomArray>& Geoms, int GNumber, int RangePosCN);
	int CheckingProc(bool StopIfStock, bool LastOnly);
	bool FixNonCollTime(bool StopIfStock);
	bool CheckOnePosition(bool StopIfStock, bool IsFast5X, const ProgTPoint& PTPoint);
	void CalcStockCollFound();
	int MoveGeoms(int Shift, CArray<NCadrGeomArray>& Dest, const CArray<NCadrGeomArray>& Src); // Shift array for Shift positions
	bool GetNonStockCollFound() const { return NonStockCollFound; }
	bool GetStockCollFound() const { return StockCollFound; }
protected:
	const CArray<NCadrGeomArray>* pGeoms;
	int _GNumber;
	class NMachUnitPair* pMUPair;
	BBox StockGabar;
	const BProgram* pProg;
	const class NToolCombined* pTool;
	int ProgNum;
	NCadrGeom Dummy;
	int RangePosCadrNum;
	bool NonStockCollFound;
	bool StockCollFound;
	bool LastCheckHaveColl;
	NCollManagerOrd& CollManager;
};

