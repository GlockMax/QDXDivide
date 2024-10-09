#include "..\Common\BMTLinkage.h"
#include "stdafx.h"
#include "BMTLinkage.h"

BMTLinkage::BMTLinkage() noexcept
{
	confInd_ = 0;
}

void BMTLinkage::Clear() noexcept
{
	confInd_ = 0;
	stGr2StPos_.Clear();
}

void BMTLinkage::SetConfInd(unsigned short ind) noexcept
{
	confInd_ = ind;
}

const BMTLinkage& BMTLinkage::SetIndW1() noexcept
{
	confInd_ = W1_VAL;
	return *this;
}

const BMTLinkage& BMTLinkage::SetIndW2() noexcept
{
	confInd_ = W2_VAL;
	return *this;
}

bool BMTLinkage::IsIndW() const noexcept
{
	return IsIndW1() || IsIndW2();
}

bool BMTLinkage::IsIndW1() const noexcept
{
	return confInd_ == W1_VAL;
}

bool BMTLinkage::IsIndW2() const noexcept
{
	return confInd_ == W2_VAL;
}

bool BMTLinkage::operator!=(const BMTLinkage& in) const noexcept
{
	return confInd_ != in.confInd_ || stGr2StPos_ != in.stGr2StPos_;
}

bool BMTLinkage::ApplyChanges(const BMTLinkageDiff& Changes)
{
	if (Changes.IsZero())
		return false;
	// returns true if some changes take place
	bool ret = false;
	if (!Changes.IsIndW())
	{
		SetConfInd(Changes.GetConfInd());
		ret = true;
	}
	ret |= stGr2StPos_.ApplyChanges(Changes.GetStGr2StPos());
	return ret;
}

BMTLinkageDiff::BMTLinkageDiff()
{
	Clear();
}

bool BMTLinkageDiff::IsZero() const
{
	return IsIndW() && GetStGr2StPos().data_ == 0xFFFFFFFFFFFFFFFF;
}

void BMTLinkageDiff::Clear()
{
	confInd_ = W1_VAL;
	for (unsigned short i = 0; i < 16; ++i)
		stGr2StPos_.Set(i, MStGr2StPos::POS_UNDEF);
}

BMTLinkageDiff BMTLinkage::operator-(const BMTLinkage& in) const
{
	BMTLinkageDiff res;
	if (confInd_ != in.confInd_)
		res.confInd_ = confInd_;
	for (unsigned int i = 0; i < 16; ++i)
		if (in.GetStGr2StPos().Get(i) != GetStGr2StPos().Get(i))
			res.GetStGr2StPos().Set(i, GetStGr2StPos().Get(i));
	return res;
}

