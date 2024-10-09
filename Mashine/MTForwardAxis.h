// MTForwardAxis.h: interface for the MTForwardAxis class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "MTRelocateAxis.h"

class MTForwardAxis : public MTRelocateAxis  
{
public:
	AXIS_TYPE GetType() const override;
	BOOL CreateMoveMatrix(BMatr &MoveMatrix,double distance) override;
	MTForwardAxis();
	virtual ~MTForwardAxis();
	MTForwardAxis* Clone() const override;
	void Draw(bool selected = false) override;
	BOOL CreateMoveMatrixSt(BMatr &MoveMatrix, const BMatr &M, double distance) const override;//���������� �������//!!!
	BOOL CreateDerivMoveMatrix(BMatr & MoveMatrix, const BMatr &M, double param, double derivparam) const override;//���������� ����������� �������//!!!
	BOOL CreateInvrMoveMatrix(BMatr & MoveMatrix, const BMatr &M, double param) const override;//���������� �������� �������//!!!
	BOOL CreateDerivInvrMoveMatrix(BMatr & MoveMatrix, const BMatr &M, double param, double derivparam) const override;//���������� ����������� �������� �������//!!!
};
