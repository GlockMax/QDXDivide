#pragma once
#include "DXDirection.h"
#include <list>
#include "DXMemory.h"

class BSP_MODEL_API DXDirection5X : public DXDirection 
{
protected:
	DXMemID **Dexels;
public:
	DXDirection5X(void);
	~DXDirection5X(void);
	bool Init(const class DXSubModel *pSubModel, DX_Dir, int s0, int s1, double St0, double St1, int s0Big, int s1Big, double v0, double v1, double vv);
	bool Init();
	void ClearAllValues(DXMemoryPtOnePart *pActMemory, double Val);
	void MakeEmpty(void);
	bool IsDefined() const;
	int Subtract(DXMemoryPtOnePart *pActMemory, class DXDirection3X & Dir3X);
	int Subtract(DXMemoryPtOnePart *pActMemory, class DXDirection5X & Dir5X);
	int Intersect(DXMemoryPtOnePart *pActMemory, class DXDirection5X & Dir5X);
	int Store(DXMemoryPtOnePart *pActMemory, const class DXDirection3X & Dir3X);
//	static void TestSubtract(void);
	DXMemID GetDexelStartPoint(int i0, int i1) const;
	bool Check(const DXMemoryPtOnePart *pActMemory) const;
	bool Heal(DXMemoryPtOnePart *pActMemory);
	void MarkUsedOrig(DXMemoryPtOnePart *pActMemory, class BDynBitArray & Used);
	void SetDexelStartPoint(int i0, int i1, DXMemID P) noexcept {Dexels[i0][i1] = P;}
	int InsOnePoint(DXMemoryPtOnePart *pActMemory, int i0, int i1, DX_DEPTH ZVal, DX_ID ID);
	int InsOneDexel(DXMemoryPtOnePart *pActMemory, int i0, int i1, DX_DEPTH *ZVal, DX_ID *ID, size_t Count);
	int InsOneDexel(DXMemoryPtOnePart* pActMemory, int i0, int i1, const std::list<std::pair<BPoint, unsigned int>>& Nodes, double (BPoint::* GetVal)(void) const);
	DX_ID RastTriangle(DXMemoryPtOnePart *pActMemory, const BPoint & P0, const BPoint & P1, const BPoint & P2, const BPoint & N, DX_ID PairID);
	void MakeBox(DXMemoryPtOnePart* pActMemory, const BBox& Box);
	void MakeFromBSP(DXMemoryPtOnePart* pActMemory, const class MBSPNode* pTree, MQuadTree& QTree);
	void MakeFromQRendMill(DXMemoryPtOnePart* pActMemory, const class GQuadRenderMill* pRendMill);
	void ClearModifiedD();
	bool Save2File(CFile* f) const override;
	void Load(CFile* f) override;
	void CreateFullCopy(DXDirection* pIn) const override;
	double GetNearestPoint(const DXMemoryPtOnePart* pActMemory, const BPoint& OrP, int OneCoordSteps, class MBodyPointInf& Inf) const;
	void FindNearest(const DXMemoryPtOnePart* pActMemory, double Xl, double Yl, double Zl, int ix, int iy, double& D2, int& bx, int& by, DXMemID& Found) const;
	void FillPointInf(const DXMemoryPtOnePart* pActMemory, int bx, int by, DXMemID Found, class MBodyPointInf& Inf) const;

};

