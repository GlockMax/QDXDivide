#pragma once
#include <map>
#include "BMatr.h"
class CopyMap
{
public:
	CopyMap(const BMatr &M);
	virtual ~CopyMap();
	int AddOldIndOr(int OldInd);
	int AddOldIndPt(int OldInd);

public:
	std::map<int, int> OldInd2NewIndOr;
	std::map<int, int> OldInd2NewIndPt;
	BMatr Matr;
};

