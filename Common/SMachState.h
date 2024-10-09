#pragma once

#include "NCMB_API.h"
#include <vector>
#include "MTCoordinate.h"

class NCMB_API SMachState
{
public:
	SMachState(const SMachState &In);
	SMachState(const class SCoordsDef *pDef);
	SMachState(const class BPoint& XYZ, const class BPoint& ABC, const class SCoordsDef* pDef);
	SMachState(const class SMachStateFix1& FixState, const class SCoordsDef* pDef);
	~SMachState(void);
	const SMachState & operator = (const SMachState &In);
	double GetCoord(int i) const { return Coords[i]; };
	void SetCoord(int i, double Val);
	void AddXYZ(const BPoint& P);
	double GetCoord(const CString &Name) const;
	void SetCoord(const CString &Name, double Val);
	const CString &GetAName(int i) const;
	bool IsVector(void) const;
	bool IsZero(void) const;
	bool GetCooArray(CooArray& Array) const;
	static bool Merge2CooArray(CooArray& Array, const SMachState& State);
	SMachState operator + (const SMachState &P) const;
	SMachState operator - (const SMachState &P) const;
	SMachState operator * (double a) const;
	friend SMachState operator * (const double a, const SMachState &P);
	int GetSize(void) const;
	void ClearValues(void);
	int GetInd(const CString &Name) const;
	void ReplaceCoord(const SMachState &In);
	const CString MakeCadr(void);
	const SCoordsDef* GetDef() const { return pCoordsDef; }
protected:
	std::vector<double> Coords;
	const class SCoordsDef *pCoordsDef;
};
