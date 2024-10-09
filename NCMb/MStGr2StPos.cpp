#include "stdafx.h"
#include "BStockGrID.h"
#include "MStGr2StPos.h"

MStGr2StPos::MStGr2StPos() noexcept
{
	Clear();
}

bool MStGr2StPos::operator != (const MStGr2StPos& in) const noexcept
{
	return data_ != in.data_;
}

void MStGr2StPos::Clear() noexcept
{
	data_ = init_data_;
}

void MStGr2StPos::Set(unsigned short i, unsigned short val) noexcept
{
	constexpr UINT64 maskF = 0xF;
	const UINT64 maskVal = (val & maskF) << (i * 4);
	const UINT64 mask0 = ~(maskF << (i * 4));
	data_ &= mask0;
	data_ |= maskVal;
}

unsigned short MStGr2StPos::Get(BStockGrID ind) const noexcept
{
	const BYTE i = BYTE(ind.Get());
	return Get(i);
}

std::vector<BStockGrID> MStGr2StPos::Find(unsigned short stPos) const
{
	std::vector<BStockGrID> ret;
	for (int i = 0; i < 16; ++i)
		if (Get(i) == stPos)
		{
			BStockGrID Gr;
			Gr.Set(i);
			ret.push_back(Gr);
		}
	return ret;
}

bool MStGr2StPos::IsPosEmpty(BStockGrID ind) noexcept
{
	return Get(ind) == POS_UNDEF;
}

bool MStGr2StPos::ApplyChanges(const MStGr2StPos& Changes)
{
	// returns true if some changes take place
	bool ret = false;
	for (unsigned int i = 0; i < 16; ++i)
		if (Changes.Get(i) != POS_UNDEF)
		{
			ret = true;
			Set(i, Changes.Get(i));
		}
	return ret;
}

void MStGr2StPos::Set(BStockGrID ind, unsigned short val) noexcept
{
	const BYTE i = BYTE(ind.Get());
	return Set(i, val);
}

unsigned short MStGr2StPos::Get(unsigned int i) const noexcept
{
	constexpr UINT64 maskF = 0xF;
	const UINT64 mask = maskF << i * 4;
	const unsigned short res = unsigned short((data_ & mask) >> (i * 4));
	return res;
}

