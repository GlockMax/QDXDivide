#include "stdafx.h"
#include "NCMDoc.h"
#include "CStocksCollection.h"
#include "OctreeSkel.h"


OctreeSkel::OctreeSkel()
{
	SkelUuid = 0;
}


OctreeSkel::~OctreeSkel()
{
}

const NCadrGeom * OctreeSkel::GetCadrGeom(cadrID ID, int StPos)
{
	if (!pDoc->NGetStockList()->IsVisible())
		return nullptr;
	const cadrID Cur = ID;
	NStock *pStock = pDoc->GetStockUnsafe(Cur.Prog);
	if (pStock == nullptr)
		return nullptr;
	if (!pStock->IsVisible())
		return nullptr;
	if (pStock->GetStockState() != AT_TURN)
		return nullptr;
	const int SkelCont = Cur.Cadr % 4;
	const NCadrGeom *pCadr = (*(pStock->GetTurnSkeleton().GetSectGeom(SkelCont)))[Cur.Cadr / 4];
	return pCadr;
}

void OctreeSkel::Initialize(CNCMDoc * pDoc, int StPos)
{
	if (!NCMComData::IsTurnEnabled())
		return;
	CStocksCollection *stocksList = pDoc->NGetStockList();
	auto NewUuid = stocksList->GetUuid();
	if (NewUuid == SkelUuid)
		return;
	SkelUuid = NewUuid;
	BBox sceneAABB = stocksList->GetGabar();
	Octree::Initialize(sceneAABB, pDoc);

	cadrID id;
	for (int stockNum = 0; stockNum < stocksList->GetSize(); stockNum++)
	{
		id.Prog = stockNum;
		CString Key;
		NStock *pStock = nullptr;
		stocksList->GetAt(stockNum, Key, (CObject *&)pStock);
		if (pStock == nullptr)
			return;
		const NTurnStockSkeleton &Skeleton = pStock->GetTurnSkeleton();
		for (int cadrNum = 0; cadrNum < Skeleton.GetSectGeom(NTurnStockSkeleton::SkelContour1)->GetSize(); cadrNum++)
		{
			NCadrGeom cCadr(*(*(Skeleton.GetSectGeom(NTurnStockSkeleton::SkelContour1)))[cadrNum]);
			BBox AABB;
			cCadr.ExpandBox(&AABB);
			id.Cadr = cadrNum * 4 + NTurnStockSkeleton::SkelContour1;
			InsertObject(id, AABB);
		}
		for (int cadrNum = 0; cadrNum < Skeleton.GetSectGeom(NTurnStockSkeleton::SkelContour2)->GetSize(); cadrNum++)
		{
			NCadrGeom cCadr(*(*(Skeleton.GetSectGeom(NTurnStockSkeleton::SkelContour2)))[cadrNum]);
			BBox AABB;
			cCadr.ExpandBox(&AABB);
			id.Cadr = cadrNum * 4 + NTurnStockSkeleton::SkelContour2;
			InsertObject(id, AABB);
		}
		for (int cadrNum = 0; cadrNum < Skeleton.GetSectGeom(NTurnStockSkeleton::SkelArcs)->GetSize(); cadrNum++)
		{
			NCadrGeom cCadr(*(*(Skeleton.GetSectGeom(NTurnStockSkeleton::SkelArcs)))[cadrNum]);
			BBox AABB;
			cCadr.ExpandBox(&AABB);
			id.Cadr = cadrNum * 4 + NTurnStockSkeleton::SkelArcs;
			InsertObject(id, AABB);
		}
	}
}
