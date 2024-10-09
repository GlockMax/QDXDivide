#pragma once
#include "MTRelocateAxis.h"

class MTMatrFieldAxis :
    public MTRelocateAxis
{
public:
	AXIS_TYPE GetType() const override;
	void CreateFieldMatr(BMatr& FieldMatr, const MTAxis* pCurAxis) const;
	virtual void CreateFieldMatr(BMatr& FieldMatr, double T) const = 0;
	virtual double CalcT(const BPoint& P) const = 0;
	MTMatrFieldAxis();
	virtual ~MTMatrFieldAxis();
	BOOL CreateMoveMatrixSt(BMatr& MoveMatrix, const BMatr& M, double angle) const override;//Возвращаем матрицу!!!
	BOOL CreateDerivMoveMatrix(BMatr& MoveMatrix, const BMatr& M, double angle1, double angle2) const override;//Возвращаем производную матрицы!!!
	BOOL CreateInvrMoveMatrix(BMatr& MoveMatrix, const BMatr& M, double angle) const override;//Возвращаем обратную матрицу!!!
	BOOL CreateDerivInvrMoveMatrix(BMatr& MoveMatrix, const BMatr& M, double angle1, double angle2) const override;////Возвращаем производную обратной матрицы!!!
};

