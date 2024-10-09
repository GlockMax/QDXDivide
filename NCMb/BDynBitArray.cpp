#include "stdafx.h"
#include "BDynBitArray.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

BDynBitArray::BDynBitArray(int SizeInBits)
{
	BitSize = SizeInBits;
	MemSize = BitSize / ( 8 * sizeof(ElemType)) + 1;
	BitsInLastElem = BitSize - 8 * sizeof(ElemType) * (MemSize - 1);
	Mem = new ElemType[MemSize];
	LastInd = -1;
	pLastMemElem = &Mem[0];
	LastMask = 0x00;
}

BDynBitArray::~BDynBitArray(void)
{
	delete[] Mem;
}
void BDynBitArray::SetAt(int Ind, bool Val) noexcept
{
	const int ElemSize = 8 * sizeof(ElemType);
	ElemType Mask = 0x01 << (Ind % ElemSize);
	if(Val)
		Mem[Ind / ElemSize] |= Mask; 
	else
		Mem[Ind / ElemSize] &= 0xffffffff - Mask; 
}
bool BDynBitArray::SetFalse(int Ind) noexcept
{
	const int ElemSize = 8 * sizeof(ElemType);
	if (++LastInd == Ind)
	{
		if (LastMask == 0)
			LastMask = 0x01;
		else
		{
			LastMask <<= 1;
			if (LastMask == 0)
			{
				LastMask = 0x01;
				++pLastMemElem;
			}
		}
	}
	else
	{
		LastInd = Ind;
		pLastMemElem = &Mem[Ind / ElemSize];
		LastMask = 0x01 << (Ind % ElemSize);
	}
	bool Res = (((*pLastMemElem) & LastMask) != 0x00);
	if (Res)
		*pLastMemElem &= 0xffffffff - LastMask;
	return Res;
}
void BDynBitArray::SetFalseNext() noexcept
{
	// Set false without checking
	++LastInd;
	if (LastMask == 0)
		LastMask = 0x01;
	else
	{
		LastMask <<= 1;
		if (LastMask == 0)
		{
			LastMask = 0x01;
			++pLastMemElem;
		}
	}
	*pLastMemElem &= 0xffffffff - LastMask;
}
void BDynBitArray::SetTrueAt(int Ind) noexcept
{
	if(Ind < 0)
		return;
	constexpr int ElemSize = 8 * sizeof(ElemType);
	const ElemType Mask = 0x01 << (Ind % ElemSize);
	Mem[Ind / ElemSize] |= Mask; 
}
bool BDynBitArray::operator[] (int Ind) const noexcept
{
	constexpr int ElemSize = 8 * sizeof(ElemType);
	const ElemType Mask = 0x01 << (Ind % ElemSize);
	return (Mem[Ind / ElemSize] & Mask) != 0x00;
}
void BDynBitArray::SetAllTrue() noexcept
{
	LastInd = -1;
	pLastMemElem = &Mem[0];
	LastMask = 0x00;
	memset(Mem, 0xFF, MemSize * sizeof(ElemType));
}
void BDynBitArray::SetAllFalse() noexcept
{
	memset(Mem, 0x00, MemSize * sizeof(ElemType));
}
bool BDynBitArray::IsAllFalse() const noexcept
{
	ElemType Sum = 0x0;
	for (int i = 0; i < MemSize; ++i)
		Sum |= Mem[i];
	return (Sum == 0x0);
}
int BDynBitArray::FindFirstTrue() const noexcept
{
	ElemType Mask = 0x0;
	for (int i = 0; i < MemSize - 1; ++i)
	{
		if (Mem[i] != Mask)
		{
			ElemType InternalMask = 0x1;
			ElemType BMem = Mem[i];
			for (int k = 0; k < 8 * sizeof(ElemType); ++k)
			{
				if ((InternalMask & BMem) != 0)
					return i * 8 * sizeof(ElemType) + k;
				InternalMask <<= 1;
			}
		}
	}
	ElemType InternalMask = 0x1;
	ElemType BMem = Mem[MemSize - 1];
	for (int k = 0; k < BitsInLastElem; ++k)
	{
		if ((InternalMask & BMem) != 0)
			return (MemSize - 1) * 8 * sizeof(ElemType) + k;
		InternalMask <<= 1;
	}
	return -1;
}

bool BDynBitArray::Save2File(CFile* f) const
{
	f->Write(&MemSize, sizeof(MemSize));
	f->Write(&BitSize, sizeof(BitSize));
	f->Write(&BitsInLastElem, sizeof(BitsInLastElem));
	f->Write(&LastInd, sizeof(LastInd));
	f->Write(&LastMask, sizeof(LastMask));
	f->Write(Mem, sizeof(Mem[0]) * MemSize);
	long long Dist = pLastMemElem - Mem;
	f->Write(&Dist, sizeof(Dist));
	return true;
}

BDynBitArray* BDynBitArray::Load(CFile* f)
{
	int MS;
	f->Read(&MS, sizeof(MemSize));
	int BS;
	f->Read(&BS, sizeof(BitSize));
	BDynBitArray* pOut = new BDynBitArray(BS);

	f->Read(&pOut->BitsInLastElem, sizeof(BitsInLastElem));
	f->Read(&pOut->LastInd, sizeof(LastInd));
	f->Read(&pOut->LastMask, sizeof(LastMask));
	f->Read(pOut->Mem, sizeof(Mem[0]) * MS);
	long long Dist = 0;
	f->Read(&Dist, sizeof(Dist));
	pOut->pLastMemElem = pOut->Mem + Dist;
	return pOut;
}

bool BDynBitArray::GetAt(int Ind) const noexcept
{
	return operator[](Ind);
}

BDynBitArray* BDynBitArray::CreateFullCopy() const
{
	BDynBitArray* pOut = new BDynBitArray(BitSize);
	auto bufMem = pOut->Mem;
	*pOut = *this;
	pOut->Mem = bufMem;
	memcpy_s(pOut->Mem, sizeof(Mem[0]) * MemSize, Mem, sizeof(Mem[0]) * MemSize);
	pOut->pLastMemElem = pOut->Mem + (pLastMemElem - Mem);
	return pOut;
}

