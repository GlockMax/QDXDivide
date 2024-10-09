#include "stdafx.h"
#include "NAnalyseState.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

NAnalyseState::NAnalyseState(void)
{
	InitVolume = 0.;
	CurVolume = 0.;
	CutVolume = 0.;
}

NAnalyseState::~NAnalyseState(void)
{
}

void NAnalyseState::SetCurVolume(double Vol)
{
	CurVolume = Vol;
	CutVolume = InitVolume - CurVolume;
}
