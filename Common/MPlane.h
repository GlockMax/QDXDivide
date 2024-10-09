// MPlane.h: interface for the MPlane class.
//
//////////////////////////////////////////////////////////////////////
#pragma once
#include "BPoint.h"

class MFace;

class AFX_EXT_CLASS MPlane  
{
public:
	bool operator == (const MPlane &Pl1) const;
	bool operator ||(const MPlane & Pl) const;
	const BPoint &GetPoint() const;
	const BPoint &GetNormal() const;
	MPlane(const MPlane &pl);
	void Set(const BPoint& n, const BPoint& b);
	void SetPointOnPlane(const BPoint& b);
	void SetNormal(const BPoint& n);
	MPlane();
	MPlane(const BPoint& n, const BPoint& b);
	MPlane(int n, int b);
	virtual ~MPlane();

protected:
	int m_n;		// нормаль к плоскости
	int m_b;		// точка на плоскости
	
public:
	bool IsAbove(const BPoint & P) const;
	bool IsAbove(const MFace & F) const;
	double CalcSign(const BPoint &P) const;
	void SetPointN(int N);
	void SetNormalN(int N);
	int GetNormalN(void) const;
	bool IntersLine(const BPoint& P, const BPoint& e, BPoint& R) const;
	double IntersLine(const BPoint& P, const BPoint& e) const;
	bool Contains(const BPoint & P) const;
	int GetPointN(void) const;
	void Save(CFile * f, const BMatr &M);
	void Load(CFile * f);
};
