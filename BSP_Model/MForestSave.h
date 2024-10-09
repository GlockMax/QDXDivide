#pragma once
#include "BSP_MODEL_API.h"
#include <map>
#include <vector>
#include "BMatr.h"
#include "MElemIDProc.h"

class BSP_MODEL_API MForestSave
{
public:
	class SaveMap
	{
	public:
		SaveMap() {}
		~SaveMap() {}
		int AddOldInd(int OldInd);
		void ResetLoc();
	public:
		std::map<int, int> OldInd2NewInd;
		std::vector<int> NewInd2OldInd;
		std::map<int, int> OldInd2NewIndLoc;
		std::vector<int> NewInd2OldIndLoc;
	};
public:
	MForestSave(CFile *f, const BMatr &M) : Matr(M) { File = f; }
	~MForestSave() {};
	void SavePointInd(int Ind);
	void SaveOrigInd(ElemID Ind);
	int GetPointInd(int Ind);
	ElemID GetOrigInd(ElemID Ind);
public:
	CFile *File;
	SaveMap OrigSaveMap;
	SaveMap PointSaveMap;
	const BMatr &Matr;
};

