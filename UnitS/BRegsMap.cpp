#include "stdafx.h"
#include <algorithm>
#include "BRegsMap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BRegsMap::BRegsMap()
{
	init();
}


BRegsMap::~BRegsMap()
{
}

void BRegsMap::init()
{
}

void BRegsMap::clear()
{
	regsMap.clear();
	regsMapInv.clear();
	Re2Set.Clear();
	init();
}

string BRegsMap::getName(int i, int &delta) const
{
	delta = -1;
	const auto found = lower_bound(arrayLimits.begin(), arrayLimits.end(), pair<int, int>(i, i),
		[](pair<int, int> p, pair<int, int> q) { return p.second <= q.first; });

	if (found != arrayLimits.end())
	{
		if ((*found).first <= i && (*found).second > i)
		{

			const auto it1 = regsMap.find((*found).first);
			if (it1 != regsMap.end())
			{
				if((*found).second - (*found).first > 1)
					delta = i - (*found).first;
				return (*it1).second;
			}
		}
	}
	delta = -1;
	const auto it = regsMap.find(i);
	if (it != regsMap.end())
		return (*it).second;
	return string();
}

void BRegsMap::storePair(int index, const string & name)
{
	regsMap[index] = name;
	regsMapInv[name] = index;
}

int BRegsMap::storeName(const string & name)
{
	int index = getIndex(name);
	if (index >= 0)
		return index;
	++lastIndex;

	storePair(lastIndex, name);
	return lastIndex;
}

int BRegsMap::storeArrName(const string & name, int size /*= 0*/)
{
	int index = getIndex(name);
	if (index >= 0)
		return index;
	++lastIndex;
	size = (size == 0) ? defaultSize : size;

	storePair(lastIndex, name);
	auto res = lastIndex;
	arrayLimits.push_back(pair<int, int>(lastIndex, lastIndex + size));
	inplace_merge(arrayLimits.begin(), arrayLimits.end() - 1, arrayLimits.end(), [](const pair<int, int> & a, const pair<int, int> & b) { return a.first < b.first; });

	lastIndex += size - 1;
	return res;
}

int BRegsMap::getIndex(const string & name) const
{
	const auto it = regsMapInv.find(name);
	if (it == regsMapInv.end())
		return -1;
	return (*it).second;
}

bool BRegsMap::hasIndex(int i) const
{
	const auto it = regsMap.find(i);
	if (it != regsMap.end())
		return true;

	const auto found = lower_bound(arrayLimits.begin(), arrayLimits.end(), pair<int, int>(i, i),
		[](pair<int, int> p, pair<int, int> q) { return p.second <= q.first; });
	if (found != arrayLimits.end())
	{
		if ((*found).first <= i && (*found).second > i)
			return true;
	}
	return false;
}

bool BRegsMap::apply(CString& String)
{
	Re2Set.Apply(String);
	return true;
}

