#include "StdAfx.h"
#include "stdio.h"
#include "GLee.h"
#include "GL\GL.h"
#include "GL\GLu.h"
#include "BConst.h"
#include "math.h"
#include "BaseRender.h"
#include "NColor.h"
#include "glText.h"
#include "RRuler.h"

RRuler::RRuler(void)
{
}

RRuler::~RRuler(void)
{
}
void RRuler::Draw(bool Active, const Text& fontList)
{
	GLint viewport[4];
	GLdouble projMatrix[16], modelMatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	glGetIntegerv(GL_VIEWPORT, viewport);
	
	const int MaxLen = 300;
	const int RightMargin = 80;
	const int BottomMargin = 16;
	const int IntsNum = 10;
	const double IntsMM[IntsNum] = {0.001, 0.01, 0.1, 1., 10., 100., 1000., 10000., 100000., 1.e8};
	const char *TextsMM[IntsNum] = {"0.001 mm", "0.01 mm", "0.1 mm", "1 mm", "10 mm", "100 mm", "1 m", "10 m", "100 m", "1 km"};
	const double IntsINCH[IntsNum] = {0.0001, 0.001, 0.01, 0.1, 1., 10., 100., 1000., 10000., 100000};
	const char *TextsINCH[IntsNum] = {"0.0001 \"", "0.001 \"", "0.01 \"", "0.1 \"", "1 \"", "10 \"", "100 \"", "1000 \"", "10000 \"", "100000 \""};
	const double *Ints = MM_INCH == M_MM ? IntsMM : IntsINCH;
	const char **Texts = MM_INCH == M_MM ? TextsMM : TextsINCH;

	double x0, y0, z0, x1, y1, z1, x3, y3, z3;
	gluUnProject(viewport[2] - RightMargin - MaxLen + 0.5, BottomMargin + 0.5, 0.5, modelMatrix, projMatrix, viewport, &x0, &y0, &z0);
	gluUnProject(viewport[2] - RightMargin + 0.5, BottomMargin + 0.5, 0.5, modelMatrix, projMatrix, viewport, &x1, &y1, &z1);
	gluUnProject(viewport[2] - RightMargin + 0.5, BottomMargin - 3.5, 0.5, modelMatrix, projMatrix, viewport, &x3, &y3, &z3);
	x3 -= x1;
	y3 -= y1;
	z3 -= z1;
	double Length = sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0) + (z1 - z0) * (z1 - z0));
	double Scale = Ints[IntsNum - 1];
	int Ind = 1;
	for(; Ind < IntsNum && Length >= Ints[Ind]; ++Ind);
	Scale = Ints[Ind - 1];
	const char *Text = Texts[Ind - 1];
	Scale /= Length;
	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT | GL_LINE_BIT | GL_LIGHTING_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glShadeModel(GL_FLAT);
	glLineWidth(2.5);
	NColor Color1 = Active ? NColor(0.8f, 0.8f, 0.8f, 1.f) : NColor(0.937f, 0.894f, 0.69f, 1.f);
	NColor Color2 = Active ? NColor(0.4f, 0.4f, 0.4f, 1.f) : NColor(0.8f, 0.8f, 0.8f, 1.f);
	NColor Color3 = Active ? NColor(0.f, 0.f, 0.f, 1.f) : NColor(0.937f, 0.894f, 0.69f, 1.f);
	glBegin(GL_LINE_STRIP);
	for(int i = 0; i < 6; ++i)
	{
		double t = 1. - i / 5.;
		if(i % 2 == 0)
			glColor3fv(Color1.array);
		else
			glColor3fv(Color2.array);

		glVertex3d(x0 * Scale * t + x1 * ((1. - Scale) * t + (1. - t)), y0 * Scale * t + y1 * ((1. - Scale) * t + (1. - t)), z0 * Scale * t + z1 * ((1. - Scale) * t + (1. - t)));
	}
	glEnd();
	glColor3fv(Color3.array);
	Scale = -15.5 / MaxLen;
	fontList.Draw3D(x0 * Scale + x1 * (1. - Scale) + x3, y0 * Scale + y1 * (1. - Scale) + y3, z0 * Scale + z1 * (1. - Scale) + z3, Text);
	glPopAttrib();
}
