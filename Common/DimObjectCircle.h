#pragma once
#include "DimObjectArc.h"

class AFX_EXT_CLASS DimObjectCircle: public DimObjectArc
{
public:
 	DECLARE_SERIAL(DimObjectCircle);
   static bool CreateOnThreePoints(const BPoint& iPoint0, const BPoint& iPoint1, const BPoint& iPoint2, DimObject*& oCircle, double iCenterRadius = 2.0, float iCenterWidth = 2.0f, float iWidth = 2.0);
    
    DimObjectCircle(class DimObjectArc* iArc): DimObjectArc(*iArc) {}
    DimObjectCircle() {}
    virtual ~DimObjectCircle() {}

    virtual bool Contains(const BPoint& iPoint) const { return IsOnCircle(iPoint); }

    virtual BPoint GetMidPoint() const { return GetCenter() + _arc.GetI().Unit() * GetRadius(); }
    virtual void GetEndPoints(BPoint& oPoint0, BPoint& oPoint1) const { oPoint0 = oPoint1 = GetStartPoint(); }

    virtual DimObjects::DimObjectsType GetType() const { return DimObjects::DimObjectCircleType; }

protected:
    virtual bool IsEqualToInternal(class DimObjectArc* iArc) const;
};