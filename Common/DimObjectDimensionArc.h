#pragma once
#include "DimObjectDimensionLinear.h"

class AFX_EXT_CLASS DimObjectDimensionArc: public DimObjectDimensionLinear
{
public:
	DECLARE_SERIAL(DimObjectDimensionArc);
    DimObjectDimensionArc(const BPoint& iCenter, const BPoint& iPoint, const BPoint& iNormal, float iWidth = 2.0, bool iRadius = true);
	DimObjectDimensionArc() {};
    virtual ~DimObjectDimensionArc() {}

    virtual void Change(const BPoint& iPoint, const BPoint& iViewDir);

    virtual DimObjects::DimObjectsType GetType() const { return DimObjects::DimObjectDimensionArcType; }
    
    virtual void Serialize(CArchive& ar);

protected:
    BPoint _normal;
};