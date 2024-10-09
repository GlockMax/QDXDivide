// NUVPoint.cpp: implementation of the NUVPoint class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SurfaceConst.h"

double SurfaceConst::MINDs = 1.e-5;
double SurfaceConst::MIN_SARCs = 0.05;
double SurfaceConst::ER2Ms = 1.e-8;
double SurfaceConst::F_D_VERTs = 1.e-4;
double SurfaceConst::F_Z_HORs = 4.e-3;
double SurfaceConst::DMINs = 1.e-6;
double SurfaceConst::LMINs = 1.e-6;
double SurfaceConst::MINSs = 1.e-6;
double SurfaceConst::MINSTs = 1.e-6;
double SurfaceConst::EPS_DPs = 1.e-12;
double SurfaceConst::EQPs = 1.e-9;
double SurfaceConst::LIMTOLs = 0.05;
double SurfaceConst::FARCs = 1.e-3;
double SurfaceConst::ARCEPSs = 1.e-5;


SurfaceConst::SurfaceConst(void)
{
}

SurfaceConst::~SurfaceConst(void)
{
}

void SurfaceConst::Scale(double Mas)
{
	MINDs *= Mas;
	MIN_SARCs *= Mas;
	ER2Ms *= Mas;
	F_D_VERTs *= Mas;
	F_Z_HORs *= Mas;
	DMINs *= Mas;
	LMINs *= Mas;
	MINSs *= Mas;
	MINSTs *= Mas;
	EPS_DPs *= Mas;
	EQPs *= Mas;
	LIMTOLs *= Mas;
	FARCs *= Mas;
	ARCEPSs *= Mas;
}
