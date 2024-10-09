#pragma once 

#include "SurfaceConst.h"

#define MIND SurfaceConst::MINDs
#define MIN_SARC SurfaceConst::MIN_SARCs
#define ER2M SurfaceConst::ER2Ms
#define F_D_VERT SurfaceConst::F_D_VERTs
#define F_Z_HOR SurfaceConst::F_Z_HORs
#define DMIN SurfaceConst::DMINs
#define LMIN SurfaceConst::LMINs
#define MINS SurfaceConst::MINSs
#define MINST SurfaceConst::MINSTs
#define EPS_DP SurfaceConst::EPS_DPs
#define EQP SurfaceConst::EQPs
#define LIMTOL SurfaceConst::LIMTOLs
#define FARC SurfaceConst::FARCs
#define ARCEPS SurfaceConst::ARCEPSs

const double PI = 3.14159265358979;
const double MINSR = 1.e-4;// In radians
const double MINAR = 1.e-5;// In radians
const double MINAD = MINAR * 57.;// In degrees
const double HMIN = 1.e-6;
const double PS=0.0031;/* точность для сравнения углов          */
const double MAXC = 1.e6;
const double M1C = 1.e-12;
const double MCR = 1.e-6;
