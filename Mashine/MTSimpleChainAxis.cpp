#include "stdafx.h"
#include "BMatr.h"
#include "MTSimpleChainAxis.h"

MTSimpleChainAxis::MTSimpleChainAxis() : MTMatrFieldAxis()
{
	CenterDist = 200.;
	CycleVal = 720.;
	MoveMin = 0.;
	MoveMax = 720.;
}

MTSimpleChainAxis::MTSimpleChainAxis(const MTSimpleChainAxis& Ax)
{
	*this = Ax;
}

const MTSimpleChainAxis& MTSimpleChainAxis::operator=(const MTSimpleChainAxis& Ax)
{
	MTMatrFieldAxis::operator=(Ax);
	CenterDist = Ax.CenterDist;
	CycleVal = Ax.CycleVal;
	return *this;
}
MTSimpleChainAxis* MTSimpleChainAxis::Clone() const
{
	return new MTSimpleChainAxis(*this);
}


MTSimpleChainAxis::~MTSimpleChainAxis()
{
}

void MTSimpleChainAxis::CreateFieldMatr(BMatr& FieldMatr, double T) const
{
	const double Marks[4] = { 0., 180., CycleVal * 0.5, CycleVal * 0.5 + 180. };
	// clamp T to 0-CycleVal interval
	T = T - int(T / CycleVal) * CycleVal;
	if (T < 0.)
		T += CycleVal;
	// create matrix
	if (T <= Marks[1])
	{
		FieldMatr.rotg(T, GetBasePoint(), GetVector());
		return;
	}
	if (T <= Marks[2])
	{
		FieldMatr.rotg(180., GetBasePoint(), GetVector());
		FieldMatr = FieldMatr * BMatr().Trans(GetVectorY() * (CenterDist * (T - Marks[1]) / (Marks[2] - Marks[1])));
		return;
	}
	if (T <= Marks[3])
	{
		FieldMatr.rotg(180., GetBasePoint(), GetVector());
		FieldMatr = FieldMatr * BMatr().Trans(GetVectorY() * CenterDist);
		FieldMatr = FieldMatr * BMatr().rotg(T - Marks[2], GetBasePoint() + GetVectorY() * CenterDist, GetVector());
		return;
	}
	{
		FieldMatr.Trans(GetVectorY() * (CenterDist * (1. - (T - Marks[3]) / (Marks[2] - Marks[1]))));
	}
}

double MTSimpleChainAxis::CalcT(const BPoint& P) const
{
	// Map P to this axis basis
	BPoint LocP = P * GetMatrOr().invr();
	LocP.SetH(0.);
	LocP.SetX(0.);// Project to YZ plane
	if (LocP.GetY() <= 0.)// bottom zone
	{
		// Zero direction is GetVectorZ direction
		double Angle = BPoint(0., 0., 1., 0.).Angle0_180(LocP);// an angle to rotate zero direction around GetVector то LocP direction
		return Angle;
	}
	if (LocP.GetY() >= CenterDist)// top zone
	{
		// Zero direction is GetVectorZ direction
		LocP.SetY(LocP.GetY() - CenterDist);
		double Angle = BPoint(0., 0., 1., 0.).Angle0_180(LocP);// an angle to rotate zero direction around GetVector то LocP direction
		return 180 - Angle + CycleVal * 0.5;
	}
	double RelDist = LocP.GetY() * (CycleVal - 360.) * 0.5 / CenterDist;
	if (LocP.GetZ() <= 0.) // right zone
	{
		return 180. + RelDist;
	}
	{// left zone
		return CycleVal - RelDist;
	}
}

MTAxis::AXIS_TYPE MTSimpleChainAxis::GetType() const
{
	return AT_SCHAIN;
}


