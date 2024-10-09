#include "stdafx.h"
#include "BProgVisInterval.h"


BProgVisInterval::BProgVisInterval()
{
}

BProgVisInterval::BProgVisInterval(int S, int E, bool U, NColor Col)
{
	Start = S;
	End = E;
	UseColor = U;
	Color = Col;
}


BProgVisInterval::~BProgVisInterval()
{
}

bool BProgVisInterval::Inside(int Num) const
{
	return Num >= Start && Num <= End;
}

void BProgVisInterval::JoinIntersected(const BProgVisInterval & Int)
{
	Start = min(Start, Int.Start);
	End = max(End, Int.End);
}
