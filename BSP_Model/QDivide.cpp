#include "stdafx.h"
#include "QDivideDlg.h"
#include "QDivide.h"
#include "MRay.h"
#include "MBSPNode.h"
#include "Math.h"
#include "MBSPTree.h"
#include "ConstDef.h"
#include "QDivide.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

typedef MBSPNode *NodePtr;

QDivide::QDivide(MBody* ibody, MBSPForest* iforest) : QDivideBase()
{
	IsActive = true;
	forest = iforest;
	pTree = (MQuadTree*)ibody;
	pTree->Move2GlobStor();
	dcollection = new DCollection();
	ArrayCount = pTree->GetColsNum() * pTree->GetRowsNum();
	Deep = int(log((float)ArrayCount) / log(2.0));

	pTree->MakeTree(NodeCount, &MyTree);

	backup = MyTree[0]->CopySubtree();

	FindAdjacentPairs();
	mode = REMOVE_NONE;
}

QDivide::~QDivide(void)
{
	if (!IsActive)
		return;

	IsActive = false;

	DeleteTree(MyTree[0], Deep, 0);
	//	pTree->RemoveTree(NodeCount, &MyTree);
	MBSPTree::DeleteTree(backup);

	delete dcollection;
	mode = REMOVE_NONE;
}

void QDivide::DeleteTree(MBSPNode *node, int deep, int level)
{
	// delete all nodes which was constucted over MQuadTree:Array[][] 
	if (MBSPNode::IsTerminal(node))
		return;
	if (level - 6  == deep)
		return;// this *node is contained in MQuadTree:Array[][] 
	++level;
	DeleteTree(node->GetLeft(), deep, level);
	DeleteTree(node->GetRight(), deep, level);

	delete node;
	return;
}

void QDivide::SetPoint(BPoint ipoint, BPoint iview)
{
	BPoint point(ipoint * CorrMatr.invr());
	BPoint view(iview * CorrMatr.invr());
	// find and delete object which was clicked
	if (MBSPNode::IsTerminal(MyTree[0]))
		return;
	MBSPNode *node = MyTree[0]->RayShooting(point, view * MAXC, true);
	if (node == (MBSPNode*)M_NDEF_PTR)
		return;// miss
	BPoint result;
	if (node->GetFace()->GetPlane().IntersLine(point, view, result))
	{
		// find point which situated in object
		result = result + view * 0.01;

		dcollection->Mark(Contains(node, result));
		dcollection->ClearPairs();

		if (mode == REMOVE_CURRENT)
		{
			MBSPNode::Replace(MyTree[0], (MBSPNode*)M_NDEF_PTR, (MBSPNode*)M_OUT_PTR);
			MyTree[0] = (MBSPNode*)MBinTree::Contract(MyTree[0]);
		} 
		else
		{
			MBSPNode::Replace(MyTree[0], (MBSPNode*)M_IN_PTR, (MBSPNode*)M_OUT_PTR);
			MyTree[0] = (MBSPNode*)MBinTree::Contract(MyTree[0]);
			MBSPNode::Replace(MyTree[0], (MBSPNode*)M_NDEF_PTR, (MBSPNode*)M_IN_PTR);
		}
		PackUp();
	}
}

void QDivide::Revert(void)
{
	MBSPTree::DeleteTree(MyTree[0]);
	MyTree[0] = backup->CopySubtree();
	PackUp();
	dcollection->ClearList();
	FindAdjacentPairs();
}

void QDivide::Recreate()
{
	FindAdjacentPairs();
}

void QDivide::PackUp()
{
	// pack up tree with root MyTree[0] to MQuadTree::Array[][]
	int lastElementIndex = ArrayCount - 1;
	pTree->PackUp(MyTree[0], 0, Deep, lastElementIndex);
}

bool QDivide::MarkConnected()
{
	if (MBSPNode::IsTerminal(MyTree[0]))
		return false;
	MBSPNode* pNode = MyTree[0]->FindNodeContaining((MBSPNode*)M_IN_PTR);
	if (pNode == (MBSPNode*)M_NDEF_PTR)
		return false;
	dcollection->Mark(pNode);
	return true;
}

MBSPNode* QDivide::Contains(MBSPNode *node, BPoint &point) const
{
	// if point drop in some node, then return this node
	// otherwise return (MBSPNode*)M_NDEF_PTR
	if (MBSPNode::IsTerminal(node))
		return (MBSPNode*)M_NDEF_PTR;
	if((point - node->GetFace()->GetPlane().GetPoint()) * node->GetFace()->GetPlane().GetNormal() > 0)
	{
		if (node->GetLeft() == (MBSPNode*)M_IN_PTR)
			return node;
		else
			return Contains(node->GetLeft(), point);
	}
	else
	{
		if (node->GetRight() == (MBSPNode*)M_IN_PTR)
			return node;
		else
			return Contains(node->GetRight(), point);
	}
}

void QDivide::LeftTraversal(MBSPNode *node)
{	
	// Create all "+" sections situated on forCutting face
	if (MBSPNode::IsTerminal(node))
	{
		return;
	}
	m_pos = forCutting->DivideByPlane(node->GetFace()->GetPlane(), *positive, *negative);
	if( m_pos == M_INT )
	{
		if( positive->Area2() < MINS*MINS )
			m_pos = M_NEG;
		else
			if( negative->Area2() < MINS*MINS )
				m_pos = M_POS;
	}
	switch(m_pos)
	{
	case M_ERR:
	case M_EMP:
		return;
	case M_IDN:
		{
			MPlane oldPlane = forCutting->GetPlane();
			MPlane newPlane(oldPlane);
			newPlane.SetPointOnPlane(forCutting->GetPlane().GetPoint() + forCutting->GetPlane().GetNormal() * 0.001);
			forCutting->SetPlane(newPlane);
			m_pos = forCutting->DivideByPlane(node->GetFace()->GetPlane(), *positive, *negative);
			forCutting->SetPlane(oldPlane);
		}

	case M_POS:
		if (node->GetLeft() == (MBSPNode*)M_IN_PTR)
		{
			dcollection->AddItemInList(new MFace(*forCutting), node);
			return;
		}
		LeftTraversal(node->GetLeft());
		return;
		break;

	case M_NEG:
		if (node->GetRight() == (MBSPNode*)M_IN_PTR)
		{
			dcollection->AddItemInList(new MFace(*forCutting), node);
			return;
		}
		LeftTraversal(node->GetRight());
		return;
		break;

	case M_INT:
		delete forCutting;
		forCutting = new MFace(*positive);		
		MFace *backupF = new MFace(*negative);

		if (node->GetLeft() != (MBSPNode*)M_IN_PTR)
		{
			LeftTraversal(node->GetLeft());
		} 
		else
		{
			dcollection->AddItemInList(new MFace(*forCutting), node);
		}	

		delete forCutting;
		forCutting = backupF;
		if (node->GetRight() != (MBSPNode*)M_IN_PTR)
		{
			LeftTraversal(node->GetRight());
		}
		else
		{
			dcollection->AddItemInList(new MFace(*forCutting), node);
		}
		return;
		break;
	}
}

void QDivide::RightTraversal(MBSPNode *node)
{
	// find intersecting faces between forCutting face and face which will creating by node-subtree
	if (MBSPNode::IsTerminal(node))
	{
		return;
	}
 	m_pos = forCutting->DivideByPlane(node->GetFace()->GetPlane(), *positive, *negative); 
	if( m_pos == M_INT )
	{
		if( positive->Area2() < MINS*MINS )
			m_pos = M_NEG;
		else
			if( negative->Area2() < MINS*MINS )
				m_pos = M_POS;
	}
	switch(m_pos)
	{
	case M_ERR:
	case M_EMP:
		return;
	case M_IDN:
		{
			MPlane oldPlane = forCutting->GetPlane();
			MPlane newPlane(oldPlane);
			newPlane.SetPointOnPlane(forCutting->GetPlane().GetPoint() + forCutting->GetPlane().GetNormal() * 0.001);
			forCutting->SetPlane(newPlane);
			m_pos = forCutting->DivideByPlane(node->GetFace()->GetPlane(), *positive, *negative);
			forCutting->SetPlane(oldPlane);
		}

	case M_POS:
		if (node->GetLeft() == (MBSPNode*)M_IN_PTR)
		{
			dcollection->AddPair(dcollection->getNode(), node);
			return;
		}
		RightTraversal(node->GetLeft());
		return;
		break;

	case M_NEG:
		if (node->GetRight() == (MBSPNode*)M_IN_PTR)
		{
			dcollection->AddPair(dcollection->getNode(), node);
			return;
		}
		RightTraversal(node->GetRight());
		return;
		break;

	case M_INT:
		delete forCutting;
		forCutting = new MFace(*positive);		
		MFace *backupF = new MFace(*negative);

		if (node->GetLeft() == (MBSPNode*)M_IN_PTR)
		{
			dcollection->AddPair(dcollection->getNode(), node);
		} 
		else
		{
			RightTraversal(node->GetLeft());
		}

		delete forCutting;
		forCutting = backupF;

		if (node->GetRight() == (MBSPNode*)M_IN_PTR)
		{
			dcollection->AddPair(dcollection->getNode(), node);
		}
		else
		{
			RightTraversal(node->GetRight());
		}
		return;
		break;
	}
}

void QDivide::FindAdjacentPairs(MBSPNode *node)
{
	// find all nodes which have adjacent face situated on node->GetFace()->GetPlane()
	if (MBSPNode::IsTerminal(node))
		return;

	if (!MBSPNode::IsTerminal(node->GetLeft()) && !MBSPNode::IsTerminal(node->GetRight()))
	{
		forCutting = new MFace(*(node->GetFace()));
		// Create left subtree "+" faces
		LeftTraversal(node->GetLeft());
		delete forCutting;
		// find intersecting faces between face in forCutting and "+" faces which will creating by the right subtree
		while(dcollection->getFace() != NULL)
		{
			forCutting = dcollection->getFace();
			RightTraversal(node->GetRight());
			dcollection->RemoveLastItem();
			delete forCutting;
		}
		FindAdjacentPairs(node->GetLeft());
		FindAdjacentPairs(node->GetRight());
		return;
	}

	if ( (node->GetLeft() == (MBSPNode*)M_IN_PTR)
		|| (node->GetRight() == (MBSPNode*)M_IN_PTR) )
	{
		// if node have (MBSPNode*)M_IN_PTR child, then all of the "+" sections which will creating by another subtree will intersect
		dcollection->AddItemInList(new MFace(*(node->GetFace())), node);
		forCutting = dcollection->getFace();
		(node->GetLeft() == (MBSPNode*)M_IN_PTR) ? RightTraversal(node->GetRight()) : RightTraversal(node->GetLeft());
		dcollection->RemoveLastItem();
		delete forCutting;
	}
	
	FindAdjacentPairs(node->GetLeft());
	FindAdjacentPairs(node->GetRight());
}

void QDivide::FindAdjacentPairs(void)
{
	positive = new MFace();
	negative = new MFace();

	dcollection->ClearPairs();
	FindAdjacentPairs(MyTree[0]);
	
	delete positive;
	delete negative;
}
