#include "stdafx.h"
#include "DXSharpEdge.h"

unsigned int DXSharpEdge::MakeAdjData(unsigned int boundInd, DXCellInd& key) const
{
	auto adjBound = noBound_;
	key = key_;
	switch (boundInd)
	{
	case 0:
		adjBound = 5;
		key.ShiftZ(-1);
		break;
	case 1:
		adjBound = 3;
		key.ShiftY(-1);
		break;
	case 2:
		adjBound = 4;
		key.ShiftX(+1);
		break;
	case 3:
		adjBound = 1;
		key.ShiftY(+1);
		break;
	case 4:
		adjBound = 2;
		key.ShiftX(-1);
		break;
	case 5:
		adjBound = 0;
		key.ShiftZ(+1);
		break;
	case noBound_:
	default:
		adjBound = noBound_;
		break;
	}
	return adjBound;
}
