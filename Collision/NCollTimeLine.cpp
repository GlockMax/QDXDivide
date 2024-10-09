#include "StdAfx.h"
#include "NCEnums.h"
#include "BTimeLine.h"
#include "NCollTimeLine.h"

NCollTimeLine::NCollTimeLine()
{
	Clear();
}

void NCollTimeLine::Clear()
{
	critical_section::scoped_lock lock(crSect_);
	timeLine_.clear();
	lastTime_ = 0;
	lastSorted_ = timeLine_.end();
	checkedTime_ = 0;
	checkedPos_ = timeLine_.begin();
	lastStartTime_ = 0;
	lastStartPos_ = timeLine_.begin();
}

void NCollTimeLine::Sort(MainTime lastTime)
{
	sort(lastSorted_, timeLine_.end(), Lesser);
	lastSorted_ = timeLine_.end();
	lastTime_ = lastTime;
}

void NCollTimeLine::Add(const NCollisionID& collID)
{
	if (!collID.NeedPlaceInTimeLine())
		return;
	if (collID.CollType == LE_NOCOLL)// fix time without collision
	{
		critical_section::scoped_lock lock(crSect_);
		if(timeLine_.empty())
			timeLine_.push_back(collID);
		else
		{
			const NCollisionID& lastCollID = timeLine_.back();
			if (lastCollID.CollType != LE_NOCOLL)
			{
				if (lastCollID.PTPoint.GetTime() == collID.PTPoint.GetTime())// to prevent case when NOCOLL time is equal to last collision time. 
					// in this case NOCOLL will be deleted while sorting timeLine_
				{
					NCollisionID newCollID(collID);
					newCollID.PTPoint.IncrTime(1);
					timeLine_.push_back(newCollID);
				}
				else
					timeLine_.push_back(collID);
			}
		}
		return;
	}
	timeLine_.push_back(collID);
}

const NCollisionID* NCollTimeLine::GetCollID(MainTime intStart, MainTime intEnd)
{
	// returns first NCollisionID in the (intStart, intEnd] interval if any and nullptr otherwise
	if (intStart == MainT_UNDEF || intEnd == MainT_UNDEF)
		return nullptr;
	critical_section::scoped_lock lock(crSect_);
	if (checkedTime_ != 0)
	{// speedup only block
		if (intStart == checkedTime_)
		{
			checkedTime_ = intEnd;
			auto it = checkedPos_;
			if (it == timeLine_.end())
				return nullptr;
			if (it->PTPoint.GetTime() <= intEnd)
			{
				auto t = it + 1;
				for (; t != timeLine_.end(); ++t)
				{
					if (t->PTPoint.GetTime() > intEnd)
						break;
				}
				checkedPos_ = t;
				return &(*it);
			}
			checkedPos_ = it;
			return nullptr;
		}
	}
	// slow block (may be faster)
	checkedTime_ = intEnd;
	checkedPos_ = timeLine_.end();
	for(auto it = timeLine_.begin(); it != timeLine_.end(); ++it)
	{
		if (it->PTPoint.GetTime() > intStart)
		{
			if (it->PTPoint.GetTime() <= intEnd)
			{
				auto t = it + 1;
				for (; t != timeLine_.end(); ++t)
				{
					if (t->PTPoint.GetTime() > intEnd)
						break;
				}
				checkedPos_ = t;
				return &(*it);
			}
			checkedPos_ = it;
			return nullptr;
		}
	}
	
	return nullptr;
}

const NCollisionID* NCollTimeLine::GetCollID(MainTime intStart)
{
	if (intStart > lastTime_)
		return nullptr;
	critical_section::scoped_lock lock(crSect_);
	{// speedup only block
		if (intStart == lastStartTime_)
		{
			if (lastStartPos_ == timeLine_.end())
				return nullptr;
			return &(*lastStartPos_);
		}
	}
	if (intStart > lastStartTime_)
	{
		// slow block (may be faster)
		lastStartTime_ = intStart;
		for (; lastStartPos_ != timeLine_.end(); ++lastStartPos_)
		{
			if (lastStartPos_->PTPoint.GetTime() >= intStart)
				return &(*lastStartPos_);
		}
		return nullptr;
	}
	// slow block (may be faster)
	lastStartTime_ = intStart;
	for (lastStartPos_ = timeLine_.begin(); lastStartPos_ != timeLine_.end(); ++lastStartPos_)
	{
		if (lastStartPos_->PTPoint.GetTime() >= intStart)
			return &(*lastStartPos_);
	}
	return nullptr;

}

std::pair<const NCollisionID*, const NCollisionID*> NCollTimeLine::GetCollIDPair(MainTime intStart) const
{
	if(timeLine_.empty())
		return std::pair<const NCollisionID*, const NCollisionID*>(nullptr, nullptr);
	auto found = std::lower_bound(timeLine_.begin(), timeLine_.end(), NCollisionID(ProgTPoint(cadrID(), intStart), nullptr, nullptr, LE_NOTDEFINED), Lesser);
	if (found == timeLine_.end())
		return std::pair<const NCollisionID*, const NCollisionID*>(&timeLine_.back(), nullptr);
	if (found == timeLine_.begin())
		return std::pair<const NCollisionID*, const NCollisionID*>(nullptr, &timeLine_.front());
	return std::pair<const NCollisionID*, const NCollisionID*>(&(*prev(found)), &(*found));
}

void NCollTimeLine::RemovePointer(NCollidableObject* pPObj)
{
	if (pPObj == nullptr)
		return;
	for (auto it = timeLine_.begin(); it != timeLine_.end(); ++it)
	{
		if (it->IDObj1.GetCollidableObj() == pPObj)
			it->IDObj1.SetCollidableObj(nullptr);
		if (it->IDObj2.GetCollidableObj() == pPObj)
			it->IDObj2.SetCollidableObj(nullptr);
	}
}

void NCollTimeLine::StopCreating()
{
	// filter same elements
	if (!timeLine_.empty())
	{
		std::vector<NCollisionID> buf;
		buf.reserve(timeLine_.size());
		buf.push_back(timeLine_.front());
		for (auto it = next(timeLine_.begin()); it != timeLine_.end(); ++it)
		{
			if (it->IsEquivalent(&buf.back(), (next(it) == timeLine_.end()) ? nullptr : &*next(it)))
				continue;
			buf.push_back(*it);
		}
		timeLine_.resize(buf.size());
		for (int i = 0; i < buf.size(); ++i)
			timeLine_[i] = buf[i];
	}
}

const NCollisionID& NCollTimeLine::GetCollIDByTime(MainTime time) const
{
	static const NCollisionID emptyID(ProgTPoint(), nullptr, nullptr, LE_NOCOLL);
	auto IDPair = GetCollIDPair(time);
	if (IDPair.second != nullptr)
	{
		if (IDPair.second->PTPoint.GetTime() == time)
			return *IDPair.second;
	}
	if (IDPair.first == nullptr)
		return emptyID;
	return *IDPair.first;
}

bool NCollTimeLine::Lesser(const NCollisionID& a0, const NCollisionID& a1)
{
	return a0.PTPoint.GetTime() < a1.PTPoint.GetTime();
}
