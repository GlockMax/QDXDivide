#pragma once
#include "MACHINE_API.h"

class MACHINE_API NMachUnitPair
{
public:
	NMachUnitPair() noexcept;
	NMachUnitPair(class MTMachineTool& InMach, const class NCUnit& InUnit) noexcept;
	void Set(class MTMachineTool& InMach, const class NCUnit& InUnit) noexcept;
	class MTMachineTool& GetMach() { return *pMach; }
	const class NCUnit& GetUnit() const { return *pUnit; }
	void Clear() noexcept;
protected:
	class MTMachineTool* pMach;
	const class NCUnit* pUnit;
};

