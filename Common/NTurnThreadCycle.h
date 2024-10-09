#pragma once
#include "BGeomArray.h"
#include "NTurnCycleBas.h"

class AFX_EXT_CLASS NTurnThreadCycle : public NTurnCycleBas
{
public:
	NTurnThreadCycle(BGeomArray &Cont, double Feed);
	~NTurnThreadCycle(void);
protected:
	BGeomArray *pCont;
	double FeedPerRev;
	int MakePath(void);

public:
	bool MakeOneStep(BGeomArray & OneStep, const BPoint & StartPoint);
	static double stat_dZ;
};
