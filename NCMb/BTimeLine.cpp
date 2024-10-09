#include "stdafx.h"
#include "NCadrGeom.h"
#include "BTimeLine.h"

void BTimeLine::AddState(const NCadrGeom& Geom)
{
	StateType Last = back().second;
	Last.ApplyChanges(Geom.GetMTLinkChange());
	push_back(TimePair(Geom.GetEndTime(), Last));
}

const StateType& BTimeLine::GetByInd(int Ind) const
{
	return at(Ind).second;
}

void BTimeLine::MakeFromGeom(const std::vector<NCadrGeomArray*>& geomArrays)
{
	clear();
	const auto count = geomArrays.size();
	if (count == 0)
		return;
	std::vector<size_t> inds(count, 0);
	size_t sumSize = 0;
	for each (const auto el in geomArrays)
		sumSize += el->GetSize();
	reserve(sumSize);
	BMTLinkage curLink;
	push_back(TimePair(0, curLink));
	bool stop = false;
	while (!stop)
	{
		bool needNew = false;
		for (int i = 0; i < count; ++i)
		{
			auto ind = inds.at(i);
			if (ind < size_t(geomArrays.at(i)->GetSize()))
				if (geomArrays.at(i)->GetAt(inds.at(i))->GetEndTime() == back().first)
				{
					back().second.ApplyChanges(geomArrays.at(i)->GetAt(inds.at(i))->GetMTLinkChange());
					++inds.at(i);
				}
		}
		MainTime minTime = MainT_UNDEF;
		for (int i = 0; i < count; ++i)
		{
			for(auto ind = inds.at(i); ind < size_t(geomArrays.at(i)->GetSize()); ind = ++inds.at(i))
			{
				if (!geomArrays.at(i)->GetAt(ind)->GetMTLinkChange().IsZero())
				{
					if (geomArrays.at(i)->GetAt(ind)->GetEndTime() < minTime)
					{
						minTime = geomArrays.at(i)->GetAt(ind)->GetEndTime();
						break;
					}
				}
			}
		}
		if (minTime == MainT_UNDEF)
			stop = true;
		else 
			push_back(TimePair(minTime, back().second));
		for each (NCadrGeomArray* el in geomArrays)
			SetIndexesTo(el);
	}
	shrink_to_fit();
}

void BTimeLine::SetIndexesTo(NCadrGeomArray* pGeomArray) const
{
	if (pGeomArray == nullptr)
		return;
	size_t thisInd = 1;
	for (size_t i = 0; i < size_t(pGeomArray->GetSize()); ++i)
	{
		auto& curGeom = *pGeomArray->GetAt(i);
		for (; thisInd < size(); ++thisInd)
		{
			const auto& el = at(thisInd);
			if (el.first > curGeom.GetStartTime())
			{
				curGeom.SetTimeInd(unsigned int(thisInd - 1));
				break;
			}
		}
		if(thisInd == size())
			curGeom.SetTimeInd(unsigned int(thisInd - 1));
	}
}

const BMTLinkage& BTimeLine::GetLinkage(const NCadrGeom& Geom, double t) const
{
	// t value is between 0 and 1
	auto startInd = Geom.GetTimeInd();
	if (t == 0.)
		return at(startInd).second;
	const MainTime curT = MainTime(Geom.GetStartTime() * (1 - t) + Geom.GetEndTime() * t);
	for(++startInd; startInd < size(); ++startInd)
		if(curT < at(startInd).first)
			return at(startInd - 1).second;
	return back().second;
}
