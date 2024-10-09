#include "BRangePos.h"
#include "StdAfx.h"
#include "BProgram.h"
#include "NCadrGeom.h"
#include "BGeomArray.h"
#include "BRangePos.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BRangePos::BRangePos(const BProgram &PG) : Prog(PG)
{
	iStart = 0;
	kStart = 0;
	ProgNum = 0;
	CadrNum = 0;
	StepParamPerCent = 0.;
}

BRangePos::~BRangePos(void)
{
}

cadrID BRangePos::FindCurHCadr(int iRet) const
{
	return cadrID(ProgNum, int(Prog.GetHCadr2History().Find(iStart + iRet, kStart, kStart + iRet)));
}

cadrID BRangePos::FindCurHistCadr(int iRet) const
{
	return cadrID(ProgNum, iStart + iRet);
}

cadrID BRangePos::FindCurCadr(int iRet) const
{
	return Prog.GetCadrByHist(iRet + iStart);
}

void BRangePos::IncrCadrNums(int Cadr)
{
	CadrNum += Cadr;
}

void BRangePos::Incrik(int Shift)
{
	const BGeomArray& ProgGeom = Prog.GetGeomArray(0);
	for (int l = 0; l < Shift && iStart < ProgGeom.GetSize(); ++l, ++iStart)
	{
		// k - index of current step excluding steps in the cycles
		// i - index of current step
		NCadrGeom* pGeom = ProgGeom.GetAt(iStart);
		if (!pGeom->IsCycle())// Cycle attrib
			kStart++;
	}
}

void BRangePosColl::SetCadrNums(int Cadr, int LastStartCadr)
{
	CadrNum = Cadr;
	LastStartCadrNum = LastStartCadr;
}

void BRangePosColl::IncrCadrNums(int Cadr, int LastStartCadr)
{
	BRangePos::IncrCadrNums(Cadr);
	LastStartCadrNum += LastStartCadr;
}

ProgTPoint BRangePosColl::GetStartTPoint() const
{
	auto HCadr = FindCurHistCadr(LastStartCadrNum);
	MainTime Time = 0;
	if (HCadr >= 0 && HCadr.Cadr < UINT(Prog.GetGeomArraySize()))
	{
		const BGeomArray& GArr = Prog.GetCompGeomArray();
		const NCadrGeom* pGeom = GArr.GetAt(HCadr.Cadr);
		Time = pGeom->GetWaitTime0() + pGeom->GetStartTime();
	}
	return ProgTPoint(HCadr, Time);
}
