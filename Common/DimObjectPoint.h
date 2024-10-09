#pragma once
#include "COMPARE_API.h"
#include "DimObject.h"

class AFX_EXT_CLASS DimObjectPoint : 	public DimObject
{
public:
	DECLARE_SERIAL(DimObjectPoint);
    DimObjectPoint(const BPoint &Point, float iRadius = 2.0): DimObject(iRadius), P(Point) {}
    DimObjectPoint(void) {}
    virtual ~DimObjectPoint(void) {}

	virtual void DrawSpecific() const override;

    const BPoint& GetBPoint() const { return P; }

    virtual BPoint FindNearestPoint(const BPoint& iPoint) const { return P; }
    virtual bool FindNearestPoint(const BPoint& iPoint, const BPoint& iViewDir, BPoint& oPoint) const { oPoint = P; return true; }

    virtual bool IsEqualTo(class DimObject* iDimObject) const { return dynamic_cast<DimObjectPoint*>(iDimObject) ? Contains(static_cast<DimObjectPoint*>(iDimObject)->P) : false; }
    
    virtual bool Contains(const BPoint& iPoint) const ;

    virtual BPoint GetMidPoint() const { return GetBPoint(); }
    virtual void GetEndPoints(BPoint& oPoint0, BPoint& oPoint1) const { oPoint0 = oPoint1 = GetBPoint(); }

    virtual BBox GetGabar() const;

    virtual DimObjects::DimObjectsType GetType() const { return DimObjects::DimObjectPointType; }

    virtual void Serialize(CArchive& ar);
	virtual bool IsDummy(void) const { return !P.IsPoint();}

protected:
    
	BPoint P;
};

