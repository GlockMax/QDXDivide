#pragma once
#include "NCTOOL_API.h"
#include "NCadrSpeedPar.h"
#include "GAnimateCadr.h"

class NCTOOL_API GAnimateCadrMill :
	public GAnimateCadr
{
public:
	GAnimateCadrMill(GBodyRender *piStart, const NSpeedPar* pPar);
	GAnimateCadrMill(const GAnimateCadr & In);
	GAnimateCadrMill(void);
	virtual ~GAnimateCadrMill(void);
	bool NextStep(void) override;
	double GetCurLengthStep() const override { return pSpeedPar->GetCurLengthStepMill(Geom);}

protected:
	virtual bool Set(const NCadrGeom & Geom, const BMatr &RotMatr, GBodyRender &End, double tPart = 1.) override;
	virtual void CalcCurAddCoord(SMachState &AddCoord, double t);
	void CalcNextPlane(void);
	bool CalcPlanes();
};
