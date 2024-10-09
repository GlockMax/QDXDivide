#pragma once
#include "DXMemory.h"
class DXPIterator
{
	friend class GQuadRenderDX5X;
public:
	DXPIterator(class DXSubModel5X &Mod, int DirInd = -1, int CellInd0 = -1, int CellInd1 = -1);
	~DXPIterator(void);
	void Start(void);
	const DXPIterator &operator ++ (void);
	bool GetPoint(float *BUF) const;
	bool IsStopped() const {return (P == DXP_END);}
protected:
	class DXSubModel5X &Mod5X;
	int DirNum;// current indexes
	int i0; 
	int i1;
	DXMemID P;
	int Cell0; // Cell indexes
	int Cell1;
	int DirIndex;
	int N0;
	int N1;
};

