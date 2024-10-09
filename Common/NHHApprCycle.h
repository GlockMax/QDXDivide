#pragma once
#include "BPoint.h"
#include "NCEnums.h"
#include "BGeomArray.h"

class AFX_EXT_CLASS NHHApprCycle
{
public:
	NHHApprCycle(BGeomArray &Cont
		, BGeomArray &CompCont
		, double LenI
		, double CcaI
		, double RI
		, double CompValI
		, bool LeftCompDirI
		, const NCadrGeom &NextGeomI
		, CycleType CTypeI);
	~NHHApprCycle(void);
	bool IsOK();
	int GetError();
protected:
	bool bOK;
	int Error;
protected:
	double Len;
	double Cca;
	double R;
	double CompVal;
	bool LeftCompDir;
	const NCadrGeom &NextGeom;
	CycleType CType;
	BGeomArray *pCont;
	BGeomArray *pCompCont;
protected:
	int MakePath(void);
	void Make1APPRLT(BGeomArray *pCont, const BPoint &Dir, const BPoint &Norm, double Comp);
	void Make1APPRLN(BGeomArray *pCont, const BPoint &Dir, const BPoint &Norm, double Comp);
	void Make1APPRCT(BGeomArray *pCont, const BPoint &Dir, const BPoint &Norm, double Comp);
	void Make1APPRLCT(BGeomArray *pCont, const BPoint &Dir, const BPoint &Norm, double Comp);


};
