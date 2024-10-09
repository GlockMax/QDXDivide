#pragma once
#include "NCMB_API.h"

class NCMB_API NSurfSettings
{
public:
	NSurfSettings(void);
	~NSurfSettings(void);
	static int GetNumAproxLine(double R, double alf);
	static double GetTolerance();
	static void SetTolerance(double val);
protected:
	static double Tolerance;
};
