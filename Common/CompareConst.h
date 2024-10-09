#pragma once
#include "COMPARE_API.h"
#include "BConst.h"

class COMPARE_API CompareConst : public BConst
{
public:
	CompareConst(void);
	~CompareConst(void);
	static void Scale(double Mas);
public:
	static double MINDs;
};
