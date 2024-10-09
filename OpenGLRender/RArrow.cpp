#include "StdAfx.h"
#include "GLee.h"
#include "GL\GL.h"
#include "GL\GLu.h"
#include "BMatr.h"
#include "BBox.h"
#include "BaseRender.h"
#include "RArrow.h"

RArrow::RArrow(void) : P(0., 0., 0., 1.), V(0., 0., 0., 1.)
{
	Defined = false;
	BaseColor[0] = 0.;
	BaseColor[1] = 1.;
	BaseColor[2] = 0.;
	CylColor[0] = 0.;
	CylColor[1] = 1.;
	CylColor[2] = 0.;
	ConeColor[0] = 0.;
	ConeColor[1] = 1.;
	ConeColor[2] = 0.;
	SlicesNum = 15;
}

RArrow::~RArrow(void)
{
}
void RArrow::Draw(bool Screen /*= false*/, bool Base /*= true*/, bool Cyl /*= true*/, bool Cone /*= true*/) const
{
	if(!Defined)
		return;
	glPushAttrib(GL_COLOR_BUFFER_BIT);
	double Scale = 1.;
	if(Screen)
	{
		double x, y, z;
		BaseRender::GetPixelDim(x, y, z);
		Scale = x;
	}
	GLUquadricObj *qobj = gluNewQuadric();
	BMatr M;
	BPoint Q(0., 0., 1., 0.);
	M.e0cong(Q, V);
	glPushMatrix();
	glTranslated(P.GetX(), P.GetY(), P.GetZ());
	if(Base)
	{
		glColor3dv(BaseColor);
		gluSphere(qobj, RadiusOfBasePoint * Scale, SlicesNum, SlicesNum);
	}
	glMultMatrixd(M.GetArray());
	if(Cyl)
	{
		glColor3dv(CylColor);
		gluCylinder(qobj, RadiusOfCylinder * Scale, RadiusOfCylinder * Scale, Length * Scale, SlicesNum, 1);
	}
	if(Cone)
	{
		glColor3dv(ConeColor);
		glTranslated(0., 0, Length * Scale);
		gluDisk(qobj, 0, RadiusOfArrow * Scale, SlicesNum, 1);
		gluCylinder(qobj, RadiusOfArrow * Scale, 0, LengthOfArrow * Scale, SlicesNum, 1);
	}
	glPopMatrix();
	glPopAttrib();
	gluDeleteQuadric(qobj);
}

void RArrow::Set(const BPoint &iP, const BPoint &iV, double WholeLength)
{
	P = iP;
	V = iV;
	Defined = true;
	if(WholeLength <= 0. && Defined)
		return;
	SetLength(WholeLength);
}

void RArrow::SetBaseColor(double r, double g, double b)
{
	BaseColor[0] = r;
	BaseColor[1] = g;
	BaseColor[2] = b;
}
void RArrow::SetCylColor(double r, double g, double b)
{
	CylColor[0] = r;
	CylColor[1] = g;
	CylColor[2] = b;
}
void RArrow::SetConeColor(double r, double g, double b)
{
	ConeColor[0] = r;
	ConeColor[1] = g;
	ConeColor[2] = b;
}
void RArrow::SetColor(double r, double g, double b)
{
	SetBaseColor(r, g, b);
	SetCylColor(r, g, b);
	SetConeColor(r, g, b);
}

void RArrow::SetParams(double BaseR, double CylR, double ConeR, double CylLen, double ConeLen, int Slices /* = 15*/)
{

	Length = CylLen;
	LengthOfArrow = ConeLen;
	RadiusOfBasePoint = BaseR;
	RadiusOfArrow = ConeR;
	RadiusOfCylinder = CylR;
	SlicesNum = Slices;
}

void RArrow::SetLength(double WholeLength)
{
	Length = WholeLength * 0.8;
	LengthOfArrow = WholeLength - Length;
	RadiusOfBasePoint = 0.1 * Length;
	RadiusOfArrow = 0.1 * Length;
	RadiusOfCylinder = 0.05 * Length;
}

BBox RArrow::GetGabar() const
{
	BBox box;
	box.Expand(RadiusOfBasePoint, RadiusOfBasePoint, RadiusOfBasePoint);
	box.Expand(-RadiusOfBasePoint, -RadiusOfBasePoint, -RadiusOfBasePoint);
	box.Expand(RadiusOfCylinder, RadiusOfCylinder, Length);
	box.Expand(-RadiusOfCylinder, -RadiusOfCylinder, 0);
	box.Expand(RadiusOfArrow, RadiusOfArrow, Length+ LengthOfArrow);
	box.Expand(-RadiusOfArrow, -RadiusOfArrow, Length);
	return box;
}