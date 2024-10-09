#include "StdAfx.h"
#include "NCMConst.h"

double NCMConst::MINDs = 1.e-5;
double NCMConst::MIN_TOLERANCEs = 1.e-3;
double NCMConst::MINDPARs = 1.e-5;
double NCMConst::MIN_ARCs = 1.e-3;
double NCMConst::DMUOs = 1.e-2;
double NCMConst::DMUs = 1.e-4;
double NCMConst::TMUs = 1.e-6;

NCMConst::NCMConst(void)
{
}

NCMConst::~NCMConst(void)
{
}

void NCMConst::Scale(double Mas)
{
	MINDs *= Mas;
	MIN_TOLERANCEs *= Mas;
	MINDPARs *= Mas;
	MIN_ARCs *= Mas;
	DMUs *= Mas;
	DMUOs *= Mas;
	TMUs *= Mas;
}
