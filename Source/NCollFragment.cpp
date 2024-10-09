#include "stdafx.h"
#include "NProgWalker.h"
#include "NCUnit.h"
#include "NMachUnitPair.h"
#include "NCollManagerOrd.h"
#include "NCollFragment.h"

NCollFragment::NCollFragment(NCollManagerOrd& iCollManager, BBox SGabar) : CollManager(iCollManager)
{
	StockGabar = SGabar;
	LastCheckHaveColl = true;
}

NCollFragment::~NCollFragment()
{
}

void NCollFragment::Set(const class NToolCombined* piTool, const BProgram* piProg, int iProgNum, NMachUnitPair* pMU)
{
	pProg = piProg;
	pTool = piTool;
	pMUPair = pMU;
	ProgNum = iProgNum;
	NonStockCollFound = false;
	StockCollFound = false;
}

void NCollFragment::Reset(const CArray<NCadrGeomArray>& Geoms, int GNumber, int RangePosCN)
{
	pGeoms = &Geoms;
	_GNumber = GNumber;
	RangePosCadrNum = RangePosCN;
	NonStockCollFound = false;
	StockCollFound = false;
}

bool NCollFragment::FixNonCollTime(bool StopIfStock)
{
	if (NonStockCollFound || (!StopIfStock && StockCollFound))
	{
		LastCheckHaveColl = true;
		return false;
	}
	else if (LastCheckHaveColl && !StockCollFound)
	{
		LastCheckHaveColl = false;
		return true;
	}
	return false;
}

int NCollFragment::CheckingProc(bool StopIfStock, bool LastOnly)
{
	NonStockCollFound = false;
	StockCollFound = false;
	LastCheckHaveColl = true;
	// returns last processed cadr index or GNumber if no critical collisions found
	SMachState StartB(&pMUPair->GetUnit().GetAddCoordsDef());// Create empty arrays these arrays will be filled in FillAddCoords
	SMachState EndB(&pMUPair->GetUnit().GetAddCoordsDef());
	NProgWalker ProgWalker(pMUPair, StockGabar, CollManager.GetCheckStep(), &StartB, &EndB, false);
	ProgWalker.SetTool(pTool);
	bool FirstCadr = true;
	cadrID CadrAbs;
	CadrAbs.Prog = ProgNum;
	for (int HCadrAbs = 0; HCadrAbs < _GNumber; ++HCadrAbs)
	{// For all cadrs 
		const NCadrGeomArray& Geom0 = pGeoms->GetAt(0);

		const int CycleStart = HCadrAbs;
		for (; Geom0.GetAt(HCadrAbs)->IsCycle(); ++HCadrAbs);
		const int CycleEnd = HCadrAbs;

		for (int ArrInd = 0; ArrInd < pGeoms->GetSize(); ++ArrInd)
		{
			const NCadrGeomArray& Geom = pGeoms->GetAt(ArrInd);
			int CadrInd = HCadrAbs;
			NCadrGeom* pGeom = Geom.GetAt(HCadrAbs);
			for (int i = CycleStart; i <= CycleEnd; ++i)
			{// Ordinary cadr first and than all cycle cadrs if any
				const bool Last = (CadrInd == _GNumber - 1);
				const bool Skip = !Last && LastOnly;
				ProgWalker.FillAddCoords(pGeom, pProg);
				if (ProgWalker.SetGeom(pGeom) && !Skip)
				{
					bool StopFound = false;
					CadrAbs.Cadr = CadrInd + RangePosCadrNum;
					bool IsFast5X = pGeom->IsFast() && pGeom->Is5x();
					if (FirstCadr)
					{
						FirstCadr = false;
						ProgWalker.SetStart();
						StopFound = CheckOnePosition(StopIfStock, IsFast5X, ProgTPoint(CadrAbs, ProgWalker.GetCurTime()));
					}
					while (ProgWalker.NextStep() && !StopFound)// step is inside one cadr only
					{
						StopFound = CheckOnePosition(StopIfStock, IsFast5X, ProgTPoint(CadrAbs, ProgWalker.GetCurTime()));
					}
					if (StopFound)
						return CadrInd;
					if (Last)// last cadr
					{
						ProgWalker.SetEnd();
						if (CheckOnePosition(StopIfStock, IsFast5X, ProgTPoint(CadrAbs, ProgWalker.GetCurTime())))
							return _GNumber - 1;
					}
				}
				CadrInd = i;
				pGeom = Geom.GetAt(i);
			}
			HCadrAbs = CycleEnd;
		}
	}
	return _GNumber;
}

bool NCollFragment::CheckOnePosition(bool StopIfStock, bool IsFast5X, const ProgTPoint& PTPoint)
{
	bool StopFound = false;
	NonStockCollFound = false;
	StockCollFound = false;
	if (CollManager.CheckCurPosition(&pMUPair->GetMach(), pTool, PTPoint, CollManager.GetIgnoreWrongAxis(), StopIfStock, IsFast5X))
	{
		CalcStockCollFound();
		StopFound = StopIfStock && StockCollFound;
	}
	if (FixNonCollTime(StopIfStock))// need to fix time without collision
	{
		NCollisionID CollID(PTPoint, nullptr, nullptr, LE_NOCOLL);
		CollManager.AddCollisionMark(CollID);
	}
	return StopFound;
}

void NCollFragment::CalcStockCollFound()
{
	int Diskr = CollManager.HaveStockCollisions();
	NonStockCollFound = Diskr & 1;
	StockCollFound = Diskr & 2;
}

int NCollFragment::MoveGeoms(int Shift, CArray<NCadrGeomArray>& Dest, const CArray<NCadrGeomArray>& Src)
{
	// returns index of the first element in Src copied to Dest
	if (Shift == 0)
	{
		//Dest.RemoveAll();
		//Dest.Append(Src);
		return 0;
	}
	// find first element to copy
	int StartInd = Shift;
	int ApproachInd = -1;
	const auto pGeom = Src.GetAt(0).GetAt(Shift);
	const auto pPrevGeom = Src.GetAt(0).GetAt(Shift - 1);// Shift is not 0
	if (pGeom->IsCycle())
	{
		// replace approach (if exists) to dummy one
		for (int Ind = StartInd; Ind < Src.GetAt(0).GetSize(); ++Ind)
		{
			const auto pLoc = Src.GetAt(0).GetAt(Ind);
			if (!pLoc->IsCycle())
			{
				ApproachInd = Ind;
				break;
			}
		}
	}
	else if (pPrevGeom->IsCycle()) // pGeom is approach
	{
		// skip approach
		StartInd++;
	}
	for (int i = 0; i < Dest.GetSize(); ++i)
	{
		Dest.GetAt(i).RemoveAll();
		for (int j = 0; j < Src.GetAt(i).GetSize() - StartInd; ++j)
			Dest.GetAt(i).Add(Src.GetAt(i).GetAt(j + StartInd));
		if(ApproachInd >= 0)
			Dest.GetAt(i).SetAt(ApproachInd - StartInd, &Dummy);
	}
	return StartInd;
}

