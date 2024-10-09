#pragma once
#include "NCMB_API.h"
#include <vector>
#include "BGeomArray.h"
#include "BMTLinkage.h"

typedef UINT64 MainTime;
constexpr MainTime MainT_UNDEF = UINT64_MAX;

typedef BMTLinkage StateType;
typedef std::pair<MainTime, StateType> TimePair;

class NCMB_API BTimeLine : public std::vector<TimePair>
{
public:
	void AddState(const class NCadrGeom& geom);
	const StateType& GetByInd(int Ind) const;
	void MakeFromGeom(const std::vector<NCadrGeomArray*>& geomArrays);
	void SetIndexesTo(NCadrGeomArray* pGeomArray) const;
	const BMTLinkage& GetLinkage(const NCadrGeom& Geom, double t) const;
};

