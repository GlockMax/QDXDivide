#include "stdafx.h"
#include "ppl.h"
#include <vector>
#include "ConstDef.h"
#include "NColor.h"
#include "MQuadBody.h"
#include "MTMashineTool.h"
#include "RRopes.h"
#include "GBodyRender.h"

//CCriticalSection GBodyRender::CritSection;
bool GBodyRender::SmoothRender = true;
NColor GBodyRender::EdgesColor(0., 0., 0., 1.);
double GBodyRender::EdgesWidth = 2.;
double GBodyRender::MinSin2 = (PI / 180.) * (PI / 180.);
int GBodyRender::DichotDegree = 5;
bool GBodyRender::FindVergPoint = false;

GBodyRender::GBodyRender(const GBodyRender &inRender)
{
	pNextRender = NULL;
	pPreviousRender = NULL;
	Gabar = inRender.Gabar;
	UseExternColor = inRender.UseExternColor;
	StockPos = inRender.StockPos;
	pRopes = new RRopes;
}

GBodyRender::GBodyRender(const MBody &Tree)
{
	pNextRender = NULL;
	pPreviousRender = NULL;
	Gabar.Expand(Tree.MaxPoint);
	Gabar.Expand(Tree.MinPoint);
	BodyRot = Tree.GetBodyRot();
	BodyRotActive = Tree.IsBodyRotActive();
	UseExternColor = false;
	StockPos = Tree.StockGr;
	pRopes = new RRopes;
}

GBodyRender::~GBodyRender(void)
{
	if (pNextRender)
		delete pNextRender;
	pNextRender = NULL;
	pPreviousRender = NULL;
	delete pRopes;
	pRopes = NULL;
}

GBodyRender* GBodyRender::GetNextRender() const
{
	return pNextRender;
}

GBodyRender* GBodyRender::GetPreviousRender() const
{
	return pPreviousRender;
}

void GBodyRender::SetNextRender(GBodyRender *next)
{
	if (next == this)
		return;
	pNextRender = next;
}

void GBodyRender::SetPreviousRender(GBodyRender *previous)
{
	if (previous == this)
		return;
	pPreviousRender = previous;
}

void GBodyRender::AddRenderInList(GBodyRender *pGBodyRender)
{
	if (pGBodyRender == this)
		return;
	if (pNextRender)
		pNextRender->AddRenderInList(pGBodyRender);
	else
	{
		pNextRender = pGBodyRender;
		pGBodyRender->pPreviousRender = this;
	}
}

int GBodyRender::FillModifArrays(MQuadBody & Tree)
{
	MQuadBody *pNextBody = (MQuadBody *)Tree.GetNextMBody();
	int result = 0;
	if (pNextRender && pNextBody)
		result = pNextRender->FillModifArrays(*pNextBody);
	/*if (result == -1)
	return result;*/

	return this->FillModifArraysCh(Tree);
}

int GBodyRender::RenderWMask(const GBodyRender & MaskRender, int Type, MTMachineTool &MT, bool complement)
{
	int result = 0;
	const GBodyRender* pMaskRender = &MaskRender;
	if (pMaskRender == nullptr)
		return 0;
	for (GBodyRender* pRender = this; pRender != nullptr; pRender = pRender->GetNextRender())
	{
		if (pMaskRender == nullptr)
			break;
		const BStockGrID StPos = pRender->GetStockPos();
		glPushMatrix();
		BMatr M = MT.GetStockMovedMatrSGr(StPos, true) * MT.GetStockMovedMatr(0, true).invr();
		glMultMatrixd(M.GetArray());
		result = pRender->RenderWMaskCh(*pMaskRender, Type, complement);
		glPopMatrix();
	}
	return result;
}

void GBodyRender::TransModifArrays(GBodyRender & inRender)
{
	// "this" is mixed render list
	// inRender is homogeneous render list (turn or mill)
	GBodyRender* pinNext = &inRender;
	for (auto pThisNext = this; pThisNext != nullptr && pinNext != nullptr; pThisNext = pThisNext->GetNextRender())
	{
		if (pThisNext->GetType() != pinNext->GetType())
			continue;
		pThisNext->TransModifArraysCh(*pinNext);
		pinNext = pinNext->GetNextRender();
	}
}

int GBodyRender::Render(enum RenderMode GlobRMode, int Type, bool DrawSurf, MTMachineTool& MT)
{
	int result = 0;
	for (GBodyRender* pRender = this; pRender != nullptr; pRender = pRender->GetNextRender())
	{
		const BStockGrID StPos = pRender->GetStockPos();
		glPushMatrix();
		BMatr M = MT.GetStockMovedMatrSGr(StPos, true) * MT.GetStockMovedMatr(0, true).invr();
		glMultMatrixd(M.GetArray());

		result = pRender->RenderCh(GlobRMode, Type, DrawSurf);

		glPopMatrix();
	}
	return result;
}

int GBodyRender::FillAllArrays(MQuadBody & Body)
{
	std::vector<GBodyRender *> Renders;
	std::vector<MQuadBody *> Bodies;

	GBodyRender *pCurRend = this;
	for (MQuadBody *pCur = &Body; pCur != nullptr; pCur = static_cast<MQuadBody *>(pCur->pNextMBody), pCurRend = static_cast<GBodyRender *>(pCurRend->pNextRender))
	{
		Bodies.push_back(pCur);
		Renders.push_back(pCurRend);
	}

	for (size_t i = 0; i < Bodies.size(); ++i)
		Renders[i]->FillAllArraysCh(*Bodies[i]);
	return 0;
}

void GBodyRender::SetSmooth(bool in){ SmoothRender = in; }
bool GBodyRender::GetSmooth(){ return SmoothRender; }
int GBodyRender::GetDichotDegree() { return DichotDegree; }
void GBodyRender::SetDichotDegree(int Val) { DichotDegree = Val; }
bool GBodyRender::GetFindVergPoint() { return FindVergPoint; }
void GBodyRender::SetFindVergPoint(bool Val) { FindVergPoint = Val; }
void GBodyRender::SetEdgesColor(const NColor & Col) { EdgesColor = Col;}
const NColor &GBodyRender::GetEdgesColor(void) { return EdgesColor; }
double GBodyRender::GetEdgesWidth(void) { return EdgesWidth; }
void GBodyRender::SetEdgesWidth(double Width) { EdgesWidth = Width; }
double GBodyRender::GetMinSin2() { return MinSin2; }
void GBodyRender::SetMinSin2(double Val) { MinSin2 = Val; }

