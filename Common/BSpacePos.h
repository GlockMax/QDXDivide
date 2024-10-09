#pragma once
#include "BMatr.h"
#include "BCurve.h"
#include "BIPolar.h"
#include "BICyl.h"

class AFX_EXT_CLASS BSpacePos
{
public:
	static int AddRotMatr(const BMatr & M, const BPoint & ABC);
	static int AddPolar(const BIPolar & Interp);
	static int AddCyl(const BICyl & Interp);
	static const BIPolar &GetPolar(int i);
	static const BICyl &GetCyl(int i);
	static const BMatr &GetRotMatr(int i);
	static const BMatr *GetRotMatrs(void);
	static const BPoint GetABC(int i);
	static void ClearRotMatrs(void);
	static int GetLastMatrInd(void);
	static int GetLastPolarInd(void);
	static int GetLastCylInd(void);
	static INT_PTR GetMatrsSize(void);
protected:
	static CArray< BMatr, const BMatr &> RotMatrs;
	static CArray< BPoint, const BPoint &> ABCs;
	static CArray< BIPolar, const BIPolar &> Polars;
	static CArray< BICyl, const BICyl &> Cyls;
public:
	static bool LastMatrUsed;
	static int LastTurret;
	static BMatr LastCSMatr;
	static BPoint LastABC;
	static class MTMachineTool *pLastMT;
public:
	static void Clear(void);
};
