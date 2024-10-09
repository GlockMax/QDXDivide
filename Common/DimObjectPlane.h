#pragma once
#include "COMPARE_API.h"
#include "DimObject.h"
#include "BPoint.h"

class AFX_EXT_CLASS DimObjectPlane: public DimObject
{
public:
	DECLARE_SERIAL(DimObjectPlane);
    static bool CreateOnThreePoints(const BPoint& iPoint0, const BPoint& iPoint1, const BPoint& iPoint2, DimObject*& oPlane, float iWidth = 2.0);

    static void CreateOnPointAndNormal(const BPoint& iOrigin, const BPoint& iNromal, DimObject*& oPlane, double iSideValue = 100.0, float iWidth = 2.0);
    
    DimObjectPlane(const BPoint& iOrigin, BPoint iDirections[2], double iKoef = 1.0, float iWidth = 2.0);
	DimObjectPlane(void) {};
    virtual ~DimObjectPlane() {}

    BPoint GetOrigin() const { return _origin; }
    BPoint GetNormal() const { return (_dirs[0] % _dirs[1]).Unit(); }

    virtual BPoint GetMidPoint() const { return GetOrigin(); }
    virtual void GetEndPoints(BPoint& oPoint0, BPoint& oPoint1) const { oPoint0 = oPoint1 = GetOrigin(); }

    virtual void DrawSpecific() const;

	virtual BPoint FindNearestPoint(const BPoint& iPoint) const { return BPoint::ProjectPointOnPlane(iPoint, _origin, _dirs[0] % _dirs[1]); }
    virtual bool FindNearestPoint(const BPoint& iPoint, const BPoint& iViewDir, BPoint& oPoint) const { return GrazingCurveElemContour::LinePlane(iPoint, iViewDir, GetOrigin(), GetNormal(), oPoint) == ONE_TRACE; }

    virtual BBox GetGabar() const;

    virtual DimObjects::DimObjectsType GetType() const { return DimObjects::DimObjectPlaneType; }

    virtual bool IsEqualTo(class DimObject* iDimObject) const;
    
    virtual void Serialize(CArchive& ar);

protected:

    BPoint _origin;
    BPoint _dirs[2];
    double _koef; // вспомогательный коэффициент для масштабирования
};