#pragma once
#include "BPoint.h"
#include "BGeomArray.h"

class AFX_EXT_CLASS NMillRectCycle
{
public:
	NMillRectCycle(BGeomArray &Cont
		, const	BPoint &StartPoint
		, const	BPoint &EndPoint
		, const	BPoint &CDir // ������ �������� ����
		, const BPoint &BDir // ������ ����������� ����� �������� ������
		, int Npas
		, double Zdist
		, double DZdown
		, double DZup
		, bool OneDirCut);
	~NMillRectCycle(void);
	bool IsOK();
	int MakePath(void);
	int MakeLayer(BPoint &CurPoint, double DownStep, double UpStep, double IntStep);
protected:
	bool bOK;
	bool OneDirCut;
protected:
	BPoint StartPoint;
	BPoint EndPoint;
	BPoint CDir; // ������ �������� ����
	BPoint BDir; // ������ ����������� ����� �������� ������
	int Npas;
	double Zdist;
	double DZdown;
	double DZup;

	BGeomArray *pCont;

};
