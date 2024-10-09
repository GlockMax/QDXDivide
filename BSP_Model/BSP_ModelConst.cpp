#include "StdAfx.h"
#include "BSP_ModelConst.h"

double BSP_ModelConst::MINDs = 1.e-5;
double BSP_ModelConst::MINDPs = 1.e-4;// Min plane distance for MInsertFace
double BSP_ModelConst::MIN_SARCs = 0.05;
double BSP_ModelConst::MAX_DFLs = 2.e-4;
double BSP_ModelConst::F_D_VERTs = 1.e-4;
double BSP_ModelConst::F_Z_HORs = 1.e-4;
double BSP_ModelConst::DMINs = 1.e-6;
double BSP_ModelConst::LMINs = 1.e-6;
double BSP_ModelConst::MINSs = 1.e-6;
double BSP_ModelConst::MINSTs = 1.e-6;
double BSP_ModelConst::EPS_DPs = 1.e-12;
double BSP_ModelConst::EQPs = 1.e-9;


BSP_ModelConst::BSP_ModelConst(void)
{
}

BSP_ModelConst::~BSP_ModelConst(void)
{
}

void BSP_ModelConst::Scale(double Mas)
{
	MINDs *= Mas;
	MINDPs *= Mas;
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
}
