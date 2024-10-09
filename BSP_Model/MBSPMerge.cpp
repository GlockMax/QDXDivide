#include "stdafx.h"
#include "mbspmerge.h"
#include "mface.h"
#include "MDivideTree.h"
#include "mbspnode.h"
#include "mbsptree.h"

MBSPMerge::MBSPMerge(void)
{
}

MBSPMerge::~MBSPMerge(void)
{
}

MERGERESULT MBSPMerge::Merge(MBSPNode & V1, const MBSPNode & V2) const
{
	Stack St;
	MERGERESULT Result = UNCHANGED;
	// First pointer in the stack can not be modified,
	// so we can push temporary address
	MBSPNode *buf = &V1;
	St.Push(&buf,(MBSPNode *)&V2);
	MBSPNode **ppV1,*pV2;
	int k = 0;
	for(; St.Pop(ppV1,pV2); k++)
	{
		MBSPNode *bV1 = (MBSPNode *)M_NDEF_PTR,*bV2 = (MBSPNode *)M_NDEF_PTR;
		MDivideTree Div((*ppV1)->GetFace(), pV2, bV1, bV2);
		Div.Init((*ppV1)->GetFace()->GetPlane());
//		Div.OutputDebug();
		M_POSITION res = Div.DoDivide();
		if(res == M_ERR)
		{
			if(DEBUGMODE)
			{
				LOGFILE.WriteString("Îøèáêà: MergeBSPT 1\n");
			}
			return -1;
		}
		if(res == M_IDN)
		{
			//int SurfID = MFOREST.GetIDProc()->GetSurfID((*ppV1)->GetBFaceN());
			//if(SurfID < 0 || SurfID == ID_EMPTY)
				//(*ppV1)->SetBFace(Div.BUF);// Surf priority
		}
//		if(!MBSPNode::IsTerminal(bV1))
//			bV1->CheckStatus();
//		if(!MBSPNode::IsTerminal(bV2))
//			bV2->CheckStatus();
		MBSPNode	**bbF1 = (*ppV1)->GetLeftPtr(),
					**bbF2 = (*ppV1)->GetRightPtr();

		if(MBSPNode::IsTerminal(*bbF1) || MBSPNode::IsTerminal(bV1))
			Result |= MergeCells(*bbF1, bV1);// One of the vertexes is terminal
		else
			St.Push(bbF1,bV1);

		if(MBSPNode::IsTerminal(*bbF2) || MBSPNode::IsTerminal(bV2))
			Result |= MergeCells(*bbF2,bV2);// One of the vertexes is terminal
		else
			St.Push(bbF2,bV2);
	}
	return Result;
}

MERGERESULT MBSPMerge::CopyMerge(MBSPNode *& pV1, MBSPNode *& pV2) const
{
	MERGERESULT Result = UNCHANGED;
	if(MBSPNode::IsTerminal(pV1) || MBSPNode::IsTerminal(pV2))
	{
		return MergeCells(pV1, pV2);;
	}
	// Both trees are not cells
	MBSPNode *pV1Copy = MBSPTree::CopySubtree(pV1);
	MBSPNode *pV2Copy = MBSPTree::CopySubtree(pV2);
	MFOREST.SetMark();
	Result = Merge(*pV1,*pV2);
	if(!(Result & T1CHANGED) || !(Result & T2CHANGED))
	{// At least one tree is unchanged
		MFOREST.RetToMark();
		MBSPTree::DeleteTree(pV1);
		if((Result & T1CHANGED)) // T2 is unchanged
		{
			MBSPTree::DeleteTree(pV1Copy);
			pV1 = pV2Copy;
		}
		else // T1 is unchanged
		{
			MBSPTree::DeleteTree(pV2Copy);
			pV1 = pV1Copy;
		}
	}
	else // Both trees were changed
	{
		MBSPTree::DeleteTree(pV1Copy);
		MBSPTree::DeleteTree(pV2Copy);
		MFOREST.RemoveMark();
	}


	return Result;
}
MERGERESULT MBSPMerge::SafeMerge(MBSPNode *& pV1, MBSPNode *& pV2) const
{
	MERGERESULT Result = UNCHANGED;
	if(MBSPNode::IsTerminal(pV1) || MBSPNode::IsTerminal(pV2))
		Result = MergeCells(pV1, pV2);
	else
		Result = Merge(*pV1,*pV2);

	return Result;
}
