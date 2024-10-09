#include "stdafx.h"
#include "ppltasks.h"
#include <vector>
#include "BRangePos.h"
#include "NCMProject.h"
#include "NCadrGeom.h"
#include "NSurfSettings.h"
#include "MBSPInter.h"
#include "NRectMesh.h"
#include "GAnimateCadr.h"
#include "GQuadRenderRot.h"
#include "GQuadRenderMill.h"
#include "GQuadRenderDX5X.h"
#include "NToolCombined.h"
#include "NToolCombinedMill.h"
#include "NToolCombinedTurn.h"
#include "NTurnProcessing.h"
#include "SProcChain.h"
#include "NGrooveProcessing.h"
#include "NToolStd.h"
#include ".\plane\NCadrsProcessing.h"
#include ".\plane\NCadrsProcessingDX5X.h"
#include "BPointsBuf.h"
#include "DXSubModel5X.h"
#include "FChain.h"
#include "NCollisionLog.h"
#include "MQuadMask.h"
#include "CadrData.h"
#include "BExtrFinder.h"
#include "CStocksCollection.h"
#include "GOneStepGeom.h"
#include "BMatrPair.h"
#include "MLocStorage.h"
#include "NMachUnitPair.h"
#include "FProg2BSP.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using namespace concurrency;

#define MLTI_THREAD_DX (NCMProject::GetProject().GetGlobalState().GetMultiThreadDX() && NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::DX) 

FProg2BSP::FProg2BSP(void)
{
	pMUPair = nullptr;
	pRender = nullptr;
	pAnimState = nullptr;
	NeedNewPosition = false;// Is used for result mode only
	IsRotationActive = false;
	Stop = true;
	Interrupt = false;
	InterruptDX5X = false;
	RotMatrActive = false;
	PauseOnColl = false;
	RotMatr;// Матрица, переводящая GeomArray, переданный в ExecGeomArray в правильное положение
	BodyRotMatr;
	Matr;// Матрица, совмещающая ось инструмента с осью Z
	pBody = nullptr;
	pBodyRotStart = nullptr;
}

FProg2BSP::~FProg2BSP(void)
{
	delete pRender;
	pRender = nullptr;
	ClearAnimState();
}

int FProg2BSP::ApplyMeshs(MeshArr *pPlArray, MQuadBody & Body, int PCNum, cadrID CaID, bool IsFast)
{
	if(!pPlArray)
		return 0;
	int Ret = 0;
	if(Body.GetModelMode() == NGlobalState::BSP)
		Ret = ApplyMeshsBSP(pPlArray, Body, PCNum, CaID, IsFast);
	else if(Body.GetModelMode() == NGlobalState::DX)
		Ret = ApplyMeshsDX(pPlArray, Body, PCNum, CaID, IsFast);
	MeshArr &PlArray = *pPlArray;
	for (int k = 0; k < PlArray.GetSize(); ++k)
		delete PlArray[k];
	return Ret;
}

int FProg2BSP::ApplyMeshsPack(MeshsArr& ChMeshArr, MQuadBody& Body, int PCNum, cadrID CaID, bool IsFast)
{
	int ApplyRet = 0;
	for (int iCh = 0; iCh < ChMeshArr.GetSize(); ++iCh)
	{
		int ARet = ApplyMeshs(ChMeshArr[iCh], Body, PCNum, CaID, IsFast);
		ApplyRet = min(ApplyRet, ARet);
		delete ChMeshArr[iCh];
	}
	return ApplyRet;
}

int FProg2BSP::ApplyMeshsBSP(MeshArr *pPlArray, MQuadBody & Body, int PCNum, cadrID CaID, bool IsFast)
{
	ElemID ElID = ID_EMPTY + SURF_NUM_SHIFT;
	MElemIDProc::SetPCn(&ElID, PCNum);

	MeshArr &PlArray = *pPlArray;
	int Ret = 0;
	if (BDebugState::Get().DrawFaces)
	{
		for (int k = 0; k < PlArray.GetSize(); ++k)
		{
			if (PlArray[k])
			{
				PlArray[k]->StoreMesh(Body, MElemIDProc::ID_DEFAULT, cadrID(-1), false);
			}
		}
		return 0;
	}
	MQuadTree &Tree = static_cast<MQuadTree &>(Body);
	std::vector<MQuadMask *> FirstMask;
	if (NTool::GetToolStockCollision() != CWS_OFF)
	{
		for (MQuadBody *pBody = &Tree; pBody != nullptr; pBody =  static_cast<MQuadBody *>(pBody->GetNextMBody()))
		{
			FirstMask.push_back(new MQuadMask(static_cast<MQuadMask &>(pBody->GetModifiedCells())));
			static_cast<MQuadMask &>(pBody->GetModifiedCells()).Clear();
		}
	}
	BBox Bounds;
	bool IsNotCutting = false;
	bool IsShank = false;
	for (int k = 0; k < PlArray.GetSize(); ++k)
	{
		if (!PlArray[k])
			continue;
		IsNotCutting |= !PlArray[k]->CuttingTool;
		IsShank |= PlArray[k]->Shank;
		PlArray[k]->Expand(Bounds);
	}
	for (MQuadTree *pTreeLoc = &Tree; pTreeLoc != nullptr; pTreeLoc = static_cast<MQuadTree *>(pTreeLoc->GetNextMBody()))
	{
		bool Do = false;
		for each(auto Group in pPlArray->GetStockGroups())
			if (Group == pTreeLoc->GetStockPos())
			{
				Do = true;
				break;
			}
		if (!Do)
			continue;
		pTreeLoc->ClearWindow();// will be expanded in StoreFace
		for (int k = 0; k < PlArray.GetSize(); ++k)
		{
			NParamMesh *pLocMesh = PlArray.GetAt(k);
			if (pLocMesh != nullptr)
			{
				if (IsFast)
				{
					auto ElID = pLocMesh->GetMeshID();
					pLocMesh->SetMeshID(MElemIDProc::SetPCn(&ElID, FAST_COLOR_NUM));
				}
				pLocMesh->StoreMesh(*pTreeLoc, ElID, CaID, true);// Store to aux
			}
		}
		pTreeLoc->SafeMergeChWithCr(MBSPInter(), Bounds, PlArray.IsConvex());
	}
	if (NTool::GetToolStockCollision() != CWS_OFF)
	{
		MQuadTree *pTreeLoc = &Tree;
		for (int l = 0; l < FirstMask.size() && pTreeLoc != nullptr
			; ++l, pTreeLoc = static_cast<MQuadTree *>(pTreeLoc->GetNextMBody()))
		{
			int RetLoc = 0;
			if ((IsFast || IsNotCutting) && NTool::GetToolStockCollision() != CWS_OFF)
			{
				const MQuadMask &SecondMask = static_cast<MQuadMask &>(pTreeLoc->GetModifiedCells());
				if (!SecondMask.IsZero())
					RetLoc = IsFast ? -1 : (IsShank ? -3 : -2);
			}
			Ret = min(Ret, RetLoc);
			static_cast<MQuadMask &>(pTreeLoc->GetModifiedCells()) |= *FirstMask[l];
			delete FirstMask[l];
		}
	}
	return Ret;
}
int FProg2BSP::ApplyMeshsDX(MeshArr *pPlArray, MQuadBody & Body, int PCNum, cadrID CaID, bool IsFast)
{
	// Process single body
	ElemID ElID = ID_EMPTY + SURF_NUM_SHIFT;
	MElemIDProc::SetPCn(&ElID, PCNum);

	MeshArr &PlArray = *pPlArray;
	int Ret = 0;
	bool IsNotCutting = false;
	bool IsShank = false;
	BBox Bounds = PlArray.GetGabar();
	for (DXSubModel5X *pMod5X = (DXSubModel5X *)&Body; pMod5X != nullptr; pMod5X = (DXSubModel5X *)pMod5X->GetNextMBody())
	{
		bool Do = false;
		for each (auto Group in pPlArray->GetStockGroups())
			if (Group == pMod5X->GetStockPos())
			{
				Do = true;
				break;
			}
		if (!Do)
			continue;
		pMod5X->ClearChanged();
		if (Bounds.Intersects(pMod5X->GetGabar()))
		{
			for (int k = 0; k < PlArray.GetSize(); ++k)
			{
				NParamMesh* pLocMesh = PlArray.GetAt(k);
				if (pLocMesh == nullptr)
					continue;
				if (IsFast)
				{
					auto ElID = pLocMesh->GetMeshID();
					pLocMesh->SetMeshID(MElemIDProc::SetPCn(&ElID, FAST_COLOR_NUM));
				}
				IsNotCutting |= !PlArray[k]->CuttingTool;
				IsShank |= PlArray[k]->Shank;
				if (PlArray.IsConvex())
				{
					pLocMesh->StoreMesh2Aux(*pMod5X, ElID, CaID);
				}
				else
				{
					pLocMesh->StoreMesh(*pMod5X, ElID, CaID, false);
				}
			}
			if (PlArray.IsConvex())
			{
				pMod5X->GetAuxDir(0).SetWindow(Bounds);
				pMod5X->GetAuxDir(1).SetWindow(Bounds);
				pMod5X->GetAuxDir(2).SetWindow(Bounds);
				pMod5X->SubtrAllDir();
			}
			else
			{
				pMod5X->GetDirectionA(0).SetWindow(Bounds);
				pMod5X->GetDirectionA(1).SetWindow(Bounds);
				pMod5X->GetDirectionA(2).SetWindow(Bounds);
				pMod5X->SubtrDirectionsA();
			}
			if ((IsFast || IsNotCutting) && NTool::GetToolStockCollision() != CWS_OFF)
			{
				if (pMod5X->IsChanged())
					Ret = IsFast ? -1 : (IsShank ? -3 : -2);

			}
		}
	}

	
#ifdef _DEBUG
//	pMod5X->Check();
#endif	

	return Ret;
}

int FProg2BSP::ExecGeomArray(NCadrGeom * GeomArray, int GSize, int GNumber,
	const NToolCombined *pTool, CStocksCollection &Stocks, const BRangePos &Pos)
{
	// Pos points to first cadr in Result mode and to first cadr in Animate mode
	//Returns a number of processed GeomArray elements or -1 if the tool is not correct
	if(!pTool || GSize < 1)
		return -1;

	if (pTool->IsVertComTool())
	{
		const NToolCombinedMill *pVertComTool = static_cast<const NToolCombinedMill*>(pTool);
		NToolCombinedMill *pVertComToolCompl = nullptr; // complement tool for non vertical cutting
		if (GNumber < 1)
			return -1;
		int Ret = 0;
		bool Has5x = false;
		int Is = 0;
		int Ie = 0;
		int i = 0;
		for (; i < GNumber && Ret >= 0; ++i)
		{
			bool IsVerticalCadr = GeomArray[i].IsVertical();
			if (Has5x != GeomArray[i].IsFull5x())
			{
				if (Is != i)
				{
					if (pVertComToolCompl == nullptr)
					{
						pVertComToolCompl = new NToolCombinedMill;
						bool ToolOK = pVertComToolCompl->MakeComplTool(pVertComTool);
						if (!ToolOK)
							return -1;
					}
					Ie = i;
					if (Has5x)
						Ret = EGAMill5x(GeomArray, Is, GSize - Is, Ie - Is, pVertComToolCompl, Stocks, Pos);
					else
						Ret = EGAMill3x(GeomArray, Is, GSize - Is, Ie - Is, pVertComToolCompl, Stocks, Pos);
					Is = Ie;
				}
				Has5x = GeomArray[i].IsFull5x();
			}
		}
		if (Is != i && Ret >= 0)
		{
			if (pVertComToolCompl == nullptr)
			{
				pVertComToolCompl = new NToolCombinedMill;
				bool ToolOK = pVertComToolCompl->MakeComplTool(pVertComTool);
				if (!ToolOK)
					return -1;
			}
			Ie = i;
			if (Has5x)
				Ret = EGAMill5x(GeomArray, Is, GSize - Is, Ie - Is, pVertComToolCompl, Stocks, Pos);
			else
				Ret = EGAMill3x(GeomArray, Is, GSize - Is, Ie - Is, pVertComToolCompl, Stocks, Pos);
		}
		delete pVertComToolCompl;
		for (int k = 0; k < GNumber && Ret >= 0; ++k)// make drill
		{
			bool IsVerticalCadr = GeomArray[k].IsVertical();
			if (IsVerticalCadr)
			{
				GeomArray[i].Tr(Matr.Inv());// to compensate previous transformation
				Ret = EGAMill3x(GeomArray, k, 1, 1, pTool, Stocks, Pos);
			}
		}
		return Ret;
	}
	if (pTool->GetClass() == AT_TURN)
	{
		int Ret = 0;
		bool HasGroove = false;
		int Is = 0;
		int Ie = 0;
		int i = 0;
		for (; i < GNumber && Ret >= 0; ++i)
		{
			if (HasGroove != GeomArray[i].IsGroove())
			{
				if (Is != i)
				{
					Ie = i;
					if (HasGroove)
						Ret = EGAGroove(GeomArray, Is, GSize - Is, Ie - Is, (NToolCombinedTurn *)pTool, Stocks, Pos);
					else
						Ret = EGATurn(GeomArray, Is, Ie - Is, (NToolCombinedTurn *)pTool, Stocks, Pos);
					Is = Ie;
				}
				HasGroove = GeomArray[i].IsGroove();
			}
		}
		if (Is != i && Ret >= 0)
		{
			Ie = i;
			if (HasGroove)
				Ret = EGAGroove(GeomArray, Is, GSize - Is, Ie - Is, (NToolCombinedTurn *)pTool, Stocks, Pos);
			else
				Ret = EGATurn(GeomArray, Is, Ie - Is, (NToolCombinedTurn *)pTool, Stocks, Pos);
		}
		return Ret;
	}

	if (GNumber < 1)
		return -1;
	int Ret = 0;
	bool Has5x = false;
	int Is = 0;
	int Ie = 0;
	int i = 0;
	for (; i < GNumber && Ret >= 0; ++i)
	{
		if (Has5x != GeomArray[i].IsFull5x())
		{
			if (Is != i)
			{
				Ie = i;
				if (Has5x)
					Ret = EGAMill5x(GeomArray, Is, GSize - Is, Ie - Is, pTool, Stocks, Pos);
				else
					Ret = EGAMill3x(GeomArray, Is, GSize - Is, Ie - Is, pTool, Stocks, Pos);
				Is = Ie;
			}
			Has5x = GeomArray[i].IsFull5x();
		}
	}
	if (Is != i && Ret >= 0)
	{
		Ie = i;
		if (Has5x)
			Ret = EGAMill5x(GeomArray, Is, GSize - Is, Ie - Is, pTool, Stocks, Pos);
		else
			Ret = EGAMill3x(GeomArray, Is, GSize - Is, Ie - Is, pTool, Stocks, Pos);
	}
	return Ret;
}

GBodyRender *FProg2BSP::GetRender()
{
	return pRender;
}

NAnimState *FProg2BSP::GetAnimState()
{
	return pAnimState;
}

void FProg2BSP::ClearAnimState()
{
	delete pAnimState;
	pAnimState = nullptr;
}

void FProg2BSP::ResetInterrupt()
{
	Interrupt = false;
}

void FProg2BSP::SetRender(GBodyRender *pR)
{ 
	pRender = pR; 
}

void FProg2BSP::SetRotMatr(const BMatr &T)
{
	RotMatrActive = true;
	RotMatr = T;
}

void FProg2BSP::SetBodyRotMatr(const BMatr & T)
{
	BodyRotMatr = T;
}

void FProg2BSP::SetBodyRotStart(MBody * pBody)
{
	pBodyRotStart = pBody;
}

void FProg2BSP::SetMatr(const BMatr &T)
{
	Matr = T;
}

void FProg2BSP::SetMatr(const BMatrPair& T)
{
	Matr = T;
}

bool FProg2BSP::ClearRotMatr()
{
	bool b = RotMatrActive;
	RotMatrActive = false;
	return b;
}
void FProg2BSP::ReSetRender(CStocksCollection &Stocks, NCMApplicationType State)
{
	MBody *pBodyInt = Stocks.GetVisibleBodiesList(State);
	if (!pBodyInt)
		return;
	MQuadBody &Body = static_cast<MQuadBody &>(*pBodyInt);
	if(pRender)
	{
		pRender->FillModifArrays(Body);
	}
	else
	{
		GBodyRender * pNewRender = nullptr;
		switch(State)
		{
		case AT_MILL:
			switch(NCM_PROJECT.GetGlobalState().GetModelMode())
			{
			case NGlobalState::DX:
				//pNewRender = new GQuadRenderDX5X((DXSubModel5X &)Body);
				pNewRender = GQuadRenderDX5X::CreateGQuadRenderDX5XList(static_cast<DXSubModel5X &>(Body));
				break;
			case NGlobalState::BSP:
				//pNewRender = new GQuadRenderMill(Body);
				pNewRender = GQuadRenderMill::CreateGQuadRenderMillList(Body);
				break;
			}
			break;
		case AT_TURN:
			//pNewRender = new GQuadRenderRot(Body);
			pNewRender = GQuadRenderRot::CreateGQuadRenderRotList(Body);
			break;
		}
		pNewRender->FillAllArrays(Body);
		pRender = pNewRender;// Extra variable pNewRender is necessary to prevent access to the pRender before render is in fact created
	}
	Body.ClearModified();
}

void FProg2BSP::Init(bool Pause)
{
	delete pRender;
	pRender = nullptr;
	ClearAnimState();
	NeedNewPosition = false;
	IsRotationActive = false;
	Stop = true;
	Interrupt = false;
	RotMatrActive = false;
	RotMatr.SetE();
	Matr.SetE();
	PauseOnColl = Pause;
}
int FProg2BSP::EGATurn(NCadrGeom * GeomArray, int IndStart, int GNumber,
	NToolCombinedTurn *pTool, CStocksCollection &Stocks, const BRangePos &Pos)
{
	// Pos points to first cadr in Result mode and to first cadr in Animate mode
	//Returns a number of processed GeomArray elements or -1 if the tool is not correct
	GeomArray += IndStart;
	MBody *pBodyInt = pBodyRotStart;
	if (pBodyInt == nullptr)
		pBodyInt = Stocks.GetVisibleBodiesList(AT_TURN);
	if (pBodyInt == nullptr)
		return -1;
	if (!pBodyInt->IsRotBody())
		return GNumber - 1;// ignore cutting

	const int StockPos = GeomArray[0].GetStockPos();
	const auto Groups = GeomArray[0].GetMTLink().GetStGr2StPos().Find(StockPos);
	MQuadBody &Body = static_cast<MQuadBody &>(*pBodyInt);
	double b[16] = { 0, 1, 0, 0,// Перевод из XZ в XY (X->Y)
					0, 0, 1, 0,
					1, 0, 0, 0,
					0, 0, 0, 1};
	const BMatr Mb(Matr.Or() * BMatr(b));

	for (int l = 0; l < GNumber; ++l)
		GeomArray[l].Tr(Mb);

	NTurnProcessing NTP;
	if (!NTP.SetTool(pTool))
	{
		if (NCM_PROJECT.GetGlobalState().GetAnimMode() == NGlobalState::Result)
		{
			CollPositions.emplace_back(Pos, LE_EMPTYTOOL);
			CollPositions.back().SetCadrNums(IndStart, IndStart);
		}
		return GNumber - 1;
	}
	NTP.SetTol(NSurfSettings::GetTolerance());
	static int CompI = 0;// For the compression
	int Start;
	for(Start = 0; Start < GNumber && !Interrupt;	Start += 1)
	{
		if(!GeomArray[Start].HaveGeom() || GeomArray[Start].IsGroove() || GeomArray[Start].GetAttr() == 6 /*без обработки(TURN only)*/)
			continue;
		Stocks.SetTurnStockChanged(true);
		NTP.SetCadrNum(Start);
		// Start - relative to GeomArray 
		// GNumber - relative to GeomArray 
		MeshsArr ChMeshArr(0);

		MFOREST.SetMark();
		NTP.CrCadrMeshs(ChMeshArr, GeomArray[Start], Mb);
		BMatr M(BMatr().RotX(BPoint(0., 0., 0., 1.), -90.));
		if(RotMatrActive)
			M = BMatr(b).T() * RotMatr * M;// NT
		else
			M = BMatr(b).T() * M;// NT

		// Extract Y rotation from M. (M = SomeMatr * YRot^(-1)
		BMatr YRot;
		M.ExtractYRot(YRot);
		// ensure X is positive
		BMatr XMatr = M * YRot;
		BBox Gabar = ChMeshArr.GetGabar();
		Gabar.Transform(XMatr);
		const double XExtr = (fabs(Gabar.Xmax) > fabs(Gabar.Xmin)) ? Gabar.Xmax : Gabar.Xmin;
		if (XExtr < 0.)
			XMatr = XMatr * BMatr().RotY(0., 0., 0., 180.);
		// END:ensure X is positive
		MFOREST.TransformFMark(XMatr);
		MFOREST.RemoveMark();
		//		if (StockPos != 0)
		{
			for (int MeshI = 0; MeshI < ChMeshArr.GetSize(); ++MeshI)
				ChMeshArr.GetAt(MeshI)->SetStockPos(Groups);
		}

		int ApplyRet = 0;
		for(int iCh = 0; iCh < ChMeshArr.GetSize(); ++iCh)
		{
			MeshArr *pPlArray = ChMeshArr[iCh];
			if(!ChMeshArr[iCh])
				continue;
			if (pPlArray->GetSize() < 1)
				continue;
			bool IsFast = (FAST_COLOR_NUM == (MFOREST.GetIDProc()->GetPCn((*pPlArray)[0]->GetMeshID())));
			int ARet = ApplyMeshs(pPlArray, Body, 0, GeomArray[Start].GetHistID(), IsFast);
			ApplyRet = min(ApplyRet, ARet);
			delete ChMeshArr[iCh];
		}
		if (NTool::GetToolStockCollision() != CWS_OFF && ApplyRet < 0 && NCM_PROJECT.GetGlobalState().GetAnimMode() == NGlobalState::Result)
		{
			auto CollType = CalcCollType(*pTool, ApplyRet);
			CollPositions.emplace_back(Pos, CollType);
			CollPositions.back().SetCadrNums(Start + IndStart, Start + IndStart);
		}
		if (Start + 1 >= GNumber || Interrupt)
		{
			GeomArray[Start].Tr(BMatr(b).T());
			ReSetRender(Stocks, AT_TURN);

			PrepNewAnimState(nullptr, &GeomArray[Start], Pos, Start);
		}
		Sleep(0);
		int MemCompressStep = 16;
		if((++CompI) % MemCompressStep == 0)
			MFOREST.Compress();
	}
//	MFOREST.Compress();

	int ReturnNumber = max(0, ((Start >= GNumber ? GNumber : Start) - 1));
	if(Interrupt)
		ReturnNumber = -ReturnNumber - 1;

	Interrupt = false;
	
	return ReturnNumber;
}

CollLogElemType FProg2BSP::CalcCollType(const NToolCombined& Tool, int ApplyRet)
{
	if (ApplyRet < -1)
	{
		if (Tool.GetName() == _T("ComplTool"))
			return LE_COLLVERT;
		return ApplyRet == -2 ? LE_COLLHOLD : LE_COLLSHANK;
	}
	return LE_COLLTOOL;
}

bool FProg2BSP::PrepNewAnimState(const NToolCombined *pTool, const NCadrGeom *pGeom, const BRangePos &Pos, int Ind)
{// pGeom must have movement
	// This method works for rerendering mode only
	if(!pGeom)
		return false;
	const cadrID CadrID = Pos.FindCurCadr(Pos.CadrNum + Ind);
	const cadrID CadrIDhist = Pos.FindCurHCadr(Pos.CadrNum + Ind);
	NCadrGeom TrCadr(*pGeom);
	if(pAnimState)
		pAnimState->Set(pRender, pTool, TrCadr.Tr(Matr.Inv()), CadrID, CadrIDhist, 1.);
	else
		pAnimState = new NAnimState(pRender, pTool, TrCadr.Tr(Matr.Inv()), CadrID, CadrIDhist, 1.);
	return true;
}

bool FProg2BSP::PrepNewPosition(const BRangePos &Pos, int Ind)
{
	if (!NeedNewPosition)
		return false;
	NeedNewPosition = false;
	// This method works for result mode only
	cadrID CadrID = Pos.FindCurCadr(Pos.CadrNum + Ind);
	cadrID CadrIDhist = Pos.FindCurHCadr(Pos.CadrNum + Ind);
	if (pAnimState)
		pAnimState->Set(pRender, CadrID, CadrIDhist);
	else
		pAnimState = new NAnimState(pRender, CadrID, CadrIDhist);
	return true;
}

void FProg2BSP::SetMT(NMachUnitPair *pMU)
{
	pMUPair = pMU;
}

int FProg2BSP::EGAMill5x(NCadrGeom * GeomArray, int IndStart, int GSize, int GNumber,
	const NToolCombined *pTool, CStocksCollection &Stocks, const BRangePos &Pos)
{
	// Pos points to first cadr in Result mode and to first cadr in Animate mode
	GeomArray += IndStart;
	MBody *pBodyInt = pBodyRotStart;
	if (pBodyInt == nullptr)
		pBodyInt = Stocks.GetVisibleBodiesList(AT_MILL);
	if (pBodyInt == nullptr)
		return -1;
	if (pBodyInt->IsRotBody())
		return GNumber - 1;// ignore cutting

	const int StockPos = GeomArray[0].GetStockPos();
	const auto Groups = GeomArray[0].GetMTLink().GetStGr2StPos().Find(StockPos);
	MQuadBody &Body = static_cast<MQuadBody &>(*pBodyInt);
	SProcChain SPC(*pMUPair, NSurfSettings::GetTolerance());
	SPC.SetTool(pTool);

	int MemCompressStep = (NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::BSP) ? 100 : 512;
	static INT_PTR CompI = 0;// For the compression
	INT_PTR ChainEnd = 0;// Needed to return right value. We can't place CurChain declaration here.
	INT_PTR ChainLength;
	INT_PTR Start = 0;
	for(; Start < GNumber && !Interrupt;	Start += ChainLength)
	{
		Stocks.SetMillStockChanged(true);
		// Start - relative to GeomArray 
		// GNumber - relative to GeomArray 
		ChainLength = SPC.SendCadrsArray(GeomArray, max(GNumber, Start + 1), Start);

		if(!SPC.IsDefined())// Undef cadrs were sent
			continue;


		bool LastFragment = (Start + ChainLength >= GNumber);
		bool IsFast = false;
		BMatr M = BodyRotMatr;
		// This fragment needed for AT_MILL_TURN only!
		if (NCMComData::GetType() == AT_MILL_TURN)
		{
			M = BMatr().RotX(BPoint(0., 0., 0., 1.), -90.) * M;

		}
		// END:This fragment needed for AT_MILL_TURN only
		MeshsArr ChMeshArr;
		MFOREST.SetMark();
		SPC.CrMeshs(ChMeshArr);
		MFOREST.TransformFMark( M );
		MFOREST.RemoveMark();
		//		if (StockPos != 0)
		{
			for (int MeshI = 0; MeshI < ChMeshArr.GetSize(); ++MeshI)
				ChMeshArr.GetAt(MeshI)->SetStockPos(Groups);
		}

		int PCNum = MElemIDProc::AddPC(MElemIDProc::MakePC(pTool->Color)) + RESERVED_PCS;
		int ApplyRet = ApplyMeshsPack(ChMeshArr, Body, PCNum, GeomArray[Start].GetHistID(), IsFast);

		if (NTool::GetToolStockCollision() != CWS_OFF && ApplyRet < 0)
		{
//			ToolStockCollType = ApplyRet == -2 ? LE_COLLHOLD : LE_COLLTOOL;
		}
		PrepNewPosition(Pos, int(Start + ChainLength - 1));
		CompI += ChainLength;
		if (CompI  > MemCompressStep)
		{
			MFOREST.Compress();
			CompI = 0;
		}
		ChainEnd = Start + ChainLength;
	}
	if (Interrupt)
	{
		// Find last processed GeomCadr
		NCadrGeom *pGeom = nullptr;
		for (auto i = ChainLength - 1; i >= 0; --i)
			if (!(pGeom = &GeomArray[Start + i])->IsUndef())
				break;
		ReSetRender(Stocks, AT_MILL);
		PrepNewAnimState(nullptr, pGeom, Pos, int(Start + ChainLength - 1));
		InterruptDX5X = Interrupt;
	}
	//	MFOREST.Compress();

	auto ReturnNumber = (Start >= GNumber ? GNumber : Start + ChainEnd) - 1;
	
	Interrupt = false;

	return (int)ReturnNumber;
}
int FProg2BSP::EGAMillDX(GOneStepGeom & OneStepGeom, int iStartInd, int GNumber, const NToolCombined *pTool, CStocksCollection &Stocks, const BRangePos &Pos, DX_TOOLTYPE DXType)
{
	// Pos points to first cadr in Result mode and to first cadr in Animate mode
	if (pTool->num_tools < 1)
	{
		if (NCM_PROJECT.GetGlobalState().GetAnimMode() == NGlobalState::Result)
		{
			CollPositions.emplace_back(Pos, LE_EMPTYTOOL);
			CollPositions.back().SetCadrNums(iStartInd, iStartInd);
		}
		return GNumber;
	}
	const int StockPos = OneStepGeom[0]->GetStockPos();
	const auto Groups = OneStepGeom[0]->GetMTLink().GetStGr2StPos().Find(StockPos);
	auto iMax = min(OneStepGeom.GetSize(), GNumber); /* StartInd + GNumber */
	int i = iStartInd;
	// Find first cadr and previous start point
	for(; i < iMax; ++i)
		if (OneStepGeom[i]->HaveGeom5())
			break;
	if(i == iMax)
	{
		NeedNewPosition = false;
		Interrupt = false;
		return i;
	}
	MBody *pBodyInt = pBodyRotStart;
	if (pBodyInt == nullptr)
		pBodyInt = Stocks.GetVisibleBodiesList(AT_MILL);
	if (pBodyInt == nullptr)
		return -1;
	if (pBodyInt->IsRotBody())
		return int(iMax);// ignore cutting

	if (DXType == TT_NO)
		return ExecGeomArrayDXC(OneStepGeom, iStartInd, GNumber, pTool, Stocks, Pos);
	else if (DXType == TT_5X_BALL)
	{
		NToolCombined *pCopyTool = pTool->CreateCopyTool();
		pCopyTool->RemoveEndBall();
		int Res = ExecGeomArrayDXC(OneStepGeom, iStartInd, GNumber, pCopyTool, Stocks, Pos);
		delete pCopyTool;
		DXType = TT_5X_FULL_BALL;
	}

	ElemID ElID = ID_EMPTY + SURF_NUM_SHIFT;
	MElemIDProc::SetPCn(&ElID, MElemIDProc::AddPC(MElemIDProc::MakePC(pTool->Color)) + RESERVED_PCS);
	NTool *pCutTool = (NToolStd *)(pTool->GetLowermostTool());
	double ToolH = pCutTool->Gabar.dZ();
	double Diameter; double CornerRadius; double ELength; double FLength; double BottomAngle; double SideAngle; double Height;
	pCutTool->GetStdParams(Diameter, CornerRadius, ELength, FLength, BottomAngle, SideAngle, Height);
	double ToolR = CornerRadius;
	if (DXType == TT_3X_CYL)
		ToolR = Diameter / 2.;
	double ZDisp = pCutTool->GetZdisp();// + ToolR;
	BMatr MTMatr;
	// This fragment needed for AT_MILL_TURN only!
	if (NCMComData::IsTurnEnabled())
	{
		MTMatr.RotX(BPoint(0., 0., 0., 1.), -90.);
	}
	// END:This fragment needed for AT_MILL_TURN only
	BMatr LocRotMatr = RotMatr * BodyRotMatr;

	BMatr Shift = Matr.Or() * BMatr().Trans(0., 0., ZDisp) * LocRotMatr * MTMatr;
	// Find last cadr and last end point
	auto iLast = iMax - 1;
	for(; iLast > i; --iLast)
		if (OneStepGeom[iLast]->HaveGeom5())
			break;
	BPoint NextEndP(0);
	BPoint LastEndP(OneStepGeom[iLast]->GetEP() + BPoint(0., 0., 1., 0.));// StartInd may be 0 and so [i - 1] can't work
	if (DXType == TT_5X_FULL_BALL && !OneStepGeom[iLast]->IsRotate())
	{
		LastEndP = (LastEndP * Matr.Or() * BMatr().Trans(0., 0., ToolR) * LocRotMatr);
	}
	// Main loop

	BPoint Base(0., 0., ZDisp, 1.);
	if(DXType == TT_5X_FULL_BALL)
		Base.Set(0., 0., ToolR + ZDisp, 1.);
	int StartInd = i;
	bool LastCadr = true;
	BExtrFinder ExtrFinder;
	BPoint P0, P1;
	BPoint Q1;
	BPointsBuf Pts;
	if (OneStepGeom[StartInd]->IsArc() || OneStepGeom[StartInd]->IsRotate())
	{
		RecalcPts(OneStepGeom[StartInd], Pts, Base);
		P0 = Pts.GetPoint(0);
		P1 = Pts.GetPoint(1);
	}
	else
	{
		P0 = OneStepGeom[StartInd]->GetBP();
		P1 = OneStepGeom[StartInd]->GetEP();
	}
	if (DXType == TT_5X_FULL_BALL && !OneStepGeom[StartInd]->IsRotate())
	{
		P0 = (P0 * Matr.Or() * BMatr().Trans(0., 0., ToolR) * LocRotMatr);
		P1 = (P1 * Matr.Or() * BMatr().Trans(0., 0., ToolR) * LocRotMatr);
	}
	BPoint ToolZDir = BPoint(0., 0., 1., 0.) * Matr.Inv();
	DXSubModel &StockDXModel = static_cast<DXSubModel &>(*pBodyInt);
	for (INT_PTR iNext = StartInd; iNext <= iLast && !Interrupt;) // Extra cycle for compress only
	{
		Stocks.SetMillStockChanged(true);
		pBody = &StockDXModel;
		//StartCutting1DirThreads();
		static int PartInd = 0; // PartInd is used for compress only
		for(; iNext <= iLast && !Interrupt; ++PartInd)
		{
			for (MBody *pNextMBody = &StockDXModel; pNextMBody != nullptr; pNextMBody = pNextMBody->GetNextMBody())
				((DXSubModel*)pNextMBody)->ClearChanged();
			if(PartInd > 50000) // PartInd is used for compress only
				break;
			i = int(iNext);
			iNext = OneStepGeom.GetNextInd(iNext);
			const NCadrGeom *pCur = OneStepGeom[i];
			if(!pCur->HaveGeom5())
				continue;
			CadrData MaskCadrData = GetSpecMask(pCur);
			if(pCur->IsArc() || pCur->IsRotate())
			{ // Insert all lines except the last line
				int ia = 1;
				for(; ia < Pts.GetSize() - 1; ++ia)
				{
					Q1 = Pts.GetPoint(ia + 1);
					if (DXType == TT_5X_FULL_BALL && !pCur->IsRotate())
					{
						Q1 = (Q1 * Matr.Or() * BMatr().Trans(0., 0., ToolR) * LocRotMatr);
					}
					if (LastCadr)
					{
						ExtrFinder.Start(P0);
						ExtrFinder.NewPoint(P1);
					}
					bool FirstCadr = LastCadr;
					LastCadr = ExtrFinder.NewPoint(Q1);
					InsSpecCadr(NCM_PROJECT.GetGlobalState().GetModelMode(), DXType, P0, P1
							, ToolZDir
							, ToolR, ToolH, DX_ID(ElID, pCur->GetHistID()), Shift
							, FirstCadr
							, LastCadr
							, Groups
							, MaskCadrData);

					P0 = P1;
					P1 = Q1;
					FirstCadr = false;
				}
			}
			// Find next cadr
			NCadrGeom *pNext = nullptr;
			if(i == iLast)
			{
					Q1 = LastEndP;
			}
			else
			{
				for(; iNext <= OneStepGeom.GetSize(); iNext = OneStepGeom.GetNextInd(iNext))
				{
					pNext = OneStepGeom[iNext];
					if(pNext->HaveGeom5())
						break;
				}
				if(pNext->IsArc() || pNext->IsRotate())
				{
					Pts.SetSize(0);
					RecalcPts(pNext, Pts, Base);
					Q1 = Pts.GetPoint(1);
				}
				else
				{
					Q1 = pNext->GetEP();
				}
				if (DXType == TT_5X_FULL_BALL && !pNext->IsRotate())
				{
					Q1 = (Q1 * Matr.Or() * BMatr().Trans(0., 0., ToolR) * LocRotMatr);
				}
			}
			ElemID ElIDb; // For fast color
			bool ProcAsFast = false;
			switch (NTool::GetFastColorMode())
			{
			case FCM_ALL:
				ProcAsFast = pCur->IsFast();
				break;
			case FCM_NO:
				break;
			case FCM_ALLBUTVERT:
				ProcAsFast = pCur->IsFast() && !pCur->IsVerticalUp();
				break;
			}
			if (ProcAsFast)
			{
				ElIDb = ElID;
				MElemIDProc::SetPCn(&ElID, FAST_COLOR_NUM);
			}
			if(LastCadr)
			{
				ExtrFinder.Start(P0);
				ExtrFinder.NewPoint(P1);
			}
			bool FirstCadr = LastCadr;
			if(iNext > iLast)
				LastCadr = true;
			else
				LastCadr = ExtrFinder.NewPoint(Q1);
			InsSpecCadr(NCM_PROJECT.GetGlobalState().GetModelMode(), DXType, P0, P1
				, ToolZDir
				, ToolR, ToolH, DX_ID(ElID, pCur->GetHistID()), Shift
				, FirstCadr
				, LastCadr
				, Groups
				, MaskCadrData);

			P0 = P1;
			P1 = Q1;
			if (ProcAsFast)
				ElID = ElIDb;
			if(NTool::GetToolStockCollision() != CWS_OFF && pCur->IsFast() && NCM_PROJECT.GetGlobalState().GetAnimMode() == NGlobalState::Result)
			{
				bool StockIsChanged = false;
				for (DXSubModel5X* pMod5X = (DXSubModel5X*)& StockDXModel; pMod5X != nullptr; pMod5X = (DXSubModel5X*)pMod5X->GetNextMBody())
					StockIsChanged |= pMod5X->IsChanged();
				bool CollFound = false;
				if (StockIsChanged)
				{
					CollFound = true;
					auto CollType = LE_COLLTOOL;
					if (pTool->IsIgnoreUpToolStockColl() && CollType == LE_COLLTOOL)
					{
						// Find last processed GeomCadr
						NCadrGeom* pGeom = nullptr;
						int l = i;
						for (; l >= 0; --l)
							if (!(pGeom = OneStepGeom[l])->IsUndef())
								break;
						if (pGeom)
						{
							if (pGeom->IsVertical() && pGeom->GetBP().GetZ() < pGeom->GetEP().GetZ())
							{
								CollFound = false;
								Interrupt = false;
							}
						}
					}
					if (CollFound)
					{
						CollPositions.emplace_back(Pos, CollType);
						CollPositions.back().SetCadrNums(i, i);
					}
				}
			}
			PrepNewPosition(Pos, i);
		}
		if (PartInd >= 50000)
		{
			Wait4Empty1DirThreads();
			PartInd = 0;
			MFOREST.Compress();
		}
	}
	
	NeedNewPosition = false;
	if(Interrupt)
		i = -i - 1;
	Interrupt = false;
	return i;
}

int FProg2BSP::EGAMill3x(NCadrGeom * GeomArray, int IndStart,  int GSize, int GNumber,
	const NToolCombined *pCutTool, CStocksCollection &Stocks, const BRangePos &Pos)
{
	// Pos points to first cadr in Result mode and to first cadr in Animate mode
	GeomArray += IndStart;
	MBody *pBodyInt = pBodyRotStart;
	if (pBodyInt == nullptr)
		pBodyInt = Stocks.GetVisibleBodiesList(AT_MILL);
	if (pBodyInt == nullptr)
		return -1;
	if (pBodyInt->IsRotBody())
		return GNumber - 1;// ignore cutting

	MQuadBody &Body = static_cast<MQuadBody &>(*pBodyInt);
	for (int l = 0; l < GSize; ++l)
		GeomArray[l].Tr(Matr.Or());

	int StockPos = GeomArray[0].GetStockPos();
	const auto Groups = GeomArray[0].GetMTLink().GetStGr2StPos().Find(StockPos);
	NCadrsProcessing NCP;
	if (NCP.BeginIssue(pCutTool) < 0)
	{
		if (NCM_PROJECT.GetGlobalState().GetAnimMode() == NGlobalState::Result)
		{
			CollPositions.emplace_back(Pos, LE_EMPTYTOOL);
			CollPositions.back().SetCadrNums(IndStart, IndStart);
		}
		return GNumber - 1;
	}

	int MemCompressStep = 
		(NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::BSP) ? 320 : 512;

	static int CompI = 0;// For the compression
	INT_PTR ChainEnd = 0;// Needed to return right value. We can't place CurChain declaration here.
	
	INT_PTR Start = 0;
	for(INT_PTR FragmentLength; Start < GNumber && !Interrupt; Start += FragmentLength)
	{
		Stocks.SetMillStockChanged(true);
		// k - relative to NCP.Cadrs 
		// Start - relative to GeomArray 
		// GNumber - relative to GeomArray 
		// Chain - relative to NCP.Cadrs
		FChain CurChain;
		CurChain.MaxNumber = GNumber - Start;
		CurChain.End = GNumber;// This is necessary if next "for" will not run into it's body
		FragmentLength = NCP.SendCadrsArray(GeomArray, GSize, Start);
		if(NCP.IsApproximated())
			CurChain.MaxNumber = FragmentLength;
		FragmentLength = min(FragmentLength, CurChain.MaxNumber);
		bool LastFragment = (Start + FragmentLength >= GNumber);

		for(CurChain.Start = NCP.GetFirstGeomCadr(0);
			CurChain.Start < FragmentLength && !Interrupt;
			CurChain.Start = CurChain.End)
		{
			MeshsArr* pChMeshArr = new MeshsArr(pCutTool->num_tools);
			bool IsFast = false;
			BMatr M = RotMatr * BodyRotMatr;
			// This fragment needed for AT_MILL_TURN only!
			if (NCMComData::GetType() == AT_MILL_TURN)
			{
				M = M * BMatr().RotX(BPoint(0., 0., 0., 1.), -90.);
			}
			// END:This fragment needed for AT_MILL_TURN only

			NCP.FindMaxChain(CurChain);

			MFOREST.SetMark();
			IsFast = NCP.CrChainMeshs(*pChMeshArr, CurChain);
			MFOREST.TransformFMark(M);
			MFOREST.RemoveMark();
			//		if (StockPos != 0)
			{
				for (int MeshI = 0; MeshI < pChMeshArr->GetSize(); ++MeshI)
					pChMeshArr->GetAt(MeshI)->SetStockPos(Groups);
			}
			bool LastChain = (CurChain.End >= FragmentLength);
			const int PCNum = MElemIDProc::AddPC(MElemIDProc::MakePC(pCutTool->Color)) + RESERVED_PCS;
			cadrID MixedCadr(-1);
			if(CurChain.GetType() != CHAIN_EMPTY)
				MixedCadr.MakeMix(NCP.GetCadr(int(CurChain.Start))->GetHistID(), NCP.GetCadr(int(CurChain.End - 1))->GetHistID());
			const int ApplyRet = ApplyMeshsPack(*pChMeshArr, Body, PCNum, MixedCadr, IsFast);
			delete pChMeshArr;
			if(NTool::GetToolStockCollision() != CWS_OFF && ApplyRet < 0 && NCM_PROJECT.GetGlobalState().GetAnimMode() == NGlobalState::Result)
			{
				bool CollFound = true;
				auto CollType = CalcCollType(*pCutTool, ApplyRet);
				// Find last processed GeomCadr
				NCadrGeom* pGeom = nullptr;
				for (auto i = CurChain.End - 1; i >= 0; --i)
					if (!(pGeom = NCP.GetCadr(int(i)))->IsUndef())
						break;
				if (pCutTool->IsIgnoreUpToolStockColl() && pGeom && CollType == LE_COLLTOOL)
					if (pGeom->IsVertical() && pGeom->GetBP().GetZ() < pGeom->GetEP().GetZ())
					{
						CollFound = false;
						Interrupt = false;
					}
				if (CollFound)
				{
					CollPositions.emplace_back(Pos, CollType);
					CollPositions.back().SetCadrNums(int(Start + CurChain.End - 1 + IndStart), int(Start + CurChain.Start + IndStart));
					if (Interrupt)
					{
						ReSetRender(Stocks, AT_MILL);
						PrepNewAnimState(nullptr, pGeom, Pos, int(Start + (NCP.IsApproximated() ? 0 : CurChain.End - 1)));
					}
				}
			}
			PrepNewPosition(Pos, int(Start + (NCP.IsApproximated() ? 0 : CurChain.End - 1)));
			++CompI;
			if (CompI  > MemCompressStep)
			{
				MFOREST.Compress();
				CompI = 0;
			}
		}
		if(NCP.IsApproximated())
		{
			FragmentLength = 1;
			CurChain.End = 1;
		}
		ChainEnd = CurChain.End;
	}

	auto ReturnNumber = (Start >= GNumber ? GNumber : Start + ChainEnd) - 1;
	if(Interrupt)
		ReturnNumber = -ReturnNumber - 1;
	
	InterruptDX5X = Interrupt;
	Interrupt = false;

	return int(ReturnNumber);
}

int FProg2BSP::EGAGroove(NCadrGeom * GeomArray, int IndStart, int GSize, int GNumber,
	const NToolCombinedTurn *pTool, CStocksCollection &Stocks, const BRangePos &Pos)
{
	// Pos points to first cadr in Result mode and to first cadr in Animate mode
	GeomArray += IndStart;
	MBody *pBodyInt = pBodyRotStart;
	if (pBodyInt == nullptr)
		pBodyInt = Stocks.GetVisibleBodiesList(AT_MILL);
	if (pBodyInt == nullptr)
		return -1;
	if (pBodyInt->IsRotBody())
		return GNumber - 1;// ignore cutting
	if (pTool->num_tools < 1)
	{
		if (NCM_PROJECT.GetGlobalState().GetAnimMode() == NGlobalState::Result)
		{
			CollPositions.emplace_back(Pos, LE_EMPTYTOOL);
			CollPositions.back().SetCadrNums(IndStart, IndStart);
		}
		return GNumber - 1;
	}

	const int StockPos = GeomArray[0].GetStockPos();
	const auto Groups = GeomArray[0].GetMTLink().GetStGr2StPos().Find(StockPos);
	MQuadBody &Body = static_cast<MQuadBody &>(*pBodyInt);
	NGrooveProcessing NGP(pMUPair, NSurfSettings::GetTolerance());
	NGP.SetTool(pTool);

	int MemCompressStep = (NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::BSP) ? 16 : 512;
	static INT_PTR CompI = 0;// For the compression
	INT_PTR ChainEnd = 0;// Needed to return right value. We can't place CurChain declaration here.
	INT_PTR ChainLength = 1;
	int Start = 0;
	for(; Start < GNumber && !Interrupt; Start += int(ChainLength))
	{
		// Start - relative to GeomArray 
		// GNumber - relative to GeomArray 
		ChainLength = NGP.SendCadrsArray(GeomArray, GNumber, Start);

		if(!NGP.IsDefined())// Undef cadrs were sent
			continue;

		Stocks.SetMillStockChanged(true);
		MeshsArr ChMeshArr;

		const bool LastFragment = (Start + ChainLength >= GNumber);
		BMatr XZ = { 0,0,1,0,
		0,-1,0,0,
		1,0,0,0,
		0,0,0,1 };
		BMatr M = XZ * RotMatr * BodyRotMatr;
		// This fragment needed for AT_MILL_TURN only!
		if (NCMComData::GetType() == AT_MILL_TURN)
		{
			M = M * BMatr().RotX(BPoint(0., 0., 0., 1.), -90.);
		}
		// END:This fragment needed for AT_MILL_TURN only
		MFOREST.SetMark();
		NGP.CrMeshs(ChMeshArr);
		MFOREST.TransformFMark( M );
		MFOREST.RemoveMark();
//		if (StockPos != 0)
		{
			for (int MeshI = 0; MeshI < ChMeshArr.GetSize(); ++MeshI)
				ChMeshArr.GetAt(MeshI)->SetStockPos(Groups);
		}
		int ApplyRet = 0;
		int PCNum = MElemIDProc::AddPC(MElemIDProc::MakePC(pTool->Color)) + RESERVED_PCS;
		const bool IsFast = NGP.Path.IsFast();
		if (IsFast)
			PCNum = FAST_COLOR_NUM;
		for(int iCh = 0; iCh < ChMeshArr.GetSize(); ++iCh)
		{
			MeshArr *pPlArray = ChMeshArr[iCh];
			if (!ChMeshArr[iCh])
				continue;
			if (pPlArray->GetSize() < 1)
				continue;
			const int ARet = ApplyMeshs(pPlArray, Body, PCNum, GeomArray[Start].GetHistID(), IsFast);
			ApplyRet = min(ApplyRet, ARet);
			delete ChMeshArr[iCh];
		}
		if (NTool::GetToolStockCollision() != CWS_OFF && ApplyRet < 0 && NCM_PROJECT.GetGlobalState().GetAnimMode() == NGlobalState::Result)
		{
			auto CollType = CalcCollType(*pTool, ApplyRet);
			CollPositions.emplace_back(Pos, CollType);
			CollPositions.back().SetCadrNums(Start + IndStart, Start + IndStart);
		}

		PrepNewPosition(Pos, int(Start + ChainLength - 1));
		if ((++CompI) % MemCompressStep == 0)
			MFOREST.Compress();
		ChainEnd = Start + ChainLength;

	}
	if (Interrupt)
	{
		// Find last processed GeomCadr
		NCadrGeom *pGeom = nullptr;
		for (auto i = ChainLength - 1; i >= 0; --i)
			if (!(pGeom = &GeomArray[Start + i])->IsUndef())
				break;
		ReSetRender(Stocks, AT_MILL);
		PrepNewAnimState(nullptr, pGeom, Pos, int(Start + ChainLength - 1));
	}
	//	MFOREST.Compress();

	const auto ReturnNumber = (Start >= GNumber ? GNumber : Start + ChainEnd) - 1;
	
	Interrupt = false;

	return int(ReturnNumber);
}

int FProg2BSP::ExecGeomArrayDXC(const GOneStepGeom & GeomArray, int StartInd, int GNumber, const NToolCombined *pTool, CStocksCollection &Stocks, const BRangePos &Pos)
{
	// Pos points to first cadr in Result mode and to first cadr in Animate mode
	//Returns a number of processed GeomArray elements or -1 if the tool is not correct
	Wait4Empty1DirThreads();

	const int MemCompressStep = 5120;
	int i = StartInd;
	for (; i < min(GeomArray.GetSize(), GNumber); ++i)
	{
		NCadrGeom *pCur = GeomArray[i];
		int GSizeInt = 2;
		int GNumberInt = 1;
		NCadrGeom *Buf = nullptr;
		NCadrGeom BufB[2];
		NCadrGeom *pBuf = nullptr;
		if (pCur->IsArc() && !pCur->IsCylindric())
		{
			BPointsBuf Pts;
			pCur->GetPolyLine(Pts, NSurfSettings::GetTolerance(), false);

			GNumberInt = Pts.GetSize() - 1;
			GSizeInt = GNumberInt + 1;
			pBuf = new NCadrGeom[GSizeInt]; // for delete only
			Buf = pBuf;
			BPoint P0 = Pts.GetPoint(0);
			for (int k = 1; k <= GNumberInt; ++k)
			{
				BPoint P1 = Pts.GetPoint(k);
				Buf[k - 1] = *pCur;
				Buf[k - 1].SetType(line);
				Buf[k - 1].SetB(P0);
				Buf[k - 1].SetE(P1);
				P0 = P1;
			}
		}
		else
		{
			Buf = BufB;
			Buf[0] = *pCur;
		}
		int ret = 0;
		for (int c = 0; c < GNumberInt; ++c)
		{
			// Add one extra geom cadr
			BufB[0] = Buf[c];
			BufB[1] = BufB[0];
			BufB[1].Reverse();

			auto OldSize = CollPositions.size();

			ret = ExecGeomArray(BufB, 2, 1, pTool, Stocks, Pos);

			// recalc new collisions if any
			for (auto k = OldSize; k < CollPositions.size(); ++k)
				CollPositions.at(k) += i;

			if (ret < 0 || FProg2BSP::InterruptDX5X)
			{
				break;
			}
		}
		delete[] pBuf;
		if (ret < 0 || FProg2BSP::InterruptDX5X)
			break;
		if ((i + 1) % MemCompressStep == 0)
		{
			MFOREST.Compress();
		}
	}
	FProg2BSP::InterruptDX5X = false;
	auto ReturnNumber = i - StartInd;

	return ReturnNumber;
}

int FProg2BSP::ExecGeomArrayDXC(NCadrGeom * Geom, int GSize, int GNumber, const NToolCombined *pTool, class CStocksCollection &Stocks, const BRangePos &Pos)
{
	// Pos points to first cadr in Result mode and to first cadr in Animate mode
	if (!pTool || GSize < 1)
		return -1;

	Wait4Empty1DirThreads();

	BRangePos LocPos = Pos;
	//Returns a number of processed GeomArray elements or -1 if the tool is not correct
	const int MemCompressStep = 5120;
	int i = 0;
	for (; i < GNumber; ++i)
	{
		
		NCadrGeom *pCur = &Geom[i];
		//if (pCur->IsGroove())
		//{
		//	int k = i + 1;
		//	for (; Geom[k].IsGroove() && k < GNumber; ++k);
		//	LocPos.CadrNum += k - i;
		//	int ret = 0;
		//	ret = FProg2BSP::ExecGeomArray(Geom + i, k - i + 1, k - i, pTool, Stocks, LocPos);
		//	i = k - 1;
		//	if (ret < 0 || FProg2BSP::InterruptDX5X)
		//		break;
		//	continue;
		//}
		int GSizeInt = 2;
		int GNumberInt = 1;
		NCadrGeom *Buf = nullptr;
		NCadrGeom BufB[2];
		NCadrGeom *pBuf = nullptr;
		if (pCur->IsArc() && !pCur->IsCylindric())
		{
			BPointsBuf Pts;
			pCur->GetPolyLine(Pts, NSurfSettings::GetTolerance(), false);

			GNumberInt = Pts.GetSize() - 1;
			GSizeInt = GNumberInt + 1;
			pBuf = new NCadrGeom[GSizeInt]; // for delete only
			Buf = pBuf;
			BPoint P0 = Pts.GetPoint(0);
			for (int k = 1; k <= GNumberInt; ++k)
			{
				BPoint P1 = Pts.GetPoint(k);
				Buf[k - 1] = *pCur;
				Buf[k - 1].SetType(line);
				Buf[k - 1].SetB(P0);
				Buf[k - 1].SetE(P1);
				P0 = P1;
			}
		}
		else
		{
			Buf = BufB;
			Buf[0] = *pCur;
		}
		++LocPos.CadrNum;
		int ret = 0;
		for (int c = 0; c < GNumberInt; ++c)
		{
			// Add one extra geom cadr
			BufB[0] = Buf[c];
			BufB[1] = BufB[0];
			BufB[1].Reverse();

			auto OldSize = CollPositions.size();

			ret = ExecGeomArray(BufB, 2, 1, pTool, Stocks, LocPos);

			// recalc new collisions if any
			for (auto k = OldSize; k < CollPositions.size(); ++k)
				CollPositions.at(k) += i;

			if (ret < 0 || FProg2BSP::InterruptDX5X)
			{
				break;
			}
		}
		delete[] pBuf;
		if (ret < 0 || FProg2BSP::InterruptDX5X)
			break;
		if ((i + 1) % MemCompressStep == 0)
		{
			MFOREST.Compress();
		}
	}
	FProg2BSP::InterruptDX5X = false;
//	MFOREST.Compress();
//	StartCutting1DirThreads();
	auto ReturnNumber = i;

	return ReturnNumber;
}

bool FProg2BSP::WaitAllCuttings(void)
{
	if (Cutting1DirThreads.empty())
		return true;
	when_all(begin(Cutting1DirThreads), end(Cutting1DirThreads)).wait();
	Cutting1DirThreads.clear();
	return true;
}

UINT FProg2BSP::CuttingThreadProc1Dir( LPVOID pParam )
{
	int Res = 0;
	int Dir = *((int *)pParam);
	CadrData *pData = nullptr;
	for(BSBResult BRes = SlideBufferCadr.GetNext(Dir, pData); BRes != BSB_FINISHED; BRes = SlideBufferCadr.GetNext(Dir, pData))
	{
		switch(BRes)
		{
		case BSB_OK:
			if (pBody)
				((DXSubModel5X *)pBody)->RastSpecCadr(Dir, *pData);
			if (pData->IsLast())
				SlideBufferCadr.SetEmptyEvent(Dir);
			break;
		case BSB_NOT_YET:
			SlideBufferCadr.WaitForResume();
			Sleep(SleepTime);
			break;
		}
	}
	for (MBody * pBodyInt = pBody; pBodyInt != nullptr; pBodyInt = pBodyInt->GetNextMBody())
		((DXSubModel5X *)pBodyInt)->Subtr1Dir(Dir);
	return Res;   // thread completed successfully
}

int FProg2BSP::InsSpecCadr(NGlobalState::ModelModes ModelMode, DX_TOOLTYPE DXType,
		const BPoint & P0v, const BPoint & P1v, const BPoint & Nv,
		double R, double H, DX_ID PairID, const BMatr &Shift, bool FirstCadr, bool LastCadr, const std::vector<BStockGrID>& Groups, const CadrData& MaskData)
{
	CadrData Data;
	CadrData *pData = &Data;
	if (MLTI_THREAD_DX)
	{
		while(SlideBufferCadr.GetRef(pData) != BSB_OK)
			Sleep(SleepTime);
	}
	pData->MakeEmpty();
	pData->SetStockPos(Groups);
	DXSubModel::InsSpecCadr(DXType, P0v, P1v
			, Nv, R, H
			, PairID, Shift
			, FirstCadr
			, LastCadr // Should be defined
			, *pData
			, MaskData);
	if (pData->GetDXType() != TT_NO)
	{
		if (MLTI_THREAD_DX)
			SlideBufferCadr.StoreRef();
		else
		{
			if (pBody)
			{
				((DXSubModel *)pBody)->RastSpecCadr(0, *pData);
				((DXSubModel *)pBody)->RastSpecCadr(1, *pData);
				((DXSubModel *)pBody)->RastSpecCadr(2, *pData);
			}
		}
	}
	return 0;
}

int FProg2BSP::InsSpecCadrLast()
{
	if (MLTI_THREAD_DX)
	{
		CadrData *pData = nullptr;
		while (SlideBufferCadr.GetRef(pData) != BSB_OK)
			Sleep(SleepTime);

		pData->MakeLast(SlideBufferCadr.GetLastDXType());

		SlideBufferCadr.StoreRef();
	}
	return 0;
}

void FProg2BSP::StartCutting1DirThreads()
{
	if (MLTI_THREAD_DX)
	{
		WaitAllCuttings();
		SlideBufferCadr.Clear();
		pBody = nullptr;
		static int Dir0 = 0;
		Cutting1DirThreads.push_back(create_task([&] { return CuttingThreadProc1Dir(&Dir0); }));
		static int Dir1 = 1;
		Cutting1DirThreads.push_back(create_task([&] { return CuttingThreadProc1Dir(&Dir1); }));
		static int Dir2 = 2;
		Cutting1DirThreads.push_back(create_task([&] { return CuttingThreadProc1Dir(&Dir2); }));
	}
}

void FProg2BSP::StopCutting1DirThreads()
{
	if (MLTI_THREAD_DX)
	{
		Wait4Empty1DirThreads();
		SlideBufferCadr.SetFinished();
		SlideBufferCadr.SetResumedEvent();
		WaitAllCuttings();
	}
}

bool FProg2BSP::Wait4Empty1DirThreads()
{
	if (MLTI_THREAD_DX)
	{
		SlideBufferCadr.PrepareEvents();
		InsSpecCadrLast();
		return SlideBufferCadr.Wait4Empty();
	}
	return true;
}

void FProg2BSP::RecalcPts(NCadrGeom *pGeom, BPointsBuf &Pts, const BPoint &Base)
{
	if(pGeom->IsRotate())
	{
		BPoint XYZ0(0., 0., 0., 1.), ABC0(0., 0., 0., 1.);
		pGeom->CalcCurCoordMCS(0., MCS, XYZ0, ABC0);
		BPoint XYZ1(0., 0., 0., 1.), ABC1(0., 0., 0., 1.);
		pGeom->CalcCurCoordMCS(1., MCS, XYZ1, ABC1);
		BStockToolInds STInds(0, 0, 0);
		BCurve *pRecalcCurve = pMUPair->GetMach().Make5xLine(pGeom->GetRCPMask(), pMUPair, XYZ0, ABC0, XYZ1, ABC1, Base, STInds);
		BCurve *pCurveB = pGeom->GetCurve();
		pGeom->SetCurve(pRecalcCurve);
		pGeom->GetPolyLine(Pts, NSurfSettings::GetTolerance(), false);
		BCurve::Storage.ClearLastCurve();
		pGeom->SetCurve(pCurveB);
	}
	else
		pGeom->GetPolyLine(Pts, NSurfSettings::GetTolerance(), false);

}

void FProg2BSP::PostCollisions(CWnd* pMainWnd)
{
	if (pMainWnd == nullptr)
		return;
	if (CollPositions.empty())
		return;
	for each (const BRangePosColl & CollPos in CollPositions)
	{
		ProgTPoint CadrIDStart = CollPos.GetStartTPoint();
		const cadrID CadrID = CollPos.FindCurHistCadr(CollPos.CadrNum);
		NCollLogElem* pCollLogElem = new NCollLogElem(CadrIDStart, ProgTPoint(CadrID, 0), CollPos.GetCollType());// we don't know time of collision
		CString Str;
		if (CollPos.GetCollType() == LE_COLLTOOL)
			Str.LoadString(IDS_CUTTER_SETCOLLNOTE);
		pCollLogElem->SetCollNote(std::string(Str));
		pMainWnd->PostMessage(WM_APP_COLLCHECK_ADDLOG, 3, (LPARAM)pCollLogElem);
	}
	CollPositions.clear();
}

void FProg2BSP::SetMCS(const BMatr& inMCS)
{
	MCS = inMCS;
}

CadrData FProg2BSP::GetSpecMask(const NCadrGeom* pGeom) const
{
	CadrData MaskCadrData;
	MaskCadrData.MakeEmtyMask();
	return MaskCadrData;
}
