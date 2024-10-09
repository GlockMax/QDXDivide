#pragma once
#include "NCMB_API.h"

class NCMB_API BStockToolInds
{
public:
	BStockToolInds();
	BStockToolInds(int stPos, int turret, int attach);
	int stPos_;
	int turret_;
	int attach_;
};
