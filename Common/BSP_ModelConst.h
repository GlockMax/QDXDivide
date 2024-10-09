#pragma once
#include "BSP_MODEL_API.h"
#include "BConst.h"

class BSP_MODEL_API BSP_ModelConst : public BConst
{
public:
	BSP_ModelConst(void);
	~BSP_ModelConst(void);
	static void Scale(double Mas);
public:
	static double MINDs;
	static double MINDPs;
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
};
