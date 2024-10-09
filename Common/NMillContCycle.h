#pragma once
#include "BPoint.h"
#include "BGeomArray.h"

class AFX_EXT_CLASS NMillContCycle
{
public:
	NMillContCycle(BGeomArray &Cont, BGeomArray &CompCont
		, BGeomArray &ContProg, BGeomArray &ContComp, BGeomArray &ContRough
		, const	BPoint &StartPointI
		, const	BPoint &EndPointI 
		, bool NeedRoughI
		, bool NeedFinishI
		, double CompI
		, double ZdistRoI
		, double ZdistFiI
		, double DZdownRoI
		, double DZdownFiI
		, double DistI
		, enum LR DirCompI
		, bool OneDirCutI);
	~NMillContCycle(void);
	bool IsOK();
	int MakePath(void);
	int MakeLayerRough(BPoint &StartPoint, double DownStep, double UpStep, bool Forward);
protected:
	bool MakeCompSubCont(double Dist);
protected:
	bool bOK;
	BGeomArray SubCont;
	BGeomArray CompSubCont;
protected:
	BPoint StartPoint;
	BPoint EndPoint;
	bool NeedRough;
	bool NeedFinish;
	double Comp;
	double ZdistRo;
	double ZdistFi;
	double DZdownRo;
	double DZdownFi;
	double Dist;
	enum LR DirComp; 
	bool OneDirCut;

	BGeomArray *pCont;
	BGeomArray *pCompCont;
	BGeomArray *pContProg;
	BGeomArray *pContComp;
	BGeomArray *pContRough;

};
