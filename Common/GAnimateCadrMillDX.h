#pragma once
#include "NCTOOL_API.h"
#include "GAnimateCadrMill.h"

class NCTOOL_API GAnimateCadrMillDX : public GAnimateCadrMill
{
public:
	GAnimateCadrMillDX(class GBodyRender *piStart, const NSpeedPar* pPar);
	GAnimateCadrMillDX(const GAnimateCadr & In);
//	GAnimateCadrMillDX(void);
	~GAnimateCadrMillDX();
	bool NextStep(void) override;
protected:
	bool Set(const NCadrGeom & Geom, const BMatr &RotMatr, GBodyRender &End, double tPart = 1.) override;

};

