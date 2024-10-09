// MTRotaryAxis.cpp: implementation of the MTRotaryAxis class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BMatr.h"
#include "MTRotaryAxis.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MTRotaryAxis::MTRotaryAxis():MTRelocateAxis()
{
	MoveMin = -360;
	MoveMax = 360;
}

MTRotaryAxis::~MTRotaryAxis()
{
}

MTRotaryAxis* MTRotaryAxis::Clone() const
{
	return new MTRotaryAxis(*this);
}

BOOL MTRotaryAxis::CreateMoveMatrix(BMatr &MoveMatrix, double angle)
{
	BOOL Result = MTAxis::CreateMoveMatrix(MoveMatrix, angle);

	MoveMatrix = MoveMatrix * BMatr().rotg(angle, GetBasePoint(), GetVector());

	return Result;
}

MTAxis::AXIS_TYPE MTRotaryAxis::GetType() const
{
	return AT_ROT;
}
BOOL MTRotaryAxis::CreateMoveMatrixSt(BMatr &MoveMatrix, const BMatr &M, double angle) const
{
	BPoint V = GetVector() * M;
	BPoint P = GetBasePoint() * M;
	MoveMatrix.rotg(angle,P,V);
	return TRUE;
}
BOOL MTRotaryAxis::CreateDerivMoveMatrix(BMatr &MoveMatrix, const BMatr &M, double angle1, double angle2) const//!!!
{
	MoveMatrix.Set0();
	BPoint V = GetVector() * M;
	BPoint P = GetBasePoint() * M;
	MoveMatrix.drotg(angle1, angle2, P, V);
	return TRUE;
}

BOOL MTRotaryAxis::CreateInvrMoveMatrix(BMatr & MoveMatrix, const BMatr &M, double angle) const//!!!
{
	CreateMoveMatrixSt(MoveMatrix, M, angle);
	MoveMatrix = MoveMatrix.invr();
	return TRUE;
}

BOOL MTRotaryAxis::CreateDerivInvrMoveMatrix(BMatr & MoveMatrix, const BMatr &M, double angle1 , double angle2) const
{
	//(Aобр.)'=-Aобр.*A'*Aобр.
	BMatr F, A, Invr;//Aобр.= Invr, A' = A, -E = F.
	F.Set(-1., 0., 0., 0.,
			0., -1., 0., 0.,
			0., 0., -1., 0.,
			0., 0., 0., -1.);
	CreateMoveMatrixSt(A, M, angle1);
	Invr = A.invr();// Invr = A.T();
	CreateDerivMoveMatrix(A, M, angle1, angle2);
	MoveMatrix = F * Invr * A * Invr;
	return TRUE;
}

void MTRotaryAxis::Draw(bool selected)
{
	if (!Visible)
		return;

	DrawArrow(GetVector(), NColor(1., 0., 0., 1.));

	if (selected)
		DrawGabar(GetVector());
}

