#pragma once
#include <math.h>
#include "nsortedarray.h"

class AFX_EXT_CLASS BAnglesArray :
	public NSortedArray
{
public:
	BAnglesArray(void);
	virtual ~BAnglesArray(void);
	static int Compare(double a0, double a1);
	static bool IsBetween(double t, double a0, double a1);
	static double GetDelta(double a0, double a1);
	static double GetMidAngle(double a0, double a1);
	bool FindBetween(double a0, double a1);
};
