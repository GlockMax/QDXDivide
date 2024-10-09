#pragma once
#include "NCTOOL_API.h"
#include "cadrID.h"
#include "NAnimState.h"
#include "GBodyRender.h"
#include "MTMashineTool.h"
#include "NMachUnitPair.h"

class NToolCombined;
class BProgram;

class NCTOOL_API GDynamicCadr
{
public:
	GDynamicCadr(GBodyRender *piStart);
	virtual ~GDynamicCadr(void);
	virtual double GetCurParam(void) = 0;
public:
	virtual bool SetA(const NAnimState &AState);
	virtual void Init() {};
	cadrID GetCadrID(void) const;
	cadrID GetCadrIDhist(void) const;
	void SetMT(class MTMachineTool* pMT, const class NCUnit* pU, class NCollTimeLine* pTL);
	const NToolCombined * GetLastTool(void) const;
	_int16 GetFlags() const { return Geom.GetType().flags;}
	void SetProg(const BProgram *pInProg) { pProg = pInProg;}
	bool FindGeom(void);
	int GetProgNum() const;
	const BPoint& GetXYZ(void) const { return XYZ; }
	const BPoint& GetABC(void) const { return ABC; }
	GBodyRender * GetActiveRender() const;
	NMachUnitPair GetMUPair() const { return MUPair; }
	GBodyRender* GetEnd(void) const;
	GBodyRender* GetStart(void) const;
	const NCadrGeom& GetGeom() const { return Geom; }
protected:
	virtual bool Set(const NCadrGeom & Geom, const BMatr &RotMatr, GBodyRender &End, double tPart) = 0;
	void SetMT2CurPos();
	virtual bool CalcCurCoordDC(BPoint &XYZ, BPoint &ABC) = 0;
	void SetGeom(const NCadrGeom &InGeom);
	virtual bool CalcCurCoordMCS(BPoint &XYZ, BPoint &ABC) = 0;
protected:
//	GDynamicCadr(void);
	virtual void CalcCurAddCoord(SMachState &AddCoord, double t) = 0;
private:
protected:
	const BProgram *pProg;
	NMachUnitPair MUPair;
	NCollTimeLine* pTimeLine;
	const NToolCombined *pLastTool;
	cadrID CadrID;
	cadrID CadrIDhist;
	NCadrGeom Geom;
	BMatr MCS; // this matrix corresponds to coord. system rotation and ignores machine tool rotation
	NCadrGeom TrGeom;
	GBodyRender *pStart;
	GBodyRender *pEnd;
	const SCoordsDef *pCoordsDef;
	BPoint XYZ, ABC;
};
