#include "stdafx.h"
#include "BObjectManager.h"
#include "NActiveObject.h"

IMPLEMENT_SERIAL(NActiveObject, NObject, 0)

NActiveObject::NActiveObject(void)
{
}

NActiveObject::~NActiveObject(void)
{
	P_OBJECT_MANAGER->DeleteActiveObject(this);
}

void NActiveObject::Disable(void)
{
	P_OBJECT_MANAGER->DeleteActiveObject(this);
}

PObjectStatus NActiveObject::OnStatusChanged(int ObjectInd, PObjectStatus NewStatus)
{
	// Returns status value to be placed to relation
	return NewStatus;
}
