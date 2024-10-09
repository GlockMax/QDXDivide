#include "stdafx.h"
#include ".\nqueueelem.h"

NQueueElem::NQueueElem(void *pData)
{
	pNext = NULL;
	pData = pData;
}

NQueueElem::NQueueElem(void)
{
	pNext = NULL;
	pData = NULL;
}

NQueueElem::~NQueueElem(void)
{
}

NQueueElem * NQueueElem::GetNext(void)
{
	return pNext;
}

void * NQueueElem::GetData(void)
{
	return pData;
}
