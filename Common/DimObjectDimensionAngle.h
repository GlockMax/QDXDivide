#pragma once
#include "DimObjectDimension.h"
#include "DimObjectArc.h"

class AFX_EXT_CLASS DimObjectDimensionAngle : public DimObjectDimension
{
public:
	DECLARE_SERIAL(DimObjectDimensionAngle);
    static bool CreateWithCenter(const BPoint& iCenter, const BPoint& iPoint0, const BPoint& iPoint1, DimObject*& oAngle, float iWidth = 2.0);
    
    static bool CreateOnThreePoints(const BPoint& iPoint0, const BPoint& iPoint1, const BPoint& iPoint2, DimObject*& oAngle, float iWidth = 2.0);

    DimObjectDimensionAngle(const DimObjectArc* iArc, float iWidth = 2.0);
	DimObjectDimensionAngle() {};

    virtual ~DimObjectDimensionAngle() {}

    virtual void DrawSpecific() const;

    virtual void Change(const BPoint& iPoint, const BPoint& iViewDir);

    virtual double GetDimValue() const;// { return static_cast<DimObjectArc*>(_dimLine)->GetAngle() * 180.0 / PI; }

    virtual DimObjects::DimObjectsType GetType() const { return DimObjects::DimObjectDimensionAngleType; }

protected:
};