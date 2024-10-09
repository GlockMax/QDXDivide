#pragma once
#include "BPoint.h"
#include "BGeomArray.h"

class AFX_EXT_CLASS NMillCircInCycle
{
public:
	NMillCircInCycle(BGeomArray &Cont
		, const	BPoint &StartPoint
		, const	BPoint &EndPoint 
		, const	BPoint &Center
		, bool CCWDir 
		, bool NeedAppr
		, bool NeedRough
		, bool NeedFinish
		, double DAppr
		, double DRough
		, double DFinish
		, double XYdist
		, double ZdistRo
		, double ZdistFi
		, double DZdownRo
		, double DZdownFi
		, double DZup);
	~NMillCircInCycle(void);
	bool IsOK();
	int MakePath(void);
	int MakeLayerAppr(BPoint &CurPoint, double DownStep);
	int MakeLayerRough(BPoint &CurPoint, double UpStep);
	int MakeLayerFinSide(BPoint &CurPoint, double DownStep, double Angle);
protected:
	bool bOK;
	curve ArcType;
protected:
	BPoint StartPoint;
	BPoint EndPoint;
	BPoint Center;
	bool CCWDir; 
	bool NeedAppr;
	bool NeedRough;
	bool NeedFinish;
	double DAppr;// ¬се диаметры заданы на центр инструмента
	double DRough;
	double DFinish;
	double XYdist;
	double ZdistRo;
	double ZdistFi;
	double DZdownRo;
	double DZdownFi;
	double DZup;

	BGeomArray *pCont;

};
