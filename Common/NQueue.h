#pragma once

class NQueueElem;

class AFX_EXT_CLASS NQueue
{
public:
	NQueue(void);
	virtual ~NQueue(void);
protected:
	NQueueElem *pStart;
	NQueueElem *pEnd;

public:
	void * Peek(void);
	void * Dequeue(void);
	int Length(void);
	void Clear();
	virtual bool Enqueue(void * pNewData);
	NQueueElem * GetStartElem(void);
};
