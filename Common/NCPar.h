#pragma once

class AFX_EXT_CLASS NCPar // Parameters for the lathe cycles
{
public:
	NCPar(void);
	~NCPar(void);
	void Reset(void);
public:
	double PIT	; //	Thread pitch as a value (enter without sign)
	double MPIT	; //	Thread pitch as a thread size Range of values: 3 (for M3) ... 60 (for M60)
	double SPL	; //	Thread starting point in the longitudinal axis
	double FPL	; //	Thread end point in the longitudinal axis
	double DM1	; //	Thread diameter at the starting point
	double DM2	; //	Thread diameter at the end point
	double APP	; //	Run-in path (enter without sign)
	double ROP	; //	Run-out path (enter without sign)
	double TDEP	; //	Thread depth (enter without sign)
	double FAL	; //	Finishing allowance (enter without sign)
	double IANG	; //	Infeed angle Range of values: “+” (for flank infeed at the flank) “–” (for alternating flank infeed)
	double NSP	; //	Starting point offset for the first thread turn (enter without sign)
	int NRC	; //	Number of roughing cuts (enter without sign)
	int NID	; //	Number of idle passes (enter without sign)
	int VARI	; //	Determination of type of the machining for the thread Range of values: 1 ... 4
	int NUMT	; //	Number of thread turns (enter without sign)
};
