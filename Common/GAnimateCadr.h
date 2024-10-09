#pragma once
#include "NCTOOL_API.h"
#include "GDynamicCadr.h"
#include "NCadrGeom.h"
#include "NCadrSpeedPar.h"
#include "GQuadRender.h"

class NCTOOL_API GAnimateCadr : public GDynamicCadr
{
public:
	GAnimateCadr(GBodyRender *piStart, const NSpeedPar* pPar);
	GAnimateCadr(const GAnimateCadr & In);
	virtual ~GAnimateCadr(void);
	void Init() override { CurStep = 0; CurTVal = 0; CurTime = 0; }

protected:
	GLdouble plane[4];
	GLdouble plane1[4];// For a large arcs only
	int StepsNum;
	int CurStep;
	double CurTVal;
	MainTime CurTime;
	const class NCollisionID* pColl;
	double Surplus;// The rest of the LengthStep not processed in the previous cadr
	bool LineType;
	bool LargeArc;
	double tPart;
	const NSpeedPar* pSpeedPar;
	bool Set(const NCadrGeom & Geom, const BMatr &RotMatr, GBodyRender &End, double tPart) override = 0;
public:
	virtual bool NextStep(void) = 0;
	double GetTimeS(void) const;
	double GetGeomT() const;
	bool CalcCurCoordDC(BPoint &XYZ, BPoint &ABC) override;
	bool CalcCurCoordMCS(BPoint &XYZ, BPoint &ABC) override;
	double GetCurParam(void) noexcept override;
	void CalcPlane4Rotate(void);
	void RenderTop(RenderMode GlobRMode, int Type, const std::vector<class GAnimateCadrMill*>& DynBSPCadrs, bool Collide);
	void SetStart(GBodyRender *pS);
	void Restart(void);
	void RestartFrom(GAnimateCadr& In);
	void Recalc(void);
	virtual double GetCurLengthStep() const = 0;
	const class NCollisionID* GetCollID() const { return pColl; }
protected:
	virtual void CalcCurAddCoord(SMachState &AddCoord, double t);
	double GetCadrLength(const NCadrGeom &InGeom) const;
	void IncrCurTVal();
	void RecalcCurTVal();
	void PlanesRenderOne(GBodyRender* pStart, GBodyRender* pEnd, RenderMode GlobRMode, int Parm);
	void RenderWAllData(const std::vector<class GAnimateCadrMill*>& DynCadrs
		, const std::vector<GQuadRenderMill*>& MillBSPRenders
		, const std::vector<std::vector<GQuadRenderMill*>>& EndsForAllCadrs
		, enum RenderMode GlobRMode, int Type);
};
