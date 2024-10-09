#pragma once
#include "MTRelocateAxis.h"

class MTSpinAxis : public MTRelocateAxis
{
public:
	AXIS_TYPE GetType() const override;
	void Draw(bool selected = false) override;
	MTSpinAxis* Clone() const override;
	void InitVector(const BPoint& vector) override;

	BOOL CreateMoveMatrixSt(BMatr& MoveMatrix, const BMatr& M, double p) const override;
	BOOL CreateDerivMoveMatrix(BMatr& MoveMatrix, const BMatr& M, double param, double derivparam) const override;//!!!
	BOOL CreateInvrMoveMatrix(BMatr& MoveMatrix, const BMatr& M, double param) const override;//!!!
	BOOL CreateDerivInvrMoveMatrix(BMatr& MoveMatrix, const BMatr& M, double param, double derivparam) const override;//!!!
};

