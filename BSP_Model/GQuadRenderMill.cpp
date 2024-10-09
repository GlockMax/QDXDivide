#include "stdafx.h"
#include "ConstDef.h"
#include "NObjEnums.h"
#include "NCMComData.h"
#include "BaseRender.h"
#include "BDCEL_BSP.h"
#include "BDCEL_STL.h"
#include "MBSPNode.h"
#include "SOrigSurf.h"
#include "ConstDef.h"
#include "NLine.h"
#include "MElemIDProc.h"
#include "NTiParams.h"
#include "NColor.h"
#include "math.h"
#include "TBladeCont.h"
#include "RDCEL.h"
#include "BRotSolid.h"
#include "MLocStorage.h"
#include "MTMashineTool.h"
#include "MBSPCell.h"
#include "RRopes.h"
#include "GQuadRenderMill.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

GQuadRenderMill::GQuadRenderMill(const MQuadBody &Tree) :
	GQuadRender(Tree)
{
	RendArray = nullptr;
	RendArrayReady = false;
	if (MFOREST.GetIDProc()->GetStockEdgesS())
		ActivateExtArr();
}

GQuadRenderMill* GQuadRenderMill::CreateGQuadRenderMillList(const MQuadBody &Tree)
{
	MBody *pBody = Tree.GetNextMBody();
	GQuadRenderMill* pResult = new GQuadRenderMill(Tree);
	while (pBody)
	{
		GQuadRenderMill* pNext = new GQuadRenderMill( *((MQuadBody *)pBody) );
		pResult->AddRenderInList(pNext);
		pBody = pBody->GetNextMBody();
	}
	return pResult;
}

GQuadRenderMill* GQuadRenderMill::CreateListCopy(const GQuadRenderMill &inRender)
{
	GQuadRenderMill *result = new GQuadRenderMill(inRender);
	GQuadRenderMill *last = result;
	GQuadRenderMill *next = NULL;

	GBodyRender *nextBody = inRender.GetNextRender();
	while (nextBody)
	{
		next = new GQuadRenderMill( *(GQuadRenderMill*)nextBody );
		last->SetNextRender(next);
		next->SetPreviousRender(last);

		last = next;
		nextBody = nextBody->GetNextRender();
	}
	return result;
}

GQuadRenderMill::GQuadRenderMill(const GQuadRenderMill & inRender) :
	GQuadRender(inRender)
{
	RendArray = nullptr;
	RendArrayReady = false;
	typedef  GLvoid **p2;
	typedef  GLvoid *p1;

	Array = new p2[ColsNum];
	GLvoid **bp = new p1[RowsNum * ColsNum];
	for(int ix = 0; ix < ColsNum; ++ix)
	{
		Array[ix] = bp + ix * RowsNum;
		for(int iy = 0; iy < RowsNum; ++iy)
		{
			GLfloat *pC = (GLfloat *)inRender.Array[ix][iy];
			if(Empty(pC))
			{
				Array[ix][iy] = pC;
				continue;
			}
			int TriNum = int(pC[0]);
			GLfloat *pRet = new GLfloat[1 + (TRI_SIZE + ELEM_ID_SIZE) * TriNum];
			memcpy(pRet, pC, (1 + (TRI_SIZE + ELEM_ID_SIZE) * TriNum) * sizeof(GLfloat));
			Array[ix][iy] = pRet;
		}
	}
}

GQuadRenderMill::~GQuadRenderMill(void)
{
	for(int ix = 0; ix < ColsNum ; ++ix)
	{
		for(int iy = 0; iy < RowsNum ; ++iy)
		{
			if(!Empty(Array[ix][iy]))
				delete[] Array[ix][iy];
		}
	}
	delete[] Array[0];
	delete[] Array;
	ClearRendArray();
}

GLvoid * GQuadRenderMill::FillNVArray(MBSPNode *pRoot, bool Smooth)
{// Returns GLfloat array. First element of the array contains triangles number
	MBSPNode FacetRoot;
	pRoot->CrFacets(FacetRoot);
	bool fl = false;
	int TriNum = 0;
	for(MBSPNode *pNode = &FacetRoot ; !MBSPNode::IsTerminal(pNode); pNode = pNode->GetLeft())
	{
		for(MBSPNode *pN = pNode->GetRight() ; !MBSPNode::IsTerminal(pN) ; pN = pN->GetLeft())
		{
			if(pN->GetFace()->Area2() > MINST * MINST)
			{
				int FSize = pN->GetFace()->GetCount() - 2;
				TriNum +=  FSize > 0 ? FSize : 0;
				fl = true;
			}
		}
	}
	GLfloat *pRet = (GLfloat *)QR_EMPTY_DEF;
	if(fl)
	{
		pRet = new GLfloat[1 + (TRI_SIZE + ELEM_ID_SIZE) * TriNum];
		pRet[0] = GLfloat(TriNum);
		int TriInd = 0;
		for(MBSPNode *pNode = &FacetRoot ; !MBSPNode::IsTerminal(pNode); pNode = pNode->GetLeft())
		{
			ElemID ElID = pNode->GetBFaceN();
			int BFaceN = MFOREST.GetIDProc()->GetSurfID(ElID);
			PackedColor col = MFOREST.GetIDProc()->GetColor(ElID);
			
			bool SmoothLoc = Smooth && (BFaceN >= 0 && BFaceN != ID_EMPTY);
			for(MBSPNode *pN = pNode->GetRight(); !MBSPNode::IsTerminal(pN); pN = pN->GetLeft())
			{
				const MFace *pFace = pN->GetFace();
				if(pFace->Area2() > MINST * MINST && pFace->GetCount() > 2)
				{
					const BPoint *p0 = pFace->GetPoint(0);
					BPoint n0b(pFace->GetPlane().GetNormal());
					BPoint n0(n0b);
					if( SmoothLoc )
					{
						SOrigSurf *pSurf = MFOREST.GetOrigSurf(BFaceN);
						n0 = pSurf ? pSurf->GetNormal(*p0) : n0b;
						if( n0.GetH() < -0.5)
							n0 = n0b;
					}
					double dirNk = -1.;// заменено после добавления знаков радиуса в GetNormal
					const BPoint *p1 = pFace->GetPoint(1);
					BPoint n1(n0b);
					if( SmoothLoc )
					{
						SOrigSurf *pSurf = MFOREST.GetOrigSurf(BFaceN);
						n1 = pSurf ? pSurf->GetNormal(*p1) : n0b;
						if( n1.GetH() < -0.5)
							n1 = n0b;
					}
					for (int i=2; i < pFace->GetCount(); i++)
					{
						const BPoint *p = pFace->GetPoint(i);
						BPoint n(n0b);
						if( SmoothLoc )
						{
							SOrigSurf *pSurf = MFOREST.GetOrigSurf(BFaceN);
							n = pSurf ? pSurf->GetNormal(*p) : n0b;
							if( n.GetH() < -0.5)
								n = n0b;
						}
						StoreTriangle(pRet + TRI_SIZE * TriInd + 1, col,
							*p0, n0 * dirNk, *p1, n1 * dirNk, *p, n * dirNk);
						n1 = n;
						p1 = p;
						StoreID(pRet + 1 + TRI_SIZE * TriNum + ELEM_ID_SIZE * TriInd, ElID);
						++TriInd;
					}
				}
			}
		}
	}
	FacetRoot.DeleteAll();
	return (GLfloat *)pRet;
}
void GQuadRenderMill::FillCell(MQuadBody & Body, int ix, int iy)
{
	if(Body.GetModelMode() != NGlobalState::BSP)
		return;
	MQuadTree::ActLocStorage Act(static_cast<MQuadTree&>(Body), ix, iy);
	const MQuadTree &Tree = (MQuadTree &)Body;
	if(!Empty(Array[ix][iy]))
		delete[] Array[ix][iy];
		
	GLvoid *pRet = nullptr;

	if(MBSPNode::IsTerminal(Tree.GetRootNode(ix, iy)))
	{
		pRet = FillSimpleCell(Tree, ix, iy);
	}
	else
	{
		MBSPNode::Collapse(Tree.GetRootNodeR(ix, iy));
		MLocStorage::CompressFaces(MBSPForest::GetLocStorage(), Tree.GetRootNodeV(ix, iy), true);
		MLocStorage::CompressPoints(MBSPForest::GetLocStorage(), true);
		MFOREST.SetLocMark();
		MQuadTree::Cell CurCell(&Tree);
		MBSPNode* pNode = nullptr;
		const int NodesNum = Tree.CrCellTree(ix, iy, CurCell, pNode);
		pRet = FillNVArray(pNode, SmoothRender);
		Tree.ClearCellTree(pNode, NodesNum);
		MFOREST.RetToLocMark();
		MBSPForest::GetLocStorage()->Shrink();
		//DEBUG
//		MBSPNode::ClearMarked(Tree.GetRootNodeR(ix, iy));
	}
	Array[ix][iy] = pRet;

	if(ExtArr != nullptr)
	{
		delete ExtArr[ix][iy];
		ExtArr[ix][iy] = (MBSPCell*)QR_EMPTY;
		if(!Empty(pRet))
		{
			MBSPCell* pNewCell = Tree.MakeBSPCell(ix, iy);
			if (FillBSPCell(*pNewCell, SmoothAngle, (GLfloat*)pRet, true, false) > 0)
				ExtArr[ix][iy] = pNewCell;
			else
				delete pNewCell;
		}
	}
}

size_t GQuadRenderMill::FillBSPCell(MBSPCell& BSPCell, double Angle, const float* pCI, bool Sharp, bool Error)
{
	BDCEL_BSP El;
	El.SetSmoothAngle(Angle);
	El.MakeFromStockTri(pCI);
	BSPCell.FillFromBDCEL(El, Sharp, Error);
	return BSPCell.Size();
}

int GQuadRenderMill::RenderCh(enum RenderMode GlobRMode, int Type, bool DrawSurf)
{
	GlobRMode;
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	if (BodyRotActive)
	{
		glPushMatrix();
		glMultMatrixd(BodyRot.Or().GetArray());
	}
	const int stride = sizeof(GLfloat) * TRI_SIZE / 3;
	glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	if (RendArray != nullptr && RendArrayReady)// for comparision
	{
		for (int ix = 0; ix < ColsNum; ++ix)
			for (int iy = 0; iy < RowsNum; ++iy)
			{
				glEnable(GL_LIGHTING);
				if (RendArray[ix][iy] != nullptr)
					RendArray[ix][iy]->Render(true, true);
			}
	}
	else
	{
		glPushAttrib(GL_ENABLE_BIT | GL_LINE_BIT | GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT);
		if (ExtArr != NULL)
		{
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(1., 1.);
		}
		if (DrawSurf)
		{
			if (UseExternColor || IsCollided())
				glDisableClientState(GL_COLOR_ARRAY);
			else
				glEnableClientState(GL_COLOR_ARRAY);
			for (int ix = 0; ix < ColsNum; ++ix)
				for (int iy = 0; iy < RowsNum; ++iy)
				{
					GLfloat* pC = (GLfloat*)Array[ix][iy];
					if (Empty(pC))
						continue;
					if (IsCollided())
						glColor3fv(GetCollidedColor().array);
					glNormalPointer(GL_FLOAT, stride, pC + 1);
					glColorPointer(4, GL_UNSIGNED_BYTE, stride, pC + (1 + 3 + 3));
					glVertexPointer(3, GL_FLOAT, stride, pC + (1 + 3));
					glDrawArrays(GL_TRIANGLES, 0, 3 * int(pC[0]));
				}
		}
		// Draw sharp edges
		if ((ExtArr != NULL) && (Type & 4) && EdgesWidth > 0.01)
		{
			BaseRender::SetWFMode();
			glDisableClientState(GL_COLOR_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			glLineWidth(float(EdgesWidth));
			glColor3fv(EdgesColor.array);
			glPushName(OG_SKELETON);
			if (pRopes->IsEmpty())
			{
				for (int ix = 0; ix < ColsNum; ++ix)
					for (int iy = 0; iy < RowsNum; ++iy)
					{
						class MBSPCell* pI = ExtArr[ix][iy];
						if (Empty(pI))
							continue;
						pI->DrawSharpEdges();
					}
			}
			else
			{
				pRopes->Draw();
			}
			glPopName();
			BaseRender::RemoveWFMode();
		}
		glPopAttrib();
	}
	if (BodyRotActive)
		glPopMatrix();
	glPopClientAttrib();

	singleLock.Unlock();
	return 0;
}
bool GQuadRenderMill::FindSegments(SnapRopes& oSegments) const
{
	if(ExtArr == nullptr)
		return false;
	return pRopes->GetSegments(oSegments);
}
GLvoid * GQuadRenderMill::FillSimpleCell(const MQuadTree & Tree, int ix, int iy)
{
	const MBSPNode *pRoot = Tree.GetRootNode(ix, iy);
	if(pRoot != (MBSPNode *)M_IN_PTR)
		return (GLvoid *)QR_EMPTY_DEF;
	int Mask = Zt | Zb;
	int TriNum = 4;

	if(ix == 0)
	{	Mask |= Xb; TriNum += 2; }
	if(ix == Tree.ColsNum-1)
	{	Mask |= Xt; TriNum += 2; }
	if(iy == 0)
	{	Mask |= Yb; TriNum += 2; }
	if(iy == Tree.RowsNum-1)
	{	Mask |= Yt; TriNum += 2; }

	GLfloat	*pRet = new GLfloat[1 + (TRI_SIZE + ELEM_ID_SIZE) * TriNum];
	pRet[0] = GLfloat(TriNum);
	int TriInd = 1;
	
	PackedColor col = MFOREST.GetIDProc()->GetColor(MElemIDProc::ID_NULL);

	if( Mask & Zt )
	{
		BPoint n(0,0,1,0);
		BPoint p0 = Tree.MinPoint + BPoint(Tree.Xstep * ix, Tree.Ystep * iy, Tree.MaxPoint.GetZ() - Tree.MinPoint.GetZ(), 0);
		StoreTriangle(pRet + TriInd, col,
			p0 , n, p0 + BPoint(Tree.Xstep, 0, 0, 0), n, p0 + BPoint(Tree.Xstep, Tree.Ystep, 0, 0),n);
		StoreTriangle(pRet + TriInd + TRI_SIZE, col,
			p0 , n, p0 + BPoint(Tree.Xstep, Tree.Ystep, 0, 0), n, p0 + BPoint(0, Tree.Ystep, 0, 0),n);
		StoreID(pRet + TRI_SIZE * TriNum + ELEM_ID_SIZE * (TriInd/TRI_SIZE), MElemIDProc::ID_NULL);
		StoreID(pRet + TRI_SIZE * TriNum + ELEM_ID_SIZE * (TriInd/TRI_SIZE + 1), MElemIDProc::ID_NULL);
		TriInd += TRI_SIZE * 2;
	}
	if( Mask & Zb )
	{
		BPoint n(0,0,-1,0);
		BPoint p0 = Tree.MinPoint + BPoint(Tree.Xstep * ix, Tree.Ystep * iy, 0, 0);
		StoreTriangle(pRet + TriInd, col,
			p0 + BPoint(Tree.Xstep, Tree.Ystep, 0, 0),n,p0 + BPoint(Tree.Xstep, 0, 0, 0), n, p0 , n	);
		StoreTriangle(pRet + TriInd + TRI_SIZE, col,
			p0 + BPoint(0, Tree.Ystep, 0, 0),n, p0 + BPoint(Tree.Xstep, Tree.Ystep, 0, 0), n,	p0 , n);
		StoreID(pRet + TRI_SIZE * TriNum + ELEM_ID_SIZE * (TriInd/TRI_SIZE), MElemIDProc::ID_NULL);
		StoreID(pRet + TRI_SIZE * TriNum + ELEM_ID_SIZE * (TriInd/TRI_SIZE + 1), MElemIDProc::ID_NULL);
		TriInd += TRI_SIZE * 2;
	}

	if( Mask & Xt )
	{
		BPoint n(1,0,0,0);
		BPoint p0 = Tree.MaxPoint + BPoint(0, -Tree.Ystep * (RowsNum - iy), 0, 0);
		StoreTriangle(pRet + TriInd, col,
			p0 + BPoint(0, Tree.Ystep, Tree.MinPoint.GetZ() - Tree.MaxPoint.GetZ(), 0),n, p0 + BPoint(0, Tree.Ystep, 0, 0), n, p0 , n);
		StoreTriangle(pRet + TriInd + TRI_SIZE, col,
			 p0 + BPoint(0, 0, Tree.MinPoint.GetZ() - Tree.MaxPoint.GetZ(), 0),n, p0 + BPoint(0, Tree.Ystep, Tree.MinPoint.GetZ() - Tree.MaxPoint.GetZ(), 0), n, p0 , n);
		StoreID(pRet + TRI_SIZE * TriNum + ELEM_ID_SIZE * (TriInd/TRI_SIZE), MElemIDProc::ID_NULL);
		StoreID(pRet + TRI_SIZE * TriNum + ELEM_ID_SIZE * (TriInd/TRI_SIZE + 1), MElemIDProc::ID_NULL);
		TriInd += TRI_SIZE * 2;
	}
	if( Mask & Xb )
	{
		BPoint n(-1,0,0,0);
		BPoint p0 = Tree.MinPoint + BPoint(0, Tree.Ystep * iy, 0, 0);
		StoreTriangle(pRet + TriInd, col,
			p0 + BPoint(0, Tree.Ystep, Tree.MaxPoint.GetZ() - Tree.MinPoint.GetZ(), 0),n, p0 + BPoint(0, Tree.Ystep, 0, 0), n, p0 , n);
		StoreTriangle(pRet + TriInd + TRI_SIZE, col,
			p0 + BPoint(0, 0, Tree.MaxPoint.GetZ() - Tree.MinPoint.GetZ(), 0),n, p0 + BPoint(0, Tree.Ystep, Tree.MaxPoint.GetZ() - Tree.MinPoint.GetZ(), 0), n,	p0 , n);
		StoreID(pRet + TRI_SIZE * TriNum + ELEM_ID_SIZE * (TriInd/TRI_SIZE), MElemIDProc::ID_NULL);
		StoreID(pRet + TRI_SIZE * TriNum + ELEM_ID_SIZE * (TriInd/TRI_SIZE + 1), MElemIDProc::ID_NULL);
		TriInd += TRI_SIZE * 2;
	}
	if( Mask & Yt )
	{
		BPoint n(0,1,0,0);
		BPoint p0 = Tree.MaxPoint + BPoint(-Tree.Xstep * (ColsNum - ix), 0, 0, 0);
		StoreTriangle(pRet + TriInd, col,
			p0 , n, p0 + BPoint(Tree.Xstep, 0, 0, 0), n, p0 + BPoint(Tree.Xstep, 0, Tree.MinPoint.GetZ() - Tree.MaxPoint.GetZ(), 0),n);
		StoreTriangle(pRet + TriInd + TRI_SIZE, col,
			p0 , n, p0 + BPoint(Tree.Xstep, 0, Tree.MinPoint.GetZ() - Tree.MaxPoint.GetZ(), 0), n, p0 + BPoint(0, 0, Tree.MinPoint.GetZ() - Tree.MaxPoint.GetZ(), 0),n);
		StoreID(pRet + TRI_SIZE * TriNum + ELEM_ID_SIZE * (TriInd/TRI_SIZE), MElemIDProc::ID_NULL);
		StoreID(pRet + TRI_SIZE * TriNum + ELEM_ID_SIZE * (TriInd/TRI_SIZE + 1), MElemIDProc::ID_NULL);
		TriInd += TRI_SIZE * 2;
	}
	if( Mask & Yb )
	{
		BPoint n(0,-1,0,0);
		BPoint p0 = Tree.MinPoint + BPoint(Tree.Xstep * ix, 0, 0, 0);
		StoreTriangle(pRet + TriInd, col,
			p0 , n, p0 + BPoint(Tree.Xstep, 0, 0, 0), n, p0 + BPoint(Tree.Xstep, 0, Tree.MaxPoint.GetZ() - Tree.MinPoint.GetZ(), 0),n);
		StoreTriangle(pRet + TriInd + TRI_SIZE, col,
			p0 , n, p0 + BPoint(Tree.Xstep, 0, Tree.MaxPoint.GetZ() - Tree.MinPoint.GetZ(), 0), n, p0 + BPoint(0, 0, Tree.MaxPoint.GetZ() - Tree.MinPoint.GetZ(), 0),n);
		StoreID(pRet + TRI_SIZE * TriNum + ELEM_ID_SIZE * (TriInd/TRI_SIZE), MElemIDProc::ID_NULL);
		StoreID(pRet + TRI_SIZE * TriNum + ELEM_ID_SIZE * (TriInd/TRI_SIZE + 1), MElemIDProc::ID_NULL);
		TriInd += TRI_SIZE * 2;
	}


	return pRet;
}
int GQuadRenderMill::RenderWMaskCh(const GBodyRender & QMaskRender, int Type, bool complement)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	if (BodyRotActive)
	{
		glPushMatrix();
		glMultMatrixd(BodyRot.Or().GetArray());
	}
	const GQuadRenderMill &MaskRender = (GQuadRenderMill &)QMaskRender;
// Render masked elements only if complement == false or nonmasked otherwise
	const int stride = sizeof(GLfloat) * TRI_SIZE / 3;
	glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);


	glPushAttrib(GL_ENABLE_BIT | GL_LINE_BIT | GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT);
	if(ExtArr != NULL)
	{
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1., 1.);
	}
	if (UseExternColor || IsCollided())
		glDisableClientState(GL_COLOR_ARRAY);
	else
		glEnableClientState(GL_COLOR_ARRAY);
	if (IsCollided())
		glColor3fv(GetCollidedColor().array);
	glEnable(GL_LIGHTING);
	for (int ix = 0; ix < ColsNum; ++ix)
		for (int iy = 0; iy < RowsNum; ++iy)
		{
			GLfloat* pC = (GLfloat*)Array[ix][iy];
			if (Empty(pC) || MaskRender.ColsNum <= ix || MaskRender.RowsNum <= iy)
				continue;
			if ((NotDefined(MaskRender.Array[ix][iy])) != complement)
				continue;
			glNormalPointer(GL_FLOAT, stride, pC + 1);
			glColorPointer(4, GL_UNSIGNED_BYTE, stride, pC + (1 + 3 + 3));
			glVertexPointer(3, GL_FLOAT, stride, pC + (1 + 3));
			glDrawArrays(GL_TRIANGLES, 0, 3 * int(pC[0]));
		}
	// Draw sharp edges
	if ((ExtArr != NULL) && (Type & 4) && EdgesWidth > 0.01)
	{
		BaseRender::SetWFMode();
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glLineWidth(float(EdgesWidth));
		glColor3fv(EdgesColor.array);
		glPushName(OG_SKELETON);
		for (int ix = 0; ix < ColsNum; ++ix)
			for (int iy = 0; iy < RowsNum; ++iy)
			{
				if (MaskRender.ColsNum <= ix || MaskRender.RowsNum <= iy)
					continue;
				if ((NotDefined(MaskRender.Array[ix][iy])) != complement)
					continue;
				MBSPCell* pI = ExtArr[ix][iy];
				if (Empty(pI))
					continue;
				pI->DrawSharpEdges();
			}
		glPopName();
		BaseRender::RemoveWFMode();
	}
	glPopAttrib();

	glPopClientAttrib();
	if (BodyRotActive)
		glPopMatrix();
	singleLock.Unlock();
	return 0;
}
int GQuadRenderMill::RenderWMaskChMult(const std::vector<const GQuadRender*>& Ends, int Type)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();

	if (BodyRotActive)
	{
		glPushMatrix();
		glMultMatrixd(BodyRot.Or().GetArray());
	}
	// difference with RenderWMaskCh	const GQuadRenderMill& MaskRender = (GQuadRenderMill&)QMaskRender;
	// Render masked elements only if complement == false or nonmasked otherwise
	const int stride = sizeof(GLfloat) * TRI_SIZE / 3;
	glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glPushAttrib(GL_ENABLE_BIT | GL_LINE_BIT | GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT);
	if (ExtArr != NULL)
	{
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1., 1.);
	}
	if (UseExternColor || IsCollided())
		glDisableClientState(GL_COLOR_ARRAY);
	else
		glEnableClientState(GL_COLOR_ARRAY);
	if (IsCollided())
		glColor3fv(GetCollidedColor().array);
	glEnable(GL_LIGHTING);
	for (int ix = 0; ix < ColsNum; ++ix)
		for (int iy = 0; iy < RowsNum; ++iy)
		{
			// Start of difference with RenderWMaskCh
			GLfloat* pC = (GLfloat*)Array[ix][iy];
			if (Empty(pC))
				continue;
			bool MaskDefined = false;
			for each (auto pMask in Ends)
			{
				if (pMask != nullptr && pMask->ColsNum > ix && pMask->RowsNum > iy)
					MaskDefined |= !NotDefined(static_cast<const GQuadRenderMill*>(pMask)->Array[ix][iy]);
			}
			if (MaskDefined)
				continue;
			// End of  difference with RenderWMaskCh
			glNormalPointer(GL_FLOAT, stride, pC + 1);
			glColorPointer(4, GL_UNSIGNED_BYTE, stride, pC + (1 + 3 + 3));
			glVertexPointer(3, GL_FLOAT, stride, pC + (1 + 3));
			glDrawArrays(GL_TRIANGLES, 0, 3 * int(pC[0]));
		}
	// Draw sharp edges
	if ((ExtArr != NULL) && (Type & 4) && EdgesWidth > 0.01)
	{
		BaseRender::SetWFMode();
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glLineWidth(float(EdgesWidth));
		glColor3fv(EdgesColor.array);
		glPushName(OG_SKELETON);
		for (int ix = 0; ix < ColsNum; ++ix)
			for (int iy = 0; iy < RowsNum; ++iy)
			{
				// Start of difference with RenderWMaskCh
				bool MaskDefined = false;
				for each (auto pMask in Ends)
				{
					if (pMask != nullptr && pMask->ColsNum > ix && pMask->RowsNum > iy)
						MaskDefined |= !NotDefined(static_cast<const GQuadRenderMill*>(pMask)->Array[ix][iy]);
				}
				if (MaskDefined)
					continue;
				// End of  difference with RenderWMaskCh
				MBSPCell* pI = ExtArr[ix][iy];
				if (Empty(pI))
					continue;
				pI->DrawSharpEdges();
			}
		glPopName();
		BaseRender::RemoveWFMode();
	}
	glPopAttrib();

	glPopClientAttrib();
	if (BodyRotActive)
		glPopMatrix();
	singleLock.Unlock();
	return 0;
}
int GQuadRenderMill::RenderWMaskMult(const std::vector<const class GQuadRender*>& Rends, int Type, MTMachineTool& MT)
{
	int result = 0;
	std::vector<const GQuadRender*> Ends(Rends.size());
	for (int k = 0; k < Ends.size(); ++k)
		Ends[k] = Rends[k];
	for (GQuadRenderMill* pRender = this; pRender != nullptr; pRender = static_cast<GQuadRenderMill*>(pRender->GetNextRender()))
	{
		const BStockGrID StPos = pRender->GetStockPos();
		glPushMatrix();
		BMatr M = MT.GetStockMovedMatrSGr(StPos, true) * MT.GetStockMovedMatr(0, true).invr();
		glMultMatrixd(M.GetArray());

		result = pRender->RenderWMaskChMult(Ends, Type);

		glPopMatrix();
		for (int k = 0; k < Ends.size(); ++k)
			if(Ends[k] != nullptr)
				Ends[k] = static_cast<const GQuadRender*>(Ends[k]->pNextRender);
	}
	return result;
}

float* GQuadRenderMill::MakeDCEL(BDCEL_BSP& DCEL) const
{
	__int32 TriNum = 0;
	for (int ix = 0; ix < ColsNum; ++ix)
		for (int iy = 0; iy < RowsNum; ++iy)
		{
			GLfloat* pC = (GLfloat*)Array[ix][iy];
			if (Empty(pC))
				continue;
			TriNum += int(pC[0]);
		}

	if (TriNum <= 0)
		return nullptr;
	GLfloat* pCn = new GLfloat[1 + TriNum * 3 * DCEL.GetSVS()];
	pCn[0] = GLfloat(TriNum);
	GLfloat* pCur = pCn + 1;
	for (int ix = 0; ix < ColsNum; ++ix)
		for (int iy = 0; iy < RowsNum; ++iy)
		{
			GLfloat* pC = (GLfloat*)Array[ix][iy];
			if (Empty(pC))
				continue;
			int LocTriNum = int(pC[0]);
			pC += 1;
			for (int i = 0; i < LocTriNum * 3; ++i)
			{
				memcpy(pCur, pC, DCEL.GetSVS() * sizeof(pC[0]));
				pCur += DCEL.GetSVS();
				pC += TRI_SIZE / 3;
			}
		}
	DCEL.MakeFromStockTri(pCn);
	return pCn;
}

UINT64 GQuadRenderMill::GetHASH(const GBodyRender* pEnd) const
{
	auto pEndMill = dynamic_cast<const GQuadRenderMill*>(pEnd);
	UINT64 HASH = 0;
	for (int ix = 0; ix < ColsNum; ++ix)
		for (int iy = 0; iy < RowsNum; ++iy)
		{
			float* pCEnd = (pEndMill == nullptr) ? (float*)QR_EMPTY : pEndMill->GetArray()[ix][iy];
			float* pC = NotDefined(pCEnd) ? GetArray()[ix][iy] : pCEnd;
			HASH ^= UINT64(pC);
		}

	return HASH;
}

GBodyRender* GQuadRenderMill::MakeCopy() const
{
	GQuadRenderMill* pRet = new GQuadRenderMill(*this);
	return pRet;
}

GQuadRenderMill* GQuadRenderMill::MakeCommonRender(int RInd, const std::vector<std::vector<GQuadRenderMill*>>& Renders)
{
	GQuadRenderMill* pCommon = nullptr;
	for each(const auto& Str in Renders)
	{
		const auto pRend = Str.at(RInd);
		if (pRend != nullptr)
		{
			if (pCommon == nullptr)
			{
				pCommon = new GQuadRenderMill(*pRend);
			}
			else
			{
				pCommon->Join(pRend);
			}
		}
	}
	return pCommon;
}

void GQuadRenderMill::Join(const GQuadRenderMill* pOther)
{
	if (pOther == nullptr)
		return;
	if (pOther->ColsNum != ColsNum || pOther->RowsNum != RowsNum)
		return;
	for (int ix = 0; ix < ColsNum; ++ix)
		for (int iy = 0; iy < RowsNum; ++iy)
		{
			GLfloat* pC = (GLfloat*)Array[ix][iy];
			if (NotDefined(pOther->Array[ix][iy]))
				continue;
			if (!NotDefined(pC))// prefer old values
				continue;
			Array[ix][iy] = pOther->Array[ix][iy];
		}
}

void GQuadRenderMill::Clear()
{
	// destructor deletes all data pointed by this render
	// clear removes all data pointers so that the destructor does not try to delete it
	for (int ix = 0; ix < ColsNum; ++ix)
	{
		for (int iy = 0; iy < RowsNum; ++iy)
		{
			if (!Empty(Array[ix][iy]))
				Array[ix][iy] = nullptr;
		}
	}
}

void GQuadRenderMill::WriteSTL(CStdioFile & f) const
{
	BDCEL_BSP List;
	GLfloat* pCn = MakeDCEL(List);
	List.WriteSTL(f);
	delete[] pCn;
}
double GQuadRenderMill::GetVolume() const
{
	double V = 0.;
	for(int ix = 0; ix < ColsNum ; ++ix)
		for(int iy = 0; iy < RowsNum ; ++iy)
		{
			GLfloat *pC = (GLfloat *)Array[ix][iy];
			if(Empty(pC))
				continue;
			for(int it = 0; it < int(pC[0]); ++it)
			{
				GLfloat *b = (GLfloat *)pC + 1 + it * 3 * 7;// See StoreTriangle
				BPoint P0(0), P1(0), P2(0);
				ExtractTriangle(b, P0, P1, P2);
				BPoint n = (P1 - P0) % (P2 - P0);
				V += (1./6.) * (P0 * n);
			}
		}
	return V;
}
void GQuadRenderMill::ExtractTriangle(GLfloat *pRet, BPoint & P0, BPoint & P1, BPoint & P2)
{
	P0.Set(pRet[ 3], pRet[ 4], pRet[ 5], 1.);
	P1.Set(pRet[10], pRet[11], pRet[12], 1.);
	P2.Set(pRet[17], pRet[18], pRet[19], 1.);
}

void GQuadRenderMill::ExtractID(GLfloat* pC, int TriInd, ElemID& ID)
{
//	pRet + 1 + TRI_SIZE * TriNum + ELEM_ID_SIZE * TriInd
	ID = *((ElemID*)(pC + 1 + TRI_SIZE * int(pC[0]) + ELEM_ID_SIZE * TriInd));
}

int GQuadRenderMill::FillAllArraysCh(MQuadBody& Body)
{
	GQuadRender::FillAllArraysCh(Body);
	// make ropes
	BLinesDCEL LinesDCEL;
	if (ExtArr != nullptr)
	{
		// copy edges to common LinesDCEL
		UINT GlobSize = 0;
		for (int ix = 0; ix < ColsNum; ++ix)
			for (int iy = 0; iy < RowsNum; ++iy)
			{
				MBSPCell* pCell = ExtArr[ix][iy];
				if (pCell != (MBSPCell*)QR_EMPTY)
					GlobSize += UINT(pCell->LinesDCEL.Size());
			}
		LinesDCEL.Resize(GlobSize);
		GlobSize = 0;
		for (int ix = 0; ix < ColsNum; ++ix)
			for (int iy = 0; iy < RowsNum; ++iy)
			{
				MBSPCell* pCell = ExtArr[ix][iy];
				if (pCell != (MBSPCell*)QR_EMPTY)
				{
					pCell->SetShift(GlobSize);
					for (UINT k = 0; k < pCell->Size(); ++k)
						LinesDCEL[GlobSize + k].ShiftCopy(pCell->LinesDCEL[k], GlobSize);
					GlobSize += UINT(pCell->LinesDCEL.Size());
				}
			}
		// establish connections between cells
		for (int ix = 0; ix < ColsNum; ++ix)
			for (int iy = 0; iy < RowsNum; ++iy)
			{
				if (ix == ColsNum - 1 && iy == RowsNum - 1)
					continue;
				MBSPCell* pCell00 = ExtArr[ix][iy];
				if (pCell00 == (MBSPCell*)QR_EMPTY)
					continue;
				if (ix != ColsNum - 1)
				{
					MBSPCell* pCell10 = ExtArr[ix + 1][iy];
					pCell00->Bind(true, pCell10, LinesDCEL);
				}
				if (iy != RowsNum - 1)
				{
					MBSPCell* pCell01 = ExtArr[ix][iy + 1];
					pCell00->Bind(false, pCell01, LinesDCEL);
				}
			}
	}
	// fill ropes
	RRopes* pNewRopes = new RRopes;
	if (!pNewRopes->CreateFromLines(LinesDCEL))
	{
		delete pNewRopes;
		pNewRopes = new RRopes;
	}
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	delete pRopes;
	pRopes = pNewRopes;
	return 0;
}

void GQuadRenderMill::StoreTriangle(GLfloat *pRet, const PackedColor col, 
				const BPoint & p0, const BPoint & n0,
				const BPoint & p1, const BPoint & n1,
				const BPoint & p2, const BPoint & n2)
{
	// This fragment needed for AT_MILL_TURN only!
	// Для токарной и токарно-фрезерной заготовок модель (BSP дерево) повернута 
	// относительно истинного положения на -90 градусов вокруг OX
	if(NCMComData::IsTurnEnabled())
	{// Rotate +90 around OX
		pRet[ 0] = GLfloat(n0.GetX());
		pRet[ 1] = GLfloat(-n0.GetZ());
		pRet[ 2] = GLfloat(n0.GetY());
		pRet[ 3] = GLfloat(p0.GetX());
		pRet[ 4] = GLfloat(-p0.GetZ());
		pRet[ 5] = GLfloat(p0.GetY());
		pRet[ 6] = col.fval;
		pRet[ 7] = GLfloat(n1.GetX());
		pRet[ 8] = GLfloat(-n1.GetZ());
		pRet[ 9] = GLfloat(n1.GetY());
		pRet[10] = GLfloat(p1.GetX());
		pRet[11] = GLfloat(-p1.GetZ());
		pRet[12] = GLfloat(p1.GetY());
		pRet[13] = col.fval;
		pRet[14] = GLfloat(n2.GetX());
		pRet[15] = GLfloat(-n2.GetZ());
		pRet[16] = GLfloat(n2.GetY());
		pRet[17] = GLfloat(p2.GetX());
		pRet[18] = GLfloat(-p2.GetZ());
		pRet[19] = GLfloat(p2.GetY());
		pRet[20] = col.fval;
		return;
	}
	// END:This fragment needed for AT_MILL_TURN only
	pRet[ 0] = GLfloat(n0.GetX());
	pRet[ 1] = GLfloat(n0.GetY());
	pRet[ 2] = GLfloat(n0.GetZ());
	pRet[ 3] = GLfloat(p0.GetX());
	pRet[ 4] = GLfloat(p0.GetY());
	pRet[ 5] = GLfloat(p0.GetZ());
	pRet[ 6] = col.fval;
	pRet[ 7] = GLfloat(n1.GetX());
	pRet[ 8] = GLfloat(n1.GetY());
	pRet[ 9] = GLfloat(n1.GetZ());
	pRet[10] = GLfloat(p1.GetX());
	pRet[11] = GLfloat(p1.GetY());
	pRet[12] = GLfloat(p1.GetZ());
	pRet[13] = col.fval;
	pRet[14] = GLfloat(n2.GetX());
	pRet[15] = GLfloat(n2.GetY());
	pRet[16] = GLfloat(n2.GetZ());
	pRet[17] = GLfloat(p2.GetX());
	pRet[18] = GLfloat(p2.GetY());
	pRet[19] = GLfloat(p2.GetZ());
	pRet[20] = col.fval;
}

const BPoint GQuadRenderMill::RayCasting(const BPoint & Pi, const BPoint & Vi, double MinW, BPoint &N) const
{
	// Pi and Vi are in stock CS i.e. take in consideration stock position but ignores MillTurn rotation and BodyRot
	BPoint P = Pi * BodyRot.Inv();
	BPoint V = Vi * BodyRot.Inv();
	// Find the intersection point nearest to P in the V direction
	// N - external normal in P (from Stl)
	// Vector V must have length = 1 ???
	double MinT = 1.e12;
	for(int ix = 0; ix < ColsNum ; ++ix)
		for(int iy = 0; iy < RowsNum ; ++iy)
		{
			GLfloat *pC = (GLfloat *)Array[ix][iy];
			if(Empty(pC))
				continue;
			for(int it = 0; it < int(pC[0]); ++it)
			{
				GLfloat *b = (GLfloat *)pC + 1 + it * 3 * 7;// See StoreTriangle
				BPoint p0(0), p1(0), p2(0);
				ExtractTriangle(b, p0, p1, p2);
				p0 = p0 - P;
				p1 = p1 - P;
				p2 = p2 - P;
				BPoint n0(b[0], b[1], b[2], 0.);// Normal in the vertex
				//Calculate normal
				BPoint n = (p1 - p0) % (p2 - p0);
				n = n * ((n * n0 < 0) ? -1. : 1.);
				double z =	n * V;
				if(fabs(z) < MINAR)
					continue;
				z = 1./z;
				double t = p0 * n * z;
				if(t < 0. || t > MinT)
					continue;
				BPoint IntP(V * t);
				if(((p1 - p0) % (p0 - IntP)) * n > 0 && IntP.LineD2(p1, p0) > MinW * MinW)
					continue;
				if(((p2 - p1) % (p1 - IntP)) * n > 0 && IntP.LineD2(p2, p1) > MinW * MinW)
					continue;
				if(((p0 - p2) % (p2 - IntP)) * n > 0 && IntP.LineD2(p0, p2) > MinW * MinW)
					continue;
				MinT = t;
				N = n0;
			}
		}
	if(MinT >= 1.e6)
		return 	BPoint(0., 0., 0., -1.);

	if(V * N > 0.)
		N = N * (-1.);
	N = N * BodyRot.Or();
	return (V * MinT + P) * BodyRot.Or();
}
enum NCMApplicationType GQuadRenderMill::GetType() const 
{
	return AT_MILL;
}
void GQuadRenderMill::SetDefaults(NTiParams &Par)
{
	CString Key = "Defaults/Render/Edges";
	EdgesWidth = Par.GetDouble((Key + "@Thick").GetBuffer(), 2.);
	Key = "Defaults/Render/Edges/Color";
	EdgesColor.R() = float(Par.GetDouble((Key + "@R").GetBuffer(), 0.));
	EdgesColor.G() = float(Par.GetDouble((Key + "@G").GetBuffer(), 0.));
	EdgesColor.B() = float(Par.GetDouble((Key + "@B").GetBuffer(), 0.));
	EdgesColor.A() = 1.;
}

void GQuadRenderMill::SetArray(float ***A)
{
	for(int ix = 0; ix < ColsNum; ix++)
		for(int iy = 0; iy < RowsNum; iy++)
			Array[ix][iy] = (GLvoid *)A[ix][iy];
}

bool GQuadRenderMill::MakeEnvelopeCont(std::list<BClrContour>& Contours, TurnGenMode TGMode) const
{
	if (TGMode == TGM_NO)
		return true;

	BDCEL_BSP DCEL;
	GLfloat* pCn = MakeDCEL(DCEL);
	bool res = BRotSolid::MakeEnvConts(DCEL, Contours, TGMode);
	delete[] pCn;
	return res && !Contours.empty();
}

void GQuadRenderMill::ActivateRendArray(double MaxEdgeLength)
{
	typedef RDCEL ** t1;
	typedef RDCEL * t2;
	
	ClearRendArray();

	RendArray = new t1[ColsNum];
	for (int ix = 0; ix < ColsNum; ++ix)
	{
		RendArray[ix] = new t2[RowsNum];
	}
	for (int ix = 0; ix < ColsNum; ++ix)
	{
		for (int iy = 0; iy < RowsNum; ++iy)
		{
			if (Empty(Array[ix][iy]))
				RendArray[ix][iy] = nullptr;
			else
			{
				BDCEL_BSP EL;
				EL.MakeFromStockTri((GLfloat *)Array[ix][iy]);
				EL.Switch2TRMMode();
				EL.DivideEdges(MaxEdgeLength, 2000);
				RendArray[ix][iy] = new RDCEL(EL);
			}
		}
	}
}

void GQuadRenderMill::ClearRendArray()
{
	RendArrayReady = false;
	if (RendArray == nullptr)
		return;
	for (int ix = 0; ix < ColsNum; ++ix)
	{
		for (int iy = 0; iy < RowsNum; ++iy)
			delete RendArray[ix][iy];
		delete[] RendArray[ix];
	}
	delete[] RendArray;
	RendArray = nullptr;
}

RDCEL *GQuadRenderMill::GetRend(int ix, int iy) const
{
	if (RendArray == nullptr)
		return nullptr;
	return RendArray[ix][iy];
}
