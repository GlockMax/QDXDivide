#include "stdafx.h"
#include "BBox.h"
#include "ConstDef.h"
#include "math.h"
#include "MElemIDProc.h"
#include "SOCyl.h"
#include "SOPlane.h"
#include "SOTorus.h"
#include "SOCone.h"
#include "SOSphere.h"
#include "MBSPNode.h"
#include "MBSPInter.h"
#include "NRectMesh.h"
#include "NMeshArray.h"
#include "NCadrGeom.h"
#include "BGeomArray.h"
#include "MQuadTreeSliced.h"
#include "DXSubModel5X.h"
#include "mquadtreerot.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

MQuadTreeRot::MQuadTreeRot(void)
{
}

MQuadTreeRot::~MQuadTreeRot(void)
{
}

bool MQuadTreeRot::Combine(MQuadBody * pInTree, int SectorsNum)
{

	SectorsNum = max( 4, SectorsNum);
	SectorsNum = min( 500, SectorsNum);

	if(BDebugState::Get().UseSlicedTree)
	{
		BPoint SlicedMin(pInTree->GetMin());
		BPoint SlicedMax(pInTree->GetMax());
		SlicedMax.SetX(GetMax().GetX());
		SlicedMax.SetZ(GetMax().GetX());
		SlicedMin.SetX(-SlicedMax.GetX());
		SlicedMin.SetZ(-SlicedMax.GetX());
		MQuadTreeSliced *pSlicedTree =
			new MQuadTreeSliced(SlicedMin, SlicedMax, RowsNum, true);
		pSlicedTree->CreateRotSolid(*this, SectorsNum);

		if (pInTree->GetRowsNum() != GetRowsNum())
			return false;

		if (pInTree->GetModelMode() == NGlobalState::BSP)
		{
			pSlicedTree->Subdivide2QuadTree(*static_cast<MQuadTree*>(pInTree), true);
			MBSPInter Inter;
			pInTree->RemoveWindow();
			pInTree->SafeMergeCh(Inter);
		}
		else
		{
			DXSubModel5X *pBodyDX = static_cast<DXSubModel5X *>(pInTree);
			if (!pBodyDX)
				return false;

			MQuadTree* pQuadTree =
				new MQuadTree(pInTree->GetMin(), pInTree->GetMax(), RowsNum, RowsNum, StockGr, false);
			pSlicedTree->Subdivide2QuadTree(*pQuadTree, false);
			pBodyDX->GetDirectionA(0).SetWindow(pBodyDX->GetGabar());
			pBodyDX->GetDirectionA(1).SetWindow(pBodyDX->GetGabar());
			pBodyDX->GetDirectionA(2).SetWindow(pBodyDX->GetGabar());
			pBodyDX->MakeFromBSP(*pQuadTree, pBodyDX->GetDirectionsA());

			pBodyDX->Intersect();
			delete pQuadTree;
		}
		delete pSlicedTree;
		// MFOREST.Compress();
	}
	else
	{

		//MQuadTree *pTreeBuf = pInTree->CreateSame();
		//
		//for(int ix = Win.ixStart; ix < Win.ixEnd ; ++ix)
		//{
		//	MeshArr MeshArray;
		//	for(int iy = Win.iyStart; iy < Win.iyEnd ; ++iy)
		//	{
		//		MBSPNode *pRoot = Array[ix][iy];
		//		if(MBSPNode::IsTerminal(pRoot))
		//			continue;

		//		MakeTurnMesh(MeshArray, pRoot, SectorsNum);
		//	}// iy
		//	for(int k = 0; k < MeshArray.GetSize(); ++k)
		//	{
		//		if(!MeshArray[k])
		//			continue;
		//		MeshArray[k]->StoreMesh(*pTreeBuf, MeshArray[k]->GetMeshID());
		//		delete MeshArray[k];
		//	}
		//}
		//BBox Bounds;
		//Bounds.Expand(pTreeBuf->GetMin());
		//Bounds.Expand(pTreeBuf->GetMax());
		//pTreeBuf->Create(Bounds, false);
		//pTreeBuf->CrStandard(false);

		//MBSPInter Inter;
		//pInTree->SafeMergeCh(Inter, *pTreeBuf);

		//pTreeBuf->Reset(M_IN_PTR);
		//delete pTreeBuf;
	}

	return true;
}

bool MQuadTreeRot::MakeTurnMesh(MeshArr &MeshArray, const MBSPNode * pRoot, int SectorsNum)
{
	if(MBSPNode::IsTerminal(pRoot))
		return true;

	MBSPNode FacetRoot;
	pRoot->CrFacets(FacetRoot);
	bool fl = false;
	int LinesNum = 0;
	for(MBSPNode *pNode = &FacetRoot ; !MBSPNode::IsTerminal(pNode); pNode = pNode->GetLeft())
	{
		for(MBSPNode *pN = pNode->GetRight() ; !MBSPNode::IsTerminal(pN) ; pN = pN->GetLeft())
		{
			if(pN->GetFace()->Area2() > MINST * MINST && pN->GetFace()->GetCount() == 4)
			{
				LinesNum++;
				fl = true;
			}
		}
	}
	if(fl)
	{
		int LineInd = 0;
		for(MBSPNode *pNode = &FacetRoot ; !MBSPNode::IsTerminal(pNode); pNode = pNode->GetLeft())
		{
			ElemID ElID = pNode->GetBFaceN();
			int SurfNum = MFOREST.GetIDProc()->GetSurfID(ElID);
			if(SurfNum < 0)
				continue;
			SOrigSurf *pSurf = MFOREST.GetOrigSurf(SurfNum);
			if (pSurf == nullptr)
				continue;
			for(MBSPNode *pN = pNode->GetRight(); !MBSPNode::IsTerminal(pN); pN = pN->GetLeft())
			{
				const MFace *pFace = pN->GetFace();
				if(pFace->Area2() > MINST * MINST && pFace->GetCount() == 4)
				{
					//find p0 and p1
                    const BPoint *p0 = NULL, *p1 = NULL;
					int ii = 0;
					for(; ii < 4; ++ii)
					{
						p0 = pFace->GetPoint(ii);
						p1 = pFace->GetPoint((ii + 1) % 4);
						if(p0->GetZ() < 0. && p1->GetZ() < 0.)
							break;
					}
					if(pN->GetBFaceN() == MElemIDProc::ID_ORIENT)
					{
						const BPoint *pB = p0; p0 = p1; p1 = pB;
					}
					NParamMesh *pMesh = MakeOneTurnMesh(*p1, *p0, pSurf, SectorsNum);
					pMesh->SetMeshID(MElemIDProc::SetSurfID(&ElID, pMesh->m_iSurf));
					MeshArray.Add(pMesh);
					
					++LineInd;
				}
			}
		}
	}
//	MFOREST.RetToMark();
	FacetRoot.DeleteAll();

	return true;
}

NParamMesh * MQuadTreeRot::MakeOneTurnMesh(const BPoint & P0, const BPoint & P1, const SOrigSurf * pSurf, int SectorsNum)
{// Create mesh by Y revolution
	// Define new surface
	BPoint q0( P0 ); q0.ProjXY();
	BPoint q1( P1 ); q1.ProjXY();

	SOrigSurf *pNewSurf;
	switch(pSurf->GetType())
	{
	case PLANE:
		{
			double Angle = (q1 - q0).Angle(BPoint(0.,1.,0.,0.));
			if( fabs( Angle ) < MIND || fabs( fabs( Angle ) - 180.) < MIND)
				pNewSurf = new SOCyl(BPoint(0.,0.,0.,1.), BPoint(0.,1.,0.,0.)
					, fabs(q0.GetX()) * (q1.GetY() > q0.GetY() ? -1. : 1.));
			else if (fabs( fabs( Angle ) - 90.) < MIND)
				pNewSurf = new SOPlane(BPoint(0., Angle > 0. ? 1. : -1. ,0.,0.), q0);
			else
			{
				SOCone Buf(BPoint(0.,0.,0.,1.), BPoint(0.,1.,0.,0.), q0, q1);
				pNewSurf = new SOCone(Buf.GetCenter(), Buf.Get_N_Vector()
					, Buf.GetAngle() * (q1.GetY() > q0.GetY() ? -1. : 1.));
			}
		}
		break;
	case CYLINDER:
		{
			const SOCyl *pCyl = (SOCyl *)pSurf;
			double Rl = fabs(pSurf->GetCenter().GetX());
			if( Rl < MIND )
				pNewSurf = new SOSphere(pCyl->GetRadius()
					, BPoint(0., pSurf->GetCenter().GetY(), 0., 1.));
			else
				pNewSurf = new SOTorus(Rl
					, pCyl->GetRadius()
					, BPoint(0., pSurf->GetCenter().GetY(), 0., 1.)
					, BPoint(0.,1.,0.,0.));
		}
		break;
	default:
		pNewSurf = NULL;
		break;
	}
	NRectMesh *pMesh = new NRectMesh;
	pMesh->m_iSurf = MFOREST.AddOrigSurf(pNewSurf);
	pMesh->m_sgn = 1;
	if(pNewSurf->GetType() == PLANE && ( fabs(q0.GetX()) < MIND || fabs(q1.GetX()) < MIND ))
	{// One circle
		BPoint &q = q0; // the point for rotation
		int Orient = 1;
		if(fabs(q0.GetX()) < MIND)
		{
			Orient = -1;
			q = q1;
		}
		pMesh->SetSize(1, SectorsNum);
		pMesh->m_arr[0][0] = MFOREST.AddPoints(q);
		BMatr M;
		for(int i = 1; i < SectorsNum; ++i)
		{
			M.RotY(BPoint(0.,0.,0.,1.), -(360. * i * Orient) / SectorsNum);
			pMesh->m_arr[0][i] = MFOREST.AddPoints(q * M);
		}
	}
	else
	{
		pMesh->SetSize(2, SectorsNum + 1);
		pMesh->m_arr[0][0] = MFOREST.AddPoints(q0);
		pMesh->m_arr[1][0] = MFOREST.AddPoints(q1);
		pMesh->m_arr[0][SectorsNum] = pMesh->m_arr[0][0];
		pMesh->m_arr[1][SectorsNum] = pMesh->m_arr[1][0];
		BMatr M;
		for(int i = 1; i < SectorsNum; ++i)
		{
			M.RotY(BPoint(0.,0.,0.,1.), -(360. * i) / SectorsNum);
			pMesh->m_arr[0][i] = MFOREST.AddPoints(q0 * M);
			pMesh->m_arr[1][i] = MFOREST.AddPoints(q1 * M);
		}
	}
	return pMesh;
}

bool MQuadTreeRot::GetContour(BGeomArray & GeomArr)
{
	for(int ix = Win.ixStart; ix < Win.ixEnd ; ++ix)
	{
		for(int iy = Win.iyStart; iy < Win.iyEnd ; ++iy)
		{
			const MBSPNode *pRoot = GetRootNode(ix, iy);
			if(MBSPNode::IsTerminal(pRoot))
				continue;
			MQuadTree::ActLocStorage Act(*this, ix, iy);
			ContElemArray ContElems;
			ExtractContInfo(ContElems, pRoot);
			for(int l = 0; l < ContElems.GetSize(); ++l)
			{
				NCadrGeom *pGeom = ContElems[l].CrCadrGeom();
				if (pGeom)
				{
					GeomArr.Add(pGeom);
				}
			}
		}// iy
	}// ix
	return true;
}

int MQuadTreeRot::ExtractContInfo(ContElemArray & ContElems, const MBSPNode * pRoot) const
{
	if(MBSPNode::IsTerminal(pRoot))
		return true;

	MBSPNode FacetRoot;
	pRoot->CrFacets(FacetRoot);
	bool fl = false;
	int LinesNum = 0;
	for(MBSPNode *pNode = &FacetRoot ; !MBSPNode::IsTerminal(pNode); pNode = pNode->GetLeft())
	{
		for(MBSPNode *pN = pNode->GetRight() ; !MBSPNode::IsTerminal(pN) ; pN = pN->GetLeft())
		{
			if(pN->GetFace()->Area2() > MINST * MINST && pN->GetFace()->GetCount() == 4)
			{
				LinesNum++;
				fl = true;
			}
		}
	}
	if(fl)
	{
		int LineInd = 0;
		for(MBSPNode *pNode = &FacetRoot ; !MBSPNode::IsTerminal(pNode); pNode = pNode->GetLeft())
		{
			ElemID ElID = pNode->GetBFaceN();
//			SOrigSurf *pSurf = MFOREST.GetOrigSurf(MFOREST.GetIDProc()->GetSurfID(ElID));
			for(MBSPNode *pN = pNode->GetRight(); !MBSPNode::IsTerminal(pN); pN = pN->GetLeft())
			{
				const MFace *pFace = pN->GetFace();
				if(pFace->Area2() > MINST * MINST && pFace->GetCount() == 4)
				{
					//find p0 and p1
                    const BPoint *p0 = NULL, *p1 = NULL;
					int ii = 0;
					for(; ii < 4; ++ii)
					{
						p0 = pFace->GetPoint(ii);
						p1 = pFace->GetPoint((ii + 1) % 4);
						if(p0->GetZ() < 0. && p1->GetZ() < 0.)
							break;
					}
					if(pN->GetBFaceN() == MElemIDProc::ID_ORIENT)
					{
						const BPoint *pB = p0; p0 = p1; p1 = pB;
					}
					if ((*p0 - *p1).D2() < MIND * MIND)
						continue;
					ContElems.Add(ContElem(p0, p1, ElID));
					
					++LineInd;
				}
			}
		}
	}
	FacetRoot.DeleteAll();

	return 0;
}

void MQuadTreeRot::Transform(const BMatr & M)
{
	// M may contain translation and changing signs of Z and Y only
	BBox B;
	B.Expand(MinPoint * M);
	B.Expand(MaxPoint * M);
	MinPoint = B.GetMinPoint();
	MaxPoint = B.GetMaxPoint();
	MBSPForest::NTransfData TransfData;
	GetTransfData(TransfData);
	MFOREST.TransformSelected(TransfData, M);
	if (!M.HasNotRot())
	{
		SwapY();
	}
}

MQuadTreeRot * MQuadTreeRot::CreateFullCopy(void)
{
	MQuadTreeRot *pQ = new MQuadTreeRot;
	MQuadTree::CreateFullCopy(pQ);
	return pQ;
}

