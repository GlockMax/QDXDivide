#pragma once
#include "DXMemory.h"
#include "DXCell5X.h"

class DXMemoryCell5X : public DXMemoryOnePart
{
public:
	DXMemoryCell5X(unsigned int StartSize);
	~DXMemoryCell5X(void);
	void SetAtC(DXMemID Ind, const DXCell5X &CVal) { C[Ind.Up][Ind.Low] = CVal; }
	class DXCell5X & SetNew(DXMemID Ind) { return C[Ind.Up][Ind.Low].Clear(); }
	void SetAtiZ(DXMemID Ind, __int16 ZVal) { iZ[Ind.Up][Ind.Low] = ZVal; }
	class DXCell5X & GetAtC(DXMemID Ind) const { return C[Ind.Up][Ind.Low]; }
	int GetAtiZ(DXMemID Ind) const { return iZ[Ind.Up][Ind.Low]; }
	int GetGlobSize(void);
	int GetUsedSize(void);
protected:
	bool Extend(void);
	class DXCell5X *C[MaxPartsNum];
	__int16 *iZ[MaxPartsNum];
};
