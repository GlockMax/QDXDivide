#pragma once
#include "BGeomArray.h"
#include "NTurnCycleBas.h"

class AFX_EXT_CLASS NTurnCutCycle : public NTurnCycleBas
{
public:
	NTurnCutCycle(BGeomArray &Cont, bool Xfeed);
	~NTurnCutCycle(void);
protected:
	BGeomArray *pCont;
	double R2;
	int MakePath(void);
	bool orient;
public:
	static double stat_dX;
	static double stat_R;
	static double stat_dZ;

public:
	bool MakeOneStep(BGeomArray & OneStep, const BPoint & StartPoint);
	void OrientCont(void);
	void RestoreCont(void);
};
