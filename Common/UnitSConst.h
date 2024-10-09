#pragma once

#include "BConst.h"

class AFX_EXT_CLASS UnitSConst : public BConst
{
public:
	UnitSConst(void);
	~UnitSConst(void);
	static void Scale(double Mas);
public:
	static double MINDs;
	static double MIN_SARCs;
	static double MAX_DFLs;
	static double F_D_VERTs;
	static double F_Z_HORs;
	static double DMINs;
	static double LMINs;
	static double LNSs;
	static double RMUs;
	static double DMUs;
};
