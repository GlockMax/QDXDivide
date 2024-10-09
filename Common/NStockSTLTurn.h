#pragma once
#include "NStockSTL.h"
class AFX_EXT_CLASS NStockSTLTurn :
    public NStockStl
{
public:
	DECLARE_SERIAL(NStockSTLTurn)
	NStockSTLTurn(void);
	~NStockSTLTurn(void);
	bool CreateBody(void);
	StockCrType GetType() const override;
};

