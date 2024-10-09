#include "StdAfx.h"
#include "NCollisionObjectID.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

NCollisionObjectID::NCollisionObjectID(void)
{
	Group = OG_GR_EMPTY;
	ID = -1;
	Behav = -1;
	SelfNum = -1;
	pCollidableObj = nullptr;
	ColsNum = 1;
	RowsNum = 1;
	LevelsNum = 1;
}

NCollisionObjectID::~NCollisionObjectID(void)
{
}

void NCollisionObjectID::SetDimensions(int Nx, int Ny, int Nz)
{
	ColsNum = Nx / IndStep;
	RowsNum = Ny / IndStep;
	LevelsNum = Nz / IndStep;
}

size_t NCollisionObjectID::MakeKey(int i, int j, int k) const noexcept
{
	return size_t(i) / IndStep * ColsNum * LevelsNum + size_t(j) / IndStep * LevelsNum + size_t(k) / IndStep;
}

bool NCollisionObjectID::operator==(const NCollisionObjectID& tst) const
{
	return memcmp(this, &tst, sizeof(*this)) == 0;
}

int NCollisionObjectID::MakeBehav(int AxisInd, bool Stock, bool CutTool)
{
	return (AxisInd << 2) + ((Stock ? 1 : 0) << 1) + (CutTool ? 1 : 0);
}

bool NCollisionObjectID::IsBehavCutting() const
{
	return (Behav & 1) != 0;
}

void NCollisionObjectID::MakeBehavCutting(bool Cutting)
{
	if (Cutting)
		Behav |= 1;
	else
		Behav &= ~1;
}


