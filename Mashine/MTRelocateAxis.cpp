#include "stdafx.h"
#include "MTRelocateAxis.h"


MTRelocateAxis::MTRelocateAxis() : MTAxis()
{
}


MTRelocateAxis::~MTRelocateAxis()
{
}

void MTRelocateAxis::SetSpecPtsVal(int ind, double val)
{
	auto PrevSize = SpecialPts.size();
	if (ind >= PrevSize)
	{
		SpecialPts.resize(ind + 1);
		for (auto i = PrevSize; i < SpecialPts.size(); ++i)
			SpecialPts[i] = 0.;
	}
	SpecialPts[ind] = val;
}

double MTRelocateAxis::GetSpecPtsVal(int ind) const
{
	if (ind < 0 || ind >= SpecialPts.size())
		return 0.;
	return SpecialPts[ind];
}

size_t MTRelocateAxis::GetSpecPtsSize() const
{
	return SpecialPts.size();
}

MTRelocateAxis::MTRelocateAxis(const MTRelocateAxis& Ax)
{
	*this = Ax;
}

const MTRelocateAxis& MTRelocateAxis::operator=(const MTRelocateAxis& Ax)
{
	MTAxis::operator=(Ax);
	SpecialPts = Ax.SpecialPts;
	return *this;
}
