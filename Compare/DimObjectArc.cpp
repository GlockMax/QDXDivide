#include "StdAfx.h"
#include <cmath>
#include "ConstDef.h"
#include <cmath>
#include "DimObjectArc.h"
#include "DimObjectCircle.h"
#include "NSurfSettings.h"
#include "BaseRender.h"
#include "NCEnums.h"
#include "GrazingCurveElemContour.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(DimObjectArc, DimObject, 0)

DimObjectArc::DimObjectArc(const ArcData& iData, double iCenterRadius, float iCenterWidth, float iWidth)
    : DimObject(iWidth), _centerRadius(iCenterRadius), _centerWidth(iCenterWidth)
{
    // создать дугу
    _arc.Set(iData._curve, iData._pts[0], iData._pts[1]);
    _arc.SetPl(P_UNDEF);
    if (!iData._isLine)
    {
        BPoint vect = iData._center - iData._pts[0];
        _arc.SetI(vect);
        _arc.SetN(iData._normal);
    }
}

DimObjectArc::DimObjectArc(double iCenterRadius, float iCenterWidth, float iWidth)
    : DimObject(iWidth), _centerRadius(iCenterRadius), _centerWidth(iCenterWidth)
{
}

bool DimObjectArc::CreateOnThreePoints(const BPoint& iPoint0, const BPoint& iPoint1, const BPoint& iPoint2, DimObject*& oArc, double iCenterRadius, float iCenterWidth, float iWidth)
{
    ArcData data(iPoint0, iPoint1, iPoint2);

    if (data._isLine)
        return false;

    oArc = new DimObjectArc(data, iCenterRadius, iCenterWidth, iWidth);

    return true;
}

bool DimObjectArc::CreateOnThreePoints(const BPoint& iPoint0, const BPoint& iPoint1, const BPoint& iPoint2, double iAngle, DimObject*& oArc, double iCenterRadius, float iCenterWidth, float iWidth)
{
    ArcData data(iPoint0, iPoint1, iPoint2, iAngle);
    
    if (data._isLine)
        return false;
    
    if (data._isCircle)
    {
        DimObjectArc* arc = new DimObjectArc(data, iCenterRadius, iCenterWidth, iWidth);
        arc->MakeClosed();
        oArc = new DimObjectCircle(arc);
        delete arc;
    }
    else
        oArc = new DimObjectArc(data, iCenterRadius, iCenterWidth, iWidth);

    return true;
}

bool DimObjectArc::Create(const BPoint& iCenter, const BPoint& iPoint0, const BPoint& iPoint2, DimObject*& oArc, bool iIsLessThanPI, double iCenterRadius, float iCenterWidth, float iWidth)
{
    ArcData data(iCenter, iPoint0, iPoint2, iIsLessThanPI);

    if (data._isLine)
        return false;

    oArc = new DimObjectArc(data, iCenterRadius, iCenterWidth, iWidth);

    return true;
}

DimObjectArc::ArcData::ArcData(const BPoint& iPoint0, const BPoint& iPoint1, const BPoint& iPoint2): _isLine(false), _isCircle(false)
{
    // взять точки в 3D
    _pts[0] = iPoint0;
    _pts[1] = iPoint2;
    BPoint internalPt = iPoint1;
    internalPt.Norm();

    // найти середины отрезков
    BPoint mid[2] = {(_pts[0] + internalPt) * 0.5,
                     (internalPt + _pts[1]) * 0.5};

    // проверка случая, когда точки лежат на одной прямой или совпадают
    _isLine = DimObject::IsOnLine(_pts[0], internalPt, _pts[1]);
    if (_isLine)
        _curve = line;
    else
    {
        // найти направления перпендикуляров
        BPoint vect[2] = {_pts[0] - internalPt, _pts[1] - internalPt};
        BPoint normal = (vect[0] % vect[1]).Unit();
        BPoint perp[2] = {(vect[0] % normal).Unit(), (vect[1] % normal).Unit()};

        // пересечь прямые - найти центр
        BPoint center = GrazingCurveElemContour::TraceOfLines(mid[0], mid[0] + perp[0], mid[1], mid[1] + perp[1]);

        // направление
        BPoint vects[3] = {(_pts[0] - center), (_pts[1] - center), (internalPt - center)};
        BPoint normals[3] = {vects[0] % vects[1], vects[0] % vects[2], vects[2] % vects[1]};
        double coses[2] = {normals[0] * normals[1], normals[1] * normals[2]};
        NCadrGeom::Curve curve;
	    if (coses[0] > 0.0 && coses[1] > 0.0)
            curve.type = cwarc;
        else
            curve.type = ccwarc;

        _center = center;
        _curve = curve;
        if (normals[0].D2() < MIND)
            _normal = normals[1].Unit();
        else
            _normal = normals[0].Unit() * (-1.0); // это может быть костыль, а может не быть
    }
}

DimObjectArc::ArcData::ArcData(const BPoint& iPoint0, const BPoint& iPoint1, const BPoint& iPoint2, double iAngle): _isLine(false), _isCircle(false)
{
    if (fabs(iAngle) < MIND)
    {
        _pts[0] = _pts[1] = iPoint0;
        _curve = line;
        _isLine = true;
        return;
    }
    
    ArcData data(iPoint0, iPoint1, iPoint2);
    _center = data._center;
    _curve = data._curve;
    _normal = data._normal;
    _pts[0] = data._pts[0];
    _pts[1] = data._pts[1];
    _isLine = data._isLine;

    if (_isLine)
        return;

    int k = (int)(iAngle / (2.0 * PI));
    double angle = fabs(iAngle) - 2.0 * PI * fabs((double)k);
    if (angle < MIND)
        _isCircle = true;
    else
    {
        BPoint vect = iPoint1 - _center;
        double magn2 = vect.D2();
        if (magn2 < MIND * MIND)
            return;

        double r = sqrt(magn2);
        vect = vect * (1.0 / r);
        
        BPoint dir = vect % _normal;
        static const double PI_2 = PI * 0.5;
        if (fabs(PI - fabs(angle)) < MIND)
        {
            _pts[0] = _center + dir * r;
            _pts[1] = _center - dir * r;
        }
        else 
        {
            double sign = (PI - fabs(angle)) < 0.0 ? -1.0 : 1.0;

            double magn = r * std::tan(angle * 0.5);
            _pts[0] = _center + ((iPoint1 + dir * magn) - _center).Unit() * r * sign;
            _pts[1] = _center + ((iPoint1 - dir * magn) - _center).Unit() * r * sign;

            if (_curve.type == cwarc)
                _normal *= -1.0;
        }
    }
}

DimObjectArc::ArcData::ArcData(const BPoint& iCenter, const BPoint& iPoint0, const BPoint& iPoint2, bool iIsLessThanPI): _isLine(false), _isCircle(false)
{
    if (!DimObject::IsOnLine(iCenter, iPoint0, iPoint2))
    {
        // направление
        BPoint normal = (iPoint0 - iCenter) % (iPoint2 - iCenter);

        _center = iCenter;
        NCadrGeom::Curve curve;
        curve.type = cwarc;
        _curve = curve;
        double sign = iIsLessThanPI ? -1.0 : 1.0;
        _normal = normal.Unit() * sign;
    }
    else
        _isLine = true;

    _pts[0] = iPoint0;
    _pts[1] = iPoint2;
}

BPoint DimObjectArc::GetMidPoint() const
{
    BPoint pts[2], center = GetCenter();
    GetEndPoints(pts[0], pts[1]);

    BPoint vects[2] = {pts[0] - center, pts[1] - center},
           dir = vects[0] + vects[1];
    double magn2 = dir.D2();
    if (magn2 < MIND * MIND)
    {// дуга окружности в PI
        dir = vects[0] % GetNormal();
        if (_arc.GetType().type == ccwarc)
            dir *= -1.0;
        dir.Unit();
    }
    else
    {
        dir = dir * (1.0 / sqrt(magn2));

        BPoint normals[2] = {vects[0] % vects[1], GetNormal()};
        double cos = normals[0] * normals[1];
        if (cos > 0.0)
        {
            if (_arc.GetType().type == cwarc)
                dir *= -1.0;
        }
        else
            if (_arc.GetType().type == ccwarc)
                dir *= -1.0;
    }

    return center + dir * GetRadius();
}

void DimObjectArc::DrawSpecific() const
{
    // аттрибуты
    glPushAttrib(GL_LINE_BIT);
    glLineWidth(_widthMain);
    _arc.Draw(NSurfSettings::GetTolerance() * 0.1, 1.0);
    glPopAttrib();

    if (Highlighted)
        if (!_arc.IsLine())
        {
            // создать с ним окружность
            double r = _centerRadius * DimObjects::GetScaleKoef();

            BPoint centerVect = _arc.GetI();
            centerVect.Unit();
            centerVect *= r;

            BPoint center = _arc.GetC();
            BPoint pt = center - centerVect;

            NCadrGeom centerArc;
            centerArc.Set(_arc.GetType(), pt, pt);
            centerArc.SetI(centerVect);
            centerArc.SetN(GetViewDir());

            // нарисовать эту окружность
            glPushAttrib(GL_LINE_BIT);
            glLineWidth(_centerWidth);
            centerArc.Draw(NSurfSettings::GetTolerance(), 1.0);
            glPopAttrib();
        }
}

double DimObjectArc::GetAngle() const
{
    BPoint center = GetCenter(),
           start = GetStartPoint(),
           end = GetEndPoint();
    BPoint vect[2] = {start - center, end - center};
    vect[0].Unit();
    vect[1].Unit();
    
    return IsLessThanPI() ? acos(vect[0] * vect[1]) : 2.0 * PI - acos(vect[0] * vect[1]);
}

BPoint DimObjectArc::FindNearestPoint(const BPoint& iPoint) const
{
    // спроецировать точку на плоскость
    BPoint center = GetCenter(),
		proj = BPoint::ProjectPointOnPlane(iPoint, center, GetNormal());
    
    // направление
    BPoint dir = proj - center;
    dir.Unit();

    // пересечение направления с дугой и окружностью в центре
    BPoint crossPt[2] = {center + dir * GetRadius(), center + dir * _centerRadius * DimObjects::GetScaleKoef()};

    if ((crossPt[0] - proj).D2() < (crossPt[1] - proj).D2())
        if (IsOnArc(crossPt[0]))
            return crossPt[0];
        else
        {
            double deltas[2] = {(GetStartPoint() - crossPt[0]).D2(), (GetEndPoint() - crossPt[0]).D2()};
            return deltas[0] < MIND * MIND ? GetStartPoint() : GetEndPoint();
        }
    else
        return center;
}

bool DimObjectArc::FindNearestPoint(const BPoint& iPoint, const BPoint& iViewDir, BPoint& oPoint) const
{
    // пересечь прямую и плоскость
    BPoint proj;
    AvailableTrace result = GrazingCurveElemContour::LinePlane(iPoint, iViewDir, GetCenter(), GetNormal(), proj);
    if (result != ONE_TRACE)
        return false;

    // направление
    BPoint center = GetCenter(),
           dir = proj - center;
    dir.Unit();

    // пересечение направления с дугой и окружностью в центре
    BPoint crossPt[2] = {center + dir * GetRadius(), center + dir * _centerRadius * DimObjects::GetScaleKoef()};
    if ((crossPt[0] - proj).D2() < (crossPt[1] - proj).D2())
        if (Contains(crossPt[0]))
            oPoint = crossPt[0];
        else
        {
            double deltas[2] = {(GetStartPoint() - crossPt[0]).D2(), (GetEndPoint() - crossPt[0]).D2()};
            oPoint = deltas[0] < MIND * MIND ? GetStartPoint() : GetEndPoint();
        }
    else
        oPoint = center;

    return true;
}

bool DimObjectArc::Contains(const BPoint& iPoint) const
{
    // проверить принадлежность точки окружности
    bool result = IsOnCircle(iPoint);
    if (!result)
        return false;

    // проверить принадлежность точки концам
    double deltas[2] = {(GetStartPoint() - iPoint).D2(), (GetEndPoint() - iPoint).D2()};
    if (deltas[0] < MIND * MIND || deltas[1] < MIND * MIND)
        return true;
    // проверить принадлежность точки дуге
    return IsOnArc(iPoint);
}

bool DimObjectArc::IsOnCircle(const BPoint& iPoint) const
{
	BPoint pt = BPoint::ProjectPointOnPlane(iPoint, GetCenter(), GetNormal());
    if (fabs((pt - iPoint).D2()) > MIND * MIND && fabs((pt - GetCenter()).D2()) > MIND * MIND)
        return false;

    double delta = fabs(sqrt((iPoint - GetCenter()).D2()) - GetRadius());
    return delta < MIND;
}

bool DimObjectArc::IsOnArc(const BPoint& iPoint) const
{
    //// проверить принадлежность точки дуге
    //BPoint vects[3] = {GetStartPoint() - GetCenter(),
    //                   iPoint - GetCenter(),
    //                   GetEndPoint() - GetCenter()};
    //BPoint normals[3] = {vects[0] % vects[1], vects[1] % vects[2], vects[0] % vects[2]};

    //bool isCwarc = _arc.GetType().type == cwarc;
    //bool isCodirect = normals[0] * normals[1] > 0.0 && normals[0] * normals[2] > 0.0;

    //if (isCwarc == isCodirect)
    //    return true;
    //else 
    //    return false;
	//double S, E, C;
	//_arc.GetArcA(&S, &E, &C);
 //   BPoint vects[3] = {GetStartPoint() - GetCenter(), iPoint - GetCenter(), GetEndPoint() - GetCenter()};
	//double A = vects[1].Angle(vects[0], GetNormal()) + 180.;
	//if(A * C < 0.)
	//	return false;
	//if(fabs(A) < fabs(C))
	//	return true;
	//return false;
	return true;
}

BBox DimObjectArc::GetGabar() const
{
    BBox box;
	_arc.ExpandBox(&box);
    return box;
}

bool DimObjectArc::IsEqualTo(class DimObject* iDimObject) const
{
    if (iDimObject->GetType() != GetType())
        return false;

    DimObjectArc* arc = static_cast<DimObjectArc*>(iDimObject);
    if (arc->_arc.GetCurve() != _arc.GetCurve())
        return false;

    double magn2 = (arc->GetCenter() - GetCenter()).D2();
    if (magn2 > MIND * MIND)
        return false;

    return IsEqualToInternal(arc);
}

bool DimObjectArc::IsEqualToInternal(class DimObjectArc* iArc) const
{
    double delta2[2] = {(iArc->_arc.GetB() - _arc.GetB()).D2(), (iArc->_arc.GetE() - _arc.GetE()).D2()};
    if ((delta2[0] < MIND * MIND) && (delta2[1] < MIND * MIND))
    {
        double cos = iArc->GetNormal() * GetNormal();
        bool flag[2] = {cos > 0.0, iArc->_arc.GetType().type == _arc.GetType().type};
        if (flag[0] == flag[1])
            return true;
        else
            return false;
    }
    else
    {
        delta2[0] = (iArc->_arc.GetB() - _arc.GetE()).D2();
        delta2[1] = (iArc->_arc.GetE() - _arc.GetB()).D2();

        if ((delta2[0] < MIND * MIND) && (delta2[1] < MIND * MIND))
        {
            double cos = iArc->GetNormal() * GetNormal();
            bool flag[2] = {cos > 0.0, iArc->_arc.GetType().type == _arc.GetType().type};
            if (flag[0] != flag[1])
                return true;
            else
                return false;
        }
        else
            return false;
    }
}

bool DimObjectArc::IsLessThanPI() const
{
    BPoint center = GetCenter(),
           pts[2];
    GetEndPoints(pts[0], pts[1]);

    BPoint normals[2] = {GetNormal(), ((pts[0] - center) % (pts[1] - center)).Unit()};
    double cos = normals[0] * normals[1];
    bool colinear = cos >= 0.0;
    return colinear == (_arc.GetType().type == ccwarc);
}

void DimObjectArc::Serialize(CArchive& ar)
{
    DimObject::Serialize(ar);
    _arc.Serialize(ar);
    SerializeElements(ar, &_centerRadius, 1);
    SerializeElements(ar, &_centerWidth, 1);
}