// MBinTree.h: interface for the MBinTree class.
//
//////////////////////////////////////////////////////////////////////
#pragma once
#include "BSP_MODEL_API.h"

class MTreeNode;

class  BSP_MODEL_API MBinTree
{
public:	
	MBinTree(const MBinTree &tr);
	MBinTree();
	virtual ~MBinTree();

protected:
	MTreeNode* GetRoot();
	MTreeNode* CreateTreeNode(MTreeNode *pLeft = NULL, MTreeNode *pRight = NULL);
	
	// Root pointer
	MTreeNode * m_pRoot;

	int m_size;
public:
	void SetRoot(MTreeNode * pNode);
	size_t GetMaxDepth(void);
	static MTreeNode * Contract(MTreeNode *pNode);
};
