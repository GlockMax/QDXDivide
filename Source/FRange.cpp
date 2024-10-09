#include "stdafx.h"
#include "math.h"
#include "NMachUnitPair.h"
#include "NCUnit.h"
#include "NToolStd.h"
#include "NCM.h"
#include "ConstDef.h"
#include "BSpacePos.h"
#include "NCadrGeom.h"
#include "BGeomArray.h"
#include "BProgram.h"
#include "NToolCombined.h"
#include "GAnimateCadrRot.h"
#include "GAnimateCadrMill.h"
#include "MQuadTreeRot.h"
#include "NAnimState.h"
#include "FAnimateDisp.h"
#include "GQuadRenderRot.h"
#include "GQuadRenderMill.h"
#include "GQuadRenderDX5X.h"
#include "NCMDoc.h"
#include "NCUnitStatePrev.h"
#include "FProg2BSP.h"
#include "GOneStepGeom.h"
#include "mmsystem.h"
#include "MLocStorage.h"
#include "NChannel.h"
#include "DXSubModel5X.h"
#include "NCollFragment.h"
#include "frange.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

class NStock;

FRange::FRange(class NChannel* pChan, const NSpeedPar* piSpeedPar, int PN, const BProgram* pPr, int StartH, int EndH)
	: RangePos(*pPr)
{
	pChannel = pChan;
	pSpeedPar = piSpeedPar;
	pFRTool = nullptr;
	pWorkTool = nullptr;
	AnimMode = NGlobalState::Anim_no;
	StockState = AT_UNDEF;
	pProg = pPr;
	RangePos.ProgNum = PN;
	StartHCadr = StartH;
	EndHCadr = EndH;
	CuttingTypeChanged = false;
	DefineType();
}

FRange::~FRange(void)
{
}

void FRange::SetMT2CurPosAddCoord(const NCadrGeom& Geom)
{
	NMachUnitPair MUPair(pChannel->GetMachCopy(), *pChannel->GetUnitCn());
	if (!pProg)
		return;
	if (Geom.IsUndef())
		return;
	// Calc XYZ ABC
	BMatr MCS = MUPair.GetMach().GetMCS(&Geom, pFRTool, MUPair.GetUnit().GetFixedDef());
	BPoint XYZ(0., 0., 0., 1.);
	BPoint ABC(0., 0., 0., 1.);
	Geom.CalcCurCoordMCS(0., MCS, XYZ, ABC);

	if (MUPair.GetUnit().GetAddCoordsDef().GetSize() > 0)
	{
		SMachState AddCoord(&MUPair.GetUnit().GetAddCoordsDef());
		pProg->GetAddCoord(Geom.GetMSHistNum(), AddCoord);
		MUPair.GetMach().SetStatePartial(Geom.GetMTLink(), SMachState(XYZ, ABC, &MUPair.GetUnit().GetFixedDef()), AddCoord);
		return;
	}
	MUPair.GetMach().SetState(Geom.GetMTLink(), SMachState(XYZ, ABC, &MUPair.GetUnit().GetFixedDef()));
}

FRange::intPair FRange::AnimateRangeMultiStocks(CWnd* pMainWnd)
{
	NCollManagerOrd& CollM = GetDoc()->GetCollManager();
	if (pFRTool == NULL && AnimMode == NGlobalState::Result && !CollM.IsForeType())
		return intPair(EndHCadr, EndHCadr);
	if (StockState == AT_MILL)
		pChannel->GetProg2BSP().StartCutting1DirThreads();
	int TurretNum = 0, AttachNum = 0;
	if (pFRTool)
	{
		TurretNum = pFRTool->Turret;
		AttachNum = pFRTool->Attach;
	}
	const BGeomArray& ProgGeom = pProg->GetCompGeomArray(0);
	// Skip cadrs before StartHCadr
	int i = 0;
	int k = 0;
	for (; k < StartHCadr && i < ProgGeom.GetSize(); i++)
	{
		// k - index of current step excluding steps in the cycles
		// i - index of current step
		NCadrGeom* pGeom = ProgGeom.GetAt(i);
		if (!pGeom->IsCycle())// Cycle attrib
			k++;
	}
	// Get cadrs from StartHCadr to EndHCadr

	RangePos.kStart = 0;
	RangePos.iStart = 0;
	bool NeedBreak = false;
	RangePos.StepParamPerCent = 0.;
	int iRet = 0;// A number of processed GeomArr elements
	while (k < EndHCadr && i < ProgGeom.GetSize())
	{// Divide ProgGeom to fragments by ABC changing points, 5x cadrs and time limit (Max4ReRendering)
		RangePos.kStart = k;// Start history position of new GeomArr
		RangePos.iStart = i;// Start ProgGeom position for new GeomArr
		//kStart, iStart are used in FindCurHCadr
		CArray<NCadrGeomArray> GeomArrays;
		GeomArrays.SetSize(pProg->GetGeomArraysCount());
		NCadrGeom* pGeom = ProgGeom.GetAt(i);
		int ii = i;
		for (int gi = 0; gi < GeomArrays.GetSize(); ++gi)
		{
			i = ii;
			GeomArrays[gi].Add(pProg->GetCompGeomArrays().at(gi)->GetAt(i));
			i++;
			if (pGeom->IsCycle())
			{
				for (; i < ProgGeom.GetSize() && ProgGeom.GetAt(i)->IsCycle(); ++i)
				{
					GeomArrays[gi].Add(pProg->GetCompGeomArrays().at(gi)->GetAt(i));
				}
				GeomArrays[gi].Add(pProg->GetCompGeomArrays().at(gi)->GetAt(i));
				i++;
			}
		}
		++k;
		int GNumber = int(GeomArrays[0].GetSize());
		if (k <= EndHCadr && i < ProgGeom.GetSize())
		{
			// Add one extra geom cadr
			for (; i < ProgGeom.GetSize(); i++)
			{
				NCadrGeom* pGeom;
				for (int gi = 0; gi < GeomArrays.GetSize(); ++gi)
				{
					pGeom = pProg->GetCompGeomArrays().at(gi)->GetAt(i);
					GeomArrays[gi].Add(pGeom);
				}
				if (!pGeom->IsUndef())
					break;
				if (!pGeom->IsCycle())// Cycle attrib
					k++;
			}
		}
		// 


		CStocksCollection& Stocks = GetStocksCollection();

		Matrs.resize(GeomArrays.GetSize());
		MCSMatrs.resize(GeomArrays.GetSize());
		RotMatrs.resize(GeomArrays.GetSize());
		for (auto gi = GeomArrays.GetSize() - 1; gi >= 0; --gi)
		{
			NCadrGeomArray& GeomArr = GeomArrays[gi];

			// find first representative cadr
			int FirstRepInd = 0;
			for (; FirstRepInd < GeomArr.GetSize(); ++FirstRepInd)
				if (!GeomArr[FirstRepInd]->IsUndef())
					break;
			if (FirstRepInd < GeomArr.GetSize())// FirstRepInd found
			{
				int CadrMatrNum = GeomArr[FirstRepInd]->GetMatrNum();
				SMachState MachState(BPoint(0., 0., 0., 1.), BSpacePos::GetABC(CadrMatrNum), &pChannel->GetUnitCn()->GetFixedDef());
				int StockPositon = GeomArr[FirstRepInd]->GetStockPos();
				BMatr Stock2ToolMatr =
					pChannel->GetMachCopy().GetStock2ToolMatrWRec(MachState, StockPositon, TurretNum, AttachNum);
				RotMatrs[gi] = Stock2ToolMatr;
				BMatr StockMoveMatrInv;
				if (pProg->HaveAddCoord())
				{
					SetMT2CurPosAddCoord(*GeomArr[FirstRepInd]);
					StockMoveMatrInv = pChannel->GetMachCopy().GetStockMatr(StockPositon, true)
						* pChannel->GetMachCopy().GetStockMovedMatr(StockPositon, true).invr();
				}
				BMatr CadrRotMatr = BSpacePos::GetRotMatr(CadrMatrNum);
				NMachUnitPair MUPair(pChannel->GetMachCopy(), *pChannel->GetUnitCn());
				BMatr MCS = MUPair.GetMach().GetMCS(GeomArr[FirstRepInd], pFRTool, MUPair.GetUnit().GetFixedDef());
				//pChannel->GetProg2BSP().SetMatr(CadrRotMatr * StockMoveMatrInv * Stock2ToolMatr.invr());// TO CHECK
				// Matr совмещает ось инструмента с осью Z
				Matrs[gi] = CadrRotMatr * StockMoveMatrInv * Stock2ToolMatr.invr();
				MCSMatrs[gi] = MCS;

			}
			else
			{
				Matrs[gi].SetE();
				MCSMatrs[gi].SetE();
				RotMatrs[gi].SetE();
			}
		}
//		RangePos.LastStartCadrNum = 0;
		iRet = DynProcFragmentColl(GeomArrays, GNumber, Stocks);
		if (iRet < 0)
		{
			iRet = -iRet - 1;
			NeedBreak = true;
			pChannel->ResetNeedProbe();
			GetDoc()->InterruptAnimation();
		}
		if (GetDoc()->IsInterrupted())
		{
			break;
		}
	}
	if (StockState == AT_MILL)
		pChannel->GetProg2BSP().StopCutting1DirThreads();

	pChannel->GetProg2BSP().PostCollisions(pMainWnd);

	if (GetDoc()->IsInterrupted())
	{
		cadrID InterruptCadrID = RangePos.FindCurHCadr(iRet);
		intPair InterruptDiap(InterruptCadrID.Cadr, InterruptCadrID.Cadr);
		return NeedBreak ? InterruptDiap.invert() : InterruptDiap;
	}
	return intPair(EndHCadr, EndHCadr);
}

int FRange::DynProcFragment(const CArray<NCadrGeomArray>& Geoms, int GNumber, class CStocksCollection& Stocks)
{
	switch (AnimMode)
	{
	case NGlobalState::Animation:
		return AnimateFragment(Geoms, GNumber, Stocks);
	case NGlobalState::Result:
		return ReRenderFragment(Geoms, GNumber, Stocks);
	case NGlobalState::Anim_no:
	default:
		return 0;
	}
}

FRange::intPair FRange::AnimateRange(CWnd* pMainWnd)
{
	NCollManagerOrd& CollM = GetDoc()->GetCollManager();
	if (pFRTool == NULL && AnimMode == NGlobalState::Result && !CollM.IsForeType())
		return intPair(EndHCadr, EndHCadr);
	// Make new tool if needed
	// pWorkTool is temporary. It has all margins and will be deleted at the end of this method
	const auto pOrigTool = pFRTool;
	if (pFRTool != nullptr)// pFRTool may be nullptr if Animate. Can't make reaction here
	{
		pWorkTool = pFRTool->CreateTool4Cutting();
		if (pWorkTool == nullptr)
			pWorkTool = pFRTool;
	}

	intPair RetDiap(0, 0);

	if (pProg->GetGeomArraysCount() > 1)
	{
		RetDiap = AnimateRangeMultiStocks(pMainWnd);
	}
	else
	{// ordinary processing (one stock)
		if (StockState == AT_MILL)
			pChannel->GetProg2BSP().StartCutting1DirThreads();
		CArray<NCadrGeomArray> GeomArrays;
		GeomArrays.SetSize(1);
		int TurretNum = 0, AttachNum = 0;
		if (pFRTool)
		{
			TurretNum = pFRTool->Turret;
			AttachNum = pFRTool->Attach;
		}
		const BGeomArray& ProgGeom = pProg->GetCompGeomArray(0);
		// Skip cadrs before StartHCadr
		int i = 0;
		int k = 0;
		for (; k < StartHCadr && i < ProgGeom.GetSize(); i++)
		{
			// k - index of current step excluding steps in the cycles
			// i - index of current step
			NCadrGeom* pGeom = ProgGeom.GetAt(i);
			if (!pGeom->IsCycle())// Cycle attrib
				k++;
		}
		// Get cadrs from StartHCadr to EndHCadr

		RangePos.kStart = 0;
		RangePos.iStart = 0;
		int Max4ReRendering = 640;
		auto StartTime = timeGetTime();
		bool TimeCheck = false;
		bool NeedBreak = false;
		RangePos.StepParamPerCent = 0.;
		int iRet = 0;
		while (k < EndHCadr && i < ProgGeom.GetSize())
		{// Divide ProgGeom to fragments by ABC changing points, 5x cadrs and time limit (Max4ReRendering)
			RangePos.kStart = k;// Start history position of new GeomArr
			RangePos.iStart = i;// Start ProgGeom position for new GeomArr
			//kStart, iStart are used in FindCurHCadr
			if (AnimMode == NGlobalState::Result && TimeCheck)
			{
				TimeCheck = false;
				auto CurTime = timeGetTime();
				if (CurTime - StartTime < 250)
					Max4ReRendering *= 2;
				else if (CurTime - StartTime > 600)
				{
					Max4ReRendering /= 2;
					++Max4ReRendering; // to prevent zero value
				}
				StartTime = CurTime;
			}
			NCadrGeomArray& GeomArr = GeomArrays[0];
			GeomArr.RemoveAll();
			NCadrGeom* pGeom = ProgGeom.GetAt(i++);
			GeomArr.Add(pGeom);
			if (pGeom->IsCycle())
			{
				for (; i < ProgGeom.GetSize() && ProgGeom.GetAt(i)->IsCycle(); ++i)
				{
					pGeom = ProgGeom.GetAt(i);
					GeomArr.Add(pGeom);
				}
				pGeom = ProgGeom.GetAt(i++);
				GeomArr.Add(pGeom);
			}
			++k;
			for (; k < EndHCadr && i < ProgGeom.GetSize(); ++i)
			{
				if (!pGeom->IsCycle() && AnimMode == NGlobalState::Result && GeomArr.GetSize() > Max4ReRendering)
				{
					TimeCheck = false;
					break;
				}
				int PrevMatrNum = pGeom->GetMatrNum();
				bool PrevIs5x = pGeom->IsFull5x();
				bool PrevIsAddCoord = (pGeom->type.type == addcoord);
				pGeom = ProgGeom.GetAt(i);
				if (PrevIs5x != pGeom->IsFull5x())
					break;
				if (pGeom->GetMatrNum() != PrevMatrNum)
					break;

				if (pGeom->type.type == addcoord || PrevIsAddCoord)
					break;

				GeomArr.Add(pGeom);
				if (!pGeom->IsCycle())// Cycle attrib
					k++;
			}
			int GNumber = int(GeomArr.GetSize());
			if (k <= EndHCadr && i < ProgGeom.GetSize())
			{
				// Add one extra geom cadr
				for (; i < ProgGeom.GetSize(); i++)
				{
					pGeom = ProgGeom.GetAt(i);
					GeomArr.Add(pGeom);
					if (!pGeom->IsUndef())
						break;
					if (!pGeom->IsCycle())// Cycle attrib
						k++;
				}
			}
			// 


			CStocksCollection& Stocks = GetStocksCollection();

			// find first representative cadr
			int FirstRepInd = 0;
			for (; FirstRepInd < GeomArr.GetSize(); ++FirstRepInd)
				if (!GeomArr[FirstRepInd]->IsUndef())
					break;
			if (FirstRepInd < GeomArr.GetSize())// FirstRepInd found
			{
				Matrs.resize(1);
				MCSMatrs.resize(1);
				RotMatrs.resize(1);
				int CadrMatrNum = GeomArr[FirstRepInd]->GetMatrNum();
				SMachState MachState(BPoint(0., 0., 0., 1.), BSpacePos::GetABC(CadrMatrNum), &pChannel->GetUnitCn()->GetFixedDef());
				int StockPositon = GeomArr[FirstRepInd]->GetStockPos();

				BMatr StockMoveMatrInv;
				BMatr Stock2ToolMatr;
				if (pChannel->GetMachCopy().HaveStockMoved())
				{
					SetMT2CurPosAddCoord(*GeomArr[FirstRepInd]);
					StockMoveMatrInv = pChannel->GetMachCopy().GetStockMatr(StockPositon, true)
						* pChannel->GetMachCopy().GetStockMovedMatr(StockPositon, true).invr();
					Stock2ToolMatr = pChannel->GetMachCopy().GetStockMoved2ToolMatrWRec(MachState, StockPositon, TurretNum, AttachNum);
				}
				else
				{
					Stock2ToolMatr =
						pChannel->GetMachCopy().GetStock2ToolMatrWRec(MachState, StockPositon, TurretNum, AttachNum);
				}
				RotMatrs[0] = Stock2ToolMatr;
				BMatr CadrRotMatr = BSpacePos::GetRotMatr(CadrMatrNum);
				NMachUnitPair MUPair(pChannel->GetMachCopy(), *pChannel->GetUnitCn());
				BMatr MCS = MUPair.GetMach().GetMCS(GeomArr[FirstRepInd], pFRTool, MUPair.GetUnit().GetFixedDef());
				//pChannel->GetProg2BSP().SetMatr(CadrRotMatr * StockMoveMatrInv * Stock2ToolMatr.invr());// TO CHECK
				// Matr совмещает ось инструмента с осью Z
				Matrs[0] = CadrRotMatr * StockMoveMatrInv * Stock2ToolMatr.invr();
				MCSMatrs[0] = MCS;
				iRet = 0;// A number of processed GeomArr elements
		//		RangePos.LastStartCadrNum = 0;
				iRet = DynProcFragmentColl(GeomArrays, GNumber, Stocks);
				if (iRet < 0)
				{
					iRet = -iRet - 1;
					NeedBreak = true;
					pChannel->ResetNeedProbe();
					GetDoc()->InterruptAnimation();
				}
				//FRAGMENT CHANGE		GeomArr.RemoveAll();
				if (GetDoc()->IsInterrupted())
				{
					break;
				}
			}
		}
		if (StockState == AT_MILL)
			pChannel->GetProg2BSP().StopCutting1DirThreads();

		if (GetDoc()->IsInterrupted())
		{
			auto& LProg2BSP = pChannel->GetProg2BSP();
			if (!LProg2BSP.CollPositions.empty())
			{
				const cadrID InterruptCadrIDStart = LProg2BSP.CollPositions.back().FindCurHCadr(LProg2BSP.CollPositions.back().GetLastStartCadrNum());
				const cadrID InterruptCadrID = LProg2BSP.CollPositions.back().FindCurHCadr(LProg2BSP.CollPositions.back().CadrNum);
				RetDiap = intPair(InterruptCadrIDStart.Cadr, InterruptCadrID.Cadr);
			}
			else
			{
				const cadrID InterruptCadrID = RangePos.FindCurHCadr(iRet);
				RetDiap = intPair(InterruptCadrID.Cadr, InterruptCadrID.Cadr);
			}
			if (NeedBreak)
				RetDiap.invert();
		}
		else
			RetDiap = intPair(EndHCadr, EndHCadr);

		pChannel->GetProg2BSP().PostCollisions(pMainWnd);
	}
	if (pWorkTool != pOrigTool)
	{
		delete pWorkTool;
	}
	pWorkTool = nullptr;
	return RetDiap;
}

int FRange::GetProgNum(void) const
{
	return RangePos.ProgNum;
}

int FRange::ReRenderFragment(const CArray<NCadrGeomArray>& Geoms, int GNumber, CStocksCollection& Stocks)
{
	if (pFRTool == NULL)
		return GNumber - 1;
	const NCadrGeomArray& Geom = Geoms.GetAt(0);
	if (Geom.IsEmpty())
		return 0;
	GetDoc()->Wait4Permission(*pChannel, Geom.GetAt(0)->GetStartTime());
	// This fragment needed for AT_MILL_TURN only!
	if (CuttingTypeChanged)
	{
		CuttingTypeChanged = false;

		Stocks.ApplyStocksState(StockState, GetStockGr4Turn());
	}
	// END:This fragment needed for AT_MILL_TURN only
	int ret = 0;
	MBody* pBody = Stocks.GetVisibleBodiesList(StockState);
	if (pBody != nullptr)
	{
		bool DifferentBodyRot = false;
		for (MBody* pCurBody = pBody->GetNextMBody(); pCurBody != nullptr; pCurBody = pCurBody->GetNextMBody())
			if (pCurBody->GetBodyRot().Or() != pBody->GetBodyRot().Or())
			{
				DifferentBodyRot = true;
				break;
			}
		pChannel->GetProg2BSP().SetBodyRotMatr(pBody->GetBodyRot().Inv());
		pChannel->GetProg2BSP().SetBodyRotStart(pBody);
		if (DifferentBodyRot)
		{
			MBody* pPrevBody = pBody;
			for (MBody* pCurBody = pBody->GetNextMBody(); pPrevBody != nullptr; pCurBody = ((pCurBody == nullptr) ? nullptr : pCurBody->GetNextMBody()))
			{
				bool NeedCut = pCurBody == nullptr;
				if (!NeedCut)
					NeedCut = pCurBody->GetBodyRot().Or() != pPrevBody->GetBodyRot().Or();
				if (NeedCut)
				{
					if (pPrevBody->IsBodyRotActive())
						pChannel->GetProg2BSP().SetBodyRotMatr(pPrevBody->GetBodyRot().Inv());
					else
						pChannel->GetProg2BSP().SetBodyRotMatr(BMatr());
					pPrevBody->ResetNextBody();
					for (auto gi = Geoms.GetSize() - 1; gi >= 0; --gi)
					{
						pChannel->GetProg2BSP().SetRotMatr(RotMatrs[gi]);
						pChannel->GetProg2BSP().SetMatr(Matrs[gi]);
						pChannel->GetProg2BSP().SetMCS(MCSMatrs[gi]);
						int RetLoc = ReRenderFragmentInt(Geoms[gi], GNumber, Stocks);
						if (RetLoc < 0 || ret >= 0)
							ret = RetLoc;
					}
					pPrevBody->SetNextMBody(pCurBody);
					pChannel->GetProg2BSP().SetBodyRotStart(pCurBody);
				}
				pPrevBody = pCurBody;
			}
		}
		else
		{
			for (auto gi = Geoms.GetSize() - 1; gi >= 0; --gi)
			{
				pChannel->GetProg2BSP().SetRotMatr(RotMatrs[gi]);
				pChannel->GetProg2BSP().SetMatr(Matrs[gi]);
				pChannel->GetProg2BSP().SetMCS(MCSMatrs[gi]);
				int RetLoc = ReRenderFragmentInt(Geoms[gi], GNumber, Stocks);
				if (RetLoc < 0 || ret >= 0)
					ret = RetLoc;
			}
		}
		pChannel->GetProg2BSP().SetBodyRotMatr(BMatr());
		pChannel->GetProg2BSP().SetBodyRotStart(nullptr);
	}
	GetDoc()->EnableNextChannel(*pChannel);
	return ret;
}

int FRange::DynProcFragmentColl(const CArray<NCadrGeomArray>& Geoms, int GNumber, CStocksCollection& Stocks)
{
	NCollManagerOrd& CollM = GetDoc()->GetCollManager();
	if (!CollM.IsForeType())
	{
		auto Res = DynProcFragment(Geoms, GNumber, Stocks);
		return Res;
	}

	// Version for collision
	assert(AnimMode == NGlobalState::Result);
	MTMachineTool AuxMach;
	const auto pMU = GetDoc()->GetpMUPair();
	AuxMach.FullCopyWOVis(pMU->GetMach());
	NMachUnitPair Pair(AuxMach, pMU->GetUnit());
	const auto& LProg2BSP = pChannel->GetProg2BSP();

	auto OldSize = LProg2BSP.CollPositions.size();

	if (pFRTool->GetTestTool() != nullptr)
	{
		DynProcFragmentColl1Tool(pFRTool->GetTestTool(), Geoms, GNumber, Stocks, Pair);
		for (auto ci = OldSize; ci < LProg2BSP.CollPositions.size(); ++ci)
			CollM.AddCutCollision(LProg2BSP.CollPositions.at(ci));
		OldSize = LProg2BSP.CollPositions.size();
	}
	auto Res = DynProcFragmentColl1Tool(pFRTool, Geoms, GNumber, Stocks, Pair);

	for (auto ci = OldSize; ci < LProg2BSP.CollPositions.size(); ++ci)
		CollM.AddCutCollision(LProg2BSP.CollPositions.at(ci));
	CollM.PrepTimeLine(Geoms.GetAt(0).GetAt(GNumber - 1)->GetEndTime());

	AuxMach.ClearFullCopyWOVis();
	return Res;
}

int FRange::DynProcFragmentColl1Tool(const NToolCombined* pTool, const CArray<NCadrGeomArray>& Geoms, int GNumber, CStocksCollection& Stocks, NMachUnitPair& Pair)
{
	NCollManagerOrd& CollM = GetDoc()->GetCollManager();
	NCollFragment CollFrag(CollM, GetDoc()->GetModifStocksGabar());
	CollFrag.Set(pTool, pProg, RangePos.ProgNum, &Pair);

	int LocGNumber = GNumber;
	CollFrag.Reset(Geoms, GNumber, RangePos.iStart);
	const int LastCadr = CollFrag.CheckingProc(true, false);
	bool StockCollFound = CollFrag.GetStockCollFound();
	if (StockCollFound)
		LocGNumber = LastCadr + 1;
	int Res = GetDoc()->IsCollWOStock() ? RangePos.CadrNum + LocGNumber - 1 : DynProcFragment(Geoms, LocGNumber, Stocks);
	if (GetDoc()->IsCollWOStock())
		pChannel->GetProg2BSP().PrepNewPosition(RangePos, 0);
	if (Res < 0 || GetDoc()->IsInterrupted())
		StockCollFound = false;// to go to ClearFullCopyWOVis
	if (StockCollFound)
	{
		const CArray<NCadrGeomArray>* pOldGeoms = &Geoms;
		CArray<NCadrGeomArray> NewGeoms[2];
		NewGeoms[0].SetSize(Geoms.GetSize());
		NewGeoms[1].SetSize(Geoms.GetSize());

		int BigGNumber = GNumber;
		for (int PingPong = 0; StockCollFound; PingPong = (++PingPong) % 2)
		{
			pChannel->GetProg2BSP().Wait4Empty1DirThreads();
			GetDoc()->CreateModifStocksRender();
			GetDoc()->GetCollManager().ReplaceStocks(Stocks.GetVisibleRendersList(), nullptr);

			CollFrag.Reset(*pOldGeoms, LocGNumber, RangePos.iStart);
			CollFrag.CheckingProc(false, true); // check last cadr to find real collision with the stock

			RangePos.Incrik(LocGNumber);
			CArray<NCadrGeomArray>* pNewGeoms = &NewGeoms[PingPong];
			const int RealShift = CollFrag.MoveGeoms(LocGNumber, *pNewGeoms, *pOldGeoms);// pNewGeoms is cleared inside
			BigGNumber -= RealShift;
			LocGNumber = BigGNumber;
			CollFrag.Reset(*pNewGeoms, BigGNumber, RangePos.iStart);
			const int LastCadrl = CollFrag.CheckingProc(true, false);
			StockCollFound = CollFrag.GetStockCollFound();
			if (StockCollFound)
				LocGNumber = LastCadrl + 1;
			Res = DynProcFragment(*pNewGeoms, LocGNumber, Stocks);
			if (Res < 0 || GetDoc()->IsInterrupted())
				break;
			pOldGeoms = pNewGeoms;
		}
	}
	return Res;
}

int FRange::ReRenderFragmentInt(const NCadrGeomArray& Geom, int GNumber, CStocksCollection& Stocks)
{
	// Skip non geom cadrs
	for (RangePos.CadrNum = 0; RangePos.CadrNum < GNumber; ++RangePos.CadrNum)
		if (Geom[RangePos.CadrNum]->HaveGeom5())
			break;
	if (RangePos.CadrNum == GNumber)
		return RangePos.CadrNum;

	bool EmptyTool = pFRTool == nullptr;
	if (!EmptyTool)
		EmptyTool = pFRTool->num_tools <= 0;
	if (EmptyTool)
	{
		pChannel->GetProg2BSP().CollPositions.emplace_back(RangePos, LE_EMPTYTOOL);
		return GNumber - 1;
	}

	DX_TOOLTYPE DXType = CanDXBeUsed(Stocks, Geom, GNumber);
	if (DXType != TT_NO)
		return ReRenderFragmentDX(Geom, GNumber, Stocks, pWorkTool, DXType);

	NCadrGeom* CadrsGeom = MakeRightOrd(Geom, RangePos.CadrNum, GNumber);
	// Cycle cadrs are in the right sequence now (in CadrsGeom)
	auto& LProg2BSP = pChannel->GetProg2BSP();
	const auto OldSize = LProg2BSP.CollPositions.size();
	int ret = 0;
	if (NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::DX)
	{
		ret = LProg2BSP.ExecGeomArrayDXC(CadrsGeom, int(Geom.GetSize() - RangePos.CadrNum)
			, GNumber - RangePos.CadrNum, pWorkTool, Stocks, RangePos);
	}
	else
	{
		ret = LProg2BSP.ExecGeomArray(CadrsGeom, int(Geom.GetSize() - RangePos.CadrNum)
			, GNumber - RangePos.CadrNum, pWorkTool, Stocks, RangePos);
	}
	// recalc new collisions if any (Result mode. RangePos.CadrNum points to start.
	for (auto k = OldSize; k < LProg2BSP.CollPositions.size(); ++k)
		LProg2BSP.CollPositions.at(k) += RangePos.CadrNum;

	delete[] CadrsGeom;

	return ret < 0 ? -RangePos.CadrNum - ret - 1 : RangePos.CadrNum + ret;
}

NCadrGeom* FRange::MakeRightOrd(const NCadrGeomArray& Geom, int IndShift, int& GNumber)
{
	bool IncrGNumber = false;
	// GNumber should be increased by 1 if it cuts cycle body
	NCadrGeom* CadrsGeom = new NCadrGeom[Geom.GetSize() - IndShift];
	for (int i = IndShift; i < Geom.GetSize(); ++i)
	{
		if (Geom[i]->IsCycle())// Cycle attrib
		{// Process cycle
		 //Find first non cycle cadr
			int k = 1;
			bool Found = false;
			for (; i + k < Geom.GetSize(); ++k)
			{
				if (!Geom[i + k]->IsCycle())//Non Cycle attrib
				{
					if (Geom[i + k]->HaveGeom5())
					{
						CadrsGeom[i - IndShift].FullCopy(*Geom[i + k]);
					}
					if (i + k >= GNumber && !IncrGNumber) // GNumber cuts cycle body (do it only once)
					{
						IncrGNumber = true;
						++GNumber;
					}
					++k;
					Found = true;
					break;
				}
			}
			--k;
			int last = i + k;
			// Process cycle body
			for (; i < last; ++i)
			{
				CadrsGeom[i + (Found ? 1 : 0) - IndShift].FullCopy(*Geom[i]);
			}
		}
		else
			CadrsGeom[i - IndShift].FullCopy(*Geom[i]);
	}// Cycle cadrs are in the right sequence now (in CadrsGeom)
	return CadrsGeom;
}

int FRange::ReRenderFragmentDX(const NCadrGeomArray& Geom, int GNumber, class CStocksCollection& Stocks, const NToolCombined* pTool, DX_TOOLTYPE DXType)
{
	GOneStepGeom OneStepGeom(Geom, false, DXType);
	return pChannel->GetProg2BSP().EGAMillDX(OneStepGeom, RangePos.CadrNum, GNumber, pTool, Stocks, RangePos, DXType);
}

int FRange::AnimateFragmentMill(const CArray<NCadrGeomArray>& Geoms, int GNumber, CStocksCollection& Stocks)
{
	vector<double> StartT(Geoms.GetSize());
	vector<INT_PTR> ActiveCadrNum(Geoms.GetSize());
	vector <INT_PTR> i(Geoms.GetSize());
	std::vector<std::unique_ptr<GOneStepGeom>> OneStepGeoms(Geoms.GetSize());

	for (int gi = 0; gi < Geoms.GetSize(); ++gi)
	{
		StartT[gi] = 0.;
		ActiveCadrNum[gi] = 0;
		i[gi] = 0;
		const NCadrGeomArray& Geom = Geoms.GetAt(gi);
		DX_TOOLTYPE DXType = CanDXBeUsed(Stocks, Geom, GNumber);
		OneStepGeoms[gi] = std::make_unique<GOneStepGeom>(Geom, true, DXType);
		OneStepGeoms[gi]->SetTrackFeed(pSpeedPar->IsTrackFeed());
	}
	auto& LProg2BSP = pChannel->GetProg2BSP();
	for (bool Cont = true; Cont;)
	{
		double StepParamPerCent = 0.;
		bool ResetCuttingType = CuttingTypeChanged;
		for (auto gi = Geoms.GetSize() - 1; gi >= 0; --gi)
		{
			StepParamPerCent = RangePos.StepParamPerCent;
			OneStepGeoms[gi]->SetMaxStepLength(pSpeedPar->GetLengthStep());
			OneStepGeoms[gi]->SetMaxStepTime(1.15 * pSpeedPar->GetTimeStep() * pSpeedPar->GetTrackFeedScale() / 1000.);// 1.15 is a magic number to compensate timer inaccuracy

			bool IsInterrupted = GetDoc()->IsInterrupted();
			Cont = OneStepGeoms[gi]->Fill(GNumber, i[gi], StartT[gi], !IsInterrupted && (NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::DX), ActiveCadrNum[gi]
				, StepParamPerCent, pSpeedPar->ParMill, &DivideF, this, &GetNextBreakTime);// i now points to the first cadr to be processed

			if (IsInterrupted && (NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::DX))
				if(OneStepGeoms[gi]->GetSize() > 0)
					OneStepGeoms[gi]->RemoveAt(OneStepGeoms[gi]->GetSize() - 1, 1); // Extra cadr added in fill i.e. IsInterrupted is true
			if (!Cont)
				break;
			RangePos.CadrNum = int(ActiveCadrNum[gi]);
			LProg2BSP.SetRotMatr(RotMatrs[gi]);
			LProg2BSP.SetMatr(Matrs[gi]);
			LProg2BSP.SetMCS(MCSMatrs[gi]);

			const auto OldSize = LProg2BSP.CollPositions.size();

			int Res = AnimateOneStep(gi == 0 ? ResetCuttingType : false, *OneStepGeoms[gi], Stocks, StartT[gi], StepParamPerCent == 0.);// CuttingTypeChanged changes inside

			// recalc new collisions if any. Animate mode.
			for (auto k = OldSize; k < LProg2BSP.CollPositions.size(); ++k)
				OneStepGeoms.at(gi)->RecalcPosition(LProg2BSP.CollPositions.at(k));

			if (Res < 0)
			{
				RangePos.StepParamPerCent = StepParamPerCent;
				return -RangePos.CadrNum - 1;
			}
			if (IsInterrupted)
			{
				RangePos.StepParamPerCent = StepParamPerCent;
				return RangePos.CadrNum;
			}
		}
		RangePos.StepParamPerCent = StepParamPerCent;
	}
	LProg2BSP.Wait4Empty1DirThreads();
	RangePos.CadrNum = int(ActiveCadrNum[0]);
	return RangePos.CadrNum;
}

void FRange::DivideF(bool TrackFeed, NCadrGeom& Source, double TValue, NCadrGeom& Dest, FRange* pR)
{
	BPoint XYZ0, XYZ1, ABC0, ABC1;
	if (Source.IsRCP())
	{
		BMatr MCS;
		Source.CalcCurCoordMCS(0., MCS, XYZ0, ABC0);
		Source.CalcCurCoordMCS(1., MCS, XYZ1, ABC1);
	}
	// ordinary processing
	double t = TValue;
	if (TrackFeed)
		t = NCadrGeom::DivideTByTime(Source, TValue, Dest);
	else
		NCadrGeom::DivideT(Source, TValue, Dest);

	if (Source.IsRCP())
	{// recalc source and dest
		if (pR == nullptr)
			return;
		SMachState P(&pR->pChannel->GetUnitCn()->GetFixedDef());
		int Tur = 0;
		int Att = 0;
		if (pR->pFRTool)
		{
			Tur = pR->pFRTool->Turret;
			Att = pR->pFRTool->Attach;
		}
		pR->pChannel->GetMachCopy().CalcIntState(Source.GetRCPMask(), t, SMachState(XYZ0, ABC0, P.GetDef())
			, SMachState(XYZ1, ABC1, P.GetDef()), P, Source.GetStockPos(), Tur, Att);
		Source.SetN(P.GetCoord(3), P.GetCoord(4), P.GetCoord(5));
		Source.SetE(P.GetCoord(0), P.GetCoord(1), P.GetCoord(2));
		Dest.SetI(Source.GetN());
		Dest.SetB(Source.GetE());
	}
}

MainTime FRange::GetNextBreakTime(MainTime StartTime, FRange* pR)
{
	auto& CollM = pR->GetDoc()->GetCollManager();
	const auto Res = CollM.GetCollID(StartTime);
	if (Res == nullptr)
		return MainT_UNDEF;
	return Res->PTPoint.GetTime();
}

int FRange::AnimateFragmentTurn(const CArray<NCadrGeomArray>& Geoms, int GNumber, CStocksCollection& Stocks)
{
	vector<double> StartT(Geoms.GetSize());
	vector<INT_PTR> ActiveCadrNum(Geoms.GetSize());
	vector <INT_PTR> i(Geoms.GetSize());
	std::vector<GOneStepGeom*> OneStepGeoms(Geoms.GetSize());

	for (int gi = 0; gi < Geoms.GetSize(); ++gi)
	{
		StartT[gi] = 0.;
		ActiveCadrNum[gi] = 0;
		i[gi] = 0;
		const NCadrGeomArray& Geom = Geoms.GetAt(gi);
		OneStepGeoms[gi] = new GOneStepGeom(Geom, true, TT_NO);
		OneStepGeoms[gi]->SetTrackFeed(pSpeedPar->IsTrackFeed());
	}
	for (bool Cont = true; Cont;)
	{
		double StepParamPerCent = 0.;
		bool ResetCuttingType = CuttingTypeChanged;
		for (auto gi = Geoms.GetSize() - 1; gi >= 0; --gi)
		{
			StepParamPerCent = RangePos.StepParamPerCent;
			OneStepGeoms[gi]->SetMaxStepLength(pSpeedPar->GetLengthStep());
			OneStepGeoms[gi]->SetMaxStepTime(1.15 * pSpeedPar->GetTimeStep() * pSpeedPar->GetTrackFeedScale() / 1000.);// 1.15 is a magic number to compensate timer inaccuracy

			bool IsInterrupted = GetDoc()->IsInterrupted();
			Cont = OneStepGeoms[gi]->Fill(GNumber, i[gi], StartT[gi], !IsInterrupted, ActiveCadrNum[gi]
				, StepParamPerCent, pSpeedPar->ParRot, &DivideF, this, &GetNextBreakTime);// i now points to the first cadr to be processed

			if (IsInterrupted)
				if (!OneStepGeoms[gi]->IsEmpty())
					OneStepGeoms[gi]->RemoveAt(OneStepGeoms[gi]->GetSize() - 1, 1); // Extra cadr added in fill i.e. IsInterrupted is true
			RangePos.CadrNum = int(ActiveCadrNum[gi]);
			auto& LProg2BSP = pChannel->GetProg2BSP();
			LProg2BSP.SetRotMatr(RotMatrs[gi]);
			LProg2BSP.SetMatr(Matrs[gi]);
			LProg2BSP.SetMCS(MCSMatrs[gi]);

			const auto OldSize = LProg2BSP.CollPositions.size();

			int Res = AnimateOneStep(gi == 0 ? ResetCuttingType : false, *OneStepGeoms[gi], Stocks, StartT[gi]);// CuttingTypeChanged changes inside

			// recalc new collisions if any. Animate mode.
			for (auto k = OldSize; k < LProg2BSP.CollPositions.size(); ++k)
				OneStepGeoms.at(gi)->RecalcPosition(LProg2BSP.CollPositions.at(k));

			if (Res < 0)
			{
				RangePos.StepParamPerCent = StepParamPerCent;
				return -RangePos.CadrNum - 1;
			}
			if (IsInterrupted)
			{
				RangePos.StepParamPerCent = StepParamPerCent;
				return RangePos.CadrNum;
			}
		}
		RangePos.StepParamPerCent = StepParamPerCent;
	}
	for (int gi = 0; gi < Geoms.GetSize(); ++gi)
	{
		delete OneStepGeoms[gi];
	}
	RangePos.CadrNum = int(ActiveCadrNum[0]);
	return RangePos.CadrNum;
}

int FRange::AnimateFragment(const CArray<NCadrGeomArray>& Geoms, int GNumber, CStocksCollection& Stocks)
{
	if (Stocks.empty())
		return -1;
	if (StockState == AT_MILL)
		return AnimateFragmentMill(Geoms, GNumber, Stocks);
	else
		return AnimateFragmentTurn(Geoms, GNumber, Stocks);

	return RangePos.CadrNum;
}

int FRange::AnimateOneStep(bool ResetCuttingType, GOneStepGeom& CadrsGeom, CStocksCollection& Stocks, double tPart, bool NeedReSetRender/* = true*/)
{
	auto& LProg2BSP = pChannel->GetProg2BSP();
	int ret = 0;
	if (!CadrsGeom.IsGeomEmpty())
	{
		MBody* pBody = nullptr;
		GetDoc()->Wait4Permission(*pChannel, CadrsGeom[0]->GetStartTime());
		GQuadRender* pNewStartRender = nullptr;
		pBody = Stocks.GetVisibleBodiesList(StockState);
		if (NeedReSetRender || CuttingTypeChanged)
		{
			// This fragment needed for AT_MILL_TURN only!
			if (CuttingTypeChanged)
			{
				CuttingTypeChanged = false;

				Stocks.ApplyStocksState(StockState, GetStockGr4Turn());

				if (!Stocks.GetVisibleRendersList()) //if(!pRender)
					Stocks.CreateStocksRender(); //CreateRenderMill();
				pBody = Stocks.GetVisibleBodiesList(StockState);
			}
			// END:This fragment needed for AT_MILL_TURN only
			if (pBody != nullptr)
			{
				if (StockState == AT_MILL)
				{
					if (NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::BSP)
						LProg2BSP.SetRender(GQuadRenderMill::CreateGQuadRenderMillList(static_cast<MQuadBody&>(*pBody)));
					else
						LProg2BSP.SetRender(GQuadRenderDX5X::CreateGQuadRenderDX5XList(static_cast<DXSubModel5X&>(*pBody)));
				}
				else
					LProg2BSP.SetRender(GQuadRenderRot::CreateGQuadRenderRotList(static_cast<MQuadBody&>(*pBody)));
			}
		}
		if (pBody != nullptr)
		{
			bool DifferentBodyRot = false;
			for (MBody* pCurBody = pBody->GetNextMBody(); pCurBody != nullptr; pCurBody = pCurBody->GetNextMBody())
				if (pCurBody->GetBodyRot().Or() != pBody->GetBodyRot().Or())
				{
					DifferentBodyRot = true;
					break;
				}
			INT_PTR LastIndex = 0;
			LProg2BSP.SetBodyRotMatr(pBody->GetBodyRot().Inv());
			LProg2BSP.SetBodyRotStart(pBody);
			if (DifferentBodyRot)
			{
				GetDoc()->SetDifferentBodyRotFlag(false);
				// Disable multi thread DX
				bool BufMTDX = NCMProject::GetProject().GetGlobalState().GetMultiThreadDX();
				NCMProject::GetProject().GetGlobalState().SetMultiThreadDX(false);

				MBody* pPrevBody = pBody;
				for (MBody* pCurBody = pBody->GetNextMBody(); pPrevBody != nullptr; pCurBody = ((pCurBody == nullptr) ? nullptr : pCurBody->GetNextMBody()))
				{
					bool NeedCut = pCurBody == nullptr;
					if (!NeedCut)
						NeedCut = pCurBody->GetBodyRot().Or() != pPrevBody->GetBodyRot().Or();
					if (NeedCut)
					{
						if (pPrevBody->IsBodyRotActive())
							LProg2BSP.SetBodyRotMatr(pPrevBody->GetBodyRot().Inv());
						else
							LProg2BSP.SetBodyRotMatr(BMatr());
						pPrevBody->ResetNextBody();
						ret = CutOneStep(CadrsGeom, Stocks, CadrsGeom.GetDXType(), LastIndex);
						pPrevBody->SetNextMBody(pCurBody);
						LProg2BSP.SetBodyRotStart(pCurBody);
					}
					pPrevBody = pCurBody;
				}
				NCMProject::GetProject().GetGlobalState().SetMultiThreadDX(BufMTDX);
				GetDoc()->SetDifferentBodyRotFlag(true);
			}
			else
			{
				ret = CutOneStep(CadrsGeom, Stocks, CadrsGeom.GetDXType(), LastIndex);
			}
			LProg2BSP.SetBodyRotMatr(BMatr());
			LProg2BSP.SetBodyRotStart(nullptr);

			LastIndex = CadrsGeom.FindLastDefined(LastIndex);
			LastIndex = max(0, LastIndex);

			if (NeedReSetRender || ResetCuttingType)
			{
				if (NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::DX)
				{
					LProg2BSP.Wait4Empty1DirThreads();
				}
				std::set<int> StPosSet;
				pChannel->GetStPosSet(StPosSet);
				if (CadrsGeom[LastIndex]->GetStockPos() == *StPosSet.begin())
				{
					LProg2BSP.ReSetRender(Stocks, StockState);
					if (ret >= 0)
					{
						if (ResetCuttingType)
						{
							pNewStartRender = static_cast<GQuadRender*>(Stocks.GetVisibleRendersList());
							pChannel->GetDoc()->NGetStockList()->ChangeVisibleRendersToNULL(false);//pRenderRot = nullptr;
							NAnimState* pAnimState = new NAnimState(pNewStartRender, StockState);
							while (!pChannel->GetAnimateDisp().Place2RQueue(GetDoc()->IsInterrupted(), pAnimState))
							{
								GetDoc()->EnableNextChannel(*pChannel);
								GetDoc()->WaitCuttingEnabled(*pChannel);
							}
						}
						const cadrID CadrID = RangePos.FindCurCadr(RangePos.CadrNum);
						const cadrID CadrIDhist = RangePos.FindCurHCadr(RangePos.CadrNum);
						const NCadrGeom& Geom = *CadrsGeom[LastIndex];
						// tPart may be time parametr but should be geometric parameter always
						double tp = tPart == 0. ? 1. : tPart;
						if (pSpeedPar->IsTrackFeed() && tp != 1. && tp != -1.)
							tp = Geom.RecalcT2Geom(tp);
						NAnimState* pAnimState = new NAnimState(LProg2BSP.GetRender(), pFRTool, Geom, CadrID, CadrIDhist, tp);
						while (!pChannel->GetAnimateDisp().Place2RQueue(GetDoc()->IsInterrupted(), pAnimState))
						{
							GetDoc()->EnableNextChannel(*pChannel);
							GetDoc()->WaitCuttingEnabled(*pChannel);
						}
						LProg2BSP.SetRender(NULL);// Necessary because render is stored in NAnimState and will be deleted in it
					}
				}
			}
		}
	}
	GetDoc()->EnableNextChannel(*pChannel);
	return ret;
}

int FRange::CutOneStep(GOneStepGeom& CadrsGeom, CStocksCollection& Stocks, DX_TOOLTYPE DXType, INT_PTR& LastIndex)
{
	int ret = -5;
	if (StockState == AT_MILL)
	{
		if (NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::DX)
		{
			ret = (pWorkTool == nullptr) ? 0 : pChannel->GetProg2BSP().EGAMillDX(CadrsGeom, 0, int(CadrsGeom.GetSize()), pWorkTool, Stocks, RangePos, DXType);
			LastIndex = CadrsGeom.GetSize() - 1;
		}
		else
		{
			NCadrGeom* CadrsG = new NCadrGeom[CadrsGeom.GetSize()];
			for (INT_PTR i = 0; i < CadrsGeom.GetSize(); i = CadrsGeom.GetNextInd(i))
				CadrsG[i].FullCopy(*CadrsGeom[i]);
			ret = (pWorkTool == nullptr) ? 0 : pChannel->GetProg2BSP().ExecGeomArray(CadrsG, int(CadrsGeom.GetSize()), int(CadrsGeom.GetSize() - 1), pWorkTool, Stocks, RangePos);
			delete[] CadrsG;
			LastIndex = CadrsGeom.GetSize() - 2;
		}
	}
	else
	{
		NCadrGeom* CadrsG = new NCadrGeom[CadrsGeom.GetSize()];
		for (INT_PTR i = 0; i < CadrsGeom.GetSize(); i = CadrsGeom.GetNextInd(i))
			CadrsG[i].FullCopy(*CadrsGeom[i]);
		ret = (pWorkTool == nullptr) ? 0 : pChannel->GetProg2BSP().ExecGeomArray(CadrsG, int(CadrsGeom.GetSize()), int(CadrsGeom.GetSize()), pWorkTool, Stocks, RangePos);
		delete[] CadrsG;
		LastIndex = CadrsGeom.GetSize() - 1;
	}
	return ret;

}

void FRange::SetAnimMode(NGlobalState::AnimModes mode)
{
	AnimMode = mode;
}

NCMApplicationType FRange::GetStockState(void) const
{
	return StockState;
}

const BMTLinkage& FRange::GetMTLink() const
{
	return MTLink;
}

void FRange::DefineType(void)
{
	// Define pFRTool
	int b;
	NToolID	ToolID = pProg->GetToolByHist(StartHCadr, &b);
	NToolCombined* pToolCom = NULL;
	GetDoc()->NGetToolList()->Lookup(ToolID, pToolCom);
	pFRTool = pToolCom;
	// Define StockState
	bool OldFashion = false;
	if (OldFashion)
	{
		// Range may have different tools here but not different tool types
		StockState = AT_UNDEF;
		if (pToolCom)
			StockState = pToolCom->GetClass();
		if (StockState == AT_UNDEF)
			StockState = AT_MILL;
		if (StockState == AT_TURN)
		{
			int HistNum = pProg->GetHistByHCadr(StartHCadr);
			if (pProg->GetGeomArray().GetAt(HistNum)->IsGroove())
				StockState = AT_MILL;
		}
	}
}

const NCadrGeom& FRange::GetStartCadr()
{
	return *pProg->GetCompGeomArray().GetAt(pProg->GetHistByHCadr(StartHCadr));
}

const BProgram* FRange::GetProg(void) const
{
	return pProg;
}

FRange* FRange::ExtractPart(void)
{
	// Extract maximum part without tool changes and Groove/Turn changes and AddCoord changes and turn spindle state changes and machine linkage change
	// and set this to the rest part 
	// In other words divide range and return first part and modify this as a second part
	// Return NULL if whole range has single tool
	int LocEnd;
	NToolID ToolID = pProg->GetToolByHist(StartHCadr, &LocEnd);
	int LocStart = LocEnd;

	FRange* pNewRange = new FRange(*this);
	pNewRange->EndHCadr = min(LocStart, EndHCadr);
	pNewRange->Check4AddCoord();
	pNewRange->Check4TurnGroove();
	pNewRange->Check4LinkChange();
	pNewRange->Check4TurnSpindle();// defines type for the pNewRange
	if (EndHCadr == pNewRange->EndHCadr)
	{
		//		TransformRange = pNewRange->TransformRange;
		StockState = pNewRange->StockState;
		delete pNewRange;
		DefineType();
		return nullptr;//infinite looping protection
	}
	StartHCadr = pNewRange->EndHCadr;
	DefineType();
	return pNewRange;
}

bool FRange::HasCutting(void)
{
	const BGeomArray& ProgGeom = pProg->GetCompGeomArray();
	// Skip cadrs before StartHCadr
	NCadrGeom* pGeom;
	int i, k;
	for (i = 0, k = 0; k < StartHCadr && i < ProgGeom.GetSize(); i++)
	{
		// k - index of current step excluding steps in the cycles
		// i - index of current step
		pGeom = ProgGeom.GetAt(i);
		if (!pGeom->IsCycle())// Cycle attrib
			k++;
	}
	// Get cadrs from StartHCadr to EndHCadr

	while (k < EndHCadr && i < ProgGeom.GetSize())
	{
		pGeom = ProgGeom.GetAt(i++);
		if (pGeom->HaveGeom5() || pGeom->GetType().type == addcoord || pGeom->IsMachConfCh())
			return true;
		if (pGeom->IsCycle())
		{
			for (; i < ProgGeom.GetSize() && ProgGeom.GetAt(i)->IsCycle(); ++i)
			{
				pGeom = ProgGeom.GetAt(i);
				if (pGeom->HaveGeom5())
					return true;
			}
			pGeom = ProgGeom.GetAt(i++);
			if (pGeom->HaveGeom5())
				return true;
		}
		++k;
		for (; k < EndHCadr && i < ProgGeom.GetSize(); ++i)
		{
			pGeom = ProgGeom.GetAt(i);
			if (pGeom->HaveGeom5() || pGeom->GetType().type == addcoord || pGeom->IsMachConfCh())
				return true;
			if (!pGeom->IsCycle())// Cycle attrib
				k++;
		}
	}
	// 

	return false;
}

const NToolCombined* FRange::GetFRTool(void) const
{
	return pFRTool;
}

CStocksCollection& FRange::GetStocksCollection(void) const
{
	return *GetDoc()->NGetStockList();
}

DX_TOOLTYPE FRange::CanDXBeUsed(const CStocksCollection& StockBody, const NCadrGeomArray& Geom, int GNumber) const
{
	if (NCM_PROJECT.GetGlobalState().GetModelMode() != NGlobalState::DX)
		return TT_NO;
	// Special processing for a ball nose tool
	DX_TOOLTYPE CanBeUsed = TT_NO;
	if (pFRTool == nullptr)
		return CanBeUsed;
	const NTool* pWorkTool = pFRTool->GetLowermostTool();
	if (pWorkTool)
	{
		if (pWorkTool->IsBallNose())
		{
			if (pWorkTool->IsFullBall())
				CanBeUsed = TT_5X_FULL_BALL;
			else if (pWorkTool->IsCylWBallNose())
				CanBeUsed = TT_3X_CYL_BALL;
		}
		else if (pWorkTool->IsFlatNose())
			CanBeUsed = TT_3X_CYL;

	}
	if (CanBeUsed != TT_NO)
	{
		for (int i = 0; i < pFRTool->num_tools; ++i)
		{
			NTool* pTool = pFRTool->GetTool(i);
			if (pTool != pWorkTool)
			{
				if (pTool->IsCutting() || NTool::GetToolStockCollision() == CWS_ALL)
				{
					CanBeUsed = TT_NO;
					break;
				}
			}
		}
	}
	if (CanBeUsed == TT_NO)
		return TT_NO;

	if (CanBeUsed == TT_3X_CYL || CanBeUsed == TT_3X_CYL_BALL)
	{
		bool Has5x = false;
		for (int i = 0; i < GNumber && !Has5x; ++i)
			Has5x = Geom.GetAt(i)->IsFull5x();
		if (Has5x)
			CanBeUsed = (CanBeUsed == TT_3X_CYL_BALL) ? TT_5X_BALL : TT_NO;
	}

	return CanBeUsed;
}

void FRange::Check4TurnGroove()
{
	if (pFRTool)
		if (pFRTool->GetClass() != AT_TURN)
			return;

	const BGeomArray& ProgGeom = pProg->GetCompGeomArray();
	// k - index of current step excluding steps in the cycles
	// i - index of current step
	int k = StartHCadr;
	int i = pProg->GetHistByHCadr(k);
	// Get cadrs from StartHCadr to EndHCadr
	bool IsGroove = ProgGeom.GetAt(i)->IsGroove();
	for (; k < EndHCadr && i < ProgGeom.GetSize(); ++i)
	{
		NCadrGeom* pGeom = ProgGeom.GetAt(i);
		if (pGeom->IsGroove() != IsGroove)
			break;
		if (!pGeom->IsCycle())// Cycle attrib
			k++;
	}
	EndHCadr = k;
}

void FRange::Check4LinkChange()
{
	const BGeomArray& ProgGeom = pProg->GetCompGeomArray();
	// k - index of current step excluding steps in the cycles
	// i - index of current step
	int k = StartHCadr;
	int i = pProg->GetHistByHCadr(k);
	const BMTLinkage Link = ProgGeom.GetAt(i)->GetMTLink();
	// Get cadrs from StartHCadr to EndHCadr
	for (; k < EndHCadr && i < ProgGeom.GetSize(); ++i)
	{
		NCadrGeom* pGeom = ProgGeom.GetAt(i);
		if (pGeom->GetMTLink() != Link)
			break;
		if (!pGeom->IsCycle())// Cycle attrib
			k++;
	}
	EndHCadr = k;
	// Define Linkage
	MTLink = Link;
}

void FRange::Check4TurnSpindle()
{
	switch (NCMComData::GetType())
	{
	case AT_MILL_TURN:
	{
		const BGeomArray& ProgGeom = pProg->GetCompGeomArray();
		// k - index of current step excluding steps in the cycles
		// i - index of current step
		int k = StartHCadr;
		int i = pProg->GetHistByHCadr(k);
		bool TurnSpin = (ProgGeom.GetAt(i)->GetType().flags & CU_SPTU) != 0;
		bool IsGroove = ProgGeom.GetAt(i)->IsGroove();
		// Get cadrs from StartHCadr to EndHCadr
		for (; k < EndHCadr && i < ProgGeom.GetSize(); ++i)
		{
			NCadrGeom* pGeom = ProgGeom.GetAt(i);
			if (((pGeom->GetType().flags & CU_SPTU) != 0) != TurnSpin)
				break;
			if (!pGeom->IsCycle())// Cycle attrib
				k++;
		}
		EndHCadr = k;
		StockState = (TurnSpin && !IsGroove) ? AT_TURN : AT_MILL;
	}
	break;
	case AT_MILL:
		StockState = AT_MILL;
		break;
	case AT_TURN:
		StockState = AT_TURN;
		break;
	default:
		StockState = AT_UNDEF;
		break;
	}
}

void FRange::Check4AddCoord()
{
	if (!pProg->HaveAddCoord())
		return;

	const BGeomArray& ProgGeom = pProg->GetCompGeomArray();
	// k - index of current step excluding steps in the cycles
	// i - index of current step
	int k = StartHCadr;
	int i = pProg->GetHistByHCadr(k);
	// Get cadrs from StartHCadr to EndHCadr
	bool IsAddCoord = ProgGeom.GetAt(i)->GetType().type == addcoord;
	for (; k < EndHCadr && i < ProgGeom.GetSize(); ++i)
	{
		NCadrGeom* pGeom = ProgGeom.GetAt(i);
		if ((pGeom->GetType().type == addcoord) != IsAddCoord)
		{
			if (IsAddCoord)
				break;
			IsAddCoord = true;
		}
		if (!pGeom->IsCycle())// Cycle attrib
			k++;
	}
	EndHCadr = k;
}

void FRange::SetCuttingTypeChanged(const FRange& Prev)
{
	const bool StockStateChanged = (Prev.GetStockState() != GetStockState()) || (Prev.GetMTLink() != GetMTLink());
	CuttingTypeChanged = (!Prev.CuttingTypeChanged == StockStateChanged);
}

void FRange::ProcCuttingTypeChanged()
{
	if (CuttingTypeChanged)
	{
		CuttingTypeChanged = false;
		CStocksCollection& Stocks = GetStocksCollection();
		Stocks.ApplyStocksState(StockState, GetStockGr4Turn());
	}
}

//bool FRange::GetRangeMatr(BMatr & Matr) const
//{
//	if (!pProg->HaveStockTransform())
//		return false;
//
//	const BGeomArray &ProgGeom = pProg->GetCompGeomArray();
//	// k - index of current step excluding steps in the cycles
//	// i - index of current step
//	int k = StartHCadr;
//	int i = pProg->GetHistByHCadr(k);
//	// Get cadrs from StartHCadr to EndHCadr
//	for (; k < EndHCadr && i < ProgGeom.GetSize(); ++i)
//	{
//		NCadrGeom *pGeom = ProgGeom.GetAt(i);
//		Matr = Matr * pGeom->GetStockTransfMatr();
//		if (!pGeom->IsCycle())// Cycle attrib
//			k++;
//	}
//	return true;
//}

std::vector<BStockGrID> FRange::GetStockGr4Turn() const
{
	pChannel->GetMachCopy().SetCurConf(MTLink);
	return MTLink.GetStGr2StPos().Find(pChannel->GetUnitCn()->GetStockPos4Turn());
}
