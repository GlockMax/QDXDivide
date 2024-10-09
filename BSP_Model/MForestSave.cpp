#include "stdafx.h"
#include "MBSPForest.h"
#include "MElemIDProc.h"
#include "MForestSave.h"

int MForestSave::SaveMap::AddOldInd(int OldInd)
{
	auto O2N = std::ref(OldInd2NewIndLoc);
	auto N2O = std::ref(NewInd2OldIndLoc);
	int NewIndSign = -1;

	if (OldInd >= 0)
	{
		O2N = std::ref(OldInd2NewInd);
		N2O = std::ref(NewInd2OldInd);
		NewIndSign = 1;
	}
	auto it = O2N.get().find(OldInd);
	if (it != O2N.get().end())
		return static_cast<int>(it->second);
	int NewInd = int(N2O.get().size()) * NewIndSign;
	N2O.get().push_back(OldInd);
	O2N.get()[OldInd] = NewInd;
	return static_cast<int>(NewInd);
}

void MForestSave::SaveMap::ResetLoc()
{
	NewInd2OldIndLoc.clear();
	OldInd2NewIndLoc.clear();
}

void MForestSave::SavePointInd(int Ind)
{
	int NewInd = PointSaveMap.AddOldInd(Ind);
	File->Write(&NewInd, sizeof(NewInd));
}

void MForestSave::SaveOrigInd(ElemID Ind)
{
	MElemIDProc* pIDProc = MFOREST.GetIDProc();
	int SurfID = pIDProc->GetSurfID(Ind);
	if (SurfID >= 0 && SurfID != ID_EMPTY)
	{
		int NewSurfID = OrigSaveMap.AddOldInd(SurfID);
		pIDProc->SetSurfID(&Ind, NewSurfID);
	}
	File->Write(&Ind, sizeof(Ind));
}
int MForestSave::GetPointInd(int Ind)
{
	return PointSaveMap.AddOldInd(Ind);
}

ElemID MForestSave::GetOrigInd(ElemID Ind)
{
	const MElemIDProc* pIDProc = MFOREST.GetIDProc();
	const int SurfID = pIDProc->GetSurfID(Ind);
	if (SurfID >= 0 && SurfID != ID_EMPTY)
	{
		const int NewSurfID = OrigSaveMap.AddOldInd(SurfID);
		pIDProc->SetSurfID(&Ind, NewSurfID);
	}
	return Ind;
}
