#pragma once

#include "MTRelocateAxis.h"

class MTRotaryAxis : public MTRelocateAxis
{
public:
	AXIS_TYPE GetType() const override;
	BOOL CreateMoveMatrix(BMatr &MoveMatrix, double angle) override;
	MTRotaryAxis();
	virtual ~MTRotaryAxis();
	void Draw(bool selected = false) override;
	MTRotaryAxis* Clone() const override;
	BOOL CreateMoveMatrixSt(BMatr &MoveMatrix, const BMatr &M, double angle) const override;//���������� �������!!!
	BOOL CreateDerivMoveMatrix(BMatr &MoveMatrix, const BMatr &M, double angle1, double angle2) const override;//���������� ����������� �������!!!
	BOOL CreateInvrMoveMatrix(BMatr &MoveMatrix, const BMatr &M, double angle) const override;//���������� �������� �������!!!
	BOOL CreateDerivInvrMoveMatrix(BMatr & MoveMatrix, const BMatr &M, double angle1 , double angle2) const override;////���������� ����������� �������� �������!!!
};
