#pragma once

class MStack
{
	class Elem
	{
		friend class MStack;
	public:
		Elem();
		~Elem();
	protected:
		void *Data;
		Elem *Next;
	public:
		Elem(void * d, Elem * e);
	};
public:
	MStack(void);
	~MStack(void);
protected:
	Elem *Top;
public:
	void * Pop(void);
	void Push(void * ptr);
	void Clear(void);
};
