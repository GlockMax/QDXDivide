#pragma once

class AFX_EXT_CLASS NWeldCadr
{
public:
	NWeldCadr(void);
	~NWeldCadr(void);
	bool GetRootDist(double t, double *pDist, double *pT1);
public:
	double RootDist0;
	double RootDist1;
	double RootT1_0;
	double RootT1_1;
	static double RootR;
	bool ConsiderF;
};
