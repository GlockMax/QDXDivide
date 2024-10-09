#pragma once
#include "RPickID.h"
#include "OctreeProg.h"
#include "OctreeSkel.h"
#include "OctreeSkelTool.h"

class RayCastManager
{
private:
	OctreeProg octreeProg;
	OctreeSkel octreeSkel;
public:
	RayCastManager();
	~RayCastManager();

	void InitializeProg(class CNCMDoc *pDoc, int StPos);
	RPickID PickRCM(const BPoint& rayPoint, const BPoint& rayVector, double Epsilon, double MaxL2, CNCMDoc *pDoc, const bool& StopPick);
private:
protected:
	int StPos;
};

class RayCastManPack
{
public:
	void Initialize(class CNCMDoc* pDoc);
	void InitializeTools(class CNCMDoc* pDoc, bool StockView);
	RPickID PickRCM(const BPoint& rayPoint, const BPoint& rayVector, double Epsilon, double MaxL2, CNCMDoc* pDoc);
	RPickID PickRCMSync(const BPoint& rayPoint, const BPoint& rayVector, double Epsilon, double MaxL2, CNCMDoc* pDoc);
	void SetStopPick() { StopPick = true; }
	void ResetStopPick() { StopPick = false; }
protected:
	std::vector<RayCastManager> Mans;
	bool StopPick;
	OctreeSkelTool octreeSkelTool;
};

