#pragma once
#include "StdAfx.h"
#include "NCollisionScene.h"
#include "NActiveObject.h"

class NCollManager : public NActiveObject
{
public:
	NCollManager(CNCMDoc* pD);
	virtual ~NCollManager(void);
	virtual void Prepare(bool Fore) = 0;
	bool IsEnabled(void);
	NCollisionScene * GetScene(void) const;
	double GetCheckStep(void) const noexcept { return CheckStep; }
	void SetCheckStep(double Step) noexcept { CheckStep = Step; }
	void Reset(const class MTMachineTool &MT) noexcept { pMainMach = &MT;}
	void SetGroupMask(int Mask) noexcept { GroupMask = Mask; }
	bool MaskHasStocks() const noexcept { return (GroupMask & OG_STOCKS) != 0; }
	void MakeTool(int AxisIndex, class NToolCombined *pTool, bool Active, bool ProbeOnly = false);
	void Serialize(CArchive& ar) override;
	int MakeStocks();
	int ReplaceStocks(class GBodyRender* pStart, class GBodyRender* pEnd);
	int MakeStock(const NCollisionObjectID& NID, const class GBodyRender* pStart, const class GBodyRender* pEnd, bool Replace);
	int CalcBehav(int AxisInd, bool Stock, bool ActTool);
	bool GetStopOnColl() const { return StopOnColl; }
	void SetStopOnColl(bool Val) { StopOnColl = Val; }
	bool GetIgnoreWrongAxis() const { return IgnoreWrongAxis; }
	void SetIgnoreWrongAxis(bool Val) { IgnoreWrongAxis = Val; }
	virtual bool CheckLocCurPosition(class NMachUnitPair* pMUPair, const NToolCombined* pActiveTool) { pMUPair;  pActiveTool; return false; }
	void RemFromMask(PObjectGroup OGr) { GroupMask &= ~OGr; }

public:
	static bool SetCurMatrices(class NCollisionScene *pCollisionScene, class MTMachineTool* pMach, const class NToolCombined *pActiveTool);
	static bool ChangeTool(class NCollisionScene *pCollisionScene, const class MTMachineTool &MT, const class NToolCombined *pTool);
	static bool ChangeActiveToolResp(class NCollisionScene *pCollisionScene, bool IsFast5X, bool Restore);

protected:
	virtual int MakeCollisionScene(void);
	int MakeToolsInPlacePos(void);
	int MakeToolsInActPos();
	void Clear(void);
protected:
	NCollisionScene *pCollisionScene;
	bool IgnoreMargins;
	double MarginMach;
	double MarginPart;
	double MarginFixture;
	double MarginTool;
	double MarginStock;
	double CheckStep;
	int GroupMask;
	const MTMachineTool *pMainMach;
	std::atomic<bool> StopOnColl;
	bool IgnoreWrongAxis;
	CNCMDoc* pGDoc;
};
