#pragma once
#include "MQuadBody.h"
#include "MFace.h"
#include "MBSPMerge.h"
#include "DCollection.h"

class MBSPNode;

enum Sides
{
	Xt = 0x01,
	Xb = 0x02,
	Yt = 0x04,
	Yb = 0x08,
	Zt = 0x10,
	Zb = 0x20
};

class AFX_EXT_CLASS MQuadTree :
	public MQuadBody
{
public:
	class ActLocStorage
	{
	public:
		ActLocStorage(MQuadTree& QTree, int ix, int iy);
		~ActLocStorage();
	};
	class Cell
	{
	public:
		~Cell(void);
		Cell(const MQuadTree *pTree);
		const Cell & SetCur(int ix, int iy);
		int CrStandardFace(const MPlane& Face, MFace* pNewFace) const;
		const MFace& CrBoundFace(Sides Side, MFace& Face);
	protected:
		BPoint Vertexes[8];
		double Xmin,Ymin,Zmin;
		double Xmax,Ymax,Zmax;
		const MQuadTree *pTree;

	};

public:
	virtual ~MQuadTree(void);
	void Init(bool GlobalStor);
	MQuadTree( const BPoint& MinPoint, const BPoint& MaxPoint, int RowsNum, int ColsNum, BStockGrID Position, bool GlobalStor);
	MQuadTree(void);
	void CreateFullCopy(MBody *pQ) override;
	MLocStorage* GetLocStorage(int ix, int iy);
	const MLocStorage* GetLocStorageC(int ix, int iy) const;
protected:
	MBSPNode*** Array;
	MBSPNode*** AuxArray;
	MLocStorage*** LocStArr;
public:
	int StoreFace(MFace* pFace, ElemID ElID, cadrID CaID, bool ToAux) override;
	void MarkUsedFacesOrigCh(class BDynBitArray & UsedF, class BDynBitArray & UsedO) override;
	void SetNewIndexes(int* newind) override;
	void MarkUsedPoints(class BDynBitArray& Used) const override;
	void MarkUsedOrigCh(class BDynBitArray & Used);
	void CreateTree(bool ToAux);
	void CreateTreeLoc(int ix, int iy, bool ToAux);
	void SortFaces(const MQuadTree &Base);
	void DrawFaces(void);
	bool CheckNormals(void);
	void CopyMergeCh(MBSPMerge & Op , MBody & In);
	void SafeMergeCh(const MBSPMerge& Op) override;
	void SafeMergeChWithCr(MBSPMerge& Op, const BBox& Bounds, bool Convex);
	MERGERESULT SafeMergeLoc(const MBSPMerge& Op, int ix, int iy);
//	void ClearCh(void);
	void CreateCh(const BBox& Bounds, bool Convex, bool ToAux);
	void EmptyCellsDet(bool ToAux);
	void DetCell(int ix, int iy, int dx, int dy, bool ToAux);
	//MBodyParams GetParams();
	//MBodyParams GetCellParams(int ix, int iy);
	void Save(MForestSave &SaveData) override;
	void Load(CFile *f) override;
	virtual MQuadTree * InvertCh(bool ToAux) override;
	//MBSPNode * CrSection(const MPlane & Plane, int &n);
	int CrCellTree(int ix, int iy, Cell &C, MBSPNode *& pRoot) const;
	static int ClearCellTree(MBSPNode *pRoot, int NodesNum);
	bool Contains(const BPoint & P);
	void CrStandard1Cell(MBSPNode *& rpRoot, const MQuadTree::Cell & Cell);
	size_t GetMaxDepth(void);
	MBSPForest::ForestSizes GetSizes() const;

	
	void MakeTree(int &NodeCount, MBSPNode ***MyTree) const;
	void RemoveTree(int NodeCount, MBSPNode ***MyTree);
	void PackUp(MBSPNode* node, int level, int count, int& index);
	NGlobalState::ModelModes GetModelMode(void) const { return NGlobalState::BSP;}
	virtual bool CheckCh(void) const;
	MQuadTree * CreateFullCopy(void);
public:
#pragma region LOOP_THROUGH_ALL_BODIES
	// List functions
	void ResetList(M_PTR Value);
	virtual void CrStandard(bool ToAux);
	class MBSPCell* MakeBSPCell(int ix, int iy) const;
#pragma endregion LOOP_THROUGH_ALL_BODIES
public:
	// Non list functions
	void Reset(M_PTR Value);
	void SetMBit(int ix, int iy);
	void GetTransfData(MBSPForest::NTransfData &TransfData);
	void Translate(const BMatr &M) override;
	void ChangeYDir();
	void MakeFromTriangles(int NTri, const float* Triangles);
	void MakeFromBDCEL(class BDCEL &DCEL);
	void RenumUsedFaces(const int* newind);
	void SwapY();
	const MBSPNode* GetRootNode(int i, int j) const { return Array[i][j]; }
	MBSPNode* GetRootNodeV(int i, int j) const { return Array[i][j]; }
	MBSPNode*& GetRootNodeR(int i, int j) const { return Array[i][j]; }
	void SetRootNode(int i, int j, MBSPNode* pRoot) { Array[i][j] = pRoot; }
	void SetRootNodeAux(int i, int j, MBSPNode* pRoot) { AuxArray[i][j] = pRoot; }
	void SetRootNodeConst(int i, int j, enum M_PTR pRoot) { Array[i][j] = (MBSPNode*)pRoot; }
	bool RayShooting(const BPoint& P, const BPoint& V, bool type, BPoint& FoundP, BPoint& FoundN, ElemID& FoundID, cadrID& FoundCadrID);
	void SetGlobalStorOnly();
	void Move2GlobStor();
	bool Is1Connected();
};
