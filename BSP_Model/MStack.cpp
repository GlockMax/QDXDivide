#include "stdafx.h"
#include ".\mstack.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

MStack::Elem::Elem()
{
	Next = NULL;
}
MStack::Elem::~Elem()
{
}
MStack::Elem::Elem(void * d, Elem * e)
{
	Data = d;
	Next = e;
}
MStack::MStack(void)
{
	Top = NULL;
}

MStack::~MStack(void)
{
}

void * MStack::Pop(void)
{
	if(Top == NULL)
		return NULL;
	Elem b = *Top;
	delete Top;
	Top = b.Next;
	return b.Data;
}

void MStack::Push(void * ptr)
{
	Top = new Elem(ptr, Top);
}

void MStack::Clear()
{
	while(Pop());
}

