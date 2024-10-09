#pragma once
template<unsigned int BIT_SIZE>
class BBitArray
{
public:
	BBitArray(void){};
	~BBitArray(void){};
	bool GetAt(int i) { return (Array[i / ElemSize] & 0x01 << (i % ElemSize)) != 0x00;}
	void SetAt(int i, bool val)
	{
		ElemType Mask = 0x01 << (i % ElemSize);
		i /= ElemSize;
		if(((Array[i] & Mask) != 0) == val) 
			return; // Do not change
		NumberOfOnes += val ? 1 : -1;
		Array[i] = val ? (Array[i] | Mask) : (Array[i] & ~Mask);
	}
	void SetAll(bool val)
	{
		memset(Array, val ? 0xFF : 0x00, ArraySize * sizeof(ElemType));
		NumberOfOnes = val ? BIT_SIZE : 0;
	}
	bool IsEmpty(void) { return NumberOfOnes == 0;}
protected:
	typedef BYTE ElemType;
	static const int ElemSize = 8 * sizeof(ElemType);
	static const int ArraySize = (BIT_SIZE / ElemSize) + (((BIT_SIZE % ElemSize) > 0) ? 0 : 1);
	ElemType Array[ArraySize];
	int NumberOfOnes;
};
