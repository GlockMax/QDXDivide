#pragma once
#include "NCMB_API.h"

class NCMB_API BDynBitArray
{
public:
	BDynBitArray(int SizeInBits);
	~BDynBitArray(void);
protected:
	typedef __int32 ElemType;
	ElemType *Mem;
	int BitSize;
	int MemSize;
	int BitsInLastElem;
	int LastInd;
	ElemType *pLastMemElem;
	ElemType LastMask;
public:
	void SetAt(int Ind, bool Val) noexcept;
	void SetTrueAt(int Ind) noexcept;
	bool operator[] (int Ind) const noexcept;
	void SetAllTrue() noexcept;
	void SetAllFalse() noexcept;
	bool IsAllFalse() const noexcept;
	bool SetFalse(int Ind) noexcept;
	void SetFalseNext(void) noexcept;
	bool IsEmpty(void) const  noexcept { return LastInd == -1; }
	int FindFirstTrue() const noexcept;
	bool Save2File(CFile* f) const;
	static BDynBitArray* Load(CFile* f);
	bool GetAt(int Ind) const noexcept;
	BDynBitArray* CreateFullCopy() const;
};
