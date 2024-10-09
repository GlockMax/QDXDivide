#pragma once

class NPartSurface;

typedef CTypedPtrArray <CPtrArray,NPartSurface*> PartArr;

class AFX_EXT_CLASS PartArray :public PartArr
{

public:
	void ClearAll();
};