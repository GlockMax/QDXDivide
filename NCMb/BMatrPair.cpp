#include "stdafx.h"
#include "BMatrPair.h"


BMatrPair::BMatrPair(const BMatr &Matr)
{
	operator = (Matr);
}

BMatrPair::BMatrPair()
{
}


BMatrPair::~BMatrPair()
{
}

const BMatrPair& BMatrPair::operator = (const BMatr &Matr)
{
	MOrig = Matr;
	MInvr = MOrig.invr();
	return *this;
}

const BMatrPair& BMatrPair::operator=(const BMatrPair& Matr)
{
	MOrig = Matr.Or();
	MInvr = Matr.Inv();
	return *this;
}
