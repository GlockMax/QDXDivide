#include "StdAfx.h"
#include "NCMbConst.h"

double NCMbConst::MINDs = 1.e-5;
double NCMbConst::MIN_SARCs = 0.05;
double NCMbConst::MAX_DFLs = 2.e-4;
double NCMbConst::F_D_VERTs = 1.e-4;
double NCMbConst::F_Z_HORs = 4.e-3;
double NCMbConst::DMINs = 1.e-6;
double NCMbConst::LMINs = 1.e-6;
double NCMbConst::FARCs = 1.e-3;
double NCMbConst::ARCEPSs = 1.e-5;

NCMbConst::NCMbConst(void)
{
}

NCMbConst::~NCMbConst(void)
{
}

void NCMbConst::Scale(double Mas)
{
	MINDs *= Mas;
	MIN_SARCs *= Mas;
	MAX_DFLs *= Mas;
	F_D_VERTs *= Mas;
	F_Z_HORs *= Mas;
	DMINs *= Mas;
	LMINs *= Mas;
	FARCs *= Mas;
	ARCEPSs *= Mas;
}
