#include "StdAfx.h"
#include "ConstDef.h"
#include "DimObjectSegment.h"
#include "NSurfSettings.h"
#include "curve.h"

IMPLEMENT_SERIAL(DimObjectSegment, DimObject, 0)

DimObjectSegment::DimObjectSegment(const BPoint &iPoint0, const BPoint &iPoint1, float iWidth): DimObject(iWidth)
{
    _pts[0] = iPoint0;
    _pts[1] = iPoint1;
}

void DimObjectSegment::DrawSpecific() const
{
    // аттрибуты
    glPushAttrib(GL_LINE_BIT);
    glLineWidth(_widthMain);
    
    DrawGLPrimitive(_pts, 2);
    
    glPopAttrib();
}

BPoint DimObjectSegment::FindNearestPoint(const BPoint& iPoint) const
{
    // спроецировать точку на прямую
    BPoint proj = GrazingCurveElemContour::PointProjLine(_pts[0], _pts[1], iPoint);

    // проверить принадлежность отрезку
    BPoint vect[2] = {proj - _pts[0], proj - _pts[1]};
    double cos = vect[0] * vect[1];
    if (cos < 0.0)
        return proj;
    else if (vect[0].D2() < vect[1].D2())
        return _pts[0];
    else
        return _pts[1];
}

bool DimObjectSegment::FindNearestPoint(const BPoint& iPoint, const BPoint& iViewDir, BPoint& oPoint) const
{
    BPoint dirs[2] = {iViewDir, _pts[1] - _pts[0]};
    BPoint pt = iPoint;
    if ((dirs[0] % dirs[1]).D2() > MIND * MIND)
        pt = GrazingCurveElemContour::TraceOfLines(iPoint, iPoint + iViewDir, _pts[0], _pts[1]);
    
    oPoint = FindNearestPoint(pt);

    return true;
}

bool DimObjectSegment::Contains(const BPoint& iStartSegm, const BPoint& iEndSegm, const BPoint& iPoint)
{
    BPoint pt = GrazingCurveElemContour::PointProjLine(iStartSegm, iEndSegm, iPoint);
    if (fabs((pt - iPoint).D2()) > MIND * MIND)
        return false;

    BPoint vect[2] = {(iPoint - iStartSegm), (iPoint - iEndSegm)};
    
    // проверить принадлежность точки концам отрезка
    double magn2[2] = {vect[0].D2(), vect[1].D2()};
    if (magn2[0] < MIND * MIND || magn2[1] < MIND * MIND)
        return true;

    double cos = vect[0] * vect[1];
    return cos < 0.0;
}

BBox DimObjectSegment::GetGabar() const
{
    BBox box;
    box.Expand(_pts[0]);
    box.Expand(_pts[1]);
    return box;
}

void DimObjectSegment::Serialize(CArchive& ar)
{
    DimObject::Serialize(ar);
    SerializeElements(ar, &_pts[0], 2);
}

bool DimObjectSegment::IsEqualTo(class DimObject* iDimObject) const
{
    if (iDimObject->GetType() != GetType())
        return false;

    DimObjectSegment* segm = static_cast<DimObjectSegment*>(iDimObject);

	for(int i = 0; i < 2; ++i)
	{
		double magn2 = (segm->_pts[i] - _pts[i]).D2();
		if (magn2 > MIND * MIND)
			return false;
	}

    return true;
}

