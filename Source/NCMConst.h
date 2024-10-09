#pragma once

#include "BConst.h"

class NCMConst : public BConst
{
public:
	NCMConst(void);
	~NCMConst(void);
	static void Scale(double Mas);
public:
	static double MINDs;
	static double MIN_TOLERANCEs;
	static double MINDPARs;
	static double MIN_ARCs;
	static double DMUs;
	static double DMUOs;
	static double TMUs;
};
