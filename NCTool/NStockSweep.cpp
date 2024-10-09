#include "stdafx.h"
#include "MBSPNode.h"
#include "gl\glu.h"
#include "BDebugState.h"
#include "ConstDef.h"
#include "MQuadTree.h"
#include "NMeshArray.h"
#include "NMakeStdMesh.h"
#include "NRectMesh.h"
#include "NParamMesh.h"
#include "MBSPInter.h"
#include "SOPlane.h"
#include "GQuadRenderMill.h"
#include "MElemIDProc.h"
#include "math.h"
#include "TBladeCont.h"
#include "MQuadTreeSliced.h"
#include "GQuadRenderDX5X.h"
#include "NStockSweep.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(NStockSweep, NStockOrigR, 0)

NStockSweep::NStockSweep(void) 
	: BaseCenter(0., 0., 0., 1.)
{
}

void NStockSweep::CreateCopyTo(class NStock* &stock) const
{
	if(!stock)
		stock = new NStockSweep();
	NStockOrigR::CreateCopyTo(stock);

	NStockSweep *pSweep = (NStockSweep*)stock;
	pSweep->BaseCenter = this->BaseCenter;
	pSweep->Vector = this->Vector;
	pSweep->ABC = this->ABC;
	pSweep->Cont.MakeCopy(this->Cont);
}

NStockSweep::~NStockSweep(void)
{
}

void NStockSweep::Serialize(CArchive &ar)
{
	NStock::Serialize(ar);
	SerializeElements(ar, &BaseCenter, 1);
	SerializeElements(ar, &Vector, 1);
	SerializeElements(ar, &ABC, 1); 
	Cont.Serialize(ar);
	if(ar.IsLoading())
		Create();
}

void NStockSweep::Create()
{
	NStock::Create();
	Gabar.Init();
	BBox G = Cont.GetGabar();
	G = G.Move(- 0.5 * Vector.GetX(), - 0.5 * Vector.GetY(), - 0.5 * Vector.GetZ()); 
	Gabar = G;
	G = G.Move(Vector.GetX(), Vector.GetY(), Vector.GetZ()); 
	Gabar.Expand(G);
	IsCreated = true;
}
void NStockSweep::Create(const CString &iFileName, NCadrGeom *GeomArr, int Num, const BPoint &iBase, const BPoint &iVector, const BPoint &iABC)
{// GeomArr lies in XY plane
	IsCreated = false;
	if(Num < 3)
		return;
	ABC = iABC;
	BMatr M = BMatr().RotX(BPoint(0., 0., 0., 1.), ABC.GetX())
		* BMatr().RotY(BPoint(0., 0., 0., 1.), -ABC.GetY())
		* BMatr().RotZ(BPoint(0., 0., 0., 1.), ABC.GetZ());
	BaseCenter = iBase;
	Vector = iVector * M;
	if(!Cont.MakeContour(GeomArr, Num))
		return;
	Cont.Trans(M * BMatr().Trans(BPoint(0.,0.,0.,1), BaseCenter + Vector * 0.5));
	Cont.SetNormal(Cont.GetNormal() * (-1.));

	Create();
	if (IsCreated)
		FileName = iFileName;
}

bool NStockSweep::CreateBody(void)
{
	int Num = int(BDebugState::Get().CellSize + 0.5);
	MQuadTree *pTree = nullptr;
	switch (NCM_PROJECT.GetGlobalState().GetModelMode())
	{
	case NGlobalState::DX:
		pTree = MakeBSPTree(Num, true);
		break;
	case NGlobalState::BSP:
		pTree = MakeBSPTree(Num);
		break;
	}
	if (pTree == nullptr)
		return false;

	SetStockType(AT_MILL);
	bool Res = CreateBodyFrBSP(pTree);

	switch (NCM_PROJECT.GetGlobalState().GetModelMode())
	{
	case NGlobalState::DX:
		delete pTree;
		MFOREST.Compress();
		break;
	case NGlobalState::BSP:
		break;
	}
	MillStockChanged = false;
	TurnStockChanged = false;
	return Res;
}

MQuadTree *NStockSweep::MakeBSPTree(int Num, bool ForDX5X /*= false */) const
{
	BPoint DeltaMin(-ST_DEC, -ST_DEC, -ST_DEC, 0.);
	BPoint DeltaMax(ST_INC, ST_INC, ST_INC, 0.);
	MeshArr MeshArray;
	MeshArray.RemoveAll();

	Cont.CrMesh4Mill(MeshArray, Vector, ColorNum, ColorNum, ForDX5X ? 0 : -1);

	MQuadTree *pTree = new MQuadTree(Gabar.GetMinPoint() + DeltaMin, Gabar.GetMaxPoint() + DeltaMax, Num, Num, GetStockGr(), false);

	for(int l = 0; l < MeshArray.GetSize(); ++l)
	{
		pTree->StoreMesh(*MeshArray[l], MeshArray[l]->GetMeshID(), cadrID(-1), false);
		delete MeshArray[l];
	}
	BBox Bounds = Gabar;
	Bounds.Zmin -= 1.; // Needed to activate empty cells determination
	pTree->CreateBodyStd(Bounds, false, false);
	pTree->Invert(false);
	pTree->CrStandard(false);
	return pTree;
}

const BPoint & NStockSweep::GetBase(void)
{
	return BaseCenter;
}

void NStockSweep::MoveBase(double x, double y, double z)
{
	BPoint v = BPoint(x, y, z, 1.) * BMatr().RotX(0., 0., 0., 90.) - BaseCenter;
	Cont.Trans(BMatr().Trans(BPoint(0.,0.,0.,0.), v));
	BaseCenter.Set(x,y,z,1.);
	Create();
}

NStock::StockCrType NStockSweep::GetType() const
{
	return SCT_SWEEP;
}

