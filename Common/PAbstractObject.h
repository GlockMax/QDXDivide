#pragma once
#include "NObjEnums.h"
class AFX_EXT_CLASS PAbstractObject
{
public:
	PAbstractObject(void){};
	~PAbstractObject(void){};
	virtual class PropList * Create(class PropList *) = 0;
	virtual class PropList * Change(class PropList *, class PropSingle *, int, enum UpdateModes &AdditionalModes, int &Group) = 0;
};
