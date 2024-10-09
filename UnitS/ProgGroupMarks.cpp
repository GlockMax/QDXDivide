#include "..\Common\ProgGroupMarks.h"
#include "stdafx.h"
#include "ProgGroupMarks.h"

ProgGroupMarks::ProgGroupMarks()
{
	const auto count = sizeof(starts_) / sizeof(CString);
	endWordByGroupInd_.resize(count + 1);// +1 beckause of subrGroup_
	for (int i = 0; i < count; ++i)
	{
		const CString& name = starts_[i];
		numbers_.emplace(name, i + 2);// +2 beckause of subrGroup_
	}
	for (int i = 0; i < count; ++i)
	{
		const CString& name = ends_[i];
		numbers_.emplace(name, -(i + 2));// +2 beckause of subrGroup_
		endWordByGroupInd_.at(i + 1) = name;// +1 beckause of subrGroup_
	}
	const auto extra = sizeof(extras_) / sizeof(CString);
	ifExtras_.resize(extra);
	for (int i = 0; i < extra; ++i)
	{
		const CString& name = extras_[i];
		numbers_.emplace(name, -(0 + 2));// +2 beckause of subrGroup_ 0 is IF index
		ifExtras_[i] = name;
	}
}

ProgGroupMarks::~ProgGroupMarks()
{
}

void ProgGroupMarks::Restart(size_t progsCount)
{
	while (!activeStack_.empty())
		activeStack_.pop();
	for (size_t i = 0; i < endLineByStartLine_.size(); ++i)
	{
		endLineByStartLine_.at(i).clear();
		groupErrors_.at(i).clear();
	}
	endLineByStartLine_.resize(progsCount);
	groupErrors_.resize(progsCount);
}

int ProgGroupMarks::CloseOneProg(cadrID linesCount)
{
	const int res = AddSubEnd(linesCount);
	const bool error = !activeStack_.empty();
	while (!activeStack_.empty())
		activeStack_.pop();
	return 0;
}

pair<unsigned int, ProgGroupMarks::GroupEnd> ProgGroupMarks::GetSmallestGroup(cadrID lineInd) const
{
	if (lineInd.Prog < 0 || lineInd.Prog >= endLineByStartLine_.size())
		return pair<unsigned int, GroupEnd>(0, GroupEnd(0, 0));
	const auto& oneProg = endLineByStartLine_.at(lineInd.Prog);
	pair<unsigned int, GroupEnd> minGr(0, GroupEnd(MAXUINT, 0));
	unsigned int minGrLength = MAXUINT;
	for each (const auto& group in oneProg)
	{
		if (group.first < lineInd.Cadr && group.second.first >= lineInd.Cadr)
		{// lineInd belongs this group
			const auto len = group.second.first - group.first;
			if (len < minGrLength)
			{
				minGr = group;
				minGrLength = len;
			}
		}
	}
	return minGr;
}

void ProgGroupMarks::AddError(cadrID curCadr, int errCode)
{
	NProgError locErr;
	locErr.Set(curCadr, CString(), errCode);
	groupErrors_.at(curCadr.Prog).push_back(locErr);
}

void ProgGroupMarks::SetErrorProgName(int progInd, const CString& progName)
{
	auto& oneProgErr = groupErrors_.at(progInd);
	for (int i = 0; i < oneProgErr.size(); ++i)
		oneProgErr.at(i).SetProgName(progName);
}

const NProgError* ProgGroupMarks::GetProgError(int progInd, int errInd) const
{
	if(progInd < 0 || progInd >= groupErrors_.size())
		return nullptr;
	const auto& progErr = groupErrors_.at(progInd);
	if (errInd < 0 || errInd >= progErr.size())
		return nullptr;
	return &progErr.at(errInd);
}

bool ProgGroupMarks::IsGroupCycle(const GroupEnd& groupEnd) const
{
	return groupEnd.second > ifGroup_ + 1; // Group 1 - subprogram; 2 - if; other - cycles
}

int ProgGroupMarks::GetGroupToEnd() const
{// returns the group number for the last started group or 0
	int groupID = -1;
	if (!activeStack_.empty())
		groupID = activeStack_.top().first;
	return groupID + 1;
}

bool ProgGroupMarks::IsGroupName(const CString& name) const
{
	return IsStartGroupName(name) || IsEndGroupName(name);
}

bool ProgGroupMarks::IsStartGroupName(const CString& name) const
{
	return GetGroup(name) > 0;
}

bool ProgGroupMarks::IsEndGroupName(const CString& name) const
{
	return GetGroup(name) < 0;
}

int ProgGroupMarks::GetGroup(const CString& name) const
{// returns the group number or 0 if name is not group name
	const auto it = numbers_.find(name);
	if (it == numbers_.end())
		return 0;
	return it->second;
}

cadrID ProgGroupMarks::GetEndOfGroup(cadrID startLine) const
{// two groups can not have same startLine
	// returns the ending line number, or -1 if no group starts in this startLine
	const auto it = endLineByStartLine_.at(startLine.Prog).find(startLine.Cadr);
	if (it == endLineByStartLine_.at(startLine.Prog).end())
		return cadrID(-1);
	if(it->second.first == MAXINT)
		return cadrID(-1);
	return cadrID(startLine.Prog, it->second.first);
}

cadrID ProgGroupMarks::GetEndOfGroupChain(cadrID startLine) const
{
	for (cadrID endID = GetEndOfGroup(startLine); !endID.IsEmpty(); endID = GetEndOfGroup(endID))
	{
		const auto it = endLineByStartLine_.at(endID.Prog).find(endID.Cadr);
		if (it == endLineByStartLine_.at(endID.Prog).end())// current group is last group in the chain
			return endID;
	}
	return cadrID(-1);
}

int ProgGroupMarks::AddWord(const CString& name, cadrID lineInd)
{// returns groupInd or -error code
	int groupInd = GetGroup(name);
	if (groupInd == 0)
		return 0;
	if (groupInd > 0)
	{// group starts
		OpenGroup(lineInd, groupInd);
	}
	else
	{// group ends
		groupInd *= -1;
		const int actGroupInd = GetGroupToEnd();
		if (actGroupInd == 0)
		{
			AddError(lineInd, 1202);// ERROR no opened group exists
			return -1;
		}
		// Get correspondent end word
		const CString endWord = endWordByGroupInd_.at(actGroupInd - 1);
		if (endWord != name)
		{ // may be if group or error
			if (groupInd == 2 && actGroupInd == 2)
			{// if group
				if (name == ifExtras_[0] || name == ifExtras_[1]) // else
				{
					// close active group
					if (!CloseActGroup(lineInd))
						return -1;
					// open group
					OpenGroup(lineInd, groupInd);
				}
				else
				{
					AddError(lineInd, 1204);// ERROR internal error
					return -1;
				}
			}
			else
			{
				AddError(lineInd, 1203);// ERROR try to close wrong group
			// store end word expected
				int errGroupInd = activeStack_.top().first;
				groupErrors_.at(lineInd.Prog).back().Values[0] = endWordByGroupInd_.at(errGroupInd);
				return -1;
			}
		}
		else
		{
			if (!CloseActGroup(lineInd))
				return -1;
		}
	}

	return abs(groupInd);
}

bool ProgGroupMarks::CloseActGroup(cadrID lineInd)
{
	const auto it = endLineByStartLine_.at(lineInd.Prog).find(activeStack_.top().second.Cadr);
	if (it == endLineByStartLine_.at(lineInd.Prog).end())
	{
		AddError(lineInd, 1204);// ERROR internal error
		return false;
	}
	it->second.first = lineInd.Cadr;
	activeStack_.pop();
	return true;
}

bool ProgGroupMarks::OpenGroup(cadrID lineInd, int groupInd)
{
	endLineByStartLine_.at(lineInd.Prog).emplace(int(lineInd.Cadr), GroupEnd(MAXINT, groupInd));
	activeStack_.push(pair<int, cadrID>(groupInd - 1, lineInd));
	return true;
}

int ProgGroupMarks::AddSubStart(cadrID lineInd)
{
	const int res = AddSubEnd(lineInd);
	// start new subroutine
	endLineByStartLine_.at(lineInd.Prog).emplace(int(lineInd.Cadr), GroupEnd(MAXINT, 1));// 1 - subprogram group
	activeStack_.push(pair<int, cadrID>(subrGroup_, lineInd));
	return subrGroup_ + 1;
}

int ProgGroupMarks::AddSubEnd(cadrID lineInd)
{
	const int actGroupInd = GetGroupToEnd();// 1 based
	--lineInd;
	if (actGroupInd == subrGroup_ + 1)
	{
		// to end started subroutine
		if (activeStack_.empty())
		{
			AddError(lineInd, 1204);// ERROR internal error
			return -1;
		}
		const auto it = endLineByStartLine_.at(lineInd.Prog).find(activeStack_.top().second.Cadr);
		if (it == endLineByStartLine_.at(lineInd.Prog).end())
		{
			AddError(lineInd, 1204);// ERROR internal error
			return -1;
		}
		it->second.first = lineInd.Cadr;
		activeStack_.pop();
	}
	if (!activeStack_.empty())
	{
		AddError(lineInd, 1203);// ERROR active group
		// store end word expected
		int groupInd = GetGroupToEnd();// 1 based
		groupErrors_.at(lineInd.Prog).back().Values[0] = endWordByGroupInd_.at(groupInd - 1);
		return -1;
	}
	return subrGroup_ + 1;
}

int ProgGroupMarks::CalcGlobLevel(cadrID lineInd) const
{// calculate global nesting level for the given line Ind
	// global means respecting to all groups
	int level = 0;
	for each (auto pair in endLineByStartLine_.at(lineInd.Prog))
	{
		if (pair.first < lineInd.Cadr)
			level++;
		if (pair.second.first <= lineInd.Cadr)
			level--;
	}
	return level;
}
