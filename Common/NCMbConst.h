#pragma once

#include "BConst.h"

class NCMB_API NCMbConst : public BConst
{
public:
	NCMbConst(void);
	~NCMbConst(void);
	static void Scale(double Mas);
public:
	static double MINDs;
	static double MIN_SARCs;
	static double MAX_DFLs;
	static double F_D_VERTs;
	static double F_Z_HORs;
	static double DMINs;
	static double LMINs;
	static double FARCs;
	static double ARCEPSs;
};
