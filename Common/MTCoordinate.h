// MTCoordinate.h: interface for the MTCoordinate class.
//
//////////////////////////////////////////////////////////////////////
#pragma once
#include "NCMB_API.h"

class NCMB_API MTCoordinate
{
public:
	double Value;
	CString AxisName;
	MTCoordinate();
	MTCoordinate(const CString& AN, double V);
	void Set(const CString& AN, double V);
	~MTCoordinate();

};

class NCMB_API CooArray : public CArray<MTCoordinate, const MTCoordinate&>
{
public:
	CooArray(const CooArray& In);
	CooArray();
	const CooArray& operator=(const CooArray& In);
};