// MBSPTree.h: interface for the MBSPTree class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <afxtempl.h>
#include "MBinTree.h"
#include "MBSPForest.h"

//#include "Gl\GLu.h"
//#include "glText.h"

class MFace;
class MBSPMerge;
class MBSPNode;

#define NODES_WIDTH 3
#define NODES_HEIGHT 2
#define NODES_MARK 0.4


// ---BSP депебн---
class AFX_EXT_CLASS MBSPTree : public MBinTree//, public MBSPForest
{
public:
//	virtual void DrawTransp(double Coef = 0.5);
	MBSPNode * GetRoot();

	void SetRoot(MBSPNode *pRoot);
	
//	void DrawSkeleton();
//	void DrawTree(MBSPNode *pRoot, bool bNorm = false);

	void ClearAll();
	static void DeleteTree(MBSPNode *pRoot);
	static bool DeleteAndCheckTree(MBSPNode *pRoot, const MBSPNode *pCheck);
	static void SetOrder(MBSPNode *pRoot);
	static void SpecRotCopy(MBSPNode *pRoot, const BMatr & M);
	static MBSPNode * CopySubtree(MBSPNode *pRoot);
	static MBSPNode * CopySubtreeFull(MBSPNode *pRoot, class CopyMap &Map);
	MBSPTree();

	virtual ~MBSPTree();

protected:
//	void DrawBranchs(MTreeNode *pRoot, BPoint s, int k, int &count, bool left = true);

//	Text m_text;
public:
//	void CrStandard(void);
	void DrawFacets(void);
	bool CheckNormals(void);
	static MBSPNode * Invert(MBSPNode * pNode);
};
