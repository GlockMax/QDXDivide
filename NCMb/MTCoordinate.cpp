#include "..\Common\MTCoordinate.h"
#include "..\Common\MTCoordinate.h"
// MTCoordinate.cpp: implementation of the MTCoordinate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MTCoordinate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MTCoordinate::MTCoordinate()
{
	Value=0.;
}
MTCoordinate::MTCoordinate(	const CString& AN, double V)
{
	Set(AN, V);
}

void MTCoordinate::Set(	const CString& AN, double V)
{
	AxisName = AN;
	Value = V;
}

MTCoordinate::~MTCoordinate()
{

}

CooArray::CooArray(const CooArray& In)
{
	SetSize(In.GetSize());
	for (int i = 0; i < GetSize(); ++i)
		GetAt(i) = In.GetAt(i);
}

CooArray::CooArray() : CArray<MTCoordinate, const MTCoordinate&>()
{
}

const CooArray& CooArray::operator=(const CooArray& In)
{
	SetSize(In.GetSize());
	for (int i = 0; i < GetSize(); ++i)
		GetAt(i) = In.GetAt(i);
	return *this;
}
