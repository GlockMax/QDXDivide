#include "stdafx.h"
#include "NSectionSetProg.h"

NSectionSetProg::NSectionSetProg(int PlanesNum) : NSectionSet(PlanesNum)
{
	Trimed = false;
}

NSectionSetProg::~NSectionSetProg(void)
{
}

void NSectionSetProg::DrawSections()
{
	NSectionSet::DrawSections();
}

int NSectionSetProg::SetNPlanes(int Num)
{
	NSectionSet::SetNPlanes(Num);
	return NPlanes;
}
