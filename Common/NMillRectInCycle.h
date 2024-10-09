#pragma once
#include "BPoint.h"
#include "BGeomArray.h"

class AFX_EXT_CLASS NMillRectInCycle
{
public:
	NMillRectInCycle(BGeomArray &Cont
		, const	BPoint &StartPoint1I
		, const	BPoint &StartPoint2I
		, const	BPoint &CenterI
		, const	BPoint &EndPointI 
		, const	BPoint &EXI // единичный вектор длинной стороныВектор рабочего хода
		, const BPoint &EYI // единичный вектор короткой стороны
		, double DXroughI
		, double DYroughI
		, double DXfinishI
		, double DYfinishI
		, bool CCWDirI
		, bool NeedApprI
		, bool NeedRoughI
		, bool NeedFinishI
		, double XYdistI
		, double ZdistRoI
		, double ZdistFiI
		, double DZdownRoI
		, double DZdownFiI
		, double CornerRI
		, double ApprAngleI
		, int ApprTypeI // 1 - по слоям, 2 - под заданным углом, 3 - по квадрату, 4 - вертикально
		, double DZupI);
	~NMillRectInCycle(void);
	bool IsOK();
	int MakePath(void);
	int MakeLayerAppr(BPoint &CurPoint, double DownStep);
	int MakeLayerRough(BPoint &StartPoint, const BPoint &EndPoint);
	int MakeOnePassRough(BPoint &CurPoint, const BPoint & XDir, const BPoint & YDir, double DAppr, double CornerR, bool CCW);
	int MakeLayerFinSide(BPoint &CurPoint);
protected:
	bool bOK;
	curve ArcType;
	BPoint StartPoint;
protected:
	BPoint StartPoint1;
	BPoint StartPoint2;
	BPoint Center;
	BPoint EndPoint; 
	BPoint EX; // единичный вектор длинной стороныВектор рабочего хода
	BPoint EY; // единичный вектор короткой стороны
	double DXrough;
	double DYrough;
	double DXfinish;
	double DYfinish;
	bool CCWDir; 
	bool NeedAppr;
	bool NeedRough;
	bool NeedFinish;
	double XYdist;
	double ZdistRo;
	double ZdistFi;
	double DZdownRo;
	double DZdownFi;
	double CornerR;
	double ApprAngle;
	int ApprType;
	double DZup;

	BGeomArray *pCont;

};
