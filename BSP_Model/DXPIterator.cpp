#include "stdafx.h"
#include "MBSPFOREST.h"
#include "DXSubModel5X.h"
#include "DXDirection5X.h"
#include "SOrigSurf.h"
#include "DXPIterator.h"


DXPIterator::DXPIterator(DXSubModel5X &Mod, int DirInd, int CellInd0, int CellInd1) : Mod5X(Mod)
{
	DirIndex = DirInd;
	Cell0 = CellInd0;
	Cell1 = CellInd1;
	Start();
}


DXPIterator::~DXPIterator(void)
{
}

void DXPIterator::Start(void)
{
	DirNum = (DirIndex < 0) ? 0 : DirIndex;
	const DXDirection5X &Dir = Mod5X.GetDirection(DirNum);
	i0 = (Cell0 < 0) ? 0 : Dir.GetN0Big() * Cell0;
	i1 = (Cell1 < 0) ? 0 : Dir.GetN1Big() * Cell1;
	P = Dir.GetDexelStartPoint(i0, i1);
	N0 = (Cell0 < 0) ? Dir.GetN0() - 1 : Dir.GetN0Big() + i0 - 1;
	N1 = (Cell1 < 0) ? Dir.GetN1() - 1 : Dir.GetN1Big() + i1 - 1;
	if(N0 == Dir.GetN0() - 2)
		++N0;
	if(N1 == Dir.GetN1() - 2)
		++N1;
	if(P == DXP_END)
		operator ++();
}

const DXPIterator &DXPIterator::operator ++ (void)
{
	if(P != DXP_END)
	{
		P = Mod5X.GetAtNext(P);
		if(P != DXP_END)
			return *this;
	}
	bool End = false;
	while(P == DXP_END && !End)
	{
		const DXDirection5X &Dir = Mod5X.GetDirection(DirNum);
		if(i1 < N1)
		{
			++i1;
			P = Dir.GetDexelStartPoint(i0, i1);
			continue;
		}
		i1 = (Cell1 < 0) ? 0 : Dir.GetN1Big() * Cell1;
		if(i0 < N0)
		{
			++i0;
			P = Dir.GetDexelStartPoint(i0, i1);
			continue;
		}
		i0 = (Cell0 < 0) ? 0 : Dir.GetN0Big() * Cell0;
		if(DirIndex < 0 && DirNum < 2)
		{
			++DirNum;
			P = Mod5X.GetDirection(DirNum).GetDexelStartPoint(i0, i1);
			continue;
		}
		End = true;
	}

	return *this;
}

bool DXPIterator::GetPoint(float *BUF) const
{
	if(P == DXP_END)
		return false;
	double dx = Mod5X.GetMin().GetX();
	double dy = Mod5X.GetMin().GetY();
	double dz = Mod5X.GetMin().GetZ();
	const DXMemoryPt &Mem = Mod5X.GetMemory();
	DX_DEPTH Val = Mem.GetAtZ(P);
	switch(DirNum)
	{
	case 0:
		*(BUF + 3) = float(Mod5X.GetXstep() * i0 + dx);
		*(BUF + 4) = float(Mod5X.GetYstep() * i1 + dy);
		*(BUF + 5) = float(Val + dz);
		break;
	case 1:
		*(BUF + 3) = float(Mod5X.GetXstep() * i1 + dx);
		*(BUF + 4) = float(Val + dy);
		*(BUF + 5) = float(Mod5X.GetZstep() * i0 + dz);
		break;
	case 2:
		*(BUF + 3) = float(Val + dx);
		*(BUF + 4) = float(Mod5X.GetYstep() * i0 + dy);
		*(BUF + 5) = float(Mod5X.GetZstep() * i1 + dz);
		break;
	}

	ElemID ID = Mem.GetAtID(P).Elem;
	
	MElemIDProc *pProc = MFOREST.GetIDProc();
	int SurfNum = pProc->GetSurfID(ID);
	SOrigSurf * pSurf = NULL;
	if(SurfNum >= 0 && SurfNum != ID_EMPTY)
		pSurf = MFOREST.GetOrigSurf(SurfNum);
	if(!pSurf)
	{
		*(BUF    ) = 0.;
		*(BUF + 1) = 0.;
		*(BUF + 2) = 1.;
	}
	else
	{
		pSurf->GetNormalFast(BUF);
	}
	static int PrevPCNum = -1; // for StoreNPC
	static float PrevColor = 0.; // for StoreNPC
	int PCNum = pProc->GetPCn(ID);
	if(PrevPCNum != PCNum)// This trick is for speed up only
	{
		PrevPCNum = PCNum;
		PrevColor = pProc->GetColor(ID).fval;
	}
	*(BUF + 6) = PrevColor;
	return true;
}
