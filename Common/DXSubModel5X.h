#pragma once

#include "DXModel.h"
#include "DXSubModel.h"
#include "MElemIDProc.h"
#include "NCadrGeom.h"
#include "DXMemory.h"
#include "DXPIterator.h"
#include "MBSPForest.h"
#include "DXDirection3X.h"
#include "DXDirection5X.h"

class BSP_MODEL_API DXSubModel5X : public DXSubModel
{
public:
	virtual ~DXSubModel5X(void);
	DXSubModel5X(/*const*/ DXModel &ParentModel, BStockGrID Position);
	void Reset();
	void MakeEmpty();
	void ReCreate();
protected:
	DXMemoryPt Memory;

	DXDirection5X Directions[3];// Z, Y, X 
	DXDirection5X DirectionsA[3];// Z, Y, X 
	DXDirection3X AuxDirs[3];// Z, Y, X
	DXDirection3X AuxAuxDirs[3];// Z, Y, X
	bool StoreInA;
public:
	void Intersect();
	void SubtrAllDir();
	int SubtrDirectionsA(void);
	int Subtr1Dir(DXDirection::DX_Dir DirSym);
	int Subtr1Dir(int k);
	int StoreFace(MFace * pFace, ElemID ElID, cadrID CaID, bool ToAux) override;
	void CreateCh(const BBox &Bounds, bool Convex, bool ToAux);
	void MarkUsedFacesOrigCh(class BDynBitArray & UsedF, class BDynBitArray & UsedO) override;
	void SetNewIndexes(int* newind) override;
	void MarkUsedPoints(class BDynBitArray& Used) const override;
	void MarkUsedOrigCh(class BDynBitArray & Used);
	void CopyMergeCh(MBSPMerge & Op , MBody & In);
	void SafeMergeCh(const MBSPMerge & Op) override;
	void ClearCh(void);
	MBody * InvertCh(bool ToAux) override;
	NGlobalState::ModelModes GetModelMode(void) const { return NGlobalState::DX;}
	const DXDirection5X &GetDirection(int k) const  { return Directions[k];}
	DXDirection5X &GetDirection(int k) { return Directions[k]; }
	DXDirection5X &GetDirectionA(int k) { return DirectionsA[k]; }
	DXDirection5X *GetDirectionsA() { return DirectionsA; }
	DXDirection3X &GetAuxDir(int k) { return AuxDirs[k]; }
	int GetBigSize(int i) const { return Parent.nBig[i];}
	virtual bool CheckCh(void) const;
	bool Heal(void);
	void Normalization();
	bool BusyUnBusy(int I0, int I1, int I2, int NumDir);
	static int InsBCadr(const BPoint& P0v, const BPoint& P1v, const BPoint& Nv, double R, double H, ElemID ElID, const BMatr& Shift, class CadrData& Data, const class CadrData& MaskData);
	static int InsFullBCadr(const BPoint &, const BPoint &, const BPoint &, double, ElemID ElID, const BMatr &Shift, class CadrData &, const class CadrData& MaskData);
	static int InsCCadr(const BPoint &, const BPoint &, const BPoint &, double, double, ElemID ElID, const BMatr &, class CadrData &, const class CadrData& MaskData);
	int RastBCadr(int Dir, const CadrData &Data);
	int RastFullBCadr(int Dir, const CadrData &Data);
	int RastCCadr(int Dir, const CadrData &Data);
	bool IsChanged(void) const override;
	void ClearChanged(void) override;
	int Merge1Dir(int k);
	int GetLevelsNum(void) const { return GetParent().LevelsNum;}
	DXMemoryPt &GetMemory(void) { return Memory;}
	const DXMemoryPt &GetMemory(void) const { return Memory;}
	DXMemoryPtOnePart &GetMemory(unsigned int Dir) { return Memory.GetDirPart(Dir); }
	const DXMemoryPtOnePart &GetMemory(unsigned int Dir) const  { return Memory.GetDirPart(Dir); }
	const DXMemoryPtOnePart &GetMemoryC(unsigned int Dir) const { return Memory.GetDirPart(Dir); }
	void ClearMemory(void) { Memory.DeleteAll();}
	size_t GetGlobSize(void) { return Memory.GetGlobSize(); }
	size_t GetUsedSize(void) { return Memory.GetUsedSize(); }
	DXMemID GetAtNext(DXMemID Ind) { return Memory.GetAtNext(Ind);}
	bool CheckEmpty(int i0, int i1, DX_DEPTH Val);
	void RastTriangle(const BPoint & P0, const BPoint & P1, const BPoint & P2, const BPoint & N, DX_ID PairID);
	void MakeCyl(double D, double H, const BMatr &Tr);
	void MakeFromBSP(MQuadTree& QTree, DXDirection5X *Dirs = nullptr);
	void MakeFromQRendMill(const class GQuadRenderMill* pRendMill);
	void FillChangedAreaWin();
	void JoinModifiedCells();
	void ClearChangedAreaWin() { Win.Clear(); }
	void SetStoreInA(bool Val) { StoreInA = Val; }
	bool GetStoreInA(void) const { return StoreInA; }
	void ClearModified() override;
	void Translate(const BMatr &M) override;
	void GetTransfData(MBSPForest::NTransfData &TransfData) const;
	void Save(MForestSave& SaveData) override;
	void Load(CFile* f) override;
	void CreateFullCopy(MBody* pQ) override;
	double GetNearestPoint(const BPoint& OrP, MBodyPointInf& Inf) const;
	void MakeBox(const BBox& Box);
};

