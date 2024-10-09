#include "stdafx.h"
#include "ViewRules.h"

using namespace std;

ViewRules::ViewRules()
{
}


ViewRules::~ViewRules()
{
}

void ViewRules::AddRule(WORD Message, UINT nFlag, const VConditions & Cond, int ViewActionInd, int StopMessage)
{
	VRules &MRules = Rules[Message];
	MRules.push_back(make_pair(make_pair(nFlag, Cond), make_pair(ViewActionInd, StopMessage)));
}

bool ViewRules::CheckConditions(const VConditions & Cond) const
{
	for each (int vKey in Cond)
	{
		bool Res = ((GetAsyncKeyState(abs(vKey)) & 0xFFFFFFFE) != 0);
		if (Res != (vKey > 0))
			return false;
	}
	return true;
}

pair<int, WORD> ViewRules::GetActionInd(WORD Message, UINT nFlags) const
{
	auto it = Rules.find(Message);
	if (it != Rules.end())
	{
		const VRules &MRules = it->second;
		for (auto i = MRules.begin(); i != MRules.end(); ++i)
		{
			if (i->first.first == nFlags)
			{
				if (CheckConditions(i->first.second))
					return i->second;
			}
		}
	}
	return make_pair(-1, 0);
}
