#include "stdafx.h"
#include "ConstDef.h"
#include "NObjEnums.h"
#include <future>
#include "BaseRender.h"
#include "SOSphere.h"
#include "SOCyl.h"
#include "NCMComData.h"
#include "MBSPForest.h"
#include "SOrigSurf.h"
#include "math.h"
#include "RTriRandMesh.h"
#include "DXSubModel5X.h"
#include "DXSubModel5X.h"
#include "DXCell5X.h"
#include "DXMaps.h"
#include "BDebugState.h"
#include "MCubeMask.h"
#include "DXFinalCycle.h"
#include "DXBigCell5X.h"
#include "BStl.h"
#include "BDCEL_BSP.h"
#include "DXSharpEdge.h"
#include "RRopes.h"
#include "GQuadRenderDX5X.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//const double P = 1.e-8;
using namespace std;
const double MINSINCOS = 0.21;

int GQuadRenderDX5X::WholeCellsNum = 512 * 512 * 6;
double GQuadRenderDX5X::SmoothAngle = 6.;

//конструктор 
GQuadRenderDX5X::GQuadRenderDX5X(const DXSubModel5X &Model) : GBodyRender(Model)
{
	ColsNum = ((DXSubModel5X &)Model).ColsNum;
	RowsNum = ((DXSubModel5X &)Model).RowsNum;

	LevelsNum = ((DXSubModel5X &)Model).GetLevelsNum();
	MinPointShift = Gabar.GetMinPoint() - BPoint(0., 0., 0., 1.);
	pBigCells = new TAArray;
	pBigCells->SetSizes(ColsNum, RowsNum, LevelsNum);
}

GQuadRenderDX5X::GQuadRenderDX5X(const GQuadRenderDX5X &inRender) :
	GBodyRender(inRender)
{
	ColsNum = inRender.ColsNum;
	RowsNum = inRender.RowsNum;
	LevelsNum = inRender.LevelsNum;
	MinPointShift = inRender.MinPointShift;
	pBigCells = new TAArray;
	pBigCells->SetSizes(ColsNum, RowsNum, LevelsNum);
	DXBigCell5X **Stor = pBigCells->GetStor();
	for (int i = 0; i < ColsNum * RowsNum * LevelsNum; ++i)
	{
		DXBigCell5X *pinCell = inRender.pBigCells->GetStor()[i];
		Stor[i] = (pinCell == nullptr) ? nullptr : new DXBigCell5X(*pinCell);
	}
}
//деструктор
GQuadRenderDX5X::~GQuadRenderDX5X(void)
{
	pBigCells->ClearAll(ColsNum, RowsNum, LevelsNum);
	delete pBigCells;
}

GQuadRenderDX5X* GQuadRenderDX5X::CreateGQuadRenderDX5XList(const DXSubModel5X &Tree)
{
	DXSubModel5X *pBody = static_cast<DXSubModel5X *>(Tree.GetNextMBody());
	GQuadRenderDX5X* pResult = new GQuadRenderDX5X(Tree);// memory leak
	while (pBody)
	{
		GQuadRenderDX5X* pNext = new GQuadRenderDX5X(*pBody);
		pResult->AddRenderInList(pNext);
		pBody = static_cast<DXSubModel5X *>(pBody->GetNextMBody());
	}
	return pResult;
}

GQuadRenderDX5X* GQuadRenderDX5X::CreateListCopy(const GQuadRenderDX5X &inRender)
{
	GQuadRenderDX5X *result = new GQuadRenderDX5X(inRender);
	GQuadRenderDX5X *last = result;
	GQuadRenderDX5X *next = NULL;

	GBodyRender *nextBody = inRender.GetNextRender();
	while (nextBody)
	{
		next = new GQuadRenderDX5X( *(GQuadRenderDX5X*)nextBody );
		last->SetNextRender(next);
		next->SetPreviousRender(last);

		last = next;
		nextBody = nextBody->GetNextRender();
	}
	return result;
}

void GQuadRenderDX5X::WriteSTL(CStdioFile &f) const
{
	__int32 GlobTriNum = 0;
	for (int i = 0; i < GetAreasNum(); ++i)
	{
		const RTriRandMesh *pTrianglArea = GetArea(i);
		if (!pTrianglArea)
			continue;
		GlobTriNum += pTrianglArea->GetTriNum();
	}
	f.SeekToBegin();
	char s[5 * 16 + 1] = "STL export                                                                      ";
	f.Write(s, 5 * 16);// title

	f.Write(&GlobTriNum, 4);

	for (int i = 0; i < GetAreasNum(); ++i)
	{
		const RTriRandMesh *pTrianglArea = GetArea(i);
		if (!pTrianglArea)
			continue;
		__int32 TriNum = pTrianglArea->GetTriNum();
		if (TriNum <= 0)
			return;

		for (int k = 0; k < TriNum; ++k)
		{
			char buf[2] = { '\x0', '\x0' };
			GLfloat fbuf[4 * 3];
			BPoint Pt[3], n;
			pTrianglArea->ExtractTriangle(k, Pt[0], Pt[1], Pt[2], n);
			fbuf[0] = GLfloat(-n.GetX());// Normal// '-' - added 2.11.18
			fbuf[1] = GLfloat(-n.GetY());// Normal
			fbuf[2] = GLfloat(-n.GetZ());// Normal
			for (int j = 0; j < 3; ++j)
			{
				const BPoint &P = Pt[j];
				fbuf[j * 3 + 3] = GLfloat(P.GetX());// Vertex
				fbuf[j * 3 + 4] = GLfloat(P.GetY());// Vertex
				fbuf[j * 3 + 5] = GLfloat(P.GetZ());// Vertex
			}
			f.Write(fbuf, 4 * 3 * 4);
			f.Write(buf, 2);
		}
	}
}
size_t GQuadRenderDX5X::StoreSTL(GLfloat*& pStor) const
{
	size_t GlobTriNum = 0;
	for (int i = 0; i < GetAreasNum(); ++i)
	{
		const RTriRandMesh* pTrianglArea = GetArea(i);
		if (!pTrianglArea)
			continue;
		GlobTriNum += pTrianglArea->GetTriNum();
	}
	pStor = new GLfloat[GlobTriNum * 18];
	auto fbuf = pStor;
	for (int i = 0; i < GetAreasNum(); ++i)
	{
		const RTriRandMesh* pTrianglArea = GetArea(i);
		if (!pTrianglArea)
			continue;
		size_t TriNum = pTrianglArea->GetTriNum();
		if (TriNum <= 0)
			return 0;

		for (int k = 0; k < TriNum; ++k)
		{
			BPoint Pt[3], n;
			pTrianglArea->ExtractTriangle(k, Pt[0], Pt[1], Pt[2], n);
			for (int j = 0; j < 3; ++j)
			{
				fbuf[j * 6 + 0] = GLfloat(-n.GetX());// Normal// '-' - added 2.11.18
				fbuf[j * 6 + 1] = GLfloat(-n.GetY());// Normal
				fbuf[j * 6 + 2] = GLfloat(-n.GetZ());// Normal
				const BPoint& P = Pt[j];
				fbuf[j * 6 + 3] = GLfloat(P.GetX());// Vertex
				fbuf[j * 6 + 4] = GLfloat(P.GetY());// Vertex
				fbuf[j * 6 + 5] = GLfloat(P.GetZ());// Vertex
			}
			fbuf += 18;
		}
	}
	return GlobTriNum;
}
//дать тип
NCMApplicationType GQuadRenderDX5X::GetType() const
{
	return AT_MILL;
}
//рисование
int GQuadRenderDX5X::RenderCh(enum RenderMode GlobRMode, int Type, bool DrawSurf)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	if (BodyRotActive)
	{
		glPushMatrix();
		glMultMatrixd(BodyRot.Or().GetArray());
	}

	BaseRender::ClearVBOs(); // Do it in the main thread only

	bool EdgesIndexesActive = MFOREST.GetIDProc()->GetStockEdgesDX();
	glPushAttrib(GL_ENABLE_BIT | GL_LINE_BIT | GL_COLOR_BUFFER_BIT);
	if(EdgesIndexesActive)
	{
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1., 1.);
	}
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	if (IsCollided())
	{
		glDisableClientState(GL_COLOR_ARRAY);
		glColor3fv(GetCollidedColor().array);
	}
	else
		glEnableClientState(GL_COLOR_ARRAY);
	if (DrawSurf)
	{
		for (int ix = 0; ix < ColsNum; ++ix)
			for (int iy = 0; iy < RowsNum; ++iy)
				for (int iz = 0; iz < LevelsNum; ++iz)
				{
					DXBigCell5X* pBuf = pBigCells->Get(ix, iy, iz);
					if (!pBuf)
						continue;
					RTriRandMesh* pTrianglArea = &pBuf->GetTriangleArea();
					if (pTrianglArea != nullptr)
						pTrianglArea->DrawTriSurf();
				}
	}
	if (EdgesIndexesActive && (Type & 4))
	{
		if (GlobRMode == RM_TRANSLUCENT)
		{
			glPushAttrib(GL_ENABLE_BIT);
			glDisable(GL_LIGHTING);
		}
		else
			BaseRender::SetWFMode();
		glDisableClientState(GL_COLOR_ARRAY);
		if(pRopes->IsEmpty())
		{
			for (int ix = 0; ix < ColsNum; ++ix)
				for (int iy = 0; iy < RowsNum; ++iy)
					for (int iz = 0; iz < LevelsNum; ++iz)
					{
						DXBigCell5X* pBuf = pBigCells->Get(ix, iy, iz);
						if (pBuf != nullptr)
						{
							pBuf->DrawSharpEdges(EdgesColor, EdgesWidth);
							//RTriRandMesh* pTrianglArea = &pBuf->GetTriangleArea();
							//if (pTrianglArea != nullptr)
							//	pTrianglArea->DrawTriEdges(GlobRMode, pBuf->GetSharpEdges(), EdgesColor, EdgesWidth);
						}
					}
		}
		else
		{
			BaseRender::UnbindArrays();
			glLineWidth(float(EdgesWidth));
			glColor3fv(EdgesColor.array);
			glPushName(OG_SKELETON);
			pRopes->Draw();
			glPopName();
		}
		if (GlobRMode == RM_TRANSLUCENT)
			glPopAttrib();
		else
			BaseRender::RemoveWFMode();
	}
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	BaseRender::UnbindArrays();
	glPopAttrib();
	if (BodyRotActive)
		glPopMatrix();
	return 0;
}

void GQuadRenderDX5X::FillPartRenders(const MQuadBody& Body, bool All)
{// Fill "big" cell
	DXSubModel5X& Body5X = (DXSubModel5X&)Body;
	if (All)
		Body5X.SetMaxWin();
	else
	{
		Body5X.JoinModifiedCells();
		Body5X.FillChangedAreaWin();
	}
	const MQuadBody::Window& ModifWin = Body5X.GetWindow();
	const MCubeMask& ModifiedCells = static_cast<const MCubeMask&>(Body5X.GetModifiedCells());
	if (!ModifWin.IsDefined())
		return;
	// Create triangle mesh

	Body5X.Normalization(); // model normalization for the mesh creation

	int nX = ModifWin.ixEnd - ModifWin.ixStart;
	int nY = ModifWin.iyEnd - ModifWin.iyStart;
	DXMaps* pBColumnArr = new DXMaps(nX, nY);
	DXMaps& BColumnArr = *pBColumnArr;  // boundary cells storage

	TAArray* pNewBigCells = new TAArray;
	TAArray& NewBigCells = *pNewBigCells;// = *pNewBigCells;
	NewBigCells.SetSizes(ColsNum, RowsNum, LevelsNum);
	if (!All)
		NewBigCells.ResetChanged(ModifWin, ModifiedCells);

	const double StepX = Body5X.GetXstep();
	const double StepY = Body5X.GetYstep();
	const double StepZ = Body5X.GetZstep();
	DXBigCell5X::GroupData::StepX = StepX;
	DXBigCell5X::GroupData::StepY = StepY;
	DXBigCell5X::GroupData::StepZ = StepZ;
	DXBigCell5X::GroupData::MinPointShift = MinPointShift;
	DXBigCell5X::GroupData::pActMemory = &Body5X.GetMemory();
	// Store all dexel points to BCells as edges of cells
	// Z direction
	if (BDebugState::Get().Async)
	{
		std::vector<std::future<void>> Futures;
		Futures.reserve(max((ModifWin.ixEnd - ModifWin.ixStart), (ModifWin.iyEnd - ModifWin.iyStart)));
		for (int B0 = ModifWin.ixStart; B0 < ModifWin.ixEnd; ++B0)// X index of big cell
		{
			Futures.push_back(std::async(std::launch::async, [&](int k0)
				{
					for (int B1 = ModifWin.iyStart; B1 < ModifWin.iyEnd; ++B1)// Y index of big cell
					{
						DoZProc(k0, B1, BColumnArr, Body5X, NewBigCells, All);
					}
				}, B0));
		}
		BaseRender::ClearVBOs(); // Do it in the main thread only
		for (int i = 0; i < Futures.size(); ++i)
			Futures[i].get();

		// Y direction
		Futures.clear();
//		Futures.reserve((ModifWin.ixEnd - ModifWin.ixStart));
		for (int B1 = ModifWin.ixStart; B1 < ModifWin.ixEnd; ++B1) // X index
		{
			Futures.push_back(std::async(std::launch::async, [&](int k1) {DoYProc(k1, BColumnArr, Body5X, NewBigCells, All); }, B1));
		}
		for (int i = 0; i < Futures.size(); ++i)
			Futures[i].get();

		// X direction
		Futures.clear();
//		Futures.reserve(ModifWin.iyEnd - ModifWin.iyStart);
		for (int B0 = ModifWin.iyStart; B0 < ModifWin.iyEnd; ++B0) // Y index
		{
			Futures.push_back(std::async(std::launch::async, [&](int k0) {DoXProc(k0, BColumnArr, Body5X, NewBigCells, All); }, B0));
		}
		for (int i = 0; i < Futures.size(); ++i)
			Futures[i].get();
	}
	else
	{
		for (int B0 = ModifWin.ixStart; B0 < ModifWin.ixEnd; ++B0)// X index of big cell
			for (int B1 = ModifWin.iyStart; B1 < ModifWin.iyEnd; ++B1)// Y index of big cell
				DoZProc(B0, B1, BColumnArr, Body5X, NewBigCells, All);

		// Y direction
		for (int B1 = ModifWin.ixStart; B1 < ModifWin.ixEnd; ++B1) // X index
			DoYProc(B1, BColumnArr, Body5X, NewBigCells, All);

		// X direction
		for (int B0 = ModifWin.iyStart; B0 < ModifWin.iyEnd; ++B0) // Y index
			DoXProc(B0, BColumnArr, Body5X, NewBigCells, All);
	}

	BColumnArr.MakeTrianglesPar();
	// This fragment needed for AT_MILL_TURN only!
	// Для токарной и токарно-фрезерной заготовок модель повернута 
	// относительно истинного положения на -90 градусов вокруг OX
	if (NCMComData::IsTurnEnabled())
	{
		BMatr M;
		M.RotX(0., 0., 0., 90.);
		DXBigCell5X **Store = NewBigCells.GetStor();
		for (int i = 0; i < RowsNum * ColsNum * LevelsNum; ++i)
		{
			DXBigCell5X *pBuf = Store[i];
			if (pBuf == nullptr)
				continue;

			pBuf->Transform(M);
		}
	}
	RRopes* pNewRopes = nullptr;
	if (All)
		pNewRopes = MakeRopes(NewBigCells, BColumnArr, Body5X);
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	TransModifArraysCh(NewBigCells);
	if (pNewRopes == nullptr)
		pRopes->Clear();
	else
	{
		delete pRopes;
		pRopes = pNewRopes;
	}
	singleLock.Unlock();
	pNewBigCells->ClearAll(ColsNum, RowsNum, LevelsNum);
	delete pNewBigCells;
	delete pBColumnArr;
}

void GQuadRenderDX5X::Adjust(UINT Ind, UINT SharpInd, const DXBigCell5X& BigCell, const DXMaps& BColumnArr, const DXSubModel5X& Body5X, const TAArray& BigCells, BLinesDCEL& LinesDCEL) const
{
	// Ind - index in Bounds from BigCell
	const DXSharpEdge& SharpEdge = BigCell.GetSharpEdges()[SharpInd];
	const bool IsEnd = SharpEdge.boundMapIndStart_ != Ind;// is end point on the face of big cell
	const UINT BoundInd = IsEnd ? SharpEdge.boundInds_.end_ : SharpEdge.boundInds_.start_;// bound ind for the point from face
	if (BoundInd != 5 && BoundInd != 3 && BoundInd != 2)
		return; // only up faces is interesting here
	DXCellInd AdjKey;// global key of neighbour small cell
	const auto AdjBound = SharpEdge.MakeAdjData(IsEnd ? SharpEdge.boundInds_.end_ : SharpEdge.boundInds_.start_, AdjKey);// index of face point in neighbour small cell
	const int B0 = AdjKey.Ix / Body5X.GetParent().nBig[0];
	const int B1 = AdjKey.Iy / Body5X.GetParent().nBig[1];
	const int B2 = AdjKey.Iz / Body5X.GetParent().nBig[2];
	const DXCell5X* pCell = BColumnArr[B0][B1].GetCell(AdjKey);// neighbour small cell
	if (pCell == nullptr)
		return;// internal error
	// find pair for sharp edge
	const DXBigCell5X* pNeigCell = BigCells.Get(B0, B1, B2);// neighbour big cell
	if (pNeigCell == nullptr)
		return;// internal error
	const DXBigCell5X& NeigCell = *pNeigCell;// neighbour big cell
	if (&BigCell == &NeigCell)
		return; // current small cell and NeigSmallCell belongs to the same big cell
	// get another bound
	const DXBound* pBound = pCell->GetBound(AdjBound);
	if (pBound == nullptr)
		return;// internal error
	const UINT NeigSharpIndex = pBound->GetIndexSharp()[0];// index of SharpEdge in neighbour big cell (it is single so its index is 0)
	if (NeigSharpIndex == UINT_MAX )
		return; // we have bound but no incident sharp edges 
	const DXSharpEdge& NeigSharpEdge = NeigCell.GetSharpEdges()[NeigSharpIndex];// SharpEdge in neighbour big cell
	const bool IsEndNeig = NeigSharpEdge.boundInds_.end_ == AdjBound;// is end point of neighbour SharpEdge on face of big cell
	LinesDCEL[NeigSharpEdge.edgeInd_ + (IsEndNeig ? 1 : 0)].SetPoint(IsEnd ? SharpEdge.endPt_ : SharpEdge.startPt_);// change the pointer to one of the different but matching points from neighboring cells
	LinesDCEL.Connect(SharpEdge.edgeInd_, NeigSharpEdge.edgeInd_);// join 2 edges with coinsiding points
}

RRopes* GQuadRenderDX5X::MakeRopes(const TAArray& BigCells, const DXMaps& BColumnArr, const DXSubModel5X& Body5X) const
{
	// make LinesDCEL
	BLinesDCEL LinesDCEL;
	size_t EdgesCount = 0;
	DXBigCell5X** Store = BigCells.GetStor();
	// calculate LinesDCEL size
	for (int i = 0; i < RowsNum * ColsNum * LevelsNum; ++i)
	{
		DXBigCell5X* pBuf = Store[i];
		if (pBuf == nullptr)
			continue;
		auto& SharpEdges = pBuf->GetSharpEdges();
		if (SharpEdges.empty())
			continue;
		SharpEdges.begin()->edgeInd_ = static_cast<UINT>(EdgesCount * 2);
		EdgesCount += SharpEdges.size();
	}
	LinesDCEL.Resize(EdgesCount * 2);
	// make edges for internal cells (concurrently)
	std::vector<std::future<void>> Futures;
	Futures.reserve(RowsNum * ColsNum);
	for (int i = 0; i < RowsNum * ColsNum * LevelsNum; i += LevelsNum)
	{
		Futures.push_back(std::async(std::launch::async, [&](int i)
			{
				for (int j = i; j < i + LevelsNum; ++j)
				{
					DXBigCell5X* pBuf = Store[j];
					if (pBuf == nullptr)
						continue;
					auto& SharpEdges = pBuf->GetSharpEdges();
					for (int k = 0; k < SharpEdges.size(); ++k)
					{
						DXSharpEdge& SharpEdge = SharpEdges[k];
						SharpEdge.edgeInd_ = SharpEdges.begin()->edgeInd_ + k * 2;
						LinesDCEL.ReplaceEdge(SharpEdge.edgeInd_, SharpEdge.startPt_, SharpEdge.endPt_, UINT_MAX, UINT_MAX);
					}
					const auto& Bounds = pBuf->GetMapBounds();
					for (int k = 0; k < Bounds.size(); ++k)
					{
						const auto& Bound = Bounds[k];
						if (Bound.Have2Edges())
						{
							const DXSharpEdge& Sh0 = SharpEdges[Bound.GetIndexSharp()[0]];
							const DXSharpEdge& Sh1 = SharpEdges[Bound.GetIndexSharp()[1]];
							LinesDCEL.Connect(Sh0.edgeInd_, Sh1.edgeInd_);
						}
					}
				}
			}
		, i));
	}
	for (int i = 0; i < Futures.size(); ++i)
		Futures[i].get();
	// make edges for boundary cells (sequentially)
	for (int i = 0; i < RowsNum * ColsNum * LevelsNum; ++i)
	{
		DXBigCell5X* pBuf = Store[i];
		if (pBuf == nullptr)
			continue;
		auto& Bounds = pBuf->GetMapBounds();
		for (UINT k = 0; k < Bounds.size(); ++k)
		{
			const auto& Bound = Bounds[k];
			if (!Bound.Have2Edges())
			{
				UINT ShInd0 = Bound.GetIndexSharp()[0];
				if (ShInd0 == UINT_MAX)
					continue;// internal error
				Adjust(k, ShInd0, *pBuf, BColumnArr, Body5X, BigCells, LinesDCEL);
			}
		}
	}
	// LinesDCEL finished
	RRopes* pNewRopes = new RRopes;
	if (!pNewRopes->CreateFromLines(LinesDCEL))
	{
		delete pNewRopes;
		pNewRopes = new RRopes;
	}
	return pNewRopes;
}

void GQuadRenderDX5X::TransModifArraysCh(GBodyRender & QinRender)
{
	GQuadRenderDX5X* pinRender = dynamic_cast<GQuadRenderDX5X*>(&QinRender);
	if (pinRender == nullptr)
		return;
	GQuadRenderDX5X& inRender = *pinRender;
	TransModifArraysCh(*inRender.pBigCells);
	pinRender->pRopes->Clear();
	pRopes->Clear();
	return;
}

int GQuadRenderDX5X::TransModifArraysCh(TAArray & inArray)
{
	DXBigCell5X **inStore = inArray.GetStor();
	DXBigCell5X **Store = pBigCells->GetStor();
	int count = 0;
	for (int i = 0; i < RowsNum * ColsNum * LevelsNum; ++i)
	{
		DXBigCell5X *pBuf = inStore[i];
		if (pBuf == nullptr)
			continue;

		++count;
		inStore[i] = Store[i];
		Store[i] = pBuf;
	}
	return count;
}

const class BPoint GQuadRenderDX5X::RayCasting(const class BPoint & Pi, const class BPoint & Vi, double MinW, class BPoint &N) const
{ 
	BPoint p0(0), p1(0), p2(0);// vertexes of the triangle containing given point
	return RayCasting(Pi, Vi, MinW, N, p0, p1, p2);
}

BPoint GQuadRenderDX5X::RayCasting(const BPoint& Pi, const BPoint& Vi, double MinW, BPoint& N, BPoint& v0, BPoint& v1, BPoint& v2) const
{
	// Pi and Vi are in stock CS i.e. take in consideration stock position but ignores MillTurn rotation and BodyRot
	BMatrPair CMatr;
	if (NCMComData::IsTurnEnabled())
		CMatr = BodyRot.Or() * BMatr().RotX(0., 0., 0., 90.);
	else
		CMatr = BodyRot;
	BPoint P = Pi * CMatr.Inv();
	BPoint V = Vi * CMatr.Inv();
	// Find the intersection point nearest to P in the V direction
	// N - external normal in P (from Stl)
	// Vector V must have length = 1 ???
	double MinT = 1.e12;
	RayMarch RMarch(this, P, V);
	BMatrPair CorrMatrInv;
	if (NCMComData::IsTurnEnabled())
	{
		CorrMatrInv = BMatr().RotX(0., 0., 0., 90.);// Gabar and ijk indexes are in roteted CS but pTriRender is in correct CS;
		P *= CorrMatrInv.Or();
		V *= CorrMatrInv.Or();
	}
	else
		CorrMatrInv.SetE();
	int i, j, k;
	while (RMarch.GetNextBigCell(i, j, k))
	{
		DXBigCell5X* pBigCell = pBigCells->Get(i, j, k);
		if (!pBigCell)
			continue;
		RTriRandMesh* pTriRender = &pBigCell->GetTriangleArea();
		if (!pTriRender)
			continue;
		BPoint CurN;
		double t = pTriRender->RayCasting(P, V, MinW, CurN, v0, v1, v2);
		if (t < MinT)
		{
			MinT = t;
			N = CurN;
			break;
		}
	}
	if (MinT >= 1.e6)
		return 	BPoint(0., 0., 0., -1.);

	N = N * CMatr.Or();
	if (V * N > 0.)
		N = N * (-1.);
	N = N * BodyRot.Or() * CorrMatrInv.Inv();
	return (V * MinT + P) * BodyRot.Or() * CorrMatrInv.Inv();
}

MQuadBody::Window GQuadRenderDX5X::CalcWindow(const BBox & Bounds) const
{
	if (Bounds.Zmin > Gabar.Zmax ||
		Bounds.Zmax < Gabar.Zmin ||
		Bounds.Ymin > Gabar.Ymax ||
		Bounds.Ymax < Gabar.Ymin ||
		Bounds.Xmin > Gabar.Xmax ||
		Bounds.Xmax < Gabar.Xmin)
	{
		return MQuadBody::Window();
	}

	int xS, xE, zS, yS, yE, zE;
	GetCellIndexes(Bounds.Xmin, Bounds.Ymin, Bounds.Zmin, xS, yS, zS);
	GetCellIndexes(Bounds.Xmax, Bounds.Ymax, Bounds.Zmax, xE, yE, zE);
	MQuadBody::Window Win;
	Win.Set(xS, xE + 1, yS, yE + 1, zS, zE + 1);
	return Win;
}

void GQuadRenderDX5X::GetCellIndexes(double x, double y, double z, int & ix, int & iy, int & iz) const
{
	double Xstep = Gabar.dX() / ColsNum;
	ix = min(ColsNum - 1, max(0, int((x - Gabar.GetMinPoint().GetX()) / Xstep)));
	double Ystep = Gabar.dY() / RowsNum;
	iy = min(RowsNum - 1, max(0, int((y - Gabar.GetMinPoint().GetY()) / Ystep)));
	double Zstep = Gabar.dZ() / LevelsNum;
	iz = min(LevelsNum - 1, max(0, int((z - Gabar.GetMinPoint().GetZ()) / Zstep)));
}

int GQuadRenderDX5X::GetGlobSize(void) const
{
	return 0;// pTrianglArea->GetGlobSize();
}


int GQuadRenderDX5X::FillAllArraysCh(MQuadBody & Body)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();

	if(!Proper(Body))
		return -1;
	//if(!((DXSubModel5X &)Body).Check())
	//	Beep(1000, 100);

	FillPartRenders(Body, true);

	return 0;
}

int GQuadRenderDX5X::FillModifArrays(MQuadBody & Tree)
{
	std::vector<GQuadRenderDX5X *> Renders;
	std::vector<MQuadBody *> Bodies;

	GQuadRenderDX5X *pCurRend = this;
	for (MQuadBody *pCur = &Tree; pCur != nullptr && pCurRend != nullptr; pCur = static_cast<MQuadBody *>(pCur->GetNextMBody()), pCurRend = static_cast<GQuadRenderDX5X *>(pCurRend->GetNextRender()))
	{
		Bodies.push_back(pCur);
		Renders.push_back(pCurRend);
	}

	int NBodies = int(Bodies.size());
	MQuadBody **Array = Bodies.data();
	GQuadRenderDX5X **ArrayR = Renders.data();
	for (int i = 0; i < NBodies; ++i)
		ArrayR[i]->FillModifArraysCh(*(Array[i]));

	return 0;
}

int GQuadRenderDX5X::FillModifArraysCh(MQuadBody & Tree)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	if (!Proper(Tree))
		return -1;

	FillPartRenders(Tree, false);

	return 0;
}

bool GQuadRenderDX5X::FindSegments(SnapRopes& oSegments) const
{
	if (!MFOREST.GetIDProc()->GetStockEdgesDX())
		return false;
	return pRopes->GetSegments(oSegments);

}

bool GQuadRenderDX5X::MakeEnvelopeCont(std::list<BClrContour>& Contours, TurnGenMode TGMode) const
{
	if (TGMode == TGM_NO)
		return true;

	BDCEL_BSP DCEL;
	GLfloat* pCn = MakeDCEL(DCEL);
	bool res = BRotSolid::MakeEnvConts(DCEL, Contours, TGMode);
	delete[] pCn;
	return res && !Contours.empty();
}

void GQuadRenderDX5X::TAArray::SetSizes(int nX, int nY, int nZ)
{
	B3DArray<DXBigCell5X *>::SetSizes(nX, nY, nZ);
	DXBigCell5X **Stor = GetStor();
	for (int i = 0; i < nX * nY * nZ; ++i)
	{
		Stor[i] = nullptr;
	}
}

void GQuadRenderDX5X::TAArray::ResetChanged(const MQuadBody::Window &ModifWin, const MCubeMask &ModifiedCells)
{
	for (int B0 = ModifWin.ixStart; B0 < ModifWin.ixEnd; ++B0)
	{
		for (int B1 = ModifWin.iyStart; B1 < ModifWin.iyEnd; ++B1)
		{
			for (int B2 = ModifWin.izStart; B2 < ModifWin.izEnd; ++B2)
			{
				if (ModifiedCells.GetBit(B0, B1, B2))
				{
					DXBigCell5X *&Rend = Get(B0, B1, B2);
					delete Rend;
					Rend = new DXBigCell5X;
				}
			}
		}
	}
}

void GQuadRenderDX5X::TAArray::ClearAll(int nX, int nY, int nZ)
{
	DXBigCell5X **Stor = GetStor();
	for (int i = 0; i < nX * nY * nZ; ++i)
	{
		delete Stor[i];
		Stor[i] = nullptr;
	}
}

int GQuadRenderDX5X::TAArray::ClearEmpty(int nX, int nY, int nZ)
{
	DXBigCell5X **Stor = GetStor();
	int count = 0;
	for (int i = 0; i < nX * nY * nZ; ++i)
	{
		DXBigCell5X *&Rend = Stor[i];
		if (Rend)
		{
			if (Rend->GetTriangleArea().GetTriNum() == 0)
			{
				delete Rend;
				Rend = nullptr;
			}
			else
				++count;
		}
	}
	return count;
}

UINT64 GQuadRenderDX5X::GetHASH(const GBodyRender* pEnd) const
{
	pEnd;
	UINT64 HASH = 0;
	for (int i = 0; i < pBigCells->GetSize(); ++i)
	{
		DXBigCell5X* ptr = pBigCells->GetStor()[i];
		HASH ^= UINT64(ptr);
	}
	return HASH;
}

float* GQuadRenderDX5X::MakeDCEL(BDCEL_BSP& DCEL) const
{
	__int32 GlobTriNum = 0;
	for (int i = 0; i < GetAreasNum(); ++i)
	{
		const RTriRandMesh* pTrianglArea = GetArea(i);
		if (!pTrianglArea)
			continue;
		GlobTriNum += pTrianglArea->GetTriNum();
	}
	GLfloat* pStor = new GLfloat[GlobTriNum * 3 * DCEL.GetSVS() + 1];
	pStor[0] = GLfloat(GlobTriNum);
	auto fbuf = pStor + 1;
	for (int i = 0; i < GetAreasNum(); ++i)
	{
		const RTriRandMesh* pTrianglArea = GetArea(i);
		if (!pTrianglArea)
			continue;
		size_t TriNum = pTrianglArea->GetTriNum();
		if (TriNum <= 0)
			return 0;

		for (int k = 0; k < TriNum; ++k)
		{
			BPoint Pt[3], n;
			pTrianglArea->ExtractTriangle(k, Pt[0], Pt[1], Pt[2], n);
			for (int j = 0; j < 3; ++j)
			{
				fbuf[j * DCEL.GetSVS() + 0] = GLfloat(-n.GetX());// Normal// '-' - added 2.11.18
				fbuf[j * DCEL.GetSVS() + 1] = GLfloat(-n.GetY());// Normal
				fbuf[j * DCEL.GetSVS() + 2] = GLfloat(-n.GetZ());// Normal
				const BPoint& P = Pt[j];
				fbuf[j * DCEL.GetSVS() + 3] = GLfloat(P.GetX());// Vertex
				fbuf[j * DCEL.GetSVS() + 4] = GLfloat(P.GetY());// Vertex
				fbuf[j * DCEL.GetSVS() + 5] = GLfloat(P.GetZ());// Vertex
				if (DCEL.GetSVS() > 6)
					fbuf[j * DCEL.GetSVS() + 6] = 0.;//color
			}
			fbuf += 3 * DCEL.GetSVS();
		}
	}
	DCEL.MakeFromStockTri(pStor);
	return pStor;
}

int GQuadRenderDX5X::GetAreasNum() const
{
	return RowsNum * ColsNum * LevelsNum;
}

RTriRandMesh *GQuadRenderDX5X::GetArea(int Ind)
{
	DXBigCell5X *pBuf = pBigCells->GetStor()[Ind];
	if (!pBuf)
		return nullptr;
	return &pBuf->GetTriangleArea();
}

const RTriRandMesh *GQuadRenderDX5X::GetArea(int Ind) const
{
	DXBigCell5X *pBuf = pBigCells->GetStor()[Ind];
	if (!pBuf)
		return nullptr;
	return &pBuf->GetTriangleArea();
}

RTriRandMesh * GQuadRenderDX5X::GetBigCellMesh(int i, int j, int k) const
{
	DXBigCell5X * pBigCell = pBigCells->Get(i, j, k);
	if (!pBigCell)
		return nullptr;
	pBigCell->GetSharpEdges().clear();// to prevent problems after Open4Read
	auto pMesh = &pBigCell->GetTriangleArea();
	size_t PtsNum;
	pMesh->Open4Read(PtsNum);
	return pMesh;
}

bool GQuadRenderDX5X::Proper(const MQuadBody & Tree)
{
	return (ColsNum == Tree.ColsNum && RowsNum == Tree.RowsNum);
}

int GQuadRenderDX5X::GetWholeCellsNum() { return WholeCellsNum; }
void GQuadRenderDX5X::SetWholeCellsNum(int Val) { WholeCellsNum = Val; }
double GQuadRenderDX5X::GetSmoothAngle(void) { return SmoothAngle; }
void GQuadRenderDX5X::SetSmoothAngle(double Angle) { SmoothAngle = Angle; }

GBodyRender* GQuadRenderDX5X::MakeCopy() const
{
	GQuadRenderDX5X* pRet = new GQuadRenderDX5X(*this);
	return pRet;
}

void GQuadRenderDX5X::DoZProc(int B0, int B1, DXMaps& BColumnArr, const DXSubModel5X& Body5X, TAArray& NewBigCells, bool All)
{
	const MCubeMask& ModifiedCells = static_cast<const MCubeMask&>(Body5X.GetModifiedCells());
	const MQuadBody::Window& ModifWin = Body5X.GetWindow();
	bool UseMC = BDebugState::Get().MarchCubes;
	const double StepX = Body5X.GetXstep();
	const double StepY = Body5X.GetYstep();
	const double StepZ = Body5X.GetZstep();
	const double DEX_DIST = UseMC ? 1.e-5 : 0;
	DXBigCell5X::GroupData::StepX = StepX;
	DXBigCell5X::GroupData::StepY = StepY;
	DXBigCell5X::GroupData::StepZ = StepZ;
	DXBigCell5X::GroupData::MinPointShift = MinPointShift;
	DXBigCell5X::GroupData::pActMemory = &Body5X.GetMemory();
	// Store all dexel points to BCells as edges of cells
	const DXMemoryPtOnePart& Mem = Body5X.GetMemory(0);// PARALLEL
	const DXDirection5X& Direction = Body5X.GetDirection(0);
	const int max0 = Direction.GetN0(); // Последние не обрабатываем ???
	const int max1 = Direction.GetN1();
	const int Start0 = B0 * Body5X.GetParent().nBig[0];
	const int End0 = min(max0 - Start0, Body5X.GetParent().nBig[0]);
	DXMap& BigColumn = BColumnArr[B0 - ModifWin.ixStart][B1 - ModifWin.iyStart];
	BigColumn.Set(Body5X, B0, B1);
	BigColumn.InitFastZ();
	int Start1 = B1 * Body5X.GetParent().nBig[1];
	int End1 = min(max1 - Start1, Body5X.GetParent().nBig[1]);
	for (int I0 = 0; I0 <= End0; ++I0)// X index inside big cell (relative)
	{
		const bool Border0Down = (I0 == 0);
		const bool Border0Up = (I0 == End0);
		float Coord[3];
		Coord[0] = float((I0 + Start0) * StepX + MinPointShift.GetX());
		for (int I1 = 0; I1 <= End1; ++I1)// Y index inside big cell (relative)
		{
			const bool Border1Down = (I1 == 0);
			const bool Border1Up = (I1 == End1);
			Coord[1] = float((I1 + Start1) * StepY + MinPointShift.GetY());
			BigColumn.InitZFast(I0, I1);
			bool krout = false;
			for (DXMemID CurPoint = Direction.GetDexelStartPoint(I0 + Start0, I1 + Start1); // First point of dexel
				CurPoint != DXP_END; // dexel points ended
				CurPoint = Mem.GetAtNext(CurPoint))
			{
				const auto Z = Mem.GetAtZ(CurPoint); // point coord in the dexel direction
				if (Z < 0.)
					continue;
				Coord[2] = float(Z + MinPointShift.GetZ());
				int I2 = (int)((Z + DEX_DIST) / StepZ);
				unsigned int Degraded = 0;
				const double Diff = I2 * StepZ - Z;
				if (fabs(Diff) < DEX_DIST)
				{
					if (I2 > 0)
					{
						--I2;
						Degraded = 1;
					}
				}
				int B2 = I2 / Body5X.GetParent().nBig[2];
				for (int DegInd = 0; DegInd < (Degraded == 0 ? 1 : 2) && B2 < ModifWin.izEnd; ++DegInd, ++I2)
				{
					if (!All)
					{
						if (B2 < ModifWin.izStart || !ModifiedCells.GetBit(B0, B1, B2))
							continue;
					}
					DXCellInd Ind0(I0, I1, I2);
					DXBigCell5X*& pBigCell = NewBigCells.Get(B0, B1, B2);
					if (!pBigCell)
					{
						pBigCell = new DXBigCell5X;
					}
					unsigned int Edge;
					const DX_ID ID = Body5X.GetMemory().GetAtID(CurPoint); // получим идентификатор нормали точки
					BigColumn.SetEdge(Coord, ID.Elem, Edge, krout, Degraded * (2 - DegInd), pBigCell);
					if (!(Border0Up || Border1Up))
						BigColumn.AddEdgeZ(Ind0, 0, 8, Edge, pBigCell);
					if (!(Border0Down || Border1Down))
					{
						DXCellInd Ind1(Ind0.Ix, Ind0.Iy - 1, Ind0.Iz);
						BigColumn.AddEdgeZ(Ind1, -1, 11, Edge, pBigCell);
						if (!Border1Up)
							BigColumn.AddEdgeZ(Ind0, -1, 9, Edge, pBigCell);
						if (!Border0Up)
							BigColumn.AddEdgeZ(Ind1, 0, 10, Edge, pBigCell);
					}
					else if (!Border1Down)
						BigColumn.AddEdgeZ(DXCellInd(Ind0.Ix, Ind0.Iy - 1, Ind0.Iz), 0, 10, Edge, pBigCell);
					else if (!Border0Down)
						BigColumn.AddEdgeZ(Ind0, -1, 9, Edge, pBigCell);
					B2 = (I2 + 1) / Body5X.GetParent().nBig[2];
				}
				if (B2 >= ModifWin.izEnd)
					break;
				krout = !krout; // invert dexel line direction
			}
		}
	}
	BigColumn.FinishFast();
}

void GQuadRenderDX5X::DoYProc(int B1, DXMaps& BColumnArr, const DXSubModel5X& Body5X, TAArray& NewBigCells, bool All)
{
	const MCubeMask& ModifiedCells = static_cast<const MCubeMask&>(Body5X.GetModifiedCells());
	const MQuadBody::Window& ModifWin = Body5X.GetWindow();
	bool UseMC = BDebugState::Get().MarchCubes;
	const double StepX = Body5X.GetXstep();
	const double StepY = Body5X.GetYstep();
	const double StepZ = Body5X.GetZstep();
	const double DEX_DIST = UseMC ? 1.e-5 : 0;
	const DXMemoryPtOnePart& Mem = Body5X.GetMemory(1);
	const DXDirection5X& Direction = Body5X.GetDirection(1);
	const int max0 = Direction.GetN0(); // Последние не обрабатываем ???
	const int max1 = Direction.GetN1();
	const int Start1 = B1 * Body5X.GetParent().nBig[0];
	const int End1 = min(max1 - Start1, Body5X.GetParent().nBig[0]);
	for (int b2 = 0; b2 < ModifWin.iyEnd - ModifWin.iyStart; ++b2)
		BColumnArr[B1 - ModifWin.ixStart][b2].InitFastY();
	for (int I1 = 0; I1 <= End1; ++I1)// X index inside big cell (relative)
	{
		const bool Border1Down = (I1 == 0);
		const bool Border1Up = (I1 == End1);
		float Coord[3];
		Coord[0] = float((I1 + Start1) * StepX + MinPointShift.GetX());
		for (int b2 = 0; b2 < ModifWin.iyEnd - ModifWin.iyStart; ++b2)
			BColumnArr[B1 - ModifWin.ixStart][b2].InitYFast(I1);
		for (int B0 = ModifWin.izStart; B0 < ModifWin.izEnd; ++B0)// Z index
		{
			const int Start0 = B0 * Body5X.GetParent().nBig[2];
			const int End0 = min(max0 - Start0, Body5X.GetParent().nBig[2]);
			for (int I0 = 0; I0 <= End0; ++I0)// Z index inside big cell (relative)
			{
				const bool Border0Down = (I0 == 0);
				const bool Border0Up = (I0 == End0);
				Coord[2] = float((I0 + Start0) * StepZ + MinPointShift.GetZ());
				bool krout = false;
				for (DXMemID CurPoint = Direction.GetDexelStartPoint(I0 + Start0, I1 + Start1);//получим значение первой точки в декселе	
					CurPoint != DXP_END;  // dexel points ended
					CurPoint = Mem.GetAtNext(CurPoint))
				{
					auto Y = Mem.GetAtZ(CurPoint); // point coord in the dexel direction
					if (Y < 0.)
						continue;
					Coord[1] = float(Y + MinPointShift.GetY());
					int I2g = (int)((Y + DEX_DIST) / StepY);// absolute index
					unsigned int Degraded = 0;
					const double Diff = I2g * StepY - Y;
					if (fabs(Diff) < DEX_DIST)
					{
						if (I2g > 0)
						{
							--I2g;
							Degraded = 1;
						}
					}
					int B2 = I2g / Body5X.GetParent().nBig[1];
					for (int DegInd = 0; DegInd < (Degraded == 0 ? 1 : 2) && B2 < ModifWin.iyEnd; ++DegInd, ++I2g)
					{
						if (!All)
						{
							if (B2 < ModifWin.iyStart || !ModifiedCells.GetBit(B1, B2, B0))
								continue;
						}
						int I2 = I2g - B2 * Body5X.GetParent().nBig[1];// relative index
						DXCellInd Ind0(I1, I2, I0 + Start0);
						DXBigCell5X*& pBigCell = NewBigCells.Get(B1, B2, B0);
						if (!pBigCell)
						{
							pBigCell = new DXBigCell5X;
						}
						unsigned int Edge;
						const DX_ID ID = Body5X.GetMemory().GetAtID(CurPoint); // получим идентификатор нормали точки
						DXMap& BigColumn = BColumnArr[B1 - ModifWin.ixStart][B2 - ModifWin.iyStart];
						BigColumn.SetEdge(Coord, ID.Elem, Edge, krout, Degraded * (2 - DegInd), pBigCell);
						// Calls with smaller z should be processed before to ensure fast search coorect functionality
						DXCellInd Ind1(Ind0.Ix, Ind0.Iy, Ind0.Iz - 1);
						if (!(Border0Down || Border1Down))
							BigColumn.AddEdgeY(Ind1, -1, 5, Edge, pBigCell);
						if (!Border1Up && !Border0Down)
							BigColumn.AddEdgeY(Ind1, 0, 7, Edge, pBigCell);
						if (!(Border0Up || Border1Up))
							BigColumn.AddEdgeY(Ind0, 0, 3, Edge, pBigCell);
						if (!Border0Up && !Border1Down)
							BigColumn.AddEdgeY(Ind0, -1, 1, Edge, pBigCell);
						B2 = (I2g + 1) / Body5X.GetParent().nBig[1];
					}
					if (B2 >= ModifWin.iyEnd)
						break;
					krout = !krout; // invert dexel line direction
				}
			}
		}
	}
	for (int b2 = 0; b2 < ModifWin.iyEnd - ModifWin.iyStart; ++b2)
		BColumnArr[B1 - ModifWin.ixStart][b2].FinishFast();
}

void GQuadRenderDX5X::DoXProc(int B0, DXMaps& BColumnArr, const DXSubModel5X& Body5X, TAArray& NewBigCells, bool All)
{
	const MCubeMask& ModifiedCells = static_cast<const MCubeMask&>(Body5X.GetModifiedCells());
	const MQuadBody::Window& ModifWin = Body5X.GetWindow();
	bool UseMC = BDebugState::Get().MarchCubes;
	const double StepX = Body5X.GetXstep();
	const double StepY = Body5X.GetYstep();
	const double StepZ = Body5X.GetZstep();
	const double DEX_DIST = UseMC ? 1.e-5 : 0;
	const DXMemoryPtOnePart& Mem = Body5X.GetMemory(2);
	const DXDirection5X& Direction = Body5X.GetDirection(2);
	const int max0 = Direction.GetN0() - 1; // Последние не обрабатываем ???
	const int max1 = Direction.GetN1() - 1;
	const int Start0 = B0 * Body5X.GetParent().nBig[1];
	const int End0 = min(max0 - Start0, Body5X.GetParent().nBig[1]);
	for (int b2 = 0; b2 < ModifWin.ixEnd - ModifWin.ixStart; ++b2)
		BColumnArr[b2][B0 - ModifWin.iyStart].InitFastX();
	for (int I0 = 0; I0 <= End0; ++I0)// Y index inside big cell (relative)
	{
		const bool Border0Down = (I0 == 0);
		const bool Border0Up = (I0 == End0);
		float Coord[3];
		Coord[1] = float((I0 + Start0) * StepY + MinPointShift.GetY());
		for (int b2 = 0; b2 < ModifWin.ixEnd - ModifWin.ixStart; ++b2)
			BColumnArr[b2][B0 - ModifWin.iyStart].InitXFast(I0);
		for (int B1 = ModifWin.izStart; B1 < ModifWin.izEnd; ++B1)// Z index
		{
			const int Start1 = B1 * Body5X.GetParent().nBig[2];
			const int End1 = min(max1 - Start1, Body5X.GetParent().nBig[2]);
			for (int I1 = 0; I1 <= End1; ++I1)// Z index inside big cell (relative)
			{
				const bool Border1Down = (I1 == 0);
				const bool Border1Up = (I1 == End1);
				Coord[2] = float((I1 + Start1) * StepZ + MinPointShift.GetZ());
				bool krout = false;
				for (DXMemID CurPoint = Direction.GetDexelStartPoint(I0 + Start0, I1 + Start1);
					CurPoint != DXP_END;  // dexel points ended
					CurPoint = Mem.GetAtNext(CurPoint))
				{
					const auto X = Mem.GetAtZ(CurPoint); // point coord in the dexel direction
					if (X < 0.)
						continue;
					Coord[0] = float(X + MinPointShift.GetX());
					int I2g = (int)((X + DEX_DIST) / StepX);
					unsigned int Degraded = 0;
					const double Diff = I2g * StepX - X;
					if (fabs(Diff) < DEX_DIST)
					{
						if (I2g > 0)
						{
							--I2g;
							Degraded = 1;
						}
					}
					int B2 = I2g / Body5X.GetParent().nBig[0];
					for (int DegInd = 0; DegInd < (Degraded == 0 ? 1 : 2) && B2 < ModifWin.ixEnd; ++DegInd, ++I2g)
					{
						if (!All)
						{
							if (B2 < ModifWin.ixStart || !ModifiedCells.GetBit(B2, B0, B1))
								continue;
						}
						const int I2 = I2g - B2 * Body5X.GetParent().nBig[0];// relative index
						DXCellInd Ind0(I2, I0, I1 + Start1);
						DXBigCell5X*& pBigCell = NewBigCells.Get(B2, B0, B1);
						if (!pBigCell)
						{
							pBigCell = new DXBigCell5X;
						}
						unsigned int Edge;
						const DX_ID ID = Body5X.GetMemory().GetAtID(CurPoint); // получим идентификатор нормали точки
						DXMap& BigColumn = BColumnArr[B2 - ModifWin.ixStart][B0 - ModifWin.iyStart];
						//								BigColumn.NewInitXFast(I0);
						BigColumn.SetEdge(Coord, ID.Elem, Edge, krout, Degraded * (2 - DegInd), pBigCell);
						// Calls with smaller z should be processed before to ensure fast search coorect functionality
						DXCellInd Ind1(Ind0.Ix, Ind0.Iy, Ind0.Iz - 1);
						if (!(Border0Down || Border1Down))
							BigColumn.AddEdgeX(Ind1, -1, 6, Edge, pBigCell);
						if (!Border0Up && !Border1Down)
							BigColumn.AddEdgeX(Ind1, 0, 4, Edge, pBigCell);
						if (!(Border0Up || Border1Up))
							BigColumn.AddEdgeX(Ind0, 0, 0, Edge, pBigCell);
						if (!Border1Up && !Border0Down)
							BigColumn.AddEdgeX(Ind0, -1, 2, Edge, pBigCell);
						B2 = (I2g + 1) / Body5X.GetParent().nBig[0];
					}
					if (B2 >= ModifWin.ixEnd)
						break;
					krout = !krout; // invert dexel line direction
				}
			}
		}
	}
	for (int b2 = 0; b2 < ModifWin.ixEnd - ModifWin.ixStart; ++b2)
		BColumnArr[b2][B0 - ModifWin.iyStart].FinishFast();
}

GQuadRenderDX5X::RayMarch::RayMarch(const GQuadRenderDX5X *pRender, const BPoint & P, const BPoint & V)
{
	CurInd[0] = -1;
	CurInd[1] = -1;
	CurInd[2] = -1;

	if (pRender == nullptr)
		return;

	BBox RGabar = pRender->GetGabar();
	BPoint N;
	if (RGabar.Contains(P))
		Start = P;
	else
		Start = RGabar.RayCasting(P, V, 1.e-6, N);
	if (!Start.IsPoint())
		return;

	MaxInd[0] = pRender->ColsNum - 1;
	MaxInd[1] = pRender->RowsNum - 1;
	MaxInd[2] = pRender->LevelsNum - 1;

	Size[0] = RGabar.dX() / pRender->ColsNum;
	Size[1] = RGabar.dY() / pRender->RowsNum;
	Size[2] = RGabar.dZ() / pRender->LevelsNum;;

	Start -= RGabar.GetMinPoint();
	for (int i = 0; i < 3; ++i)
	{
		CurInd[i] = int(Start[i] / Size[i]);
		CurInd[i] = min(CurInd[i], MaxInd[i]);
		Start[i] -= CurInd[i] * Size[i];
		Start[i] /= Size[i];
		Step[i] = V[i] * (1. / Size[i]);
	}
	Step.SetH(0.);
	Step *= 1. / sqrt(Step.D2());
}

bool GQuadRenderDX5X::RayMarch::GetNextBigCell(int & i, int & j, int & k)
{
	if(CurInd[0] < 0)
		return false;
	i = CurInd[0];
	j = CurInd[1];
	k = CurInd[2];

	double tMin = 2.;
	int iMin = -1;
	for (int l = 0; l < 3; ++l)
	{
		double Rest = Step[l] > 0. ? 1. - Start[l] : Start[l];
		double t = Rest / fmax(fabs(Step[l]), 0.5 * Rest);
		if (t < tMin)
		{
			tMin = t;
			iMin = l;
		}
	}
	if (iMin < 0)
		return false; // internal error
	Start += Step * tMin;
	Start[iMin] = Step[iMin] > 0. ? 0. : 1.;
	CurInd[iMin] += Step[iMin] > 0. ? 1 : -1;
	if (CurInd[iMin] < 0 || CurInd[iMin] > MaxInd[iMin])
		CurInd[0] = -1;
	return true;
}
