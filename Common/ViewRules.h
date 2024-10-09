#pragma once
#include "NCMB_API.h"
#include <vector>
#include <map>

typedef std::vector<int> VConditions;
typedef std::vector<std::pair<std::pair<UINT, VConditions>, std::pair<int, WORD>>> VRules;
typedef std::map<WORD, VRules> ViewRulesMap;
class NCMB_API ViewRules
{
public:
	ViewRules();
	~ViewRules();
	void AddRule(WORD Message, UINT nFlag, const VConditions & Conditions, int ViewActionInd, int StopMessage);
	bool CheckConditions(const VConditions & Conditions) const;
	std::pair<int, WORD> GetActionInd(WORD Message, UINT nFlags) const;
	void Clear() { Rules.clear(); }
protected:
	ViewRulesMap Rules;
};

