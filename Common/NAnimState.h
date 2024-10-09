#pragma once
#include "BSP_MODEL_API.h"
#include "cadrID.h"
#include "NCMComData.h"
#include "NCadrGeom.h"
class GBodyRender;
class NToolCombined;


class BSP_MODEL_API NAnimState
{
public:
	NAnimState(GBodyRender *pRender, const NToolCombined *pTool, const NCadrGeom &CadrGeom, cadrID ID, cadrID IDhist, double tPart = 1.);
	NAnimState(GBodyRender* pR, cadrID ID, cadrID IDhist);
	NAnimState(GBodyRender* pStartRender, NCMApplicationType SState);
	void Set(GBodyRender *pRender, const NToolCombined *pTool, const NCadrGeom &CadrGeom, cadrID ID, cadrID IDhist, double tPart = 1.);
	void Set(GBodyRender *pR, cadrID ID, cadrID IDhist);
	NAnimState(const NAnimState *pAnimState);
	virtual ~NAnimState(void);
	const NToolCombined * GetTool(void) const;
	cadrID GetCadrID(void) const;
	cadrID GetCadrIDhist(void) const;
	bool IsDefined() { return Defined;}
	void DeleteRender(void);
	GBodyRender& GetRender(void) const { return *pRender; }
	GBodyRender* GetpRender(void) const { return pRender; }
	auto GetStockState() const noexcept { return StockState; }
	bool GetCrAnimCadr() const noexcept { return CrAnimCadr; }
	void NULLRender() noexcept { pRender = nullptr; }

protected:
	bool Defined;
	cadrID CadrID;
	cadrID CadrIDhist;
	const NToolCombined *pTool;
	GBodyRender* pRender;
	bool CrAnimCadr;
	NCMApplicationType StockState;
public:
	double tPart; // The part of original cadr for turn processing and movement direction for mill processing
	NCadrGeom CadrGeom;
};
