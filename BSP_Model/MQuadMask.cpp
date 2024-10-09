#include "stdafx.h"
#include "MQuadMask.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


MQuadMask::MQuadMask(void)
{
	Store = NULL;
}
MQuadMask::MQuadMask(int ColsN, int RowsN)
{
	RowsNum = RowsN;
	ColsNum = ColsN;
	Store = new pInt32[RowsNum];
	div_t s = div(ColsNum , sizeof(MM_BaseType)*8);
	int Size = s.rem > 0 ? s.quot + 1 : s.quot;

	MM_BaseType *pBase = new MM_BaseType[RowsNum * Size];
	for(int i = 0; i < RowsNum; ++i, pBase += Size)
		Store[i] = pBase;

	LastRow = LastCol = -2;// iteration is over
	Clear();
}

MQuadMask::MQuadMask(const MQuadMask &In)
{
	RowsNum = In.RowsNum;
	ColsNum = In.ColsNum;
	
	Store = new pInt32[RowsNum];
	div_t s = div(ColsNum , sizeof(MM_BaseType)*8);
	int Size = s.rem > 0 ? s.quot + 1 : s.quot;
	MM_BaseType *pBase = new MM_BaseType[RowsNum * Size];
	for(int i = 0; i < RowsNum; ++i, pBase += Size)
		Store[i] = pBase;

	LastRow = LastCol = -2;// iteration is over

	for(int i = 0; i < RowsNum; ++i)
		for(int j = 0; j < Size; ++j)
			Store[i][j] = In.Store[i][j];
}

MQuadMask::~MQuadMask(void)
{
	if(Store)
		delete[] Store[0];
	delete Store;
}

void MQuadMask::SetBit(int Col, int Row)
{
	if(Col >= ColsNum || Row >= RowsNum) // Is not error
		return;
	div_t s = div(Col , sizeof(MM_BaseType) * 8);
	Store[Row][s.quot] |= MM_BaseType(1) << s.rem;
}

void MQuadMask::Set3Bits(int Col, int Row, bool cBorder, bool rBorder)
{
	div_t s = div(Col , sizeof(MM_BaseType) * 8);
	if(Col < ColsNum && Row < RowsNum) // Is not error
		Store[Row][s.quot] |= MM_BaseType(1) << s.rem;
	if(rBorder && Col < ColsNum) // Y - 1
		Store[Row - 1][s.quot] |= MM_BaseType(1) << s.rem;
	if(cBorder && Row < RowsNum) // X - 1
	{
		s = div(Col - 1 , sizeof(MM_BaseType) * 8);
		Store[Row][s.quot] |= MM_BaseType(1) << s.rem;
	}
}

bool MQuadMask::GetBit(int Col, int Row)
{
	div_t s = div(Col , sizeof(MM_BaseType)*8);
	return (Store[Row][s.quot] & (MM_BaseType(1) << s.rem)) != MM_BaseType(0);
}

bool MQuadMask::NextPair(int * pCol, int * pRow)
{
	while( !LastElem )
	{
		LastCol = (int(LastCol/(sizeof(MM_BaseType)*8)) + 1)*sizeof(MM_BaseType)*8;
		if( LastCol >= ColsNum)
		{
			++LastRow;
			if( LastRow >= RowsNum )
			{
				*pRow = *pCol = -2;
				return false;
			}
			LastCol = 0;
		}
		LastElem = Store[LastRow][int(LastCol/(sizeof(MM_BaseType)*8))];
	}
// LastElem is not empty
	*pRow = LastRow;
	div_t s = div(LastCol , sizeof(MM_BaseType)*8);
	MM_BaseType Mask = (MM_BaseType(1) << s.rem);
	for(int i = s.rem; i < sizeof(MM_BaseType)*8; ++i, Mask <<= 1)
		if( (Mask & LastElem) != 0 )
		{
			LastElem &= ~Mask;
			LastCol += i - s.rem; 
			*pCol = LastCol;
			return true;
		}
	return false;// internal error
}

void MQuadMask::StartIterate(void)
{
	LastRow = 0;
	LastCol = 0;
	LastElem = Store[0][0];
}

void MQuadMask::Clear(void)
{
	div_t s = div(ColsNum , sizeof(MM_BaseType)*8);
	int Size = s.rem > 0 ? s.quot + 1 : s.quot;
	for(int i = 0; i < RowsNum; ++i)
		for(int j = 0; j < Size; ++j)
			Store[i][j] = MM_BaseType(0);
}

bool MQuadMask::operator == (const MQuadMask &In)
{
	if(RowsNum != In.RowsNum || ColsNum != In.ColsNum)
		return false;
	div_t s = div(ColsNum , sizeof(MM_BaseType)*8);
	int Size = s.rem > 0 ? s.quot + 1 : s.quot;
	for(int i = 0; i < RowsNum; ++i)
		for(int j = 0; j < Size; ++j)
			if(Store[i][j] != In.Store[i][j])
				return false;
	return true;
}

bool MQuadMask::IsZero() const
{
	div_t s = div(ColsNum, sizeof(MM_BaseType) * 8);
	int Size = s.rem > 0 ? s.quot + 1 : s.quot;
	for (int i = 0; i < RowsNum; ++i)
		for (int j = 0; j < Size; ++j)
			if (Store[i][j] != MM_BaseType(0))
				return false;
	return true;
}

const MQuadMask & MQuadMask::operator|=(const MQuadMask & in)
{
	div_t s = div(ColsNum, sizeof(MM_BaseType) * 8);
	int Size = s.rem > 0 ? s.quot + 1 : s.quot;
	for (int i = 0; i < RowsNum; ++i)
		for (int j = 0; j < Size; ++j)
			Store[i][j] |= in.Store[i][j];
	return *this;
}
