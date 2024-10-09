#include "stdafx.h"
#include "nmulticounter.h"

NMultiCounter::NMultiCounter(void)
{
	Clear();
}

NMultiCounter::~NMultiCounter(void)
{
}
void NMultiCounter::Clear(void)
{
	for(int i = 0; i < COUNTERS_NUM; ++i)
		Counters[i] = 0;
}
int NMultiCounter::GetValue(int Ind)
{
	if(Ind < 0 || Ind >= COUNTERS_NUM)
		return -1;
	return Counters[Ind];
}

bool NMultiCounter::Increment(int Ind)
{
	if(Ind < 0 || Ind >= COUNTERS_NUM)
		return false;
	Counters[Ind] += 1;
	return true;
}