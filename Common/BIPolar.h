#pragma once
#include "binterp.h"

class AFX_EXT_CLASS BIPolar :
	public BInterp
{
public:
	BIPolar(void);
	~BIPolar(void);
	bool MapP2PA(const BPoint &Pi, BPoint &XYZ, BPoint &ABC) const;
	bool MapPA2P(const BPoint &XYZ, const BPoint &ABC, BPoint &Pi) const;
	const BMatr GetMatr(double Angle) const;
	bool Activate(enum Plane PPl, enum Plane IPl, const BPoint &C, const BPoint &B, const BPoint &S, const BPoint &RotDir, const NCBase &Base, double A0);
};
