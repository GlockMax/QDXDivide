#pragma once

#include <vector>
#include <stack>

class MBSPNode;
class MFace;

class DCollection
{
protected:
	template<typename T, typename U>
	struct Item
	{
		T element;
		U index;
	};

	struct CompareItem
	{
		bool operator() (const Item<MBSPNode*, int> &a, const Item<MBSPNode*, int> &b) const
		{
			return a.element > b.element;
		}
	};

	std::vector<MBSPNode*> pairs; // adjecent pairs [firstPart][secondPart] [firstPart][secondPart]
	std::stack<MBSPNode*> nodes;
	std::stack<MFace*> faces;
	MFace *forCutting;
	std::vector<Item<MBSPNode*, int>> firstHalf;
	std::vector<Item<MBSPNode*, int>> secondHalf;
	std::stack<MBSPNode*> pStack;
	static const int pairCapacity = 500000;
	int pairCount;

	INT_PTR FindPairsIndex(MBSPNode*, std::vector<Item<MBSPNode*, int>> &list) const;
	void FillList(std::vector<Item<MBSPNode*, int>> &list, int shift);
	void MarkNode(MBSPNode *node);
	void FindAdjacent(MBSPNode *node, INT_PTR index, std::vector<Item<MBSPNode*, int>> &list, int shift);

public:
	DCollection(void);
	~DCollection(void);
	void AddItemInList(MFace *face, MBSPNode *node);
	void ClearList(void);
	void ClearPairs(void);
	void AddPair(MBSPNode *nodeOne, MBSPNode *nodeTwo);
	void RemoveLastItem(void);
	void Mark(MBSPNode *node);
	MFace* getFace(void) const;
	MBSPNode* getNode(void) const;
};
