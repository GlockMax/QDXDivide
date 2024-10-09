#pragma once

template<typename T, unsigned int SIZE>
class BUndoRedoBuf
{
protected:
	unsigned int Start;
	unsigned int End;
	unsigned int Position;
	T Storage[SIZE];
public:
	BUndoRedoBuf(void)
	{
		Start = 0;
		End = 0;
		Position = 0;
	};

	~BUndoRedoBuf(void){};

	bool Add(const T& Val)
	{
		if (Val == Get())
			return false;
		Position = (Position + 1) % SIZE;
		if (Position == Start)
			Start = (Start + 1) % SIZE;
		Storage[Position] = Val;
		End = Position;
		return true;
	};

	T &Get()
	{
		return Storage[Position];
	};

	const T &GetPrev()
	{
		auto PrevPos = (Position == 0) ? SIZE - 1 : Position - 1;
		return Storage[PrevPos];
	};

	void StepBack()
	{
		if (Position == Start)
			return;
		Position = (Position == 0) ? SIZE - 1 : Position - 1;
	};

	void StepFor()
	{
		if (Position == End)
			return;
		Position = (Position + 1) % SIZE;
	};

	bool CanStepBack()
	{
		return Position != Start;
	};

	bool CanStepFor()
	{
		return Position != End;
	};
};
