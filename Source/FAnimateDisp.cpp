#include "stdafx.h"
#include "ConstDef.h"
#include "math.h"
#include "FProg2BSP.h"
#include "NAnimState.h"
#include "GAnimateCadr.h"
#include "FAnimateDisp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

FAnimateDisp::FAnimateDisp(void)
{
}

FAnimateDisp::~FAnimateDisp(void)
{
}

void FAnimateDisp::ClearQueues()
{
	// Clear renders queues
	while( NAnimState *pAnimState = (NAnimState *)RendersQueue.Dequeue() )
	{
		pAnimState->DeleteRender();
		delete pAnimState;
	}
//	RendersQueue.Clear();
	while( NAnimState *pAnimState = (NAnimState *)RendersQueueBack.Dequeue() )
	{
		pAnimState->DeleteRender();
		delete pAnimState;
	}
//	RendersQueueBack.Clear();
}

bool FAnimateDisp::Place2RQueue(bool Interrupted, NAnimState *pAState)
{
	if (RendersQueue.Enqueue(pAState))
		return true;
	return Interrupted;
}
