#include "stdafx.h"
#include "BaseRender.h"
#include "BDebugState.h"
#include "ConstDef.h"
#include "MQuadTree.h"
#include "NMeshArray.h"
#include "NMakeStdMesh.h"
#include "NRectMesh.h"
#include "NParamMesh.h"
#include "MBSPInter.h"
#include "SOPlane.h"
#include "GQuadRenderRot.h"
#include "MElemIDProc.h"
#include "math.h"
#include "TBladeCont.h"
#include "MQuadTreeSliced.h"
#include "NStockSTLMillTurn.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(NStockSTLMillTurn, NStockStl, 0)

NStockSTLMillTurn::NStockSTLMillTurn(void) 
	: BaseCenter(0., 0., 0., 1.), Invert(false)
{
	IsCreated = false;
	pOrigRender = nullptr;
	Name.Empty();
}

void NStockSTLMillTurn::CreateCopyTo(class NStock* &stock) const
{
	if(!stock)
		stock = new NStockSTLMillTurn();
	NStockStl::CreateCopyTo(stock);

	NStockSTLMillTurn *pMillTurn = (NStockSTLMillTurn*)stock;
	pMillTurn->BaseCenter = this->BaseCenter;
	pMillTurn->Invert = this->Invert;
	// GQuadRenderRot *pOrigRender;
	pMillTurn->Name = this->Name;
}

NStockSTLMillTurn::~NStockSTLMillTurn(void)
{
	delete pOrigRender;
}

void NStockSTLMillTurn::Serialize(CArchive &ar)
{
	NStockStl::Serialize(ar);
	if(ar.IsLoading())
	{
		Create();
//		Reset(); // will be called in CStocksCollection::Serialize
	}
}

void NStockSTLMillTurn::Create()
{
	NStockStl::Create();
}

NStock::StockCrType NStockSTLMillTurn::GetType() const
{
	return SCT_STLMT;
}

bool NStockSTLMillTurn::CreateBody(void)
{
	BMatrPair Matr = BMatr().RotX(BPoint(0., 0., 0., 1.), -90.);
	Recalc();
	Stl.Transform(Matr.Or());
	Gabar.Transform(Matr.Or());
	NStockStl::CreateBody();
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
	MakeRotMulti(Contours);

	SetStockType(AT_TURN);

	if(NCMComData::IsMillEnabled())
	{
		SetStockType(AT_MILL_TURN);
	}
	else
	{
		RemoveMillBody();
	}
	MillStockChanged = false;
	TurnStockChanged = false;
	return true;
}

bool NStockSTLMillTurn::CreateRenderRot(void)
{
	NStock::CreateRenderRot();

	delete pOrigRender;
	pOrigRender = new GQuadRenderRot(*pRenderRot);

	return pOrigRender != NULL;
}

void NStockSTLMillTurn::SetColor(const NColor & Col)
{
	if (ColorNum != STOCK_COLOR_NUM || !IsRealStock())
		MElemIDProc::ReplacePC(Color, Col);
	NStock::SetColor(Col);
	CreateBody();
}

void NStockSTLMillTurn::Recalc()
{
	if(IsCreated)
	{
		Gabar = Stl.GetGabar();
		//Gabar.Init();
		//BBox G = Stl.GetGabar();
		//double Xmax = Stl.FindMaxZDist();
		//Gabar.Expand(- Xmax - ST_DEC,- Xmax - ST_DEC, G.Zmin - ST_DEC);
		//Gabar.Expand(  Xmax + ST_INC,  Xmax + ST_INC, G.Zmax + ST_INC);
	}
}