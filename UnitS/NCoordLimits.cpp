#include "stdafx.h"
#include "NCoordLimits.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

NCoordLimits::NCoordLimits(void)
{
	Clear();
}

NCoordLimits::~NCoordLimits(void)
{
}
void NCoordLimits::Clear()
{
	for(int i = 0; i < LIMITS_NUM; ++i)
	{
		Min[i] = 0.;
		Max[i] = 0.;
		Active[i] = false;
	}
}

void NCoordLimits::SetInactive(void)
{
	for(int i = 0; i < LIMITS_NUM; ++i)
		Active[i] = false;
}

bool NCoordLimits::IsValid(int Num, double Val)
{
	if(Num >= LIMITS_NUM)
		return false;
	if(Active[Num])
	{
		if(Val < Min[Num] || Val > Max[Num])
			return false;
	}
	return true;
}
void NCoordLimits::SetMax(const NCoordLimits &InLimits)
{
	for(int i = 0; i < LIMITS_NUM; ++i)
	{
		Min[i] = InLimits.Max[i];
	}
}
void NCoordLimits::SetMin(const NCoordLimits &InLimits)
{
	for(int i = 0; i < LIMITS_NUM; ++i)
	{
		Min[i] = InLimits.Min[i];
	}
}
