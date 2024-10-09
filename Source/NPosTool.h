#pragma once
#include "RArrow.h"
#include "BMatr.h"

class NPosTool
{
public:
	NPosTool();
	~NPosTool(void);
	void Draw(RenderMode GlobRMode);
	void Activate(const BMatr &M);
	void DeActivate(void) { Active = false;}
	bool IsActive(void) const { return Active;}
	void SetStockGabar(const BBox &Gabar, const BMatr &ToolMatr);
protected:
	BMatr ReToolMatr;
	BBox ReStockGabar;
	RArrow PosToolZ;
	RArrow PosToolX;
	RArrow PosToolY;
	BMatr PosMatr;
	bool Active;
};

