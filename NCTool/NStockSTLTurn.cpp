#include "stdafx.h"
#include "BMatrPair.h"
#include "NStockSTLTurn.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(NStockSTLTurn, NStockStl, 0)

NStockSTLTurn::NStockSTLTurn(void)
{
}

NStockSTLTurn::~NStockSTLTurn(void)
{
}

bool NStockSTLTurn::CreateBody(void)
{
	BMatrPair Matr = BMatr().RotX(BPoint(0., 0., 0., 1.), -90.);
	Recalc();
	Stl.Transform(Matr.Or());
	Gabar.Transform(Matr.Or());
	Stl.Transform(Matr.Inv());
	Gabar.Transform(Matr.Inv());

	std::list<BClrContour> Contours;
	if (!Stl.MakeSpinContours(BMatr(), Contours))
		return false;
	if (Contours.empty())
		return false;
	Gabar.Init();
	for each (const auto & Cont in Contours)
	{
		BBox G;
		for each (const auto & P in Cont)
			G.Expand(P);
		Gabar.Expand(-G.Xmax, -G.Xmax, G.Ymin);
		Gabar.Expand(G.Xmax, G.Xmax, G.Ymax);
	}
	SetStockType(AT_TURN);// should be before MakeRotMulti for right edges drowing

	MakeRotMulti(Contours);

	MillStockChanged = false;
	TurnStockChanged = false;
	return true;
}

NStock::StockCrType NStockSTLTurn::GetType() const
{
	return SCT_STLT;
}
