#include "stdafx.h"
#include "BSpacePos.h"
#include "NToolCombined.h"
#include "SMachStateFix.h"
#include "SCoordsDef.h"
#include "BProgram.h"
#include "NCUnit.h"
#include "NMachUnitPair.h"
#include "gdynamiccadr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//GDynamicCadr::GDynamicCadr(void)
//{
//}
GDynamicCadr::GDynamicCadr(GBodyRender *piStart)
{
	pStart = piStart;
	pTimeLine = nullptr;
	pEnd = NULL;
	pLastTool = NULL;
	pProg = NULL;
	pCoordsDef = NULL;

	CadrID = cadrID(-1);
	CadrIDhist = cadrID(-1);
	pCoordsDef = NULL;
	XYZ.Set(0., 0., 0., 1.);
	ABC.Set(0., 0., 0., 1.);
}

GDynamicCadr::~GDynamicCadr(void)
{
	delete pStart;
}

void GDynamicCadr::SetMT(class MTMachineTool* pMT, const class NCUnit* pU, NCollTimeLine* pTL)
{
	MUPair.Set(*pMT, *pU);
	pCoordsDef = &pU->GetAddCoordsDef();
	pTimeLine = pTL;
}

const NToolCombined * GDynamicCadr::GetLastTool(void) const
{
	return pLastTool;
}

void GDynamicCadr::SetMT2CurPos()
{
	if(!pProg)
		return;
	if(Geom.IsUndef())
		return;
	ABC.Set(0., 0., 0., 1.);
	CalcCurCoordMCS(XYZ, ABC);
	if(pCoordsDef)
		if(pCoordsDef->GetSize() > 0)
		{
			SMachState AddCoord(pCoordsDef);
			CalcCurAddCoord(AddCoord, 0.);
			MUPair.GetMach().SetStatePartial(Geom.GetMTLink(), SMachState(XYZ, ABC, &MUPair.GetUnit().GetFixedDef()), AddCoord);
			return;
		}
	MUPair.GetMach().SetStatePartial(Geom.GetMTLink(), SMachState(XYZ, ABC, &MUPair.GetUnit().GetFixedDef()));
}

void GDynamicCadr::SetGeom(const NCadrGeom & InGeom)
{
	Geom = InGeom;
	MCS = MUPair.GetMach().GetMCS(&Geom, pLastTool, MUPair.GetUnit().GetFixedDef());
}

bool GDynamicCadr::SetA(const NAnimState &AState)
{
	Init();
	CadrID = AState.GetCadrID();
	CadrIDhist = AState.GetCadrIDhist();
	pLastTool = AState.GetTool();
	BMatr StockMoveMatrInv;
	if (MUPair.GetMach().HaveStockMoved())
	{
		SetGeom(AState.CadrGeom);
		SetMT2CurPos();
		int StockPositon = Geom.GetStockPos();
		StockMoveMatrInv = MUPair.GetMach().GetStockMatr(StockPositon, true)
			* MUPair.GetMach().GetStockMovedMatr(StockPositon, true).invr();
	}
	return Set(AState.CadrGeom, BSpacePos::GetRotMatr(AState.CadrGeom.GetMatrNum()) * StockMoveMatrInv, AState.GetRender(), AState.tPart);
}
cadrID GDynamicCadr::GetCadrID() const
{
	return CadrID;
}

cadrID GDynamicCadr::GetCadrIDhist() const
{
	return CadrIDhist;
}

GBodyRender * GDynamicCadr::GetActiveRender() const
{
	if(pStart)
		return pStart;
	else
		return pEnd;
}

GBodyRender* GDynamicCadr::GetEnd(void) const
{
	return pEnd;
}

GBodyRender* GDynamicCadr::GetStart(void) const
{
	return pStart;
}


bool GDynamicCadr::FindGeom()
{
	if(!pProg)
		return false;

	SetGeom(*pProg->GetCompGeomArray().GetAt(pProg->GetHistByHCadr(CadrIDhist.Cadr)));

	return true;
}

int GDynamicCadr::GetProgNum() const
{
	if(pProg == nullptr)
		return -1;
	if(pProg->GetHistorySize() <= 0)
		return -1;
	return pProg->GetHistory()[0].Prog;
}