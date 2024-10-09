#include "stdafx.h"
#include "math.h"
#include "MBSPForest.h"
#include "ConstDef.h"
#include "NCMComData.h"
#include "SOSphere.h"
#include "SOPlane.h"
#include "SOCyl.h"
#include "MBSPMerge.h"
#include "MBodyParams.h"
#include "MElemIDProc.h"
#include "MFace.h"
#include "CadrData.h"
#include "DXSubModel5X.h"
#include "MCubeMask.h"
#include "MForestSave.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

DXSubModel::DXSubModel(/*const*/ DXModel &ParentModel, BStockGrID Position)
	: Parent(ParentModel)
	, MQuadBody(ParentModel.GGabar.GetMinPoint(), ParentModel.GGabar.GetMaxPoint(), ParentModel.RowsNum, ParentModel.ColsNum, Position)
{
}


DXSubModel::~DXSubModel(void)
{

}

//MBodyParams DXSubModel::GetParams()
//{
//	return MBodyParams();
//}

void DXSubModel::ReCreate()
{
	MQuadBody::ReCreate(Parent.GGabar.GetMinPoint(), Parent.GGabar.GetMaxPoint(), Parent.RowsNum, Parent.ColsNum);
	nX = Parent.nX;
	nY = Parent.nY;
	nZ = Parent.nZ;
}

int DXSubModel::InsSpecCadr(DX_TOOLTYPE DXType,
		const BPoint & P0, const BPoint & P1, const BPoint & N,
		double R, double H, DX_ID PairID, const BMatr &Shift, bool FirstCadr, bool LastCadr, class CadrData &Data, const CadrData& MaskData)
{
	// Ensure P1 is lower than P0
	const BPoint *pP0 = &P0;
	const BPoint *pP1 = &P1;
	BPoint V(*pP1 - *pP0);
	double VN = V * N;
	if(VN > 0)
	{
		const BPoint *pB = pP0; pP0 = pP1; pP1 = pB;
		bool b = FirstCadr; FirstCadr = LastCadr; LastCadr = b;
	}
	Data.FirstCadr = FirstCadr;
	Data.LastCadr = LastCadr;
	Data.HistID = PairID.Cadr;
	switch(DXType)
	{
	case TT_3X_CYL_BALL:
		DXSubModel5X::InsBCadr(*pP0, *pP1, N, R, H, PairID.Elem, Shift, Data, MaskData);
		break;
	case TT_3X_CYL:
		DXSubModel5X::InsCCadr(*pP0, *pP1, N, R, H, PairID.Elem, Shift, Data, MaskData);
		break;
	case TT_5X_FULL_BALL:
		{
			BMatr MTMatr;
			// This fragment needed for AT_MILL_TURN only!
			if (NCMComData::IsTurnEnabled())
			{
				MTMatr.RotX(BPoint(0., 0., 0., 1.), -90.);
			}
			// END:This fragment needed for AT_MILL_TURN only
			DXSubModel5X::InsFullBCadr(P0, P1, N, R, PairID.Elem, MTMatr, Data, MaskData);
			break;
		}
	}
	return 0;
}

int DXSubModel::RastSpecCadr(int Dir, const CadrData &Data)
{
	std::vector<DXSubModel *> Bodies;
	for (DXSubModel *pCur = this; pCur != nullptr; pCur = static_cast<DXSubModel *>(pCur->pNextMBody))
	{
		if (Data.IsLast())
			Bodies.push_back(pCur);
		else if (pCur->GetGabar().Intersects(Data.Bounds))
		{
			for each (auto Group in Data.GetStockGroups())
			{
				if (Group == pCur->GetStockPos())
				{
					Bodies.push_back(pCur);
					break;
				}
			}
		}
	}

	switch (Data.DXType)
	{
	case TT_3X_CYL_BALL:
		for each (DXSubModel *pBodyInt in Bodies)
			pBodyInt->RastBCadr(Dir, Data);
		break;
	case TT_3X_CYL:
		for each (DXSubModel *pBodyInt in Bodies)
			pBodyInt->RastCCadr(Dir, Data);
		break;
	case TT_5X_FULL_BALL:
		for each (DXSubModel *pBodyInt in Bodies)
			pBodyInt->RastFullBCadr(Dir, Data);
		break;
	default:
		return 0;
	} 
	return 0;
}

void DXSubModel::ResetpModifiedCells()
{
	delete pModifiedCells;
	pModifiedCells = new MCubeMask(RowsNum, ColsNum, Parent.LevelsNum);
}

void DXSubModel::GetCellIndexes(double x, double y, double z, int & ix, int & iy, int &iz)
{
	MQuadBody::GetCellIndexes(x, y, ix, iy);
	iz = min(Parent.LevelsNum - 1, max(0, int((z - MinPoint.GetZ()) * Parent.nBig[2] / Parent.sZ)));
}

void DXSubModel::SetWindow(const BBox& Bounds)
{
	if (pNextMBody)
		((MQuadBody*)pNextMBody)->SetWindow(Bounds);
	if (Bounds.Zmin >= MaxPoint.GetZ() ||
		Bounds.Zmax <= MinPoint.GetZ() ||
		Bounds.Ymin >= MaxPoint.GetY() ||
		Bounds.Ymax <= MinPoint.GetY() ||
		Bounds.Xmin >= MaxPoint.GetX() ||
		Bounds.Xmax <= MinPoint.GetX())
	{
		Win.Set(0, 0, 0, 0, 0, 0);
		return;
	}

	int xS, xE, yS, yE, zS, zE;
	GetCellIndexes(Bounds.Xmin, Bounds.Ymin, Bounds.Zmin, xS, yS, zS);
	GetCellIndexes(Bounds.Xmax, Bounds.Ymax, Bounds.Zmax, xE, yE, zE);
	Win.Set(xS, xE + 1, yS, yE + 1, zE, zE + 1);
}

void DXSubModel::SetMaxWin()
{
	Win.Expand(0, 0, 0);
	Win.Expand(Parent.ColsNum, Parent.RowsNum, Parent.LevelsNum);
}

void DXSubModel::Save(MForestSave& SaveData)
{
	MQuadBody::Save(SaveData);
	CFile* f = SaveData.File;
	f->Write(&nX, sizeof(nX));
	f->Write(&nY, sizeof(nY));
	f->Write(&nZ, sizeof(nZ));
}

void DXSubModel::Load(CFile* f)
{
	MQuadBody::Load(f);
	f->Read(&nX, sizeof(nX));
	f->Read(&nY, sizeof(nY));
	f->Read(&nZ, sizeof(nZ));
}

void DXSubModel::CreateFullCopy(MBody* pQt)
{
	MQuadBody::CreateFullCopy(pQt);
	DXSubModel* pQ = dynamic_cast<DXSubModel*>(pQt);
	if (pQ == nullptr)
		return;
	memcpy_s(&pQ->Parent, sizeof(Parent), &Parent, sizeof(Parent));
	pQ->nX = nX;
	pQ->nY = nY;
	pQ->nZ = nZ;
}

