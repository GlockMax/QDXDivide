// MRay.h: interface for the MRay class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "BSP_MODEL_API.h"
#include "MSLine.h"

class BSP_MODEL_API MRay  : public MSLine
{
public:
	BPoint IntersPlane(const MPlane &Pl) const;
	MRay();
	MRay( const BPoint & P, const BPoint & V);
	virtual ~MRay();
protected:

public:
	// returns -1 point lie in the negative s/s; +1 - in the positive; 0 - on the base point plane
	int PointDirTest(const BPoint & P);
};
