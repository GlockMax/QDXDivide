#pragma once
#include "float.h"
#include <vector>
#include "MBSPForest.h"
#include "MElemIDProc.h"
#include "BDynBitArray.h"

typedef float DX_DEPTH;
const DX_DEPTH DX_DEPTH_MIN = -FLT_MAX; 
const DX_DEPTH DX_DEPTH_MAX = FLT_MAX;
const int FLAG_LENGTH = 2;


union DXMemID
{
	static const unsigned int UpSize = 7;
	static const unsigned int LowSize = 23;

	DXMemID(void) {}
	DXMemID(_int32 InVal) { Val = InVal; }
	bool operator == (const DXMemID &In) const { return Val == In.Val; }
	bool operator != (const DXMemID &In) const { return Val != In.Val; }
	_int32 Val;
	struct
	{
		unsigned int Up : UpSize;
		unsigned int Low : LowSize;
		unsigned int GlobPart : 32 - UpSize - LowSize; // Size must correspond to GlobPartsNum value
	};
};
const DXMemID DXP_END = 0xFFFFFFFF;

class DXMemoryOnePart
{
public:
	DXMemoryOnePart(unsigned int StartSize);
//	DXMemoryOnePart();
	virtual ~DXMemoryOnePart(void);
	void SetGlobPart(unsigned int iGlobPart) { GlobPart = iGlobPart; DeleteAll(); }
protected:
	static const unsigned int MaxPartsNum = (0x1 << DXMemID::UpSize); // This value must corresponds to DXMemID.Up size
	unsigned int BaseSize;
	DXMemID* Next[MaxPartsNum] = { nullptr };
	BDynBitArray *Flags[MaxPartsNum];// if Flags[i][j] == true - element is not used
	unsigned int Sizes[MaxPartsNum];
	unsigned int HolesNum[MaxPartsNum];
	unsigned int PartsNum;
	unsigned int PartsNumAllocated;
	unsigned int GlobPart;
	DXMemID LastPos;
	CCriticalSection CriticalSection;
	bool IsLastPosEndPos;
protected:
	virtual bool Extend(void) = 0;
	int CalcCurSize(void);
public:
	DXMemID FindEmptyPos(void);
	DXMemID InsertAfter(DXMemID Ind);
	void InsertAfter(DXMemID Ind, DXMemID PIDVal);
	void SetAtNext(DXMemID Ind, DXMemID PIDVal) { Next[Ind.Up][Ind.Low] = PIDVal;}
	DXMemID GetAtNext(DXMemID Ind) const { return Next[Ind.Up][Ind.Low];}
	bool GetAtFlag(DXMemID Ind) const { return (*Flags[Ind.Up])[Ind.Low];}
	void RemoveAt(DXMemID Ind);
	void DeleteAll(void);
	unsigned int GetDataCount(void) const;
	unsigned int GetUsedCount(void) const;
	void Lock(void) { CriticalSection.Lock(); }
	void Unlock(void) { 	CriticalSection.Unlock();}
	bool CheckID(DXMemID Ind){ return Ind.Up >= 0 && Ind.Up < MaxPartsNum && Ind.Low >= 0 && Ind.Low < Sizes[Ind.Up]; }
	virtual void Save(class MForestSave& SaveData) const;
	virtual void Load(CFile* f);
	virtual void CreateFullCopy(DXMemoryOnePart* pIn) const;
};

class DXMemoryPtOnePart : public DXMemoryOnePart
{
public:
	DXMemoryPtOnePart();
	~DXMemoryPtOnePart(void);
	void SetAt(DXMemID Ind, DX_DEPTH ZVal, DX_ID IDVal) { SetAtZ(Ind, ZVal); SetAtID(Ind, IDVal);}
	void SetAtZ(DXMemID Ind, DX_DEPTH ZVal) { Z[Ind.Up][Ind.Low] = ZVal;}
	void SetAtID(DXMemID Ind, DX_ID IDVal) { ID[Ind.Up][Ind.Low] = IDVal;}
	// NEW: Установить метку тела
	void SetAtTag(DXMemID where, BYTE TagVal) { TAGS[where.Up][where.Low] = TagVal; }

	DX_DEPTH GetAtZ(DXMemID Ind) const { return Z[Ind.Up][Ind.Low]; }
	DX_DEPTH &GetAtZRef(DXMemID Ind) { return Z[Ind.Up][Ind.Low]; }
	ElemID GetAtIDE(DXMemID Ind) const { return ID[Ind.Up][Ind.Low].Elem; }
	cadrID GetAtIDC(DXMemID Ind) const { return ID[Ind.Up][Ind.Low].Cadr; }
	DX_ID GetAtID(DXMemID Ind) const { return ID[Ind.Up][Ind.Low]; }
	// NEW: Получить метку тела
	BYTE GetAtTag(DXMemID Ind) const { return TAGS[Ind.Up][Ind.Low]; }

	// NEW: Инициализировать метки
	void InitTags();

	int GetGlobSize(void);
	int GetUsedSize(void);
	void GetTransfData(MBSPForest::NTransfData &TransfData) const;
	void Save(class MForestSave& SaveData) const override;
	void Load(CFile* f) override;
	void CreateFullCopy(DXMemoryOnePart* pIn) const override;
	void ReplOInd(ElemID* In2Out);
protected:
	bool Extend(void);
	DX_DEPTH* Z[MaxPartsNum] = { nullptr };
	DX_ID* ID[MaxPartsNum] = {nullptr};
	// NEW: массив меток
	BYTE* TAGS[MaxPartsNum] = {nullptr};
	static constexpr int START_SIZE = 100000;
};


class DXMemoryPt
{
public:
	DXMemoryPt();
	virtual ~DXMemoryPt(void) {};
protected:
	static const int LocPartsNum = 1;
	static const int GlobPartsNum = 3 * LocPartsNum; // Value must correspond to GlobPart size
	DXMemoryPtOnePart DirParts[GlobPartsNum];
public:
	DXMemoryPtOnePart& GetDirPart(unsigned int Dir) { return DirParts[Dir];}
	const DXMemoryPtOnePart& GetDirPart(unsigned int Dir) const { return DirParts[Dir];}
	DXMemID GetAtNext(DXMemID Ind) const { return DirParts[Ind.GlobPart].GetAtNext(Ind); }
	int GetDataCount(void) const { int Res = 0; for (unsigned int i = 0; i < GlobPartsNum; ++i) Res += DirParts[i].GetDataCount(); return Res; }
	DX_ID GetAtID(DXMemID Ind) const { return DirParts[Ind.GlobPart].GetAtID(Ind); }
	DX_DEPTH GetAtZ(DXMemID Ind) const { return DirParts[Ind.GlobPart].GetAtZ(Ind); }
	int GetGlobSize(void) { int Res = 0; for (unsigned int i = 0; i < GlobPartsNum; ++i) Res += DirParts[i].GetGlobSize(); return Res; }
	int GetUsedSize(void) { int Res = 0; for (unsigned int i = 0; i < GlobPartsNum; ++i) Res += DirParts[i].GetUsedSize(); return Res; }
	void DeleteAll(void) { for (unsigned int i = 0; i < GlobPartsNum; ++i) DirParts[i].DeleteAll(); }
	void Save(class MForestSave& SaveData) const;
	void Load(CFile* f);
	void CreateFullCopy(DXMemoryPt* pIn) const;
};

