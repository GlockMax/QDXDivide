// MBSPNode.cpp: implementation of the MBSPNode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MBSPNode.h"
#include "MBSPTree.h"
#include "MBSPForest.h"
#include "BSPEnums.h"
#include "MRay.h"
#include "math.h"
#include "ConstDef.h"
#include "SOTorus.h"
#include "mbspnode.h"
#include "MBodyParams.h"
#include "MForestSave.h"
#include "CopyMap.h"
#include "BDynBitArray.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MBSPNode::MBSPNode() : m_cadrID(-1), MTreeNode()
{
	m_indFace = -1;
	m_basFace = ID_EMPTY + SURF_NUM_SHIFT;
}

MBSPNode::~MBSPNode()
{

}

inline const BPoint* MBSPNode::GetPoint(int index) const
{
#ifdef _DEBUG
	const MFace* pFace = GetFace();
	if (0 < index && index > pFace->GetCount())
		return NULL;
#endif
	return GetFace()->GetPoint(index);
}

//void MBSPNode::SetFace(const MFace *pFace)
//{
//	m_indFace = MFOREST.AddFaces(pFace);
//}

void MBSPNode::SetFace(const MFace& Face)
{
	m_indFace = MFOREST.AddFace(Face);
}

void MBSPNode::SetFace(int iF)
{
	m_indFace = iF;
}

const MFace* MBSPNode::GetFace() const
{
	return MFOREST.GetFace(m_indFace);
}

MFace* MBSPNode::GetFacePtr() const
{
	return MFOREST.GetFacePtr(m_indFace);;
}

ElemID MBSPNode::GetBFaceN() const
{
	return m_basFace;
}

void MBSPNode::DrawTransp(const BPoint& VDir, double Coef) const
{
	bool flag = VDir * GetFace()->GetPlane().GetNormal() > 0.;
	MBSPNode* Far, * Near;
	if (flag)
	{// Right son is near to the view point
		Far = GetLeft();
		Near = GetRight();
	}
	else
	{// Right son is far from the view point
		Far = GetRight();
		Near = GetLeft();
	}
	if (!IsTerminal(Far))
		Far->DrawTransp(VDir, Coef);
	DrawFacet(Coef);
	//	if(glIsList(m_basFace))
	//		glCallList(m_basFace);
	if (!IsTerminal(Near))
		Near->DrawTransp(VDir, Coef);
}



M_POSITION MBSPNode::TrimPos(const MPlane& Plane)
{// Trim this and all descendant faces by Plane (keep positive side)
	MFace F1, F2;
	M_POSITION res = GetFace()->DivideByPlane(Plane, F1, F2);
	switch (res)
	{
	case M_INT:
		ReplaceFacePoints(F1);
		break;
	case M_POS:
	case M_IDN:
		break;
	case M_NEG:
	{
		SetFace(MFace(GetFace()->GetPlane().GetNormalN(), GetFace()->GetPlane().GetPointN()));
		if (DEBUGMODE)
		{
			LOGFILE.WriteString("TrimPos:Remove\n");
		}
	}
	break;
	case M_ERR:
		if (DEBUGMODE)
		{
			LOGFILE.WriteString("TrimPos:M_ERR\n");
		}
		break;
	}
	MBSPNode* pNode = (MBSPNode*)GetRight();
	if (!IsTerminal(pNode))
	{
		if (pNode->TrimPos(Plane) == M_NEG)
		{
			if (pNode->GetFace()->GetPlane().GetNormal() * GetFace()->GetPlane().GetNormal() > 0)
				SetRight(pNode->GetRight());
			else
				SetRight(pNode->GetLeft());
			delete pNode;
		}
	}
	pNode = (MBSPNode*)GetLeft();
	if (!IsTerminal(pNode))
	{
		if (pNode->TrimPos(Plane) == M_NEG)
		{
			if (pNode->GetFace()->GetPlane().GetNormal() * GetFace()->GetPlane().GetNormal() > 0)
				SetLeft(pNode->GetLeft());
			else
				SetLeft(pNode->GetRight());
			delete pNode;
		}
	}
	return res;
}
M_POSITION MBSPNode::TrimNeg(const MPlane& Plane)
{// Trim this and all descendant faces by Plane (keep positive side)
	MFace F2, F1;
	M_POSITION res = GetFace()->DivideByPlane(Plane, F1, F2);
	switch (res)
	{
	case M_INT:
		ReplaceFacePoints(F2);
		break;
	case M_NEG:
	case M_IDN:
		break;
	case M_POS:
	{
		MFace F(GetFace()->GetPlane().GetNormalN(), GetFace()->GetPlane().GetPointN());
		SetFace(F);
		if (DEBUGMODE)
		{
			LOGFILE.WriteString("TrimNeg:Remove\n");
		}
	}
	break;
	case M_ERR:
		if (DEBUGMODE)
		{
			LOGFILE.WriteString("TrimPos:ERROR\n");
		}
		break;
	}
	MBSPNode* pNode = (MBSPNode*)GetRight();
	if (!IsTerminal(pNode))
	{
		if (pNode->TrimNeg(Plane) == M_POS)
		{
			if (pNode->GetFace()->GetPlane().GetNormal() * GetFace()->GetPlane().GetNormal() > 0)
				SetRight(pNode->GetRight());
			else
				SetRight(pNode->GetLeft());
			delete pNode;
		}
	}
	pNode = (MBSPNode*)GetLeft();
	if (!IsTerminal(pNode))
	{
		if (pNode->TrimNeg(Plane) == M_POS)
		{
			if (pNode->GetFace()->GetPlane().GetNormal() * GetFace()->GetPlane().GetNormal() > 0)
				SetLeft(pNode->GetLeft());
			else
				SetLeft(pNode->GetRight());
			delete pNode;
		}
	}
	return res;
}

void MBSPNode::ReplaceFacePoints(const MFace& InFace)
{
	if (m_indFace < 0)
	{
		MFOREST.GetFacePtr(m_indFace)->ReplacePoints(InFace);
	}
	else
	{
		MFace F(*GetFace());
		F.ReplacePoints(InFace);
		SetFace(F);
	}
}

void MBSPNode::MakeAllPlanes(void)
{
	MFace F(GetFace()->GetPlane().GetNormalN(), GetFace()->GetPlane().GetPointN());
	F.CrPlane();
	SetFace(F);
	if (!IsTerminal(m_pRight))
		((MBSPNode*)m_pRight)->MakeAllPlanes();
	if (!IsTerminal(m_pLeft))
		((MBSPNode*)m_pLeft)->MakeAllPlanes();
}

void MBSPNode::Trim(void)
{
	MBSPNode* pNode = (MBSPNode*)GetLeft();
	if (!IsTerminal(pNode))
	{
		if (pNode->TrimPos(GetFace()->GetPlane()) == M_NEG)
		{
			if (pNode->GetFace()->GetPlane().GetNormal() * GetFace()->GetPlane().GetNormal() > 0)
				SetLeft(pNode->GetLeft());
			else
				SetLeft(pNode->GetRight());
			delete pNode;
		}
		if (!IsTerminal(GetLeft()))
			((MBSPNode*)GetLeft())->Trim();
	}
	pNode = (MBSPNode*)GetRight();
	if (!IsTerminal(pNode))
	{
		if (pNode->TrimNeg(GetFace()->GetPlane()) == M_POS)
		{
			if (pNode->GetFace()->GetPlane().GetNormal() * GetFace()->GetPlane().GetNormal() > 0)
				SetRight(pNode->GetRight());
			else
				SetRight(pNode->GetLeft());
			delete pNode;
		}
		if (!IsTerminal(GetRight()))
			((MBSPNode*)GetRight())->Trim();
	}
}

bool MBSPNode::Contains(const BPoint& P) const
{
	if ((P - GetFace()->GetPlane().GetPoint()) * GetFace()->GetPlane().GetNormal() > 0)
	{
		if (IsTerminal(GetLeft()))
			return GetLeft() == (MTreeNode*)M_IN_PTR;
		else
			return ((MBSPNode*)GetLeft())->Contains(P);
	}
	else
	{
		if (IsTerminal(GetRight()))
			return GetRight() == (MTreeNode*)M_IN_PTR;
		else
			return ((MBSPNode*)GetRight())->Contains(P);
	}
}

MBSPNode* MBSPNode::CheckStatus(void)
{
	MBSPNode* LeftStatus = (MBSPNode*)GetLeft();
	if (!IsTerminal(LeftStatus))
	{
		MBSPNode* S = LeftStatus->CheckStatus();
		if (IsTerminal(S))
		{
			delete LeftStatus;
			LeftStatus = S;
			SetLeft(S);
		}
	}
	MBSPNode* RightStatus = (MBSPNode*)GetRight();
	if (!IsTerminal(RightStatus))
	{
		MBSPNode* S = RightStatus->CheckStatus();
		if (IsTerminal(S))
		{
			delete RightStatus;
			RightStatus = S;
			SetRight(S);
		}
	}
	if (LeftStatus == RightStatus)
		return LeftStatus;
	return this;
}

void MBSPNode::SetBFace(int BasFaceNum)
{
	m_basFace = BasFaceNum;
}

int MBSPNode::CrSection(const MBSPNode& Root)
{// Creates subdivision of "this" face by the BSP tree started at Root
 // Creates list of MBSPNodes starting at this node. Each node contains one face of section
 // Pointer to the left son points to next node. Right pointer indicates a type of face
 // M_IN_PTR if face lies inside the body and M_OUT_PTR otherwise
 // Returns number of faces in the list
	int calc = 0;
	MFace P1, P2;
	/*
	static count = 0;
	count ++;
	if(count > 33)
		AfxMessageBox("CrSection");
		*/
	M_POSITION res = GetFace()->DivideByPlane(Root.GetFace()->GetPlane(), P1, P2);
	if (res == M_INT)
	{
		if (P1.Area2() < MINS * MINS)
			res = M_NEG;
		else
			if (P2.Area2() < MINS * MINS)
				res = M_POS;
	}
	switch (res)
	{
	case M_INT:
	{
		calc += 1;
		ReplaceFacePoints(P1);
		MBSPNode* pNode = new MBSPNode(*this);
//		pNode->SetBFace(GetBFaceN());
//		pNode->SetLeft(GetLeft());// To build continuous list
		SetLeft(pNode);// next node
		if (IsTerminal(Root.GetLeft()))
			SetRight(Root.GetLeft());
		else
			calc += CrSection(*Root.GetLeft());
		pNode->SetFace(P2);// Must operate with pNode, i.e. GetLeft may be changed
		if (IsTerminal(Root.GetRight()))
			pNode->SetRight(Root.GetRight());
		else
			calc += pNode->CrSection(*Root.GetRight());
	}
	break;
	case M_POS:
		if (IsTerminal(Root.GetLeft()))
			SetRight(Root.GetLeft());
		else
			calc += CrSection(*Root.GetLeft());
		break;
	case M_NEG:
		if (IsTerminal(Root.GetRight()))
			SetRight(Root.GetRight());
		else
			calc += CrSection(*Root.GetRight());
		break;
	case M_IDN:
	{
		MPlane OldPlane = GetFace()->GetPlane();

		MPlane NewPlane(OldPlane);
		// Move plane
		NewPlane.SetPointOnPlane(
			OldPlane.GetPoint() + OldPlane.GetNormal() * 0.001
		);
		GetFacePtr()->SetPlane(NewPlane);
		CrSection(Root);
		GetFacePtr()->SetPlane(OldPlane);
	}
	break;
	case M_ERR:
		if (DEBUGMODE)
			LOGFILE.WriteString("CrSection:M_ERR\n");
		break;
	case M_EMP:
		if (DEBUGMODE)
			LOGFILE.WriteString("CrSection:M_EMP\n");
		break;
	default:
		break;
	}
	return calc;
}

MBSPNode** MBSPNode::GetLeftPtr(void) const
{
	return (MBSPNode**)&m_pLeft;
}
MBSPNode** MBSPNode::GetRightPtr(void) const
{
	return (MBSPNode**)&m_pRight;
}

void MBSPNode::Invert(void)
{
	m_pLeft = MBSPTree::Invert((MBSPNode*)m_pLeft);
	m_pRight = MBSPTree::Invert((MBSPNode*)m_pRight);
}

int MBSPNode::CrFacet(MBSPNode& FacetRoot) const
{
	// Create left subtree section
	FacetRoot.SetLeft((MBSPNode*)M_NDEF_PTR);
	FacetRoot.SetFace(*GetFace());// Must create new copy of this face because it will be changed
	FacetRoot.SetBFace(GetBFaceN());
	if (IsTerminal(GetLeft()))
		FacetRoot.SetRight(GetLeft());
	else
	{
		FacetRoot.SetRight((MBSPNode*)M_NDEF_PTR);
		FacetRoot.CrSection(*GetLeft());
	}
	// For each node in FacetRoot list
	for (MBSPNode* pNode = &FacetRoot; !IsTerminal(pNode); pNode = pNode->GetLeft())
	{
		//Create section of node's face by the right subtree
		MBSPNode* pNewList = new MBSPNode;
		pNewList->SetLeft((MBSPNode*)M_NDEF_PTR);
		pNewList->SetFace(pNode->GetFaceN());
		if (IsTerminal(GetRight()))
			pNewList->SetRight(GetRight());
		else
		{
			pNewList->SetRight((MBSPNode*)M_NDEF_PTR);
			pNewList->CrSection(*GetRight());
		}
		// Remove all extra nodes (nodes with the Right ptr equal to the pNode Right ptr
		ClearSectList(pNewList, pNode->GetRight());
		// Ajoin new list
		pNode->SetRight(pNewList);
	}
	//DEBUG
	//
	//if (MBSPNode::IsTerminal(GetLeft()) != MBSPNode::IsTerminal(GetRight()))
	//{// exactly one child is terminal
	//	const MBSPNode* SignOfTerminal = MBSPNode::IsTerminal(GetLeft()) ? GetLeft() : GetRight();
	//	if (SignOfTerminal == (MBSPNode*)M_OUT_PTR)
	//	{// exactly one child is OUT
	//		for (MBSPNode* pCur = &FacetRoot; !IsTerminal(pCur); pCur = pCur->GetLeft())
	//		{
	//			if(!IsTerminal(pCur->GetRight()))
	//				return 0;// there is non empty part of face
	//		}
	//		return 1;// this node should be removed
	//	}
	//}
	return 0;
}

void MBSPNode::ClearSectList(MBSPNode*& pStartNode, const MBSPNode* Mask)
{// Remove all nodes of Mask type from the section list started at pStartNode
 // pStartNode may be changed if first node is to be removed

	if (IsTerminal(pStartNode))
		return;
	// Find new start node
	for (MBSPNode* pN = pStartNode; !IsTerminal(pN); pN = pStartNode)
	{
		if (pN->GetRight() == Mask)
		{
			pStartNode = pN->GetLeft();
			delete pN;
		}
		else
		{
			if (Mask == (MBSPNode*)M_IN_PTR)// Signum flag
				pN->SetBFace(MElemIDProc::ID_ORIENT);
			else
				pN->SetBFace(MElemIDProc::ID_NULL);
			break;
		}
	}
	if (IsTerminal(pStartNode))
		return;
	// Remove internal Mask nodes
	MBSPNode* pPrevN = pStartNode;
	MBSPNode* pNextN;
	for (MBSPNode* pN = pPrevN->GetLeft(); !IsTerminal(pN); pN = pNextN)
	{
		pNextN = pN->GetLeft();
		if (pN->GetRight() == (MBSPNode*)Mask)
		{
			pPrevN->SetLeft(pNextN);
			delete pN;
		}
		else
		{
			pPrevN = pN;
			if (Mask == (MBSPNode*)M_IN_PTR)// Signum flag
				pN->SetBFace(MElemIDProc::ID_ORIENT);
			else
				pN->SetBFace(MElemIDProc::ID_NULL);
		}
	}
}

int MBSPNode::DrawFacet(double Coef) const
{
	MFOREST.SetMark();
	MBSPNode FacetRoot;
	CrFacet(FacetRoot);
	bool fl = false;
	for (MBSPNode* pNode = &FacetRoot; !IsTerminal(pNode) && !fl; pNode = pNode->GetLeft())
	{
		for (MBSPNode* pN = pNode->GetRight(); !IsTerminal(pN) && !fl; pN = pN->GetLeft())
		{
			if (pN->GetFace()->Area2() > MINST* MINST)
				fl = true;
		}
	}
	if (fl)
	{
		for (MBSPNode* pNode = &FacetRoot; !IsTerminal(pNode); pNode = pNode->GetLeft())
		{
			ElemID ElID = pNode->GetBFaceN();
			int BFaceN = MFOREST.GetIDProc()->GetSurfID(ElID);
			PackedColor col = MFOREST.GetIDProc()->GetColor(ElID);
			glColor4ub(col.fr, col.fg, col.fb, GLubyte(col.fa * (1. - Coef)));

			bool SmoothLoc = /*Smooth*/ true && (BFaceN >= 0 && BFaceN != ID_EMPTY);
			for (MBSPNode* pN = pNode->GetRight(); !IsTerminal(pN); pN = pN->GetLeft())
			{
				const MFace* pFace = pN->GetFace();
				if (pFace->Area2() > MINST* MINST&& pFace->GetCount() > 2)
				{
					BPoint n0b(pFace->GetPlane().GetNormal());
					double dirNk = -1.;// заменено после добавления знаков радиуса в GetNormal

					if (pN->GetFace()->GetCount() == 3)
						glBegin(GL_TRIANGLES);
					else
						glBegin(GL_TRIANGLE_FAN);
					for (int i = 0; i < pN->GetFace()->GetCount(); i++)
					{
						const BPoint* p = pFace->GetPoint(i);
						BPoint n(n0b);
						if (SmoothLoc)
						{
							SOrigSurf* pSurf = MFOREST.GetOrigSurf(BFaceN);
							n = pSurf ? pSurf->GetNormal(*p) : n0b;
							if (n.GetH() < -0.5)
								n = n0b;
						}
						glNormal3d(n.GetX() * dirNk, n.GetY() * dirNk, n.GetZ() * dirNk);
						glVertex3d(p->GetX(), p->GetY(), p->GetZ());
					}
					glEnd();
				}
			}
		}
	}
	MFOREST.RetToMark();
	FacetRoot.DeleteAll();
	return 0;
}

void MBSPNode::DrawFacets(void)
{
	if (!IsTerminal(GetLeft()))
		GetLeft()->DrawFacets();

	//	int list = glGenLists(1);
	//	glNewList(list ,GL_COMPILE);
	DrawFacet();
	//	glEndList();
	//	SetBFace(list);

	if (!IsTerminal(GetRight()))
		GetRight()->DrawFacets();
}

void MBSPNode::DrawSection(const MBSPNode& Root)
{
	MBSPNode Sect;
	MFOREST.SetMark();
	Sect.SetFace(*GetFace());
	Sect.CrSection(Root);
	MBSPNode* pCur = &Sect;
	while (pCur != (MBSPNode*)M_NDEF_PTR)
	{
		if (pCur->GetRight() == (MBSPNode*)M_IN_PTR)
			pCur->GetFace()->Draw();
		pCur = pCur->GetLeft();
	}
	Sect.DeleteAll();

	MFOREST.RetToMark();
}

// уточнение вершины используя обратный обход дерева
// параметры:
// pTree	- древо, котрое уточняется
// pRoot	- корневая вершина
//void MBSPNode::ImproveNode(MBSPTree *pTree, MBSPNode *pRoot)
//{
//	if (IsTerminal(this))
//		return ;
//
//	// посетить левую ветвь
//	GetLeft()->ImproveNode(pTree, this);
//
//	// посетить правую ветвь
//	GetRight()->ImproveNode(pTree, this);
//
//	// уточнить вершину
//	GetFace()->RefinedFace(pTree, this);
//}

// ПОПЫТКА ОЦЕНИТЬ ВРЕМЕННЫЕ ЗАТРАТЫ

// версия алгоритма 1
//void MBSPNode::ImproveNode1(MBSPTree *pTree, MBSPNode *pRoot, int &l)
//{
//	l++;
//	bool test = false;
//	if (IsTerminal(this))
//		return ;
//
//	// посетить левую ветвь
//	GetLeft()->ImproveNode1(pTree, this, l);
//
//	// посетить правую ветвь
//	GetRight()->ImproveNode1(pTree, this, l);	
////	test = true;
//
//	// уточнить вершину
//	char str[50];
//	int count=0;
//	GetFace()->RefinedFace1(pTree, this, test, count);
//	if (count > 0)
//	{
//		sprintf(str, "Уточнение Вершины: %d вызывов", count);
//		AfxMessageBox(str);
//	}
//}

// версия алгоритма 2
// уточние дерева начиная с данной вершины
// параметры:
// pTree	- древо, котрое уточняется
// pRoot	- корневая вершина
// bMaybeRefined - флаг, говорит можно ли уточнять вершину
//		(в случае, когда вершина уточняется память выделенная под ней освобождается)
// bLeft	- направление уточнения по дереву
//void MBSPNode::ImproveNode2(MBSPTree *pTree, MBSPNode *pRoot, bool bMaybeRefined, bool bLeft, int &l)
//{
//	l++;
//	if (IsTerminal(this) || !bMaybeRefined)
//		return ;
//
//	// уточнить вершину
//	bool bNextRef = true, f = false;
//	MBSPNode * pNode = GetFace()->RefinedFace2(pTree, pRoot, this, bLeft);
//	if (pNode != this && pNode)
//	//MBSPNode * pNode;
//	//if (GetFace()->LocRefinedFace(pTree, this))
//	{
//		pNode = pRoot;
//		f = true;
//		if (bLeft)
//			pNode->GetLeft()->ImproveNode2(pTree, pNode, bNextRef, true, l);
//		else
//			pNode->GetRight()->ImproveNode2(pTree, pNode, bNextRef, false, l);
//	}
//	else
//	{
//		// посетить левую ветвь
//		pNode->GetLeft()->ImproveNode2(pTree, pNode, bNextRef, true, l);
//		
//		// если было проведено уточнение старые вершины в дереве были удалены
//		// поэтому при повторном прохождении через эти вершины их 
//		// уточнять не нужно (не возможно)	
//		if(f)
//			bNextRef = false;
//
//		// посетить правую ветвь
//		pNode->GetRight()->ImproveNode2(pTree, pNode, bNextRef, false, l);
//	}
//}

//void MBSPNode::NormNorm(void)
//{
//	/*
//	const BPoint *pP = MFOREST.GetPoint(GetFace()->GetPlane().GetNormalN());
//	*pP = *pP * (1./sqrt(pP->D2()));
//	if(!IsTerminal(GetLeft()))
//		GetLeft()->NormNorm();
//	if(!IsTerminal(GetRight()))
//		GetRight()->NormNorm();
//*/
//}
bool MBSPNode::CheckNormals(void)
{
	if (fabs(GetFace()->GetPlane().GetNormal().GetH()) > HMIN)
	{
		AfxMessageBox(IDS_CHSKNORMALS_ERR);
		return false;
	}
	if (!IsTerminal(GetLeft()))
		GetLeft()->CheckNormals();
	if (!IsTerminal(GetRight()))
		GetRight()->CheckNormals();
	return true;


}

int MBSPNode::MakeCellPlanes(const MQuadTree::Cell& Cell)
{
	MFace NewFace;
	int r = Cell.CrStandardFace(GetFace()->GetPlane(), &NewFace);
	int rr = 0, rl = 0;
	switch (r)
	{
	case 0:// Face was created
		SetFace(NewFace);
		if (!IsTerminal(m_pRight))
			rr = ((MBSPNode*)m_pRight)->MakeCellPlanes(Cell);
		if (!IsTerminal(m_pLeft))
			rl = ((MBSPNode*)m_pLeft)->MakeCellPlanes(Cell);
		break;
	case -1:// cell in negative subspace
		if (!IsTerminal(m_pRight))
			rr = ((MBSPNode*)m_pRight)->MakeCellPlanes(Cell);
		break;
	case 1:// cell in positive subspace
		if (!IsTerminal(m_pLeft))
			rl = ((MBSPNode*)m_pLeft)->MakeCellPlanes(Cell);
		break;
	}
	if (rr != 0)
	{
		MBSPNode* pNode = (MBSPNode*)m_pRight;
		SetRight(rr > 0 ? pNode->GetLeft() : pNode->GetRight());
		delete pNode;
	}
	if (rl != 0)
	{
		MBSPNode* pNode = (MBSPNode*)m_pLeft;
		SetLeft(rl > 0 ? pNode->GetLeft() : pNode->GetRight());
		delete pNode;
	}
	return r;
}

void MBSPNode::RenumUsedFaces(const int* newind)
{
	if (m_indFace > 0)
		m_indFace = newind[m_indFace];
	if (!IsTerminal(m_pRight))
		((MBSPNode*)m_pRight)->RenumUsedFaces(newind);
	if (!IsTerminal(m_pLeft))
		((MBSPNode*)m_pLeft)->RenumUsedFaces(newind);
}

void MBSPNode::MarkUsedFacesOrig(class BDynBitArray& UsedF, class BDynBitArray& UsedO)
{
	int Ind = MFOREST.GetIDProc()->GetSurfID(m_basFace);
	if (Ind >= 0 && Ind != ID_EMPTY)
		UsedO.SetTrueAt(Ind);
	UsedF.SetTrueAt(m_indFace);
	if (!IsTerminal(m_pRight))
		((MBSPNode*)m_pRight)->MarkUsedFacesOrig(UsedF, UsedO);
	if (!IsTerminal(m_pLeft))
		((MBSPNode*)m_pLeft)->MarkUsedFacesOrig(UsedF, UsedO);
}
void MBSPNode::MarkUsedOrig(class BDynBitArray& Used)
{
	int Ind = MFOREST.GetIDProc()->GetSurfID(m_basFace);
	if (Ind >= 0 && Ind != ID_EMPTY)
		Used.SetTrueAt(Ind);
	if (!IsTerminal(m_pRight))
		((MBSPNode*)m_pRight)->MarkUsedOrig(Used);
	if (!IsTerminal(m_pLeft))
		((MBSPNode*)m_pLeft)->MarkUsedOrig(Used);
}

int MBSPNode::SortFaces(const MBSPNode* pBase)
{
	pBase;
	return 0;
}

void MBSPNode::DrawFaces(const BPoint& VDir)
{
	bool flag = VDir * GetFace()->GetPlane().GetNormal() > 0.;
	MBSPNode* Far, * Near;
	if (flag)
	{// Right son is nearer to the view point
		Far = GetLeft();
		Near = GetRight();
	}
	else
	{// Right son is father from the view point
		Far = GetRight();
		Near = GetLeft();
	}
	if (!IsTerminal(Far))
		Far->DrawFaces(VDir);
	{
		BPoint n = GetFace()->GetPlane().GetNormal();
		glNormal3d(n.GetX(), n.GetY(), n.GetZ());
		GetFace()->Draw();
	}

	if (!IsTerminal(Near))
		Near->DrawFaces(VDir);
}


int MBSPNode::CrFacets(MBSPNode& FacetRoot) const
{
	if (GetBFaceN() != MElemIDProc::ID_DUMMY)
	{
		const int res = CrFacet(FacetRoot);
		//DEBUG
		//if (res == 1) // Mark this node to delete
		//{
		//	MTreeNode& No = *(MTreeNode*)this;
		//	((MBSPNode&)No).SetBFace(MElemIDProc::ID_REMOVE);
		//}
	}

	MBSPNode* pNewNode;
	MBSPNode* pNode;
	for (pNode = &FacetRoot; !IsTerminal(pNode->GetLeft()); pNode = pNode->GetLeft());
	if (!IsTerminal(GetLeft()))
	{
		pNewNode = new MBSPNode;
		pNode->SetLeft(pNewNode);
		GetLeft()->CrFacets(*pNewNode);
		for (pNode = pNewNode; !IsTerminal(pNode->GetLeft()); pNode = pNode->GetLeft());
	}
	if (!IsTerminal(GetRight()))
	{
		pNewNode = new MBSPNode;
		pNode->SetLeft(pNewNode);
		GetRight()->CrFacets(*pNewNode);
	}
	return 0;
}

MBSPNode* MBSPNode::CopySubtree(void)
{
	MBSPNode* pOut = new MBSPNode;
	pOut->SetCadrID(GetCadrID());
	pOut->SetBFace(GetBFaceN());
	pOut->SetFace(*GetFace());
	pOut->SetLeft(MBSPTree::CopySubtree(GetLeft()));
	pOut->SetRight(MBSPTree::CopySubtree(GetRight()));
	return pOut;
}

MBSPNode* MBSPNode::CopySubtreeFull(class CopyMap& Map)
{
	MBSPNode* pOut = new MBSPNode;
	pOut->SetCadrID(GetCadrID());
	unsigned int BFaceN = GetBFaceN();
	int SurfInd = MFOREST.GetIDProc()->GetSurfID(BFaceN);
	MFOREST.GetIDProc()->SetSurfID(&BFaceN, Map.AddOldIndOr(SurfInd));
	pOut->SetBFace(BFaceN);
	pOut->SetCadrID(GetCadrID());

	if (GetFaceN() >= 0)
	{
		MFace NewFace(*GetFace());
		NewFace.GetPlane().SetNormalN(Map.AddOldIndPt(NewFace.GetPlane().GetNormalN()));
		NewFace.GetPlane().SetPointN(Map.AddOldIndPt(NewFace.GetPlane().GetPointN()));
		for (int i = 0; i < NewFace.GetCount(); ++i)
			NewFace.SetPointAt(i, Map.AddOldIndPt(NewFace.GetIndPoint(i)));

		pOut->SetFace(NewFace);
	}
	else
	{
		pOut->SetFace(GetFaceN());
	}

	pOut->SetLeft(MBSPTree::CopySubtreeFull(GetLeft(), Map));
	pOut->SetRight(MBSPTree::CopySubtreeFull(GetRight(), Map));
	return pOut;
}

void MBSPNode::GetTransfData(MBSPForest::NTransfData& TransfData) const
{
	const MFace* pFace = GetFace();
	// Points
	int Ind = pFace->GetPlane().GetNormalN();
	if (Ind >= 0)
		TransfData.PointsIndexes.emplace(Ind);
	Ind = pFace->GetPlane().GetPointN();
	if (Ind >= 0)
		TransfData.PointsIndexes.emplace(Ind);
	for (int i = 0; i < pFace->GetCount(); ++i)
	{
		Ind = pFace->GetPointN(i);
		if (Ind >= 0)
			TransfData.PointsIndexes.emplace(Ind);
	}

	// Surfaces
	int SurfInd = MFOREST.GetIDProc()->GetSurfID(GetBFaceN());
	if (SurfInd >= 0 && SurfInd != ID_EMPTY)
		TransfData.SurfIndexes.emplace(SurfInd);

	// Recursion
	if (!IsTerminal(GetLeft()))
		GetLeft()->GetTransfData(TransfData);
	if (!IsTerminal(GetRight()))
		GetRight()->GetTransfData(TransfData);
}

MBSPForest::ForestSizes MBSPNode::GetSizes() const
{
	MBSPForest::ForestSizes Sizes;
	std::vector<std::pair<const MBSPNode*, int >> Nodes;
	Nodes.push_back(std::pair<const MBSPNode*, int >(this, 1));
	while (!Nodes.empty())
	{
		int Depth = Nodes.back().second;
		Sizes.MaxTreeDepth = max(Sizes.MaxTreeDepth, Depth);
		const MBSPNode* pNode = Nodes.back().first;
		Sizes.Trees += sizeof(*pNode);
		Nodes.pop_back();
		if (!IsTerminal(pNode->GetLeft()))
			Nodes.push_back(std::pair<const MBSPNode*, int >(pNode->GetLeft(), Depth + 1));
		if (!IsTerminal(pNode->GetRight()))
			Nodes.push_back(std::pair<const MBSPNode*, int >(pNode->GetRight(), Depth + 1));
	}
	return Sizes;
}

MBSPNode* MBSPNode::FindNodeContaining(const MBSPNode* InOut)
{
	if (GetLeft() == InOut)
		return this;
	if (GetRight() == InOut)
		return this;
	MBSPNode* node = (MBSPNode*)M_NDEF_PTR;
	if (!IsTerminal(GetLeft()))
		node = GetLeft()->FindNodeContaining(InOut);
	if (node != (MBSPNode*)M_NDEF_PTR)
		return node;
	if (!IsTerminal(GetRight()))
		node = GetRight()->FindNodeContaining(InOut);
	return node;
}

//MBodyParams MBSPNode::GetParams(void)
//{
//	MBodyParams Par;
//	MFOREST.SetMark();
//	MBSPNode FacetRoot;
//	CrFacets(FacetRoot);
//	for(MBSPNode *pNode = &FacetRoot ; !MBSPNode::IsTerminal(pNode); pNode = pNode->GetLeft())
//	{
//		for(MBSPNode *pN = pNode->GetRight(); !MBSPNode::IsTerminal(pN); pN = pN->GetLeft())
//		{
//			const MFace *pFace = pN->GetFace();
//			if(pFace->GetCount() > 2)
//			{
//				const BPoint *p0 = pFace->GetPoint(0);
//				Par.Bounds.Expand(*p0);
//				BPoint vb(*p0 - BPoint(0.,0.,0.,1));
//				BPoint n0 = pFace->GetPlane().GetNormal();
//				const BPoint *p1 = pFace->GetPoint(1);
//				Par.Bounds.Expand(*p1);
//				BPoint vp(*p1 - *p0);
//				for (int i=2; i < pFace->GetCount(); i++)
//				{
//					const BPoint *p = pFace->GetPoint(i);
//					Par.Bounds.Expand(*p);
//					BPoint vn(*p - *p0);
//					BPoint s(vp%vn);
//					Par.SurfArea += 0.5*sqrt(s.D2());
//					Par.Volume += sqrt(s.D2())*(vb*n0)/(-6.*sqrt(n0.D2()));
//					vp = vn;
//				}
//			}
//		}
//	}
//	MFOREST.RetToMark();
//	FacetRoot.DeleteAll();
//	return Par;
//}
void MBSPNode::Save(MForestSave& SaveData)
{
	RWBlock Block;
	CFile* f = SaveData.File;
	const MFace* pFace = GetFace(); 
	Block.b = SaveData.GetPointInd(pFace->GetPlane().GetPointN());
	Block.n = SaveData.GetPointInd(pFace->GetPlane().GetNormalN());
	Block.nBFace = SaveData.GetOrigInd(m_basFace);
	MBSPNode* pNode = GetLeft();
	Block.pL = pNode;
	Block.pR = GetRight();;
	f->Write(&Block, sizeof(Block));
	if (!MBSPNode::IsTerminal(pNode))
		pNode->Save(SaveData);
	pNode = Block.pR;
	if (!MBSPNode::IsTerminal(pNode))
		pNode->Save(SaveData);
}
void MBSPNode::Load(CFile* f)
{
	RWBlock Block;
	f->Read(&Block, sizeof(Block));
	MFace F(Block.n, Block.b);
	SetFace(F);
	m_basFace = Block.nBFace;
	if (MElemIDProc::GetPCn(m_basFace) != FAST_COLOR_NUM) // change all colors except FAST_COLOR_NUM to STOCK_COLOR_NUM
		MElemIDProc::SetPCn(&m_basFace, STOCK_COLOR_NUM);
	//read left
	MBSPNode* pNode = Block.pL;
	if (!MBSPNode::IsTerminal(pNode))
	{
		pNode = new MBSPNode;
		pNode->Load(f);
	}
	SetLeft(pNode);
	//read right
	pNode = Block.pR;
	if (!MBSPNode::IsTerminal(pNode))
	{
		pNode = new MBSPNode;
		pNode->Load(f);
	}
	SetRight(pNode);
}

void MBSPNode::ReplPInd(const int* In2Out)
{
	MFace* pFace = MFOREST.GetFacePtr(m_indFace);
	if (pFace == nullptr)
		return;
	MPlane& Plane = pFace->GetPlane();
	int PtN = Plane.GetPointN();
	if (PtN >= 0)
		PtN = In2Out[PtN];
	Plane.SetPointN(PtN);
	int NoN = Plane.GetNormalN();
	if (NoN >= 0)
		NoN = In2Out[NoN];
	Plane.SetNormalN(NoN);

	if (!MBSPNode::IsTerminal(GetLeft()))
		GetLeft()->ReplPInd(In2Out);
	if (!MBSPNode::IsTerminal(GetRight()))
		GetRight()->ReplPInd(In2Out);
}

void MBSPNode::ReplOInd(const int* In2Out)
{
	ElemID ElID = GetBFaceN();
	int BFaceN = MFOREST.GetIDProc()->GetSurfID(ElID);
	if (BFaceN >= 0 && BFaceN != ID_EMPTY)
	{
		BFaceN = In2Out[BFaceN];
		MFOREST.GetIDProc()->SetSurfID(&ElID, BFaceN);
	}
	SetBFace(ElID);

	if (!MBSPNode::IsTerminal(GetLeft()))
		GetLeft()->ReplOInd(In2Out);
	if (!MBSPNode::IsTerminal(GetRight()))
		GetRight()->ReplOInd(In2Out);
}

void MBSPNode::RayShredding(const BPoint& P, const BPoint& V, std::list<MBSPNode*>& Nodes) const
{// DO NOT CALL! Call RayShreddingSpec instead. This method don't use ActLocStorage
	// Define large line
	BPoint Pbs = P;
	BPoint Pes = P + V * MAXC;
	double tb = 0, te = 1;// begin and end parameters of subline
	auto pSt = std::make_unique<StackForShooting>();
	StackForShooting& St = *pSt;//StackForShooting St; -> warning C6262 (stack size)
	St.Push((MBSPNode*)this, tb, te, (MBSPNode*)M_NDEF_PTR);
	MBSPNode* pNode;
	MBSPNode* pRetNode;
	MBSPNode* pPrev = (MBSPNode*)M_NDEF_PTR;
	while (St.Pop(pNode, tb, te, pRetNode))
	{
		if (IsTerminal(pNode))
		{
			if (pRetNode == (MBSPNode*)M_NDEF_PTR || pNode == pPrev) // Additional node
			{
				pPrev = pNode;
				continue;
			}
			pPrev = pNode;
			Nodes.push_back(pRetNode);
			continue;
		}
		const MPlane& Plt = pNode->GetFace()->GetPlane();
		double sb = Plt.CalcSign(Pbs * (1. - tb) + Pes * tb);
		double se = Plt.CalcSign(Pbs * (1. - te) + Pes * te);
		if (sb * se >= 0.)
		{
			if (max(sb, se) > 0.)
				St.Push(pNode->GetLeft(), tb, te, pRetNode);
			else
				St.Push(pNode->GetRight(), tb, te, pRetNode);
			continue;
		}
		// sb*se < 0
		bool orient = V * Plt.GetNormal() < 0.;
		// true - "near" = left
		MBSPNode* pNear, * pFar;
		if (orient)
		{
			pNear = pNode->GetLeft();
			pFar = pNode->GetRight();
		}
		else
		{
			pFar = pNode->GetLeft();
			pNear = pNode->GetRight();
		}
		double tt = (tb * fabs(se) + te * fabs(sb)) / (fabs(sb) + fabs(se));
		St.Push(pFar, tt, te, pNode);
		St.Push(pNear, tb, tt, pRetNode);
	}
}

void MBSPNode::RayShreddingSpec(const BPoint& P, const BPoint& V, MQuadTree& Tree, StackForShooting& St, std::list<std::pair<BPoint, unsigned int>>& Nodes) const
{
	// this method can work with special tree only
	// The tree mast be created by MQuadTree::MakeTree
	// Define large line
	BPoint Pbs = P;
	BPoint Pes = P + V * MAXC;
	BPoint Qv = Pes - Pbs;// for speedup
	double tb = 0, te = 1;// begin and end parameters of subline
	St.Clear();
	St.Push((MBSPNode*)this, tb, te, (MBSPNode*)M_NDEF_PTR);
	std::vector<IndexPairRange> IPRSt;
	IPRSt.push_back(IndexPairRange(0, Tree.GetColsNum(), 0, Tree.GetRowsNum()));
	MBSPNode* pNode;
	MBSPNode* pRetNode;
	MBSPNode* pPrev = (MBSPNode*)M_NDEF_PTR;
	while (St.Pop(pNode, tb, te, pRetNode))
	{
		IndexPairRange Range0, Range1;
		IndexPairRange Range = IPRSt.back();
		IPRSt.pop_back();
		MQuadTree::ActLocStorage Act(Tree, Range.GetX(), Range.GetY());
		if (IsTerminal(pNode))
		{
			if (pRetNode == (MBSPNode*)M_NDEF_PTR || pNode == pPrev) // Additional node
			{
				pPrev = pNode;
				continue;
			}
			pPrev = pNode;
			BPoint R;
			pRetNode->GetFace()->GetPlane().IntersLine(P, V, R);
			Nodes.push_back(std::pair<BPoint, unsigned int>(R, pRetNode->GetBFaceN()));
			continue;
		}

		const MPlane& Plt = pNode->GetFace()->GetPlane();
		//double sb = Plt.CalcSign(Pbs * (1. - tb) + Pes * tb);
		//double se = Plt.CalcSign(Pbs * (1. - te) + Pes * te);
		// next fragment is for speedup above 2 lines only
		const BPoint& PlaneP = Plt.GetPoint();
		const BPoint& PlaneN = Plt.GetNormal();
		double s1 = (Pbs - PlaneP) * PlaneN;
		double s2 = Qv * PlaneN;
		double sb = s1 + s2 * tb;
		double se = s1 + s2 * te;

		Range.Divide(Range0, Range1);
		if (sb * se >= 0.)
		{

			if (max(sb, se) > 0.)
			{
				IPRSt.push_back(Range1);
				St.Push(pNode->GetLeft(), tb, te, pRetNode);
			}
			else
			{
				IPRSt.push_back(Range0);
				St.Push(pNode->GetRight(), tb, te, pRetNode);
			}
			continue;
		}
		// sb*se < 0
		bool orient = V * Plt.GetNormal() < 0.;
		// true - "near" = left
		MBSPNode* pNear, * pFar;
		if (orient)
		{
			IPRSt.push_back(Range0);
			IPRSt.push_back(Range1);
			pNear = pNode->GetLeft();
			pFar = pNode->GetRight();
		}
		else
		{
			IPRSt.push_back(Range1);
			IPRSt.push_back(Range0);
			pFar = pNode->GetLeft();
			pNear = pNode->GetRight();
		}
		double tt = (tb * fabs(se) + te * fabs(sb)) / (fabs(sb) + fabs(se));
		St.Push(pFar, tt, te, pNode);
		St.Push(pNear, tb, tt, pRetNode);
	}
	//	delete pSt;
}


MBSPNode* MBSPNode::RayShooting(const BPoint& P, const BPoint& V, bool type) const
{
	// Define large line
	BPoint Pbs = P;
	BPoint Pes = P + V;
	double tb = 0, te = 1;// begin and end parameters of subline
	StackForShooting St;
	St.Push((MBSPNode*)this, tb, te, (MBSPNode*)M_NDEF_PTR);
	MBSPNode* pNode;
	MBSPNode* pRetNode;
	while (St.Pop(pNode, tb, te, pRetNode))
	{
		if (IsTerminal(pNode))
		{
			if (pRetNode == (MBSPNode*)M_NDEF_PTR)
				continue;
			if (pNode == (MBSPNode*)M_IN_PTR && type)
				return pRetNode;
			if (pNode == (MBSPNode*)M_OUT_PTR && !type)
				return pRetNode;
			continue;
		}
		const MPlane& Plt = pNode->GetFace()->GetPlane();
		double sb = Plt.CalcSign(Pbs * (1. - tb) + Pes * tb);
		double se = Plt.CalcSign(Pbs * (1. - te) + Pes * te);
		if (sb * se >= 0.)
		{
			if (max(sb, se) > 0.)
				St.Push(pNode->GetLeft(), tb, te, pRetNode);
			else
				St.Push(pNode->GetRight(), tb, te, pRetNode);
			continue;
		}
		// sb*se < 0
		bool orient = V * Plt.GetNormal() < 0.;
		// true - "near" = left
		MBSPNode* pNear, * pFar;
		if (orient)
		{
			pNear = pNode->GetLeft();
			pFar = pNode->GetRight();
		}
		else
		{
			pFar = pNode->GetLeft();
			pNear = pNode->GetRight();
		}
		double tt = (tb * fabs(se) + te * fabs(sb)) / (fabs(sb) + fabs(se));
		St.Push(pFar, tt, te, pNode);
		St.Push(pNear, tb, tt, pRetNode);
	}
	return (MBSPNode*)M_NDEF_PTR;
}

bool MBSPNode::Collapse(MBSPNode*& InNode)
{
	// Return true if some nodes where collapsed
	if (IsTerminal(InNode))
		return false;
	bool Res = Collapse((MBSPNode*&)(InNode->m_pLeft));
	Res |= Collapse((MBSPNode*&)(InNode->m_pRight));
	if (InNode->m_pLeft == InNode->m_pRight)
	{
		MBSPNode* buf = InNode;
		InNode = (MBSPNode*)InNode->m_pLeft;
		delete buf;
		return true;
	}
	return Res;
}

bool MBSPNode::ClearMarked(MBSPNode*& InNode)
{
	// Return true if some nodes where deleted
	if (IsTerminal(InNode))
		return false;
	bool Res = ClearMarked((MBSPNode*&)(InNode->m_pLeft));
	Res |= ClearMarked((MBSPNode*&)(InNode->m_pRight));
	if (InNode->GetBFaceN() == MElemIDProc::ID_REMOVE)
	{
		MBSPNode* buf = InNode;
		InNode = (InNode->m_pRight == (MBSPNode*)M_OUT_PTR) ? (MBSPNode*)InNode->m_pLeft : (MBSPNode*)InNode->m_pRight;
		delete buf;
		return true;
	}
	return Res;
}

void MBSPNode::Replace(MBSPNode* node, MBSPNode* what, MBSPNode* with)
{
	if (MBSPNode::IsTerminal(node))
		return;
	if (node->GetLeft() == what)
		node->SetLeft(with);
	if (node->GetRight() == what)
		node->SetRight(with);
	Replace(node->GetLeft(), what, with);
	Replace(node->GetRight(), what, with);
}

void MBSPNode::IndexPairRange::Divide(IndexPairRange& Range0, IndexPairRange& Range1)
{
	++TreeLevel;
	Range0 = *this;
	Range1 = *this;
	if (isSingle())
		return;
	if (TreeLevel <= 6)
		return; // gabar planes are processing
	bool xDivide = (TreeLevel <= 6 + XLevels);
	if (xDivide && ixMin != ixMax - 1)
		Range0.ixMax = Range1.ixMin = (ixMin + ixMax) / 2;
	else if (!xDivide && iyMin != iyMax - 1)
		Range0.iyMax = Range1.iyMin = (iyMin + iyMax) / 2;
}
