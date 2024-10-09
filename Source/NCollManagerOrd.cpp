#include "stdafx.h"
#include "NCMDoc.h"
#include "NCUnit.h"
#include "MTPart.h"
#include "NCollSettingsDlg.h"
#include "MTMashineTool.h"
#include "WM_APP_Messages.h"
#include "NCollRange.h"
#include "NCMProject.h"
#include "NOMScaner.h"
#include "BObjectManager.h"
#include "NMachUnitPair.h"
#include "NCadrSpeedPar.h"
#include "GDynamicCadr.h"
#include "MTConfig.h"
#include "NCollManagerOrd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CCriticalSection NCollManagerOrd::CritSection;

NCollManagerOrd::NCollManagerOrd(CNCMDoc* pD) : NCollManager(pD)
{
	CheckType = 2;
	BackMode = false;
	pCollCheckThread = nullptr;
	RestartTimerID = -1;
	BackCheckCompleted = false;
	pCollRange = nullptr;
	pMainWnd = nullptr;
	ToolStockActive = false;
	ToolStockMode = CWS_CUT;
	LogIsStale = true;
	CuttingDisabled = false;
}


NCollManagerOrd::~NCollManagerOrd()
{
}

void NCollManagerOrd::SetToolStockActive(bool Val)
{
	ToolStockActive = Val;
	if (ToolStockActive)
		NTool::SetToolStockCollision(ToolStockMode);
	else
		NTool::SetToolStockCollision(CWS_OFF);
	NCMProject::GetProject().GetGlobalState().SetMultiThreadDX_And(NTool::GetToolStockCollision() == CWS_OFF);
}

void NCollManagerOrd::SetToolStockMode(CollisWithStock Val)
{
	ToolStockMode = Val;
	SetToolStockActive(ToolStockActive);
}

void NCollManagerOrd::StopCreating()
{
	CollTimeLine.StopCreating();
}

int NCollManagerOrd::MakeCollisionScene(void)
{
	const auto res = NCollManager::MakeCollisionScene();
	// Add stocks
	MakeStocks();
	// Make sensitive pairs
	const std::set<std::pair<int, int>>& Enemies = pMainMach->GetEnemies();
	for each (auto Pair in Enemies)
	{
		if (Pair.first == Pair.second)
			continue;
		if (pMainMach->GetNearestMoveRotAxisInd(Pair.first) == pMainMach->GetNearestMoveRotAxisInd(Pair.second))
			continue;
		pCollisionScene->AddSensitivePair(CalcBehav(Pair.first, false, false), CalcBehav(Pair.second, false, false));
	}
	return res;
}
bool NCollManagerOrd::AskSettings()
{
	NCollSettingsDlg Dlg(CuttingDisabled);
	Dlg.m_CheckStep = CheckStep;
	Dlg.m_MargFix = MarginFixture;
	Dlg.m_MargTool = MarginTool;
	Dlg.m_MargStock = MarginStock;
	Dlg.m_MargMach = MarginMach;
	Dlg.m_MargPart = MarginPart;
	Dlg.m_MaskFix = ((GroupMask & OG_FIXTURES) != 0);
	Dlg.m_MaskTool = ((GroupMask & OG_TOOLS) != 0);
	Dlg.m_MaskStock = ((GroupMask & OG_STOCKS) != 0);
	Dlg.m_MaskMach = ((GroupMask & OG_MACHINE) != 0);
	Dlg.m_MaskPart = ((GroupMask & OG_PARTS) != 0);
	Dlg.m_MaskHold = ((GroupMask & OG_HOLD) != 0);
	Dlg.m_MaskCut = ToolStockMode == CWS_CUT /* || ToolStockMode == CWS_ALL */;
//	Dlg.m_MaskHold = ToolStockMode == CWS_ALL;
	Dlg.m_TurnOffset = NTool::GetTurnOffset();
	Dlg.m_XYOffset = NTool::GetXYOffset();
	Dlg.m_ZOffset = NTool::GetZOffset();
	Dlg.m_XYOffsetSh = NTool::GetXYOffsetSh();
	Dlg.m_ZOffsetSh = NTool::GetZOffsetSh();
	Dlg.m_NotIgnoreLimits = !GetIgnoreWrongAxis();
	Dlg.m_IgnoreUp = NTool::GetIgnoreUpToolStockColl();
	bool ParametersChanged = false;
	if (Dlg.DoModal() != IDOK)
		return false;

	AbortBack();

	if (CheckStep != Dlg.m_CheckStep)
	{
		CheckStep = Dlg.m_CheckStep;
//		NSpeedPar::Intern.SetLengthStep(CheckStep);
	}
	if (MarginFixture != Dlg.m_MargFix)
	{
		ParametersChanged = true;
		MarginFixture = Dlg.m_MargFix;
	}
	if (MarginTool != Dlg.m_MargTool)
	{
		ParametersChanged = true;
		MarginTool = Dlg.m_MargTool;
	}
	if (MarginStock != Dlg.m_MargStock)
	{
		ParametersChanged = true;
		MarginStock = Dlg.m_MargStock;
	}
	if (MarginMach != Dlg.m_MargMach)
	{
		ParametersChanged = true;
		MarginMach = Dlg.m_MargMach;
	}
	if (MarginPart != Dlg.m_MargPart)
	{
		ParametersChanged = true;
		MarginPart = Dlg.m_MargPart;
	}
	const int bGroupMask = GroupMask;
	GroupMask = OG_GR_EMPTY;
	if (Dlg.m_MaskFix) GroupMask |= OG_FIXTURES;
	if (Dlg.m_MaskTool) GroupMask |= OG_TOOLS;
	if (Dlg.m_MaskStock) GroupMask |= OG_STOCKS;
	if (Dlg.m_MaskMach) GroupMask |= OG_MACHINE;
	if (Dlg.m_MaskPart) GroupMask |= OG_PARTS;
	if (Dlg.m_MaskHold) GroupMask |= OG_HOLD;
	if (GroupMask != bGroupMask)
		ParametersChanged = true;
	SetToolStockMode(Dlg.m_MaskCut ? CWS_CUT : CWS_OFF);
	//	if (Dlg.m_MaskHold) SetToolStockMode(CWS_ALL);
	if(NTool::GetTurnOffset() != Dlg.m_TurnOffset 
		|| NTool::GetXYOffset() != Dlg.m_XYOffset
		|| NTool::GetZOffset() != Dlg.m_ZOffset
		|| NTool::GetXYOffsetSh() != Dlg.m_XYOffsetSh
		|| NTool::GetZOffsetSh() != Dlg.m_ZOffsetSh
		)
		ParametersChanged = true;
	NTool::SetTurnOffset(Dlg.m_TurnOffset);
	NTool::SetXYOffset(Dlg.m_XYOffset);
	NTool::SetZOffset(Dlg.m_ZOffset);
	NTool::SetXYOffsetSh(Dlg.m_XYOffsetSh);
	NTool::SetZOffsetSh(Dlg.m_ZOffsetSh);
	SetIgnoreWrongAxis(Dlg.m_NotIgnoreLimits == FALSE);
	NTool::SetIgnoreUpToolStockColl(Dlg.m_IgnoreUp);

	if (ParametersChanged)
	{
		ResetCheckType();
	}
	LogIsStale = true;
	return true;
}

bool NCollManagerOrd::ChangeCheckType(MTMachineTool& MT)
{
	int NewCheckType = CheckType == 2 ? 0 : 2;
	NewCheckType = (NewCheckType == 2) ? 2 : (BackMode ? 0 : 1);
	return SetCheckType(NewCheckType, MT);
}

bool NCollManagerOrd::SetCheckType(int NewCheckType, const MTMachineTool& MT)
{
	Reset(MT);
	switch (NewCheckType)
	{
	case 0:// Back
		Prepare(false);
		break;
	case 1:// Fore
	{
		ClearLogs();
		Prepare(true);
	}
		break;
	case 2:// Disable
//		ClearLogs();
		break;
	default:
		return false;
	}
	CheckType = NewCheckType;
	return true;
}

bool NCollManagerOrd::ResetCheckType()
{
	return SetCheckType(CheckType, *pMainMach);
}

void NCollManagerOrd::ClearLogs(void)
{
	LogIsStale = true;
	BackLog.Clear();
	ForeLog.Clear();
	CollTimeLine.Clear();
	BackCheckCompleted = false;

	CWnd* pMWnd = GetMyMainWnd();
	if (pMWnd != nullptr)
		if(::IsWindow(pMainWnd->GetSafeHwnd()))
			pMWnd->SendMessage(WM_APP_COLLCHECK_ADDLOG, 0);// Needed to clear report pane
}

PObjectStatus NCollManagerOrd::OnStatusChanged(int ObjectInd, PObjectStatus NewStatus)
{
	if (IsBackType())
	{
		if (NewStatus != ST_EMPTY)
		{
			Disable((NewStatus == DELETED) ? ObjectInd : -1);// Исключаем вызов Highlight для уже удаленного объекта
			CheckType = 0;
			BeginRestart();
		}
	}
	LogIsStale = true;
	if (NewStatus == DELETED)
	{
		for (NOMScaner Sc(NCMProject::GetProject().GetObjectManager(), this); !Sc.IsEnded(); Sc.Next())
		{
			if (Sc.GetPassiveInd() == ObjectInd)
				CollTimeLine.RemovePointer(static_cast<NCollidableObject*>(Sc.GetPassiveObject()));
		}
		return ST_EMPTY;
	}
	return USED;
}

int NCollManagerOrd::StartBack(cadrID& StartHIDAbs, cadrID& EndHIDAbs, NMachUnitPair* pMUPair)
{
	AbortBack();
	ClearLogs();

	if (CheckType != 0 || !pMainMach)
		return -1;

	pCollRange = new NCollRange(pGDoc);
	pCollRange->Set(StartHIDAbs, EndHIDAbs, IgnoreWrongAxis);
	pCollCheckThread = pCollRange->StartChecking(pMUPair);
	return 0;
}

int NCollManagerOrd::HaveStockCollisions() const
{ // returns 1 - have non stock collision, 2 - have stock collision, 3 - both
	//int Res = 0;
	//const auto& ActColl = pCollisionScene->GetActiveCollisions();
	//for each (auto Ind in ActColl)
	//{
	//	const NCollisionObjectID* pObjID = pCollisionScene->GetObjID(Ind);
	//	if (pObjID == nullptr)
	//		continue;
	//	Res |= (pObjID->GetGroup() == OG_STOCKS) ? 2 : 1;
	//}
	return pCollisionScene->GetDiskr();
}

void NCollManagerOrd::AddCutCollision(const BRangePosColl& PosColl)
{
	CollTimeLine.Add(NCollisionID(PosColl.GetStartTPoint(), nullptr, nullptr, PosColl.GetCollType()));
}

void NCollManagerOrd::PrepTimeLine(MainTime LastTime)
{
	CollTimeLine.Sort(LastTime);
}

const NCollisionID* NCollManagerOrd::GetCollID(MainTime intStart)
{
	return CollTimeLine.GetCollID(intStart);
}

const NCollisionID* NCollManagerOrd::GetCollID(MainTime intStart, MainTime intEnd)
{
	return CollTimeLine.GetCollID(intStart, intEnd);
}

bool NCollManagerOrd::Add2Log(const NCollisionID& CollID)
{
	CollTimeLine.Add(CollID);

	if (CollID.CollType == LE_NOCOLL)// fix time without collision
		return true;

	NCollisionLog* pLog = GetActiveLog();
	if (!pLog)
		return false;

	const auto CadrAbs = static_cast<cadrID>(CollID.PTPoint);
	const auto inType = CollID.CollType;
	int pos = pLog->FindElem(CadrAbs);
	if (pos < 0)
		return false;

	CString CollNote;
	switch (inType)
	{
	case LE_COLLISION:
		if (pCollisionScene != nullptr)
		{
			const auto& ActColl = pCollisionScene->GetActiveCollisions();
			for each (auto Ind in ActColl)
			{
				const NCollisionObjectID* pObjID = pCollisionScene->GetObjID(Ind);
				if (pObjID == nullptr)
					continue;
				const MTAxis* pAxis = pMainMach->GetCurConfig()[pObjID->GetID()];
				if (pAxis == nullptr)
					continue;
				CString b;
				switch (pAxis->GetType())
				{
				case MTAxis::AT_VTOOL:
				{
					const auto pCO = pObjID->GetCollidableObj();
					const NToolCombined* pTool = dynamic_cast<const NToolCombined*>(pCO);
					if (pTool != nullptr)
					{
						NToolID ToolID;
						ToolID.Set(pTool->Position, pTool->Turret);
						CString Val;
						Val.Format("%s", (ToolID.GetPrintStr() + _T(" ") + pTool->GetName()).GetString());
						b.Format("Tool:%s", Val.GetString());
					}
					break;
				}
				case MTAxis::AT_STOCK:
				{
					if (pObjID->GetGroup() == OG_PARTS || pObjID->GetGroup() == OG_FIXTURES)
					{
						const NCollidableObject* pCO = pObjID->GetCollidableObj();
						const class NStock* pStock = dynamic_cast<const class NStock*>(pCO);
						if (pStock != nullptr)
						{
							CString FileName = pStock->GetFileName();
							const int index = FileName.ReverseFind('\\');
							int ind = FileName.ReverseFind('.');
							ind = ind >= 0 ? ind : FileName.GetLength();
							CString ShortFileName = FileName.Mid(index + 1, ind - index - 1);
							b.Format("File:%s", ShortFileName.GetString());
						}
					}
					else
						b.LoadString(IDS_STOCK);
					break;
				}
				case MTAxis::AT_TOOL:
				{
					const NCollidableObject* pCO = pObjID->GetCollidableObj();
					const NToolCombined* pTool = dynamic_cast<const NToolCombined*>(pCO);
					if (pTool != nullptr)
					{
						NToolID ToolID;
						ToolID.Set(pTool->Position, pTool->Turret);
						CString Val;
						Val.Format("%s", (ToolID.GetPrintStr() + _T(" ") + pTool->GetName()).GetString());
						b.Format("Active tool:%s", Val.GetString());
					}
					break;
				}
				}
				if (b.IsEmpty())
				{
					CString text;
					text = "Axis:%s;";
					b.Format(text, pAxis->GetName());
				}
				CollNote += b;
			}
		}
		break;
	case LE_OUTOFRANGE:
		{
			const int AxisInd = CollID.IDObj1.GetID();
			if (AxisInd >= 0 && AxisInd < pMainMach->GetCurConfig().GetSize())
			{
				const MTAxis* pAxis = pMainMach->GetCurConfig()[AxisInd];
				if (pAxis != nullptr)
				{
					CString Val;
					Val.Format("%s (%-6.1f:%-6.1f)", pAxis->GetName().GetString(), pAxis->GetMin(), pAxis->GetMax());
					CString text;
					text = "Out of range on %s axis";
					CollNote.Format(text,  Val);
				}
			}
		}
		break;
	}
	NCollLogElem& CollLogElem = *new NCollLogElem(CollID.PTPoint, CollID.PTPoint, inType);
	CollLogElem.SetCollNote(std::string(CollNote));
	if (pLog->AddElem(pos, CollLogElem) && pMainWnd != nullptr)
		pMainWnd->PostMessage(WM_APP_COLLCHECK_ADDLOG, 1, (LPARAM)&CollLogElem);
	else
		delete& CollLogElem;
	return true;
}

NCollisionLog* NCollManagerOrd::GetActiveLog()
{
	switch (CheckType)
	{
	case 0:
		return &BackLog;
	case 1:
		return &ForeLog;
	default:
		return NULL;
	}

}

void NCollManagerOrd::BeginRestart()
{
	StopRestart();
	BackLog.Clear();
	CWnd* pMWnd = GetMyMainWnd();
	if (pMWnd != nullptr)
	{
		pMWnd->SendMessage(WM_APP_COLLCHECK_ADDLOG, 0);// Needed to clear report pane
		RestartTimerID = int(pMWnd->SetTimer(T_COLLRESTART, RestartTimeout, lpfnTimer));
	}
}

void NCollManagerOrd::StopRestart()
{
	if (IsRestarting())
	{
		CWnd* pMWnd = GetMyMainWnd();
		if (pMWnd != nullptr)
			pMWnd->KillTimer(RestartTimerID);
		RestartTimerID = -1;
	}
}

bool NCollManagerOrd::IsBackCheckInProgress()
{
	return pCollCheckThread != NULL;
}

bool NCollManagerOrd::IsCollisionFound()
{
	const NCollisionLog* pLog = GetForeLog();
	if (pLog == nullptr)
		return false;
	return pLog->GetSize() != 0;
}

void NCollManagerOrd::AbortBack(void)
{
	StopRestart();

	if (!IsBackCheckInProgress())
		return;

	if (!pCollisionScene)
		return;

	if (!pCollRange)
		return;

	pCollRange->Abort();
	CollcheckStopped(true);
}

void NCollManagerOrd::RestartTimeoutElapsed(class MTMachineTool& MT)
{
	Reset(MT);
	Prepare(false);
	CheckType = 0;
	BackMode = true;
	pGDoc->FullCollisionDetection();
}

void NCollManagerOrd::lpfnTimer(HWND, UINT, UINT_PTR, DWORD)
{
	//pGDoc->GetCollManager().StopRestart();
	//pGDoc->GetCollManager().RestartTimeoutElapsed(pGDoc->GetMachTool());
}


void NCollManagerOrd::CollcheckStopped(bool Aborted)
{
	if (pCollCheckThread)
		WaitForSingleObject(pCollCheckThread->m_hThread, INFINITE);
	delete pCollCheckThread;
	pCollCheckThread = NULL;
	delete pCollRange;
	pCollRange = NULL;
	if (!Aborted)
		BackCheckCompleted = true;
}

bool NCollManagerOrd::Disable(int ExcludeInd)
{
	if (CheckType == 2)// Disabled
		return false;
	AbortBack();
	// ExcludeInd - passive index to exclude from processing. Corresponding object may be already deleted
	for (NOMScaner Sc(NCMProject::GetProject().GetObjectManager(), this); !Sc.IsEnded(); Sc.Next())
	{
		if (Sc.GetPassiveInd() != ExcludeInd)
			Sc.GetPassiveObject()->SetCollided(false);
	}
	Clear();
	CheckType = 2;// Disable
	pMainMach = NULL;
//	LogIsStale = false;
	return true;
}

void NCollManagerOrd::ClearCollided()
{
	for (NOMScaner Sc(NCMProject::GetProject().GetObjectManager(), this); !Sc.IsEnded(); Sc.Next())
			Sc.GetPassiveObject()->SetCollided(false);
}

void NCollManagerOrd::Prepare(bool Fore)
{
	IgnoreMargins = false;

	if (pCollisionScene && !LogIsStale)
		return;

	if (!pMainMach)
		return;

	C_OBJECT_MANAGER((*this))->AddActiveObject(this, 1, GroupMask | OG_STOCKS | OG_TOOLS | OG_MACHINE); // Changing of a stock, tool or machine must lead to restart

	pCollisionScene = NCollisionScene::GenCollisionScene(this, CollideDef);
	bool HaveStock = ((GroupMask & OG_STOCKS) != 0);
	if (!Fore)
	{// for back mode only
		if (HaveStock)
			GroupMask &= ~OG_STOCKS;// remove OG_STOCKS
	}
	MakeCollisionScene();
	if (!Fore)
	{// for back mode only
		if (HaveStock)
			GroupMask |= OG_STOCKS;// return OG_STOCKS
	}
	LogIsStale = false;
}

bool NCollManagerOrd::CheckCurPosition(MTMachineTool* pMach, const class NToolCombined* pActiveTool
	, const ProgTPoint& PTPoint, bool IgnoreLimits, bool DontFixStock, bool IsFast5X)
{
	if (!pCollisionScene)
		return false;

	pCollisionScene->ClearActiveCollisions();
	pCollisionScene->SetActiveCollisionsEnabled(true);
	pCollisionScene->SetFixStock(!DontFixStock);

	CSingleLock singleLock(&CritSection);
	singleLock.Lock();

	ChangeTool(pCollisionScene, *pMach, pActiveTool);

	const int WrongAxis = pMach->ForceActual();// place it here to prevent multiple calls to MakeActual

	if (!SetCurMatrices(pCollisionScene, pMach, pActiveTool))
		return false;

	ChangeActiveToolResp(pCollisionScene, IsFast5X, false);
	pCollisionScene->SetCadrIDHist(PTPoint);

	bool res = (pCollisionScene->RunDetection() > 0);
	
	if (!res && !IgnoreLimits)
		if (ProcWrongAxis(WrongAxis, pMach, PTPoint))
			res = true;

	ChangeActiveToolResp(pCollisionScene, IsFast5X, true);
	pCollisionScene->SetActiveCollisionsEnabled(false);
	pCollisionScene->SetFixStock(true);
	return res;
}

bool NCollManagerOrd::ProcWrongAxis(int WrongAxis, MTMachineTool* pMach, const ProgTPoint& PTPoint)
{
	if (WrongAxis >= 0) // axis value doesn't belong to the range
	{
		// Find direct children of WrongAxis
		const MTPartArray& Parts = pMach->GetParts();
		const CString& WrongAxisName = pMach->GetCurConfig()[WrongAxis]->GetName();
		bool Found = false;
		for (int i = 0; i < Parts.GetSize(); ++i)
		{
			const CString& AxisName = Parts[i]->GetAxisName();
			const CString& ParentName = pMach->GetCurConfig().GetAxis(AxisName)->GetParent();
			if (ParentName == WrongAxisName)
			{
				NCollisionObjectID Obj1ID;
				Obj1ID.SetGroup(OG_MACHINE);
				Obj1ID.SetID(WrongAxis);
				Obj1ID.SetSelfNum(i);
				Obj1ID.SetCollidableObj(Parts[i]);
				NCollisionID CollID(PTPoint, &Obj1ID, nullptr, LE_OUTOFRANGE);
				AddCollisionMark(CollID);
				Found = true;
			}
		}
		if (!Found)
		{
			NCollisionObjectID Obj1ID;
			Obj1ID.SetGroup(OG_MACHINE);
			Obj1ID.SetID(WrongAxis);
			Obj1ID.SetSelfNum(-1);
			Obj1ID.SetCollidableObj(nullptr);
			NCollisionID CollID(PTPoint, &Obj1ID, nullptr, LE_OUTOFRANGE);
			AddCollisionMark(CollID);
		}
		return true;
	}
	return false;
}

void NCollManagerOrd::SetMainWnd(CWnd* pWnd)
{
	pMainWnd = pWnd;
}

CWnd* NCollManagerOrd::GetMyMainWnd()
{
	if (pMainWnd != nullptr)
		return pMainWnd;
	return AfxGetMainWnd();
}

void NCollManagerOrd::AddCollisionMark(NCollisionID& CollID)
{
	if (CollID.HasObj())
		GetScene()->AddActiveCollision(CollID);
	if (GetScene()->GetFixStock() || !CollID.HasStock())
		Add2Log(CollID);
}

int NCollManagerOrd::CollideDef(void* client_data, void* obj1, void* obj2, const void* coll_data)
{
	if (client_data == nullptr)
		return 0;// DT_CONTINUE;
	// obj1 and obj2 are indexes in ObjectIDs not pointers
	NCollManagerOrd* pCollMan = (NCollManagerOrd*)client_data;
	NCollisionScene* pScene = pCollMan->GetScene();
	pCollMan->AddCollisionMark(NCollisionID(pScene->GetCadrIDAbs(), pScene->GetObjID(size_t(obj1)), pScene->GetObjID(size_t(obj2)), LE_COLLISION));
	return 0;// DT_CONTINUE;
}

