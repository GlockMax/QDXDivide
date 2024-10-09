// NCallStack.cpp: implementation of the NCallStack class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CadrID.h"
#include "NCallStack.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NCallStack::NCallStack()
{

}

NCallStack::~NCallStack()
{
	cadrID E;
	int N;
	while(GetSize() > 0)
		Pop(E,N);

}



int NCallStack::Push(cadrID &elem, const int &num)
{
	cadrID *pE = new cadrID;
	*pE = elem;
	int *pN = new int;
	*pN = num;
	Add(pN);
	Add(pE);
	return 0;
}

int NCallStack::Pop(cadrID &elem, int &num)
{
	if(GetSize() <= 1)
		return -1;
	cadrID *ptr = (cadrID *)GetAt(GetUpperBound());
	elem = *ptr;
	delete ptr;
	RemoveAt(GetUpperBound());
	int *pI = (int *)GetAt(GetUpperBound());
	num = *pI;
	delete pI;
	RemoveAt(GetUpperBound());
	return 0;
}

void NCallStack::Clear()
{
	cadrID elem;
	int num;
	while(Pop(elem,num)==0);
}
NCallStack::NCallStack(const NCallStack& In)
{
	Clear();
	for(int i = 0; i < In.GetSize(); i += 2)
	{
		cadrID *pE = new cadrID;
		*pE = *((cadrID *)In[i+1]);
		int *pN = new int;
		*pN = *((int *)In[i]);
		Add(pN);
		Add(pE);
	}
}
NCallStack& NCallStack::operator =(const NCallStack& In)
{
	Clear();
	for(int i = 0; i < In.GetSize(); i += 2)
	{
		cadrID *pE = new cadrID;
		*pE = *((cadrID *)In[i+1]);
		int *pN = new int;
		*pN = *((int *)In[i]);
		Add(pN);
		Add(pE);
	}
	return *this;
}
