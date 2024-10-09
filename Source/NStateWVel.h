#pragma once
#include "NWeldParams.h"
#include "ncontrolpanel.h"

class NStateWVel :
	public NStateDouble
{
public:
	NStateWVel(NWeldParams &Par, int Ind, class NStateNote * pN);
	virtual ~NStateWVel(void);
	virtual CString GetValueString();
protected:
	NWeldParams &Params;
};
