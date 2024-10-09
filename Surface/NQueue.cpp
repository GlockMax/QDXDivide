#include "stdafx.h"
#include "nqueueelem.h"
#include "nqueue.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

NQueue::NQueue(void)
{
	pStart = NULL;
	pEnd = NULL;
}

NQueue::~NQueue(void)
{
	Clear();
}

void * NQueue::Peek(void)
{
	if(pStart)
		return pStart->pData;
	else
		return NULL;
}

void * NQueue::Dequeue(void)
{
	NQueueElem *pBuf = pStart;
	if(pStart)
	{
		pStart = pStart->pNext;
		void *pDataBuf = pBuf->pData;
		delete pBuf;
		if(pStart == NULL)
			pEnd = NULL;
		return pDataBuf;
	}
	return NULL;
}

bool NQueue::Enqueue(void * pNewData)
{
	NQueueElem *pNewElem = new NQueueElem(pNewData);
	pNewElem->pData = pNewData;
	if(pEnd)
		pEnd->pNext = pNewElem;
	pEnd = pNewElem;
	if(!pStart)
		pStart = pEnd;
	return true;
}

int NQueue::Length(void)
{
	NQueueElem *pCur = pStart;
	int L = 0;
	for(; pCur != NULL; ++L)
		pCur = pCur->pNext;
	return L;
}

void NQueue::Clear(void)
{
	NQueueElem *pCur = pStart;
	pStart = NULL;
	pEnd = NULL;
	for(int L = 0; pCur != NULL; ++L)
	{
		NQueueElem *pBuf = pCur;
		pCur = pCur->pNext;
		delete pBuf;
	}
}

NQueueElem * NQueue::GetStartElem(void)
{
	return pStart;
}
