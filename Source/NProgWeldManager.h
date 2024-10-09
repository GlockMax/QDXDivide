#pragma once
#include "nactiveobject.h"

class NProgWeldManager :
	public NActiveObject
{
public:
	NProgWeldManager(class CNCMDoc &Doc);
	virtual ~NProgWeldManager(void);
	virtual PObjectStatus OnStatusChanged(int ObjectInd, PObjectStatus NewStatus);
	int MakeAll(int ChangedObInd);

protected:
	class CNCMDoc &Doc;
};
