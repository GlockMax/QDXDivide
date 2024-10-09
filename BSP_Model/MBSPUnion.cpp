#include "stdafx.h"
#include "mbspunion.h"
#include "mbsptree.h"
#include "mbspnode.h"

MBSPUnion::MBSPUnion(void)
{
}

MBSPUnion::~MBSPUnion(void)
{
}

MERGERESULT MBSPUnion::MergeCells(MBSPNode *& pV1, MBSPNode * pV2) const
{
	MERGERESULT Result = UNCHANGED;
	if(pV1 == (MBSPNode *)M_IN_PTR)
	{
		bool Changed = !MBSPTree::DeleteAndCheckTree(pV2, pV1);
		if(Changed)
			Result = T2CHANGED;
		return Result;
	}
	if(pV1 == (MBSPNode *)M_OUT_PTR)
	{
		if(pV2 == pV1)
			return Result;
		if(MBSPNode::Collapse(pV2))
			if(pV2 == pV1)
				return Result;
		pV1 = pV2;
		Result = T1CHANGED;
		return Result;
	}
	// pV1 is not terminal
	if(pV2 == (MBSPNode *)M_IN_PTR)
	{
		bool Changed = !MBSPTree::DeleteAndCheckTree(pV1, pV2);
		pV1 = (MBSPNode *)M_IN_PTR;
		if(Changed)
			Result = T1CHANGED;
		return Result;
	}
	Result = T2CHANGED;
	return Result;
}
