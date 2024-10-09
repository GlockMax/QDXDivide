#pragma once
#include "NCMB_API.h"

enum NCMApplicationType
{
	AT_MILL,
	AT_TURN,
	AT_MILL_TURN,
	AT_PUNCH,
	AT_WELD,
	AT_UNDEF
};
typedef unsigned __int16 NCMConfig;


class NCMB_API NCMComData
{
public:
	NCMComData(void);
	~NCMComData(void);
	static bool IsMillEnabled();
	static bool IsTurnEnabled();
	static bool IsWeldEnabled();
	static NCMApplicationType GetType();
	static void SetType(NCMApplicationType InType);
protected:
	static NCMApplicationType Type;
};
