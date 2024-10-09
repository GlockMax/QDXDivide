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
	BOOL CreateMoveMatrixSt(BMatr &MoveMatrix, const BMatr &M, double distance) const override;//Возвращаем матрицу//!!!
	BOOL CreateDerivMoveMatrix(BMatr & MoveMatrix, const BMatr &M, double param, double derivparam) const override;//Возвращаем производную матрицы//!!!
	BOOL CreateInvrMoveMatrix(BMatr & MoveMatrix, const BMatr &M, double param) const override;//Возвращаем обратную матрицу//!!!
	BOOL CreateDerivInvrMoveMatrix(BMatr & MoveMatrix, const BMatr &M, double param, double derivparam) const override;//Возвращаем производную обратной матрицы//!!!
};
