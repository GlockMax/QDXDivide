#include "stdafx.h"
#include "NCadrGeom.h"
#include "BProgram.h"
#include "NSyncCheckPoints.h"

NCheckPoint::NCheckPoint()
{
	ChanInd = 0;
	CadrHistInd = -1;
	pProg = nullptr;
	Priority = 0;
	IsBefore = true;
	Type = NCheckPointType::CP_Empty;
}

NCheckPoint::~NCheckPoint()
{
}

NSyncCheckPoints::NSyncCheckPoints()
{
}

NSyncCheckPoints::~NSyncCheckPoints()
{
}

NCheckPoint::NSyncID::NSyncID()
{
	IVal = 0;
	ChanInd = 0;
}

NCheckPoint::NSyncID::NSyncID(int Chan, int Val)
{
	ChanInd = Chan;
	IVal = Val;
}

NCheckPoint::NSyncID::NSyncID(int Chan, const CString& Val)
{
	ChanInd = Chan;
	IVal = 0;
	SVal = Val;
}

NCheckPoint::NSyncID::~NSyncID()
{
}

bool NCheckPoint::NSyncID::IsCorr(const NSyncID& ID)
{
	if((IVal == 0) != (ID.IVal == 0))// different types
		return false;
	if (IVal != 0)// number type
		return Equal ? IVal == ID.IVal : IVal >= ID.IVal;
	return Equal ? SVal == ID.SVal : SVal >= ID.SVal;
}

bool NCheckPoint::NSyncID::IsEmpty() const
{
	return IVal == 0 && SVal.IsEmpty();
}

void NCheckPoint::NSyncID::SetEmpty()
{
	IVal = 0;
	SVal.Empty();
}

bool NCheckPoint::Compare(const NCheckPoint& elem1, const NCheckPoint& elem2)
{
	if (elem1.GetBefore() != elem2.GetBefore())
		return elem2.GetBefore();
	return elem2.GetPriority() < elem1.GetPriority();
}

void NCheckPoint::SetCadrHistInd(int Val, BProgram* pInProg) noexcept
{
	CadrHistInd = Val;
	pProg = pInProg;
}

MainTime NCheckPoint::GetTime() const
{
	const NCadrGeom* pCadr = pProg->GetCompGeomArray(0)[CadrHistInd];
	if (GetBefore())
		return pCadr->GetStartTime();
	else
		return pCadr->GetEndTime();
}

