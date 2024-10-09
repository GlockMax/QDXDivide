#pragma once
#include "NRendersList.h"

class NQueue;
class GAnimateCadr;

class AFX_EXT_CLASS GPauseAnimate
{
public:
	GPauseAnimate(NQueue &StartQueue, NQueueLim &EndQueue, GAnimateCadr *pAnimCadr);
	~GPauseAnimate(void);

protected:
	GPauseAnimate(void);
	NRendersList RendersList;
	int Steps;
	GAnimateCadr *pPrevAnimCadr;
	GAnimateCadr *pCurAnimCadr;
	NAnimState *pCurAnimState;
	bool VeryFirstStep;
public:
	int StartNewStep(bool forward, int steps_num);
	GAnimateCadr *GetPrevAnimCadr();
	GAnimateCadr *GetCurAnimCadr();
	bool NextStep();
	const NAnimState *GetCurAnimState();
	void GoToStartPos();
	static GBodyRender * CopyRender(const GBodyRender *pInRender);
};
