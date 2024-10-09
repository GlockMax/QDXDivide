#pragma once
#include "NColor.h"
#include "rtriangles.h"
#include <array>
#include <vector>

class OPENGLRENDER_API RTriRandMesh :
	public RTriangles
{
public:
	RTriRandMesh(void);
	RTriRandMesh(const RTriRandMesh& inMesh) : RTriangles(inMesh) {};
	virtual ~RTriRandMesh(void);
	virtual int GetTriNum(void) const;
	virtual int ExtractTriangle(int Ind, class BPoint &p0, BPoint &p1, BPoint &p2, BPoint &n) const;
	int ExtractTriangle(int Ind, class BPoint &p0, BPoint &p1, BPoint &p2) const;
	int ExtractTriangleInds(int Ind, int *pi0, int *pi1, int *pi2) const;
	bool DrawTriSurf();
	bool DrawTriEdges(enum RenderMode GlobRMode, const std::vector<unsigned int>& Indexes, const NColor& EdgesColor, double EdgesWidth);
	int AddTriInd(int i0, int i1, int i2);
	double RayCasting(const class BPoint& P, const class BPoint& V, double MinW, class BPoint& N, BPoint& v0, BPoint& v1, BPoint& v2) const;
};

