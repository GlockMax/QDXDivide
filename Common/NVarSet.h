#pragma once
#include "BRegsMap.h"
#include <map>
#include "UNITS_API.h"

class UNITS_API NVarSet
{
public:
	NVarSet();
	virtual ~NVarSet();
	void Clear();
	bool Apply(CString &String);
	bool AddReplacePair(const CString &RegExp, const CString &ReplExp, bool LocalVar);
	bool AddDefinePair(const CString &Src, const CString &Dest);
	void PushRegsMap();
	void PopRegsMap();
	int StoreName(const string& name, bool LocalVar);
	int StoreArrName(const string& name, bool LocalVar, int size = 0);
	void SetDefaultName(const CString& Name) { DefaultName = Name; }
	bool HasIndex(int i) const;
	CString GetName(int i, int& delta) const;
protected:
	BRegsMap& GetActRegsMap(bool LocalVar);
protected:
	std::vector<BRegsMap> RegsMaps;
	std::vector<std::pair<CString, CString>> Defines;
	CString DefaultName;
	bool IsReplEmpty;
	static const int START_INDEX = 12000;
	static const int START_SIZE = 10000;
};

