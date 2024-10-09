#pragma once
#include "mquadtree.h"
class BGeomArray;



typedef CArray< ContElem, const ContElem &> ContElemArray;

class AFX_EXT_CLASS MQuadTreeRot :
	public MQuadTree
{
public:
	MQuadTreeRot(const BPoint & MinPoint, const BPoint & MaxPoint, int RowsNum, int ColsNum, BStockGrID Position, bool GlobalStor)
		: MQuadTree(MinPoint, MaxPoint, RowsNum, ColsNum, Position, GlobalStor) {};
	MQuadTreeRot(void);
	virtual ~MQuadTreeRot(void);
	bool Combine(MQuadBody * pInTree, int SectorsNum);
	bool MakeTurnMesh(MeshArr &MeshArray, const MBSPNode * pRoot, int SectorsNum);
protected:
	NParamMesh * MakeOneTurnMesh(const BPoint & P0, const BPoint & P1, const SOrigSurf * pSurf, int SectorsNum);
public:
	bool GetContour(BGeomArray & GeomArr);
	int ExtractContInfo(ContElemArray & ContElems, const MBSPNode * pRoot) const;
	virtual void Transform(const BMatr &M) override;
	MQuadTreeRot * CreateFullCopy(void);
};
