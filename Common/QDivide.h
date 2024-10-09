#pragma once
#include "BMatr.h"
#include "BSP_MODEL_API.h"
#include "MQuadTree.h"
#include "QDivideBase.h"

class BSP_MODEL_API QDivide : public QDivideBase
{
public:
	QDivide(MBody* body, MBSPForest* forest);
	virtual ~QDivide();
protected:

	MQuadTree* pTree;
	MBSPNode *backup;

	int NodeCount;
	int ArrayCount;
	int Deep;
	DCollection *dcollection;

	MFace *positive;
	MFace *negative;
	M_POSITION m_pos;
	MFace *forCutting;

	void LeftTraversal(MBSPNode *node);
	void RightTraversal(MBSPNode *node);
	void FindAdjacentPairs(MBSPNode *node);

	MBSPNode **MyTree;
	MBSPForest *forest;

	void MakeTree();
	MBSPNode* Contains(MBSPNode *node, BPoint &point) const;
	void DeleteTree(MBSPNode* node, int Deep, int level);

public:
	void SetPoint(BPoint point, BPoint view) override;
	void Revert() override;
	void Recreate() override;
	void FindAdjacentPairs();
	void PackUp();
	bool MarkConnected();
};

