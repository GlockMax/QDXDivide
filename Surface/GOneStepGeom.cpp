#include "stdafx.h"
#include "ConstDef.h"
#include "NCadrSpeedPar.h"
#include "NCadrGeom.h"
#include "BRangePos.h"
#include "GOneStepGeom.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

GOneStepGeom::GOneStepGeom(const NCadrGeomArray &inGeom, bool iNeedBackNums, DX_TOOLTYPE iDXType) : Geom(inGeom)
{
	IsRightOrdered = false;
	TrackFeed = false;
	MaxStepLength = 1.;
	MaxStepTime = 1.;
	Restart();
	NewCadrs.SetSize(0, 2);// Not more than 2 cadrs may be stored
	NeedBackNums = iNeedBackNums;
	DXType = iDXType;
}


GOneStepGeom::~GOneStepGeom()
{
	for (int i = 0; i < NewCadrs.GetSize(); ++i)
		delete NewCadrs.GetAt(i);
	NewCadrs.RemoveAll();
}

bool GOneStepGeom::Fill(int GNumber, INT_PTR &Start, double &StartT, bool DivideCadr, INT_PTR &ActiveCadrNum, double &StepParamPerCent, const NCadrSpeedPar & CadrSpeedPar
	, void DivideCallB(bool TrackFeed, NCadrGeom& Source, double TValue, NCadrGeom& Dest, FRange*), FRange* pR, MainTime GetTimeCallB(MainTime, FRange*))
{
	if (NeedBackNums)
		BackNums.clear();
	int BackNumsVal = 0;
	// Returns a number of Geom elements used
	// if !DivideCadr than last cadr should not be divided and one extra cadr should be added
	bool Ret = false;
	RemoveAll();
	for (int v = 0; v < NewCadrs.GetSize(); ++v)
		delete NewCadrs.GetAt(v);
	NewCadrs.RemoveAll();

	double MaxStepParameter = TrackFeed ? MaxStepTime : MaxStepLength;
	double StepParameter = MaxStepParameter * StepParamPerCent;
	double LocMaxStepParameter = MaxStepParameter;// may be changed in the loop
	MainTime NextBreakTime = MainT_UNDEF - 1;
	auto i = Start;
	for (; (i < GNumber || i == ApproachCadrNum + 1) && StepParameter < LocMaxStepParameter; ++i)
	{
		Ret = true;
		if (i == ApproachCadrNum + 1 && ApproachCadrActive)// Jump to cycle start
		{
			i = CurCycleStart;
			ApproachCadrActive = false;
		}
		if (i == ApproachCadrNum && !ApproachCadrActive)// Cycle ended
		{
			CurCycleStart = -1;
			ApproachCadrNum = -2;// -1 is bad
			continue;
		}
		NCadrGeom *pCur = Geom[i];
		BackNumsVal = int(i);
		if (pCur->IsCycle() && CurCycleStart < 0)// Cycle started
		{// Process cycle
			CurCycleStart = i;
			//Find first non cycle cadr (ApproachCadrNum)
			for (int k = 1; k < Geom.GetSize() - 1 - i; ++k)
			{
				if (!Geom[i + k]->IsCycle())//Non Cycle attrib
				{
					i = i + k;
					ApproachCadrNum = i;
					ApproachCadrActive = true;
					break;
				}
			}
			if (!ApproachCadrActive)// Approach cadr was not found
			{
				CurCycleStart = -1;
				i = GNumber;
				break;
			}
			pCur = Geom[i];
			BackNumsVal = int(i);
		}
		if (TrackFeed)
		{
			if (pCur->IsZeroTime())
				continue;
		}
		else
		{
			if (pCur->IsUndef())// Don't use HaveGeom because Type may be "rotate"
				continue;
		}
		ActiveCadrNum = i; // The number of the last processed cadr

		if (StartT > 0. && i == Start) 
		{// May be executed one time at most
			NCadrGeom Buf(*pCur);
			NCadrGeom *pExtra = new NCadrGeom;
			NewCadrs.Add(pExtra); // to be able to delete
			DivideCallB(TrackFeed, Buf, StartT, *pExtra, pR);
			pCur = pExtra;
			BackNumsVal = int(i);
		}
		else
			StartT = 0.;
		const double CurLength = TrackFeed ? pCur->GetWholeTimeS() : CadrSpeedPar.GetScaledCadrLength(*pCur);
		const double CurParameter = (pCur->GetType().type == addcoord) ? 0. : CurLength;
		if (NextBreakTime == MainT_UNDEF - 1)
		{// first cadr in this loop
			NextBreakTime = GetTimeCallB(pCur->GetStartTime(), pR);
		}
		bool BreakFound = false;
		if (pCur->GetEndTime() > NextBreakTime)
		{
			double BCurParameter = (pCur->GetType().type == addcoord) ? 0 : CurParameter;
			double buf = StepParameter + (NextBreakTime - pCur->GetStartTime() - pCur->GetWaitTime0()) * BCurParameter / pCur->GetWorkTime();// part parameter
			if (buf <= LocMaxStepParameter)
			{
				BreakFound = true;
				LocMaxStepParameter = buf;
			}
		}
		StepParameter = (pCur->GetType().type == addcoord) ? 0 : StepParameter + CurParameter;
		BreakFound |= fabs(StepParameter - LocMaxStepParameter) * 10 > LocMaxStepParameter;

		if (StepParameter >= LocMaxStepParameter)
		{// May be executed one time at most
			if (BreakFound && DivideCadr)
			{// Cadr part should be generated
				double RestT = (LocMaxStepParameter - (StepParameter - CurParameter)) / CurParameter;
				const double Tst = CurLength / (100. * MIND);
				if (i == Start && Tst * RestT < 1.)
					RestT = 0.01;
				if (Tst * RestT >= 1.)
				{
					NCadrGeom *pExtra = new NCadrGeom(*pCur);
					NewCadrs.Add(pExtra); // to be able to delete
					NCadrGeom Buf;
					DivideCallB(TrackFeed, *pExtra, RestT, Buf, pR);
					pCur = pExtra;
					BackNumsVal = int(i);
					StartT += RestT * (1. - StartT);
					--i;
				}
				else 
					StartT = 0.;
			}
			else
				StartT = 0.;
		}
		Add(pCur);
		if(NeedBackNums)
			BackNums.push_back(BackNumsVal);
	}
	// i now points to the first cadr to be processed in the next Fill
	Start = i;
	if (!DivideCadr && !IsEmpty())// don't use IsGeomEmpty
	{
		//Find next geom cadr
		INT_PTR ni = i;
		for (; ni < Geom.GetSize(); ++ni)
			if (Geom[ni]->HaveGeom())
				break;
		if (ni == Geom.GetSize())
			--ni;
		//END: Find next geom cadr
		Add(Geom[ni]);
		if (NeedBackNums)
			BackNums.push_back(-1);
	}

	StepParamPerCent = (StepParameter == 0.) ? 0. : StepParameter / LocMaxStepParameter;
	StepParamPerCent = StepParamPerCent >= 0.9 ? 0. : StepParamPerCent;
	IsRightOrdered = true;
	return Ret;
}

void GOneStepGeom::Restart(void)
{
	CurCycleStart = -1;
	ApproachCadrNum = -2;// -1 is bad
	ApproachCadrActive = false;
}

INT_PTR GOneStepGeom::GetNextInd(INT_PTR Ind)
{
	if (IsRightOrdered)
		return Ind + 1;

	INT_PTR i = Ind + 1;
	if (i == ApproachCadrNum + 1 && ApproachCadrActive)// Jump to cycle start
	{
		i = CurCycleStart;
		ApproachCadrActive = false;
	}
	if (i == ApproachCadrNum && !ApproachCadrActive)// Cycle ended
	{
		CurCycleStart = -1;
		ApproachCadrNum = -2;// -1 is bad
		++i;
	}
	if (i >= Geom.GetSize())
		return i;
	NCadrGeom *pCur = Geom[i];
	if (pCur->IsCycle() && CurCycleStart < 0)// Cycle started
	{// Process cycle
		CurCycleStart = i;
		//Find first non cycle cadr (ApproachCadrNum)
		for (int k = 1; k < Geom.GetSize() - 1 - i; ++k)
		{
			if (!Geom[i + k]->IsCycle())//Non Cycle attrib
			{
				i = i + k;
				ApproachCadrNum = i;
				ApproachCadrActive = true;
				break;
			}
		}
	}
	return i;
}

int GOneStepGeom::GetOriginalCadrNum(int Ind) const
{
	if (BackNums.empty())
		return 0;
	if (Ind < 0 || Ind >= int(BackNums.size()))
		return 0;
	return BackNums[Ind];
}

INT_PTR GOneStepGeom::FindLastDefined(INT_PTR LastIndex)
{
	// find last not undef cadr before LastIndex
	INT_PTR LastDef = -1;
	for (INT_PTR i = 0; i <= LastIndex; i = GetNextInd(i))
		if (!(*this)[i]->IsUndef())
			LastDef = i;
	return LastDef;
}

void GOneStepGeom::RecalcPosition(BRangePosColl& OneColl)
{
	OneColl.SetCadrNums(GetOriginalCadrNum(OneColl.CadrNum), GetOriginalCadrNum(OneColl.GetLastStartCadrNum()));
}
