#pragma once
#include "BSP_MODEL_API.h"
class MBaseMask
{
public:
	virtual ~MBaseMask() {}
	virtual void Clear(void) = 0;
	virtual void Set3Bits(int , int , bool , bool ) {};

};
class BSP_MODEL_API MQuadMask : public MBaseMask
{
public:
	MQuadMask(int ColsNum, int RowsNum);
	MQuadMask(const MQuadMask &In);
	virtual ~MQuadMask(void);
protected:
	MQuadMask(void);

	typedef __int32 MM_BaseType;
	typedef MM_BaseType *pInt32;
	int RowsNum;
	int ColsNum;
	MM_BaseType **Store;
	int LastRow;
	int LastCol;
	MM_BaseType LastElem;
public:
	void SetBit(int Col, int Row);
	void Set3Bits(int Col, int Row, bool cBorder, bool rBorder) override;
	bool GetBit(int Col, int Row);
	bool NextPair(int * pCol, int * pRow);
	void StartIterate(void);
	void Clear(void) override;
	bool operator == (const MQuadMask &In);
	bool IsZero() const;
	const MQuadMask& operator |= (const MQuadMask& in);
};
