#pragma once
#include "NCMB_API.h"
#include <vector>
#include "BStockGrID.h"

class NCMB_API MStGr2StPos
{
	friend class BMTLinkageDiff;
	friend class BMTLinkage;
public:
	MStGr2StPos() noexcept;
	bool operator != (const MStGr2StPos& in) const noexcept;
	void Clear() noexcept;
	unsigned short Get(BStockGrID ind) const noexcept;
	std::vector<BStockGrID> Find(unsigned short stPos) const;
	static const unsigned int POS_UNDEF = 0xF;// value for marking position as empty
	bool IsPosEmpty(BStockGrID ind) noexcept;
	bool ApplyChanges(const MStGr2StPos& Changes);
	void Set(BStockGrID ind, unsigned short val) noexcept;
protected:
	void Set(unsigned short i, unsigned short val) noexcept;
	unsigned short Get(unsigned int i) const noexcept;

protected:
	UINT64 data_;
	static const UINT64 init_data_ = 0xFEDCBA9876543210;
};
