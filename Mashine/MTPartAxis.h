#pragma once

#include "MTAxis.h"
#include "BPoint.h"	// Added by ClassView

class MTPartAxis : public MTAxis  
{
public:
	MTPartAxis();
	virtual ~MTPartAxis();
	MTPartAxis* Clone() const override;
	virtual AXIS_TYPE GetType() const override;
	BOOL CreateMoveMatrix(BMatr& MoveMatrix, double p) override;

	BOOL CreateMoveMatrixSt(BMatr &MoveMatrix, const BMatr &M, double p) const override;
	BOOL CreateDerivMoveMatrix(BMatr &MoveMatrix, const BMatr &M, double param, double derivparam) const override;//!!!
	BOOL CreateInvrMoveMatrix(BMatr &MoveMatrix, const BMatr &M, double param) const override;//!!!
	BOOL CreateDerivInvrMoveMatrix(BMatr &MoveMatrix, const BMatr &M, double param, double derivparam) const override;//!!!
};
