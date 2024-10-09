#pragma once
#include "CadrID.h"
#include "UNITS_API.h"

class UNITS_API NEvalStopCrit
{
public:
	NEvalStopCrit();
	NEvalStopCrit(int N);
	NEvalStopCrit(cadrID ID);
	bool IsInfinite() const noexcept;
	bool IsCount() const noexcept;
	int GetCount() const noexcept;
	cadrID GetStopCadr() const noexcept;
	void SetInfinite() noexcept;
protected:
	int Count;
	cadrID StopCadr;
};

