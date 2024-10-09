#include "stdafx.h"
#include "BaseRender.h"
#include "DDistance.h"

DDistance::DDistance()
{
    active_ = false;
    base_.Set(0., 0., 0., -1.);
}

bool DDistance::IsActive() const
{
    return active_;
}

void DDistance::SetActive(bool set)
{
    active_ = set;
}

void DDistance::SetSnapPoint(const BPoint& p)
{
    snap_ = p;
}

void DDistance::SetClickPoint()
{
    if (snap_.IsPoint())
    {
        base_ = snap_;
        snap_.Set(0., 0., 0., -1.);
    }
}

void DDistance::SetLastPoint(const BPoint& p)
{
    last_ = p;
}

void DDistance::Draw()
{
    if (!IsActive())
        return;
    if (base_.IsPoint())
    {
        if (last_.IsPoint())
            DrawRubberLine(base_, last_);
        if (snap_.IsPoint())
            DrawLine(base_, snap_);
        else if (last_.IsPoint())
            DrawLine(base_, base_);
    }
}

void DDistance::DrawLine(const BPoint& p0, const BPoint& p1) const
{
    glPushAttrib(GL_LINE_BIT | GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glColor3d(0., 0., 0.);
    glLineWidth(3.0);

    BaseRender::DrawFlatArrow(p0, p1, 25., 5.);

    glPointSize(7.0);
    glEnable(GL_POINT_SMOOTH);
    glBegin(GL_POINTS);
    glColor3d(1., 0., 0.);
    glVertex3dv(p1.GetArray());
    glColor3d(0., 1., 0.);
    glVertex3dv(p0.GetArray());
    glEnd();

    glPopAttrib();
}

void DDistance::DrawRubberLine(const BPoint& p0, const BPoint& p1) const
{
    glPushAttrib(GL_LINE_BIT | GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glColor3d(0., 0., 0.);
    glLineWidth(1.0);
    glBegin(GL_LINES);
    glVertex3dv(p0.GetArray());
    glVertex3dv(p1.GetArray());
    glEnd();

    glPopAttrib();
}

const BPoint& DDistance::GetBase() const
{
    return base_;
}

const BPoint& DDistance::GetSnap() const
{
    return snap_;
}
