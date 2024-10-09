// MTreeNode.cpp: implementation of the MTreeNode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <list>
#include "bspenums.h"
#include "mtreenode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// ---ВЕРШИНА В ДВОИЧНОМ ДЕРЕВЕ---

MTreeNode::MTreeNode()
{
	m_pLeft = (MTreeNode *)M_NDEF_PTR;
	m_pRight = (MTreeNode *)M_NDEF_PTR;
//	str="/x0"; // подлежит удалению
}

MTreeNode::MTreeNode(const MTreeNode &n)
{
	*this = n;
}

MTreeNode::MTreeNode(MTreeNode *pLeft, MTreeNode *pRight)
{
	m_pLeft = pLeft;
	m_pRight = pRight;
}

MTreeNode::~MTreeNode()
{

}

// задать левого потомка
void MTreeNode::SetLeft(MTreeNode *pNode)
{
	m_pLeft = pNode;
}

// задать правого потомка
void MTreeNode::SetRight(MTreeNode *pNode)
{
	m_pRight = pNode;
}

// определить левого потомка
MTreeNode * MTreeNode::GetLeft()const
{
	return m_pLeft;
}

// определить правого потомка
MTreeNode * MTreeNode::GetRight()const
{
	return m_pRight;
}


// сравнение двух узлов
int MTreeNode::Equality(MTreeNode *n)
{
	return (GetLeft() == n->GetLeft() &&
			GetRight() == n->GetRight()) ? 1 : 0;
}

void MTreeNode::DeleteAll()
{
	ForEachDescendant(Del);
	//std::list<MTreeNode *> Nodes2Delete;
	//std::list<bool> Nodes2DeleteFlags;
	//Nodes2Delete.push_back(this);
	//Nodes2DeleteFlags.push_back(true);
	//do
	//{
	//	// Fill tail
	//	for (MTreeNode *pCur = Nodes2Delete.back();;)
	//	{
	//		// until both children are terminal
	//		bool Left = !IsTerminal(pCur->m_pLeft);
	//		bool Right = !IsTerminal(pCur->m_pRight);
	//		bool Flag = true;// there are two children
	//		if (!(Left && Right))
	//			Flag = false;
	//		if (Left)
	//			pCur = pCur->m_pLeft;
	//		else if (Right)
	//			pCur = pCur->m_pRight;
	//		else
	//			break;
	//		Nodes2Delete.push_back(pCur);
	//		Nodes2DeleteFlags.push_back(Flag); 
	//	}
	//	// Clear tail
	//	while (Nodes2DeleteFlags.back() == false) // First element is "true" always so cycle should stop
	//	{
	//		delete Nodes2Delete.back();
	//		Nodes2Delete.pop_back();
	//		Nodes2DeleteFlags.pop_back();
	//	}
	//	// Change last
	//	if (Nodes2Delete.size() > 1)
	//	{
	//		MTreeNode *pLast = Nodes2Delete.back();
	//		delete pLast;
	//		Nodes2Delete.pop_back();
	//		Nodes2Delete.push_back(Nodes2Delete.back()->m_pRight);
	//		Nodes2DeleteFlags.back() = false;
	//	}
	//} 
	//while (Nodes2Delete.size() > 1);
}

void MTreeNode::ForEachDescendant(void Func(MTreeNode *))
{
	std::list<MTreeNode *> Nodes2Delete;
	std::list<bool> Nodes2DeleteFlags;
	Nodes2Delete.push_back(this);
	Nodes2DeleteFlags.push_back(true);
	do
	{
		// Fill tail
		for (MTreeNode *pCur = Nodes2Delete.back();;)
		{
			// until both children are terminal
			bool Left = !IsTerminal(pCur->m_pLeft);
			bool Right = !IsTerminal(pCur->m_pRight);
			bool Flag = true;// there are two children
			if (!(Left && Right))
				Flag = false;
			if (Left)
				pCur = pCur->m_pLeft;
			else if (Right)
				pCur = pCur->m_pRight;
			else
				break;
			Nodes2Delete.push_back(pCur);
			Nodes2DeleteFlags.push_back(Flag);
		}
		// Clear tail
		while (Nodes2DeleteFlags.back() == false) // First element is "true" always so cycle should stop
		{
			Func(Nodes2Delete.back());
			Nodes2Delete.pop_back();
			Nodes2DeleteFlags.pop_back();
		}
		// Change last
		if (Nodes2Delete.size() > 1)
		{
			MTreeNode *pLast = Nodes2Delete.back();
			Func(pLast);
			Nodes2Delete.pop_back();
			Nodes2Delete.push_back(Nodes2Delete.back()->m_pRight);
			Nodes2DeleteFlags.back() = false;
		}
	} while (Nodes2Delete.size() > 1);
}

size_t MTreeNode::GetMaxDepth(void)
{
	size_t Ret = 1;
	std::list<MTreeNode *> Nodes2Delete;
	std::list<bool> Nodes2DeleteFlags;
	Nodes2Delete.push_back(this);
	Nodes2DeleteFlags.push_back(true);
	do
	{
		// Fill tail
		for (MTreeNode *pCur = Nodes2Delete.back();;)
		{
			// until both children are terminal
			bool Left = !IsTerminal(pCur->m_pLeft);
			bool Right = !IsTerminal(pCur->m_pRight);
			bool Flag = true;// there are two children
			if (!(Left && Right))
				Flag = false;
			if (Left)
				pCur = pCur->m_pLeft;
			else if (Right)
				pCur = pCur->m_pRight;
			else
				break;
			Nodes2Delete.push_back(pCur);
			Nodes2DeleteFlags.push_back(Flag);
		}
		Ret = max(Ret, Nodes2Delete.size());
		// Clear tail
		for (MTreeNode *pCur = Nodes2Delete.back(); Nodes2DeleteFlags.back() == false; pCur = Nodes2Delete.back())
		{
			Nodes2Delete.pop_back();
			Nodes2DeleteFlags.pop_back();
		}
		// Change last
		if (Nodes2Delete.size() > 1)
		{
			Nodes2Delete.pop_back();
			Nodes2Delete.push_back(Nodes2Delete.back()->m_pRight);
			Nodes2DeleteFlags.back() = false;
		}
	} while (Nodes2Delete.size() > 1);
	return Ret;
}

