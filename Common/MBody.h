#pragma once
#include "BMatrPair.h"
#include "NCMProject.h"
#include "MElemIDProc.h"
#include "BStockGrID.h"
#include "CadrID.h"

class NCadrGeom;
class MBodyParams;
class NParamMesh;
class MBSPMerge;
class MFace;
class BBox;

class ContElem
{
public:
	ContElem(void){};
	ContElem(const BPoint *ip0, const BPoint *ip1, ElemID iElID)
	{
		p0 = ip0;
		p1 = ip1;
		ElID = iElID;
	};
	NCadrGeom *CrCadrGeom(void);
	const BPoint *p0;
	const BPoint *p1;
	ElemID ElID;
};
class AFX_EXT_CLASS MBody
{
	friend class GBodyRender;
public:
	MBody(BStockGrID Position);
	virtual ~MBody(void);
	virtual void Load(CFile * f);
	static ElemID MakeID4TurnSurf(ContElem Elem);
	static void RecalcID(const MFace& Face, ElemID &ElID);
	virtual void Save(class MForestSave &SaveData);
	virtual NGlobalState::ModelModes GetModelMode(void) const = 0;
//	virtual MBodyParams GetParams() = 0;
	void AddBodyInList(MBody *pMBody);
	void SetNextMBody(MBody *next);
	MBody* GetNextMBody() const;
	void ResetNextBody();
	const BPoint & GetMin(void) const	{return MinPoint;}
	const BPoint & GetMax(void) const	{return MaxPoint;}
	void ReCreate(const BPoint & MinPoint, const BPoint & MaxPoint);
	virtual void Translate(const BMatr &M);
	virtual void Transform(const BMatr &M);
	void SetBodyRot(const BMatrPair &BRot) { BodyRot = BRot; }
	void SetBodyRot(const BMatr &M) { BodyRot = BMatrPair(M); }
	const BMatrPair &GetBodyRot() const { return BodyRot; }
	bool IsBodyRotActive() const { return BodyRotActive; }
	void SetBodyRotActive(bool Val = true) { BodyRotActive = Val; }
	void RecalcMinMax();
	void StoreMesh(const NParamMesh& Mesh, ElemID ElID, cadrID CaID, bool ToAux);
	virtual int StoreFace(MFace* pFace, ElemID ElID, cadrID CaID, bool ToAux) = 0;
	BStockGrID GetStockPos() const { return StockGr; }
	void SetStockPos(BStockGrID Gr)  { StockGr = Gr; }
	bool IsRotBody() const;

protected:
	BPoint MinPoint;
	BPoint MaxPoint;
	BMatrPair BodyRot; // This matrix is pure rotation matrix and ensures rotation of this body to it's right position around global zero point
	bool BodyRotActive;
	BStockGrID StockGr;
	MBody *pNextMBody;

#pragma region LOOP_THROUGH_ALL_BODIES
public:

	virtual void MarkUsedPoints(class BDynBitArray& Used) const = 0;
	virtual void SetNewIndexes(int* newind) = 0;
	virtual void MarkUsedFacesOrig(class BDynBitArray& UsedF, class BDynBitArray& UsedO);
	virtual void MarkUsedFacesOrigCh(class BDynBitArray & UsedF, class BDynBitArray & UsedO) = 0;

	virtual void MarkUsedOrig(class BDynBitArray & Used);
	virtual void MarkUsedOrigCh(class BDynBitArray & Used) = 0;

	virtual void CopyMerge(MBSPMerge & Op , MBody & In);
	virtual void CopyMergeCh(MBSPMerge & Op , MBody & In) = 0;

	void SafeMerge(const MBSPMerge & Op);
	virtual void SafeMergeCh(const MBSPMerge & Op) = 0;

	//virtual void Clear(void);
	//virtual void ClearCh(void) = 0;

	virtual bool Check(void) const;
	virtual bool CheckCh(void) const = 0;

	virtual MBody * Invert(bool ToAux);
	virtual MBody * InvertCh(bool ToAux) = 0;

	void CreateBodyStd(const BBox &Bounds, bool Convex, bool ToAux);
	virtual void CreateCh(const BBox &Bounds, bool Convex, bool ToAux) = 0;
#pragma endregion LOOP_THROUGH_ALL_BODIES
	virtual void CreateFullCopy(MBody* pQ);
};
