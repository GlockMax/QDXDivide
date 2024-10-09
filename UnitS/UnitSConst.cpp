#include "StdAfx.h"
#include "UnitSConst.h"

double UnitSConst::MINDs = 1.e-5;
double UnitSConst::LMINs = 1.e-6;
double UnitSConst::DMUs = 1.e-4;
double UnitSConst::LNSs = 1.e-7;
double UnitSConst::RMUs = 0.031622776601683793319988935444327;

UnitSConst::UnitSConst(void)
{
}

UnitSConst::~UnitSConst(void)
{
}

void UnitSConst::Scale(double Mas)
{
	MINDs *= Mas;
	LMINs *= Mas;
	DMUs *= Mas;
	LNSs *= Mas;
	RMUs *= Mas;
}
