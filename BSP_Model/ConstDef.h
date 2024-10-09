#pragma once 

#include "BSP_ModelConst.h"

#define MIND BSP_ModelConst::MINDs
#define MINDP BSP_ModelConst::MINDPs
#define MIN_SARC BSP_ModelConst::MIN_SARCs
#define MAX_DFL BSP_ModelConst::MAX_DFLs
#define F_D_VERT BSP_ModelConst::F_D_VERTs
#define F_Z_HOR BSP_ModelConst::F_Z_HORs
#define DMIN BSP_ModelConst::DMINs
#define LMIN BSP_ModelConst::LMINs
#define MINS BSP_ModelConst::MINSs
#define MINST BSP_ModelConst::MINSTs
#define EPS_DP BSP_ModelConst::EPS_DPs
#define EQP BSP_ModelConst::EQPs

const double PI = 3.14159265358979;
const double MINSR = 1.e-4;// In radians
const double MINAR = 1.e-5;// In radians
const double MINAD = MINAR * 57.;// In degrees
const double HMIN = 1.e-6;
const double PS=0.0031;/* точность для сравнения углов          */
const double MAXC = 1.e6;
