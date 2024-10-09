#pragma once
#include "OPENGLRENDER_API.h"

class OPENGLRENDER_API RElem
{
public:
	int StartInd;
	int Size;
	unsigned int FirstName;
	unsigned int SecondName; 
	int kInd;// index of current step excluding steps in the cycles
};

class OPENGLRENDER_API RElemArray
{
protected:

public:
	RElemArray(void);
	~RElemArray(void);
	INT_PTR GetSize(void) { return RElemsNum;}
	void SetSize(INT_PTR ElemsNum);
	void SetElem(INT_PTR ElemsInd, INT_PTR SInd, INT_PTR SSize, int FirstName, int SecondName, int k);
	const RElem& GetElem(INT_PTR i) const { return Elems[i];}
	INT_PTR FindFirst(int kInd) const;
	INT_PTR FindLast(int kInd) const;
	INT_PTR Find(int kInd, bool *IsEqual) const;

protected:
	RElem *Elems;
	INT_PTR RElemsNum;
};
