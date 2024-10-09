#pragma once
#include "NCMB_API.h"

class NCMB_API BProgressPercent
{
public:
	BProgressPercent();
	void Reset();
	void ClearStr();
	void StartPart();
	const CString& FillCurStr();
	void NextStep(double Val);
protected:
	int ProgressPerCent;
	int LastPerCent;
	int OldPerCent;
	int PerCentStep;
	CString str;

};

