#include "stdafx.h"
#include "MCubeMask.h"


MCubeMask::MCubeMask(void)
{
}

MCubeMask::MCubeMask(int ColsN, int RowsN, int LevelsN)
{
	RowsNum = RowsN;
	ColsNum = ColsN;
	LevelsNum = LevelsN;
	Store.SetSizes((ColsNum - 1) / BaseIndexStep + 1, ((RowsNum - 1) / BaseIndexStep + 1), ((LevelsNum - 1) / BaseIndexStep + 1));
	Clear();
}

MCubeMask::MCubeMask(const MCubeMask &In)
{
	RowsNum = In.RowsNum;
	ColsNum = In.ColsNum;
	LevelsNum = In.LevelsNum;
	Store.SetSizes((ColsNum - 1) / BaseIndexStep + 1, ((RowsNum - 1) / BaseIndexStep + 1), ((LevelsNum - 1) / BaseIndexStep + 1));
}

MCubeMask::~MCubeMask(void)
{
}

void MCubeMask::SetBit(int Col, int Row, int Level)
{
	if(Col >= ColsNum || Row >= RowsNum || Level >= LevelsNum) // Is not error
		return;
	div_t dC, dR, dL;
	dC.quot = Col / BaseIndexStep;
	dR.quot = Row / BaseIndexStep;
	dL.quot = Level / BaseIndexStep;
	dC.rem = Col - dC.quot * BaseIndexStep;
	dR.rem = Row - dR.quot * BaseIndexStep;
	dL.rem = Level - dL.quot * BaseIndexStep;
	MM_BaseType NewBlock = MM_BaseType(1) << (dC.rem * BaseIndexStep * BaseIndexStep + dR.rem * BaseIndexStep + dL.rem);

	Store.Get(dC.quot, dR.quot, dL.quot) |= NewBlock;
}

bool MCubeMask::GetBit(int Col, int Row, int Level) const
{
	div_t dC, dR, dL;
	dC.quot = Col / BaseIndexStep;
	dR.quot = Row / BaseIndexStep;
	dL.quot = Level / BaseIndexStep;

	MM_BaseType Block = Store.Get(dC.quot, dR.quot, dL.quot);
	if (Block == 0)
		return false;

	dC.rem = Col - dC.quot * BaseIndexStep;
	dR.rem = Row - dR.quot * BaseIndexStep;
	dL.rem = Level - dL.quot * BaseIndexStep;

	return 0 != (Block & (MM_BaseType(1) << (dC.rem * BaseIndexStep * BaseIndexStep + dR.rem * BaseIndexStep + dL.rem)));
}

bool MCubeMask::GetUsedArea(int &ixStart, int &ixEnd, int &iyStart, int &iyEnd, int &izStart, int &izEnd)
{
	ixStart = ColsNum;
	ixEnd = -1;
	iyStart = RowsNum;
	iyEnd = -1;
	izStart = LevelsNum;
	izEnd = -1;
	for (int ibX = 0, isX = 0; ibX < ColsNum / BaseIndexStep; ++ibX, isX += BaseIndexStep)
	{
		bool fX = (isX < ixStart || isX + BaseIndexStep > ixEnd);
		for (int ibY = 0, isY = 0; ibY < RowsNum / BaseIndexStep; ++ibY, isY += BaseIndexStep)
		{
			bool fY = (isY < iyStart || isY + BaseIndexStep > iyEnd);
			for (int ibZ = 0; ibZ < LevelsNum / BaseIndexStep; ++ibZ)
			{
				MM_BaseType Block = Store.Get(ibX, ibY, ibZ);
				if (Block == 0)
					continue;
				auto isZ = ibZ * BaseIndexStep;
				bool fZ = (isZ < izStart || isZ + BaseIndexStep > izEnd);
				if (!(fX || fY || fZ)) // all indexes are in the region found already
					continue;
				// Block may have interesting bit (some limits can be changed)
				if (fX) // X limits can be changed
				{
					GetBlockMinMax(XSectMask, BaseIndexStep * BaseIndexStep, Block, isX, ixStart, ixEnd);
				}
				if (fY) // Y limits can be changed
				{
					GetBlockMinMax(YSectMask, BaseIndexStep, Block, isY, iyStart, iyEnd);
				}
				if (fZ) // Z limits can be changed
				{
					GetBlockMinMax(ZSectMask, 1, Block, isZ, izStart, izEnd);
				}
			}
		}
	}
	return ixEnd >= 0;
}

void MCubeMask::GetBlockMinMax(MM_BaseType Mask, int MaskShift, MM_BaseType Block, int Ind, int &Min, int &Max) const
{
	for (int iSect = 0; iSect < BaseIndexStep; ++iSect)
	{
		if (Mask & Block)
		{
			auto b = Ind + iSect;
			Min = min(Min, b);
			Max = max(Max, b);
		}
		Mask <<= MaskShift;
	}
}

void MCubeMask::Clear(void)
{
	Store.SetCommonValue(0);
}

bool MCubeMask::operator == (const MCubeMask &In)
{
	const MM_BaseType *Stor = Store.GetStor();
	const MM_BaseType *InStor = In.Store.GetStor();
	return 0 == 
		memcmp(Stor, InStor, ((ColsNum - 1) / BaseIndexStep + 1) * ((RowsNum - 1) / BaseIndexStep + 1) * ((LevelsNum - 1) / BaseIndexStep + 1) * sizeof(MM_BaseType));
}

MCubeMask &MCubeMask::operator |= (const MCubeMask &In)
{
	Store.Or(In.Store);
	return *this;
}
