#include "stdafx.h"
//#include "RTriRandMesh.h"
#include "BDCEL_IND.h"


BDCEL_IND::BDCEL_IND(void) : BDCEL_BSP()
{
	TrianglesInd = NULL;
	SOURCE_VERTEX_SIZE = 8;
}


BDCEL_IND::~BDCEL_IND(void)
{
}

size_t BDCEL_IND::PIndByIInd(size_t Ind) const  // Point index in float array by point index in triangles array
{
	if(TrianglesInd)
		return TrianglesInd[Ind];
	return Ind;
}

int BDCEL_IND::MakeFromTriMesh(const float *pCI, const unsigned int *pIndexes, int NTri)
{
	if (pCI == NULL || pIndexes == NULL)
		return 0;
	if (NTri < 1)
		return 0;
	pN = new float[3 * NTri];
	TrianglesInd = pIndexes;

	int Res = 0;// MakeFromTriangles(pCI, TrianglesInd, NTri);
	return Res;
}

int BDCEL_IND::IdentifyPoints(const float *pCI, const unsigned int *pIndexes, int NTri, int *&BackNums)
{
	if (pCI == NULL || pIndexes == NULL)
		return 0;
	if (NTri < 1)
		return 0;
	pN = new float[3 * NTri];
	TrianglesInd = pIndexes;

	int *PNums = new int[3 * NTri]; // source point numbers array
	for (int j = 0; j < 3 * NTri; ++j)
		PNums[j] = pIndexes[j];
	int Res = BDCEL::IdentifyPoints(pCI, PNums, NTri, BackNums);
	delete[] PNums;
	return Res;
}
