#pragma once

#include "BConst.h"

class AFX_EXT_CLASS NCToolConst : public BConst
{
public:
	NCToolConst(void);
	~NCToolConst(void);
	static void Scale(double Mas);
public:
	static double MINDs;
	static double MIN_SARCs;
	static double MAX_DFLs;
	static double F_D_VERTs;
	static double F_Z_HORs;
	static double DMINs;
	static double LMINs;
	static double MINSs;
	static double MINSTs;
	static double EPS_DPs;
	static double EQPs;
	static double TOOL_OFFSETs;
	static double ST_DECs;
	static double ST_INCs;
	static double MRDs;
	static double MINTOOLDIMs;
};
