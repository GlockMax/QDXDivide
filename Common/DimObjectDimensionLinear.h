#pragma once
#include "DimObjectDimension.h"

class AFX_EXT_CLASS DimObjectDimensionLinear: public DimObjectDimension
{
public:
	DECLARE_SERIAL(DimObjectDimensionLinear);
    DimObjectDimensionLinear(const BPoint& iPoint0, const BPoint& iPoint1, const BPoint& iPoint2, float iWidth = 2.0, char* iPrefix = "", double iKoef = 1.0);
    virtual ~DimObjectDimensionLinear() {}

    void DrawSpecific() const override;

    void Change(const BPoint& iPoint, const BPoint& iViewDir) override;

    double GetDimValue() const override;

    DimObjects::DimObjectsType GetType() const override { return DimObjects::DimObjectDimensionLinearType; }
    
    void Serialize(CArchive& ar) override;

    /// Метод определения совпадения объектов
    bool IsEqualTo(class DimObject* iDimObject) const override;

protected:
	DimObjectDimensionLinear(void) {}

protected:
    double _koef;
};