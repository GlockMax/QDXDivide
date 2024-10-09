#include "stdafx.h"
#include "BEdgesAttrs.h"


BEdgesAttrs::BEdgesAttrs() : EdgesColor(0., 0., 0., 1.)
{
	EdgesWidth = 2.;
	SmoothAngle = 15.;
}


BEdgesAttrs::~BEdgesAttrs()
{
}

void BEdgesAttrs::SetEdgesColor(const NColor & Col)
{
	EdgesColor = Col;
}

void BEdgesAttrs::SetEdgesWidth(double Width)
{
	EdgesWidth = Width;
}

void BEdgesAttrs::SetSmoothAngle(double Angle)
{
	SmoothAngle = Angle;
}

void BEdgesAttrs::Serialize(CArchive & ar)
{
	EdgesColor.Serialize(ar);

	SerializeElements(ar, &EdgesWidth, 1);
	SerializeElements(ar, &SmoothAngle, 1);
}
