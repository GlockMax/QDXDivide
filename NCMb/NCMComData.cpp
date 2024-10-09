#include "stdafx.h"
#include "NCMComData.h"

NCMApplicationType NCMComData::Type = AT_PUNCH;

NCMComData::NCMComData(void)
{
}

NCMComData::~NCMComData(void)
{
}

bool NCMComData::IsMillEnabled()
{
	return Type == AT_MILL || Type == AT_MILL_TURN;
}

bool NCMComData::IsTurnEnabled()
{
	return Type == AT_TURN || Type == AT_MILL_TURN;
}

bool NCMComData::IsWeldEnabled()
{
	return Type == AT_WELD;
}

NCMApplicationType NCMComData::GetType()
{
	return Type;
}

void NCMComData::SetType(NCMApplicationType InType)
{
	Type = InType;
}
