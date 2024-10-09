#pragma once
#include "binterp.h"

class AFX_EXT_CLASS BICyl :
	public BInterp
{
public:
	BICyl(void);
	~BICyl(void);
	bool MapP2PA(const BPoint &Pi, BPoint &XYZ, BPoint &ABC) const;
	bool MapVv2PA(const BPoint &Vv, BPoint &XYZ, BPoint &ABC) const;
	bool Activate(enum Plane PPl, enum Plane IPl, const BPoint &C, const BPoint &B, const BPoint &S, double R, const class NCBase &Base, double A0);
protected:
	double Rad;
};
