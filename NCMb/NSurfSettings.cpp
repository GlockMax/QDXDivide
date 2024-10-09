#include "stdafx.h"
#include "ConstDef.h"
#include "nsurfsettings.h"
#include "math.h"

double NSurfSettings::Tolerance = 0.1;

NSurfSettings::NSurfSettings(void)
{
}

NSurfSettings::~NSurfSettings(void)
{
}
double NSurfSettings::GetTolerance()
{
	return Tolerance;
}
void NSurfSettings::SetTolerance(double val)
{
	Tolerance = val;
}

int NSurfSettings::GetNumAproxLine(double R, double alf)
{
	alf = fabs(alf) * PI / 180.;// EK
	double b = 2. * acos(1. - Tolerance / R);
	if( b >= alf )
		return 1;
	return (int(alf * 0.5 / b) + 1) * 2;
}
