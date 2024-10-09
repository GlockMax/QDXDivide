#pragma once
#include "BGeomArray.h"
#include "NTurnCycleBas.h"

class AFX_EXT_CLASS NTurnCycle : public NTurnCycleBas
{
protected:
	enum CEType
	{
		Up,
		Down,
		UpDown,
		DownUp,
		Undef
	};
public:
	class ContElement
	{
	public:
		ContElement();
		~ContElement(){};
	public:
		BPoint Point;
		CEType Type;
		int StartGeomNum;
		int NextElemNum; // Номер след. элм-та с таким же X (если достижим)
	};
public:
	NTurnCycle(BGeomArray &Cont, double XStep, double XRetr, double iSign);
	~NTurnCycle(void);
protected:
	BGeomArray *pCont;
	double Step;
	double Retr;
	double Sign;
	int MakePath(void);
	int MakePathNonMonotone(void);
	bool orient;
	BPoint BasePoint;
	double RotSign;
	CArray <ContElement, const ContElement &> Elements;
public:
	static double ZDir;
public:
	bool MakeOneStep(BGeomArray & OneStep, const BPoint & StartPoint, double StepX);
	bool GetZ(double X, double * pZ);
	void OrientCont(void);
	void RestoreCont(void);
protected:
	static int CompareElements( const void *a, const void *b);
	int MakeReturn(const BPoint &P0, const BPoint &P1, BGeomArray &GeomArray);
	int MakeElementStep(int e, const BGeomArray *pBCont, BGeomArray &GeomArray);
	int MakeZStep(int e, const BGeomArray *pBCont, BGeomArray &GeomArray);
};
