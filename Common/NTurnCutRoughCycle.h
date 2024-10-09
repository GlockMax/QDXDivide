#pragma once
#include "BGeomArray.h"
#include "NTurnCycleBas.h"

class AFX_EXT_CLASS NTurnCutRoughCycle : public NTurnCycleBas
{
public:
	NTurnCutRoughCycle(BGeomArray &Cont, double XStep, double Rz, bool Xfeed);
	~NTurnCutRoughCycle(void);
protected:
	BGeomArray *pCont;
	double Step;
	double R;
	int MakePath(void);
	bool orient;
public:
	bool MakeOneStep(BGeomArray & OneStep, const BPoint & StartPoint, double StepX);
	void OrientCont(void);
	void RestoreCont(void);
};
