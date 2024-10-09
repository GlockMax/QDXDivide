#pragma once
#include "BGeomArray.h"
#include "NTurnCycleBas.h"

class AFX_EXT_CLASS NTurnThreadACycle : public NTurnCycleBas
{
public:
	NTurnThreadACycle(BGeomArray &Cont, double ThreadQ1, double ThreadR1, double ThreadP1, double ThreadQ2, double ThreadR2, double ThreadP2, double Feed, double SyncroTime);
	~NTurnThreadACycle(void);
protected:
	BGeomArray *pCont;
	double R1;
	double P1;
	double Q1;
	double R2;// Cone parameter
	double P2;
	double Q2;
	double FeedPerRev;
	double SyncroTime;
	int MakePath(void);

public:
	bool MakeOneStep(BGeomArray & OneStep, const BPoint & StartPoint);
	static double stat_dZ;
	int CalcPassesNum(void);
};
