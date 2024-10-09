#include "StdAfx.h"
#include "ConstDef.h"
#include "DimObject.h"
#include "DimObjectSegment.h"
#include "DimObjectArc.h"
#include "DimObjectCircle.h"
#include "DimObjectPlane.h"
#include "BaseRender.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(DimObject, CObject, 0)

Text DimObject::DimText(1000);
int DimObject::Digits = 0;

DimObject::DimObject(const DimObject &iObject)
{
    ObjectID = iObject.ObjectID; // при добавлении этого объекта в DimObjects ObjectID всё равно перепишется
    Highlighted = false;
    Selected = false;
    _changeState = false;
    _oldPt = iObject._oldPt;
    _color = iObject._color;
    _widthMain = iObject._widthMain;
    _hidden = iObject._hidden;
}

DimObject::Color::Color()
{
    _common[0] = 0.0;
    _common[1] = 0.0;
    _common[2] = 1.0;

    _whenAimed[0] = 1.0;
    _whenAimed[1] = 0.0;
    _whenAimed[2] = 1.0;

    _whenActive[0] = 0.0;
    _whenActive[1] = 1.0;
    _whenActive[2] = 1.0;

    _whenActiveNAimed[0] = 1.0;
    _whenActiveNAimed[1] = 1.0;
    _whenActiveNAimed[2] = 1.0;
}

void DimObject::Draw() const
{
    if (_hidden)
		return;

	glPushName(ObjectID);
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	if(_depthTest)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	int Index = (Highlighted ? 1 : 0) + (Selected ? 2 : 0);
	switch(Index)
	{
	case 0:
	default:
        glColor3dv(_color._common);
		break;
	case 1:
        glColor3dv(_color._whenAimed);
		break;
	case 2:
		glColor3dv(_color._whenActive);
		break;
	case 3:
		glColor3dv(_color._whenActiveNAimed);
		break;
	}

	DrawSpecific();
	
	glPopAttrib();
	glPopName();
}

void DimObject::EnableChangeState(const BPoint& iPoint, const BPoint& iViewDir)
{
    _changeState = true; 
    
    FindNearestPoint(iPoint, iViewDir, _oldPt);
}

BPoint DimObject::GetViewDir()
{
	GLint viewport[4];
	GLdouble projMatrix[16],modelMatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	glGetIntegerv(GL_VIEWPORT, viewport);
	GLdouble x = 0., y = 0., z = 0.;
	gluUnProject(viewport[0], viewport[1], 0., modelMatrix, projMatrix, viewport, &x, &y, &z);
	BPoint P(x, y, z, 1.);
	gluUnProject(viewport[0], viewport[1], 1., modelMatrix, projMatrix, viewport, &x, &y, &z);
	P = BPoint(x, y, z, 1.) - P;
	double d = sqrt(P.D2());
    if(d > MIND)
		d = 1. / d;
	else
		d = 1.;
	return P * d;
}

BPoint DimObject::GetXDir()
{
    GLint viewport[4];
	GLdouble projMatrix[16],modelMatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	glGetIntegerv(GL_VIEWPORT, viewport);
	GLdouble x = 0., y = 0., z = 0.;
	gluUnProject(0.0, 0.0, 0.0, modelMatrix, projMatrix, viewport, &x, &y, &z);
	BPoint P(x, y, z, 1.);
	gluUnProject(1.0, 0.0, 0.0, modelMatrix, projMatrix, viewport, &x, &y, &z);
	P = BPoint(x, y, z, 1.) - P;
	double d = sqrt(P.D2());
	if(d > MIND)
		d = 1. / d;
	else
		d = 1.;
	return P * d;
}

BPoint DimObject::GetAxisDir(int iAxisIndex)
{
    GLint viewport[4];
	GLdouble projMatrix[16],modelMatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	glGetIntegerv(GL_VIEWPORT, viewport);
	GLdouble x = 0., y = 0., z = 0.;
    gluUnProject(0.0, 0.0, 0.0, modelMatrix, projMatrix, viewport, &x, &y, &z);
    BPoint P(x, y, z, 1.);

    if (iAxisIndex > 2)
        return BPoint(0.0, 0.0, 0.0, -1.0);
    double xyz[3] = {0.0, 0.0, 0.0};
    xyz[iAxisIndex] = 1.0;
    gluUnProject(xyz[0], xyz[1], xyz[2], modelMatrix, projMatrix, viewport, &x, &y, &z);
	P = BPoint(x, y, z, 1.) - P;

    double d = sqrt(P.D2());
	if(d > MIND)
		d = 1. / d;
	else
		d = 1.;
	return P * d;
}

void DimObject::DrawGLPrimitive(const BPoint* iPoints, unsigned int iCount)
{
    switch (iCount)
    {
    case 1:
        glBegin(GL_POINTS);
        break;
    case 2:
        glBegin(GL_LINES);
        break;
    case 3:
        glBegin(GL_TRIANGLES);
        break;
    case 4:
        glBegin(GL_QUADS);
        break;
    default:
        glBegin(GL_LINE_LOOP);
        break;
    };
    for (unsigned int i = 0; i < iCount; ++i)
        glVertex3dv(iPoints[i].GetArray());
    glEnd();
}

bool DimObject::IsOnLine(const BPoint& iP0, const BPoint& iP1, const BPoint& iP2)
{
    BPoint vect[2] = {iP0 - iP1, iP2 - iP1};
    double magn2[2] = {vect[0].D2(), vect[1].D2()};
    double mind2 = MIND * MIND;

    bool isOnLine = (magn2[0] < mind2) || (magn2[1] < mind2);
    if (!isOnLine)
    {
        BPoint vectNorm[2] = {(iP0 - iP1).Unit(), (iP2 - iP1).Unit()};
        double cos = vectNorm[0] * vectNorm[1];
        isOnLine = (1.0 - fabs(cos)) < MIND;
    }

    return isOnLine;
}

void DimObject::IntersectObjects(class DimObject* iDimObject0, class DimObject* iDimObject1, std::vector<BPoint>& oIntersectPoints)
{
    DimObject* objs[2] = {iDimObject0, iDimObject1};
    if (iDimObject0->GetType() == DimObjects::DimObjectArcType)
        objs[0] = new DimObjectCircle(static_cast<DimObjectArc*>(iDimObject0));
    if (iDimObject1->GetType() == DimObjects::DimObjectArcType)
        objs[1] = new DimObjectCircle(static_cast<DimObjectArc*>(iDimObject1));

    bool pt[3][2] = {dynamic_cast<DimObjectSegment*>(objs[0]) ? true : false, dynamic_cast<DimObjectSegment*>(objs[1]) ? true : false,
                     dynamic_cast<DimObjectCircle*>(objs[0]) ? true : false,  dynamic_cast<DimObjectCircle*>(objs[1]) ? true : false,
                     dynamic_cast<DimObjectPlane*>(objs[0]) ? true : false,   dynamic_cast<DimObjectPlane*>(objs[1]) ? true : false};

    int specialCase = -1;

    for (unsigned int i = 0; i < 3; ++i)
    {
        for (unsigned int j = 0; j < 3; ++j)
            if (pt[i][0] && pt[j][1])
            {
                specialCase = i * 3 + j;
                break;
            }

        if (specialCase != -1)
            break;
    }

    if (specialCase == -1)
        return;

    oIntersectPoints.clear();
    switch (specialCase)
    {
    case 0:
    {
        IntersectSegments(objs[0], objs[1], oIntersectPoints);
        break; // done
    }
    case 1:
        IntersectSegmentNCircle(objs[0], objs[1], oIntersectPoints);
        break; // done
    case 2:
        IntersectSegmentNPlane(objs[0], objs[1], oIntersectPoints);
        break; // done
    case 3:
        IntersectSegmentNCircle(objs[1], objs[0], oIntersectPoints);
        break; // done
    case 4:
        IntersectCircles(objs[0], objs[1], oIntersectPoints);
        break; // done
    case 5:
        IntersectCircleNPlane(objs[0], objs[1], oIntersectPoints);
        break; // done
    case 6:
        IntersectSegmentNPlane(objs[1], objs[0], oIntersectPoints);
        break; // done
    case 7:
        IntersectCircleNPlane(objs[1], objs[0], oIntersectPoints);
        break; // done
    case 8:
        IntersectPlanes(objs[0], objs[1], oIntersectPoints);
        break; // done
    };

    if (iDimObject0->GetType() == DimObjects::DimObjectArcType)
        delete objs[0];
    if (iDimObject1->GetType() == DimObjects::DimObjectArcType)
        delete objs[1];
}

void DimObject::IntersectSegments(class DimObject* iDimObject0, class DimObject* iDimObject1, std::vector<BPoint>& oIntersectPoints)
{
    // find the point on the first segment nearest to the second segment
    const BPoint* pts[4] = {&static_cast<DimObjectSegment*>(iDimObject0)->GetStartPoint(), &static_cast<DimObjectSegment*>(iDimObject0)->GetEndPoint(),
                     &static_cast<DimObjectSegment*>(iDimObject1)->GetStartPoint(), &static_cast<DimObjectSegment*>(iDimObject1)->GetEndPoint()};
    oIntersectPoints.push_back(GrazingCurveElemContour::TraceOfLines(*pts[0], *pts[1], *pts[2], *pts[3]));
    oIntersectPoints[0] = GrazingCurveElemContour::PointProjLine(*pts[0], *pts[1], oIntersectPoints[0]);
}

void DimObject::IntersectSegmentNArc(class DimObject* iDimObject0, class DimObject* iDimObject1, std::vector<BPoint>& oIntersectPoints)
{
    BPoint pt,
           pts[2] = {static_cast<DimObjectSegment*>(iDimObject0)->GetStartPoint(), 
                     static_cast<DimObjectSegment*>(iDimObject0)->GetEndPoint()},
           origin = static_cast<DimObjectArc*>(iDimObject1)->GetCenter(),
           normal = static_cast<DimObjectArc*>(iDimObject1)->GetNormal();
    AvailableTrace result = GrazingCurveElemContour::LinePlane(pts[0], (pts[1] - pts[0]).Unit(), origin, normal, pt);
    if (result == ABSENCE_TRACE)
        return;
    else if (result == ONE_TRACE)
    {
        //if (iDimObject0->Contains(pt))
            if (iDimObject1->Contains(pt))
                oIntersectPoints.push_back(pt);
    }
    else
    {
        // построить плоскость, перпендикулярную плоскости окружности, проходящую через отрезок
        pt = pts[0] + normal * sqrt((pts[0] - pts[1]).D2());
        DimObject* plane;
        bool success = DimObjectPlane::CreateOnThreePoints(pts[0], pts[1], pt, plane);
        if (!success)
            return; // но этого быть не должно

        // пересечь с окружностью (или дугой)
        std::vector<BPoint> vect;
        IntersectArcNPlane(iDimObject1, plane, vect);
        delete plane;

        // проверить принадлежность точки объектам
        for (unsigned int i = 0; i < vect.size(); ++i)
            //if (iDimObject0->Contains(vect[i])) // для того, чтобы отрезок считался прямой
                if (iDimObject1->Contains(vect[i]))
                    oIntersectPoints.push_back(vect[i]);
    }

    return;
}

void DimObject::IntersectSegmentNCircle(class DimObject* iDimObject0, class DimObject* iDimObject1, std::vector<BPoint>& oIntersectPoints)
{
    IntersectSegmentNArc(iDimObject0, iDimObject1, oIntersectPoints);

    return;
}

void DimObject::IntersectSegmentNPlane(class DimObject* iDimObject0, class DimObject* iDimObject1, std::vector<BPoint>& oIntersectPoints)
{
    BPoint pt,
           pts[2] = {static_cast<DimObjectSegment*>(iDimObject0)->GetStartPoint(), 
                     static_cast<DimObjectSegment*>(iDimObject0)->GetEndPoint()},
           origin = static_cast<DimObjectPlane*>(iDimObject1)->GetOrigin(),
           normal = static_cast<DimObjectPlane*>(iDimObject1)->GetNormal();

    AvailableTrace result = GrazingCurveElemContour::LinePlane(pts[0], (pts[1] - pts[0]).Unit(), origin, normal, pt);
    if (result != ONE_TRACE)
        return;

    // проверить принадлежность отрезку
    //if (iDimObject0->Contains(pt)) // для того, чтобы отрезок считался прямой
        oIntersectPoints.push_back(pt);
}

void DimObject::IntersectArcs(class DimObject* iDimObject0, class DimObject* iDimObject1, std::vector<BPoint>& oIntersectPoints)
{
    BPoint normal = static_cast<DimObjectArc*>(iDimObject1)->GetNormal(),
           directions[2] = {static_cast<DimObjectArc*>(iDimObject1)->GetI(), static_cast<DimObjectArc*>(iDimObject1)->GetI() % normal};
    DimObject* plane = new DimObjectPlane(static_cast<DimObjectArc*>(iDimObject1)->GetCenter(), directions);
    
    std::vector<BPoint> pts;
    IntersectCircleNPlane(iDimObject0, plane, pts);
    delete plane;
    if (pts.empty())
    {
        BPoint c[2] = {static_cast<DimObjectArc*>(iDimObject0)->GetCenter(), static_cast<DimObjectArc*>(iDimObject1)->GetCenter()};
        BPoint vect = (c[1] - c[0]);

        double magn = vect.D2();
        if (magn < MIND * MIND)
            return;
        
        magn = sqrt(magn);
        vect *= 1.0 / magn;
        double cos = normal * vect;
        if (fabs(cos) > MIND)
            return;
        
        double r[2] = {static_cast<DimObjectArc*>(iDimObject0)->GetRadius(), static_cast<DimObjectArc*>(iDimObject1)->GetRadius()};
        if (magn > r[0] + r[1])
            return;

        if (magn < r[0] - r[1] || magn < r[1] - r[0])
            return;

        double h = 0.5 * (r[1] * r[1] - r[0] * r[0] + magn * magn) / magn;
        BPoint p = c[1] - vect * h;
        double tmpR = sqrt(r[1] * r[1] - h * h);
        BPoint tmpRDir = vect % normal;
        pts.push_back(p + tmpRDir * tmpR);
        pts.push_back(p - tmpRDir * tmpR);
    }

    for (unsigned int i = 0; i < (unsigned int)pts.size(); ++i)
        if (iDimObject0->Contains(pts[i]))
            if (iDimObject1->Contains(pts[i]))
                oIntersectPoints.push_back(pts[i]);

    return;
}

void DimObject::IntersectArcNCircle(class DimObject* iDimObject0, class DimObject* iDimObject1, std::vector<BPoint>& oIntersectPoints)
{
    IntersectArcs(iDimObject0, iDimObject1, oIntersectPoints);

    return;
}

void DimObject::IntersectArcNPlane(class DimObject* iDimObject0, class DimObject* iDimObject1, std::vector<BPoint>& oIntersectPoints)
{
    std::vector<BPoint> vect;
    IntersectCircleNPlane(iDimObject0, iDimObject1, vect);
    if (vect.empty())
        return;

    for (unsigned int i = 0; i < (unsigned int)vect.size(); ++i)
        if (iDimObject0->Contains(vect[i]))
            oIntersectPoints.push_back(vect[i]);

    return;
}

void DimObject::IntersectCircles(class DimObject* iDimObject0, class DimObject* iDimObject1, std::vector<BPoint>& oIntersectPoints)
{
    IntersectArcs(iDimObject0, iDimObject1, oIntersectPoints);

    return;
}

void DimObject::IntersectCircleNPlane(class DimObject* iDimObject0, class DimObject* iDimObject1, std::vector<BPoint>& oIntersectPoints)
{
    BPoint origin[2] = {static_cast<DimObjectArc*>(iDimObject0)->GetCenter(), static_cast<DimObjectPlane*>(iDimObject1)->GetOrigin()},
           normal[2] = {static_cast<DimObjectArc*>(iDimObject0)->GetNormal(), static_cast<DimObjectPlane*>(iDimObject1)->GetNormal()},
           pts[2];

    double r = static_cast<DimObjectArc*>(iDimObject0)->GetRadius();

    AvailableTrace result = GrazingCurveElemContour::CirclePlane(origin[0], normal[0], origin[1], normal[1], r * r, pts);
    if (result == ONE_TRACE)
        oIntersectPoints.push_back(pts[0]);
    else if (result == TWO_TRACE)
    {
        oIntersectPoints.push_back(pts[0]);
        oIntersectPoints.push_back(pts[1]);
    }

    return;
}

void DimObject::IntersectPlanes(class DimObject* iDimObject0, class DimObject* iDimObject1, std::vector<BPoint>& oIntersectPoints)
{
    BPoint origin[2] = {static_cast<DimObjectPlane*>(iDimObject0)->GetOrigin(), static_cast<DimObjectPlane*>(iDimObject1)->GetOrigin()},
           normal[2] = {static_cast<DimObjectPlane*>(iDimObject0)->GetNormal(), static_cast<DimObjectPlane*>(iDimObject1)->GetNormal()},
           pts[2];

    AvailableTrace result = GrazingCurveElemContour::PlanePlane(origin[0], normal[0], origin[1], normal[1], pts);
    if (result != ONE_TRACE)
        return;

    oIntersectPoints.push_back(pts[0]); // точка на прямой
    oIntersectPoints.push_back(pts[1]); // направление прямой

    return;
}

void DimObject::Serialize(CArchive& ar)
{
	SerializeElements(ar, &_color, 1);
	SerializeElements(ar, &_widthMain, 1);
	SerializeElements(ar, &_hidden, 1);
	SerializeElements(ar, &ObjectID, 1);
}