// MTForwardAxis.cpp: implementation of the MTForwardAxis class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "MachineTool.h"
#include "MTForwardAxis.h"
#include "BMatr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MTForwardAxis::MTForwardAxis():MTRelocateAxis()
{
	MoveMin = -500.;
	MoveMax = 500.;
}

MTForwardAxis::~MTForwardAxis()
{

}

MTForwardAxis* MTForwardAxis::Clone() const
{
	return new MTForwardAxis(*this);
}

BOOL MTForwardAxis::CreateMoveMatrix(BMatr &MoveMatrix, double distance)
{	
	
	BOOL Result = MTAxis::CreateMoveMatrix(MoveMatrix, distance);

	MoveMatrix = MoveMatrix * BMatr().Trans(GetVector() * distance);

	return Result;
}

MTAxis::AXIS_TYPE MTForwardAxis::GetType() const
{
	return AT_FOR;
}
BOOL MTForwardAxis::CreateMoveMatrixSt(BMatr &MoveMatrix, const BMatr &M, double distance) const
{
	BPoint V = GetVector() * M;
	//BPoint P = BasePoint * M;
	MoveMatrix.Trans(V.GetX()*distance,V.GetY()*distance,V.GetZ()*distance);
	return TRUE;
}
BOOL MTForwardAxis::CreateDerivMoveMatrix(BMatr & MoveMatrix, const BMatr &M, double param, double derivparam) const//!!!
{
	BPoint P;
	MoveMatrix.Set0();
	BPoint V = GetVector() * M;
	//BPoint P = BasePoint * M;
	P.Set(V.GetX()*derivparam, V.GetY()*derivparam, V.GetZ()*derivparam, 0.);
	MoveMatrix.SetRow(3, P);
	return TRUE;
}

BOOL MTForwardAxis::CreateInvrMoveMatrix(BMatr & MoveMatrix, const BMatr &M, double param) const//!!!
{
	CreateMoveMatrixSt(MoveMatrix, M, param);
	MoveMatrix = MoveMatrix.invr();
	return TRUE;
}

BOOL MTForwardAxis::CreateDerivInvrMoveMatrix(BMatr & MoveMatrix, const BMatr &M, double param, double derivparam) const//!!!
{
	//(Aобр.)'=-Aобр.*A'*Aобр.
	BMatr F, A, Invr;//Aобр.= Invr, A' = A, -E = M.
	F.Set(-1., 0., 0., 0.,
			0., -1., 0., 0.,
			0., 0., -1., 0.,
			0., 0., 0., -1.);
	CreateMoveMatrixSt(A, M, param);
	Invr = A.invr();// Invr = A.T();
	CreateDerivMoveMatrix(A, M, param, derivparam);
	MoveMatrix = F * Invr * A * Invr;
	return TRUE;
}

void MTForwardAxis::Draw(bool selected)
{
	if (!Visible)
		return;

	DrawArrow(GetVector(), NColor(1., 0., 0., 1.));

	if (selected)
		DrawGabar(GetVector());
}
