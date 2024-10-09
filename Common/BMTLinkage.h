#pragma once
#include "NCMB_API.h"
#include "MStGr2StPos.h"

class NCMB_API BMTLinkage
{
public:
	BMTLinkage() noexcept;
	unsigned short GetConfInd() const  noexcept { return confInd_; }
	void Clear() noexcept;
	void SetConfInd(unsigned short ind) noexcept;
	const BMTLinkage& SetIndW1() noexcept;
	const BMTLinkage& SetIndW2() noexcept;
	bool IsIndW() const noexcept;
	bool IsIndW1() const noexcept;
	bool IsIndW2() const noexcept;
	bool operator != (const BMTLinkage& in) const noexcept;
	const MStGr2StPos& GetStGr2StPos() const  noexcept { return stGr2StPos_; }
	MStGr2StPos& GetStGr2StPos()  noexcept { return stGr2StPos_; }
	bool ApplyChanges(const BMTLinkageDiff& Changes);

	BMTLinkageDiff operator - (const BMTLinkage& in) const;
protected:
	unsigned short confInd_;
	MStGr2StPos stGr2StPos_;
	static const unsigned short W1_VAL = UINT16_MAX;
	static const unsigned short W2_VAL = UINT16_MAX - 1;
};

class NCMB_API BMTLinkageDiff : public BMTLinkage
{
public:
	BMTLinkageDiff();
	bool IsZero() const;
	void Clear();
};

