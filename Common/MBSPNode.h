// MBSPNode.h: interface for the MBSPNode class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "MTreeNode.h"
#include "BSPEnums.h"
#include "BPoint.h"
#include "MQuadTree.h"
#include "CadrID.h"
#include <list>

class MFace;
class MRay;
class MPlane;
class MBSPTree;
const int MAX_STACK_LEN1 = 4096;
class AFX_EXT_CLASS MBSPNode : public MTreeNode
{
public:
	struct RWBlock
	{
		int b, n;
		unsigned int nBFace;
		MBSPNode* pL;
		MBSPNode* pR;
	};
	class IndexPairRange
	{
		int ixMin;
		int ixMax;
		int iyMin;
		int iyMax;
		int TreeLevel;
		int XLevels;
		int YLevels;
	public:
		IndexPairRange(){}
		IndexPairRange(int xMin, int xMax, int yMin, int yMax) {
			ixMin = xMin; ixMax = xMax; iyMin = yMin; iyMax = yMax; TreeLevel = 0;
			XLevels = int(ceil(log2(ixMax))); YLevels = int(ceil(log2(iyMax)));
		}
		bool isSingle() const {
			return ixMin == ixMax - 1 && iyMin == iyMax - 1;
		}
		int GetX() const { return ixMin; }
		int GetY() const { return iyMin; }
		void Divide(IndexPairRange& Range0, IndexPairRange& Range1);
	};

	class StackForShooting
	{
	public:
		StackForShooting();
		~StackForShooting();

	protected:
		int Top;
		MBSPNode *M1[MAX_STACK_LEN1];
		double tPb[MAX_STACK_LEN1];// First otr point parameter
		double tPe[MAX_STACK_LEN1];// End otr point parameter
		MBSPNode* M2[MAX_STACK_LEN1];// the node corresponding to first point

	public:
		bool Push(MBSPNode *p, double tP1, double tP2, MBSPNode *p1);
		bool Pop(MBSPNode *&p, double &tP1, double &tP2, MBSPNode *&p1);
		void Clear();
	};
protected:
	int m_indFace;
	ElemID m_basFace;
	cadrID m_cadrID;
public:
	cadrID GetCadrID() const { return m_cadrID; }
	void SetCadrID(cadrID ID) { m_cadrID = ID; }
	void DrawTransp(const BPoint &VDir, double Coef = 0.5) const;

	const MFace * GetFace() const;
	MFace * GetFacePtr() const;
	ElemID GetBFaceN() const;
	int GetFaceN(void) const { return m_indFace;}
	void SetFace(int iF);
	//void SetFace(const MFace* pFace);
	void SetFace(const MFace& Face);
	const BPoint* GetPoint(int index) const;
	MBSPNode();
	~MBSPNode();
	M_POSITION TrimPos(const MPlane & Plane);
	M_POSITION TrimNeg(const MPlane & Plane);
	void ReplaceFacePoints(const MFace& InFace);
	void MakeAllPlanes(void);
	void Trim(void);
	bool Contains(const BPoint & P) const;
	MBSPNode * CheckStatus(void);
	MBSPNode * RayShooting(const BPoint& P, const BPoint& V, bool type) const;
	void RayShredding(const BPoint& P, const BPoint& V, std::list<MBSPNode*>& Nodes) const;
	void RayShreddingSpec(const BPoint& P, const BPoint& V, MQuadTree &Tree, StackForShooting& St, std::list<std::pair<BPoint, unsigned int>>& Nodes) const;
	void SetBFace(int BasFaceNum);
	int CrSection(const MBSPNode &Root);
	MBSPNode * GetLeft(void) const {return (MBSPNode *)m_pLeft;}
	MBSPNode * GetRight(void) const {return (MBSPNode *)m_pRight;}
	void Invert(void);
	MBSPNode ** GetLeftPtr(void) const;
	MBSPNode ** GetRightPtr(void) const;
	bool CheckNormals(void);

	static MBSPNode * CheckNode(MBSPNode *& InNode)
	{
		if(IsTerminal(InNode))
			return InNode;
		if(InNode->m_pLeft == InNode->m_pRight)
		{
			InNode = (MBSPNode *)InNode->m_pLeft;
		}
		return InNode;
	}

	static bool CheckNode1(MBSPNode *& InNode)
	{ // Return true if something changed
		if(IsTerminal(InNode))
			return false;
		if(InNode->m_pLeft == InNode->m_pRight)
		{
			MBSPNode *buf = InNode;
			InNode = (MBSPNode *)InNode->m_pLeft;
			delete buf; //????? MEMORY LEEKS
			return true;
		}
		return false;
	}

	int CrFacet(MBSPNode & FacetRoot) const;
	static void ClearSectList(MBSPNode *& pStartNode, const MBSPNode * Mask);
	int DrawFacet(double Transp = 0.) const;
	void DrawFacets(void);
	void DrawSection(const MBSPNode & Sect);
	int MakeCellPlanes(const MQuadTree::Cell & Cell);
	void RenumUsedFaces(const int* newind);
	void MarkUsedFacesOrig(class BDynBitArray & UsedF, class BDynBitArray & UsedO);
	void MarkUsedOrig(class BDynBitArray & Used);
	int SortFaces(const MBSPNode * pBase);
	void DrawFaces(const BPoint & VDir);
	int CrFacets(MBSPNode & FacetRoot) const;
	MBSPNode * CopySubtree(void);
	MBSPNode * CopySubtreeFull(class CopyMap &Map);
//	MBodyParams GetParams(void);
	void Save(MForestSave &SaveData);
	void Load(CFile * f);
	void ReplPInd(const int *In2Out);
	void ReplOInd(const int *In2Out);
	static bool Collapse(MBSPNode *& InNode);
	static bool ClearMarked(MBSPNode *& InNode);
	static void Replace(MBSPNode *node, MBSPNode *what, MBSPNode *with);
	void GetTransfData(MBSPForest::NTransfData &TransfData) const;
	MBSPForest::ForestSizes GetSizes() const;
	MBSPNode* FindNodeContaining(const MBSPNode* InOut);
};
