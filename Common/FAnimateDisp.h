#pragma once
#include "NQueueLim.h"
#include "NQueueBack.h"

class FAnimateDisp
{
public:
	FAnimateDisp(void);
	~FAnimateDisp(void);
	NQueueLim RendersQueue;
	NQueueBack RendersQueueBack;

public:
	void ClearQueues();
	bool Place2RQueue(bool Interrupted, class NAnimState *pAState);
protected:
};
