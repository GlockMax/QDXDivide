#include "stdafx.h"
#include "NProgram.h"
#include "ConstDef.h"
#include "math.h"
#include "NCUnit.h"
#include "NCMDoc.h"
#include "NCUnitState.h"
#include "nmilestone.h"

NMileStone::NMileStone(enum StoneNames Name)
{
	this->Name = Name;
}

NMileStone::~NMileStone(void)
{
}

// MiToolChange!
bool NMiToolChange::IsReached(const NMileStones * pSt)
{
	pStones = pSt;// Must be done in all IsReached functions
	if((GetPrevState()->CurToolN != GetCurState()->CurToolN)
		|| (GetPrevState()->CurTurret != GetCurState()->CurTurret))
		return true;
	return false;
}
// MiProgStartEnd!
bool NMiProgStartEnd::IsReached(const NMileStones * pSt)
{
	pStones = pSt;// Must be done in all IsReached functions
	if(GetPrevState()->ProgramStarted != GetCurState()->ProgramStarted)
		return true;
	return false;
}
// MiZOnly!
bool NMiZOnly::IsReached(const NMileStones * pSt)
{
	pStones = pSt;// Must be done in all IsReached functions
	if(GetCurState()->Geom.IsLine() &&
		fabs(GetCurState()->CurZ) > MIND &&
		fabs(GetCurState()->CurX) < MIND &&
		fabs(GetCurState()->CurY) < MIND)
		return true;
	return false;
}
// MiXOnly!
bool NMiXOnly::IsReached(const NMileStones * pSt)
{
	pStones = pSt;// Must be done in all IsReached functions
	if(GetCurState()->Geom.IsLine() &&
		fabs(GetCurState()->CurX) > MIND &&
		fabs(GetCurState()->CurX) < MIND &&
		fabs(GetCurState()->CurZ) < MIND)
		return true;
	return false;
}
// MiYOnly!
bool NMiYOnly::IsReached(const NMileStones * pSt)
{
	pStones = pSt;// Must be done in all IsReached functions
	if(GetCurState()->Geom.IsLine() &&
		fabs(GetCurState()->CurY) > MIND &&
		fabs(GetCurState()->CurX) < MIND &&
		fabs(GetCurState()->CurZ) < MIND)
		return true;
	return false;
}
// MiBookmark
bool NMiBookmark::IsReached(const NMileStones * pSt)
{
	pStones = pSt;// Must be done in all IsReached functions
	cadrID CurID = GetCurState()->GetCurCadrID();
	BProgram* pLoaProg = GetUnit()->GetCurLoaProg();
	if (!pLoaProg)
		return false;
	return pLoaProg->HasBookMark(CurID.Cadr);
}
// MiCollision
bool NMiCollision::IsReached(const NMileStones* pSt)
{
	pStones = pSt;// Must be done in all IsReached functions
	cadrID CurIDabs = GetCurState()->GetCurHCadrAbsEx();
	BProgram* pLoaProg = GetUnit()->GetCurLoaProg();
	if (!pLoaProg)
		return false;
	if (CurIDabs.Cadr >= UINT(pLoaProg->GetGeomArraySize()))
		return false;
	const auto pGeom = pLoaProg->GetCompGeomArray().GetAt(CurIDabs.Cadr);
	if (pGeom == nullptr)
		return false;
	const auto pDoc = GetUnit()->GetDoc();
	if (pDoc->GetCollManager().GetCollID(pGeom->GetStartTime(), pGeom->GetEndTime()) != nullptr)
		return true;
	return false;
}
// MiError
bool NMiError::IsReached(const NMileStones* pSt)
{
	pStones = pSt;// Must be done in all IsReached functions
	cadrID CurIDabs = GetCurState()->GetCurHCadrAbsEx();
	BProgram* pLoaProg = GetUnit()->GetCurLoaProg();
	if (!pLoaProg)
		return false;
	const auto& ErrHist = pLoaProg->GetErrorHistory();
	NProgError Err;
	if (ErrHist.Lookup(CurIDabs.Cadr + 1, Err))
		return true;
	return false;
}
bool NMiProgOptStop::IsReached(const NMileStones * pSt)
{
	pStones = pSt;// Must be done in all IsReached functions
	if(GetCurState()->ProgStop || GetCurState()->OptionalStop)
		return true;
	return false;
}
// MiSpindleSpeed!
bool NMiSpindleSpeed::IsReached(const NMileStones * pSt)
{
	pStones = pSt;// Must be done in all IsReached functions
	if(GetPrevState()->Spindle[SPINDLE_MILL].Speed != GetCurState()->Spindle[SPINDLE_MILL].Speed)
		return true;
	if(GetPrevState()->Spindle[SPINDLE_TURN].Speed != GetCurState()->Spindle[SPINDLE_TURN].Speed)
		return true;
	return false;
}
// MiSpindleForwardReverse!
bool NMiSpindleForwardReverse::IsReached(const NMileStones * pSt)
{
	pStones = pSt;// Must be done in all IsReached functions
	if(GetPrevState()->Spindle[SPINDLE_MILL].SpindleDir != GetCurState()->Spindle[SPINDLE_MILL].SpindleDir)
		return true;
	if(GetPrevState()->Spindle[SPINDLE_TURN].SpindleDir != GetCurState()->Spindle[SPINDLE_TURN].SpindleDir)
		return true;
	return false;
}
// MiFeed!
bool NMiFeed::IsReached(const NMileStones * pSt)
{
	pStones = pSt;// Must be done in all IsReached functions
	if(GetPrevState()->Feed != GetCurState()->Feed)
		return true;
	return false;
}
// MiCoolant!
bool NMiCoolant::IsReached(const NMileStones * pSt)
{
	pStones = pSt;// Must be done in all IsReached functions
	if(GetPrevState()->Coolant != GetCurState()->Coolant)
		return true;
	return false;
}
// MiPositioning!
bool NMiPositioning::IsReached(const NMileStones * pSt)
{
	pStones = pSt;// Must be done in all IsReached functions
	if(	GetPrevState()->CurCurveType != fast &&
		GetCurState()->CurCurveType == fast)
		return true;
	return false;
}
// MiCircularInterpolation!
bool NMiCircularInterpolation::IsReached(const NMileStones * pSt)
{
	pStones = pSt;// Must be done in all IsReached functions
	if(	GetPrevState()->CurCurveType != cwarc &&
		GetPrevState()->CurCurveType != ccwarc &&
		(GetCurState()->CurCurveType == cwarc ||
		 GetCurState()->CurCurveType == ccwarc))
		return true;
	return false;
}
// MiLinearInterpolation!
bool NMiLinearInterpolation::IsReached(const NMileStones * pSt)
{
	pStones = pSt;// Must be done in all IsReached functions
	if(	GetPrevState()->CurCurveType != line &&
		GetCurState()->CurCurveType == line)
		return true;
	return false;
}
// MiCycle!
bool NMiCycle::IsReached(const NMileStones * pSt)
{
	pStones = pSt;// Must be done in all IsReached functions
	if(GetPrevState()->CurCycle != GetCurState()->CurCycle)
		return true;
	return false;
}
// MiSubprogramCall!
bool NMiSubprogramCall::IsReached(const NMileStones * pSt)
{
	pStones = pSt;// Must be done in all IsReached functions
	static int StackLength = 0;
	if(!GetCurState()->ProgramStarted)
	{
		StackLength = 0;
		return false;
	}
	bool ret = GetUnit()->GetCallStackSize() > StackLength;
	StackLength = GetUnit()->GetCallStackSize();
	return ret && GetCurState()->OrderChanged;
}
// MiSubprogramStartEnd!
bool NMiSubprogramStartEnd::IsReached(const NMileStones * pSt)
{
	pStones = pSt;// Must be done in all IsReached functions
	static int StackLength = 0;
	if(!GetCurState()->ProgramStarted)
	{
		StackLength = 0;
		return false;
	}
	bool ret = GetUnit()->GetCallStackSize() < StackLength;
	StackLength = GetUnit()->GetCallStackSize();
	return ret && GetCurState()->OrderChanged;
}
// MiToolLengthCompensationActive!
bool NMiToolLengthCompensationActive::IsReached(const NMileStones * pSt)
{
	pStones = pSt;// Must be done in all IsReached functions
	if(GetPrevState()->CurZComp == UD_OFF &&
		GetCurState()->CurZComp != UD_OFF)
		return true;
	return false;
}
// MiToolLengthCompensationCancel!
bool NMiToolLengthCompensationCancel::IsReached(const NMileStones * pSt)
{
	pStones = pSt;// Must be done in all IsReached functions
	if(GetPrevState()->CurZComp != UD_OFF &&
		GetCurState()->CurZComp == UD_OFF)
		return true;
	return false;
}
// MiToolDiameterCompensationActive!
bool NMiToolDiameterCompensationActive::IsReached(const NMileStones * pSt)
{
	pStones = pSt;// Must be done in all IsReached functions
	if(GetPrevState()->CurDirComp == LR_OFF &&
		GetCurState()->CurDirComp != LR_OFF)
		return true;
	return false;
}
// MiToolDiameterCompensationCancel!
bool NMiToolDiameterCompensationCancel::IsReached(const NMileStones * pSt)
{
	pStones = pSt;// Must be done in all IsReached functions
	if(GetPrevState()->CurDirComp != LR_OFF &&
		GetCurState()->CurDirComp == LR_OFF)
		return true;
	return false;
}
// MiLifeTime!
bool NMiLifeTime::IsReached(const NMileStones * pSt)
{
	pStones = pSt;// Must be done in all IsReached functions
	if(	GetCurState()->GetRestLifeTime() < 0.)
		return true;
	return false;
}

// MiCheckZeroFeed!
bool NMiCheckZeroFeed::IsReached(const NMileStones * pSt)
{
	pStones = pSt;// Must be done in all IsReached functions
	if( !GetCurState()->Geom.IsFast() &&
		!GetCurState()->Geom.IsUndef() &&
		GetCurState()->Feed == 0. )
		return true;
	return false;
}
int NMiCheckZeroFeed::GetCode(void) const
{
	return 1154;
}
// MiCheckLengthComp
bool NMiCheckLengthComp::IsReached(const NMileStones * pSt)
{
	pStones = pSt;// Must be done in all IsReached functions
	static bool ToolChanged = false;
	if((GetPrevState()->CurToolN != GetCurState()->CurToolN)
		|| (GetPrevState()->CurTurret != GetCurState()->CurTurret))
		ToolChanged = true;
	if(ToolChanged)
	{
		if(GetCurState()->Geom.IsLine() &&
			fabs(GetCurState()->CurZ) > MIND )
		{// Z movement
			ToolChanged = false;
			if(GetCurState()->CurZComp == UD_OFF)
				return true;
		}
	}
	return false;
}
int NMiCheckLengthComp::GetCode(void) const
{
	return 1088;
}
// MiCheckDiamCompOff
bool NMiCheckDiamCompOff::IsReached(const NMileStones * pSt)
{
	pStones = pSt;// Must be done in all IsReached functions
	bool ToolChanged = false;
	if((GetPrevState()->CurToolN != GetCurState()->CurToolN)
		|| (GetPrevState()->CurTurret != GetCurState()->CurTurret))
		ToolChanged = true;
	if(ToolChanged && GetCurState()->CurDirComp != LR_OFF)
		return true;
	return false;
}
int NMiCheckDiamCompOff::GetCode(void) const
{
	return 1117;
}
// MiCheckFeedTurnSpindleTurnOff
bool NMiCheckFeedTurnSpindleTurnOff::IsReached(const NMileStones * pSt)
{
	pStones = pSt;// Must be done in all IsReached functions
	if(GetCurState()->Geom.HaveGeom() &&  !GetCurState()->Geom.IsFast() && GetUnit()->IsToolTurn()
		&& (GetCurState()->Spindle[SPINDLE_TURN].SpindleDir == R_UNDEF || GetCurState()->Spindle[SPINDLE_TURN].Speed == 0.))	
		return true;
	return false;
}
int NMiCheckFeedTurnSpindleTurnOff::GetCode(void) const
{
	return 1153;
}

// MiCheckFeedTurnSpindleMillOn
bool NMiCheckFeedTurnSpindleMillOn::IsReached(const NMileStones * pSt)
{
	pStones = pSt;// Must be done in all IsReached functions
	if(GetCurState()->Geom.HaveGeom() &&  !GetCurState()->Geom.IsFast() && GetUnit()->IsToolTurn()
		&& (GetCurState()->Spindle[SPINDLE_MILL].SpindleDir != R_UNDEF && GetCurState()->Spindle[SPINDLE_MILL].Speed != 0.))
		return true;
	return false;
}
int NMiCheckFeedTurnSpindleMillOn::GetCode(void) const
{
	return 1183;
}
// MiCheckFeedMillSpindleMillOff
bool NMiCheckFeedMillSpindleMillOff::IsReached(const NMileStones * pSt)
{
	pStones = pSt;// Must be done in all IsReached functions
	if(GetCurState()->Geom.HaveGeom() &&  !GetCurState()->Geom.IsFast() && GetUnit()->IsToolMill()
		&& (GetCurState()->Spindle[SPINDLE_MILL].SpindleDir == R_UNDEF || GetCurState()->Spindle[SPINDLE_MILL].Speed == 0.))	
		return true;
	return false;
}
int NMiCheckFeedMillSpindleMillOff::GetCode(void) const
{
	return 1184;
}
// MiCheckFeedMillSpindleTurnOn
bool NMiCheckFeedMillSpindleTurnOn::IsReached(const NMileStones * pSt)
{
	pStones = pSt;// Must be done in all IsReached functions
	if(GetCurState()->Geom.HaveGeom() &&  !GetCurState()->Geom.IsFast() && GetUnit()->IsToolMill()
		&& (GetCurState()->Spindle[SPINDLE_TURN].SpindleDir != R_UNDEF && GetCurState()->Spindle[SPINDLE_TURN].Speed != 0.))	
		return true;
	return false;
}
int NMiCheckFeedMillSpindleTurnOn::GetCode(void) const
{
	return 1185;
}
// MiCheckLengthOffsetNum
bool NMiCheckLengthOffsetNum::IsReached(const NMileStones * pSt)
{
	pStones = pSt;// Must be done in all IsReached functions
	if(GetCurState()->CurZComp == UD_OFF)
		return false;
	if(GetCurState()->CurLengthCompNum == GetCurState()->CurToolN.GetToolNum())
		return false;
	return true;
}
int NMiCheckLengthOffsetNum::GetCode(void) const
{
	return 1094;
}
// MiCheckDiamOffsetNum
bool NMiCheckDiamOffsetNum::IsReached(const NMileStones * pSt)
{
	pStones = pSt;// Must be done in all IsReached functions
	if(GetCurState()->CurDirComp == LR_OFF)
		return false;
	int AddNum = 30;
	CString str;
	if(GetUnit()->GetOtherValue("NumForDiamOffset", str))
		AddNum = atoi(str);
	if(int(GetCurState()->GetRegister(4107) + 0.5) == GetCurState()->CurToolN.GetToolNum() + AddNum)
		return false;
	return true;
}
int NMiCheckDiamOffsetNum::GetCode(void) const
{
	return 1095;
}
// MiCheckIJKRDef
bool NMiCheckIJKRDef::IsReached(const NMileStones * pSt)
{
	pStones = pSt;// Must be done in all IsReached functions
	if(GetCurState()->CurCurveType == cwarc || GetCurState()->CurCurveType == ccwarc)
	{
		return !GetUnit()->ChangedIJKR();
	}
	return false;
}
int NMiCheckIJKRDef::GetCode(void) const
{
	return 1089;
}

// MiCheckXYChamf
bool NMiCheckXYChamf::IsReached(const NMileStones * pSt)
{
	pStones = pSt;// Must be done in all IsReached functions
	if(GetCurState()->Chamfer.GetType() != OFF)
	{
		if(!GetCurState()->ChangedCoord.x || !GetCurState()->ChangedCoord.y)
			return true;
		return false;
	}
	else if(GetPrevState()->Chamfer.GetType() != OFF)
	{
		if(!GetCurState()->ChangedCoord.x || !GetCurState()->ChangedCoord.y)
			return true;
		return false;
	}
	return false;
}
int NMiCheckXYChamf::GetCode(void) const
{
	return 1112;
}
int NMileStone::GetCode(void) const
{
	return 0;
}

const NCUnitStatePrev *NMileStone::GetPrevState() const
{
	return pStones->pPrevState;
}

NCUnitState *NMileStone::GetCurState() const
{
	return pStones->pUnit->GetState();
}

NCUnit *NMileStone::GetUnit() const
{
	return pStones->pUnit;
}
