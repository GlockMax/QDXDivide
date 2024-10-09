#include "StdAfx.h"
#include "MTCompensations.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

MTCompensations::MTCompensations(void)
{
	CompTool = true;
	CompStock = true;
	CompLocal = true;
	RCPMask = 0;
}


MTCompensations::~MTCompensations(void)
{
}
