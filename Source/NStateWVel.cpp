#include "stdafx.h"
#include "NStateWVel.h"

NStateWVel::NStateWVel(NWeldParams &Par, int Ind, NStateNote * pN)
: Params(Par), NStateDouble(Par.GetParPtr(Ind), pN)
{
}

NStateWVel::~NStateWVel(void)
{
}
CString NStateWVel::GetValueString()
{
	Params.EnsureActual();
	return NStateDouble::GetValueString();
}
