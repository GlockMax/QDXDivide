#pragma once

template <class ELEM_TYPE, int MAX_STACK_SIZE> class BFixedSizeStack
{
public:
	BFixedSizeStack(void)
	{
		Top = -1;
	}

	~BFixedSizeStack(void)
	{
	}

	bool Push(const ELEM_TYPE &Elem)
	{
		if(Top >= MAX_STACK_SIZE)
			return false;
		StackMemory[++Top] = Elem;
		return true;
	}

	bool Pop(ELEM_TYPE &Elem)
	{	if(Top < 0)
			return false;
		Elem = StackMemory[Top--];
		return true;
	}

	const ELEM_TYPE &GetElem(int i)
	{
		return StackMemory[i];
	}

	int GetSize(void)
	{
		return Top+1;
	}

	void Clear(void)
	{
		Top = -1;
	}

protected:
	int Top;
	ELEM_TYPE StackMemory[MAX_STACK_SIZE];
};
