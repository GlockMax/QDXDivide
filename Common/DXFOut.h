// DXFOut.h: interface for the DXFOut class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "NCadrGeomArray.h"

class AFX_EXT_CLASS DXFOut
{
public:
	DXFOut(const NCadrGeomArray * InArray, const CString& DefaultName = CString());
	~DXFOut();

protected:
	NCadrGeomArray * pCadrGeomArray;
protected:
	DXFOut();
};
