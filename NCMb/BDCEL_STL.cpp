#include "stdafx.h"
#include "BStlB.h"
#include "BDCEL_STL.h"

BDCEL_STL::BDCEL_STL(void)
	: BDCEL_TRM()
{
	SOURCE_VERTEX_SIZE = 6;
	SetSmoothAngle(15.);
}

BDCEL_STL::~BDCEL_STL(void)
{
}

int BDCEL_STL::MakeFromStl(const BStlB &Stl)
{
	return MakeFromSeqTri(Stl.GetTriangles(), true);
}
