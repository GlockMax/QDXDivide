#include "stdafx.h"
#include "NAnimState.h"
#include "nqueueelem.h"
#include "nqueueback.h"

NQueueBack::NQueueBack(void)
{
	MaxLength = 100;
}

NQueueBack::~NQueueBack(void)
{
}

bool NQueueBack::Enqueue(void* pNewData)
{
	if (Length() >= MaxLength - 1)
	{
		auto pbuf = pStart;
		auto pAnimState = static_cast<NAnimState*>(pStart->pData);
		pAnimState->DeleteRender();	
		delete pAnimState;
		pStart = pStart->pNext;
		delete pbuf;
	}
	return NQueue::Enqueue(pNewData);
}
