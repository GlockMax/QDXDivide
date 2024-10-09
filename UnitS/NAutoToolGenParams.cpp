#include "stdafx.h"
#include "NAutoToolGenParams.h"

NAutoToolGenParams::NAutoToolGenParams() noexcept
{
	Clear();
}

NAutoToolGenParams::~NAutoToolGenParams()
{
}

void NAutoToolGenParams::Clear()
{
	Cool = false;

	CrToolType = 0;
	CrToolCode = 0;
	CrAxisNum = 0;
	CrTurretNum = 0;
	CrMillDiameter = 0;
	CrMillHeight = 0;
	CrMillRadius = 0;
	CrMillAngle = 0;
	CrMillLength = 0;
	CrShankD = 0;
	CrShankH = 0;
	CrHolderD = 0;
	CrHolderH = 0;
	AToolID.Set(CrToolCode, CrTurretNum);
	hpos = 0;
}
