#pragma once
#include "NCMB_API.h"
#include "BPoint.h"

union PackedColor
{
	float fval;
	unsigned int ival;
	struct
	{
		unsigned char fr : sizeof(float) * 2;
		unsigned char fg : sizeof(float) * 2;
		unsigned char fb : sizeof(float) * 2;
		unsigned char fa : sizeof(float) * 2;
	};
};

class NCMB_API ClrPoint : public BPoint
{
public:
	ClrPoint(const float* base);
	ClrPoint(const BPoint& P);
	ClrPoint(const BPoint& P, PackedColor Col);
	void SetColor(float Val);
	void SetColor(PackedColor Val);
	PackedColor GetColor() const { return Color; }
protected:
	PackedColor Color;
};

