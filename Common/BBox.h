// BBox.h: interface for the BBox class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "BMatr.h"
#include "NCMB_API.h"

enum RelPos
{
	RP_MIN,
	RP_MID,
	RP_MAX,
	RP_ZER
};

const float MAXVAL = 1.e20f;

class NCMB_API BBox  
{
public:
	BBox();
	virtual ~BBox();
	void Project(BBox &in,const BMatr &m);
	bool Expand(const BPoint &p);
	const BBox& Transform(const BMatr &m);
	BBox Move(double x,double y,double z);
	bool HaveInt(const BBox &box);
	void Offset(double dx,double dy,double dz);
	void Expand(const BBox &in);
	void Project(BBox &in);
	double dZ() const;
	double dY() const;
	double dX() const;
	bool IsDefined()const;
	bool Expand(double x,double y,double z);
	void Init();
	double MaxLineD2(const BPoint& p1, const BPoint& p2) const;
	double Xmin, Xmax, Ymin, Ymax, Zmin, Zmax;
	bool operator != (const BBox &In) const { return !operator == (In);}

protected:
	bool Def;
public:
	const BPoint GetMinPoint(void) const;
	const BPoint GetMaxPoint(void) const;
	const BPoint GetCenterPoint(void) const;
	bool operator ==(const BBox &p) const;
	const BPoint GetRelPoint(RelPos X, RelPos Y, RelPos Z) const;
	static double FindIntervVal(RelPos m_C, double MinVal, double MaxVal);
	float * MakeTriangles(void) const;
	const BPoint RayCasting(const BPoint & P, const BPoint & V, double MinW, BPoint &N) const;
	void ApplyLimits(void);
	void SetDefined(void) { Def = true;}
	bool Contains(const BPoint &P) const;
	bool Intersects(const BBox &In) const;
};
