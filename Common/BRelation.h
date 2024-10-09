#pragma once
#include "NObject.h"
#include "BMaskArray.h"
template <unsigned int ACTIVE_OBJECTS, unsigned int PASSIVE_OBJECTS>
class BRelation
{
public:
	BRelation(void){};
	~BRelation(void){};
	BYTE GetAt(int ix, int iy){ return MaskArray.GetAt(ix, iy); };
	void Clear(int ix, int iy){ MaskArray.SetAt(ix, iy, ST_EMPTY); };
	void SetAt(int ix, int iy, PObjectStatus Status){ MaskArray.SetAt(ix, iy, Status); };

private:
	BMaskArray<4, PASSIVE_OBJECTS, ACTIVE_OBJECTS> MaskArray;
};
