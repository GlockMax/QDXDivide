#pragma once
#include "NTurnCycleBas.h"

class AFX_EXT_CLASS NMillCycle : public NTurnCycleBas
{
public:
	NMillCycle(void);
	virtual ~NMillCycle(void);
protected:
	bool ReturnToStartPlane;
	static double PreviousDepth;
	static double StartZ;
	static double Depth;
};

