#pragma once
#include <list>
#include "NCollManager.h"

class NCollManagerTouch :
	public NCollManager
{
public:
	NCollManagerTouch(CNCMDoc* pD, const std::list<int> &MovingAx, GBodyRender* pBodyRender);
	virtual ~NCollManagerTouch();
	bool MakeSensitivePairs();
protected:
	int MakeCollisionScene(void) override;
public:
	bool CheckLocCurPosition(class NMachUnitPair* pMUPair, const class NToolCombined* pActiveTool) override;
	void Prepare(bool Fore) override;
	virtual PObjectStatus OnStatusChanged(int ObjectInd, PObjectStatus NewStatus) override;
protected:
	const std::list<int>& MovingAxInds;
	GBodyRender* pBodyRender;
};

