#include "stdafx.h"
#include "NCMDoc.h"
#include "MTMashineTool.h"
#include "NOMScaner.h"
#include "MTPart.h"
#include "GQuadRenderMill.h"
#include "BObjectManager.h"
#include "GDynamicCadr.h"
#include "MTConfig.h"
#include "NCollManagerTouch.h"

NCollManagerTouch::NCollManagerTouch(CNCMDoc* pD, const list<int>& MovingAx, GBodyRender* pRender) : MovingAxInds(MovingAx), NCollManager(pD)
{
	pBodyRender = pRender;
}

NCollManagerTouch::~NCollManagerTouch()
{
}

bool NCollManagerTouch::MakeSensitivePairs()
{
	// Make sensitive pairs
	// Moving axes against unmoving axes
	list<int> UnmovingAxInds;
	list<int> Tmp;
	if (MovingAxInds.empty())
		return false;
	for (int k = 0; k < pMainMach->GetCurConfig().GetSize(); ++k)
	{
		auto pCurAx = pMainMach->GetCurConfig()[k];
		auto Type = pCurAx->GetType();
		if (Type != MTAxis::AT_PART && Type != MTAxis::AT_TOOL && Type != MTAxis::AT_VTOOL && Type != MTAxis::AT_STOCK)
			continue;
		bool Moving = false;
		for each (auto MovingAxInd in MovingAxInds)
		{
			auto pMovingAx = pMainMach->GetCurConfig()[MovingAxInd];
			if (pMainMach->GetCurConfig().IsDescendant(*pMovingAx, *pCurAx) || pMovingAx == pCurAx)
			{
				Tmp.push_back(k);
				Moving = true;
				break;
			}
		}
		if (!Moving)
		{
			UnmovingAxInds.push_back(k);
		}
	}
	if (UnmovingAxInds.empty())
		return false;
	for each (auto MovingAxInd in Tmp)
		for each (auto UnmovingAxInd in UnmovingAxInds)
		{
			pCollisionScene->AddSensitivePair(CalcBehav(MovingAxInd, false, false), CalcBehav(UnmovingAxInd, false, false));
		}
	return true;
}

int NCollManagerTouch::MakeCollisionScene(void)
{
	auto res = NCollManager::MakeCollisionScene();

	MakeStocks();

	MakeSensitivePairs();
	return res;
}

bool NCollManagerTouch::CheckLocCurPosition(class NMachUnitPair* pMUPair, const NToolCombined* pActiveTool)
{
	if (!pCollisionScene)
		return false;

	ChangeTool(pCollisionScene, *pMainMach, pActiveTool);
	if (!SetCurMatrices(pCollisionScene, &pMUPair->GetMach(), pActiveTool))
		return false;

	return (pCollisionScene->RunDetection() > 0);
}

void NCollManagerTouch::Prepare(bool Fore)
{
	IgnoreMargins = true;

	if (pCollisionScene)
		return;

	if (!pMainMach)
		return;

	C_OBJECT_MANAGER((*this))->AddActiveObject(this, 3, GroupMask | OG_STOCKS | OG_TOOLS | OG_MACHINE); // Changing of a stock, tool or machine must lead to restart

	pCollisionScene = NCollisionScene::GenCollisionScene(nullptr, nullptr);

	MakeCollisionScene();
}

PObjectStatus NCollManagerTouch::OnStatusChanged(int ObjectInd, PObjectStatus NewStatus)
{
	if (NewStatus == DELETED)
		return ST_EMPTY;
	return USED;
}
