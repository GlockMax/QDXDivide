#include "stdafx.h"
#include "BStockToolInds.h"

BStockToolInds::BStockToolInds()
{
	stPos_ = 0;
	turret_ = 0;
	attach_ = 0;
}

BStockToolInds::BStockToolInds(int stPos, int turret, int attach)
{
	stPos_ = stPos;
	turret_ = turret;
	attach_ = attach;
}
