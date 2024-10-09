// MTreeNode.h: interface for the MTreeNode class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "BSPEnums.h"

// ---бепьхмю б дбнхвмнл депебе---
class AFX_EXT_CLASS MTreeNode
{
	static const INT_PTR BSP_TERM_MASK = ~(M_IN_PTR | M_OUT_PTR | M_NDEF_PTR);
public:
	MTreeNode(const MTreeNode &n);
	inline MTreeNode * GetRight() const;
	inline MTreeNode * GetLeft() const;
	void SetRight(MTreeNode *pNode);
	void SetLeft(MTreeNode *pNode);

	int Equality(MTreeNode *n);
	static void Del(MTreeNode *p) { delete p; }
	static bool IsTerminal(const MTreeNode * pNode) { return ((INT_PTR)pNode & BSP_TERM_MASK) == NULL;}
	MTreeNode();
	MTreeNode (MTreeNode *pLeft, MTreeNode *pRight);
	~MTreeNode();

protected:
	MTreeNode *m_pRight;
	MTreeNode *m_pLeft;

public:
	size_t GetMaxDepth(void);
	void DeleteAll();
	void ForEachDescendant(void Func(MTreeNode *));
};
