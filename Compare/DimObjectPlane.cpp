#include "StdAfx.h"
#include "ConstDef.h"
#include "DimObjectPlane.h"
#include "BaseRender.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(DimObjectPlane, DimObject, 0)

bool DimObjectPlane::CreateOnThreePoints(const BPoint& iPoint0, const BPoint& iPoint1, const BPoint& iPoint2, DimObject*& oPlane, float iWidth)
{
    BPoint pts[3] = {iPoint0, iPoint1, iPoint2};

    bool isOnLine = DimObject::IsOnLine(iPoint0, iPoint1, iPoint2);
    if (isOnLine)
        return false;

    // определить нормаль
    BPoint vect[2] = {pts[1] - pts[0], pts[2] - pts[0]};
    BPoint normal = vect[0] % vect[1];
    normal.Unit();
    
    // определить направления и origin
    double magn = sqrt(vect[0].D2());
    BPoint tmpDirs[2] = {vect[0].Unit() * magn, normal % vect[0] * magn};

    BPoint directions[2] = {(tmpDirs[0] + tmpDirs[1]) * (-0.5), (tmpDirs[0] - tmpDirs[1]) * (-0.5)},
           origin = pts[0] - directions[0];

    oPlane = new DimObjectPlane(origin, directions, 1.0 / DimObjects::GetScaleKoef(), iWidth);

    return true;
}

void DimObjectPlane::CreateOnPointAndNormal(const BPoint& iOrigin, const BPoint& iNormal, DimObject*& oPlane, double iSideValue, float iWidth)
{
    double abs[3] = {fabs(iNormal.GetX()), fabs(iNormal.GetY()), fabs(iNormal.GetZ())};
    int minCoord = abs[0] < abs[1] ? (abs[0] < abs[2] ? 0 : 2) : (abs[1] < abs[2] ? 1 : 2);
    BPoint vect(0.0, 0.0, 0.0, 0.0);
    vect[minCoord] = 1.0;

    BPoint xDir = (iNormal % vect).Unit(),
           yDir = (xDir % iNormal).Unit();
    
    static const double root2Inv = 1.0 / sqrt(2.0);
    double magn = iSideValue * root2Inv/* * DimObjects::GetScaleKoef()*/; // задаём в мм

    BPoint dirs[2] = {(xDir + yDir) * root2Inv * magn, (xDir - yDir) * root2Inv * magn};

    oPlane = new DimObjectPlane(iOrigin, dirs, 1.0 / DimObjects::GetScaleKoef(), iWidth);
}

DimObjectPlane::DimObjectPlane(const BPoint& iOrigin, BPoint iDirections[2], double iKoef, float iWidth)
    : DimObject(iWidth), _origin(iOrigin), _koef(iKoef)
{
    _dirs[0] = iDirections[0];
    _dirs[1] = iDirections[1];
}

void DimObjectPlane::DrawSpecific() const
{
    //double koef = DimObjects::GetScaleKoef() * _koef; // масштабирование не нужно
    BPoint dirs[2] = {_dirs[0]/* * koef*/, _dirs[1]/* * koef*/};

    // аттрибуты
    glPushAttrib(GL_LINE_BIT);
    glLineWidth(_widthMain);
    glBegin(GL_LINE_LOOP);
        glVertex3dv((_origin + dirs[0]).GetArray());
        glVertex3dv((_origin + dirs[1]).GetArray());
        glVertex3dv((_origin - dirs[0]).GetArray());
        glVertex3dv((_origin - dirs[1]).GetArray());
    glEnd();
    glPopAttrib();
}

BBox DimObjectPlane::GetGabar() const
{
    BBox box;
    //double koef = DimObjects::GetScaleKoef() * _koef;// масштабирование не нужно
    BPoint dirs[2] = {_dirs[0]/* * koef*/, _dirs[1]/* * koef*/};
    box.Expand(_origin + dirs[0]);
    box.Expand(_origin + dirs[1]);
    box.Expand(_origin - dirs[0]);
    box.Expand(_origin - dirs[1]);
    return box;
}

bool DimObjectPlane::IsEqualTo(class DimObject* iDimObject) const
{
    if (iDimObject->GetType() != GetType())
        return false;

    double cos = static_cast<DimObjectPlane*>(iDimObject)->GetNormal() * GetNormal();
    if (fabs(cos - 1.0) > MIND)
        return false;

    BPoint proj = GrazingCurveElemContour::PointProjLine(GetOrigin(), GetOrigin() + GetNormal(), static_cast<DimObjectPlane*>(iDimObject)->GetOrigin());
    double magn2 = (GetOrigin() - proj).D2();
    if (magn2 > MIND * MIND)
        return false;
    
    return true;
}

void DimObjectPlane::Serialize(CArchive& ar)
{
    DimObject::Serialize(ar);
    SerializeElements(ar, &_origin, 1);
    SerializeElements(ar, &_dirs[0], 2);
    SerializeElements(ar, &_koef, 1);
}