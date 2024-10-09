#include "stdafx.h"
#include "ResLocal.h"
#include "BProgressPercent.h"

BProgressPercent::BProgressPercent()
{
	Reset();
	StartPart();
}

void BProgressPercent::Reset()
{
	ProgressPerCent = -1;
	LastPerCent = -1;
}

void BProgressPercent::ClearStr()
{
	str.Empty();
}

void BProgressPercent::StartPart()
{
	OldPerCent = -1;
	PerCentStep = 1;
}

const CString& BProgressPercent::FillCurStr()
{
	if (ProgressPerCent >= 0
		&& ProgressPerCent != LastPerCent)
	{
		LastPerCent = ProgressPerCent;
		str.Format(IDS_RUN_PC, ProgressPerCent);
	}
	return str;
}

void BProgressPercent::NextStep(double Val)
{
	int NewPerCent = int(100. * Val);
	if (NewPerCent - OldPerCent > PerCentStep)
	{
		if (NewPerCent > 100)
			PerCentStep = NewPerCent / 50;
		ProgressPerCent = NewPerCent;
		OldPerCent = NewPerCent;
		AfxGetApp()->DoWaitCursor(0);
	}
}
