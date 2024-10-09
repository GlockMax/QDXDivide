#pragma once
#include "BSP_MODEL_API.h"
#include "B3DArray.h"
#include "MQuadMask.h"

class BSP_MODEL_API MCubeMask : public MBaseMask
{
public:
	MCubeMask(int ColsNum, int RowsNum, int LevelsNum);
	MCubeMask(const MCubeMask &In);
	~MCubeMask(void);
protected:
	MCubeMask(void);
	typedef unsigned __int64 MM_BaseType;
	static const int BaseTypeSize = sizeof(MM_BaseType) * 8;
	static const int BaseIndexStep = 4; // BaseIndexStep ^ 3 = BaseTypeSize
	B3DArray<MM_BaseType> Store;

	int RowsNum;
	int ColsNum;
	int LevelsNum;

	const MM_BaseType XSectMask = 0x000000000000FFFF;
	const MM_BaseType YSectMask = 0x000F000F000F000F;
	const MM_BaseType ZSectMask = 0x1111111111111111;

	void GetBlockMinMax(MM_BaseType Mask, int MaskShift, MM_BaseType Block,int Ind, int &Min, int &Max) const;

public:
	void SetBit(int Col, int Row, int Level);
	bool GetBit(int Col, int Row, int Level) const;
	bool GetUsedArea(int &ixStart, int &ixEnd, int &iyStart, int &iyEnd, int &izStart, int &izEnd);
	void Clear(void) override;
	bool operator == (const MCubeMask &In);
	MCubeMask &operator |= (const MCubeMask &In);
};
