#pragma once
#include <set>
#include <vector>
#include "NCMDoc.h"
#include "NSyncCheckPoints.h"

class NSyncGenerator
{
public:
	class ChannelData
	{
		friend class NSyncGenerator;
	public:
		ChannelData(NChannel* pInChannel);
		~ChannelData();
		void SetTimesCn();
		const NCheckPoint* GetFreeCP() const;
		const NChannel& GetChannel() const { return *pChannel; }
		NChannel& GetChannel() { return *pChannel; }
	protected:
		NCheckPoint::NSyncID ID2Wait;
		cadrID CPCurInd;
		class BProgram* pCurProg;
		class BProgram* pNextProg;
		cadrID CPNextInd;
		MainTime CurTime;
		NChannel* pChannel;
		int FreeCPInd;
	};
public:
	NSyncGenerator(CNCMDoc& InDoc);
	~NSyncGenerator();
	void SetTimes();
	const NCheckPoint* FindNextFreeCP();
protected:
	CNCMDoc& Doc;
	std::set<NCheckPoint::NSyncID, NCheckPoint::NSyncID::Less> ActSyncIDs;
	std::vector<ChannelData> ChanDatas;
};

