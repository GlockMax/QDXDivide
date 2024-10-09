#pragma once
#include "NTurnCycleBas.h"
class AFX_EXT_CLASS NThreadingTapCycle :
	public NTurnCycleBas
{
public:
	NThreadingTapCycle(BGeomArray &Cont, double Feed, bool FirstCycleCadr, bool RetToStart, double Ref, double ReverseTime, const BMatr &M0, const BMatr &M1);
	virtual ~NThreadingTapCycle(void);
protected:
	BGeomArray *pCont;
	double Feed;
	double RefZ;
	bool ReturnToStartPlane;
	double ReverseTime;
	static double PreviousDepth;
	static double StartZ;
	int MakePath(void);
};

