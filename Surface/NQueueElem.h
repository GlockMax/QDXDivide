#pragma once

class NQueueElem
{
	friend class NQueue;
	friend class NQueueBack;
public:
	NQueueElem(void *pData);
	virtual ~NQueueElem(void);
protected:
	NQueueElem(void);
	NQueueElem *pNext;
	void *pData;
public:
	NQueueElem * GetNext(void);
	void * GetData(void);
};
