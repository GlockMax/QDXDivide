#include "StdAfx.h"
#include "constdef.h"
#include "BICyl.h"

BICyl::BICyl(void)
{
	Rad = 0.;
}

BICyl::~BICyl(void)
{
}
bool BICyl::MapP2PA(const BPoint &Pi, BPoint &XYZ, BPoint &ABC) const
{
	// Map Decart point to XYZ ABC
	BPoint V = Pi - Center;
	BPoint Vrd = RotDir * (RotDir * V);// Project to the RotDir direction
	BPoint Virt = BPoint(1., 1., 1., 0.) - XYZStd - RotDir;// Virtual axis
	double Angle = Virt * V * 180. / (PI * Rad);
	ABC = ABCStd * (Angle + Alfa0) + BPoint(0., 0., 0., 1.);
	XYZ = XYZStd * (XYZStd * V) + Vrd + BPoint(0., 0., 0., 1.);// Keep 2 coordinates
	return true;
}
bool BICyl::MapVv2PA(const BPoint &Vv, BPoint &XYZ, BPoint &ABC) const
{
	// Map virtual vector to XYZ ABC
	BPoint Vrd = RotDir * (RotDir * Vv);// Project to the RotDir direction
	BPoint Virt = BPoint(1., 1., 1., 0.) - XYZStd - RotDir;// Virtual axis
	double Angle = Virt * Vv * 180. / (PI * Rad);
	ABC = ABCStd * Angle;
	XYZ = XYZStd * (XYZStd * Vv) + Vrd;// Keep 2 coordinates
	return true;
}
bool BICyl::Activate(enum Plane PPl, enum Plane IPl, const BPoint &C, const BPoint &B, const BPoint &S, double R, const NCBase &Base, double A0)
{
	BInterp::Activate(PPl, IPl, C, B, S, Base, A0);
	RotDir.Set(0., 0., 1., 0.);
	ABCStd.Set(0., 0., 1., 0.);
	Rad = R;
	return true;
}
