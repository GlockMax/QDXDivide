// NxyzImage.cpp: implementation of the NxyzImage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BaseRender.h"
#include "BPoint.h"
#include "Gl\GLu.h"
#include "math.h"
#include "NxyzImage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const double t_s = 7.; // размер символов
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NxyzImage::NxyzImage()
{
	IsVisible = TRUE;	// видимость всех осей
	ZeroPos = ZPS_AXIS | ZPS_FSYS | ZPS_SSYS;		// видимость осей в нуле (у объекта)
	IsVisibleBase = TRUE; // видимость базовых  осей

	AbsoluteSize = FALSE;		// изображение осей в абсодлютном размере
	IsVisibleZeroXYZ = FALSE;	// видимость названий кординат
	
	num_x = num_y = num_zpl = num_zmin = 0;
}

NxyzImage::~NxyzImage()
{

}

void NxyzImage::DrawStockMarker(double x, double y, double z) const
{
	glLineWidth(1.);
	glBegin(GL_LINES);
	// Box
	const double dx = 0.5, dy = 0.25, dz = 0.15 * 2;
	glColor3d(0.5, 0.5, 0.5);
	glVertex3d(x - dx * s1, y - dy * s1, z - dz * s1);
	glVertex3d(x + dx * s1, y - dy * s1, z - dz * s1);
	glVertex3d(x - dx * s1, y + dy * s1, z - dz * s1);
	glVertex3d(x + dx * s1, y + dy * s1, z - dz * s1);
	glVertex3d(x - dx * s1, y + dy * s1, z);
	glVertex3d(x + dx * s1, y + dy * s1, z);
	glVertex3d(x - dx * s1, y - dy * s1, z);
	glVertex3d(x + dx * s1, y - dy * s1, z);

	glVertex3d(x - dx * s1, y - dy * s1, z - dz * s1);
	glVertex3d(x - dx * s1, y - dy * s1, z);
	glVertex3d(x + dx * s1, y - dy * s1, z - dz * s1);
	glVertex3d(x + dx * s1, y - dy * s1, z);
	glVertex3d(x - dx * s1, y + dy * s1, z - dz * s1);
	glVertex3d(x - dx * s1, y + dy * s1, z);
	glVertex3d(x + dx * s1, y + dy * s1, z - dz * s1);
	glVertex3d(x + dx * s1, y + dy * s1, z);

	glVertex3d(x - dx * s1, y - dy * s1, z - dz * s1);
	glVertex3d(x - dx * s1, y + dy * s1, z - dz * s1);
	glVertex3d(x + dx * s1, y - dy * s1, z - dz * s1);
	glVertex3d(x + dx * s1, y + dy * s1, z - dz * s1);
	glVertex3d(x - dx * s1, y - dy * s1, z);
	glVertex3d(x - dx * s1, y + dy * s1, z);
	glVertex3d(x + dx * s1, y - dy * s1, z);
	glVertex3d(x + dx * s1, y + dy * s1, z);
	glEnd();
}

void NxyzImage::DrawBase(bool DrawStockMark, double Brightness, char* string, bool highlight)
{
	if (!IsVisible)
		return;
	CalcSValue();
	s1 = s * Axes1Size;
	glPushAttrib(GL_LINE_BIT | GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	if (DrawStockMark)
	{
		DrawStockMarker(x, y, z);
	}
	// Axis
	glLineWidth(2.);
	glColor3d(Brightness, 0.5 * Brightness, 0.5 * Brightness);
	DrawLetter('X', BPoint(x + 1.5 * s1, y, z, 1.)
		, BPoint(0.15 * s1, 0., 0., 0.)
		, BPoint(0., 0.2 * s1, 0., 0.));
	glBegin(GL_LINES);
	glVertex3d(x, y, z);
	glVertex3d(x + s1, y, z);
	glEnd();
	glColor3d(0.5 * Brightness, 1 * Brightness, 0.5 * Brightness);
	DrawLetter('Y', BPoint(x, y + 1.5 * s1, z, 1.)
		, BPoint(0.2 * s1, 0., 0., 0.)
		, BPoint(0., 0.2 * s1, 0., 0.));
	glBegin(GL_LINES);
	glVertex3d(x, y, z);
	glVertex3d(x, y + s1, z);
	glColor3d(0.5 * Brightness, 0.5 * Brightness, 1 * Brightness);
	glVertex3d(x, y, z);
	glVertex3d(x, y, z + s1);
	glEnd();

	glPopAttrib();
}

void NxyzImage::CalcSValue()
{
	GLint RenderMode = 0;
	glGetIntegerv(GL_RENDER_MODE, &RenderMode);
	if (RenderMode == GL_RENDER)
	{
		double xs, ys, zs;
		BaseRender::GetPixelDim(xs, ys, zs);
		s = 12. * xs;
	}
}

void NxyzImage::DrawZero(bool DrawStockMark, double Brightness, char *string, bool highlight)
{
	if(!IsVisible)
		return;
	CalcSValue();
	s1 = s * Axes1Size;
	glPushAttrib(GL_LINE_BIT | GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT);
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);

		glLineWidth(2.);
		if(DrawStockMark)
			DrawStockMarker(0., 0., 0.);

		DrawAxesZero(s, string, highlight);

	glPopAttrib();
}

// изображение осей
void NxyzImage::Draw(bool DrawStockMark, double Brightness)
{
	if(!IsVisible)
		return;
	CalcSValue();
	s1 = s * Axes1Size;
	glPushAttrib(GL_LINE_BIT | GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT);
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		if (IsVisibleBase)
		{
			if(DrawStockMark)
			{
				DrawStockMarker(x, y, z);
			}
// Axis
			glLineWidth(2.);
			glColor3d(Brightness,0.5*Brightness,0.5*Brightness);
			DrawLetter('X', BPoint(x + 1.5*s1, y, z, 1.)
						, BPoint(0.15 * s1, 0., 0., 0.)
						, BPoint(0., 0.2 * s1, 0., 0.));
			glBegin(GL_LINES);
				glVertex3d(x,y,z);
				glVertex3d(x+s1,y,z);
			glEnd();
			glColor3d(0.5*Brightness,1*Brightness,0.5*Brightness);
			DrawLetter('Y', BPoint(x, y + 1.5*s1, z, 1.)
					, BPoint(0.2 * s1, 0., 0., 0.)
					, BPoint(0., 0.2 * s1, 0., 0.));
			glBegin(GL_LINES);
				glVertex3d(x,y,z);
				glVertex3d(x,y+s1,z);
				glColor3d(0.5*Brightness,0.5*Brightness,1*Brightness);
				glVertex3d(x,y,z);
				glVertex3d(x,y,z+s1);
			glEnd();
		}	
		if(ZeroPos & ZPS_AXIS)
		{
			if(DrawStockMark)
				DrawStockMarker(0., 0., 0.);

			DrawAxesZero(s, nullptr, false);
		}

	glPopAttrib();
}

// задание размеров осей координат
void NxyzImage::SetSizes(double s1, double s2, double s3, double s4, double s5)
{
	Axes1Size = fabs(s1);
	Axes2_xSize = fabs(s2);
	Axes2_ySize = fabs(s3);
	Axes2_zSizePl = fabs(s4);
	Axes2_zSizeMin = fabs(s5);
}

// задание параметров отображения осей координат
void NxyzImage::SetParam(BOOL is_vis,			/*TRUE*/
						 BOOL is_visbase,		/*TRUE*/
						 int iZeroPos,			/*TRUE*/
						 BOOL abs_size,			/*FALSE*/
						 BOOL is_viszer_xy)		/*FALSE*/
{
	IsVisible = is_vis;
	IsVisibleBase = is_visbase;
	ZeroPos = iZeroPos;
	AbsoluteSize = abs_size;
	IsVisibleZeroXYZ = is_viszer_xy;
}

// отображение осей в точке начала координат 
void NxyzImage::DrawAxesZero(double s, char* string, bool highlight)
{
	GLdouble s1, s2, s3, s4; // длины осей координат
	// определение размеров осей координат 
	// в зависимости от того, в отосительных или 
	// бсолютных координатах они отображаются
	if (!AbsoluteSize)
	{
		s1 = s * Axes2_xSize;
		s2 = s * Axes2_ySize;
		s3 = s * Axes2_zSizePl;
		s4 = s * Axes2_zSizeMin;
	}
	else
	{
		s1 = Axes2_xSize;
		s2 = Axes2_ySize;
		s3 = Axes2_zSizePl;
		s4 = Axes2_zSizeMin;
	}

	// отображение осевых линий
	glBegin(GL_LINES);
	glColor3d(1., 0., 0.);
	glVertex3d(0., 0., 0.);
	glVertex3d(s1, 0., 0.);
	glColor3d(0., 1, 0.);
	glVertex3d(0., 0., 0.);
	glVertex3d(0., s2, 0.);
	glColor3d(0., 0., 1);
	glVertex3d(0., 0., 0.);
	glVertex3d(0., 0., s3);
	glVertex3d(0., 0., 0.);
	glVertex3d(0., 0., -s4);
	glEnd();

	// рисуем название координат X и Y и Z
	if (IsVisibleZeroXYZ)
	{
		glBegin(GL_LINES);
		glColor3d(1, 0.5, 0.5);
		glVertex3d(s1 + 1. * s * t_s, 0.35 * s * t_s, 0); // X
		glVertex3d(s1 + 1.7 * s * t_s, -0.35 * s * t_s, 0);
		glVertex3d(s1 + 1.7 * s * t_s, 0.35 * s * t_s, 0);
		glVertex3d(s1 + 1. * s * t_s, -0.35 * s * t_s, 0);

		glColor3d(0.5, 1, 0.5);
		glVertex3d(0, s2 + 1. * s * t_s, 0); // Y
		glVertex3d(0, s2 + 1.35 * s * t_s, 0);
		glVertex3d(0, s2 + 1.35 * s * t_s, 0);
		glVertex3d(-0.35 * s * t_s, s2 + 1.7 * s * t_s, 0);
		glVertex3d(0, s2 + 1.35 * s * t_s, 0);
		glVertex3d(0.35 * s * t_s, s2 + 1.7 * s * t_s, 0);

		glColor3d(0.5, 0.5, 1);
		if (s3 > 0.)
		{
			glVertex3d(0, 0.35 * s * t_s, s3 + 1. * s * t_s);// Z
			glVertex3d(0, -0.35 * s * t_s, s3 + 1. * s * t_s);
			glVertex3d(0, -0.35 * s * t_s, s3 + 1. * s * t_s);
			glVertex3d(0, 0.35 * s * t_s, s3 + 1.7 * s * t_s);
			glVertex3d(0, 0.35 * s * t_s, s3 + 1.7 * s * t_s);
			glVertex3d(0, -0.35 * s * t_s, s3 + 1.7 * s * t_s);
		}
		if (s3 == 0. && s4 > 0.)
		{
			glVertex3d(0, -0.35 * s * t_s, -(s4 + 1. * s * t_s));// -Z
			glVertex3d(0, 0.35 * s * t_s, -(s4 + 1. * s * t_s));
			glVertex3d(0, 0.35 * s * t_s, -(s4 + 1. * s * t_s));
			glVertex3d(0, -0.35 * s * t_s, -(s4 + 1.7 * s * t_s));
			glVertex3d(0, -0.35 * s * t_s, -(s4 + 1.7 * s * t_s));
			glVertex3d(0, 0.35 * s * t_s, -(s4 + 1.7 * s * t_s));

			glVertex3d(0, -1. * s * t_s, -(s4 + 1.35 * s * t_s));
			glVertex3d(0, -0.6 * s * t_s, -(s4 + 1.35 * s * t_s));
		}

		glEnd();
	}

	// рисуем текст
	if (string)
	{
		if (highlight)
			glColor3d(1., 0., 0.);
		else
			glColor3d(0., 0., 0.);

		glListBase(1256);// MainText
		glRasterPos3d(s1 * 0.2, s2 * 0.2, s3 * 0.2);
		glCallLists((GLsizei)strlen(string), GL_UNSIGNED_BYTE, string);
	}
}

void NxyzImage::DrawLetter(char  Letter, const BPoint & P0, const BPoint & Ex, const BPoint & Ey)
{
	// P0 - center point, Ex - X axis half vector, Ey - Y axis half vector
	switch(Letter)
	{
	case 'X':
		{
			BPoint Plb = P0 - Ex - Ey;
			BPoint Prb = P0 + Ex - Ey;
			BPoint Plt = P0 - Ex + Ey;
			BPoint Prt = P0 + Ex + Ey;
			glBegin(GL_LINES);			
			glVertex3d(Plb.GetX(), Plb.GetY(), Plb.GetZ());
			glVertex3d(Prt.GetX(), Prt.GetY(), Prt.GetZ());
			glVertex3d(Prb.GetX(), Prb.GetY(), Prb.GetZ());
			glVertex3d(Plt.GetX(), Plt.GetY(), Plt.GetZ());
			glEnd();
		}
		break;
	case 'Y':
		{
			BPoint Pmb = P0 - Ey;
			BPoint Plt = P0 - Ex + Ey;
			BPoint Prt = P0 + Ex + Ey;
			glBegin(GL_LINES);			
			glVertex3d(Pmb.GetX(), Pmb.GetY(), Pmb.GetZ());
			glVertex3d(P0.GetX(), P0.GetY(), P0.GetZ());
			glVertex3d(P0.GetX(), P0.GetY(), P0.GetZ());
			glVertex3d(Prt.GetX(), Prt.GetY(), Prt.GetZ());
			glVertex3d(P0.GetX(), P0.GetY(), P0.GetZ());
			glVertex3d(Plt.GetX(), Plt.GetY(), Plt.GetZ());
			glEnd();
		}
		break;
	}
}
