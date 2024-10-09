#pragma once
#include "NCTOOL_API.h"
#include "BGeomArray.h"
class NCTOOL_API NTurnStockSkeleton
{
public:
	enum
	{
		SkelContour1 = 1,
		SkelContour2 = 2,
		SkelArcs = 3
	};
	NTurnStockSkeleton();
	~NTurnStockSkeleton();
	const BGeomArray *GetSectGeom(int Part) const;
	void Draw(void);
	bool Create(class MQuadTreeRot &QuadTreeRot);
	bool IsSorted(void) const { return Sorted; }
	unsigned int GetUuid() const { return Uuid; }
protected:
	void RecalcArcs(void);
	bool MakeUuid();
protected:
	BGeomArray SectGeom1;
	BGeomArray SectGeom2;
	BGeomArray Arcs;
	bool Sorted;
	unsigned int Uuid;
};

