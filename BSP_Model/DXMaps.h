#pragma once
#include "DXMap.h"

class DXMaps
{
public:
	DXMaps(int nX, int nY);
	~DXMaps();
	DXMap* operator [] (int ind0) const { return Line0[ind0]; }
	void MakeTrianglesPar() const;
protected:
	int n0;
	int n1;
	DXMap** Line0;
};

