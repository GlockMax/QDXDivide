#pragma once
#include "UNITS_API.h"

class UNITS_API MTCompensations
{
	friend class MTMachineTool;
	friend class NCUnit;
public:
	MTCompensations(void);
	~MTCompensations(void);
	bool operator == (const MTCompensations &In) const {return Val == In.Val;}
protected:
	union
	{
		struct
		{
			bool CompTool : 1;
			bool CompLocal : 1;
			bool CompStock : 1;
			unsigned int RCPMask : 3;
		};
		__int32 Val;
	};
};

