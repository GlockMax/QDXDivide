#include "stdafx.h"
#include "NCUnitState.h"
#include "nmilestone.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define OneStoneBlock(Name) 	case Name:\
		{static NMileStone *pStone = NULL;\
		switch(Func){case AddMi:Add(pStone = new N##Name(Name));break;\
		case RemoveMi:Remove(pStone);	delete pStone; pStone = NULL; break;\
		}}break


NMileStones::NMileStones(void)
{
	Active = true;
}

NMileStones::~NMileStones(void)
{
	Clear();
}

const NMileStone * NMileStones::IsMileStone(NCUnit *pU, const NCUnitStatePrev *pState)
{
	if(!Active)
		return NULL;
	pUnit = pU;
	pPrevState = pState;
	for(int i = 0; i < GetSize(); ++i)
		if(GetAt(i)->IsReached(this))
			return GetAt(i);
	return NULL;
}

bool NMileStones::Remove(NMileStone * pElem)
{
	int i = 0;
	for (; i < GetSize(); ++i)
		if( GetAt(i) == pElem)
		{
			RemoveAt(i, 1);
			break;
		}
	return i != GetSize();
}

bool NMileStones::Serve(enum StoneNames Name, enum MiFunctions Func)
{
	switch (Name)
	{
		OneStoneBlock(MiToolChange);
		OneStoneBlock(MiProgStartEnd);
		OneStoneBlock(MiZOnly);
		OneStoneBlock(MiXOnly);
		OneStoneBlock(MiYOnly);
		OneStoneBlock(MiBookmark);
		OneStoneBlock(MiCollision);
		OneStoneBlock(MiError);
		OneStoneBlock(MiProgOptStop);
		OneStoneBlock(MiSpindleSpeed);
		OneStoneBlock(MiSpindleForwardReverse);
		OneStoneBlock(MiFeed);
		OneStoneBlock(MiCoolant);
		OneStoneBlock(MiPositioning);
		OneStoneBlock(MiCircularInterpolation);
		OneStoneBlock(MiLinearInterpolation);
		OneStoneBlock(MiCycle);
		OneStoneBlock(MiSubprogramCall);
		OneStoneBlock(MiSubprogramStartEnd);
		OneStoneBlock(MiToolLengthCompensationActive);
		OneStoneBlock(MiToolLengthCompensationCancel);
		OneStoneBlock(MiToolDiameterCompensationActive);
		OneStoneBlock(MiToolDiameterCompensationCancel);
		OneStoneBlock(MiLifeTime);
		OneStoneBlock(MiCheckZeroFeed);
		OneStoneBlock(MiCheckLengthComp);
		OneStoneBlock(MiCheckLengthOffsetNum);
		OneStoneBlock(MiCheckDiamOffsetNum);
		OneStoneBlock(MiCheckIJKRDef);
		OneStoneBlock(MiCheckXYChamf);
		OneStoneBlock(MiCheckFeedTurnSpindleTurnOff);
		OneStoneBlock(MiCheckFeedTurnSpindleMillOn);
		OneStoneBlock(MiCheckFeedMillSpindleMillOff);
		OneStoneBlock(MiCheckFeedMillSpindleTurnOn);
		OneStoneBlock(MiCheckDiamCompOff);
	default:
		return false;
		break;
	}
	return true;
}

void NMileStones::Activate(void)
{
	Active = true;
}

void NMileStones::DeActivate(void)
{
	Active = false;
}
bool NMileStones::IsActive(void)
{
	return Active;
}


void NMileStones::FillFromDlg(const NBreakpointDlg &Dlg)
{
	Clear();

	if (Dlg.v_MiProgStartEnd) Serve(MiProgStartEnd, AddMi);
	if (Dlg.v_MiToolChange) Serve(MiToolChange, AddMi);
	if (Dlg.v_MiBookmark) Serve(MiBookmark, AddMi);
//	if (Dlg.v_MiCollision) Serve(MiCollision, AddMi);
	if (Dlg.v_MiCollision) Serve(MiError, AddMi);
	if (Dlg.v_MiProgOptStop) Serve(MiProgOptStop, AddMi);
	if (Dlg.v_MiLifeTime) Serve(MiLifeTime, AddMi);
	
	if(Dlg.v_BREAKPOINT_ON_OFF)
	{
		if(Dlg.v_MiZOnly) Serve(MiZOnly, AddMi);
		if(Dlg.v_MiXOnly) Serve(MiXOnly, AddMi);
		if(Dlg.v_MiYOnly) Serve(MiYOnly, AddMi);
		if(Dlg.v_MiSpindleSpeed) Serve(MiSpindleSpeed, AddMi);
		if(Dlg.v_MiSpindleForwardReverse) Serve(MiSpindleForwardReverse, AddMi);
		if(Dlg.v_MiFeed) Serve(MiFeed, AddMi);
		if(Dlg.v_MiCoolant) Serve(MiCoolant, AddMi);
		if(Dlg.v_MiPositioning) Serve(MiPositioning, AddMi);
		if(Dlg.v_MiCircularInterpolation) Serve(MiCircularInterpolation, AddMi);
		if(Dlg.v_MiLinearInterpolation) Serve(MiLinearInterpolation, AddMi);
		if(Dlg.v_MiCycle) Serve(MiCycle, AddMi);
		if(Dlg.v_MiSubprogramCall) Serve(MiSubprogramCall, AddMi);
		if(Dlg.v_MiSubprogramStartEnd) Serve(MiSubprogramStartEnd, AddMi);
		if(Dlg.v_MiToolLengthCompensationActive) Serve(MiToolLengthCompensationActive, AddMi);
		if(Dlg.v_MiToolLengthCompensationCancel) Serve(MiToolLengthCompensationCancel, AddMi);
		if(Dlg.v_MiToolDiameterCompensationActive) Serve(MiToolDiameterCompensationActive, AddMi);
		if(Dlg.v_MiToolDiameterCompensationCancel) Serve(MiToolDiameterCompensationCancel, AddMi);
	}
}
void NMileStones::Clear(void)
{
	for(int i = 0; i < GetSize(); ++i)
		delete GetAt(i);
	RemoveAll();
}

bool NMileStones::HaveToolChangeOnly(void)
{
	if(!IsActive() || GetCount() != 1)
		return false;
	return GetAt(0)->IsToolChange();
}
