#pragma once
#include <vector>
#include "MTAxis.h"

class MTRelocateAxis :
	public MTAxis
{
	friend class MTEdit;
	friend class MTEditPane;
public:
	MTRelocateAxis();
	virtual ~MTRelocateAxis();
	MTRelocateAxis(const MTRelocateAxis& Ax);
	const MTRelocateAxis& operator= (const MTRelocateAxis& Ax);
	void SetSpecPtsVal(int ind, double val);
	double GetSpecPtsVal(int ind) const;
	size_t GetSpecPtsSize() const;
protected:
	std::vector <double> SpecialPts;
};

