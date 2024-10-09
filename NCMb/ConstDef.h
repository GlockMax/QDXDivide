#pragma once 

#include "NCMbConst.h"

#define MIND NCMbConst::MINDs
#define MIN_SARC NCMbConst::MIN_SARCs
#define MAX_DFL NCMbConst::MAX_DFLs
#define F_D_VERT NCMbConst::F_D_VERTs
#define F_Z_HOR NCMbConst::F_Z_HORs
#define DMIN NCMbConst::DMINs
#define LMIN NCMbConst::LMINs
#define FARC NCMbConst::FARCs
#define ARCEPS NCMbConst::ARCEPSs


const double PI = 3.14159265358979;
const double MINAR = 1.e-5;// In radians
const double MINAD = MINAR * 57.;// In degrees
const double MINATD = 5.e-2;// In degrees
const double HMIN = 1.e-7;
const double PS=0.0031;/* точность для сравнения углов          */
const double MINZ = 1.e-8;