#include "stdafx.h"
#include "MTMatrFieldAxis.h"
#include "stdafx.h"
#include "BMatr.h"
#include "MTMatrFieldAxis.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

MTMatrFieldAxis::MTMatrFieldAxis() : MTRelocateAxis()
{
	MoveMin = -1000;
	MoveMax = 1000;
}

MTAxis::AXIS_TYPE MTMatrFieldAxis::GetType() const
{
    return AT_EMPTY;
}

void MTMatrFieldAxis::CreateFieldMatr(BMatr& FieldMatr, const MTAxis* pCurAxis) const
{
	const double StartT = CalcT(pCurAxis->GetBasePointOr());
	BMatr StartM;
	CreateFieldMatr(StartM, StartT);
	BMatr EndM;
	CreateFieldMatr(EndM, StartT + Movement);
	FieldMatr = StartM.invr() * EndM;
}

MTMatrFieldAxis::~MTMatrFieldAxis()
{

}

BOOL MTMatrFieldAxis::CreateMoveMatrixSt(BMatr& MoveMatrix, const BMatr& M, double t) const
{
	return TRUE;
}

BOOL MTMatrFieldAxis::CreateDerivMoveMatrix(BMatr& MoveMatrix, const BMatr& M, double t1, double t2) const
{
	return TRUE;
}

BOOL MTMatrFieldAxis::CreateInvrMoveMatrix(BMatr& MoveMatrix, const BMatr& M, double t) const
{
	return TRUE;
}

BOOL MTMatrFieldAxis::CreateDerivInvrMoveMatrix(BMatr& MoveMatrix, const BMatr& M, double t1, double t2) const
{
	return TRUE;
}
