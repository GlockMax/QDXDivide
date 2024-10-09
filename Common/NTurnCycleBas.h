#pragma once

const double DRILL_EMPTY_Z = -1;

class AFX_EXT_CLASS NTurnCycleBas
{
public:
	NTurnCycleBas(void);
	~NTurnCycleBas(void);
	bool IsOK();
protected:
	bool FirstCycleCadr;
	bool bOK;
};
