#pragma once
#include "DimObjectDimensionLinear.h"

class AFX_EXT_CLASS DimObjectDimensionCircle: public DimObjectDimensionLinear
{
public:
	DECLARE_SERIAL(DimObjectDimensionCircle);
    DimObjectDimensionCircle(const BPoint& iCenter, const BPoint& iPoint, const BPoint& iNormal, float iWidth = 2.0, bool iDiameter = true);
	DimObjectDimensionCircle() {};
    virtual ~DimObjectDimensionCircle() {}

    virtual void Change(const BPoint& iPoint, const BPoint& iViewDir);

    virtual DimObjects::DimObjectsType GetType() const { return DimObjects::DimObjectDimensionCircleType; }
    
    virtual void Serialize(CArchive& ar);

protected:
    BPoint _normal;
};