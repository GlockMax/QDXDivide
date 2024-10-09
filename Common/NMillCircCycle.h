#pragma once
#include "BPoint.h"
#include "BGeomArray.h"

class AFX_EXT_CLASS NMillCircCycle
{
public:
	NMillCircCycle(BGeomArray &Cont
		, const	BPoint &StartPoint
		, const	BPoint &EndPoint 
		, const	BPoint &Center
		, const	BPoint &CDir // Вектор рабочего хода
		, double XYdist// Расстояние между проходами
		, double Zdist
		, double DZdown
		, double DZup
		, bool OneDirCut);
	~NMillCircCycle(void);
	bool IsOK();
	int MakePath(void);
	int MakeLayer(BPoint &CurPoint, double DownStep, double UpStep, double IntStep);
protected:
	bool bOK;
	double CircR;
	BPoint Norm;
	bool OneDirCut;
protected:
	BPoint StartPoint;
	BPoint EndPoint;
	BPoint Center;
	BPoint CDir; // Вектор рабочего хода (единичный)
	double XYdist;
	double Zdist;
	double DZdown;
	double DZup;

	BGeomArray *pCont;

public:
	BPoint GetSimP(BPoint & P);
public:
	BPoint GetNextP(const BPoint & P);
};
