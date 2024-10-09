#include "StdAfx.h"
#include "..\NCMExt\NExtEnums.h"
#include "NCollisionLog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

NCollisionLog::NCollisionLog(void)
{
	LogSize = 0;
}

NCollisionLog::~NCollisionLog(void)
{
}

bool NCollisionLog::AddElem(int pos, const NCollLogElem &Elem)
{
	if(pos < 0)
		return false;
	if(LogSize >= MAX_COLLISIONS_STORED - 1)
		return false;
	//Shift Log
	for(int i = LogSize; i > pos; --i)
		Log[i] = Log[i - 1];
	Log[pos] = Elem;
	++LogSize;
	return true;
}

int NCollisionLog::FindElem(const cadrID &CadrID) const
{
	// If CadrID presents in the log return -(index + 1) otherwise
	// returns index of the first grater CadrID or LogSize 

	if(LogSize == 0)
		return LogSize;
	// Check last element first (probability is gratest)
	const NCollLogElem &Prev = Log[LogSize - 1];
	if(CadrID == Prev.PTPoint)
		return -LogSize;
	if(CadrID > Prev.PTPoint)
		return LogSize;
	if(CadrID < Log[0].PTPoint)
		return 0;
	// Find proper position to insert new Elem before
	int Start = 0;
	int End = LogSize - 1;
	while(End - Start > 1)
	{
		int Mid = (Start + End) / 2;
		if(Log[Mid].PTPoint > CadrID)
			End = Mid;
		else
			Start = Mid;
	}
	if(CadrID == Log[Start].PTPoint)
		return -(Start + 1);
	return End;
}
