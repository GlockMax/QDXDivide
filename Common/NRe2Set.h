#pragma once
#include "EDITOR_API.h"
#include <memory>
#include <string>
#include <vector>


class EDITOR_API NRe2Set
{
	friend class NVarSet;
	class RE2cont
	{
		friend class NRe2Set;
		friend class NVarSet;
	public:
		RE2cont();
		RE2cont(const std::string& itext);
		RE2cont(const RE2cont& In);
		RE2cont& operator=(const RE2cont& In);
		~RE2cont();
	protected:
		std::string text;
		class std::unique_ptr<class RE2> pRE2;
	};
public:
	NRe2Set(void);
	~NRe2Set(void);

protected:
	std::vector<RE2cont> RegExps;
	std::vector<std::string> ReplExps;
	std::vector<int> Pri;

public:
	void AddReplacePair(int Priority, const CString& RegExp, const CString& ReplExp);
	int AddReplacePairSort(const CString& RegExp, const CString& ReplExp);
	virtual void Clear();
	bool Apply(CString &String);
	bool Apply(int ind, std::string &String);
};

