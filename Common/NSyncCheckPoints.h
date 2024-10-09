#pragma once
#include "UNITS_API.h"
#include <vector>

class UNITS_API NCheckPoint
{
public:
	class UNITS_API NSyncID
	{
	public:
		NSyncID();
		NSyncID(int Chan, int Val);
		NSyncID(int Chan, const CString& Val);
		~NSyncID();
		bool IsCorr(const NSyncID& ID);
		bool IsEmpty() const;
		void SetEmpty();
		struct Less
		{
			bool operator()(const NCheckPoint::NSyncID& p1, const NCheckPoint::NSyncID& p2) const
			{
				if (p1.ChanInd != p2.ChanInd)
					return p1.ChanInd < p2.ChanInd;
				if (p1.IVal == 0 && p2.IVal == 0)
					return p1.SVal < p2.SVal;
				if (p1.IVal == 0)
					return true;
				if (p2.IVal == 0)
					return false;

				return p1.IVal < p2.IVal;
			}
		};

	protected:
		int IVal;
		CString SVal;
		bool Equal;
		int ChanInd;
	};
public:
	enum class NCheckPointType
	{
		CP_Event,
		CP_Wait,
		CP_EventRem,
		CP_Empty
	};
	NCheckPoint();
	~NCheckPoint();
	static bool Compare(const NCheckPoint& elem1, const NCheckPoint& elem2);
	void SetWait() noexcept { Type = NCheckPointType::CP_Wait; }
	void SetEvent() noexcept { Type = NCheckPointType::CP_Event; }
	void SetEventRem() noexcept { Type = NCheckPointType::CP_EventRem; }
	void SetBefore() noexcept { IsBefore = true; }
	void SetAfter() noexcept { IsBefore = false; }
	bool GetBefore() const noexcept { return IsBefore; }
	int GetPriority() const noexcept { return Priority; }
	void SetPriority(int Val) noexcept { Priority = Val; }
	int GetChanInd() const noexcept { return ChanInd; }
	void SetChanInd(int Val) noexcept { ChanInd = Val; }
	int GetCadrHistInd() const noexcept { return CadrHistInd; }
	void SetCadrHistInd(int Val, class BProgram* pProg) noexcept;
	void SetID(const NSyncID& ID) noexcept { SyncID = ID; }
	bool IsWait() const noexcept { return Type == NCheckPointType::CP_Wait; }
	bool IsEvent() const noexcept { return Type == NCheckPointType::CP_Event; }
	bool IsEventRem() const noexcept { return Type == NCheckPointType::CP_EventRem; }
	MainTime GetTime() const;
	BProgram* GetProg() const noexcept { return pProg; }
	const NSyncID& GetSyncID() const noexcept { return SyncID; }
protected:
	NSyncID SyncID;
	int ChanInd;
	int CadrHistInd;
	class BProgram* pProg;
	int Priority;
	bool IsBefore;
	NCheckPointType Type;
};


class UNITS_API NSyncCheckPoints : public std::vector<NCheckPoint>
{
public:
public:
	NSyncCheckPoints();
	~NSyncCheckPoints();
};

