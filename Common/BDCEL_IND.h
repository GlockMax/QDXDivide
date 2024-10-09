#pragma once
#include "BDCEL_BSP.h"
class NCMB_API BDCEL_IND :
	public BDCEL_BSP
{
public:
	BDCEL_IND(void);
	virtual ~BDCEL_IND(void);
	int MakeFromTriMesh(const float *pCI, const unsigned int *pIndexes, int NTri);
	int IdentifyPoints(const float *pCI, const unsigned int *pIndexes, int NTri, int *&BackNums);
protected:
	size_t PIndByIInd(size_t Ind) const override;
	const unsigned int *TrianglesInd; // Indexes of triangles vertexes in pC

};

