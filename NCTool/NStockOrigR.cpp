#include "stdafx.h"
#include "BaseRender.h"
#include "GQuadRenderDX5X.h"
#include "MBSPNode.h"
#include "GQuadRenderRot.h"
#include "DXSubModel5X.h"
#include "NStockOrigR.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(NStockOrigR, NStock, 0)


NStockOrigR::NStockOrigR()
{
	pOrigRender = nullptr;
	pOrigRenderRot = nullptr;
	IsCreated = false;
}


NStockOrigR::~NStockOrigR()
{
	delete pOrigRender;
	delete pOrigRenderRot;
}

const BPoint NStockOrigR::RayCastingOr(const BPoint & P, const BPoint & V, double MinW, BPoint &N) const
{
	if (!pOrigRender)
		return BPoint(0., 0., 0., -1.);
	return pOrigRender->RayCasting(P, V, MinW, N);
}

void NStockOrigR::DrawOriginal(enum RenderMode GlobRMode, enum RenderMode ExtRMode, bool DrawSurf)
{
	if (!NeedDraw(GlobRMode, ExtRMode))
		return;

	NStock::DrawOriginal(GlobRMode, ExtRMode, DrawSurf);
	bool DrawEdges = GetEdgesS() && (NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::BSP)
			|| GetEdgesDX() && (NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::DX);
	DrawEdges &= !WFrame;
	auto pBuf = pRender;
	auto pBufRot = pRenderRot;
	pRender = pOrigRender;
	pRenderRot = pOrigRenderRot;
	if(pRender)
		pRender->SetUseExternColor(true);
	if(pRenderRot)
		pRenderRot->SetUseExternColor(true);
	auto BufCol = GQuadRender::GetEdgesColor();
	GQuadRender::SetEdgesColor(EdgesAttrs.GetEdgesColor());
	auto BufWidth = GQuadRender::GetEdgesWidth();
	GQuadRender::SetEdgesWidth(EdgesAttrs.GetEdgesWidth());

	Render(GlobRMode, DrawEdges ? 4 : 0, DrawSurf);

	GQuadRender::SetEdgesColor(BufCol);
	GQuadRender::SetEdgesWidth(BufWidth);
	if (pRender)
		pRender->SetUseExternColor(false);
	if (pRenderRot)
		pRenderRot->SetUseExternColor(false);
	pOrigRender = pRender;
	pOrigRenderRot = pRenderRot;
	pRender = pBuf;
	pRenderRot = pBufRot;
	NStock::RestDrawOriginal();
}

CString NStockOrigR::GetFileName() const
{
	return FileName;
}

bool NStockOrigR::CreateBodyFrBSP(MQuadTree * pTree)
{
	switch (NCM_PROJECT.GetGlobalState().GetModelMode())
	{
	case NGlobalState::DX:
	{
		int DXNum = int(BDebugState::Get().DXNumber + 0.5);
		BBox Box = Gabar;
		Box.Offset(+0.001, +0.001, +0.001);
		DXModel B;
		B.Set(Box, int(GQuadRenderDX5X::GetWholeCellsNum() * WholeCellsKoef), DXNum, DXNum, DXNum);
		if (BSPTNumDX < 0)
		{
			DXWorld = B;
			BSPTNumDX = MFOREST.AddTree(new DXSubModel5X(DXWorld, GetStockGr()));
		}
		else
		{
			MBody *pBody = MFOREST.GetTree(BSPTNumDX);
			if (pBody->GetModelMode() != NGlobalState::DX)
			{
				MFOREST.DeleteTree(BSPTNumDX);
				BSPTNumDX = -1;
				DXWorld = B;
				BSPTNumDX = MFOREST.AddTree(new DXSubModel5X(DXWorld, GetStockGr()));
			}
			else
			{
				if (DXWorld != B)
				{
					DXWorld = B;
					((DXSubModel5X *)pBody)->ReCreate();
				}
				else
					((DXSubModel5X *)pBody)->MakeEmpty();
			}
		}
		DXSubModel5X *pBodyDX = (DXSubModel5X *)MFOREST.GetTree(BSPTNumDX);
		pBodyDX->SetBodyRot(pTree->GetBodyRot().Or());
		pBodyDX->SetBodyRotActive(pTree->IsBodyRotActive());

		pBodyDX->MakeFromBSP(*pTree);

	}
	break;
	case NGlobalState::BSP:
	{
		if (BSPTNum < 0)
			BSPTNum = MFOREST.AddTree(pTree);
		else
		{
			MFOREST.DeleteTree(BSPTNum);
			MFOREST.SetTree(BSPTNum, pTree);
		}
	}
	break;
	}

	delete pOrigRender;
	CreateRenderMill();
	pOrigRender = (GQuadRender *)pRender;
	pRender = NULL;
	CreateRenderMill();
	return true;
}

void NStockOrigR::CreateCopyTo(class NStock* &stock) const
{
	if (!stock)
		stock = new NStockOrigR();
	NStock::CreateCopyTo(stock);

	NStockOrigR *pOrigR = (NStockOrigR *)stock;
	pOrigR->FileName = this->FileName;
}

void NStockOrigR::SetColor(const NColor & Col)
{
	NStock::SetColor(Col);
	DeleteOrigRenders();
}

void NStockOrigR::DeleteOrigRenders()
{
	delete pOrigRender;
	pOrigRender = nullptr;
	delete pOrigRenderRot;
	pOrigRenderRot = nullptr;
}

