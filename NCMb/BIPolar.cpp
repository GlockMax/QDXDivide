#include "StdAfx.h"
#include "math.h"
#include "ConstDef.h"
#include "BMatr.h"
#include "BIPolar.h"

BIPolar::BIPolar(void) 
	: BInterp()
{
}

bool BIPolar::Activate(enum Plane PPl, enum Plane IPl, const BPoint &C, const BPoint &B, const BPoint &S, const BPoint &R, const NCBase &Base, double A0)
{
	BInterp::Activate(PPl, IPl, C, B, S, Base, A0);
	if (FanucStyle)
	{
		ABCStd.Set(0., 0., 1., 0.);
	}
	else
	{
		switch (InterpPlane)
		{
		case XY:
			//		RotDir.Set(0., 0., -1., 0.);
			ABCStd.Set(0., 0., 1., 0.);
			break;
		case XZ:
			//		RotDir.Set(0., 1., 0., 0.);
			ABCStd.Set(0., 1., 0., 0.);
			break;
		case YZ:
			//		RotDir.Set(1., 0., 0., 0.);
			ABCStd.Set(1., 0., 0., 0.);
			break;
		}
	}
	RotDir = R;
	return true;
}

BIPolar::~BIPolar(void)
{
}

bool BIPolar::MapP2PA(const BPoint &Pi, BPoint &XYZ, BPoint &ABC) const
{
	BPoint V = Pi - Center; 
	BPoint Vrd = RotDir * (RotDir * V);// Project to the RotDir direction
	V = V - Vrd; // Project to the plane orthogonal to RotDir
	if(V.D2() < MIND * MIND)
		V.Set(1., 0., 0., 1.);
	ABC = ABC - ABCStd * (ABCStd * ABC) + ABCStd * V.Angle(BasDir) * (ABCStd * RotDir);// An angle between V and BaseDir
	// Первое слагаемое выше сохраняет все поворотные координаты, кроме той, которая соответствует ABCStd
	XYZ = XYZStd * sqrt(V.D2()) + Vrd + BPoint(0., 0., 0., 1.);// Distance from the given point to Center
	return true;
}

bool BIPolar::MapPA2P(const BPoint & XYZ, const BPoint & ABC, BPoint & Pi) const
{
	Pi = XYZ;
	Pi.SetH(1.);
	Pi = Pi * GetMatr(-(ABC * ABCStd));
	return false;
}

const BMatr BIPolar::GetMatr(double Angle) const
{
	return 	BMatr().rotg(Angle, BPoint(0., 0., 0., 1.), RotDir);
}



