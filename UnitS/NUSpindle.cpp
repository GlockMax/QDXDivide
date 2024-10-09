#include "stdafx.h"
#include "NCEnums.h"
#include "NUSpindle.h"

NUSpindle::NUSpindle(void)
{
	Clear();
}

NUSpindle::~NUSpindle(void)
{
}

void NUSpindle::Clear(void)
{
	SpeedM = REVpMIN;
	Speed = 0;
	MaxSpeed = -1.;
	FixCutSpeed = FALSE;
	FixedCutSpeed = -1.;
	SetMaxSpeed = FALSE;
	SpindleDir = R_UNDEF;
}

double NUSpindle::CalcSpeed(void)
{
	// Calculate speed in rev/sec
	switch (SpeedM)
	{
	case REVpMIN:
	default:
		return Speed / 60.;
	case REVpSEC:
		return Speed;
	}
}

