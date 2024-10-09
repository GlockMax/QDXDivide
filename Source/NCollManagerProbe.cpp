#include "StdAfx.h"
#include "NCMDoc.h"
#include "MTMashineTool.h"
#include "NMachUnitPair.h"
#include "NCUnit.h"
#include "NOMScaner.h"
#include "MTPart.h"
#include "GQuadRenderMill.h"
#include "GQuadRenderDX5X.h"
#include "BObjectManager.h"
#include "MTConfig.h"
#include "NCollManagerProbe.h"

NCollManagerProbe::NCollManagerProbe(CNCMDoc* pD, GBodyRender* pRender, NToolCombined* pActiveTool) : NCollManager(pD)
{
	pBodyRender = pRender;
	pActTool = pActiveTool;
}

NCollManagerProbe::~NCollManagerProbe()
{
}

int NCollManagerProbe::MakeCollisionScene(void)
{
	const int res = NCollManager::MakeCollisionScene();
	if (!Add2CollisionScene())
		return -1;
	return res;
}

bool NCollManagerProbe::CheckLocCurPosition(class NMachUnitPair* pMUPair, const NToolCombined * pActiveTool)
{
	if (!pCollisionScene)
		return false;
	pMUPair->GetMach().MakeActual();
	MakeStocksProbe(pActiveTool, pBodyRender);
	ChangeTool(pCollisionScene, *pMainMach, pActiveTool);
	if (!SetCurMatrices(pCollisionScene, &pMUPair->GetMach(), pActiveTool))
		return false;


	return (pCollisionScene->RunDetection() > 0);
}

int NCollManagerProbe::MakeStocksProbe(const class NToolCombined *pActiveTool, GBodyRender* pBodyRender)
{
	if (!(GroupMask & OG_STOCKS))
		return 0;
	if (pActiveTool == nullptr)
		return -1;
	BBox ProbeToolGabarOr = pActiveTool->GetProbeGabar();
	if (!ProbeToolGabarOr.IsDefined())
		return -1;

	for (GBodyRender* pRender = pBodyRender; pRender != nullptr; pRender = pRender->GetNextRender())
	{
		const int AxInd = pMainMach->GetStockMovedAxisIndSGr(pRender->GetStockPos());
		if (AxInd < 0)
			continue;// internal error
		BMatr Stock2ToolMatr = pMainMach->GetStockMoved2ToolMatrCSGr(pRender->GetStockPos(), pActiveTool->Turret, pActiveTool->Attach);
		BBox ProbeToolGabar = ProbeToolGabarOr;
		ProbeToolGabar.Transform(Stock2ToolMatr);
		NCollisionObjectID ID;
		ID.SetGroup(OG_STOCKS);
		ID.SetID(AxInd);
		ID.SetBehav(CalcBehav(ID.GetID(), true, false));
		ID.SetCollidableObj(nullptr);
		if (pRender == nullptr)
			continue;
		MQuadBody::Window Win = pRender->CalcWindow(ProbeToolGabar);
		if (!Win.IsDefined())
			continue;
		GQuadRenderMill* pRenderMill = dynamic_cast<GQuadRenderMill*>(pRender);
		if (pRenderMill != nullptr)
		{
			auto Array = pRenderMill->GetArray();
			for (size_t ix = Win.ixStart; ix < Win.ixEnd; ++ix)
				for (size_t iy = Win.iyStart; iy < Win.iyEnd; ++iy)
				{
					float* pC = Array[ix][iy];
					if (pRenderMill->Empty(pC))
						continue;
					if (UsedStockCells.find(pC) == UsedStockCells.end())
					{
						UsedStockCells.insert(pC);
						pCollisionScene->AddStlObject(ID, pC, sizeof(float) * 7, -1, 0.f, false, false);
					}
				}
		}
		else
		{
			GQuadRenderDX5X* pRenderDX = dynamic_cast<GQuadRenderDX5X*>(pRender);
			if (pRenderDX != nullptr)
			{
				for (int ix = Win.ixStart; ix < Win.ixEnd; ++ix)
					for (int iy = Win.iyStart; iy < Win.iyEnd; ++iy)
						for (int iz = Win.izStart; iz < Win.izEnd; ++iz)
						{
							RTriRandMesh* pMesh = pRenderDX->GetBigCellMesh(ix, iy, iz);
							if (!pMesh)
								continue;
							size_t Size;
							float* pC = pMesh->Open4Read(Size);
							if (UsedStockCells.find(pC) == UsedStockCells.end())
							{
								UsedStockCells.insert(pC);
								pCollisionScene->AddStlObject(ID, pC - 1, pMesh->GetStride(), int(Size / 3), 0.f, false, false);
							}
						}
			}
		}
		int ActToolAxisIndex = pMainMach->GetCurConfig().GetAxisIndex(MTConfig::GetToolAxisName(pActiveTool->Turret, pActiveTool->Attach));
		if (ActToolAxisIndex < 0)
			return -1;

		pCollisionScene->Add1SensitivePair(CalcBehav(AxInd, true, false), CalcBehav(ActToolAxisIndex, false, false));
	}
	return 0;
}

void NCollManagerProbe::Prepare(bool Fore)
{
	IgnoreMargins = true;

	if (pCollisionScene)
		return;

	if (!pMainMach)
		return;

	C_OBJECT_MANAGER((*this))->AddActiveObject(this, 2, GroupMask | OG_STOCKS | OG_TOOLS | OG_MACHINE); // Changing of a stock, tool or machine must lead to restart

	pCollisionScene = NCollisionScene::GenCollisionScene(nullptr, nullptr);

	MakeCollisionScene();
}

PObjectStatus NCollManagerProbe::OnStatusChanged(int ObjectInd, PObjectStatus NewStatus)
{
	if (NewStatus == DELETED)
		return ST_EMPTY;
	return USED;
}

bool NCollManagerProbe::Add2CollisionScene()
{
	// Add active tool
	if (pActTool == nullptr)
		return false;
	int ActToolAxisIndex = pMainMach->GetCurConfig().GetAxisIndex(MTConfig::GetToolAxisName(pActTool->Turret, pActTool->Attach));
	if (ActToolAxisIndex < 0)
		return false;
	MakeTool(ActToolAxisIndex, pActTool, true, true);

	// Make sensitive pairs
	// Active tool should interact with all machine tool axes unless axes moving synchronously with this tool
	const MTAxis* pMovRotAxis = pMainMach->GetNearestMoveRotAxis(pMainMach->GetCurConfig()[ActToolAxisIndex]);

	for (NOMScaner Sc(GetObjectManager(), this); !Sc.IsEnded(); Sc.Next())
	{
		int AxInd = -1;
		if (Sc.GetPassiveGroup() == OG_MACHINE)
		{
			const MTPart* pPart = (MTPart*)Sc.GetPassiveObject();
			AxInd = pMainMach->GetCurConfig().GetAxisIndex(pPart->GetAxisName());
		}
		else if (Sc.GetPassiveGroup() == OG_FIXTURES)
		{
			AxInd = pMainMach->GetStockMovedAxisInd(0);
		}
		if (AxInd < 0)
			continue;// internal error
		const MTAxis* pCurAxis = pMainMach->GetNearestMoveRotAxis(pMainMach->GetCurConfig()[AxInd]);
		if (pCurAxis == pMovRotAxis) // common nearest moving axis
			continue;
		pCollisionScene->Add1SensitivePair(CalcBehav(AxInd, false, false), CalcBehav(ActToolAxisIndex, false, false));
	}
	return true;
}
