// MBSPTree.cpp: implementation of the MBSPTree class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "BPoint.h"
#include "MFace.h"
#include "MTreeNode.h"
#include "MBSPNode.h"
#include "MBSPMerge.h"

#include <math.h>
#include "mbsptree.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


MBSPTree::MBSPTree()
{
//	m_text.Init(wglGetCurrentDC(), USEFONTBITMAPS, "arial.ttf");
}

MBSPTree::~MBSPTree()
{
	ClearAll();
}


/////////////////////////////////////
// УДАЛЕНИЕ ЭЛЕМЕНТОВ ДЕРЕВА
// удалить верево начиная с заданного узла
// использовать обратный алгоритм для прохождения узлов дерева
// и удалить каждый узел при его посещении
void MBSPTree::DeleteTree(MBSPNode *pRoot)
{
	if (MBSPNode::IsTerminal(pRoot))
		return ;
	
	DeleteTree(pRoot->GetLeft());
	DeleteTree(pRoot->GetRight());
		
	// освободить динамическую память, занимаемую данным узлом
	delete pRoot;
}

bool MBSPTree::DeleteAndCheckTree(MBSPNode *pRoot, const MBSPNode *pCheck)
{ // Вернуть true если все терминальные узлы дерева совпадают с pCheck
	if (MBSPNode::IsTerminal(pRoot))
		return pRoot == pCheck;
	
	bool Res = true;
	Res &= DeleteAndCheckTree(pRoot->GetLeft(), pCheck);
	Res &= DeleteAndCheckTree(pRoot->GetRight(), pCheck);
		
	// освободить динамическую память, занимаемую данным узлом
	delete pRoot;

	return Res;
}

//  удаляет дерево и сбрасывает корень
void MBSPTree::ClearAll()
{
	DeleteTree((MBSPNode*)m_pRoot);
	m_pRoot = NULL;
}

// рисование BSP в виде дерева
//void MBSPTree::DrawSkeleton()
//{
//	MTreeNode *pRoot = MBinTree::GetRoot();
//	int depth = MBinTree::Depth(pRoot), count = 0;
//	BPoint s(0,0,0,1);

//	if (pRoot->GetNum() == "-1")
//		glColor3d(1., 0., 0.);
//	if (pRoot->GetNum().Find("D") != -1)
//		glColor3d(1., 0., 0.);
//	else if (pRoot->GetNum().Find("V") != -1)
//		glColor3d(1., 0.3, 1.);
//	else if (pRoot->GetNum().Find("R") != -1)
//		glColor3d(0., 1., 0.);
//	else if (pRoot->GetNum().Find("B") != -1)
//		glColor3d(1., 1., 1.);
//	else if (pRoot->GetNum().Find("T") != -1)
//		glColor3d(0.75, 0.75, 0.75);
//	else
//		glColor3d(0., 0.1, 1.);
//
//	int dx = (int) pow(2., depth);
//	dx = dx * NODES_WIDTH;
//	int dy = (int) pow(2., depth);
//	dy = dy * NODES_HEIGHT;
//
//	glBegin(GL_QUADS);
//		glVertex3d(s.GetX()-dx/4*NODES_MARK, s.GetY()+dx/4*NODES_MARK, s.GetZ());
//		glVertex3d(s.GetX()-dx/4*NODES_MARK, s.GetY()-dx/4*NODES_MARK, s.GetZ());
//		glVertex3d(s.GetX()+dx/4*NODES_MARK, s.GetY()-dx/4*NODES_MARK, s.GetZ());
//		glVertex3d(s.GetX()+dx/4*NODES_MARK, s.GetY()+dx/4*NODES_MARK, s.GetZ());
//	glEnd();
	
//	DrawBranchs(pRoot, s, depth+1, count);
	
//	glColor3d(0.3, 0.7, 0.7);
//	sprintf(str, "%s", pRoot->GetNum());
//	m_text.Draw3D(s.GetX()-2*NODES_MARK, s.GetY()+3*NODES_MARK/2, s.GetZ(), "122323"/*str*/);
//}

// отрисовка одной верви дерева (рекурсивная функция)
//void MBSPTree::DrawBranchs(MTreeNode *pRoot, BPoint s, int k, int &count, bool left)
//{
	//BPoint D, e;
	//char str[10];
	//int dx, dy;// = NODES_HEIGHT;

	//if (MTreeNode::IsTerminal(pRoot) || pRoot == (MTreeNode*)M_NDEF_PTR)
	//{
	//	if (pRoot == (MTreeNode *)M_IN_PTR)
	//		glColor3d(1., 1., 0.);
	//	else
	//		glColor3d(0., 1., 1.);

	//	// вершинка
	//	glBegin(GL_QUADS);
	//		glVertex3d(s.GetX()-2*NODES_MARK, s.GetY()+NODES_MARK/2, s.GetZ());
	//		glVertex3d(s.GetX()-2*NODES_MARK, s.GetY()-NODES_MARK/2, s.GetZ());
	//		glVertex3d(s.GetX()+2*NODES_MARK, s.GetY()-NODES_MARK/2, s.GetZ());
	//		glVertex3d(s.GetX()+2*NODES_MARK, s.GetY()+NODES_MARK/2, s.GetZ());
	//	glEnd();
	//	if (pRoot == (MTreeNode *)M_IN_PTR)
	//	{
	//		glBegin(GL_QUADS);
	//			glVertex3d(s.GetX()-NODES_MARK/2, s.GetY()+2*NODES_MARK, s.GetZ());
	//			glVertex3d(s.GetX()-NODES_MARK/2, s.GetY()-2*NODES_MARK, s.GetZ());
	//			glVertex3d(s.GetX()+NODES_MARK/2, s.GetY()-2*NODES_MARK, s.GetZ());
	//			glVertex3d(s.GetX()+NODES_MARK/2, s.GetY()+2*NODES_MARK, s.GetZ());
	//		glEnd();
	//	}
	//	return;
	//}
	//else
	//{
	//// ЛЕВАЯ ВЕТВЬ
	//	dx = (int) pow(2., k-1);
	//	dx = dx * NODES_WIDTH;
	////	if (k <= 5)
	//		dy = (int) pow(2., k-1);
	////	else
	////		dy = k;
	//	dy = dy * NODES_HEIGHT;
	//	k--;
	//	count++;
	//	D.Set(-dx/2, -dy,0 ,0);
	//	e = s + D;
	//	//---------------
	//	DrawBranchs(pRoot->GetLeft(), e , k, count);
	//	
	//	count++;
	//	// линия ветки
	//	glColor3d(0.f, 0.1f, 1.f);
	//	glBegin(GL_LINES);
	//		glVertex3d(s.GetX(), s.GetY(), s.GetZ());
	//		glVertex3d(e.GetX(), e.GetY(), e.GetZ());
	//	glEnd();

	//	sprintf(str, "%s", pRoot->GetNum());

	//	if (pRoot->GetNum().Find("D") != -1)
	//		glColor3d(1., 0., 0.);
	//	else if (pRoot->GetNum().Find("V") != -1)
	//		glColor3d(1., 0.3, 1.);
	//	else if (pRoot->GetNum().Find("R") != -1)
	//		glColor3d(0., 1., 0.);
	//	else if (pRoot->GetNum().Find("B") != -1)
	//		glColor3d(1., 1., 0.);
	//	else if (pRoot->GetNum().Find("T") != -1)
	//		glColor3d(1., 1., 1.);

	//	if (!MTreeNode::IsTerminal(pRoot->GetLeft()))
	//	{
	//		// вершинка
	//		glBegin(GL_QUADS);
	//			glVertex3d(s.GetX()-dx/4*NODES_MARK, s.GetY()+dx/4*NODES_MARK, s.GetZ());
	//			glVertex3d(s.GetX()-dx/4*NODES_MARK, s.GetY()-dx/4*NODES_MARK, s.GetZ());
	//			glVertex3d(s.GetX()+dx/4*NODES_MARK, s.GetY()-dx/4*NODES_MARK, s.GetZ());
	//			glVertex3d(s.GetX()+dx/4*NODES_MARK, s.GetY()+dx/4*NODES_MARK, s.GetZ());
	//		glEnd();
	//	/*	glBegin(GL_QUADS);
	//			glVertex3d(s.GetX()-2*NODES_MARK, s.GetY()+2*NODES_MARK, s.GetZ());
	//			glVertex3d(s.GetX()-2*NODES_MARK, s.GetY()-2*NODES_MARK, s.GetZ());
	//			glVertex3d(s.GetX()+2*NODES_MARK, s.GetY()-2*NODES_MARK, s.GetZ());
	//			glVertex3d(s.GetX()+2*NODES_MARK, s.GetY()+2*NODES_MARK, s.GetZ());
	//		glEnd();*/
	//		// номер грани
	//		glColor3d(0.3, 0.7, 0.7);

	//		sprintf(str,"%s", pRoot->GetNum());
	//		m_text.Draw3D(e.GetX()-D.GetX()-2*NODES_MARK, 
	//					  e.GetY()-D.GetY()+3*NODES_MARK/2, e.GetZ(), str);
	//	}

	//// ПРАВАЯ ВЕТВЬ

	//	D.Set(-D.GetX(), D.GetY(), 0, 0);
	//	e = s + D;
	//	///---------------
	//	DrawBranchs(pRoot->GetRight(), e , k, count, false);
	//	
	//	count++;
	//	glColor3d(0.f, 0.1f, 1.f);
	//	glBegin(GL_LINES);
	//		glVertex3d(s.GetX(), s.GetY(), s.GetZ());
	//		glVertex3d(e.GetX(), e.GetY(), e.GetZ());
	//	glEnd();

	//	sprintf(str, "%s", pRoot->GetNum());
	//	if (pRoot->GetNum().Find("D") != -1)
	//		glColor3d(1., 0., 0.);
	//	else if (pRoot->GetNum().Find("V") != -1)
	//		glColor3d(1., 0.3, 1.);
	//	else if (pRoot->GetNum().Find("R") != -1)
	//		glColor3d(0., 1., 0.); 
	//	else if (pRoot->GetNum().Find("B") != -1)
	//		glColor3d(1., 1., 0.);
	//	else if (pRoot->GetNum().Find("T") != -1)
	//		glColor3d(1., 1., 1.);
	//	
	//	if (MTreeNode::IsTerminal(pRoot->GetLeft()))
	//	{
	//		// вершинка
	//		glBegin(GL_QUADS);
	//			glVertex3d(s.GetX()-dx/4*NODES_MARK, s.GetY()+dx/4*NODES_MARK, s.GetZ());
	//			glVertex3d(s.GetX()-dx/4*NODES_MARK, s.GetY()-dx/4*NODES_MARK, s.GetZ());
	//			glVertex3d(s.GetX()+dx/4*NODES_MARK, s.GetY()-dx/4*NODES_MARK, s.GetZ());
	//			glVertex3d(s.GetX()+dx/4*NODES_MARK, s.GetY()+dx/4*NODES_MARK, s.GetZ());
	//		glEnd();
	//	/*	glBegin(GL_QUADS);
	//			glVertex3d(s.GetX()-2*NODES_MARK, s.GetY()+2*NODES_MARK, s.GetZ());
	//			glVertex3d(s.GetX()-2*NODES_MARK, s.GetY()-2*NODES_MARK, s.GetZ());
	//			glVertex3d(s.GetX()+2*NODES_MARK, s.GetY()-2*NODES_MARK, s.GetZ());
	//			glVertex3d(s.GetX()+2*NODES_MARK, s.GetY()+2*NODES_MARK, s.GetZ());
	//		glEnd();*/
	//		// номер грани
	//		glColor3d(0.3, 0.7, 0.7);

	//		sprintf(str, "%s", pRoot->GetNum());
	//		m_text.Draw3D(e.GetX()-D.GetX()-2*NODES_MARK, 
	//					  e.GetY()-D.GetY()+3*NODES_MARK/2, e.GetZ(), str);
	//	}
	//}
//}

void MBSPTree::SetRoot(MBSPNode *pRoot)
{
	MBinTree::SetRoot((MTreeNode*)pRoot);
}

MBSPNode * MBSPTree::GetRoot()
{
	return (MBSPNode *)m_pRoot;
}

//void MBSPTree::DrawTransp(double Coef)
//{
//	if(MBSPNode::IsTerminal(GetRoot()))
//		return;
//	glPushAttrib(GL_ENABLE_BIT);
//	glDisable(GL_DEPTH_TEST);
//	glDisable(GL_AUTO_NORMAL);
//	glEnable(GL_BLEND);
//	glEnable(GL_NORMALIZE);
//	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,0);
//
//	glEnable(GL_LIGHTING) ; 
//	glEnable(GL_LIGHT0) ;
////	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//	GLint viewport[4];
//	GLdouble projMatrix[16],modelMatrix[16];
//	glMatrixMode(GL_MODELVIEW);
//	glGetDoublev(GL_MODELVIEW_MATRIX ,modelMatrix);
//	glGetDoublev(GL_PROJECTION_MATRIX ,projMatrix);
//	glGetIntegerv(GL_VIEWPORT ,viewport);
//	GLdouble sxb,sxe,syb,sye,szb,sze;
//	
//	gluUnProject(0,0,0,
//		modelMatrix, projMatrix,viewport,&sxb,&syb,&szb); 
//	gluUnProject(0,0,1,
//		modelMatrix, projMatrix,viewport,&sxe,&sye,&sze); 
//	BPoint VDir(sxe-sxb,sye-syb,sze-szb,0.);// View direction
//
//// Draw subtree starting in the root vertex
//	MFOREST.SetMark();
////	GetRoot()->GetLeft()->DrawTransp(VDir);
//	GetRoot()->DrawTransp(VDir);
//	MFOREST.RetToMark();
//
////	SwapBuffers(wglGetCurrentDC());
//	glPopAttrib();
//}

//void MBSPTree::CrStandard(void)
//{
//	if(MBSPNode::IsTerminal(m_pRoot))
//		return;
//	GetRoot()->MakeAllPlanes();
//	GetRoot()->Trim();
//}

void MBSPTree::DrawFacets(void)
{
	if(MBSPNode::IsTerminal(m_pRoot))
		return;

// Draw subtree starting in the root vertex
	GetRoot()->DrawFacets();
}

// РИСОВАНИЕ ДЕРЕВА
//void MBSPTree::DrawTree(MBSPNode *pRoot, bool bNorm)
//{
	//if (MBSPNode::IsTerminal(pRoot))
	//	return;

	////if (!bNorm)
	////	glEnable(GL_LIGHTING);

	//glColor3d(0.f, 0.3f, 0.6f);

	//if (!bNorm)
	//	pRoot->GetFace()->Draw();
	//else
	//	pRoot->GetFace()->Draw(&m_text, pRoot->GetNum(), true);
	//	//pRoot->GetFace()->Draw(&m_text, pRoot->GetNum(), false);

	//DrawTree(pRoot->GetLeft(), bNorm);
	//DrawTree(pRoot->GetRight(), bNorm);
//}

bool MBSPTree::CheckNormals(void)
{
	return GetRoot()->CheckNormals();
}

MBSPNode * MBSPTree::Invert(MBSPNode * pNode)
{
	switch((INT_PTR)pNode)
	{
	case M_OUT_PTR:
		return (MBSPNode *)M_IN_PTR;
	case M_IN_PTR:
		return (MBSPNode *)M_OUT_PTR;
	case M_NDEF_PTR:
		return pNode;
	default:
		pNode->SetLeft(Invert(pNode->GetLeft()));
		pNode->SetRight(Invert(pNode->GetRight()));
		return pNode;
	}
}
MBSPNode * MBSPTree::CopySubtree(MBSPNode *pIn)
{
	if (MBSPNode::IsTerminal(pIn))
		return pIn;
	return pIn->CopySubtree();
}
MBSPNode * MBSPTree::CopySubtreeFull(MBSPNode *pIn, CopyMap &Map)
{
	if (MBSPNode::IsTerminal(pIn))
		return pIn;
	return pIn->CopySubtreeFull(Map);
}
void MBSPTree::SetOrder(MBSPNode *pRoot)
{
	if(MBSPNode::IsTerminal(pRoot))
		return;

	SetOrder(pRoot->GetLeft());
	SetOrder(pRoot->GetRight());

	pRoot->GetFacePtr()->SetOrder();

}
void MBSPTree::SpecRotCopy(MBSPNode *pRoot, const BMatr & M)
{
	if(MBSPNode::IsTerminal(pRoot))
		return;

	SpecRotCopy(pRoot->GetLeft(), M);
	SpecRotCopy(pRoot->GetRight(), M);

	pRoot->GetFacePtr()->SpecRotCopy(*pRoot->GetFace(), M);

}
