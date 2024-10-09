#include "stdafx.h"
#include "GBodyRender.h"
#include "nelemidproc.h"

const NColor FastColor(1., 0., 0., 1.);

NElemIDProc::NElemIDProc(void)
{
	SetHighlightedSurfID(-2);
}

NElemIDProc::~NElemIDProc(void)
{
}

bool NElemIDProc::GetStockEdgesS(void)
{
	return pDoc->NGetStockList()->IsSharpEdges();
}

bool NElemIDProc::GetStockEdgesDX(void)
{
	return pDoc->NGetStockList()->IsSharpEdgesDX();
}

PackedColor NElemIDProc::GetColor(ElemID id)
{
	int PCNum = GetPCn(id);
	PackedColor col;
	col.ival = 0;
	if(PCNum < RESERVED_PCS)
	{
		col.ival = 0;
		if(PCNum == STOCK_COLOR_NUM)
			col = pDoc->GetStocksColor();
		else if(PCNum == FAST_COLOR_NUM)
			col = MakePC(FastColor);
	}
	else 
	{
		col = GetPC(PCNum - RESERVED_PCS);
	}
	if (col.ival == 0)
		col = MElemIDProc::GetColor(id);
	unsigned int Alpha = unsigned int(pDoc->GetStocksList().GetTransparency() * 255);
	col.fa = Alpha;
	if(GetSurfID(id) == GetHighlightedSurfID())
	{
		col.fr = 255 - col.fr;
		col.fg = 255 - col.fg;
		col.fb = 255 - col.fb;
	}

	return col;
}

NElemIDProc::NElemIDProc(CNCMDoc *pDocum)
{
	pDoc = pDocum;
}

