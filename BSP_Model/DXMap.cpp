#pragma once
#include "stdafx.h"
#include <vector>
#include "DXSubModel5X.h"
#include "RTriRandMesh.h"
#include "DXBigCell5X.h"
#include "MCubeMask.h"
#include "SOPlane.h"
#include "MBSPForest.h"
#include "DXMap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

DXMap::DXMap() : MemCell(START_SIZE)
{
	nBig[0] = 0;
	nBig[1] = 0;
	nBig[2] = 0;
	BufLines[0] = NULL;
	BufLines[1] = NULL;
}

void DXMap::Set(const DXSubModel5X& Body5X, int B0, int B1)
{
	startX = B0 * Body5X.GetBigSize(0);
	startY = B1 * Body5X.GetBigSize(1);
	nX = min(Body5X.GetBigSize(0), Body5X.GetNX() - startX);
	nY = min(Body5X.GetBigSize(1), Body5X.GetNY() - startY);
	if (Body5X.GetWindow().IsDefined())
		nZ = Body5X.GetWNZ();
	else
		nZ = Body5X.GetNZ();
	DeltaZ = Body5X.GetZstep();
	RectID = new DXMemID[nX * nY];
	for(int i = 0; i < nX * nY; ++i)
		RectID[i] = DXP_END;
	BufLines[0] = NULL;
	BufLines[1] = NULL;
	nBig[0] = Body5X.GetBigSize(0);
	nBig[1] = Body5X.GetBigSize(1);
	nBig[2] = Body5X.GetBigSize(2);
}

DXMap::~DXMap()
{
	delete[] RectID;
//	MemCell.DeleteAll();
}

void DXMap::AddEdgeZ(const DXCellInd & CellInd, int Shift, int NumEdge, unsigned int Edge, DXBigCell5X *pBigCell)
{// Parameters should be correct
	DXMemID &RectIDPos = RectID[(CellInd.Ix + Shift) * nY + CellInd.Iy];
	DXSearchPair &SearchPair = BufLines[Shift + 1][(NumEdge >= 10) ? 0 : 1];

	DXCell5X &FoundCell = GetCellFast(CellInd, RectIDPos, SearchPair);
	FoundCell.InitEdge(NumEdge, Edge);
	FoundCell.Init(pBigCell);
}

void DXMap::AddEdgeY(const DXCellInd & CellInd, int Shift, int NumEdge, unsigned int Edge, DXBigCell5X *pBigCell)
{// Parameters should be correct
	DXMemID &RectIDPos = RectID[(CellInd.Ix + Shift) * nY + CellInd.Iy];
	DXSearchPair &SearchPair = BufLines[Shift + 1][CellInd.Iy];

	DXCell5X& FoundCell = GetCellFast(CellInd, RectIDPos, SearchPair);
//	DXCell5X& FoundCell = GetCellSlow(CellInd, RectIDPos);
	FoundCell.InitEdge(NumEdge, Edge);
	FoundCell.Init(pBigCell);
}

void DXMap::AddEdgeX(const DXCellInd & CellInd, int Shift, int NumEdge, unsigned int Edge, DXBigCell5X *pBigCell)
{// Parameters should be correct
	DXMemID &RectIDPos = RectID[(CellInd.Ix) * nY + CellInd.Iy + Shift];
	DXSearchPair &SearchPair = BufLines[Shift + 1][CellInd.Ix];

	DXCell5X &FoundCell = GetCellFast(CellInd, RectIDPos, SearchPair);
//	DXCell5X& FoundCell = GetCellSlow(CellInd, RectIDPos);
	FoundCell.InitEdge(NumEdge, Edge);
	FoundCell.Init(pBigCell);
}

void DXMap::FinishFast(void)
{
	delete[] BufLines[0];
	delete[] BufLines[1];
	BufLines[0] = NULL;
	BufLines[1] = NULL;
}

void DXMap::InitZFast(int NextI, int NextJ)
{
	int RectBase = (NextI - 1) * nY;
	if(NextI > 0)
	{
		if (NextJ < nY)
			BufLines[0][1].Init(RectID[RectBase + NextJ]);
		if(NextJ > 0)
			BufLines[0][0].Init(RectID[RectBase + NextJ - 1]);
	}
	if (NextI < nX)
	{
		RectBase += nY;
		if (NextJ < nY)
			BufLines[1][1].Init(RectID[RectBase + NextJ]);
		if (NextJ > 0)
			BufLines[1][0].Init(RectID[RectBase + NextJ - 1]);
	}
}

void DXMap::InitYFast(int NextI)
{
	int RectBase = (NextI - 1) * nY;
	if(NextI > 0)
	{
		for(int i = 0; i < nY; ++i)
			BufLines[0][i].Init(RectID[RectBase + i]);
	}
	if (NextI < nX)
	{
		RectBase += nY;
		for (int i = 0; i < nY; ++i)
			BufLines[1][i].Init(RectID[RectBase + i]);
	}
}

void DXMap::NewInitYFast(int NextI)
{
	if (BufLines[0] == nullptr)// is not initialized yet
		InitFastY();
	InitYFast(NextI);
}

void DXMap::InitXFast(int NextJ)
{
	if (NextJ > 0)
	{
		for(int i = 0; i < nX; ++i)
			BufLines[0][i].Init(RectID[i * nY + NextJ - 1]);
	}
	for(int i = 0; i < nX - 1; ++i)
		BufLines[1][i].Init(RectID[i * nY + NextJ]);
	if (NextJ < nY)
		BufLines[1][nX - 1].Init(RectID[(nX - 1) * nY + NextJ]);
}

void DXMap::InitFast(int Size)
{
	BufLines[0] = new DXSearchPair[Size];
	BufLines[1] = new DXSearchPair[Size];
	for (int i = 0; i < Size; ++i)
	{
		BufLines[0][i].SetMemory(&MemCell);
		BufLines[1][i].SetMemory(&MemCell);
	}
}

void DXMap::InitFastZ()
{
	InitFast(2);
}

void DXMap::InitFastY()
{
	InitFast(nY);
}

void DXMap::InitFastX()
{
	InitFast(nX);
}

DXCell5X & DXMap::GetCellFast(const DXCellInd &CellInd, DXMemID &RectIDPos, DXSearchPair &SearchPair)
{
	if(SearchPair.DownID == DXP_END)
	{// Empty position
		// Add new cell and store its ID in RectID
		RectIDPos = MemCell.FindEmptyPos();
		MemCell.SetAtNext(RectIDPos, DXP_END);
		MemCell.SetAtiZ(RectIDPos, short(CellInd.Iz));
		SearchPair.Init(RectIDPos);
		return MemCell.SetNew(RectIDPos);
	}
	while(CellInd.Iz >= SearchPair.UpZInd)
	{	
		SearchPair.StepUp();
	}
	if(CellInd.Iz == SearchPair.DownZInd)
		return MemCell.GetAtC(SearchPair.DownID);
	if(CellInd.Iz < SearchPair.DownZInd)
	{ // same as empty position but keep RectIDPos as next ID
		DXMemID OldID = RectIDPos;
		RectIDPos = MemCell.FindEmptyPos();
		MemCell.SetAtNext(RectIDPos, OldID);
		MemCell.SetAtiZ(RectIDPos, short(CellInd.Iz));
		SearchPair.Init(RectIDPos);
		return MemCell.SetNew(RectIDPos);
	}
	//CellInd.Iz < SearchPair.UpZInd && CellInd.Iz > SearchPair.DownZInd
	{
		SearchPair.DownID = MemCell.InsertAfter(SearchPair.DownID);
		SearchPair.DownZInd = CellInd.Iz;
		MemCell.SetAtiZ(SearchPair.DownID, short(CellInd.Iz));
		return MemCell.SetNew(SearchPair.DownID);
	}
}

DXCell5X& DXMap::GetCellSlow(const DXCellInd& CellInd, DXMemID& RectIDPos)
{
	DXSearchPair SearchPair;
	SearchPair.SetMemory(&MemCell);
	SearchPair.Init(RectIDPos);
	return GetCellFast(CellInd, RectIDPos, SearchPair);
}

DXCell5X * DXMap::GetCell(const DXCellInd &CellInd) const
{
	DXMemID &RectIDPos = RectID[(CellInd.Ix - startX) * nY + CellInd.Iy - startY];
	bool Found = false;
	DXMemID RetMemID = RectIDPos;
	for(DXMemID MemID = RetMemID; MemID != DXP_END; MemID = MemCell.GetAtNext(MemID))
	{
		int CurZ = MemCell.GetAtiZ(MemID);
		if(CurZ < CellInd.Iz)
		{
			RetMemID = MemID;
			continue;
		}
		if(CurZ == CellInd.Iz)
		{
			RetMemID = MemID;
			Found = true;
		}
		break;
	}
	// if Found then RetMemID is an ID of founded cell, otherwise RetMemID is an ID of the previous cell or DXP_END
	if (Found)
		return &MemCell.GetAtC(RetMemID);
	else
		return nullptr;
	//{// Add new cell
	//	if(RetMemID == DXP_END)
	//	{
	//		RetMemID = MemCell.FindEmptyPos();
	//		RectIDPos = RetMemID;
	//		MemCell.SetAtNext(RetMemID, DXP_END);
	//	}
	//	else
	//		RetMemID = MemCell.InsertAfter(RetMemID);
	//	MemCell.SetAtiZ(RetMemID, short(CellInd.Iz));
	//	return MemCell.SetNew(RetMemID);
	//}
}

//int DXMap::GetCount(void) const
//{
//	return MemCell.GetDataCount();
//}

DXSearchPair::DXSearchPair(void)
{
	DownID = DXP_END;
	UpID = DXP_END;
	DownZInd = 0;
	UpZInd = 0;
//	WasEqual = false;
	pMemory = nullptr;
}

bool DXSearchPair::StepUp(void)
{
	DownID = UpID;
	DownZInd = UpZInd;
	UpID = pMemory->GetAtNext(UpID);
	UpZInd = (UpID == DXP_END) ? HugeZInd : pMemory->GetAtiZ(UpID);
	return true;
}

void DXSearchPair::Init(DXMemID StartID)
{
	DownID = StartID;
	if(StartID == DXP_END)
		return;
	DownZInd = pMemory->GetAtiZ(DownID);
	UpID = pMemory->GetAtNext(DownID);
	UpZInd = (UpID == DXP_END) ? HugeZInd : pMemory->GetAtiZ(UpID);
}

void DXSearchPair::SetMemory(DXMemoryCell5X *pMem)
{
	pMemory = pMem;
}

void DXMap::MakeTrianglesBigColumn() const
{
	for (int IndX = 0; IndX < nBig[0]; ++IndX)
	{
		bool BorderXUp = (IndX == nBig[0] - 1);
		bool BorderXDown = (IndX == 0);
		for (int IndY = 0; IndY < nBig[1]; ++IndY)
		{
			bool BorderYUp = (IndY == nBig[1] - 1);
			bool BorderYDown = (IndY == 0);
			for (DXMemID CurMemID = GetMemID(IndX, IndY); CurMemID != DXP_END; CurMemID = MemCell.GetAtNext(CurMemID))
			{
				DXCell5X &CurCell = MemCell.GetAtC(CurMemID);
				DXCellInd key(IndX + startX, IndY + startY, MemCell.GetAtiZ(CurMemID));// absolute indexes
				bool BorderZUp = (key.Iz % nBig[2] == nBig[2] - 1);
				bool BorderZDown = (key.Iz % nBig[2] == 0);
				if (GBodyRender::GetFindVergPoint()) // make additional points for the minimum coordinates
				{
					if (BorderZDown)
					{ // Make additional point for the face number 0
						DXCell5X TempCell;
						TempCell.Clear();
						TempCell.pBigCell = CurCell.pBigCell;
						TempCell.Edges[4] = CurCell.Edges[0];
						TempCell.Edges[5] = CurCell.Edges[1];
						TempCell.Edges[6] = CurCell.Edges[2];
						TempCell.Edges[7] = CurCell.Edges[3];
						DXCell5X::FEP FaceEdgeP;
						TempCell.FillFaceEdgeP(FaceEdgeP); // Don't check return code
						TempCell.MakeAdditPoints(key, FaceEdgeP);
						CurCell.Bounds[0] = TempCell.Bounds[5];
					}
					if (BorderXDown)
					{ // Make additional point for the face number 4
						DXCell5X TempCell;
						TempCell.Clear();
						TempCell.pBigCell = CurCell.pBigCell;
						TempCell.Edges[1] = CurCell.Edges[3];
						TempCell.Edges[5] = CurCell.Edges[7];
						TempCell.Edges[9] = CurCell.Edges[8];
						TempCell.Edges[11] = CurCell.Edges[10];
						DXCell5X::FEP FaceEdgeP;
						TempCell.FillFaceEdgeP(FaceEdgeP); // Don't check return code
						TempCell.MakeAdditPoints(key, FaceEdgeP);
						CurCell.Bounds[4] = TempCell.Bounds[2];
					}
					if (BorderYDown)
					{ // Make additional point for the face number 1
						DXCell5X TempCell;
						TempCell.Clear();
						TempCell.pBigCell = CurCell.pBigCell;
						TempCell.Edges[2] = CurCell.Edges[0];
						TempCell.Edges[6] = CurCell.Edges[4];
						TempCell.Edges[10] = CurCell.Edges[8];
						TempCell.Edges[11] = CurCell.Edges[9];
						DXCell5X::FEP FaceEdgeP;
						TempCell.FillFaceEdgeP(FaceEdgeP); // Don't check return code
						TempCell.MakeAdditPoints(key, FaceEdgeP);
						CurCell.Bounds[1] = TempCell.Bounds[3];
					}
				}
				CurCell.MakeTriangles(key, *this); // key must be absolute i.e. its indexes must be relative to whole body but not for DXMap
												   // Place the new faces to the neighbour cells
				DXCellInd keyb = key; // To prevent key changing
				if (CurCell.GetBound(2))
				{
					if (!BorderXUp)
					{
						DXCell5X *pNextCell = GetCell(keyb.ShiftX(+1));
						if (pNextCell)
							pNextCell->Bounds[4] = CurCell.Bounds[2];
						keyb = key;
					}
				}
				if (CurCell.GetBound(3))
				{
					if (!BorderYUp)
					{
						DXCell5X *pNextCell = GetCell(keyb.ShiftY(+1));
						if (pNextCell)
							pNextCell->Bounds[1] = CurCell.Bounds[3];
						keyb = key;
					}
				}
				if (CurCell.GetBound(5))
				{
					if (!BorderZUp)
					{
						DXCell5X *pNextCell = GetCell(keyb.ShiftZ(+1));
						if (pNextCell)
							pNextCell->Bounds[0] = CurCell.Bounds[5];
						keyb = key;
					}
				}
			}
		}
	}
}

int DXMap::MakeNewEdge(unsigned int Dir, const float *Coord, const BPoint &Normal, unsigned int &Edge, DXCell5X *pCell) const
{
	DXBigCell5X *pBigCell = pCell->pBigCell;
	std::vector <DXEdge> &MapEdges = pBigCell->GetMapEdges();
	RTriRandMesh *pNewArea = &pBigCell->GetTriangleArea();
	float *BUF = pNewArea->GetNextNP();
	BUF[3] = Coord[0];
	BUF[4] = Coord[1];
	BUF[5] = Coord[2];
	ElemID ID = 0;
	int SurfID;
	float N[3] = { -float(Normal.GetX()), -float(Normal.GetY()), -float(Normal.GetZ()) };
	MElemIDProc::SetNormalToSurfID(SurfID, N);
	MElemIDProc::SetSurfID(&ID, SurfID);
	Edge = unsigned int(MapEdges.size());
	MapEdges.emplace_back();
	DXEdge *pEdge = &MapEdges.at(Edge);
	pEdge->Set(true, ID, pNewArea->GetActSizeP() - 1, 0);
	//---------------------заносим координаты и нормаль точки в массив вершин треугольника------------------------
	DXCell5X::StoreNPC(BUF, ID, *pBigCell->GetGData());
	return 0;

}

int DXMap::SetEdge(const float *Coord, ElemID Surf, unsigned int &Edge, bool i, unsigned int Degraded, DXBigCell5X *pBigCell) const
{
	std::vector <DXEdge> &MapEdges = pBigCell->GetMapEdges();
	Edge = unsigned int(MapEdges.size());
	RTriRandMesh *pNewArea = &pBigCell->GetTriangleArea();
	float *BUF = pNewArea->GetNextNP();
	BUF[3] = Coord[0];
	BUF[4] = Coord[1];
	BUF[5] = Coord[2];
	MapEdges.emplace_back(i, Surf, pNewArea->GetActSizeP() - 1, Degraded);
	//---------------------заносим координаты и нормаль точки в массив вершин треугольника------------------------
	DXCell5X::StoreNPC(BUF, Surf, *pBigCell->GetGData());
	return 0;
}
