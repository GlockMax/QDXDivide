#include "stdafx.h"
#include "NEvalStopCrit.h"

NEvalStopCrit::NEvalStopCrit()
{
	SetInfinite();
}

NEvalStopCrit::NEvalStopCrit(int N) : NEvalStopCrit()
{
	Count = N;
}

NEvalStopCrit::NEvalStopCrit(cadrID ID) : NEvalStopCrit()
{
	StopCadr = ID;
}

bool NEvalStopCrit::IsInfinite() const noexcept
{
	return Count == -1 && StopCadr.IsEmpty();
}

bool NEvalStopCrit::IsCount() const noexcept
{
	return Count >= 0;
}

int NEvalStopCrit::GetCount() const noexcept
{
	return Count;
}

cadrID NEvalStopCrit::GetStopCadr() const noexcept
{
	return StopCadr;
}

void NEvalStopCrit::SetInfinite() noexcept
{
	Count = -1;
	StopCadr.Prog = -1;
	StopCadr.Cadr = 0;
}
