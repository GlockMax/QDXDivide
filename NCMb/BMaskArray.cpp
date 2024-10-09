#include "stdafx.h"
//#include "afxtemplate.h"
#include "BMaskArray.h"

template<unsigned int FIELD_SIZE, unsigned int X_SIZE, unsigned int Y_SIZE>
BMaskArray<FIELD_SIZE, X_SIZE, Y_SIZE>::BMaskArray(void)
{
	for(int i = 0; i < FIELD_SIZE; ++i)
		Mask |= 1 << i;
	memset(mem, 0x0, sizeof(BYTE) * X_SIZE * Y_SIZE * FIELD_SIZE / 8);
}

template<unsigned int FIELD_SIZE, unsigned int X_SIZE, unsigned int Y_SIZE>
BMaskArray<FIELD_SIZE, X_SIZE, Y_SIZE>::~BMaskArray(void)
{
}

template<unsigned int FIELD_SIZE, unsigned int X_SIZE, unsigned int Y_SIZE>
BYTE BMaskArray<FIELD_SIZE, X_SIZE, Y_SIZE>::GetAt(int ix, int iy)
{
	unsigned int Num = iy * X_LENGTH + ix * sizeof(BYTE) * FIELD_SIZE / 8;
	unsigned int Rest = (ix * sizeof(BYTE) * FIELD_SIZE) % 8;
	BYTE res = (mem[Num] >> Rest) & Mask;
	return res;
}

template<unsigned int FIELD_SIZE, unsigned int X_SIZE, unsigned int Y_SIZE>
void BMaskArray<FIELD_SIZE, X_SIZE, Y_SIZE>::SetAt(int ix, int iy, BYTE val)
{
	unsigned int Num = iy * X_LENGTH + ix * sizeof(BYTE) * FIELD_SIZE / 8;
	unsigned int Rest = (ix * sizeof(BYTE) * FIELD_SIZE) % 8;
	mem[num] = (mem[Num] & (!(Mask << Rest))) | val << Rest;
}
