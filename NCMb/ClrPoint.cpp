#include "stdafx.h"
#include "ClrPoint.h"

ClrPoint::ClrPoint(const float* base) : BPoint(*base, *(base + 1), *(base + 2), 1.)
{
	Color.fval = *(base + 3);
}

ClrPoint::ClrPoint(const BPoint& P) : BPoint(P)
{
	Color.fval = 0.f;
}

ClrPoint::ClrPoint(const BPoint& P, PackedColor Col) : BPoint(P)
{
	Color = Col;
}

void ClrPoint::SetColor(float Val)
{
	Color.fval = Val;
}

void ClrPoint::SetColor(PackedColor Val)
{
	Color = Val;
}
