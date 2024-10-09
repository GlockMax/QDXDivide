#include "StdAfx.h"
#include "ConstDef.h"
#include "DimObjectSegment.h"
#include "DimObjectDimensionCircle.h"

IMPLEMENT_SERIAL(DimObjectDimensionCircle, DimObjectDimensionLinear, 0)

DimObjectDimensionCircle::DimObjectDimensionCircle(const BPoint& iCenter, const BPoint& iPoint, const BPoint& iNormal, float iWidth, bool iDiameter)
        : DimObjectDimensionLinear(iCenter + iCenter - iPoint, iPoint, iPoint, iWidth, iDiameter ? "D" : "R", iDiameter ? 1.0 : 0.5), _normal(iNormal)
{
    SetCursorType(DimData::ClosedFilled, DimData::ClosedFilled);
}

void DimObjectDimensionCircle::Change(const BPoint& iPoint, const BPoint& iViewDir)
{
    BPoint center = _dimLine->GetMidPoint();
    BPoint pt;
    AvailableTrace result = GrazingCurveElemContour::LinePlane(iPoint, iViewDir, center, _normal, pt);
    if (result != ONE_TRACE)
        return;

    BPoint newDir = pt - center;
    double magn2 = newDir.D2();
    if (magn2 < MIND * MIND)
        return;

	const BPoint &endPt = static_cast<DimObjectSegment*>(_dimLine)->GetEndPoint();
	double r = sqrt((center - endPt).D2());
	newDir *= r / sqrt(magn2);
    static_cast<DimObjectSegment*>(_dimLine)->SetEndPoint(center + newDir);
    static_cast<DimObjectSegment*>(_dimLine)->SetStartPoint(center - newDir);

    _oldPt = iPoint;
}

void DimObjectDimensionCircle::Serialize(CArchive& ar)
{
    DimObjectDimensionLinear::Serialize(ar);
    SerializeElements(ar, &_normal, 1);
}