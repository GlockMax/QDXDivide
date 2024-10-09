#pragma once
#include "UNITS_API.h"
#include <map>
#include <vector>
#include <string>
#include <utility>
#include "NRe2Set.h"

using namespace std;

class UNITS_API BRegsMap
{
	friend class NVarSet;
public:
	BRegsMap();
	~BRegsMap();
	void clear();
	string getName(int i, int &delta) const;
	int storeName(const string& name);
	int storeArrName(const string& name, int size = 0);
	INT_PTR GetSize() const { return regsMap.size(); }
	bool hasIndex(int i) const;
	bool apply(CString& String);
protected:
	void storePair(int index, const string& name);
	void init();
	int getIndex(const string& name) const;
protected:
	map<int, string> regsMap;
	map<string, int> regsMapInv;
	NRe2Set Re2Set;
	vector<pair<int, int>> arrayLimits;
	int lastIndex;
	static const int defaultSize = 1000;
};

