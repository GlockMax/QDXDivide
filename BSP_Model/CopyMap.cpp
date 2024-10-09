#include "stdafx.h"
#include "MBSPForest.h"
#include "MElemIDProc.h"
#include "SOrigSurf.h"
#include "CopyMap.h"


CopyMap::CopyMap(const BMatr &M)
{
	Matr = M;
}


CopyMap::~CopyMap()
{
}

int CopyMap::AddOldIndOr(int OldInd)
{
	if (OldInd < 0 || OldInd == ID_EMPTY)
		return ID_EMPTY;
	auto it = OldInd2NewIndOr.find(OldInd);
	if (it != OldInd2NewIndOr.end())
		return it->second;
	SOrigSurf *pNewSurf = MFOREST.GetOrigSurf((size_type)OldInd)->MakeCopy();
	pNewSurf->Transform(Matr);
	auto NewInd = MFOREST.AddOrigSurf(pNewSurf);
	OldInd2NewIndOr[OldInd] = NewInd;
	return NewInd;
}

int CopyMap::AddOldIndPt(int OldInd)
{
	if (OldInd < 0)
		return OldInd;
	auto it = OldInd2NewIndPt.find(OldInd);
	if (it != OldInd2NewIndPt.end())
		return it->second;
	auto NewInd = MFOREST.AddPoints((*MFOREST.GetPoint(OldInd)) * Matr);
	OldInd2NewIndPt[OldInd] = NewInd;
	return NewInd;
}
