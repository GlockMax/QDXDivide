#include "stdafx.h"
#include "nqueuelim.h"

NQueueLim::NQueueLim(void) : CircularFifo()
{
}

NQueueLim::~NQueueLim(void)
{
}

bool NQueueLim::Enqueue(void * pNewData)
{
	return CircularFifo::push(pNewData);
}

void * NQueueLim::Dequeue()
{
	void *pB;
	bool res = CircularFifo::pop(pB);

	return res ? pB : nullptr;
}

bool NQueueLim::IsEmpty() const
{
	return CircularFifo::wasEmpty();
}

size_t NQueueLim::Length() const // Not thread safe
{
	if (_tail.load() == _head.load())
		return 0;
	return Capacity - _head + _tail;

}

void *NQueueLim::Get(int i) const // Not thread safe
{
	UINT shift = (_head + i) % Capacity;
	return _array[shift];
}
