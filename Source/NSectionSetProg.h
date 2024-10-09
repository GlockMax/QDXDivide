#pragma once
#include "nsectionset.h"

class NSectionSetProg :
	public NSectionSet
{
public:
	NSectionSetProg(int PlanesNum = 0);
	virtual ~NSectionSetProg(void);
	void DrawSections();
	virtual int SetNPlanes(int Num);

protected:
	int *SectStart;
	bool *SectVis;

};
