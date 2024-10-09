#pragma once
#include "Collision_API.h"
#include "concurrent_vector.h"
#include "NCollisionID.h"

using namespace concurrency;
class COLLISION_API NCollTimeLine
{
public:
	NCollTimeLine();
	void Clear();
	void Sort(MainTime lastTime);
	void Add(const NCollisionID& collID);
	const NCollisionID* GetCollID(MainTime intStart, MainTime intEnd);
	const NCollisionID* GetCollID(MainTime intStart);
	std::pair<const NCollisionID*, const NCollisionID*> GetCollIDPair(MainTime intStart) const;
	void RemovePointer(class NCollidableObject* pPObj);
	void StopCreating();
	const NCollisionID& GetCollIDByTime(MainTime time) const;
protected:
	static bool Lesser(const NCollisionID& a0, const NCollisionID& a1);
protected:
	typedef concurrent_vector<NCollisionID> timeLineType;
	timeLineType timeLine_;
	std::atomic<MainTime> lastTime_;
	MainTime lastStartTime_;
	timeLineType::iterator	lastStartPos_;
	MainTime checkedTime_;
	timeLineType::iterator checkedPos_;
	timeLineType::iterator lastSorted_;
	critical_section crSect_;
};