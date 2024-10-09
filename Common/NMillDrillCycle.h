#pragma once
#include "NMillCycle.h"
class AFX_EXT_CLASS NMillDrillCycle :
	public NMillCycle
{
public:
	NMillDrillCycle(BGeomArray &Cont, double Feed, bool First, bool RetToStart, double Ref, double PeckIDepth, bool RefAbs
		, const BMatr &M0, const BMatr &M1, double AddTime, double iDepth);
	virtual ~NMillDrillCycle(void);
protected:
	BGeomArray *pCont;
	double Feed;
	double PeckIncrementDepth;
	double RefZ;
	double StepAddTime;
	int MakePath(const BPoint &StartP);
};

