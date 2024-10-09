#pragma once
#include "nqueue.h"

class  AFX_EXT_CLASS NQueueBack :
	public NQueue
{
public:
	NQueueBack(void);
	~NQueueBack(void);
	bool Enqueue(void* pNewData);
protected:
	int MaxLength;
};
