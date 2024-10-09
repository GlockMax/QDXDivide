#include "StdAfx.h"
#include "ConstDef.h"
#include "DimObjectDimensionLinear.h"
#include "DimObjectPoint.h"
#include "DimObjectSegment.h"
#include "BaseRender.h"
#include <string>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(DimObjectDimensionLinear, DimObjectDimension, 0)

DimObjectDimensionLinear::DimObjectDimensionLinear(const BPoint& iPoint0, const BPoint& iPoint1, const BPoint& iPoint2, float iWidth, char* iPrefix, double iKoef)
    : DimObjectDimension(iWidth, iPrefix), _koef(iKoef)
{
    BPoint vect = iPoint2 - iPoint1;
    _data._magnExtens = sqrt(vect.D2());

    _dimLine = new DimObjectSegment(iPoint0 + vect, iPoint2, iWidth);
    
    if (_data._magnExtens > 0.0)
        _data._dirExtens[0] = _data._dirExtens[1] = vect.Unit();
}

void DimObjectDimensionLinear::DrawSpecific() const
{
    BPoint dir = _data._dirExtens[0] * _data._magnExtens;    BPoint dimPts[2];
    GetEndPoints(dimPts[0], dimPts[1]);

    BPoint pts[2][2] = {dimPts[0] - dir, dimPts[0],
                        dimPts[1] - dir, dimPts[1]};
    if (_data._magnExtens > 0.0)
    {
        // выносные линии
        glPushAttrib(GL_LINE_BIT);
        glLineWidth(_data._widthExtens);
        DrawGLPrimitive(pts[0], 2);
        DrawGLPrimitive(pts[1], 2);
        glPopAttrib();
    }

    double scaleKoef = DimObjects::GetScaleKoef();
    if (Selected)
    {
        BaseRender::DrawBall(dimPts[0], _data._radius * scaleKoef, false);
        BaseRender::DrawBall(dimPts[1], _data._radius * scaleKoef, false);

        if (_data._magnExtens > 0.0)
        {
            BaseRender::DrawBall(pts[0][0], _data._radius * scaleKoef, false);
            BaseRender::DrawBall(pts[1][0], _data._radius * scaleKoef, false);
        }
    }
    
    // размерная линия
    glPushName(1);
    _dimLine->DrawSpecific();
        
    // стрелки
    DrawCursors(pts[0], pts[1], scaleKoef, false);

    // значение
    std::string str = GetDimValueText();
    DrawDimValue(str, dimPts, (dimPts[0] + dimPts[1]) * 0.5, scaleKoef);
    glPopName();
}

void DimObjectDimensionLinear::Change(const BPoint& iPoint, const BPoint& iViewDir)
{
    BPoint dir = _data._dirExtens[0] * _data._magnExtens;
    BPoint dimPt = static_cast<DimObjectSegment*>(_dimLine)->GetStartPoint() - dir;

	BPoint pt = BPoint::ProjectPointOnPlane(iPoint, _oldPt, iViewDir);
    BPoint vect = pt - _oldPt;

    static_cast<DimObjectSegment*>(_dimLine)->SetStartPoint(static_cast<DimObjectSegment*>(_dimLine)->GetStartPoint() + vect);
    static_cast<DimObjectSegment*>(_dimLine)->SetEndPoint(static_cast<DimObjectSegment*>(_dimLine)->GetEndPoint() + vect);

    _data._dirExtens[0] = static_cast<DimObjectSegment*>(_dimLine)->GetStartPoint() - dimPt;
    _data._magnExtens = sqrt(_data._dirExtens[0].D2());
    if (_data._magnExtens > MIND)
        _data._dirExtens[1] = _data._dirExtens[0] = _data._dirExtens[0] * (1.0 / _data._magnExtens);

    _oldPt = _oldPt + vect;
}

double DimObjectDimensionLinear::GetDimValue() const
{
    BPoint pts[2];
    GetEndPoints(pts[0], pts[1]);
    return sqrt((pts[0] - pts[1]).D2()) * _koef;
}

void DimObjectDimensionLinear::Serialize(CArchive& ar)
{
    DimObjectDimension::Serialize(ar);
    SerializeElements(ar, &_koef, 1);
}

bool DimObjectDimensionLinear::IsEqualTo(DimObject* iDimObject) const
{
    return false;
}
