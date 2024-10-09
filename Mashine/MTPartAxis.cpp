// MTPartAxis.cpp: implementation of the MTPartAxis class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MTPartAxis.h"
#include "BMatr.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MTPartAxis::MTPartAxis() : MTAxis()
{
}

MTPartAxis::~MTPartAxis()
{

}

MTPartAxis* MTPartAxis::Clone() const
{
	return new MTPartAxis(*this);
}

MTAxis::AXIS_TYPE MTPartAxis::GetType() const
{
	return AT_PART;
}

BOOL MTPartAxis::CreateMoveMatrixSt(BMatr &MoveMatrix, const BMatr &M, double p) const
{
	MoveMatrix.SetE();
	return FALSE;
}

BOOL MTPartAxis::CreateDerivMoveMatrix(BMatr &MoveMatrix, const BMatr &M, double param, double derivparam) const//!!!
{
	MoveMatrix.Set(0.0, 0.0, 0.0, 0.0,
					0.0, 0.0, 0.0, 0.0,
					0.0, 0.0, 0.0, 0.0,
					0.0, 0.0, 0.0, 0.0);
	return FALSE;
}
BOOL MTPartAxis::CreateInvrMoveMatrix(BMatr &MoveMatrix, const BMatr &M, double param) const//!!!
{
	MoveMatrix.SetE();
	return FALSE;
}
BOOL MTPartAxis::CreateDerivInvrMoveMatrix(BMatr &MoveMatrix, const BMatr &M, double param, double derivparam) const//!!!
{
	MoveMatrix.Set(0.0, 0.0, 0.0, 0.0,
					0.0, 0.0, 0.0, 0.0,
					0.0, 0.0, 0.0, 0.0,
					0.0, 0.0, 0.0, 0.0);
	return FALSE;
}

BOOL MTPartAxis::CreateMoveMatrix(BMatr& MoveMatrix, double p)
{
	if (IsCS)
		MoveMatrix = GetMatr();
	else
		MoveMatrix.SetE();

	return TRUE;
}

