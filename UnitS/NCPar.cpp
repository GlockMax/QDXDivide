#include "stdafx.h"
#include "NCPar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

NCPar::NCPar(void)
{
}

NCPar::~NCPar(void)
{
}

void NCPar::Reset(void)
{
	PIT		= 0.	; //	Thread pitch as a value (enter without sign)
	MPIT	= 0.	; //	Thread pitch as a thread size Range of values: 3 (for M3) ... 60 (for M60)
	SPL		= 0.	; //	Thread starting point in the longitudinal axis
	FPL		= 0.	; //	Thread end point in the longitudinal axis
	DM1		= 0.	; //	Thread diameter at the starting point
	DM2		= 0.	; //	Thread diameter at the end point
	APP		= 0.	; //	Run-in path (enter without sign)
	ROP		= 0.	; //	Run-out path (enter without sign)
	TDEP	= 0.	; //	Thread depth (enter without sign)
	FAL		= 0.	; //	Finishing allowance (enter without sign)
	IANG	= 0.	; //	Infeed angle Range of values: “+” (for flank infeed at the flank) “–” (for alternating flank infeed)
	NSP		= 0.	; //	Starting point offset for the first thread turn (enter without sign)
	NRC		= 0	; //	Number of roughing cuts (enter without sign)
	NID		= 0	; //	Number of idle passes (enter without sign)
	VARI	= 0	; //	Determination of type of the machining for the thread Range of values: 1 ... 4
	NUMT	= 0	; //	Number of thread turns (enter without sign)
}