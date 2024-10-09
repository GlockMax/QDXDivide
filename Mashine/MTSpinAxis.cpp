#include "stdafx.h"
#include "BMatr.h"
#include "MTSpinAxis.h"

MTAxis::AXIS_TYPE MTSpinAxis::GetType() const
{
    return AT_SPIN;
}

MTSpinAxis* MTSpinAxis::Clone() const
{
	return new MTSpinAxis(*this);
}

void MTSpinAxis::InitVector(const BPoint& vector)
{
	MTAxis::InitVector(vector);
	// ensure VectorY is orthogonal to Vector
	if (BPoint(1., 0., 0., 0.).Angle0_180(GetVector()) > 1.)
		InitVectorY(GetVector() % BPoint(1., 0., 0., 0.));
	else
		InitVectorY(GetVector() % BPoint(0., 1., 0., 0.));
}

void MTSpinAxis::Draw(bool selected)
{
	if (!Visible)
		return;

	DrawArrow(GetVector(), NColor(1., 0., 0., 1.));

	if (selected)
		DrawGabar(GetVector());
}

BOOL MTSpinAxis::CreateMoveMatrixSt(BMatr& MoveMatrix, const BMatr& M, double p) const
{
	MoveMatrix.SetE();
	return FALSE;
}

BOOL MTSpinAxis::CreateDerivMoveMatrix(BMatr& MoveMatrix, const BMatr& M, double param, double derivparam) const//!!!
{
	MoveMatrix.Set(0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0);
	return FALSE;
}
BOOL MTSpinAxis::CreateInvrMoveMatrix(BMatr& MoveMatrix, const BMatr& M, double param) const//!!!
{
	MoveMatrix.SetE();
	return FALSE;
}
BOOL MTSpinAxis::CreateDerivInvrMoveMatrix(BMatr& MoveMatrix, const BMatr& M, double param, double derivparam) const//!!!
{
	MoveMatrix.Set(0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0);
	return FALSE;
}

