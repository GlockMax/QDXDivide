#pragma once
#include "StdAfx.h"
#include "NCUnitState.h"
class AFX_EXT_CLASS NCStatesHist
{
	friend class NCUnit;
public:
	NCStatesHist();
	~NCStatesHist(void);
	void Clear(void);
	void AddState(const NCUnitState &St, const NCUnitState &AddSt, const NCUnitStatePrevGeom &GeomSt);
	void SendState(const NCUnitState &St, const NCUnitState &AddSt, const NCUnitStatePrevGeom &GeomSt);
	void Init(int EstimatedProgSize);
	void Close(void);
	void Open(void);
	void SetAddCoords(const SMachState& MachState);
	void GetStatePair(int Ind, const NCUnitState *&pSt, const NCUnitState *&pAddSt, const NCUnitStatePrevGeom *&pGeomSt) const;
	int FindMaxLowerInd(cadrID SearchArg, bool AbsHCadr = false) const;
	int GetSize() const noexcept { return Size; }
protected:
	static const int MAX_STATES_NUM = 50; // Must be even and grater than 2
	int Size;
	NCUnitState *States[MAX_STATES_NUM];
	NCUnitState *AddStates[MAX_STATES_NUM];
	NCUnitStatePrevGeom *PrevGeomStates[MAX_STATES_NUM];
	int StoreStep;
	int StoreInd;
	bool Closed;
protected:
	void Swap(int i, int j);
	void Halve(void);
	bool IsGrater(int Ind, cadrID SearchArg, bool AbsHCadr) const;

};

