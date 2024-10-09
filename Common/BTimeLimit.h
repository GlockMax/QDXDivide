#pragma once
#include "NCMB_API.h"

class NCMB_API BTimeLimit
{
public:
	BTimeLimit(DWORD Period = 40);
	~BTimeLimit(void);
	bool Check(void);
	void ReStart(void);
	bool IsElapsed(void) const;
protected:
	DWORD StartTime;
	DWORD MinTime;
};

