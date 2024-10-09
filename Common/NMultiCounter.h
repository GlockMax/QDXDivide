#pragma once
const int COUNTERS_NUM = 10;
class AFX_EXT_CLASS NMultiCounter
{
public:
	NMultiCounter(void);
	~NMultiCounter(void);
	void Clear();
	int GetValue(int Ind);
	bool Increment(int Ind);
protected:
	int Counters[COUNTERS_NUM];
};
