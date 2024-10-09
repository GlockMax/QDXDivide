#pragma once
#include "WM_APP_messages.h"


class T3DData
{
public:
	T3DData(void);
	~T3DData(void);
	double Xr, Yr, Zr, Ar;
	double Xt, Yt, Zt, Lt;
	double Period;
};
