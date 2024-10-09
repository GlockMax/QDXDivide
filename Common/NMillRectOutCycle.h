#pragma once
#include "BPoint.h"
#include "BGeomArray.h"

class AFX_EXT_CLASS NMillRectOutCycle
{
public:
	NMillRectOutCycle(BGeomArray &Cont
		, const	BPoint &StartPointI
		, const	BPoint &CenterI
		, const	BPoint &EndPointI 
		, const	BPoint &EXI // единичный вектор X стороны
		, const BPoint &EYI // единичный вектор Y стороны
		, double DXroughI
		, double DYroughI
		, double DXfinishI
		, double DYfinishI
		, bool CCWDirI
		, bool NeedRoughI
		, bool NeedFinishI
		, double XYdistI
		, double ZdistRoI
		, double ZdistFiI
		, double DZdownRoI
		, double DZdownFiI
		, double CornerRI
		, double XYclrI
		, double DZupI);
	~NMillRectOutCycle(void);
	bool IsOK();
	int MakePath(void);
	int MakeLayerRough(BPoint &StartPoint, const BPoint &EndPoint, int Npas);
	int MakeOnePassRough(BPoint &CurPoint, const BPoint & XDir, const BPoint & YDir, double CornerR);
	int MakeLayerFinSide(BPoint &CurPoint);
protected:
	bool bOK;
	curve ArcType;
protected:
	BPoint StartPoint;
	BPoint Center;
	BPoint EndPoint; 
	BPoint EX; // единичный вектор X стороны
	BPoint EY; // единичный вектор X стороны
	double DXrough;
	double DYrough;
	double DXfinish;
	double DYfinish;
	bool CCWDir; 
	bool NeedRough;
	bool NeedFinish;
	double XYdist;
	double ZdistRo;
	double ZdistFi;
	double DZdownRo;
	double DZdownFi;
	double CornerR;
	double XYclr;
	double DZup;

	BGeomArray *pCont;

public:
	const BPoint & GetStartPoint(void);
};
