#include "stdafx.h"
#include "NMachUnitPair.h"

NMachUnitPair::NMachUnitPair() noexcept
{
	Clear();
}

NMachUnitPair::NMachUnitPair(class MTMachineTool& InMach, const class NCUnit& InUnit) noexcept
{
	Set(InMach, InUnit);
}

void NMachUnitPair::Set(MTMachineTool& InMach, const NCUnit& InUnit) noexcept
{
	pMach = &InMach;
	pUnit = &InUnit;
}

void NMachUnitPair::Clear() noexcept
{
	pMach = nullptr;
	pUnit = nullptr;
}
