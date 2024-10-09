#pragma once

#include "BPoint.h"
#include "NColor.h"
enum SectArrType
{
	SAT_CIRCLE,
	SAT_LINE
};

class AFX_EXT_CLASS NSectArrParams
{
public:
	NSectArrParams(void);
	~NSectArrParams(void);
	NSectArrParams &SetDefault(SectArrType iType);
public:
	SectArrType Type;
	BPoint P;
	BPoint V;
	BPoint StartDir;
	double StartVal;
	double EndVal;
	int IntervalsNum;
	double LineThickness;
	bool Default;
	bool ColorByPart;
	NColor PartColor;
	NColor Color;
};
