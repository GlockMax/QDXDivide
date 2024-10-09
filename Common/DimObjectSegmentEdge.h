#pragma once
#include "DimObjectSegment.h"
#include "DimObjectStock.h"
// DimObjectSegmentEdge command target

class DimObjectSegmentEdge : public DimObjectSegment
{
public:
	DimObjectSegmentEdge(const BPoint &iPoint0, const BPoint &iPoint1, const BPoint &iCoordMap, float iWidth = 2.0f) : DimObjectSegment(iPoint0, iPoint1, iWidth), CoordMap(iCoordMap), ParentID(0) {}
	DimObjectSegmentEdge() {ParentID = 0;}
	virtual ~DimObjectSegmentEdge();
    virtual DimObjects::DimObjectsType GetType() const { return DimObjects::DimObjectSegmentEdgeType; }
	void SetParent(unsigned int iParentID) { ParentID = iParentID;}
	virtual void Serialize(CArchive& ar) {};
	unsigned int GetParentID() const { return ParentID;}
	const BPoint & GetCoordMap(void) const { return CoordMap;}


    /// Метод редактирует объект
    virtual void Change(const BPoint& iPoint, const BPoint& iViewDir) override;
protected:
	unsigned int ParentID;
	BPoint CoordMap; // 0 - this coordinate don't change any point, 1 - this coordinate changes PointMin, 1 - this coordinate changes PointMax

};


