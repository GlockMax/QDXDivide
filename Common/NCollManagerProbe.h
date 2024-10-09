#pragma once
#include <set>
#include "NCollManager.h"

class NCollManagerProbe :
	public NCollManager
{
public:
	NCollManagerProbe(CNCMDoc* pD, GBodyRender* pRender, NToolCombined* pActiveTool);
	virtual ~NCollManagerProbe();
protected:
	int MakeCollisionScene(void) override;
public:
	bool CheckLocCurPosition(class NMachUnitPair* pMUPair, const class NToolCombined *pActiveTool) override;
	int MakeStocksProbe(const NToolCombined * pActiveTool, GBodyRender* pBodyRender);
	void Prepare(bool Fore) override;
	virtual PObjectStatus OnStatusChanged(int ObjectInd, PObjectStatus NewStatus) override;
	bool Add2CollisionScene();
protected:
	std::set<void *> UsedStockCells;
	GBodyRender* pBodyRender;
	NToolCombined* pActTool;
};

