// MSLine.h: interface for the MSLine class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "BPoint.h"

class MPlane;
//class BPoint;

class MSLine  
{
public:
	virtual BPoint IntersPlane(const MPlane &Pl) const;
	int GetPointN();
	int GetEN();
	const BPoint & GetE() const;
	const BPoint & GetPoint() const;
	bool IsDefined();
	MSLine(const MPlane &Pl1, const MPlane &Pl2);
	MSLine(int n_p, int n_e);
	MSLine();
	virtual ~MSLine();
protected:
	int m_b; // Start point of the ray
	int m_e; // Direction vector

public:
	MSLine(const BPoint & P, const BPoint & V);
};
