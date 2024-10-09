#pragma once
#include "NCTOOL_API.h"
#include "gdynamiccadr.h"

class NCTOOL_API GReRenderCadr :
	public GDynamicCadr
{
public:
	GReRenderCadr(GBodyRender *piStart);
	virtual ~GReRenderCadr(void);
	virtual bool NextStep();
	bool CalcCurCoordDC(BPoint &XYZ, BPoint &ABC) override;
	bool CalcCurCoordMCS(BPoint &XYZ, BPoint &ABC) override;
	double GetCurParam(void) noexcept override { return -1.;}
protected:
	bool Set(const NCadrGeom & Geom, const BMatr &RotMatr, GBodyRender &End, double tPart = 1.) override;
//	GReRenderCadr(void);
	virtual void CalcCurAddCoord(SMachState &AddCoord, double t);
};
