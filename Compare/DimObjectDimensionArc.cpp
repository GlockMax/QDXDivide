#include "StdAfx.h"
#include "DimObjectSegment.h"
#include "DimObjectDimensionArc.h"

IMPLEMENT_SERIAL(DimObjectDimensionArc, DimObjectDimensionLinear, 0)

DimObjectDimensionArc::DimObjectDimensionArc(const BPoint& iCenter, const BPoint& iPoint, const BPoint& iNormal, float iWidth, bool iRadius)
        : DimObjectDimensionLinear(iCenter, iPoint, iPoint, iWidth, iRadius ? "R" : "D", iRadius ? 1.0 : 2.0), _normal(iNormal)
{
    SetCursorType(DimData::Empty, DimData::ClosedFilled);
}

void DimObjectDimensionArc::Change(const BPoint& iPoint, const BPoint& iViewDir)
{
    BPoint endPts[2];
    _dimLine->GetEndPoints(endPts[0], endPts[1]);

    BPoint pt;
    AvailableTrace result = GrazingCurveElemContour::LinePlane(iPoint, iViewDir, endPts[0], _normal, pt);
    if (result != ONE_TRACE)
        return;

    BPoint newDir = pt - endPts[0];
    newDir.Unit();
    
    double val = sqrt((endPts[0] - endPts[1]).D2());
    static_cast<DimObjectSegment*>(_dimLine)->SetEndPoint(endPts[0] + newDir * val);

    //_oldPt = iPoint;
}

void DimObjectDimensionArc::Serialize(CArchive& ar)
{
    DimObjectDimensionLinear::Serialize(ar);
    SerializeElements(ar, &_normal, 1);
}