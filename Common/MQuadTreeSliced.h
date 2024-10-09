#pragma once
#include "mquadtree.h"
#include "mquadtreeRot.h"

class AFX_EXT_CLASS MQuadTreeSliced :
	public MQuadTree
{
protected:
	MQuadTreeSliced(void);
public:
	MQuadTreeSliced( const BPoint & MinPoint, const BPoint & MaxPoint, int RowsNum, bool GlobalStor);
	void CreateRotSolid(MQuadTreeRot & RotTree, int SectorsNum);
	void CreateRotSolid( ContElemArray *ContElemArrays, int SectorsNum);
protected:

	MBSPNode * CrSectorsTree(MBSPNode *** Terminals, int *NormalNums, int SectorsNum);
	void CrSectorsSubTree(MBSPNode **ppRoot, int Ns, int Ne, MBSPNode *** Terminals, int *NormalNums, int BPind);
public:
	void CrStandardWODummy(bool ToAux);
	void CrStandard(bool ToAux) override;
	void CrStandard1CellWOD(MBSPNode *& rpRoot, const MQuadTree::Cell & Cell);
	int Subdivide2QuadTree(MQuadTree & QuadTree, bool ToAux);
};
