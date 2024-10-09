#pragma once
#include "stdafx.h"
#include "DXDirection5X.h"
#include "DXEdge.h" 
	
DXEdge::DXEdge(bool i, ElemID Surf, int BufInd, unsigned int DegradedEdge)
{
	SurfID = Surf;
	Rout = i;
	Degraded = DegradedEdge;
	IndexBuf = BufInd;
}

DXEdge::~DXEdge()
{
	
}

void DXEdge::Set(bool i, ElemID Surf, int BufInd, unsigned int DegradedEdge)
{
	SurfID = Surf;
	Rout = i;
	Degraded = DegradedEdge;
	IndexBuf = BufInd;
}

