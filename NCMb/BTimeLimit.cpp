#include "stdafx.h"
#include "mmsystem.h"
#include "BTimeLimit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

BTimeLimit::BTimeLimit(DWORD Period /*= 40*/)
{
	MinTime = Period;
	ReStart();
}


BTimeLimit::~BTimeLimit(void)
{
}

bool BTimeLimit::Check(void)
{// returns false if given time period (MinTime) is not elapsed yet
	// otherwise returns true and starts new time period
	auto CurTime = timeGetTime();
	if(CurTime - StartTime < MinTime)
		return false;
	StartTime = CurTime;
	return true;
}

bool BTimeLimit::IsElapsed(void) const
{// returns false if given time period (MinTime) is not elapsed yet
	// otherwise returns true and starts new time period
	return (timeGetTime() - StartTime > MinTime);
}

void BTimeLimit::ReStart(void)
{
	StartTime = 0;
}
