#pragma once

#include "BConst.h"

class AFX_EXT_CLASS SurfaceConst : public BConst
{
public:
	SurfaceConst(void);
	~SurfaceConst(void);
	static void Scale(double Mas);
public:
	static double MINDs;
	static double MIN_SARCs;
	static double ER2Ms;
	static double F_D_VERTs;
	static double F_Z_HORs;
	static double DMINs;
	static double LMINs;
	static double MINSs;
	static double MINSTs;
	static double EPS_DPs;
	static double EQPs;
	static double LIMTOLs;
	static double FARCs;
	static double ARCEPSs;
};
