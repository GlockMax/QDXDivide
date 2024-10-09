#include "StdAfx.h"
#undef min
#undef max
#include <memory>
#include "re2/re2/re2.h"
#include "NRe2Set.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using namespace re2;

NRe2Set::RE2cont::RE2cont()
{
	pRE2 = nullptr;
}

NRe2Set::RE2cont::RE2cont(const std::string& itext)
{
	text = itext;
	pRE2 = nullptr;
}

NRe2Set::RE2cont::RE2cont(const RE2cont& In)
{
	text = In.text;
	pRE2 = nullptr;
}

NRe2Set::RE2cont& NRe2Set::RE2cont::operator=(const RE2cont& In)
{
	text = In.text;
	pRE2 = nullptr;
	return *this;
}

NRe2Set::RE2cont::~RE2cont()
{
//	delete pRE2.release();
//	pRE2 = nullptr;
}


NRe2Set::NRe2Set(void)
{
}

NRe2Set::~NRe2Set(void)
{
	Clear();
}

void NRe2Set::Clear()
{
	RegExps.clear();
	ReplExps.clear();
	Pri.clear();
}

void NRe2Set::AddReplacePair(int Priority, const CString &RegExp, const CString &ReplExp)
{
	if(Pri.empty())
	{
		Pri.push_back(Priority);
		RegExps.push_back(string(RegExp));
		ReplExps.push_back(string(ReplExp));
		return;
	}
	auto i = Pri.size();
	for (; i > 0; --i)
		if (Pri[i - 1] <= Priority)
			break;
	Pri.insert(Pri.begin() + i, Priority);
	RegExps.insert(RegExps.begin() + i, string(RegExp));
	ReplExps.insert(ReplExps.begin() + i, string(ReplExp));
}

int NRe2Set::AddReplacePairSort(const CString& RegExp, const CString& ReplExp)
{
	// Assume Priority = 0
	int i = 0;
	for (; i < RegExps.size(); ++i)
	{
		if (Pri[i] != 0)
			continue;
		const size_t len = RegExps[i].text.length();
		if (len < RegExp.GetLength())
			break;
	}
	RegExps.insert(RegExps.begin() + i, string(RegExp));
	ReplExps.insert(ReplExps.begin() + i, string(ReplExp)); 
	Pri.insert(Pri.begin() + i, 0);
	return 0;
}

bool NRe2Set::Apply(int ind, std::string &String)
{
	if (RegExps[ind].pRE2 == nullptr)
	{
		RegExps[ind].pRE2 = std::make_unique<RE2>(RegExps[ind].text, RE2::CannedOptions::Latin1);
		if (!RegExps[ind].pRE2->ok())
			return false;
		std::string Error;
		if (!RegExps[ind].pRE2->CheckRewriteString(ReplExps[ind], &Error))
			return false;
	}
	const int Res = RE2::GlobalReplace(&String, *RegExps[ind].pRE2, ReplExps[ind]);
	return Res > 0;
}

bool NRe2Set::Apply(CString &String)
{
	std::string s(String);

	bool Res = false;
	for(int i = 0; i < RegExps.size(); ++i) 
	{
		Res |= Apply(i, s);
	}
	if(Res)
		String = CString(s.data());
	return Res;
}
