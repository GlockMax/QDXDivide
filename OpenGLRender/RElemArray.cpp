#include "StdAfx.h"
#include "RElemArray.h"

//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
//#endif

RElemArray::RElemArray(void)
{
	RElemsNum = 0;
	Elems = NULL;
}

RElemArray::~RElemArray(void)
{
	delete Elems;
}

void RElemArray::SetSize(INT_PTR ElemsNum)
{
	if(ElemsNum > RElemsNum)
	{
		delete Elems;
		Elems = new RElem[ElemsNum];
	}
	RElemsNum = ElemsNum;
}

void RElemArray::SetElem(INT_PTR ElemsInd, INT_PTR SInd, INT_PTR SSize, int FirstName, int SecondName, int k)
{
	Elems[ElemsInd].FirstName = FirstName;
	Elems[ElemsInd].SecondName = SecondName;
	Elems[ElemsInd].kInd = k;
	Elems[ElemsInd].StartInd = int(SInd);
	Elems[ElemsInd].Size = int(SSize);
}
INT_PTR RElemArray::Find(int kInd, bool *IsEqual) const
{
	// Найти елемент с наименьшим номером, у которого kInd больше или равен заданному
	// Returns index of element found
	// IsEqual = true if element's kInd is equal to the given value
	*IsEqual = false;
	if (Elems == nullptr || RElemsNum < 1)
		return -1;
	if(Elems[0].kInd >= kInd)
	{
		*IsEqual = (Elems[0].kInd == kInd);
		return 0;
	}
	INT_PTR Left = 0;
	INT_PTR Right = RElemsNum - 1;
	if(Elems[Right].kInd < kInd)
	{
		*IsEqual = false;
		return -1;
	}

	for(; Right - Left > 1;)
	{
		auto Mid = (Left + Right) / 2;
		if(Elems[Mid].kInd >= kInd)
			Right = Mid;
		else
			Left = Mid;
	}
	*IsEqual = (Elems[Right].kInd == kInd);
	return Right;
}
INT_PTR RElemArray::FindFirst(int kInd) const
{
	bool tmp;
	return Find(kInd, &tmp);
}

INT_PTR RElemArray::FindLast(int kInd) const
{
	bool tmp;
	auto Last = Find(kInd, &tmp);
	if(Last < 0)
		Last = RElemsNum;
	return Last - 1;
}
