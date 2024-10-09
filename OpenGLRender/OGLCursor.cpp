#include "stdafx.h"
#include "OGLCursor.h"
#include "GLee.h"
#include "Gl\Glu.h"
#include "BaseRender.h"
#include "BMatr.h"
#include "ConstDef.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

void OGLCursor::Set(double xi, double yi, double zi)
{
	x = xi;
	y = yi;
	z = zi;
	Defined = true;
}

void OGLCursor::Get(double &xo, double &yo, double &zo) const
{
	xo = x;
	yo = y;
	zo = z;
}

void OGLCursor::Draw() const
{// Draw image in the current raster position
	if(!Defined)
		return;

//	Shader::ShaderOFF();
	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT | GL_LINE_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glDisable(GL_LINE_SMOOTH);
	glLineWidth(width);
	// Draw new image
	BMatr M = BaseRender::GetUnprojectMatr();
	BPoint dXW = BPoint(1., 0., 0., 0.) * M * size;
	BPoint dYW = BPoint(0., 1., 0., 0.) * M * size;
	BPoint P(x, y, z, 1.);
	glColor3d(cR, cG, cB);

    switch (type)
    {
    case QuadDiagonalType:
        DrawQuadDiagonal(P, dXW, dYW);
        break;
    case QuadType:
        DrawQuad(P, dXW, dYW);
        break;
    case ClockType:
        DrawClock(P, dXW, dYW);
        break;
    case CircleType:
        DrawCircle(P, dXW, dYW);
        break;
    }

	glPopAttrib();
	glFinish();
}

void OGLCursor::DrawQuadDiagonal(const BPoint& iP, const BPoint& iDXW, const BPoint& iDYW) const
{
	glBegin(GL_LINE_LOOP);
	glVertex3dv((iP + iDXW + iDYW ).GetArray());
	glVertex3dv((iP - iDXW + iDYW ).GetArray());
	glVertex3dv((iP + iDXW - iDYW ).GetArray());
	glVertex3dv((iP - iDXW - iDYW).GetArray());
	glEnd();

	glBegin(GL_LINES);
	glVertex3dv((iP + iDXW + iDYW).GetArray());
	glVertex3dv((iP + iDXW - iDYW).GetArray());
	glVertex3dv((iP - iDXW + iDYW).GetArray());
	glVertex3dv((iP - iDXW - iDYW).GetArray());
	glEnd();
}

void OGLCursor::DrawQuad(const BPoint& iP, const BPoint& iDXW, const BPoint& iDYW) const
{
	glBegin(GL_LINE_LOOP);
	glVertex3dv((iP + iDXW + iDYW).GetArray());
	glVertex3dv((iP - iDXW + iDYW).GetArray());
	glVertex3dv((iP - iDXW - iDYW).GetArray());
	glVertex3dv((iP + iDXW - iDYW).GetArray());
	glEnd();
}

void OGLCursor::DrawClock(const BPoint& iP, const BPoint& iDXW, const BPoint& iDYW) const
{
	glBegin(GL_LINE_LOOP);
	glVertex3dv((iP + iDXW + iDYW).GetArray());
	glVertex3dv((iP - iDXW + iDYW).GetArray());
	glVertex3dv((iP + iDXW - iDYW).GetArray());
	glVertex3dv((iP - iDXW - iDYW).GetArray());
	glEnd();
}

void OGLCursor::DrawCircle(const BPoint& iP, const BPoint& iDXW, const BPoint& iDYW) const
{
    double x, y, z;
	BaseRender::GetPixelDim(x, y, z);
	double Scale = x;

	static const int SlicesNum = 15;
    static const double deltaAngle = 2.0 * PI / SlicesNum;

    glBegin(GL_LINE_LOOP);
    double angle = 0.0;
    glVertex3dv((iP + iDXW).GetArray());
    for (unsigned int i = 1; i < SlicesNum; ++i)
    {
        angle += deltaAngle;
        glVertex3dv((iP + iDXW * cos(angle) + iDYW * sin(angle)).GetArray());
    }
    glEnd();
}