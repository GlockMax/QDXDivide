#pragma once
#include "NCMB_API.h"
#include "BPoint.h"

class NCMB_API BExtrFinder
{
public:
	BExtrFinder(void);
	~BExtrFinder(void);
public:
	void Start(BPoint &P);
	bool NewPoint(BPoint &P); // Returns true if extremum found. Can change P.
protected:
	BPoint StartP;
	BPoint LastP;
	BPoint PlaneN;
	bool PlaneNDefined;
};

