#include "stdafx.h"
#include "DCollection.h"
#include <algorithm>
#include "MFace.h"
#include "MBSPNode.h"

DCollection::DCollection(void)
{	
	pairCount = 0;
	pairs.reserve(pairCapacity * 2);
}

DCollection::~DCollection(void)
{
}

void DCollection::AddItemInList(MFace *face, MBSPNode *node)
{
	nodes.push(node);
	faces.push(face);
}

void DCollection::AddPair(MBSPNode *nodeOne, MBSPNode *nodeTwo)
{
	pairs.push_back(nodeOne);
	pairs.push_back(nodeTwo);
	++pairCount;
}

void DCollection::RemoveLastItem(void)
{
	nodes.pop();
	faces.pop();
}

MFace* DCollection::getFace(void) const
{
	if (faces.empty())
		return NULL;
	return faces.top();
}

MBSPNode* DCollection::getNode(void) const
{
	if (nodes.empty())
		return NULL;
	return nodes.top();
}

void DCollection::ClearList(void)
{
	while (!nodes.empty())
	{
		nodes.pop();
		delete faces.top();
		faces.pop();
	}
	firstHalf.clear();
	secondHalf.clear();
}

void DCollection::ClearPairs(void)
{
	pairs.clear();
	pairCount = 0;
}

INT_PTR DCollection::FindPairsIndex(MBSPNode* node, std::vector<Item<MBSPNode*,int> > &list) const
{
	// binary search node in list
	// list is sorted by descending
	INT_PTR leftBound = 0, rightBound = list.size() - 1, result;
	if (node == list[leftBound].element)
		return leftBound;
	if (node == list[rightBound].element)
	{
		while (rightBound > 0 && node == list[rightBound - 1].element)
		{
			--rightBound;
		}
		return rightBound;
	}		
	if (node > list[leftBound].element)
		return -1;
	if (node < list[rightBound].element)
		return -1;

	while(rightBound - leftBound > 1) {
		result = (leftBound + rightBound) / 2;
		if (node == list[result].element)
		{
			while (result > 0 && node == list[result - 1].element)
			{
				--result;
			}
			return result;
		}
		(node < list[result].element) ? leftBound = result :  rightBound = result;
	}
	return -1;
}

void DCollection::FindAdjacent(MBSPNode *node, INT_PTR index, std::vector<Item<MBSPNode*, int>> &list, int shift)
{
	// find all nodes which adjacent to *node and push them into pStack
	// if *node is first element in pair, then push second element (shift == 1)
	while ((index < pairCount) && (node == list[index].element))
	{
		if ( (pairs[list[index].index * 2 + shift]->GetLeft() != (MBSPNode*)M_NDEF_PTR)
			&& (pairs[list[index].index * 2 + shift]->GetRight() != (MBSPNode*)M_NDEF_PTR) )
		{
			pStack.push(pairs[list[index].index * 2 + shift]);
			MarkNode(pStack.top());
		}
		++index;
	}
}

void DCollection::FillList(std::vector<Item<MBSPNode*, int>> &half, int shift)
{
	// fill half by firsts(shift == 0) or seconds(shift == 1) pairs elements which contain in DCollection::pairs
	// and save theirs pair index
	half.resize(pairCount);
	for(int i = 0; i < pairCount; ++i)
	{
		half[i].element = pairs[2 * i + shift];
		half[i].index = i;
	}
}

void DCollection::MarkNode(MBSPNode *node)
{
	if (node->GetLeft() == (MBSPNode*)M_IN_PTR)
		node->SetLeft((MBSPNode*)M_NDEF_PTR);
	if (node->GetRight() == (MBSPNode*)M_IN_PTR)
		node->SetRight((MBSPNode*)M_NDEF_PTR);
}

void DCollection::Mark(MBSPNode *node)
{
	// mark all nodes starting with *node which construct single object
	MarkNode(node);
	if (pairs.size() == 0)
		return;
	FillList(firstHalf, 0);
	FillList(secondHalf, 1);

	std::sort(firstHalf.begin(), firstHalf.end(), CompareItem());
	std::sort(secondHalf.begin(), secondHalf.end(), CompareItem());

	pStack.push(node);
	while(!pStack.empty())
	{
		MBSPNode* temp = pStack.top();
		pStack.pop();

		auto index = FindPairsIndex(temp, firstHalf);
		if (index != -1)
			FindAdjacent(temp, index, firstHalf, 1);

		index = FindPairsIndex(temp, secondHalf);
		if (index != -1)
			FindAdjacent(temp, index, secondHalf, 0);
	}
	firstHalf.clear();
	secondHalf.clear();
}