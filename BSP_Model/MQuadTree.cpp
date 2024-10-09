#include "stdafx.h"
#include "ppltasks.h"
#include "NCMComData.h"
#include "BPoint.h"
#include "BBox.h"
#include "MFace.h"
#include "MBSPNode.h"
#include "MBSPTree.h"
#include "MInsertFace.h"
#include "SOSphere.h"
#include "SOCyl.h"
#include "SOPlane.h"
#include "BMatr.h"
#include "math.h"
#include "Gl\GLu.h"
#include "ConstDef.h"
#include "MBodyParams.h"
#include "MQuadMask.h"
#include "MBSPInter.h"
#include "MForestSave.h"
#include "CopyMap.h"
#include "BDCEL.h"
#include "MLocStorage.h"
#include "MRay.h"
#include "QDivide.h"
#include "MBSPCell.h"
#include "MQuadTree.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

typedef MBSPNode *NodePtr;
typedef  MBSPNode **p2;
typedef  MBSPNode *p1;
using namespace concurrency;

MQuadTree::~MQuadTree(void)
{
	for (int ix = 0; ix < ColsNum; ++ix)
	{
		for (int iy = 0; iy < RowsNum; ++iy)
		{
			MBSPNode* pRoot = Array[ix][iy];
			if (MBSPNode::IsTerminal(pRoot))
				continue;
			MBSPTree::DeleteTree(pRoot);
		}
		delete[] Array[ix];
	}
	delete[] Array;
	for (int ix = 0; ix < ColsNum; ++ix)
	{
		for (int iy = 0; iy < RowsNum; ++iy)
		{
			MBSPNode* pRoot = AuxArray[ix][iy];
			if (MBSPNode::IsTerminal(pRoot))
				continue;
			MBSPTree::DeleteTree(pRoot);
		}
		delete[] AuxArray[ix];
	}
	delete[] AuxArray;
	if (LocStArr == nullptr)
		return;
	for (int ix = 0; ix < ColsNum; ++ix)
	{
		for (int iy = 0; iy < RowsNum; ++iy)
		{
			auto pLoc = LocStArr[ix][iy];
			if(pLoc != nullptr)
				pLoc->ClearAll();
			delete LocStArr[ix][iy];
		}
		delete[] LocStArr[ix];
	}
	delete[] LocStArr;
}

void MQuadTree::Init(bool GlobalStor)
{
	Array = new p2[ColsNum];
	AuxArray = new p2[ColsNum];
	LocStArr = new MLocStorage * *[ColsNum];
	for (int ix = 0; ix < ColsNum; ++ix)
	{
		Array[ix] = new p1[RowsNum];
		AuxArray[ix] = new p1[RowsNum];
		LocStArr[ix] = new MLocStorage * [RowsNum];
		for (int iy = 0; iy < RowsNum; ++iy)
		{
			Array[ix][iy] = (MBSPNode*)M_IN_PTR;
			AuxArray[ix][iy] = (MBSPNode*)M_IN_PTR;
			LocStArr[ix][iy] = GlobalStor ? nullptr : new MLocStorage;
		}
	}
}

MQuadTree::MQuadTree( const BPoint & Minp, const BPoint & Maxp, int Rowsn, int Colsn, BStockGrID Position, bool GlobalStor)
	: MQuadBody(Minp, Maxp, Rowsn, Colsn, Position)
{
	for (RowsNum = 1; RowsNum < Rowsn; RowsNum *= 2);
	for (ColsNum = 1; ColsNum < Colsn; ColsNum *= 2);
	Init(GlobalStor);
}

MQuadTree::MQuadTree(void) : MQuadBody(BStockGrID())
{
	Array = nullptr;
	AuxArray = nullptr;
	LocStArr = nullptr;
}

void MQuadTree::CreateFullCopy(MBody * pQt)
{
	MQuadBody::CreateFullCopy(pQt);
	MQuadTree* pQ = dynamic_cast<MQuadTree*>(pQt);
	if (pQ == nullptr)
		return;
	pQ->Array = new p2[ColsNum];
	pQ->AuxArray = new p2[ColsNum];
	pQ->LocStArr = new MLocStorage * *[ColsNum];
	BMatr M;
	CopyMap Map(M);
	for (int ix = 0; ix < ColsNum; ++ix)
	{
		pQ->Array[ix] = new p1[RowsNum];
		pQ->AuxArray[ix] = new p1[RowsNum];
		pQ->LocStArr[ix] = new MLocStorage * [RowsNum];
		for (int iy = 0; iy < RowsNum; ++iy)
		{
			ActLocStorage Act(*this, ix, iy);
			pQ->LocStArr[ix][iy] = new MLocStorage;
			pQ->Array[ix][iy] = MBSPTree::CopySubtreeFull(Array[ix][iy], Map);//LOC STOR+
			pQ->AuxArray[ix][iy] = (MBSPNode*)M_IN_PTR;
			*pQ->GetLocStorage(ix, iy) = *GetLocStorage(ix, iy);
		}
	}
}

MLocStorage* MQuadTree::GetLocStorage(int ix, int iy)
{
	return LocStArr == nullptr ? nullptr : LocStArr[ix][iy];
}

const MLocStorage* MQuadTree::GetLocStorageC(int ix, int iy) const
{
	return LocStArr == nullptr ? nullptr : LocStArr[ix][iy];
}

void MQuadTree::ResetList(M_PTR Value)
{
	if (pNextMBody)
		((MQuadTree*)pNextMBody)->ResetList(Value);
	Reset(Value);
}

MQuadTree * MQuadTree::CreateFullCopy(void)
{
	MQuadTree *pQ = new MQuadTree;
	CreateFullCopy(pQ);
	return pQ;
}

void MQuadTree::CrStandard(bool ToAux)
{
	if (pNextMBody)
		((MQuadTree*)pNextMBody)->CrStandard(ToAux);

	auto LocArray = ToAux ? AuxArray : Array;
	if (NCM_PROJECT.GetUseQuadTreeParallel())
	{
		std::vector<task<void>> tasks;
		for (int ix = Win.ixStart; ix < Win.ixEnd; ++ix)
			for (int iy = Win.iyStart; iy < Win.iyEnd; ++iy)
			{
				if (!MBSPNode::IsTerminal(LocArray[ix][iy]))
				{
					tasks.push_back(create_task([&, ix, iy] { Cell CurCell(this); ActLocStorage Act(*this, ix, iy);
					CrStandard1Cell(LocArray[ix][iy], CurCell.SetCur(ix, iy)); }));
				}
			}
		auto joinTask = when_all(begin(tasks), end(tasks));
		// Wait for the tasks to finish.
		joinTask.wait();
	}
	else
	{
		Cell CurCell(this);
		for (int ix = Win.ixStart; ix < Win.ixEnd; ++ix)
			for (int iy = Win.iyStart; iy < Win.iyEnd; ++iy)
			{
				if (!MBSPNode::IsTerminal(LocArray[ix][iy]))
				{
					ActLocStorage Act(*this, ix, iy);
					CrStandard1Cell(LocArray[ix][iy], CurCell.SetCur(ix, iy)); //LOC STOR+
				}
			}
	}
}

MQuadTree::Cell::~Cell()
{
}

MQuadTree::Cell::Cell(const MQuadTree *pT)
{
	pTree = pT;
	Xmin = pTree->MinPoint.GetX();
	Ymin = pTree->MinPoint.GetY();
	Zmin = pTree->MinPoint.GetZ();
	Xmax = Xmin + pTree->Xstep;
	Ymax = Ymin + pTree->Ystep;
	Zmax = pTree->MaxPoint.GetZ();
}

const MQuadTree::Cell& MQuadTree::Cell::SetCur(int ix, int iy)
{
	double Xmini = Xmin + ix * pTree->Xstep;
	double Ymini = Ymin + iy * pTree->Ystep;
	double Xmaxi = Xmini + pTree->Xstep;
	double Ymaxi = Ymini + pTree->Ystep;
	Zmax = pTree->MaxPoint.GetZ();
	Vertexes[0].Set(Xmini, Ymini, Zmin, 1.);
	Vertexes[1].Set(Xmaxi, Ymini, Zmin, 1.);
	Vertexes[2].Set(Xmaxi, Ymaxi, Zmin, 1.);
	Vertexes[3].Set(Xmini, Ymaxi, Zmin, 1.);
	Vertexes[4].Set(Xmini, Ymini, Zmax, 1.);
	Vertexes[5].Set(Xmaxi, Ymini, Zmax, 1.);
	Vertexes[6].Set(Xmaxi, Ymaxi, Zmax, 1.);
	Vertexes[7].Set(Xmini, Ymaxi, Zmax, 1.);
	return *this;
}

MBSPCell* MQuadTree::MakeBSPCell(int ix, int iy) const
{
	double Xmini = MinPoint.GetX() + ix * Xstep;
	double Ymini = MinPoint.GetY() + iy * Ystep;
	double Xmaxi = Xmini + Xstep;
	double Ymaxi = Ymini + Ystep;
	return new MBSPCell(ix, iy, Xmini, Xmaxi, Ymini, Ymaxi);
}

int MQuadTree::Cell::CrStandardFace(const MPlane& Plane, MFace* pNewFace) const
{
	//Creates section of this cell by the Plane
// returns 0 if face was succsesfuly created
//	+1 if the cell lies in the positive subspace
//	-1 if the cell lies in the negative subspace
//Calculate point signums
	bool signs[8];
	for(int i=0; i<8; ++i) 
		signs[i] = 0. < Plane.CalcSign(Vertexes[i]);
//Find intersection points
	static int Edges[12][4] = 
	{	{0,1,0,2}, // start point, end point, 1st face, 2nd face
		{1,2,0,3},
		{2,3,0,4},
		{3,0,0,5},
		{4,5,2,1},
		{5,6,3,1},
		{6,7,4,1},
		{7,4,5,1},
		{1,5,2,3},
		{2,6,3,4},
		{3,7,4,5},
		{0,4,5,2}
	};
	int Faces[6][2] = 
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};// Points numbers
	int IPoints[6][2] = 
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};// Faces numbers
	BPoint IP[6];
	int nIP;
	int ie;
	for(ie = 0, nIP = 0; ie < 12 && nIP < 6; ++ie)
	{
		if(signs[Edges[ie][0]] == signs[Edges[ie][1]])
			continue;
		Faces[Edges[ie][2]][Faces[Edges[ie][2]][0]<0 ? 0:1] = nIP;
		Faces[Edges[ie][3]][Faces[Edges[ie][3]][0]<0 ? 0:1] = nIP;
		IPoints[nIP][0] = Edges[ie][2];
		IPoints[nIP][1] = Edges[ie][3];
		Plane.IntersLine(Vertexes[Edges[ie][0]],
			Vertexes[Edges[ie][1]] - Vertexes[Edges[ie][0]],
			IP[nIP]);

		++nIP;
	}
//Create contour
	int cPn = 0; // Current point number
	int cFn = IPoints[0][0]; // Current face number
	if(nIP == 0)
		return signs[0] ? 1 : -1;
	pNewFace->SetPlane(Plane);
	pNewFace->SetArraySize(nIP);
	for(int k = 0; k < nIP ; ++k)
	{
		pNewFace->SetPointAt(k, IP[cPn]);
		cPn = (cPn == Faces[cFn][0]) ? Faces[cFn][1] : Faces[cFn][0];
		cFn = (cFn == IPoints[cPn][0]) ? IPoints[cPn][1] : IPoints[cPn][0];
	}
	pNewFace->Orient();// Need to be removed (if realize right orientation while creating)
	return 0;
}

void MQuadTree::SetMBit(int ix, int iy)
{
	static_cast<MQuadMask *>(pModifiedCells)->SetBit(ix, iy);
}

void MQuadTree::GetTransfData(MBSPForest::NTransfData & TransfData)
{
	for (int ix = Win.ixStart; ix < Win.ixEnd; ++ix)
		for (int iy = Win.iyStart; iy < Win.iyEnd; ++iy)
		{
			MBSPNode *pRoot = Array[ix][iy];
			if (MBSPNode::IsTerminal(pRoot))
				continue;
			ActLocStorage Act(*this, ix, iy);
			pRoot->GetTransfData(TransfData);//LOC STOR+
		}
}

void MQuadTree::Translate(const BMatr & M)
{
	MQuadBody::Translate(M);
	MBSPForest::NTransfData TransfData;
	GetTransfData(TransfData);
	MFOREST.TransformSelected(TransfData, M);
	for (int ix = 0; ix < ColsNum; ++ix)
		for (int iy = 0; iy < RowsNum; ++iy)
		{
			ActLocStorage Act(*this, ix, iy);
			if(MBSPForest::GetLocStorage() != nullptr)
				MBSPForest::GetLocStorage()->TransformPts(M);
		}
	if (NCMComData::IsTurnEnabled())
	{//Mill turn project mill body
		if (!M.HasNotRot())
		{
			for (int ix = 0; ix < ColsNum; ++ix)
				for (int iy = 0; iy < RowsNum / 2; ++iy)
				{
					auto Buf = Array[ix][iy];
					Array[ix][iy] = Array[ix][RowsNum - 1 - iy];
					Array[ix][RowsNum - 1 - iy] = Buf;
				}
		}
	}
}

void MQuadTree::MarkUsedFacesOrigCh(class BDynBitArray & UsedF, class BDynBitArray & UsedO)
{
	for(int ix = 0; ix < ColsNum ; ++ix)
		for(int iy = 0; iy < RowsNum ; ++iy)
		{
			MBSPNode *pRoot = Array[ix][iy];//LOC STOR-
			if(MBSPNode::IsTerminal(pRoot))
				continue;
			pRoot->MarkUsedFacesOrig(UsedF, UsedO);
		}
}

void MQuadTree::SetNewIndexes(int* newind)
{
	for (int ix = 0; ix < ColsNum; ++ix)
		for (int iy = 0; iy < RowsNum; ++iy)
		{
			LocStArr[ix][iy]->SetNewIndexes(newind);
		}
}

void MQuadTree::MarkUsedPoints(BDynBitArray& Used) const
{
	for (int ix = 0; ix < ColsNum; ++ix)
		for (int iy = 0; iy < RowsNum; ++iy)
		{
			LocStArr[ix][iy]->MarkUsedForestPoints(Used);
		}
}

void MQuadTree::MarkUsedOrigCh(class BDynBitArray & Used)
{
	for (int ix = 0; ix < ColsNum; ++ix)
		for (int iy = 0; iy < RowsNum; ++iy)
		{
			MBSPNode *pRoot = Array[ix][iy];
			if(MBSPNode::IsTerminal(pRoot))
				continue;
			pRoot->MarkUsedOrig(Used);
		}
}

int MQuadTree::StoreFace(MFace * pFace, ElemID ElID, cadrID CaID, bool ToAux) // Matr
{
	if(!pFace)
		return 0;
	const auto BArray = ToAux ? AuxArray : Array;
	RecalcID(*pFace, ElID);
	BPoint Min(0),Max(0);
	pFace->GetGabar(Min,Max);
	if(Min.GetZ() >= MaxPoint.GetZ() - DMIN)		return 0;
	if(Max.GetZ() <= MinPoint.GetZ() + DMIN)	return 0;
	if(Min.GetX() >= MaxPoint.GetX() - DMIN)		return 0;
	if(Max.GetX() <= MinPoint.GetX() + DMIN)	return 0;
	if(Min.GetY() >= MaxPoint.GetY() - DMIN)		return 0;
	if(Max.GetY() <= MinPoint.GetY() + DMIN)	return 0;
	MFace F1,F2;
	MPlane Plane = MFOREST.TempPlane;
	if(Max.GetZ() > MaxPoint.GetZ())
	{
		MFOREST.SetPoint(Plane.GetNormalN(),BPoint(0.,0.,-1.,0.));
		MFOREST.SetPoint(Plane.GetPointN(),MaxPoint);
		M_POSITION res = pFace->DivideByPlane(Plane,F1,F2);
		if(res == M_INT)
			pFace->ReplacePoints(F1);
	}
	if(Min.GetZ() < MinPoint.GetZ())
	{
		MFOREST.SetPoint(Plane.GetNormalN(),BPoint(0.,0.,1.,0.));
		MFOREST.SetPoint(Plane.GetPointN(),MinPoint);
		M_POSITION res = pFace->DivideByPlane(Plane,F1,F2);
		if(res == M_INT)
			pFace->ReplacePoints(F1);
	}
	pFace->GetGabar(Min,Max);
	if(Min.GetX() >= MaxPoint.GetX())		return 0;
	if(Max.GetX() <= MinPoint.GetX())		return 0;
	if(Min.GetY() >= MaxPoint.GetY())		return 0;
	if(Max.GetY() <= MinPoint.GetY())		return 0;

	BPoint MinV = Min - MinPoint;
	BPoint MaxV = Max - MinPoint;
	int ixMin = int(MinV.GetX() / Xstep);
	if (MinV.GetX() < 0.)
	{
		MFOREST.SetPoint(Plane.GetNormalN(), BPoint(1., 0., 0., 0.));
		MFOREST.SetPoint(Plane.GetPointN(), MinPoint);
		M_POSITION resD = pFace->DivideByPlane(Plane, F1, F2);
		if (resD == M_NEG) 
			return 0;
		if (resD == M_INT)
			pFace->ReplacePoints(F1);
		ixMin = 0;
	}
	int ixMax = int(MaxV.GetX() / Xstep);
	if (ixMax > ColsNum - 1)
	{
		MFOREST.SetPoint(Plane.GetNormalN(), BPoint(-1., 0., 0., 0.));
		MFOREST.SetPoint(Plane.GetPointN(), MaxPoint);
		M_POSITION res = pFace->DivideByPlane(Plane, F1, F2);
		if (res == M_NEG)
			return 0;
		if (res == M_INT)
			pFace->ReplacePoints(F1);
		ixMax = ColsNum - 1;
	}
	{// to remove iyMin warning 
		int iyMin = int(MinV.GetY() / Ystep);
		if (MinV.GetY() < 0.)
		{
			MFOREST.SetPoint(Plane.GetNormalN(), BPoint(0., 1., 0., 0.));
			MFOREST.SetPoint(Plane.GetPointN(), MinPoint);
			M_POSITION res = pFace->DivideByPlane(Plane, F1, F2);
			if (res == M_NEG)
				return 0;
			if (res == M_INT)
				pFace->ReplacePoints(F1);
			iyMin = 0;
		}
		int iyMax = int(MaxV.GetY() / Ystep);
		if (iyMax > RowsNum - 1)
		{
			MFOREST.SetPoint(Plane.GetNormalN(), BPoint(0., -1., 0., 0.));
			MFOREST.SetPoint(Plane.GetPointN(), MaxPoint);
			M_POSITION res = pFace->DivideByPlane(Plane, F1, F2);
			if (res == M_NEG)
				return 0;
			if (res == M_INT)
				pFace->ReplacePoints(F1);
			iyMax = RowsNum - 1;
		}
	}
	MFace *pXFace;
	bool XStop = false;
	for(int ix = ixMin; ix <= ixMax && !XStop; ++ix)
	{
		if(ix < ixMax)
		{
			MFOREST.SetPoint(Plane.GetNormalN(),BPoint(-1.,0.,0.,0.));
			MFOREST.SetPoint(Plane.GetPointN(),BPoint(MinPoint.GetX()+(ix+1)*Xstep,0.,0.,1.));
			M_POSITION res = pFace->DivideByPlane(Plane,F1,F2);
			if(res == M_INT)
			{
				pXFace = &F1;
				pFace->ReplacePoints(F2);
			}
			else
			{
				if(res == M_POS)
				{
					pXFace = pFace;
					XStop = true;
				}
				else
					continue; 
			}
		}
		else
			pXFace = pFace;

		pXFace->GetGabar(Min,Max);
		int iyMin = int((Min - MinPoint).GetY() / Ystep);
		if(iyMin < 0) iyMin = 0;
		int iyMax = int((Max - MinPoint).GetY() / Ystep);
		if(iyMax > RowsNum-1) iyMax = RowsNum - 1;
		MFOREST.SetPoint(Plane.GetNormalN(),BPoint(0.,-1.,0.,0.));
		MFace *pYFace;
		bool YStop = false;
		for(int iy = iyMin; iy <= iyMax && !YStop; ++iy)
		{
			MFace F11;
			if(iy < iyMax)
			{
				MFOREST.SetPoint(Plane.GetPointN(),BPoint(0.,MinPoint.GetY()+(iy+1)*Ystep,0.,1.));
				M_POSITION res = pXFace->DivideByPlane(Plane,F11,F2);
				if(res == M_INT)
				{
					pYFace = &F11;
					pXFace->ReplacePoints(F2);
				}
				else
					if(res == M_POS)
					{
						pYFace = pXFace;
						YStop = true;
					}
					else
						continue; 
			}
			else
				pYFace = pXFace;

			MFace YFace1(*pYFace);

			if (MBSPNode::IsTerminal(BArray[ix][iy]))
				BArray[ix][iy] = (MBSPNode*)M_OUT_PTR;
			MBSPNode *pRoot = new MBSPNode;
			pRoot->SetBFace(ElID);
			pRoot->SetCadrID(CaID);
			pRoot->SetFace(YFace1);
			pRoot->SetRight(BArray[ix][iy]);
			pRoot->SetLeft((MBSPNode *)M_IN_PTR);
			BArray[ix][iy] = pRoot;
			Win.Expand(ix, iy);
		}
	}
	return 0;
}

void MQuadTree::CreateTree(bool ToAux)
{
	if (NCM_PROJECT.GetUseQuadTreeParallel())
	{
		std::vector<task<void>> tasks;
		for (int ix = Win.ixStart; ix < Win.ixEnd; ++ix)
			for (int iy = Win.iyStart; iy < Win.iyEnd; ++iy)
			{
				tasks.push_back(create_task([&, ix, iy] { CreateTreeLoc(ix, iy, ToAux); }));
			}
		auto joinTask = when_all(begin(tasks), end(tasks));
		// Wait for the tasks to finish.
		joinTask.wait();
	}
	else
	{
		for (int ix = Win.ixStart; ix < Win.ixEnd; ++ix)
			for (int iy = Win.iyStart; iy < Win.iyEnd; ++iy)
			{
				CreateTreeLoc(ix, iy, ToAux);
			}
	}
}

void MQuadTree::CreateTreeLoc(int ix, int iy, bool ToAux)
{
	const auto BArray = ToAux ? AuxArray : Array;
	MBSPNode* pRoot = BArray[ix][iy];
	if (MBSPNode::IsTerminal(pRoot))
		return;
	BArray[ix][iy] = (MBSPNode*)M_NDEF_PTR;
	ActLocStorage Act(*this, ix, iy);
	while (!MBSPNode::IsTerminal(pRoot))
	{
		MInsertFace MIF((MBSPNode**)&BArray[ix][iy], pRoot->GetFaceN(), pRoot->GetBFaceN(), pRoot->GetCadrID());
		MIF.DoInsert();
		MBSPNode* pB = pRoot;
		pRoot = pRoot->GetRight();
		delete pB;
	}
}

void MQuadTree::SortFaces(const MQuadTree &Base)
{
	Base;
	for(int ix = Win.ixStart; ix < Win.ixEnd ; ++ix)
		for(int iy = Win.iyStart; iy < Win.iyEnd ; ++iy)
		{
			MBSPNode *pRoot = Array[ix][iy];
			if(MBSPNode::IsTerminal(pRoot))
				continue;
			// Invert faces list
			MBSPNode *pPrev = pRoot;
			for(MBSPNode *pNext = pPrev->GetRight(); !MBSPNode::IsTerminal(pNext);)
			{
				MBSPNode *pBuf = pNext->GetRight();
				pNext->SetRight(pPrev);
				pPrev = pNext;
				pNext = pBuf;
			}
			pRoot->SetRight(NULL);
			Array[ix][iy] = pPrev;	
		}
}

void MQuadTree::DrawFaces(void)
{
	glPushAttrib(GL_ENABLE_BIT);
	glColor4d(0.,1.,1.,BDebugState::Get().Transp);
	glDisable(GL_AUTO_NORMAL);
	glEnable(GL_BLEND);
	glEnable(GL_NORMALIZE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,0);
	glEnable(GL_LIGHTING) ; 
	glEnable(GL_LIGHT0) ;
	GLint viewport[4];
	GLdouble projMatrix[16],modelMatrix[16];
	glMatrixMode(GL_MODELVIEW);
	glGetDoublev(GL_MODELVIEW_MATRIX ,modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX ,projMatrix);
	glGetIntegerv(GL_VIEWPORT ,viewport);
	GLdouble sxb,sxe,syb,sye,szb,sze;
	
	gluUnProject(0,0,0,
		modelMatrix, projMatrix,viewport,&sxb,&syb,&szb); 
	gluUnProject(0,0,1,
		modelMatrix, projMatrix,viewport,&sxe,&sye,&sze); 
	BPoint VDir(sxe-sxb,sye-syb,sze-szb,0.); // View direction

	int Xstart, XstepT;
	if(VDir.GetX() > 0.)
	{
		Xstart = ColsNum - 1;
		XstepT = -1;
	}
	else
	{
		Xstart = 0;
		XstepT = 1;
	}

	int Ystart, YstepT;
	if(VDir.GetY() > 0.)
	{
		Ystart = RowsNum - 1;
		YstepT = -1;
	}
	else
	{
		Ystart = 0;
		YstepT = 1;
	}

	for(int yn = 0, iy = Ystart; yn < RowsNum; ++yn, iy += YstepT)
		for(int xn = 0, ix = Xstart; xn < ColsNum; ++xn, ix += XstepT)
		{
			MBSPNode *pRoot = Array[ix][iy];//LOC STOR+
			if (!MBSPNode::IsTerminal(pRoot))
			{
				ActLocStorage Act(*this, ix, iy);
				pRoot->DrawFaces(VDir);
			}
		}

	glPopAttrib();
}

bool MQuadTree::CheckNormals(void)
{
	for(int ix = Win.ixStart; ix < Win.ixEnd ; ++ix)
		for(int iy = Win.iyStart; iy < Win.iyEnd ; ++iy)
		{
			MBSPNode *pRoot = Array[ix][iy];
			if(MBSPNode::IsTerminal(pRoot))
				continue;
			if(!pRoot->CheckNormals())
				return false;
		}
	return true;
}

void MQuadTree::Reset(M_PTR Value)
{
	for(int ix = Win.ixStart; ix < Win.ixEnd ; ++ix)
		for(int iy = Win.iyStart; iy < Win.iyEnd ; ++iy)
			AuxArray[ix][iy] = (MBSPNode *)Value;
}

void MQuadTree::CopyMergeCh(MBSPMerge & Op , MBody & Inb)
{
	MQuadTree &In = (MQuadTree &)Inb;
	for(int ix = Win.ixStart; ix < Win.ixEnd ; ++ix)
		for(int iy = Win.iyStart; iy < Win.iyEnd ; ++iy)
		{
			if(Op.CopyMerge(Array[ix][iy], In.Array[ix][iy]) != UNCHANGED)//& T1CHANGED)
				SetMBit(ix, iy);
		}
	return;
}

void MQuadTree::SafeMergeCh(const MBSPMerge & Op)
{
	int ixS = Win.ixStart;
	int ixE = Win.ixEnd;
	int iyS = Win.iyStart;
	int iyE = Win.iyEnd;

	if (NCM_PROJECT.GetUseQuadTreeParallel())
	{
		std::vector<task<MERGERESULT>> tasks;
		for (int ix = ixS; ix < ixE; ++ix)
		{
			for (int iy = iyS; iy < iyE; ++iy)
			{
				tasks.push_back(create_task([&, ix, iy] { return SafeMergeLoc(Op, ix, iy); }));
			}
		}
		auto joinTask = when_all(begin(tasks), end(tasks));
		// Wait for the tasks to finish.
		joinTask.wait();

		// Fill changed bits
		auto it = tasks.cbegin();
		for (int ix = ixS; ix < ixE; ++ix)
			for (int iy = iyS; iy < iyE; ++iy)
			{
				if (it->get() & T1CHANGED)
					SetMBit(ix, iy);
				++it;
			}
	}
	else
	{
		for (int ix = ixS; ix < ixE; ++ix)
		{
			for (int iy = iyS; iy < iyE; ++iy)
			{
				auto Result = SafeMergeLoc(Op, ix, iy);
				if (Result & T1CHANGED)
					SetMBit(ix, iy);
			}
		}
	}
	return;
}

void MQuadTree::SafeMergeChWithCr(MBSPMerge& Op, const BBox& Bounds, bool Convex)
{
	if (!Win.IsDefined())
		return;

	const bool UseParallel = NCM_PROJECT.GetUseQuadTreeParallel();
	if (Bounds.Zmin <= MinPoint.GetZ())
	{
		SetWindow(Bounds);
		if (!Convex)
			CreateTree(true);
		EmptyCellsDet(true);
		RemoveWindow();
		if (UseParallel)
		{
			int yMin = Win.iyStart;
			int yMax = Win.iyEnd;
			std::vector<task<std::vector<MERGERESULT>>> tasks;
			for (int ix = Win.ixStart; ix < Win.ixEnd; ++ix)
			{
				tasks.push_back(create_task([&, ix, yMin, yMax]
					{
						std::vector<MERGERESULT> results;
						results.resize(yMax - yMin);
						for (int iy = yMin; iy < yMax; ++iy)
						{
							results[iy - yMin] = SafeMergeLoc(Op, ix, iy);
						}
						return results;
					}));
			}
			auto joinTask = when_all(begin(tasks), end(tasks));
			// Wait for the tasks to finish.
			joinTask.wait();

			// Fill changed bits
			auto it = tasks.cbegin();
			for (int ix = Win.ixStart; ix < Win.ixEnd; ++ix)
			{
				std::vector<MERGERESULT> results = it->get();
				for (int iy = Win.iyStart; iy < Win.iyEnd; ++iy)
				{
					if (results[iy - Win.iyStart] & T1CHANGED)
						SetMBit(ix, iy);
				}
				++it;
			}
		}
		else
		{
			for (int ix = Win.ixStart; ix < Win.ixEnd; ++ix)
				for (int iy = Win.iyStart; iy < Win.iyEnd; ++iy)
				{
					const auto Result = SafeMergeLoc(Op, ix, iy);
					if (Result & T1CHANGED)
						SetMBit(ix, iy);
				}
		}
	}
	else
	{
		if (UseParallel)
		{
			const int yMin = Win.iyStart;
			const int yMax = Win.iyEnd;
			std::vector<task<std::vector<MERGERESULT>>> tasks;
			for (int ix = Win.ixStart; ix < Win.ixEnd; ++ix)
			{
				tasks.push_back(create_task([&, ix, yMin, yMax, Convex]
					{
						std::vector<MERGERESULT> results;
						results.resize(yMax - yMin);
						for (int iy = yMin; iy < yMax; ++iy)
						{
							if (!Convex) 
								CreateTreeLoc(ix, iy, true);
							results[iy - yMin] = SafeMergeLoc(Op, ix, iy);
						}
						return results;
					}));
			}
			auto joinTask = when_all(begin(tasks), end(tasks));
			// Wait for the tasks to finish.
			joinTask.wait();

			// Fill changed bits
			auto it = tasks.cbegin();
			for (int ix = Win.ixStart; ix < Win.ixEnd; ++ix)
			{
				std::vector<MERGERESULT> results = it->get();
				for (int iy = Win.iyStart; iy < Win.iyEnd; ++iy)
				{
					if (results[iy - Win.iyStart] & T1CHANGED)
						SetMBit(ix, iy);
				}
				++it;
			}
		}
		else
		{
			for (int ix = Win.ixStart; ix < Win.ixEnd; ++ix)
				for (int iy = Win.iyStart; iy < Win.iyEnd; ++iy)
				{
					if (!Convex)
						CreateTreeLoc(ix, iy, true);
					const auto Result = SafeMergeLoc(Op, ix, iy);
					if (Result & T1CHANGED)
						SetMBit(ix, iy);
				}
		}
	}
}

MERGERESULT MQuadTree::SafeMergeLoc(const MBSPMerge& Op, int ix, int iy)
{
	ActLocStorage Act(*this, ix, iy);

	if (!MBSPNode::IsTerminal(AuxArray[ix][iy]))
	{
		Cell CurCell(this);
		CrStandard1Cell(AuxArray[ix][iy], CurCell.SetCur(ix, iy));
	}
//	MFOREST.DebugPrintSubtree(In.Array[ix][iy]);
	auto Res = Op.SafeMerge(Array[ix][iy], AuxArray[ix][iy]);
//	MFOREST.DebugPrintSubtree(Array[ix][iy]);
	AuxArray[ix][iy] = (MBSPNode*)M_IN_PTR;
	MBSPNode* pRoot = Array[ix][iy];
	if (!MBSPNode::IsTerminal(pRoot))
	{
		MLocStorage::CompressFaces(MFOREST.GetLocStorage(), pRoot);
		MLocStorage::CompressPoints(MFOREST.GetLocStorage());
	}
	return Res;
}

const MFace& MQuadTree::Cell::CrBoundFace(Sides Side, MFace& Face)
{
	Face.SetArraySize(4);
	BPoint n(0,0,-1,0);
	switch(Side)
	{
	case Zt:
		n.Set(0,0,-1,0);
		Face.SetPointAt(0,MFOREST.AddPoints(Vertexes[4]));
		Face.SetPointAt(1,MFOREST.AddPoints(Vertexes[5]));
		Face.SetPointAt(2,MFOREST.AddPoints(Vertexes[6]));
		Face.SetPointAt(3,MFOREST.AddPoints(Vertexes[7]));
		break;
	case Zb:
		n.Set(0,0,1,0);
		Face.SetPointAt(0,MFOREST.AddPoints(Vertexes[3]));
		Face.SetPointAt(1,MFOREST.AddPoints(Vertexes[2]));
		Face.SetPointAt(2,MFOREST.AddPoints(Vertexes[1]));
		Face.SetPointAt(3,MFOREST.AddPoints(Vertexes[0]));
		break;
	case Xt:
		n.Set(-1,0,0,0);
		Face.SetPointAt(0,MFOREST.AddPoints(Vertexes[1]));
		Face.SetPointAt(1,MFOREST.AddPoints(Vertexes[2]));
		Face.SetPointAt(2,MFOREST.AddPoints(Vertexes[6]));
		Face.SetPointAt(3,MFOREST.AddPoints(Vertexes[5]));
		break;
	case Xb:
		n.Set(1,0,0,0);
		Face.SetPointAt(0,MFOREST.AddPoints(Vertexes[4]));
		Face.SetPointAt(1,MFOREST.AddPoints(Vertexes[7]));
		Face.SetPointAt(2,MFOREST.AddPoints(Vertexes[3]));
		Face.SetPointAt(3,MFOREST.AddPoints(Vertexes[0]));
		break;
	case Yt:
		n.Set(0,-1,0,0);
		Face.SetPointAt(0,MFOREST.AddPoints(Vertexes[2]));
		Face.SetPointAt(1,MFOREST.AddPoints(Vertexes[3]));
		Face.SetPointAt(2,MFOREST.AddPoints(Vertexes[7]));
		Face.SetPointAt(3,MFOREST.AddPoints(Vertexes[6]));
		break;
	case Yb:
		n.Set(0,1,0,0);
		Face.SetPointAt(0,MFOREST.AddPoints(Vertexes[0]));
		Face.SetPointAt(1,MFOREST.AddPoints(Vertexes[1]));
		Face.SetPointAt(2,MFOREST.AddPoints(Vertexes[5]));
		Face.SetPointAt(3,MFOREST.AddPoints(Vertexes[4]));
		break;
	}
	Face.SetPlane(MPlane(MFOREST.AddPoints(n), Face.GetIndPoint(0)));
	return Face;
}

//void MQuadTree::ClearCh(void)
//{
//	for(int ix = Win.ixStart; ix < Win.ixEnd ; ++ix)
//		for(int iy = Win.iyStart; iy < Win.iyEnd ; ++iy)
//		{
//			MBSPTree::DeleteTree(Array[ix][iy]);
//			Array[ix][iy] = (MBSPNode *)M_IN_PTR;
//		}
//}

void MQuadTree::CreateCh(const BBox& Bounds, bool Convex, bool ToAux)
{// Do all necessary after "storing" all faces
	SetWindow(Bounds);
	if(!Convex)
		CreateTree(ToAux);
	if(Bounds.Zmin <= MinPoint.GetZ())
		EmptyCellsDet(ToAux);
}

void MQuadTree::EmptyCellsDet(bool ToAux)
{// To determine status (IN/OUT) of the empty cells
	const auto BArray = ToAux ? AuxArray : Array;
	//Find first non empty cell
	bool found = false;
	int ixb = -1, iyb = -1;
	for(int ix = Win.ixStart; ix < Win.ixEnd && !found; ++ix)
		for(int iy = Win.iyStart; iy < Win.iyEnd && !found; ++iy)
		{
			if(!MBSPNode::IsTerminal(BArray[ix][iy]))//LOC STOR-
			{
				found = true;
				ixb = ix;
				iyb = iy;
			}
		}
	if(!found)
		return;
	int i;
	for(i = ixb + 1; i < Win.ixEnd; ++i)// row end processing
	{
		if(MBSPNode::IsTerminal(BArray[i][iyb]))
			DetCell(i, iyb, -1, 0, ToAux);
	}
	for(i = ixb - 1; i >= Win.ixStart; --i)// row start processing
	{
		if(MBSPNode::IsTerminal(BArray[i][iyb]))
			DetCell(i, iyb, +1, 0, ToAux);
	}
	int j;
	for(i = Win.ixStart; i < Win.ixEnd ; ++i)//Cols end processing
	{
		for(j = iyb + 1; j < Win.iyEnd; ++j)
		{
			if(MBSPNode::IsTerminal(BArray[i][j]))
				DetCell(i, j, 0, -1, ToAux);
		}
	}
	for(i = Win.ixStart; i < Win.ixEnd ; ++i)//Cols end processing
	{
		for(j = iyb - 1; j >= Win.iyStart; --j)
		{
			if(MBSPNode::IsTerminal(BArray[i][j]))
				DetCell(i, j, 0, +1, ToAux);
		}
	}

}

void MQuadTree::DetCell(int ix, int iy, int dx, int dy, bool ToAux)
{
	const auto BArray = ToAux ? AuxArray : Array;
	// Determine ix,iy cell status 
	// base cell has indexes ix+dx,iy+dy
	MBSPNode *pBase = BArray[ix+dx][iy+dy];//LOC STOR+
	if(MBSPNode::IsTerminal(pBase))
	{
		BArray[ix][iy] = pBase;
		return;
	}
	BPoint TestP = MinPoint + 
		BPoint((ix + 0.5) * Xstep,(iy + 0.5) * Ystep, 0., 0.);
	ActLocStorage Act(*this, ix + dx, iy + dy);
	BArray[ix][iy] = pBase->Contains(TestP) ?
		(MBSPNode *)M_IN_PTR :
		(MBSPNode *)M_OUT_PTR;
}

//MBodyParams MQuadTree::GetCellParams(int ix, int iy)
//{
//	MBodyParams Par;
//	MBSPNode *pRoot = Array[ix][iy];//LOC STOR
//	if(MBSPNode::IsTerminal(pRoot))
//	{
//		if(pRoot == (MBSPNode *)M_IN_PTR)
//		{
//			double Zstep = (MaxPoint - MinPoint).GetZ();
//
//			Par.Volume = Xstep * Ystep * Zstep;
//
//			Par.SurfArea = 2 * Xstep * Ystep;
//			if(ix == 0)
//				Par.SurfArea += Zstep * Ystep;
//			if(ix == ColsNum-1)
//				Par.SurfArea += Zstep * Ystep;
//			if(iy == 0)
//				Par.SurfArea += Zstep * Xstep;
//			if(iy == RowsNum-1)
//				Par.SurfArea += Zstep * Xstep;
//
//			Par.Bounds.Expand(MinPoint + BPoint( ix * Xstep, iy * Ystep, 0., 0.));
//			Par.Bounds.Expand(MinPoint + BPoint( (ix + 1) * Xstep, (iy + 1) * Ystep, Zstep, 0.));
//
//			return Par;
//		}
//		return Par;
//	}
//	// Non empty cell
//	int Mask = Zt | Zb;
//	int NodesNum = 2;
//
//	if(ix == 0){ Mask |= Xb; NodesNum++; }
//	if(ix == ColsNum-1){ Mask |= Xt;  NodesNum++; }
//	if(iy == 0){ Mask |= Yb; NodesNum++; }
//	if(iy == RowsNum-1){ Mask |= Yt; NodesNum++; }
//
//	for( int i = 0; i < NodesNum ; ++i)
//	{
//		MBSPNode *pTemp = new MBSPNode;
//		pTemp->SetLeft(pRoot);
//		pRoot = pTemp;
//		pRoot->SetRight((MBSPNode *)M_OUT_PTR);
//		pRoot->SetBFace(MElemIDProc::ID_NULL);
//	}
//
//	Cell C(this);
//	C.SetCur(ix, iy);
//	MBSPNode *pTemp = pRoot;
//	MFace TempFace;
//	if( Mask & Zt )
//	{
//		pTemp->SetFace(C.CrBoundFace(Zt, TempFace));
//		pTemp = pTemp->GetLeft();
//	}
//	if( Mask & Zb )
//	{
//		pTemp->SetFace(C.CrBoundFace(Zb, TempFace));
//		pTemp = pTemp->GetLeft();
//	}
//	if( Mask & Xt )
//	{
//		pTemp->SetFace(C.CrBoundFace(Xt, TempFace));
//		pTemp = pTemp->GetLeft();
//	}
//	if( Mask & Xb )
//	{
//		pTemp->SetFace(C.CrBoundFace(Xb, TempFace));
//		pTemp = pTemp->GetLeft();
//	}
//	if( Mask & Yt )
//	{
//		pTemp->SetFace(C.CrBoundFace(Yt, TempFace));
//		pTemp = pTemp->GetLeft();
//	}
//	if( Mask & Yb )
//	{
//		pTemp->SetFace(C.CrBoundFace(Yb, TempFace));
//		pTemp = pTemp->GetLeft();
//	}
//	
//	Par += pRoot->GetParams();
//
//	for( int i = 0; i < NodesNum ; ++i)
//	{
//		MBSPNode *pT = pRoot->GetLeft();
//		delete pRoot;
//		pRoot = pT;
//	}
//	
//	return Par;
//}
void MQuadTree::Save(MForestSave &SaveData)
{
	MQuadBody::Save(SaveData);
	CFile *f = SaveData.File;
	for (int i = 0; i < ColsNum; i++)
		for (int j = 0; j < RowsNum; j++)
		{
			MBSPNode *pRoot = Array[i][j];//LOC STOR+
			ActLocStorage Act(*this, i, j);
			f->Write(&pRoot, sizeof(pRoot));
			if (!MBSPNode::IsTerminal(pRoot))
				pRoot->Save(SaveData);
			// save used points
			int PointsSize = int(SaveData.PointSaveMap.NewInd2OldIndLoc.size());
			f->Write(&PointsSize, sizeof(PointsSize));
			for (int k = 0; k < PointsSize; ++k)
			{
				BPoint Tmp(*MFOREST.GetPoint(int(SaveData.PointSaveMap.NewInd2OldIndLoc[k])) * SaveData.Matr);
				f->Write(&Tmp, sizeof(Tmp));
			}
			SaveData.PointSaveMap.ResetLoc();
		}
	// save used points
	int PointsSize = int(SaveData.PointSaveMap.NewInd2OldInd.size());
	f->Write(&PointsSize, sizeof(PointsSize));
	for (int i = 0; i < PointsSize; ++i)
	{
		BPoint Tmp(*MFOREST.GetPoint(int(SaveData.PointSaveMap.NewInd2OldInd[i])) * SaveData.Matr);
		f->Write(&Tmp, sizeof(Tmp));
	}
	// save used orig
	int OrigSize = int(SaveData.OrigSaveMap.NewInd2OldInd.size());
	f->Write(&OrigSize, sizeof(OrigSize));
	for (int i = 0; i < OrigSize; ++i)
	{
		auto *pSurf = MFOREST.GetOrigSurf(int(SaveData.OrigSaveMap.NewInd2OldInd[i]));
		pSurf->Save(SaveData.File, SaveData.Matr);
	}
}
void MQuadTree::Load(CFile * f)
{
	MQuadBody::Load(f);
	Init(false);
	ResetpModifiedCells();
	RemoveWindow();
	for (int i = 0; i < ColsNum; i++)
		for (int j = 0; j < RowsNum; j++)
		{
			ActLocStorage Act(*this, i, j);
			MBSPNode *pNode;
			f->Read(&pNode, sizeof(pNode));
			if (!MBSPNode::IsTerminal(pNode))
			{
				pNode = new MBSPNode;
				pNode->Load(f);
			}
			Array[i][j] = pNode;//LOC STOR+
			// load used points
			int LocPointsSize;
			f->Read(&LocPointsSize, sizeof(LocPointsSize));
			auto pStor = MBSPForest::GetLocStorage();
			if (pStor == nullptr)
				continue;
			pStor->m_PointArray.resize(LocPointsSize);
			for (int k = 0; k < LocPointsSize; ++k)
			{
				BPoint Point;
				f->Read(&pStor->m_PointArray.at(k), sizeof(Point));
			}
		}
	// load used points
	int PointsSize;
	f->Read(&PointsSize, sizeof(PointsSize));
	int *In2Out = new int[PointsSize];

	for (int i = 0; i < PointsSize; ++i)
	{
		BPoint Point;
		f->Read(&Point, sizeof(Point));
		In2Out[i] = MFOREST.AddPoints(Point);
	}
	// replace points indexes
	for (int i = 0; i < ColsNum; i++)
		for (int j = 0; j < RowsNum; j++)
		{
			ActLocStorage Act(*this, i, j);
			MBSPNode *pNode = Array[i][j];//LOC STOR+
			if (!MBSPNode::IsTerminal(pNode))
				pNode->ReplPInd(In2Out);
		}
	delete[] In2Out;
	// load used surfaces
	int OrigSize;
	f->Read(&OrigSize, sizeof(OrigSize));
	In2Out = new int[OrigSize];

	for (int i = 0; i < OrigSize; ++i)
	{
		SOrigSurf *pSurf = SOrigSurf::StLoad(f);
		In2Out[i] = MFOREST.AddOrigSurf(pSurf);
	}
	// replace surfaces indexes
	for (int i = 0; i < ColsNum; i++)
		for (int j = 0; j < RowsNum; j++)
		{
			MBSPNode *pNode = Array[i][j];
			if (!MBSPNode::IsTerminal(pNode))
				pNode->ReplOInd(In2Out);
		}
	delete[] In2Out;
}

MQuadTree * MQuadTree::InvertCh(bool ToAux)
{
	const auto BArray = ToAux ? AuxArray : Array;
	for(int i = 0; i < ColsNum; i++)
		for(int j = 0; j < RowsNum; j++)
		{
			BArray[i][j] = MBSPTree::Invert(BArray[i][j]);//LOC STOR-
		}

	return this;
}

//MBSPNode * MQuadTree::CrSection(const MPlane & Plane, int &n)
//{
//	n = 0;// Number of the nodes in the result list
//	MBSPNode *pRes = NULL;// result list
//	MBSPNode **ppLastNode = &pRes;
//	Cell CurCell(this);
//	for(int i = 0; i < ColsNum; i++)
//		for(int j = 0; j < RowsNum; j++)
//		{
//			MBSPNode *pRoot = Array[i][j];//LOC STOR
//			if(MBSPNode::IsTerminal(pRoot) && (pRoot != (MBSPNode *)M_IN_PTR))
//				continue;
//			MBSPNode *pCellSect = NULL;
//			CurCell.SetCur(i, j);
//			MFace NewFace;
//			if(CurCell.CrStandardFace(Plane, &NewFace) != 0)
//				continue;
//			pCellSect = new MBSPNode;
//			pCellSect->SetFace(NewFace);
//			if(pRoot == (MBSPNode *)M_IN_PTR)
//			{
//				pCellSect->SetRight(pRoot);
//				n++;
//			}
//			else
//			{
//				n += pCellSect->CrSection(*pRoot);
//			}
//			if(pCellSect)
//			{// Concatenate new and current lists
//				*ppLastNode = pCellSect;
//				MBSPNode *pB = pCellSect;
//				for(;
//					!MBSPNode::IsTerminal(pB->GetLeft());
//					pB = pB->GetLeft());
//				ppLastNode = pB->GetLeftPtr();
//			}
//		}
//	return pRes;
//}
//
int MQuadTree::CrCellTree(int ix, int iy, Cell &C, MBSPNode *& pRoot) const
{
	pRoot = Array[ix][iy];//LOC STOR(обеспечено снаружи)

	int Mask = Zt | Zb;
	int NodesNum = 2;

	if(ix == 0){ Mask |= Xb; NodesNum++; }
	if(ix == ColsNum-1){ Mask |= Xt;  NodesNum++; }
	if(iy == 0){ Mask |= Yb; NodesNum++; }
	if(iy == RowsNum-1){ Mask |= Yt; NodesNum++; }

	for( int i = 0; i < NodesNum ; ++i)
	{
		MBSPNode *pTemp = new MBSPNode;
		pTemp->SetLeft(pRoot);
		pRoot = pTemp;
		pRoot->SetRight((MBSPNode *)M_OUT_PTR);
		pRoot->SetBFace(MElemIDProc::ID_NULL);
	}

	C.SetCur(ix, iy);
	MBSPNode *pTemp = pRoot;
	MFace TempFace;
	if( Mask & Zt )
	{
		pTemp->SetFace(C.CrBoundFace(Zt, TempFace));
		pTemp = pTemp->GetLeft();
	}
	if( Mask & Zb )
	{
		pTemp->SetFace(C.CrBoundFace(Zb, TempFace));
		pTemp = pTemp->GetLeft();
	}
	if( Mask & Xt )
	{
		pTemp->SetFace(C.CrBoundFace(Xt, TempFace));
		pTemp = pTemp->GetLeft();
	}
	if( Mask & Xb )
	{
		pTemp->SetFace(C.CrBoundFace(Xb, TempFace));
		pTemp = pTemp->GetLeft();
	}
	if( Mask & Yt )
	{
		pTemp->SetFace(C.CrBoundFace(Yt, TempFace));
		pTemp = pTemp->GetLeft();
	}
	if( Mask & Yb )
	{
		pTemp->SetFace(C.CrBoundFace(Yb, TempFace));
		pTemp = pTemp->GetLeft();
	}
	return NodesNum;
}

int MQuadTree::ClearCellTree(MBSPNode *pRoot, int NodesNum)
{
	for( int i = 0; i < NodesNum ; ++i)
	{
		MBSPNode *pTemp = pRoot->GetLeft();
		delete pRoot;
		pRoot = pTemp;
	}
	return 0;
}

bool MQuadTree::Contains(const BPoint & P)
{
	BPoint dP = P - MinPoint;
	if(dP.GetX() < 0 || dP.GetY() < 0)// int() will not give negative index for near 0 value
		return false;
	int ix = int(dP.GetX() / Xstep);
	int iy = int(dP.GetY() / Ystep);
	if(ix >= ColsNum || iy >= RowsNum)
		return false;
	const auto pNode = Array[ix][iy];
	if (!MBSPNode::IsTerminal(pNode))//LOC STOR+
	{
		ActLocStorage Act(*this, ix, iy);
		return pNode->Contains(P);
	}
	// Array[ix][iy] is terminal
	if(pNode == (MBSPNode *)M_OUT_PTR)
		return false;
	double z = P.GetZ();
	if( z <= MaxPoint.GetZ() + DMIN && z >= MinPoint.GetZ() - DMIN)
		return true;
	return false;
}

void MQuadTree::CrStandard1Cell(MBSPNode *& rpRoot, const MQuadTree::Cell & Cell)
{
	if(MBSPNode::IsTerminal(rpRoot))
		return;
	MBSPNode *pRoot = rpRoot;
	int r = pRoot->MakeCellPlanes(Cell);
	if(r != 0)
	{
		MBSPNode *pNode = r > 0 ? pRoot->GetLeft() : pRoot->GetRight();
		delete pRoot;
		pRoot = pNode;
		rpRoot = pRoot;
		if(MBSPNode::IsTerminal(pRoot))
			return;
	}
	pRoot->Trim();
}

size_t MQuadTree::GetMaxDepth(void)
{
	size_t Ret = 0;
	for(int i = 0; i < ColsNum; ++i)
		for(int j = 0; j < RowsNum; ++j)
		{
			MBSPNode *pNode = Array[i][j];
			if(!MTreeNode::IsTerminal(pNode))
				Ret = max(Ret, pNode->GetMaxDepth());
		}

	return Ret;
}

MBSPForest::ForestSizes MQuadTree::GetSizes() const
{
	MBSPForest::ForestSizes Sizes;
	for (int i = 0; i < ColsNum; ++i)
		for (int j = 0; j < RowsNum; ++j)
		{
			if(GetLocStorageC(i, j) != nullptr)
				Sizes += GetLocStorageC(i, j)->GetSizes();
			MBSPNode* pNode = Array[i][j];
			if (!MTreeNode::IsTerminal(pNode))
				Sizes += pNode->GetSizes();
		}

	return Sizes;
}

bool MQuadTree::CheckCh(void) const 
{
	return true;
}

void MQuadTree::PackUp(MBSPNode *node, int level, int deep, int &index)
{
	// pack up tree with root in node to MQuadTree::Array[][]
	// deep - tree depth between root and nodes in MQuadTree::Array[][]
	// index - MBSPNode index in MQuadTree::Array[][] where will write new MBSNode
	// level - current depth
	if (level < 6)
	{
		if (MBSPNode::IsTerminal(node))
		{
			for (; index >= 0; --index)
				Array[index / ColsNum][index % RowsNum] = node;//LOC STOR-
			return;
		}
		++level;
		PackUp(node->GetLeft(), level, deep, index);
		return;
	}

	//непосредственно работа с макушкой
	if (level - 6 < deep)
	{
		if (MBSPNode::IsTerminal(node))
		{
			int shift;
			shift = int(pow(2.0, deep - (level - 6)));
			for (int i = 0; i < shift; ++i)
			{
				if (index < 0)
					return; // never will be invoke, if tree corrent
				Array[index / ColsNum][index % RowsNum] = node;//LOC STOR-
				--index;
			}
			return;
		}
		++level;
		PackUp(node->GetLeft(), level, deep, index);
		PackUp(node->GetRight(), level, deep, index);
		return;
	}

	// write node in MQuadTree::Array[][]
	if (level - 6 == deep)
	{
		if (index < 0)
			return; // never invoked, if tree is correct
		Array[index / ColsNum][index % RowsNum] = node;//LOC STOR
		--index;
		return;
	}
	return;
}

void MQuadTree::MakeTree(int &NodeCount, MBSPNode ***MyTree) const
{
	// соединяет деревья, записанные в MQuadTree::Array в единое дерево

	int NXNum=(MFOREST.AddPoints(BPoint(1., 0., 0., 0.)));//LOC STOR не нужно если это дерево используется только в RayShreddingSpec
	int NYNum=(MFOREST.AddPoints(BPoint(0., 1., 0., 0.)));
	int NZNum=(MFOREST.AddPoints(BPoint(0., 0., 1., 0.)));

	int NXNum1=(MFOREST.AddPoints(BPoint(-1., 0., 0., 0.)));
	int NYNum1=(MFOREST.AddPoints(BPoint(0., -1., 0., 0.)));
	int NZNum1=(MFOREST.AddPoints(BPoint(0., 0., -1., 0.)));

	int *XFaceNum = new int [ColsNum + 1];
	int *YFaceNum = new int [RowsNum + 1];

	BPoint temp(0., 0., 0., 1.);
	////create X planes
	double x = (GetMax().GetX() - GetMin().GetX())/ColsNum;
	BPoint XStep(x, 0., 0., 0.);
	int XPointNum = (MFOREST.AddPoints(GetMin()));
	XFaceNum[0] = (MFOREST.AddFace(MFace(NXNum, XPointNum)));
	for(int i = 1; i <= ColsNum; i++)
	{
		temp = *(MFOREST.GetPoint(XPointNum)) + XStep;
		XPointNum = MFOREST.AddPoints(temp);
		if (i == (ColsNum)) 
			XFaceNum[i]=MFOREST.AddFace(MFace(NXNum1, XPointNum));
		else
			XFaceNum[i]=MFOREST.AddFace(MFace(NXNum, XPointNum));
	}

	//create Y planes		
	double y = (GetMax().GetY() - GetMin().GetY())/RowsNum;
	BPoint YStep(0., y, 0., 0.);
	int YPointNum = (MFOREST.AddPoints(GetMin()));
	YFaceNum[0] = (MFOREST.AddFace(MFace(NYNum, YPointNum)));
	for(int i = 1; i <= RowsNum; i++)
	{
		temp=*(MFOREST.GetPoint(YPointNum)) + YStep;
		YPointNum = MFOREST.AddPoints(temp);
		if (i == (RowsNum))
			YFaceNum[i] = MFOREST.AddFace(MFace(NYNum1, YPointNum));
		else
			YFaceNum[i] = MFOREST.AddFace(MFace(NYNum, YPointNum));
	}

	//create Z planes
	int ZPointNum[2], ZFaceNum[2];

	ZPointNum[0]=(MFOREST.AddPoints(GetMin()));
	ZFaceNum[0]=(MFOREST.AddFace(MFace(NZNum, ZPointNum[0])));

	ZPointNum[1]=MFOREST.AddPoints(GetMax());
	ZFaceNum[1]=MFOREST.AddFace(MFace(NZNum1, ZPointNum[1]));

	////////////////////////////////////////////////////////////////////////////////
	//calculating X & Y layers
	int XSteps=0, YSteps=0;	//число шагов по X и Y в матрице деревьев
	for(XSteps; ColsNum > pow(2.,XSteps);XSteps++)
		;
	for(YSteps; RowsNum > pow(2.,YSteps);YSteps++)
		;
	////////////////////////////////////////////////////////////////////////////////		
	////////////////////////////////////////////////////////////////////////////////				
	(*MyTree) = new NodePtr[6 + int(pow(2.,XSteps + YSteps))];//возможна потеря памяти при деактивации
	(*MyTree)[0] = new MBSPNode; //create root node
	NodeCount = 6; //set first 6 nodes
	for(int i = 0; i < NodeCount; ++i)
	{
		(*MyTree)[i + 1] = new MBSPNode;
		(*MyTree)[i]->SetLeft((*MyTree)[i + 1]);
		(*MyTree)[i]->SetRight((MTreeNode *)M_OUT_PTR);
		ElemID PlaneID; // Added for MakeFromBSP 5.05.12
		MElemIDProc::SetPCn(&PlaneID, STOCK_COLOR_NUM);
		switch(i) 
		{
		case 0: 
			(*MyTree)[i]->SetFace(ZFaceNum[0]);
			MElemIDProc::SetSurfID(&PlaneID, MFOREST.AddOrigSurf(new SOPlane(*MFOREST.GetPoint(NZNum), GetMin())));
			break;
		case 1: 
			(*MyTree)[i]->SetFace(ZFaceNum [1]);
			MElemIDProc::SetSurfID(&PlaneID, MFOREST.AddOrigSurf(new SOPlane(*MFOREST.GetPoint(NZNum1), GetMax())));
			break;
		case 2: (*MyTree)[i]->SetFace(XFaceNum[0]);
			MElemIDProc::SetSurfID(&PlaneID, MFOREST.AddOrigSurf(new SOPlane(*MFOREST.GetPoint(NXNum), GetMin())));
			break;
		case 3: (*MyTree)[i]->SetFace(XFaceNum[ColsNum]);
			MElemIDProc::SetSurfID(&PlaneID, MFOREST.AddOrigSurf(new SOPlane(*MFOREST.GetPoint(NXNum1), GetMax())));
			break;
		case 4: (*MyTree)[i]->SetFace(YFaceNum[0]);
			MElemIDProc::SetSurfID(&PlaneID, MFOREST.AddOrigSurf(new SOPlane(*MFOREST.GetPoint(NYNum), GetMin())));
			break;
		case 5: (*MyTree)[i]->SetFace(YFaceNum[RowsNum]);
			MElemIDProc::SetSurfID(&PlaneID, MFOREST.AddOrigSurf(new SOPlane(*MFOREST.GetPoint(NYNum1), GetMax())));
			break;
		}
		(*MyTree)[i]->SetBFace(PlaneID);
	}
	NodeCount--; //так как исчисление от 0
	int Ind;
	int PInd;
	int FaceNumber;
	////////////////////////////////////////////////////////////////////////////////
	// buid x-tree
	for(int j=0; j < XSteps; j++)
		for(int k=1;k<=pow(2.,j);k++)
		{
			Ind = NodeCount+int(pow(2.,j))+k-1;
			FaceNumber = XFaceNum[int(ColsNum*(0.5+pow(2.,j)-k)/int(pow(2.,j)))];

			if(j == 0)
			{
				(*MyTree)[Ind]->SetFace(*MFOREST.GetFace(FaceNumber));
				(*MyTree)[NodeCount]->SetLeft((*MyTree)[Ind]);
				(*MyTree)[NodeCount]->SetRight((MTreeNode *)M_OUT_PTR);
			}
			else
			{
				(*MyTree)[Ind] = new MBSPNode;
				(*MyTree)[Ind]->SetFace(*MFOREST.GetFace(FaceNumber));
				PInd = NodeCount+int(pow(2.,j-1))+int((k+1)/2)-1;
				if( k%2 == 0)
					(*MyTree)[PInd]->SetRight((*MyTree)[Ind]);
				else
					(*MyTree)[PInd]->SetLeft((*MyTree)[Ind]);
			}	
		}

	//build y-tree
	NodeCount += (int(pow(2.,XSteps)) - 1); //прибавляем кол-во вершин по X
	for(int j=0; j < YSteps; j++)
	{
		for(int k=1;k<=pow(2.,j);k++)
		{
			FaceNumber = YFaceNum[int(RowsNum*(0.5+pow(2.,j)-k)/int(pow(2.,j)))];
			if(j == 0 && XSteps ==0) //если по X не создавалось
			{
				Ind = NodeCount+int(pow(2.,j))+k-1;		
				(*MyTree)[Ind]->SetFace(*MFOREST.GetFace(FaceNumber));
				(*MyTree)[NodeCount]->SetLeft((*MyTree)[Ind]);
				(*MyTree)[NodeCount]->SetRight((MTreeNode *)M_OUT_PTR);
			}
			else
				if(j==0)
					for(int h=0; h < pow(2.,XSteps); h++)
					{
						Ind = NodeCount+int(pow(2.,j))+k-1+int(pow(2.,j))*h;
						PInd = NodeCount-int(pow(2.,XSteps-1))+1+int(h/2);
						(*MyTree)[Ind] = new MBSPNode;
						(*MyTree)[Ind]->SetFace(*MFOREST.GetFace(FaceNumber));
						if(h % 2 != 0)
							(*MyTree)[PInd]->SetRight((*MyTree)[Ind]);
						else
							(*MyTree)[PInd]->SetLeft((*MyTree)[Ind]);
					}		
				else
					for(int h=0; h < pow(2.,XSteps); h++)
					{
						Ind = NodeCount+k+int(pow(2.,j))*h;
						PInd = NodeCount-int(pow(2.,XSteps+j-1))+int(pow(2.,j-1))*h+int((k+1)/2);
						(*MyTree)[Ind] = new MBSPNode;				
						(*MyTree)[Ind]->SetFace(*MFOREST.GetFace(FaceNumber));
						if(k % 2 == 0)
							(*MyTree)[PInd]->SetRight((*MyTree)[Ind]);
						else
							(*MyTree)[PInd]->SetLeft((*MyTree)[Ind]);
					}

		}
		NodeCount += int(pow(2.,XSteps+j)); //приплюсовали вершины по Y
	}

	// create planes in new nodes
	// necessary for QDivide, but unnecessary for QMeasure	
	if(XSteps == 0 && YSteps ==0)
	{
		if(!MTreeNode::IsTerminal((*MyTree)[NodeCount]->GetLeft()))
			delete (*MyTree)[NodeCount]->GetLeft();
		(*MyTree)[NodeCount]->SetLeft((MBSPNode*)M_IN_PTR);
		(*MyTree)[NodeCount]->SetRight((MTreeNode *)M_OUT_PTR);
	}
	(*MyTree)[0]->MakeAllPlanes();
	(*MyTree)[0]->Trim();

	//connect forests trees
	if(XSteps == 0 && YSteps ==0)
	{
		(*MyTree)[NodeCount]->SetLeft(Array[0][0]);
		(*MyTree)[NodeCount]->SetRight((MTreeNode *)M_OUT_PTR);
	}
	else
	{
		int h = 1;
		for(int i=ColsNum-1; i >=  0; i--)
			for(int j=RowsNum-1; j >= 0; j-=2)
			{
				Ind = NodeCount-int(pow(2.,XSteps+YSteps-1)) + h;
				(*MyTree)[Ind]->SetLeft(Array[i][j]);
				(*MyTree)[Ind]->SetRight(Array[i][j-1]);
				++h;
			}
	}

	delete[] XFaceNum;
	delete[] YFaceNum;
}

void MQuadTree::RemoveTree(int NodeCount, MBSPNode ***MyTree)
{
	for(int i = 0; i <= NodeCount; ++i)
	{
		if(!MBSPNode::IsTerminal((*MyTree)[i]))
			delete (*MyTree)[i];
	}
	delete[] *MyTree;
}

void MQuadTree::ChangeYDir()
{
	for (int ix = 0; ix < ColsNum; ++ix)
		for (int iy = 0; iy < RowsNum / 2; ++iy)
		{
			auto buf = Array[ix][iy];
			Array[ix][iy] = Array[ix][RowsNum - iy - 1];
			Array[ix][RowsNum - iy - 1] = buf;
		}
	RecalcMinMax();
}

void MQuadTree::MakeFromTriangles(int NTri, const float* Triangles)
{
	for (int TriInd = 0; TriInd < NTri; ++TriInd)
	{
		int i = TriInd * 18;
		BPoint pn(Triangles[i], Triangles[i + 1], Triangles[i + 2], 0.);
		pn = pn * (-1. / sqrt(pn.D2()));
		BPoint p0(Triangles[i + 3], Triangles[i + 4], Triangles[i + 5], 1.);
		BPoint p1(Triangles[i + 9], Triangles[i + 10], Triangles[i + 11], 1.);
		BPoint p2(Triangles[i + 15], Triangles[i + 16], Triangles[i + 17], 1.);
		int n = MFOREST.AddPoints(pn);
		int b = MFOREST.AddPoints(p0);
		MFace BFace(n, b);
		BFace.SetArraySize(3);
		BFace.SetPointAt(0, b);
		BFace.SetPointAt(1, MFOREST.AddPoints(p1));
		BFace.SetPointAt(2, MFOREST.AddPoints(p2));
		StoreFace(&BFace, MElemIDProc::ID_NULL, cadrID(-1), false);
	}
	BBox Bounds;
	Bounds.Expand(GetMin() + BPoint(-1., -1., -1., 0.));
	Bounds.Expand(GetMax() + BPoint(+1., +1., +1., 0.));
	CreateBodyStd(Bounds, false, false);
	CrStandard(false);
}

void MQuadTree::MakeFromBDCEL(BDCEL& DCEL)
{
	auto Edges = DCEL.GetEdges();
	for (int iE = 0; iE < DCEL.GetEdgesCount(); ++iE)
	{
		if (Edges[iE][2] < 0)
			continue;
		int EdgeInd = iE;
		BPoint pn = DCEL.GetEdgeVec(EdgeInd) % DCEL.GetEdgeVec(Edges[EdgeInd][1]);
		pn = pn * (-1. / sqrt(pn.D2()));
		int n = MFOREST.AddPoints(pn);
		int b[3];
		for (int i = 0; i < 3; ++i)
		{
			BPoint P(DCEL.GetPointByStorNum(DCEL.GetVertexes()[DCEL.GetESVn(EdgeInd)][2]));
			b[i] = MFOREST.AddPoints(P);
			Edges[EdgeInd][2] = -1 * (Edges[EdgeInd][2] + 2);// Mark edge
			EdgeInd = Edges[EdgeInd][1];
		}
		MFace BFace(n, b[0]);
		BFace.SetArraySize(3);
		BFace.SetPointAt(0, b[0]);
		BFace.SetPointAt(1, b[1]);
		BFace.SetPointAt(2, b[2]);
		StoreFace(&BFace, MElemIDProc::ID_NULL, cadrID(-1), false);
	}
	DCEL.UnmarkAll();

	BBox Bounds;
	Bounds.Expand(GetMin() + BPoint(-1., -1., -1., 0.));
	Bounds.Expand(GetMax() + BPoint(+1., +1., +1., 0.));
	CreateBodyStd(Bounds, false, false);
	CrStandard(false);
}

void MQuadTree::RenumUsedFaces(const int* newind)
{
	for (int ix = Win.ixStart; ix < Win.ixEnd; ++ix)
		for (int iy = Win.iyStart; iy < Win.iyEnd; ++iy)
		{
			MBSPNode* pRoot = Array[ix][iy];//LOC STOR-
			if (MBSPNode::IsTerminal(pRoot))
				continue;
			pRoot->RenumUsedFaces(newind);
		}
}

void MQuadTree::SwapY()
{
	for (int ix = 0; ix < ColsNum; ++ix)
		for (int iy = 0; iy < RowsNum / 2; ++iy)
		{
			auto Buf = Array[ix][iy];
			Array[ix][iy] = Array[ix][RowsNum - 1 - iy];
			Array[ix][RowsNum - 1 - iy] = Buf;
			auto B = LocStArr[ix][iy];
			LocStArr[ix][iy] = LocStArr[ix][RowsNum - 1 - iy];
			LocStArr[ix][RowsNum - 1 - iy] = B;
		}
}

bool MQuadTree::RayShooting(const BPoint& Pi, const BPoint& Vi, bool type, BPoint& FoundP, BPoint& FoundN, ElemID& FoundID, cadrID& FoundCadrID)
{
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
	BBox Gabar;
	Gabar.Expand(MinPoint);
	Gabar.Expand(MaxPoint);

	RayMarch2D RMarch(Gabar, ColsNum, RowsNum, P, V);
	int i = -1, j = -1;
	MQuadTree::Cell CurCell(this);
	while (RMarch.GetNextCell(i, j))
	{
		ActLocStorage Act(*this, i, j);

		MBSPNode* pNewRoot = nullptr;
		const int NodesNum = CrCellTree(i, j, CurCell, pNewRoot);
		BPoint Vt = RMarch.GetV();
		Vt.Unit();
		Vt = Vt * 0.01;
		const MBSPNode *pNode = pNewRoot->RayShooting(RMarch.GetP() - Vt, RMarch.GetV() + Vt * 2., type);
		if (pNode != (MBSPNode*)M_NDEF_PTR)
		{
			auto ID = pNode->GetBFaceN();
			const MPlane& Plane = pNode->GetFace()->GetPlane();
			ClearCellTree(pNewRoot, NodesNum);
			double t = Plane.IntersLine(P, V);
			if (t != -1.)
			{
				if (t < MinT)
				{
					MinT = t;
					Plane.IntersLine(P, V, FoundP);
					FoundN = Plane.GetNormal();
					FoundID = ID;
					FoundCadrID = pNode->GetCadrID();
					break;
				}
			}
		}
		else
			ClearCellTree(pNewRoot, NodesNum);
	}
	FoundP = FoundP * CMatr.Or();
	FoundN = FoundN * CMatr.Or();
	if (MinT >= 1.e6)
		return false;

	return true;
}

void MQuadTree::SetGlobalStorOnly()
{
}

void MQuadTree::Move2GlobStor()
{
	for (int ix = 0; ix < ColsNum; ++ix)
		for (int iy = 0; iy < RowsNum; ++iy)
		{
			MBSPNode* pRoot = Array[ix][iy];//LOC STOR-
			if (MBSPNode::IsTerminal(pRoot))
				continue;
			// move points to glob storage
			size_type* newind = new size_type[GetLocStorageC(ix, iy)->m_PointArray.size()];
			for (int iF = 0; iF < GetLocStorageC(ix, iy)->m_PointArray.size(); ++iF)
			{
				int iNewF = MFOREST.AddPoints(GetLocStorageC(ix, iy)->m_PointArray.at(iF));
				newind[iF] = iNewF;
			}
			for (int iF = 0; iF < GetLocStorageC(ix, iy)->m_FaceArray.size(); ++iF)
			{
				MFace* pFace = &GetLocStorage(ix, iy)->m_FaceArray.at(iF);
				for (int i = 0; i < pFace->GetCount(); ++i)
				{
					int iOldP = pFace->GetIndPoint(i);
					if (iOldP < 0)
						pFace->SetPointAt(i, newind[-iOldP - 1]);
				}
			}
			//END: move points to glob storage
			// move faces to glob storage
			newind = new size_type[GetLocStorageC(ix, iy)->m_FaceArray.size()];
			for (int iF = 0; iF < GetLocStorageC(ix, iy)->m_FaceArray.size(); ++iF)
			{
				int iNewF = MFOREST.AddFace(GetLocStorageC(ix, iy)->m_FaceArray.at(iF));
				newind[iF] = iNewF;
			}
			std::vector<MBSPNode*> NodeStack;
			NodeStack.push_back(pRoot);
			while (!NodeStack.empty())
			{
				MBSPNode* pNode = NodeStack.back();
				NodeStack.pop_back();
				int Ind = pNode->GetFaceN();
				if (Ind < 0)
					pNode->SetFace(newind[-Ind - 1]);
				if (!MBSPNode::IsTerminal(pNode->GetLeft()))
					NodeStack.push_back(pNode->GetLeft());
				if (!MBSPNode::IsTerminal(pNode->GetRight()))
					NodeStack.push_back(pNode->GetRight());
			}
			delete[] newind;
			//END: move faces to glob storage
		}

}

bool MQuadTree::Is1Connected()
{
	bool res = true;
	QDivide DivideBSP(this, &MBSPForest::GetActive());
	if (DivideBSP.MarkConnected())
		if (DivideBSP.MarkConnected())
			res = false;
	DivideBSP.Revert();
	DivideBSP.PackUp();
	return res;
}

MQuadTree::ActLocStorage::ActLocStorage(MQuadTree& QTree, int ix, int iy)
{
	MBSPForest::GetLocStorage() = QTree.GetLocStorage(ix, iy);
}

MQuadTree::ActLocStorage::~ActLocStorage()
{
	MBSPForest::GetLocStorage() = nullptr;
}
