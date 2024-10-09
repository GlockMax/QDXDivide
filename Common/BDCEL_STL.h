#pragma once
#include "bdcel_trm.h"

class NCMB_API BDCEL_STL :
	public BDCEL_TRM
{
public:
	BDCEL_STL(void);
	virtual ~BDCEL_STL(void);
	int MakeFromStl(const class BStlB &Stl);
};
