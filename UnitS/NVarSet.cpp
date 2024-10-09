#include "stdafx.h"
#include "NVarSet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

NVarSet::NVarSet()
{
	Clear();
}

NVarSet::~NVarSet()
{
}

void NVarSet::Clear()
{
	IsReplEmpty = true;
	Defines.clear();
	RegsMaps.clear();
	PushRegsMap();
	PushRegsMap();
}

bool NVarSet::Apply(CString & String)
{
	// do replace
	if (IsReplEmpty)
		return false;
	bool Res = false;
	// Apply defines
	for each (auto el in Defines)
		Res |= (0 != String.Replace(el.first, el.second));
	auto Regs0Ind = RegsMaps.size() > 0 ? 0 : -1;
	auto Regs1Ind = RegsMaps.size() > 1 ? 0 : -1;
	int LenR0 = -1;
	int LenR1 = -1;
	if (Regs0Ind != -1)
	{
		if (Regs0Ind < RegsMaps.front().Re2Set.RegExps.size())
			LenR0 = int(RegsMaps.front().Re2Set.RegExps.at(Regs0Ind).text.length());
		else
			Regs0Ind = -1;
	}
	if (Regs1Ind != -1)
	{
		if (Regs1Ind < RegsMaps.back().Re2Set.RegExps.size())
			LenR1 = int(RegsMaps.back().Re2Set.RegExps.at(Regs1Ind).text.length());
		else
			Regs1Ind = -1;
	}
	while (Regs0Ind != -1 || Regs1Ind != -1)
	{
		// find max length
		if (LenR0 >= LenR1)
		{
			string str(String);
			if (RegsMaps.front().Re2Set.Apply(Regs0Ind, str))
			{
				String = CString(str.data());
				Res = true;
			}
			Regs0Ind++;
			if (Regs0Ind < RegsMaps.front().Re2Set.RegExps.size())
				LenR0 = int(RegsMaps.front().Re2Set.RegExps.at(Regs0Ind).text.length());
			else
			{
				LenR0 = -1;
				Regs0Ind = -1;
			}
		}
		else// (LenR1 > LenR0)
		{
			string str(String);
			if (RegsMaps.back().Re2Set.Apply(Regs1Ind, str))
			{
				String = CString(str.data());
				Res = true;
			}
			Regs1Ind++;
			if (Regs1Ind < RegsMaps.back().Re2Set.RegExps.size())
				LenR1 = int(RegsMaps.back().Re2Set.RegExps.at(Regs1Ind).text.length());
			else
			{
				LenR1 = -1;
				Regs1Ind = -1;
			}
		}
	}
	return Res;
}

bool NVarSet::AddReplacePair(const CString & RegExp, const CString & ReplExp, bool LocalVar)
{
	IsReplEmpty = false;
	BRegsMap& Map = LocalVar ? RegsMaps.back() : RegsMaps.front();
	return Map.Re2Set.AddReplacePairSort(RegExp, ReplExp);
}

bool NVarSet::AddDefinePair(const CString & Src, const CString & Dest)
{
	IsReplEmpty = false;

	int i = 0;
	for (; i < Defines.size(); ++i)
	{
		const size_t len = Defines[i].first.GetLength();
		if (len < Src.GetLength())
			break;
	}
	Defines.insert(Defines.begin() + i, pair<CString, CString>(Src, Dest));
	return true;
}

void NVarSet::PushRegsMap()
{
	int LastIndex = 0;
	switch (RegsMaps.size())
	{
	case 0:
		LastIndex = START_INDEX;
		break;
	case 1:
		LastIndex = START_INDEX + START_SIZE;
		break;
	default:
		LastIndex = RegsMaps.back().lastIndex;
		break;
	}
	RegsMaps.resize(RegsMaps.size() + 1);
	RegsMaps.back().clear();
	RegsMaps.back().lastIndex = LastIndex;
}

void NVarSet::PopRegsMap()
{
	if(RegsMaps.size() > 1)
		RegsMaps.pop_back();
}

int NVarSet::StoreName(const string& name, bool LocalVar)
{
	return GetActRegsMap(LocalVar).storeName(name);
}

int NVarSet::StoreArrName(const string& name, bool LocalVar, int size)
{
	return GetActRegsMap(LocalVar).storeArrName(name, size);
}

BRegsMap& NVarSet::GetActRegsMap(bool LocalVar)
{
	return LocalVar ? RegsMaps.back() : RegsMaps.front();
}

bool NVarSet::HasIndex(int i) const
{
	if (RegsMaps.back().hasIndex(i))
		return true;
	return RegsMaps.front().hasIndex(i);
}

CString NVarSet::GetName(int i, int& Delta) const
{
	CString Ret(RegsMaps.back().getName(i, Delta).c_str());
	if(Ret.IsEmpty())
		return CString(RegsMaps.front().getName(i, Delta).c_str());
	return Ret;
}
