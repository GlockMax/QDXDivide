#pragma once
#include "UNITS_API.h"
#include <set>
#include "NCBase.h"

const int MAX_UCS = 310;// Maximum number of the UCSs

class UNITS_API NUCSs
{
public:
	NUCSs();
	NUCSs(const NUCSs &In);
	virtual ~NUCSs();
	const NCBase & operator[] (int i) const;
	NCBase & operator[] (int i);
	void SetUsed(int i);
	void Serialize(CArchive & ar);
	std::set<int> &GetUsed() { return Used; }
	const std::set<int> &GetUsed() const { return Used; }
	bool GetDrawName() const { return DrawName; }
	void SetDrawName(bool Val) { DrawName = Val; }
	void ClearUsed() { Used.clear(); }
	void ClearNames();
	static int MaxUcsInd;
protected:
	NCBase UCS[MAX_UCS];
	std::set<int> Used;
	bool DrawName;
};

