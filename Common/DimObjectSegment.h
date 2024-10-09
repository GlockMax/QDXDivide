#pragma once
#include "COMPARE_API.h"
#include "DimObject.h"
#include "BPoint.h"

class AFX_EXT_CLASS DimObjectSegment : public DimObject
{
public:
	DECLARE_SERIAL(DimObjectSegment);
    DimObjectSegment(const BPoint &iPoint0, const BPoint &iPoint1, float iWidth = 2.0f);
	DimObjectSegment(void) {};
    virtual ~DimObjectSegment() {}

    virtual void DrawSpecific() const;
    
    virtual BPoint GetMidPoint() const { return (_pts[0] + _pts[1]) * 0.5; }
    virtual void GetEndPoints(BPoint& oPoint0, BPoint& oPoint1) const { oPoint0 = _pts[0], oPoint1 = _pts[1]; }

    const BPoint& GetStartPoint() const { return _pts[0]; }

    const BPoint& GetEndPoint() const { return _pts[1]; }

    void SetStartPoint(const BPoint& iPoint) { _pts[0] = iPoint; }
    void SetEndPoint(const BPoint& iPoint) { _pts[1] = iPoint; }

    virtual BPoint FindNearestPoint(const BPoint& iPoint) const;
    virtual bool FindNearestPoint(const BPoint& iPoint, const BPoint& iViewDir, BPoint& oPoint) const;

    virtual bool Contains(const BPoint& iPoint) const { return Contains(_pts[0], _pts[1], iPoint); }
    static bool Contains(const BPoint& iStartSegm, const BPoint& iEndSegm, const BPoint& iPoint);

    virtual BBox GetGabar() const;

    virtual DimObjects::DimObjectsType GetType() const { return DimObjects::DimObjectSegmentType; }

    virtual void Serialize(CArchive& ar);
    virtual bool IsEqualTo(class DimObject* iDimObject) const;

protected:
    
    BPoint _pts[2];
};