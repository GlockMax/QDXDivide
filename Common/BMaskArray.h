#pragma once

template<unsigned int FIELD_SIZE, unsigned int X_SIZE, unsigned int Y_SIZE>
// FIELD_SIZE must be 1 or 2 or 4 or 8 
class BMaskArray
{
public:
	BMaskArray(void)
	{
		for(int i = 0; i < FIELD_SIZE; ++i)
			Mask |= 1 << i;
		memset(mem, 0x0, sizeof(BYTE) * X_SIZE * Y_SIZE * FIELD_SIZE / 8);
	};
	~BMaskArray(void){};
	BYTE GetAt(int ix, int iy)
	{
		unsigned int Num = iy * X_LENGTH + ix * sizeof(BYTE) * FIELD_SIZE / 8;
		unsigned int Rest = (ix * sizeof(BYTE) * FIELD_SIZE) % 8;
		BYTE res = (mem[Num] >> Rest) & Mask;
		return res;
	};
	void SetAt(int ix, int iy, BYTE val)
	{
		unsigned int Num = iy * X_LENGTH + ix * sizeof(BYTE) * FIELD_SIZE / 8;
		unsigned int Rest = (ix * sizeof(BYTE) * FIELD_SIZE) % 8;
		mem[Num] = (mem[Num] & (!(Mask << Rest))) | val << Rest;
	};
private:
	static const unsigned int X_LENGTH = int(sizeof(BYTE) * X_SIZE * FIELD_SIZE / 8 + 0.99);
	BYTE Mask;
	BYTE mem[X_LENGTH * Y_SIZE ];
};
