#include "stdafx.h"
#include "MBSPNode.h"
#include "MBSPMerge.h"

MBSPMerge::Stack::Stack()
{
	Top = -1;
}
MBSPMerge::Stack::~Stack()
{
}

bool MBSPMerge::Stack::Push(MBSPNode **p1, MBSPNode *p2)
{
	if(Top >= MAX_STACK_LEN - 1)
		return false;
	++Top;
	M1[Top] = p1;
	M2[Top] = p2;
	return true;
}

bool MBSPMerge::Stack::Pop(MBSPNode **&p1, MBSPNode *&p2)
{	if(Top < 0)
		return false;
	p1 = M1[Top];
	p2 = M2[Top];
	--Top;
	return true;
}
void MBSPMerge::Stack::Clear()
{
	Top = -1;
}

bool MBSPNode::StackForShooting::Push(MBSPNode *p,
							  double tP1, double tP2,MBSPNode *p1)
{
	if(Top >= MAX_STACK_LEN1 - 1)
		return false;
	++Top;
	M1[Top] = p;
	tPb[Top] = tP1;
	tPe[Top] = tP2;
	M2[Top] = p1;
	return true;
}

bool MBSPNode::StackForShooting::Pop(MBSPNode *&p,
							 double &tP1, double &tP2, MBSPNode *&p1)
{	if(Top < 0)
		return false;
	p = M1[Top];
	tP1 = tPb[Top];
	tP2 = tPe[Top];
	p1 = M2[Top];
	--Top;
	return true;
}
void MBSPNode::StackForShooting::Clear()
{
	Top = -1;
}

MBSPNode::StackForShooting::StackForShooting()
{
	Top = -1;
}
MBSPNode::StackForShooting::~StackForShooting()
{
}

