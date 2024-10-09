#include "stdafx.h"
#include "GBodyRender.h"
#include "GAnimateCadrMill.h"
#include "GAnimateCadrMillDX.h"
#include "GAnimateCadrRot.h"
#include "nanimstate.h"

NAnimState::NAnimState(GBodyRender *pR, const NToolCombined *pT, const NCadrGeom &CG, cadrID ID, cadrID IDhist, double t)
{
	Set(pR, pT, CG, ID, IDhist, t);
	CrAnimCadr = false;
}

void NAnimState::Set(GBodyRender *pR, const NToolCombined *pT, const NCadrGeom &CG, cadrID ID, cadrID IDhist, double t)
{
	Defined = true;
	pRender = pR;
	pTool = pT;
	CadrGeom = CG;
	CadrID = ID;
	CadrIDhist = IDhist;
	tPart = t;
	StockState = AT_UNDEF;
}

NAnimState::NAnimState(GBodyRender *pR, cadrID ID, cadrID IDhist)
{
	Set(pR, ID, IDhist);
	CrAnimCadr = false;
}

NAnimState::NAnimState(GBodyRender* pStartRender, NCMApplicationType SState)
{
	Defined = false;
	CrAnimCadr = true;
	pRender = pStartRender;
	StockState = SState;
	pTool = NULL;
	CadrID = cadrID(-1);
	CadrIDhist = cadrID(-1);
}

void NAnimState::Set(GBodyRender *pR, cadrID ID, cadrID IDhist)
{
	Defined = true;
	pRender = pR;
	pTool = NULL;
	CadrID = ID;
	CadrIDhist = IDhist;
	tPart = 1.;
	StockState = AT_UNDEF;
}

NAnimState::NAnimState(const NAnimState *pAnimState)
{
	if(!pAnimState)
	{
		Defined = false;
		return;
	}
	*this = *pAnimState;
}

NAnimState::~NAnimState(void)
{
}

const NToolCombined * NAnimState::GetTool(void) const
{
	return pTool;
}
cadrID NAnimState::GetCadrID(void) const
{
	return CadrID;
}
cadrID NAnimState::GetCadrIDhist(void) const
{
	return CadrIDhist;
}
void NAnimState::DeleteRender(void)
{
	delete pRender;
}