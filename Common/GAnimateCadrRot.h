#pragma once
#include "NCTOOL_API.h"
#include "NTiParams.h"
#include "NCadrSpeedPar.h"
#include "ganimatecadr.h"

class NCTOOL_API GAnimateCadrRot :
	public GAnimateCadr
{
public:
	GAnimateCadrRot(GBodyRender *piStart, const NSpeedPar* pPar);
	GAnimateCadrRot(const GAnimateCadr & In);
//	GAnimateCadrRot(void);
	virtual ~GAnimateCadrRot(void);
	bool NextStep(void) override;
	double GetCurLengthStep() const { return pSpeedPar->GetCurLengthStepRot(Geom); }
	virtual double GetCurParam(void) noexcept override ;
	virtual void CalcCurAddCoord(SMachState &AddCoord, double t);
protected:
	bool Set(const NCadrGeom & Geom, const BMatr &RotMatr, GBodyRender &End, double tPart) override;
};
