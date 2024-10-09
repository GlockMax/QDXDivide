#include "StdAfx.h"
#include "UnitConst.h"

double UnitConst::MINDs = 1.e-5;
double UnitConst::MIN_ARCs = 1.e-3;
double UnitConst::DMUOs = 1.e-2;
double UnitConst::DMUs = 1.e-4;
double UnitConst::TMUs = 1.e-6;

UnitConst::UnitConst(void)
{
}

UnitConst::~UnitConst(void)
{
}

void UnitConst::Scale(double Mas)
{
	MINDs *= Mas;
	MIN_ARCs *= Mas;
	DMUs *= Mas;
	DMUOs *= Mas;
	TMUs *= Mas;
}
