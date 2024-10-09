#pragma once

#include "NCMB_API.h"
#include "NColor.h"

class NCMB_API BProgVisInterval
{
public:
	BProgVisInterval();
	BProgVisInterval(int S, int E, bool U = false, NColor Col = NColor());
	~BProgVisInterval();
	bool Inside(int Num) const;
	int &GetEnd() { return End; }
	int &GetStart() { return Start; }
	int GetEnd() const { return End; }
	int GetStart() const { return Start; }
	void JoinIntersected(const BProgVisInterval& Int);
	bool GetUseColor() const { return UseColor; }
	const NColor& GetColor() const { return Color; }
protected:
	int Start;
	int End;
	bool UseColor;
	NColor Color;
};

