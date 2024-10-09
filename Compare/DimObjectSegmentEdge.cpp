// DimObjectSegmentEdge.cpp : implementation file
//

#include "stdafx.h"
#include "DimObjectSegmentEdge.h"


// DimObjectSegmentEdge

DimObjectSegmentEdge::~DimObjectSegmentEdge()
{
}


// DimObjectSegmentEdge member functions

void DimObjectSegmentEdge::Change(const BPoint& iPoint, const BPoint& iViewDir)
{
	BPoint pt = BPoint::ProjectPointOnPlane(iPoint, _oldPt, iViewDir);
	_oldPt = pt;
}
