#pragma once
#include "UNITS_API.h"
#include "cadrID.h"
#include <map>
#include <stack>
#include <vector>
#include "NProgError.h"
using namespace std;

class UNITS_API ProgGroupMarks
{
	typedef pair<unsigned int, unsigned int> GroupEnd;// first - group end cadrID, second - group index
public:
	ProgGroupMarks();
	~ProgGroupMarks();
	bool IsGroupName(const CString& name) const;
	bool IsStartGroupName(const CString& name) const;
	bool IsEndGroupName(const CString& name) const;
	int GetGroup(const CString& name) const;
	cadrID GetEndOfGroup(cadrID startLine) const;
	cadrID GetEndOfGroupChain(cadrID startLine) const;
	int AddWord(const CString& name, cadrID lineInd);
	int AddSubStart(cadrID lineInd);
	int AddSubEnd(cadrID lineInd);
	int CalcGlobLevel(cadrID lineInd) const;
	void Restart(size_t progsCount);
	int CloseOneProg(cadrID linesCount);
	pair<unsigned int, GroupEnd> GetSmallestGroup(cadrID lineInd) const;
	bool empty() const { return endLineByStartLine_.empty(); }
	void AddError(cadrID curCadr, int errCode);
	void SetErrorProgName(int progInd, const CString& progName);
	const NProgError* GetProgError(int progInd, int errInd) const;// returns nullptr when finished
	bool IsGroupCycle(const GroupEnd& groupEnd) const;
	bool CloseActGroup(cadrID lineInd);
	bool OpenGroup(cadrID lineInd, int groupInd);
protected:
	int GetGroupToEnd() const;

protected:
	map<CString, int> numbers_;// +- group number. + for start string and - for end string. Group number starts from 1
	vector<map<unsigned int, GroupEnd>> endLineByStartLine_;// vector index is the number of program file (cadrID.Prog)
	vector<CString> endWordByGroupInd_;
	vector<CString> ifExtras_; // first - else, second - elseif
	stack<pair<int, cadrID>> activeStack_;// group index (0 based), start cadr
	vector<vector<NProgError>> groupErrors_;
	// First name should be if name. Group 1 - subprogram; 2 - if; other - cycles
	inline static const CString starts_[] = { _T("IF"), _T("WHILE"), _T("FOR"), _T("LOOP"), _T("REPEAT")};
	inline static const CString ends_[] = { _T("ENDIF"), _T("ENDWHILE"), _T("ENDFOR"), _T("ENDLOOP"), _T("UNTIL") };
	inline static const CString extras_[] = { _T("ELSE"), _T("ELSIF") };
	static constexpr int subrGroup_ = 0;
	static constexpr int ifGroup_ = 1;
};

