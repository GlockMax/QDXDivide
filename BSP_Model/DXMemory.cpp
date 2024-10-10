#include "stdafx.h"
#include "MForestSave.h"
#include "SOrigSurf.h"
#include "DXMemory.h"
#include "DXMemoryCell5X.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


DXMemoryOnePart::DXMemoryOnePart(unsigned int StartSize)
{
	PartsNum = 0;
	PartsNumAllocated = 0;
	GlobPart = 0;
	HolesNum[0] = 0;
	IsLastPosEndPos = false;
	BaseSize = StartSize;
}

DXMemoryCell5X::DXMemoryCell5X(unsigned int StartSize) : DXMemoryOnePart(StartSize)
{
//	Extend();
	DeleteAll();
}

DXMemoryOnePart::~DXMemoryOnePart(void)
{
	for (unsigned int i = 0; i < PartsNum; ++i)
	{
		delete[] Next[i];
		delete Flags[i];
	}
}

DXMemoryPtOnePart::~DXMemoryPtOnePart(void)
{
	for (unsigned int i = 0; i < PartsNum; ++i)
	{
		delete[] Z[i];
		delete[] ID[i];
		// NEW
		delete[] TAGS[i];
	}
}

DXMemoryPtOnePart::DXMemoryPtOnePart() : DXMemoryOnePart(START_SIZE)
{
}

DXMemoryCell5X::~DXMemoryCell5X(void)
{
	for (unsigned int i = 0; i < PartsNum; ++i)
	{
		delete[] C[i];
		delete[] iZ[i];
	}
}

bool DXMemoryPtOnePart::Extend(void)
{
	if (PartsNum >= MaxPartsNum)
		return false;
	if (PartsNum >= PartsNumAllocated)
	{
		int CurSize = CalcCurSize();
		Sizes[PartsNum] = CurSize;
		Z[PartsNum] = new DX_DEPTH[CurSize];
		ID[PartsNum] = new DX_ID[CurSize];

		// NEW
		TAGS[PartsNum] = new BYTE[CurSize];
		memset(TAGS[PartsNum], 0, CurSize);

		// End of NEW

		Next[PartsNum] = new DXMemID[CurSize];
		Flags[PartsNum] = new BDynBitArray(CurSize);
		PartsNumAllocated = PartsNum + 1;
	}
	Flags[PartsNum]->SetAllTrue();
	HolesNum[PartsNum] = 0;
	LastPos.Up = PartsNum;
	LastPos.Low = 0;
	++PartsNum;
	return true;
}

bool DXMemoryCell5X::Extend(void)
{
	if (PartsNum >= MaxPartsNum)
		return false;
	if (PartsNum >= PartsNumAllocated)
	{
		int CurSize = CalcCurSize();
		Sizes[PartsNum] = CurSize;
		C[PartsNum] = new DXCell5X[CurSize];
		iZ[PartsNum] = new __int16[CurSize];
		Next[PartsNum] = new DXMemID[CurSize];
		Flags[PartsNum] = new BDynBitArray(CurSize);
		PartsNumAllocated = PartsNum + 1;
	}
	Flags[PartsNum]->SetAllTrue();
	HolesNum[PartsNum] = 0;
	LastPos.Up = PartsNum;
	LastPos.Low = 0;
	++PartsNum;
	return true;
}

int DXMemoryOnePart::CalcCurSize(void)
{
	//int CurSize = BaseSize;
	//for (unsigned int i = 0; i < PartsNum && CurSize < 8000000; CurSize *= 2, ++i);
	//return min(CurSize, pow(2, 23));


	int LowSize_2 = int(ldexp(1., DXMemID::LowSize)) - 1;
	int CurSize = BaseSize;
	for (unsigned int i = 0; i < PartsNum && CurSize < LowSize_2; CurSize *= 2, ++i);
	return min(CurSize, LowSize_2);
}

DXMemID DXMemoryOnePart::InsertAfter(DXMemID Ind)
{
	DXMemID FreePos = FindEmptyPos();
	if (FreePos == DXP_END)
		return DXP_END;

	Next[FreePos.Up][FreePos.Low] = Next[Ind.Up][Ind.Low];
	Next[Ind.Up][Ind.Low] = FreePos;
	return FreePos;
}

void DXMemoryOnePart::InsertAfter(DXMemID Ind, DXMemID PIDVal)
{
	if (PIDVal == DXP_END)
		return;

	Next[PIDVal.Up][PIDVal.Low] = Next[Ind.Up][Ind.Low];
	Next[Ind.Up][Ind.Low] = PIDVal;
}
DXMemID DXMemoryOnePart::FindEmptyPos()
{
	BDynBitArray &FF = *Flags[LastPos.Up];
	if (IsLastPosEndPos)
	{
		++LastPos.Low;
		if (LastPos.Low < Sizes[LastPos.Up])
		{
			FF.SetFalseNext();
			ASSERT((*Flags[LastPos.Up])[LastPos.Low] == false);
			return LastPos;
		}
		--LastPos.Low;
	}
	IsLastPosEndPos = false;
	DXMemID BPos = LastPos;
	if (PartsNum > 0)
	{
		if (HolesNum[LastPos.Up] * 100 > Sizes[PartsNum - 1])
		{
			for (++LastPos.Low; LastPos.Low < Sizes[LastPos.Up]; ++LastPos.Low)
			{
				if (FF.SetFalse(LastPos.Low))
				{
					--HolesNum[LastPos.Up];
					return LastPos;
				}
			}
			for (LastPos.Low = 0; LastPos.Low < BPos.Low; ++LastPos.Low)
			{
				if (FF.SetFalse(LastPos.Low))
				{
					--HolesNum[LastPos.Up];
					return LastPos;
				}
			}
			HolesNum[LastPos.Up] = 0;
		}
		for (++LastPos.Up; LastPos.Up < PartsNum; ++LastPos.Up)
		{
			BDynBitArray &FU = *Flags[LastPos.Up];
			if (FU.IsEmpty())
			{
				LastPos.Low = 0;
				FU.SetFalseNext();
				IsLastPosEndPos = true;
				return LastPos;
			}
			if (HolesNum[LastPos.Up] * 100 > Sizes[PartsNum - 1])
			{
				for (LastPos.Low = 0; LastPos.Low < Sizes[LastPos.Up]; ++LastPos.Low)
				{
					if (FU.SetFalse(LastPos.Low))
					{
						--HolesNum[LastPos.Up];
						return LastPos;
					}
				}
				HolesNum[LastPos.Up] = 0;
			}
		}
		for (LastPos.Up = 0; LastPos.Up < BPos.Up; ++LastPos.Up)
		{
			BDynBitArray &FU = *Flags[LastPos.Up];
			if (HolesNum[LastPos.Up] * 100 > Sizes[PartsNum - 1])
			{
				for (LastPos.Low = 0; LastPos.Low < Sizes[LastPos.Up]; ++LastPos.Low)
				{
					if (FU.SetFalse(LastPos.Low))
					{
						--HolesNum[LastPos.Up];
						return LastPos;
					}
				}
				HolesNum[LastPos.Up] = 0;
			}
		}
	}
	if (!Extend())
		return DXP_END;
	Flags[LastPos.Up]->SetFalse(LastPos.Low);
	IsLastPosEndPos = true;
	return LastPos;
}

void DXMemoryOnePart::DeleteAll(void)
{
	LastPos.Up = 0;
	LastPos.Low = 0;
	LastPos.GlobPart = GlobPart;
	IsLastPosEndPos = false;
	PartsNum = 0;
}

int DXMemoryPtOnePart::GetGlobSize(void)
{
	// NEW                                                                       ________________
	return GetDataCount() * (sizeof(*Z[0]) + sizeof(*ID[0]) + sizeof(*Next[0]) + sizeof(*TAGS[0]));
	//                                                                           ----------------
}

int DXMemoryCell5X::GetGlobSize(void)
{
	return GetDataCount() * (sizeof(*C[0]) + sizeof(*iZ[0]) + sizeof(*Next[0]));
}

int DXMemoryPtOnePart::GetUsedSize(void)
{
	// NEW                                                                       ________________
	return GetUsedCount() * (sizeof(*Z[0]) + sizeof(*ID[0]) + sizeof(*Next[0]) + sizeof(*TAGS[0]));
	//                                                                           ----------------
}

int DXMemoryCell5X::GetUsedSize(void)
{
	return GetUsedCount() * (sizeof(*C[0]) + sizeof(*iZ[0]) + sizeof(*Next[0]));
}

unsigned int DXMemoryOnePart::GetDataCount(void) const
{
	unsigned int Sum = 0;
	for (unsigned int i = 0; i < PartsNumAllocated; ++i)
		Sum += Sizes[i] / 1024;
	return Sum;
}

unsigned int DXMemoryOnePart::GetUsedCount(void) const
{
	unsigned int Sum = 0;
	for (unsigned int i = 0; i < PartsNum; ++i)
		for (unsigned int j = 0; j < Sizes[i]; ++j)
			Sum += ((*Flags[i])[j]) ? 0 : 1;

	return Sum/1024;
}

void DXMemoryOnePart::Save(MForestSave& SaveData) const
{
	CFile* f = SaveData.File;
	f->Write(&PartsNum, sizeof(PartsNum));
	f->Write(&PartsNumAllocated, sizeof(PartsNumAllocated));
	f->Write(&GlobPart, sizeof(GlobPart));
	f->Write(&LastPos, sizeof(LastPos));
	f->Write(&IsLastPosEndPos, sizeof(IsLastPosEndPos));
	for (unsigned int i = 0; i < PartsNum; ++i)
	{
		f->Write(&Sizes[i], sizeof(Sizes[0]));
		f->Write(&HolesNum[i], sizeof(HolesNum[0]));
		f->Write(Next[i], sizeof(Next[0][0]) * Sizes[i]);
		Flags[i]->Save2File(f);
	}
}

void DXMemoryOnePart::Load(CFile* f)
{
	f->Read(&PartsNum, sizeof(PartsNum));
	f->Read(&PartsNumAllocated, sizeof(PartsNumAllocated));
	f->Read(&GlobPart, sizeof(GlobPart));
	f->Read(&LastPos, sizeof(LastPos));
	f->Read(&IsLastPosEndPos, sizeof(IsLastPosEndPos));
	for (unsigned int i = 0; i < PartsNum; ++i)
	{
		f->Read(&Sizes[i], sizeof(Sizes[0]));
		f->Read(&HolesNum[i], sizeof(HolesNum[0]));
		Next[i] = new DXMemID[Sizes[i]];
		f->Read(Next[i], sizeof(Next[0][0]) * Sizes[i]);
		Flags[i] = BDynBitArray::Load(f);
	}
}

void DXMemoryOnePart::CreateFullCopy(DXMemoryOnePart* pIn) const
{
	pIn->PartsNum = PartsNum;
	pIn->PartsNumAllocated = PartsNumAllocated;
	pIn->GlobPart = GlobPart;
	pIn->LastPos = LastPos;
	pIn->IsLastPosEndPos = IsLastPosEndPos;
	for (unsigned int i = 0; i < PartsNum; ++i)
	{
		pIn->Sizes[i] = Sizes[i];
		pIn->HolesNum[i] = HolesNum[i];
		pIn->Next[i] = new DXMemID[Sizes[i]];
		memcpy_s(pIn->Next[i], sizeof(Next[0][0]) * Sizes[i], Next[i], sizeof(Next[0][0]) * Sizes[i]);
		pIn->Flags[i] = Flags[i]->CreateFullCopy();
	}
}

void DXMemoryPtOnePart::GetTransfData(MBSPForest::NTransfData & TransfData) const
{
	for (unsigned int i = 0; i < PartsNum; ++i)
	{
		for (unsigned int j = 0; j < Sizes[i]; ++j)
		{
			if (((*Flags[i])[j]) == false)
			{
				int SurfInd = MFOREST.GetIDProc()->GetSurfID(ID[i][j].Elem);
				if (SurfInd >= 0 && SurfInd != ID_EMPTY)
					TransfData.SurfIndexes.emplace(SurfInd);
			}
		}
	}
}

void DXMemoryPtOnePart::Save(MForestSave& SaveData) const
{
	DXMemoryOnePart::Save(SaveData);
	CFile* f = SaveData.File;
	for (unsigned int i = 0; i < PartsNum; ++i)
		f->Write(Z[i], sizeof(Z[0][0]) * Sizes[i]);
	for (unsigned int i = 0; i < PartsNum; ++i)
	{
		ElemID* buf = new ElemID[Sizes[i]];
		for (unsigned int k = 0; k < Sizes[i]; ++k)
		{
			if (!Flags[i]->GetAt(k)) // we should save used surfaces only
				buf[k] = SaveData.GetOrigInd(ID[i][k].Elem);
			else
				buf[k] = 0;
		}
		f->Write(buf, sizeof(buf[0]) * Sizes[i]);
		delete[] buf;
	}
}

void DXMemoryPtOnePart::Load(CFile* f)
{
	DXMemoryOnePart::Load(f);
	for (unsigned int i = 0; i < PartsNum; ++i)
	{
		Z[i] = new DX_DEPTH[Sizes[i]];
		f->Read(Z[i], sizeof(Z[0][0]) * Sizes[i]);
	}
	for (unsigned int i = 0; i < PartsNum; ++i)
	{
		ID[i] = new DX_ID[Sizes[i]];
		ElemID* buf = new ElemID[Sizes[i]];
		f->Read(buf, sizeof(buf[0]) * Sizes[i]);//BUG
		for (unsigned int k = 0; k < Sizes[i]; ++k)
			ID[i][k] = DX_ID(buf[k], cadrID(-1));
		delete[] buf;
	}

	// NEW
	for (unsigned int i = 0; i < PartsNum; ++i) {
		TAGS[i] = new BYTE[Sizes[i]];
		//std::fill(TAGS[i], TAGS[i] + Sizes[i], 0);
		//memset(TAGS[i], 0, Sizes[i]);
		for (unsigned int j = 0; j < Sizes[i]; ++j) {
			TAGS[i][j] = 0;
		}
	}
}

void DXMemoryPtOnePart::InitTags() {
	for (unsigned int i = 0; i < PartsNum; ++i) {
		TAGS[i] = new BYTE[Sizes[i]];
		//std::fill(TAGS[i], TAGS[i] + Sizes[i], 0);
		//memset(TAGS[i], 0, Sizes[i]);
		memset(TAGS[i], 0, Sizes[i]);
	}
}

void DXMemoryPtOnePart::CreateFullCopy(DXMemoryOnePart* pInt) const
{
	DXMemoryOnePart::CreateFullCopy(pInt);
	DXMemoryPtOnePart* pIn = dynamic_cast<DXMemoryPtOnePart*>(pInt);
	if (pIn == nullptr)
		return;
	for (unsigned int i = 0; i < PartsNum; ++i)
	{
		pIn->Z[i] = new DX_DEPTH[Sizes[i]];
		memcpy_s(pIn->Z[i], sizeof(Z[0][0]) * Sizes[i], Z[i], sizeof(Z[0][0]) * Sizes[i]);
	}
	for (unsigned int i = 0; i < PartsNum; ++i)
	{
		pIn->ID[i] = new DX_ID[Sizes[i]];
		memcpy_s(pIn->ID[i], sizeof(ID[0][0])* Sizes[i], ID[i], sizeof(ID[0][0])* Sizes[i]);
	}

	// NEW
	for (unsigned int i = 0; i < PartsNum; ++i)
	{
		pIn->TAGS[i] = new BYTE[Sizes[i]];
		memcpy_s(pIn->TAGS[i], sizeof(TAGS[0][0]) * Sizes[i], TAGS[i], sizeof(TAGS[0][0]) * Sizes[i]);
	}
}

void DXMemoryPtOnePart::ReplOInd(ElemID* In2Out)
{
	for (unsigned int i = 0; i < PartsNum; ++i)
	{
		for (unsigned int j = 0; j < Sizes[i]; ++j)
		{
			if (!Flags[i]->GetAt(j))
			{
				ElemID ElID = ID[i][j].Elem;
				MFOREST.GetIDProc()->SetPCn(&ElID, STOCK_COLOR_NUM);
				int BFaceN = MFOREST.GetIDProc()->GetSurfID(ElID);
				if (BFaceN >= 0 && BFaceN != ID_EMPTY)
				{
					BFaceN = In2Out[BFaceN];
					MFOREST.GetIDProc()->SetSurfID(&ElID, BFaceN);
					ID[i][j].Elem = ElID;
				}
			}
		}
	}
}

void DXMemoryOnePart::RemoveAt(DXMemID Ind)
{
	++HolesNum[Ind.Up];
	ASSERT((*Flags[Ind.Up])[Ind.Low] == false);
	Flags[Ind.Up]->SetAt(Ind.Low, true);
}

void DXMemoryPt::Save(MForestSave& SaveData) const
{
	for (auto i = 0; i < GlobPartsNum; ++i)
		DirParts[i].Save(SaveData);
	// save used orig
	CFile* f = SaveData.File;
	int OrigSize = int(SaveData.OrigSaveMap.NewInd2OldInd.size());
	f->Write(&OrigSize, sizeof(OrigSize));
	for (int i = 0; i < OrigSize; ++i)
	{
		auto* pSurf = MFOREST.GetOrigSurf(int(SaveData.OrigSaveMap.NewInd2OldInd[i]));
		if (pSurf == nullptr)
			SOrigSurf::SaveDummy(SaveData.File);
		else
			pSurf->Save(SaveData.File, SaveData.Matr);
	}
}

DXMemoryPt::DXMemoryPt()
{ 
	for (unsigned int i = 0; i < GlobPartsNum; ++i)
		DirParts[i].SetGlobPart(i);
}


void DXMemoryPt::Load(CFile* f)
{
	for (auto i = 0; i < GlobPartsNum; ++i)
		DirParts[i].Load(f);
	// load used surfaces
	int OrigSize;
	f->Read(&OrigSize, sizeof(OrigSize));
	ElemID* In2Out = new ElemID[OrigSize];

	for (int i = 0; i < OrigSize; ++i)
	{
		SOrigSurf* pSurf = SOrigSurf::StLoad(f);
		In2Out[i] = MFOREST.AddOrigSurf(pSurf);
	}
	// replace surfaces indexes
	for (auto i = 0; i < GlobPartsNum; ++i)
	{
		DirParts[i].ReplOInd(In2Out);
	}
	delete In2Out;
}

void DXMemoryPt::CreateFullCopy(DXMemoryPt* pIn) const
{
	for (auto i = 0; i < GlobPartsNum; ++i)
		DirParts[i].CreateFullCopy(&pIn->DirParts[i]);
}
