#pragma once
#include "NStateParamsPane.h"
class NDynParams
{
public:
	double DynX;
	double DynY;
	double DynZ;
	double DynA;
	double DynB;
	double DynC;
	int ToolPosition;
	double Time;
public:
	void SetXYZ(const BPoint& Val) { DynX = Val.GetX(); DynY = Val.GetY(); DynZ = Val.GetZ(); }
	void SetABC(const BPoint& Val) { DynA = Val.GetX(); DynB = Val.GetY(); DynC = Val.GetZ(); }
};
class NStateUnitPane :
	public NStateParamsPane
{
public:
	NStateUnitPane(void);
	virtual ~NStateUnitPane(void);
	void OnUpdate(CDockablePane* pSender, LPARAM lHint, CObject* pHint);
	void AnimateUpdateControl(const NDynParams& Params);
protected:
	void CreateStateView() override;
	void ResetElements(void) override;
	void DeleteElements() override;
protected:
	NDynParams DynParams;
	class NCUnitState * pUnitState;
	std::vector<std::pair<CString, class NStateElem*>> DynElemList;
	std::vector<int> DynElemInd;
};
