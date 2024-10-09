// MBinTree.cpp: implementation of the MBinTree class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MTreeNode.h"
#include "ResLocal.h"
#include "mbintree.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

MTreeNode * m_pCurrent;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/////////////////////////////////////
// �������� ������

MBinTree::MBinTree()
{
	m_pRoot = NULL;
	m_size = 0;
}

MBinTree::MBinTree(const MBinTree &tr)
{
	*this = tr;
}

MBinTree::~MBinTree()
{
}

/////////////////////////////////////
// �������� ��������� ������

// ������� ������ MTreeNode � ������������� ������ lptr � rptr.
// �� ��������� ��������� �������� NULL.
MTreeNode* MBinTree::CreateTreeNode(MTreeNode *pLeft, MTreeNode *pRight)
{
	MTreeNode *pNode;

	// ������� new ��� �������� ������ ����
	// �������� ���� ��������� lptr � rptr
	pNode = new MTreeNode (pLeft, pRight);

	// ���� ������ ������������, ���������
	// ��������� ���������� �� ������
	if (MTreeNode::IsTerminal(pNode))
	{
		AfxMessageBox(IDS_MBINTREE_ERR);
		exit(1);
	}
	// ������� ��������� �� ���������� �������� ������
	return pNode;
}

// ���������� ������ ������
MTreeNode* MBinTree::GetRoot()
{
	return m_pRoot;
}

void MBinTree::SetRoot(MTreeNode * pNode)
{
	m_pRoot = pNode;
}

size_t MBinTree::GetMaxDepth(void)
{
	if(MTreeNode::IsTerminal(GetRoot()))
		return 0;
	return GetRoot()->GetMaxDepth();
}

MTreeNode * MBinTree::Contract(MTreeNode * pNode)
{// Contract subtree by deleting the node if both children are equal

	if(MTreeNode::IsTerminal(pNode))
		return pNode;
	pNode->SetLeft(Contract(pNode->GetLeft()));
	pNode->SetRight(Contract(pNode->GetRight()));
	if(pNode->GetLeft() == pNode->GetRight())
	{
		MTreeNode *Ret = pNode->GetLeft();
		delete pNode;
		return Ret;
	}
	return pNode;
}
