#include "stdafx.h"
#include "GAnimateCadrMill.h"
#include "GAnimateCadrRot.h"
#include "GQuadRenderRot.h"
#include "GQuadRenderMill.h"
#include "GQuadRenderDX5X.h"
#include "gpauseanimate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

GPauseAnimate::GPauseAnimate(void)
{
	VeryFirstStep = true;
}
GPauseAnimate::GPauseAnimate(NQueue &StartQueue, NQueueLim &EndQueue, GAnimateCadr *pAnimCadr)
{
	VeryFirstStep = true;
	pPrevAnimCadr = pAnimCadr;
	pCurAnimState = NULL;
	Steps = 0;
	if(pPrevAnimCadr)
	{
		pCurAnimCadr = new GAnimateCadrMill(*pPrevAnimCadr);
		pCurAnimCadr->SetStart(CopyRender(pPrevAnimCadr->GetStart()));
	}
	else
	{
		pCurAnimCadr = NULL;
	}
	RendersList.Create(StartQueue, EndQueue);
}

GBodyRender * GPauseAnimate::CopyRender(const GBodyRender *pInRender)
{
	switch(NCM_PROJECT.GetGlobalState().GetModelMode())
	{
	case NGlobalState::BSP:
		//return new GQuadRenderMill(*(GQuadRenderMill *)pInRender);
		return GQuadRenderMill::CreateListCopy( *(GQuadRenderMill *)pInRender );
	case NGlobalState::DX:
		//return new GQuadRenderDX5X(*(GQuadRenderDX5X *)pInRender);
		return GQuadRenderDX5X::CreateListCopy( *(GQuadRenderDX5X *)pInRender );
	default:
		return NULL;
	}
}

GPauseAnimate::~GPauseAnimate(void)
{
	pCurAnimCadr->Restart();
//	delete pCurAnimCadr;
}

int GPauseAnimate::StartNewStep(bool forward, int steps_num)
{
	Steps = steps_num * (forward ? 1 : -1);
	return 0;
}
GAnimateCadr *GPauseAnimate::GetPrevAnimCadr()
{
	return pPrevAnimCadr;
}
GAnimateCadr *GPauseAnimate::GetCurAnimCadr()
{
	return pCurAnimCadr;
}
bool GPauseAnimate::NextStep()
{
	if(Steps == 0)
		return false;
	if(VeryFirstStep)
	{
		VeryFirstStep = false;
		if(Steps > 0)
			return true;
		pCurAnimCadr->Restart();
	}
	cadrID PrevCadrIDhist = 0;
	if (pCurAnimState)
		PrevCadrIDhist = pCurAnimState->GetCadrIDhist();
	pCurAnimState = RendersList.GetNext(Steps > 0);
	if(!pCurAnimState)
		return false;
	pCurAnimCadr->Restart();

	bool SetFlag;
	SetFlag = pCurAnimCadr->SetA(*pCurAnimState);
	
	if (PrevCadrIDhist != pCurAnimState->GetCadrIDhist())// For DX only
		Steps = Steps - ((Steps > 0) ? 1 : -1);
	return true;
}
const NAnimState *GPauseAnimate::GetCurAnimState()
{
	return pCurAnimState;
}
void GPauseAnimate::GoToStartPos()
{
	int Diff = RendersList.GetCurPosDiff();
	int step_sign = (Diff > 0) ? -1 : 1;
	for(; Diff != 0; Diff += step_sign)
	{
		Steps = step_sign;
		NextStep();
	}
}
