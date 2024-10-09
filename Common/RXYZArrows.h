#pragma once
#include "OPENGLRENDER_API.h"
#include "RArrow.h"
#include "BMatr.h"

class OPENGLRENDER_API RXYZArrows
{
public:
	RXYZArrows();
	~RXYZArrows();
	void Draw(const class NColor &Col);
protected:
	RArrow ArrowZ;
	RArrow ArrowX;
	RArrow ArrowY;
	BMatr PosMatr;
	bool Active;

};

