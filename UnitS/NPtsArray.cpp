#include "stdafx.h"
#include "math.h"
#include "nptsarray.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

NPtsArray::NPtsArray(void)
{
	Type = PA_EMPTY;
	IStartP = 0;
}

NPtsArray::~NPtsArray(void)
{
}

int NPtsArray::GetNextPoint(double *x, double *y)
{
	switch(Type)
	{
	case PA_EMPTY:
		*x = 0.;
		*y = 0.;
		return 0;
	case PA_RECT:
		{
			++IPoint1;
			if(IPoint1 > N1)
			{
				IPoint1 = 1;
				++IPoint2;
				if(IPoint2 > N2)
					return 0;
			}
			double bx = Step1 * (IPoint1 - 1);
			double by = Step2 * (IPoint2 - 1);
			*x = bx * cos(Angle) - by * sin(Angle);
			*y = by * cos(Angle) + bx * sin(Angle);
			return 1;
		}
	}
	return 0;
}
void NPtsArray::Restart(void)
{
	IPoint1 = IStartP % N1;
	IPoint2 = 1;
}
