#pragma once
#include "bdcel_trm.h"

class NCMB_API BDCEL_BSP :
	public BDCEL_TRM
{
public:
	BDCEL_BSP(void);
	virtual ~BDCEL_BSP(void);
	virtual const BPoint GetNormByNum(int Num) const override;
	int MakeFromStockTri(const float *pCI);

protected:
	float *pN;
protected:
	void StoreNormal(int Ind, const BPoint &N);


};
