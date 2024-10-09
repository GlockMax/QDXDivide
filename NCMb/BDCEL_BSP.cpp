#include "stdafx.h"
#include "math.h"
#include "BDCEL_BSP.h"

BDCEL_BSP::BDCEL_BSP(void) 
	: BDCEL_TRM()
{
	SOURCE_VERTEX_SIZE = 7;
	SetSmoothAngle(44.);
	pN = NULL;
}

BDCEL_BSP::~BDCEL_BSP(void)
{
	delete[] pN;
}

const BPoint BDCEL_BSP::GetNormByNum(int Num) const
{
	// Get normal from pN by it's index
	// Num must be divisible by 3
	int IndpN = Num;
	return BPoint(
		  double(pN[IndpN + 0])
		, double(pN[IndpN + 1])
		, double(pN[IndpN + 2])
		, 0.);
}

int BDCEL_BSP::MakeFromStockTri(const float *pCI)
{
	if (pCI == NULL)
		return 0;
	int NTri = int(pCI[0]);
	if (NTri < 1)
		return 0;
	pN = new float[3 * NTri];
	return MakeFromSeqTri(pCI, false);
}

void BDCEL_BSP::StoreNormal(int Ind, const BPoint &N)
{
	// Ind must be divisible by 3
	int IndpN = Ind;
	double d = N.D2();
	BPoint NN(N);
	if(d < 1.e-12)
	{
		NN = BDCEL::GetNormByNum(Ind);
	}
	else
	{
		NN = NN * (1. / sqrt(d));
	}
	pN[IndpN + 0] = float(NN.GetX());
	pN[IndpN + 1] = float(NN.GetY());
	pN[IndpN + 2] = float(NN.GetZ());
}
