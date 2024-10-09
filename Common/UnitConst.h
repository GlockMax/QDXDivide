#pragma once

#include "BConst.h"

class UnitConst : public BConst
{
public:
	UnitConst(void);
	~UnitConst(void);
	static void Scale(double Mas);
public:
	static double MINDs;
	static double MIN_ARCs;
	static double DMUs;
	static double DMUOs;
	static double TMUs;
};
