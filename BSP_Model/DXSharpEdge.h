#pragma once
#include "DXMap.h"
class DXSharpEdge
{
public:
	static constexpr unsigned int noBound_ = 6;
	float* startPt_;// pointer to the start point
	float* endPt_;// pointer to  the end point
	union ShInds
	{
		_int8 Val;
		struct
		{
			unsigned int start_ : 3;// number of bound for start edge point (0-5) 6(noBound_) if no bound
			unsigned int end_ : 3;// number of bound for end edge point (0-5) 6(noBound_) if no bound
		};
	} boundInds_;
	DXCellInd key_;
	unsigned int boundMapIndStart_;
	unsigned int boundMapIndEnd_;
	unsigned int edgeInd_;
public:
	DXSharpEdge(unsigned int boundMapIndStart, unsigned int boundMapIndEnd, const DXCellInd& key, unsigned int startBound, unsigned int endBound)
		: key_(key)
	{
		boundMapIndStart_ = boundMapIndStart; boundMapIndEnd_ = boundMapIndEnd; boundInds_.start_ = startBound; boundInds_.end_ = endBound; edgeInd_ = UINT_MAX;
	}
	unsigned int MakeAdjData(unsigned int boundInd, DXCellInd& key) const;
	void Invert() {	auto buf = boundMapIndStart_; boundMapIndStart_ = boundMapIndEnd_; boundMapIndEnd_ = buf;
		auto buf1 = boundInds_.start_; boundInds_.start_ = boundInds_.end_; boundInds_.end_ = buf1;
		auto buf2 = startPt_; startPt_ = endPt_; endPt_ = buf2; }
};

