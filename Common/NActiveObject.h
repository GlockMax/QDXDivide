#pragma once
#include "nobject.h"

class AFX_EXT_CLASS NActiveObject :
	public NObject
{
public:
	NActiveObject(void);
	~NActiveObject(void);
	void Disable(void);
	DECLARE_SERIAL(NActiveObject)
	virtual PObjectStatus OnStatusChanged(int ObjectInd, PObjectStatus NewStatus);
};
