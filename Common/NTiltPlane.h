#pragma once
#include "SCoordsDef.h"
#include "SMachStateFix.h"

enum RCoordPair
{
	RCP_AB,
	RCP_AC,
	RCP_BC
};

class AFX_EXT_CLASS NTiltPlane
{
	friend class MTMachineTool;
public:
	NTiltPlane(void);
	~NTiltPlane(void);
	void Reset(void);
	bool GetResAngles(const class SCoordsDef& FixedDef, double *pA, double *pB, double *pC) const;
	bool IsActive() const { return Active;}
	bool CalculateParams(class NMachUnitPair* pMUPair, const class BMatr &NewSystem, const CString &AName1, const CString &AName2, int Tur = 0, int Att = 0, int Decision = 1);
	static int Calc2AnglesPairs(const BPoint &V1, const BPoint &V2, const BPoint &T1, const BPoint &T2, const BPoint& X1, const BPoint& X2, double Angles[4]);
	const BMatr &GetCSMatr(void) const { return CSMatr;}
	const BMatr &GetNewSys(void) const { return NewSys;}
	const BMatr &GetStockMatr(void) const { return StockMatr;}
	void SetActive(bool Val) { Active = Val;} 
	void SetChanging(bool Val) { Changing = Val;} 
	bool IsChanging(void) const { return Changing;}
	int GetStartRegNum() const { return StartRegNum; }
	void SetStartRegNum(int Val) { StartRegNum = Val; }
 	const SMachState &GetFixState(void) const { return FixState;}
	SMachState FixState;
	bool Cancelling;
protected:
	bool Active;
	bool Changing;
	BMatr NewSys;
	int Turret;
	int Attach;
	BMatr CSMatr;
	BMatr StockMatr;
	SCoordsDef MTDef;
	SMachState MTState;
	int Decision; // 0 - nearest to the current state, 1 - first, 2 - second, 4 - not nearest
	// 5 - main angle > 0; 6 - main angle < 0 (main angle - angle for the rotation axis nearest to the tool axis)
	int StartRegNum;
};
