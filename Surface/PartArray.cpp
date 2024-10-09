#include "stdafx.h"
#include "PartArray.h"
#include "NpartSurface.h"

void PartArray::ClearAll()
{
	for (int i=0; i<GetSize(); i++)
		delete GetAt(i);
}
