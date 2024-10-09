#include "StdAfx.h"
#include "NCToolConst.h"

double NCToolConst::MINDs = 1.e-5;
double NCToolConst::MIN_SARCs = 0.05;
double NCToolConst::MAX_DFLs = 2.e-4;
double NCToolConst::F_D_VERTs = 1.e-4;
double NCToolConst::F_Z_HORs = 1.e-4;
double NCToolConst::DMINs = 1.e-6;
double NCToolConst::LMINs = 1.e-6;
double NCToolConst::MINSs = 1.e-6;
double NCToolConst::MINSTs = 1.e-6;
double NCToolConst::EPS_DPs = 1.e-12;
double NCToolConst::EQPs = 1.e-9;
double NCToolConst::TOOL_OFFSETs = 1.e-4;
double NCToolConst::ST_DECs = 0.0005353;
double NCToolConst::ST_INCs = 0.0008171;
double NCToolConst::MRDs = 0.001;
double NCToolConst::MINTOOLDIMs = 1.e-2;


NCToolConst::NCToolConst(void)
{
}

NCToolConst::~NCToolConst(void)
{
}

void NCToolConst::Scale(double Mas)
{
	MINDs *= Mas;
	MIN_SARCs *= Mas;
	MAX_DFLs *= Mas;
	F_D_VERTs *= Mas;
	F_Z_HORs *= Mas;
	DMINs *= Mas;
	LMINs *= Mas;
	MINSs *= Mas;
	MINSTs *= Mas;
	EPS_DPs *= Mas;
	EQPs *= Mas;
	TOOL_OFFSETs *= Mas;
	ST_DECs *= Mas;
	ST_INCs *= Mas;
	MRDs *= Mas;
	MINTOOLDIMs *= Mas;
}
