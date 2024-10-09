#pragma once
#include "BGeomArray.h"
#include "NCPar.h"
#include "NTurnCycleBas.h"

class AFX_EXT_CLASS NTurnThreadCutCycle : public NTurnCycleBas
{
public:
	NTurnThreadCutCycle(BGeomArray &Cont, const NCPar &CPar);
	~NTurnThreadCutCycle(void);
protected:
	const NCPar &Par;
	BGeomArray *pCont;
	double Step;
	double Retr;
	int MakePath(void);
	double CurDepth;
	BPoint StartPoint;
	BPoint EndPoint;
	double ToolAngle;
public:
	bool MakeOneStep(BGeomArray & OneStep, const BPoint & StartPoint, double StepX);
	void OrientCont(void);
	void RestoreCont(void);
};
