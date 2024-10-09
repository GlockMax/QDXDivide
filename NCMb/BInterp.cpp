#include "StdAfx.h"
#include "binterp.h"

BInterp::BInterp(void)	
	: ABCStd(0., 0., 0., 1.)
	, XYZStd(0., 0., 0., 1.)
	, Center(0., 0., 0., 1.)
	, RotDir(0., 0., 1., 0.)
	, BasDir(1., 0., 0., 0.)
	, PrevC0()
	, CurC0()

{
	Active = false;
	Activating = false;
	Deactivating = false;
	InterpPlane = XY;
	PreviousPlane = XY;
	FanucStyle = true;
	Alfa0 = 0.;
}

BInterp::~BInterp(void)
{
}

void BInterp::DeActivate(void)
{
	Active = false;
	Deactivating = true;
}
bool BInterp::Activate(enum Plane PPl, enum Plane IPl, const BPoint &C, const BPoint &B, const BPoint &S, const NCBase &Base, double A0)
{
	Active = true;
	PreviousPlane = PPl;
	InterpPlane = IPl;
	Center = C;
	BasDir = B;
	XYZStd = S;
	StoredBase = Base;
	//Alfa0 = A0;
	//PrevC0.SetE();
	//CurC0.SetE();
	Activating = true;
	Deactivating = false;
	return true;
}

bool BInterp::ApplyMatrs(NCadrGeom & Geom) const
{
	if (!Geom.IsLine())
		return false;
	Geom.SetB(Geom.GetB() * PrevC0);
	Geom.SetE(Geom.GetE() * CurC0);
	return true;
}

bool BInterp::ApplyOneMatr(NCadrGeom & Geom) const
{
	Geom.Tr(CurC0);
	return false;
}

void BInterp::AddAlfa0(BPoint &ABC) const
{
	ABC = ABC + ABCStd * Alfa0;
}
