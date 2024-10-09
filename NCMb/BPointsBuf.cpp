#include "stdafx.h"
#include "BMatr.h"
#include "BPointsBuf.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

BPointsBuf::BPointsBuf(void)
{
	Size = 0;
	pExtra = NULL;
}

BPointsBuf::~BPointsBuf(void)
{
	delete pExtra;
}

const BPoint & BPointsBuf::GetPoint(int i) const
{
	if(i < MainSize)
		return Main[max(0, i)];

	if(!pExtra || i >= Size)
		return Main[0];

	return (*pExtra)[i - MainSize];
}
void BPointsBuf::SetSize(int n)
{
	if(n < 0)
		return;
	Size = n;
	if(Size <= MainSize)
		return;

	if(pExtra)
		pExtra->resize(Size - MainSize);
	else
	{
		pExtra = new std::vector<BPoint>;
		pExtra->resize(Size - MainSize);
	}
}
bool BPointsBuf::SetPoint(int i, const BPoint &P)
{
	if(i < 0 || i >= Size)
		return false;

	if(i < MainSize)
		Main[i] = P;
	else
		pExtra->at(i - MainSize) = P;
	return true;
}
void BPointsBuf::AddPoint(const BPoint &P)
{
	SetSize(Size + 1);
	SetPoint(Size - 1, P);
}

void BPointsBuf::Move(const class BMatr &Matr)
{
	for(int i = 0; i < Size && i < MainSize; ++i)
		Main[i] = Main[i] * Matr;
	if(pExtra)
	{
		for(int i = 0; i < MainSize - Size; ++i)
			pExtra->at(i) = pExtra->at(i) * Matr;
	}


}
