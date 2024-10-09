#include "StdAfx.h"
#include "CompareConst.h"

double CompareConst::MINDs = 1.e-5;


CompareConst::CompareConst(void)
{
}

CompareConst::~CompareConst(void)
{
}

void CompareConst::Scale(double Mas)
{
	MINDs *= Mas;
}
