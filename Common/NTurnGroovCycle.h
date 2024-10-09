#pragma once
#include "BGeomArray.h"
#include "NTurnCycleBas.h"

class AFX_EXT_CLASS NTurnGroovCycle : public NTurnCycleBas
{
public:
	NTurnGroovCycle(BGeomArray &Cont, double XStep, double ZStep, bool Xfeed, double AddTime);
	~NTurnGroovCycle(void);
protected:
	BGeomArray *pCont;
	double Step;
	double OrtStep;
	double StepAddTime;
	int MakePath(void);
	bool orient;
public:
	bool MakeOneStep(BGeomArray & OneStep, const BPoint & StartPoint, double StepX);
	void OrientCont(void);
	void RestoreCont(void);
};
