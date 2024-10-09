#pragma once
#include "NCMB_API.h"

#define MM_INCH BConst::GetMetrics()
#define MM_I_SC BConst::GetScale()

enum NMetrics
{
	M_MM,
	M_INCH
};

class NCMB_API BConst
{
public:
	BConst(void);
	~BConst(void);
public:
	static NMetrics State;
	static NMetrics GetMetrics(void);
	static double GetScale();
};
