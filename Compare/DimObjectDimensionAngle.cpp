#include "StdAfx.h"
#include "ConstDef.h"
#include "DimObjectDimensionAngle.h"
#include "BaseRender.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(DimObjectDimensionAngle, DimObjectDimension, 0)

DimObjectDimensionAngle::DimObjectDimensionAngle(const DimObjectArc* iArc, float iWidth)
    : DimObjectDimension(iWidth, "", "°")
{
    _dimLine = new DimObjectArc(*iArc);

    BPoint center = iArc->GetCenter(), pts[2];
    iArc->GetEndPoints(pts[0], pts[1]);

    BPoint vect[2] = {pts[0] - center, pts[1] - center};

    _data._magnExtens = sqrt(vect[0].D2());
    _data._dirExtens[0] = vect[0].Unit();
    _data._dirExtens[1] = vect[1].Unit(); // vect могут быть разной длины

    static_cast<DimObjectArc*>(_dimLine)->SetEndPoint(center + _data._dirExtens[1] * _data._magnExtens);
}

bool DimObjectDimensionAngle::CreateWithCenter(const BPoint& iCenter, const BPoint& iPoint0, const BPoint& iPoint1, DimObject*& oAngle, float iWidth)
{
    // создать дугу
    DimObject* arc;
    bool success = DimObjectArc::Create(iCenter, iPoint0, iPoint1, arc, true, 2.0, 2.0, iWidth);
    if (!success)
        return false;

    oAngle = new DimObjectDimensionAngle(static_cast<DimObjectArc*>(arc), iWidth);
    delete arc;

    return true;
}

bool DimObjectDimensionAngle::CreateOnThreePoints(const BPoint& iPoint0, const BPoint& iPoint1, const BPoint& iPoint2, DimObject*& oAngle, float iWidth)
{
    DimObject* arc;
    bool success = DimObjectArc::CreateOnThreePoints(iPoint0, iPoint1, iPoint2, arc, 2.0, 2.0, iWidth);
    if (!success)
        return false;

    oAngle = new DimObjectDimensionAngle(static_cast<DimObjectArc*>(arc), iWidth);
    delete arc;

    return true;
}

void DimObjectDimensionAngle::DrawSpecific() const
{
    // выносные линии
    DimObjectArc* arc = static_cast<DimObjectArc*>(_dimLine);
    BPoint center = arc->GetCenter();
    //BPoint dimPts[2] = {center + _data._dirExtens[0] * _data._magnExtens, center + _data._dirExtens[1] * _data._magnExtens};
    BPoint dimPts[2];
    GetEndPoints(dimPts[0], dimPts[1]);
    BPoint pts[2][2] = {center, dimPts[0],
                        center, dimPts[1]};

    if (_data._magnExtens > 0.0)
    {
        // выносные линии
        glPushAttrib(GL_LINE_BIT);
        glLineWidth(_data._widthExtens);
        DrawGLPrimitive(pts[0], 2);
        DrawGLPrimitive(pts[1], 2);
        glPopAttrib();
    }

    if (Selected)
    {
        BaseRender::DrawBall(pts[0][0], _data._radius, true);
        BaseRender::DrawBall(pts[0][1], _data._radius, true);

        if (_data._magnExtens > 0.0)
        {
            BaseRender::DrawBall(dimPts[0], _data._radius, true);
            BaseRender::DrawBall(dimPts[1], _data._radius, true);
        }
    }
    
    // размерная линия
    glPushName(1);
    _dimLine->DrawSpecific();

    // стрелки
    DrawCursors(pts[0], pts[1], DimObjects::GetScaleKoef(), true);

    // значение
    DrawDimValue(GetDimValueText(), dimPts, _dimLine->GetMidPoint(), DimObjects::GetScaleKoef());
    glPopName();
}

void DimObjectDimensionAngle::Change(const BPoint& iPoint, const BPoint& iViewDir)
{
    BPoint center = static_cast<DimObjectArc*>(_dimLine)->GetCenter(),
           normal = static_cast<DimObjectArc*>(_dimLine)->GetNormal();
    
    BPoint pt;
    AvailableTrace result = GrazingCurveElemContour::LinePlane(iPoint, iViewDir, center, normal, pt);
    if (result != ONE_TRACE)
        return;

    _data._magnExtens = sqrt((pt - center).D2());
    if (_data._magnExtens < MIND)
        return;

    static_cast<DimObjectArc*>(_dimLine)->SetStartPoint(center + _data._dirExtens[0] * _data._magnExtens);
    static_cast<DimObjectArc*>(_dimLine)->SetEndPoint(center + _data._dirExtens[1] * _data._magnExtens);
    
    //_oldPt = iPoint;
}

double DimObjectDimensionAngle::GetDimValue() const
{
    double val = static_cast<DimObjectArc*>(_dimLine)->GetAngle() * 180.0 / PI;
    
    /*bool lessThanPI = static_cast<DimObjectArc*>(_dimLine)->IsLessThanPI();
    if (!lessThanPI)
        val = 360.0 - val;*/

    return val;
}