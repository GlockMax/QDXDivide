#include "stdafx.h"
#include "ConstDef.h"
#include "math.h"
#include "BMatr.h"
#include "BBox.h"
#include "MElemIDProc.h"
#include "MBSPTree.h"
#include "MBSPNode.h"
#include "MDivideTree.h"
#include "mquadtreerot.h"
#include "mquadtreesliced.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

typedef  MBSPNode ***p3;
typedef  MBSPNode **p2;
typedef  MBSPNode *p1;

MQuadTreeSliced::MQuadTreeSliced(void)
{
}

MQuadTreeSliced::MQuadTreeSliced( const BPoint & Minp, const BPoint & Maxp, int Rowsn, bool GlobalStor) :
	MQuadTree(Minp, Maxp, Rowsn, 1, BStockGrID(), GlobalStor)
{
}

void MQuadTreeSliced::CreateRotSolid( ContElemArray *ContElemArrays, int SectorsNum)
{
	int *NormalNums = new int[SectorsNum];
	double Alf = 360. / SectorsNum;
	BMatr Rot;
	Rot.RotY(BPoint(0., 0., 0., 1.), -Alf);
	BMatr Rot2;
	Rot2.RotY(BPoint(0., 0., 0., 1.), MINAD * 10. + 0.5 * Alf);
	int BPind = MFOREST.AddPoints(BPoint(0., 0., 0., 1.));
	for(int k = 0; k < SectorsNum; ++k)
		NormalNums[k] = MFOREST.AddPoints(BPoint(0., 0., 1., 0.) * BMatr().RotY(BPoint(0.,0.,0.,1.), -k * Alf + MINAD * 10. + 0.5 * Alf));
	// Create trees for the first sector
	for(int j = 0; j < RowsNum; ++j)
	{
		// Create and store contour faces
		if(ContElemArrays[j].GetSize() == 0)
		{
			SetRootNodeConst(0, j, M_OUT_PTR);
			continue;
		}
		int l = 0;
		for(; l < ContElemArrays[j].GetSize(); ++l)
		{
			BPoint P0 = *ContElemArrays[j][l].p0;
			BPoint P1 = *ContElemArrays[j][l].p1;
			P0 = P0.ProjXY() * Rot2;
			P1 = P1.ProjXY() * Rot2;
			MFace *pFace = MFOREST.CalcFace(MFOREST.AddPoints(P0)
				, MFOREST.AddPoints(P1)
				, MFOREST.AddPoints(P1 * Rot)
				, MFOREST.AddPoints(P0 * Rot)
				, 1.);
			if (pFace != nullptr)
			{
				MBSPNode* pRoot = new MBSPNode;
				pRoot->SetBFace(MakeID4TurnSurf(ContElemArrays[j][l]));
				pRoot->SetFace(*pFace);
				delete pFace;
				pRoot->SetRight(GetRootNodeV(0, j));
				pRoot->SetLeft((MBSPNode*)M_IN_PTR);
				SetRootNode(0, j, pRoot);
			}
		}
		// Create and store sector faces
		BPoint Q0 = BPoint(0., MinPoint.GetY() + l * Ystep, 0., 1.);
		BPoint Q1 = BPoint(0., MinPoint.GetY() + (l + 1) * Ystep, 0., 1.);
		BPoint Q2 = Q1 + BPoint(1., 0., 0., 1.);
		BPoint Q3 = Q0 + BPoint(1., 0., 0., 1.);
		MFace BFace0(NormalNums[0], BPind);
		BFace0.SetArraySize(4);
		BFace0.SetPointAt(0, Q0);
		BFace0.SetPointAt(1, Q1);
		BFace0.SetPointAt(2, Q2);
		BFace0.SetPointAt(3, Q3);

		MBSPNode *pRoot = new MBSPNode;
		pRoot->SetBFace(MElemIDProc::ID_DUMMY);
		pRoot->SetFace(BFace0);
		pRoot->SetRight(GetRootNodeV(0, j));
		pRoot->SetLeft((MBSPNode*)M_IN_PTR);
		SetRootNode(0, j, pRoot);

		MFace BFace1(NormalNums[1], BPind);
		BFace1.SetArraySize(4);
		BFace1.SetPointAt(0, Q0);
		BFace1.SetPointAt(1, Q1);
		BFace1.SetPointAt(2, Q2 * Rot);
		BFace1.SetPointAt(3, Q3 * Rot);

		pRoot = new MBSPNode;
		pRoot->SetBFace(MElemIDProc::ID_DUMMY);
		pRoot->SetFace(BFace1);
		pRoot->SetRight(GetRootNodeV(0, j));
		pRoot->SetLeft((MBSPNode*)M_IN_PTR);
		SetRootNode(0, j, pRoot);
	}
	BBox Bounds;
	Bounds.Expand(GetMin());
	Bounds.Expand(GetMax());
	CreateBodyStd(Bounds, false, false);
	MQuadTree::CrStandard(false);// With two dummy faces only
// Store trees
	MBSPNode **OrigTrees = new p1[RowsNum];
	for(int t = 0; t < RowsNum; ++t)
	{
		OrigTrees[t] = (MBSPNode *)M_OUT_PTR;
		const MBSPNode *pTree = GetRootNode(0, t);
		if(MBSPNode::IsTerminal(pTree))
			continue;
		MQuadTree::ActLocStorage Act(*this, 0, t);
		OrigTrees[t] = pTree->GetRight()->GetLeft();
		delete pTree->GetRight();
		delete pTree;
		SetRootNodeConst(0, t, M_IN_PTR);
		MBSPTree::SetOrder(OrigTrees[t]);
	}

	// Create sectors trees
	MBSPNode ****Terminals = new p3[RowsNum];
	for(int ix = 0; ix < RowsNum; ++ix)
	{
		Terminals[ix] = new p2[SectorsNum];
		for(int iy = 0; iy < SectorsNum; ++iy)
			Terminals[ix][iy] = (MBSPNode **)M_NDEF_PTR;
	}
	for(int i = 0; i < RowsNum; ++i)
	{
		MQuadTree::ActLocStorage Act(*this, 0, i);
		if(OrigTrees[i] == (MBSPNode *)M_OUT_PTR)
			SetRootNodeConst(0, i, M_OUT_PTR);
		else
			SetRootNode(0, i, CrSectorsTree(Terminals[i], NormalNums, SectorsNum));
	}
	MQuadTree::CrStandard(false);// With dummy faces and one sector tree

	for(int t = 0; t < RowsNum; ++t)
	{
		if (OrigTrees[t] != (MBSPNode *)M_OUT_PTR)
		{
			MQuadTree::ActLocStorage Act(*this, 0, t);
			*Terminals[t][0] = OrigTrees[t];
			for (int v = 1; v < SectorsNum; ++v)
			{
				OrigTrees[t] = MBSPTree::CopySubtree(OrigTrees[t]);
				MBSPTree::SpecRotCopy(OrigTrees[t], Rot);
				*Terminals[t][v] = OrigTrees[t];
			}
		}
		delete[] Terminals[t];
	}
	delete[] Terminals;
	delete[] OrigTrees;
	delete[] NormalNums;
}

void MQuadTreeSliced::CreateRotSolid(MQuadTreeRot & RotTree, int SectorsNum)
{
	// store all in global storage. This tree must be GlobalStor
	if(RotTree.GetColsNum() != 1)
		return;
	ContElemArray *ContElemArrays = new ContElemArray[RowsNum];
	for(int iy = Win.iyStart; iy < Win.iyEnd ; ++iy)
	{
		const MBSPNode *pRoot = RotTree.GetRootNode(0, iy);
		if(MBSPNode::IsTerminal(pRoot))
			continue;
		MQuadTree::ActLocStorage Act(RotTree, 0, iy);
		ContElemArrays[iy].SetSize(0, 256);
		RotTree.ExtractContInfo(ContElemArrays[iy], pRoot);
	}

	CreateRotSolid(ContElemArrays, SectorsNum);

	delete[] ContElemArrays;
}

MBSPNode * MQuadTreeSliced::CrSectorsTree(MBSPNode *** Terminals, int *NormalNums, int SectorsNum)
{
// Trees size, NormalNums size and SectorsNum must be equal, even and >= 4

	MBSPNode *pRoot = new MBSPNode;
	int BPind = MFOREST.AddPoints(BPoint(0., 0., 0., 1.));
	pRoot->SetFace(MFace(NormalNums[0], BPind));
	pRoot->SetBFace(MElemIDProc::ID_DUMMY);
	CrSectorsSubTree(pRoot->GetLeftPtr(), 0, SectorsNum / 2, Terminals, NormalNums, BPind);
	CrSectorsSubTree(pRoot->GetRightPtr(), SectorsNum / 2, SectorsNum, Terminals, NormalNums, BPind);

	return pRoot;
}
void MQuadTreeSliced::CrSectorsSubTree(MBSPNode **ppRoot, int Ns, int Ne, MBSPNode *** Terminals, int *NormalNums, int BPind)
{
	int N = Ne - Ns;
	if(N <= 0)
		return;
	if(N == 1)
	{
		Terminals[Ns] = ppRoot;
		return;
	}
	int Nm = Ns + N / 2;
	*ppRoot = new MBSPNode;
	(*ppRoot)->SetFace(MFace(NormalNums[Nm], BPind));
	(*ppRoot)->SetBFace(MElemIDProc::ID_DUMMY);
	CrSectorsSubTree((*ppRoot)->GetRightPtr(), Ns, Nm, Terminals, NormalNums, BPind);
	CrSectorsSubTree((*ppRoot)->GetLeftPtr(), Nm, Ne, Terminals, NormalNums, BPind);
}


void MQuadTreeSliced::CrStandardWODummy(bool ToAux)
{// Create standard trees ignoring dummy faces
	auto LocArray = ToAux ? AuxArray : Array;
	Cell CurCell(this);
	for (int ix = Win.ixStart; ix < Win.ixEnd; ++ix)
	{
		for (int iy = Win.iyStart; iy < Win.iyEnd; ++iy)
		{
			MBSPNode* pRoot = LocArray[ix][iy];
			if (!MBSPNode::IsTerminal(pRoot))//LOC STOR-
			{
				CrStandard1CellWOD(LocArray[ix][iy], CurCell.SetCur(ix, iy));
			}
		}
	}
}

void MQuadTreeSliced::CrStandard1CellWOD(MBSPNode *& rpRoot, const MQuadTree::Cell & Cell)
{
	if(MBSPNode::IsTerminal(rpRoot))
		return;

	if(rpRoot->GetBFaceN() == MElemIDProc::ID_DUMMY)
	{
		CrStandard1CellWOD(*rpRoot->GetLeftPtr(), Cell);
		CrStandard1CellWOD(*rpRoot->GetRightPtr(), Cell);
		return;
	}
	CrStandard1Cell(rpRoot, Cell);

}
void MQuadTreeSliced::CrStandard(bool ToAux)
{
	CrStandardWODummy(ToAux);
}
int MQuadTreeSliced::Subdivide2QuadTree(MQuadTree & QuadTree, bool ToAux)
{
	// this tree must be GlobalStor and QuadTree will contain positive indexes only
	// this is necessary because of MDivideTree Div(&DivFace, pRoot, pPart, pRest);
	// pPart and pRest can't be in different local storages
	if(RowsNum != QuadTree.RowsNum)
		return -1;

	if (fabs(GetMin().GetY() - QuadTree.GetMin().GetY()) > MIND
		|| fabs(GetMax().GetY() - QuadTree.GetMax().GetY()) > MIND)
		return -2;
	bool NeedCut4 = ((GetMin() - QuadTree.GetMin()).D2() > MIND * MIND
		|| (GetMax() - QuadTree.GetMax()).D2() > MIND * MIND);

	MFace CutFaces[4];
	if (NeedCut4)// Cut this tree by 4 boundary planes of QuadTree
	{// Find boundary faces
		MQuadTree::Cell Cell(&QuadTree);
		Cell.SetCur(0, 0);
		Cell.CrBoundFace(Xb, CutFaces[0]);
		Cell.CrBoundFace(Zb, CutFaces[1]);
		Cell.SetCur(QuadTree.GetColsNum() - 1, QuadTree.GetRowsNum() - 1);
		Cell.CrBoundFace(Xt, CutFaces[2]);
		Cell.CrBoundFace(Zt, CutFaces[3]);
	}
	for (int iy = 0; iy < RowsNum; ++iy)
	{
		MBSPNode *pRoot = GetRootNodeV(0, iy);
		if (NeedCut4)// Cut this tree by 4 boundary planes of QuadTree
		{
			MBSPNode *pPart = (MBSPNode *)M_NDEF_PTR;
			MBSPNode *pRest = (MBSPNode *)M_NDEF_PTR;
			for (int i = 0; i < 4; ++i)
			{
				MDivideTree Div(&CutFaces[i], pRoot, pPart, pRest);
				Div.Init(CutFaces[i].GetPlane());
				Div.DoDivide();
				pRoot = pPart;
				if (!MBSPNode::IsTerminal(pRest))
					delete pRest;
			}
		}
		SetRootNodeConst(0, iy, M_NDEF_PTR);
		for(int ix = 0; ix < QuadTree.ColsNum - 1; ++ix)
		{
			if(MBSPNode::IsTerminal(pRoot))
			{
				ToAux ? QuadTree.SetRootNodeAux(ix, iy, pRoot) : QuadTree.SetRootNode(ix, iy, pRoot);
				continue;
			}
			MQuadTree::Cell Cell(&QuadTree);
			Cell.SetCur(ix, iy);
			MFace DivFace;
			Cell.CrBoundFace(Xt, DivFace);
			MBSPNode *pPart = (MBSPNode *)M_NDEF_PTR;
			MBSPNode *pRest = (MBSPNode *)M_NDEF_PTR;
			MDivideTree Div(&DivFace, pRoot, pPart, pRest);
			Div.Init(DivFace.GetPlane());
			Div.DoDivide();
			ToAux ? QuadTree.SetRootNodeAux(ix, iy, pPart) : QuadTree.SetRootNode(ix, iy, pPart); //LOC STOR-
			pRoot = pRest;
		}
		ToAux ? QuadTree.SetRootNodeAux(QuadTree.ColsNum - 1, iy, pRoot) : QuadTree.SetRootNode(QuadTree.ColsNum - 1, iy, pRoot);
	}
	return 0;
}
