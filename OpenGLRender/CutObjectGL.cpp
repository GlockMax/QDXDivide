#include "StdAfx.h"
#include "BaseRender.h"
#include "CutObjectGL.h"

CutGL::CutGL(void)
{
	memset(m_CPlane, 0, sizeof(double) * 4);
	is_BeginTest = false;
	is_EndTest = false;
}

CutGL::~CutGL(void)
{
}

void CutGL::InvertCPlane()
{
	m_CPlane[0] *= -1.;
	m_CPlane[1] *= -1.;
	m_CPlane[2] *= -1.;
	m_CPlane[3] *= -1.;
	glClipPlane(GL_CLIP_PLANE5, m_CPlane);
}

//Устанавливает плоскость сечения
void CutGL::SetCPlane(double *eq_cp = NULL)
{
 	memcpy(m_CPlane, eq_cp, sizeof(double)*4);
	glClipPlane(GL_CLIP_PLANE5, m_CPlane);

	is_BeginTest = false;
// После этой функции должна быть прорисовка объекта
}

void CutGL::BeginTest1()
{
	is_BeginTest = true;
	glClearStencil(0);
	glStencilMask(3);
	glStencilFunc(GL_ALWAYS, 0, 0);
	glEnable(GL_STENCIL_TEST);
	glClear(GL_STENCIL_BUFFER_BIT);
	glStencilMask(1);
	glStencilOp(GL_KEEP, GL_INVERT, GL_INVERT);
	glDrawBuffer(GL_NONE);
	glDisable(GL_DEPTH_TEST);
	is_EndTest = false;
}

void CutGL::BeginTest2()
{
	InvertCPlane();
	glStencilFunc(GL_EQUAL, 1, 1);
	glStencilMask(2);
	glStencilOp(GL_KEEP, GL_INVERT, GL_INVERT);
}

void CutGL::EndTest()
{
	if (is_BeginTest)
	{
		glDisable(GL_STENCIL_TEST);
		is_EndTest = true;
		is_BeginTest = false;
	}
	glDrawBuffer(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	InvertCPlane();
}

void CutGL::CloseCPlane()
{
	memset(m_CPlane, 0, sizeof(double)* 4);
}

void CutGL::DrawCPlane(const NColor &Color)
{
	double CPlane[4];
	glGetClipPlane(GL_CLIP_PLANE5, CPlane);

	if ((is_EndTest) && (CPlane[2] != 0))
	{
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();

			GLint vMat[4];
			GLdouble pMat[16], mMat[16];
			glGetDoublev(GL_PROJECTION_MATRIX, pMat);
			glGetDoublev(GL_MODELVIEW_MATRIX, mMat);
			glGetIntegerv(GL_VIEWPORT, vMat);

			GLdouble z_min, x_min, y_min,z_max, x_max, y_max;
			gluUnProject(vMat[0], vMat[1], 0, mMat, pMat, vMat, &x_min, &y_min, &z_min);
			gluUnProject(vMat[2]+vMat[0],vMat[3]+vMat[1],1,mMat,pMat,vMat,&x_max,&y_max,&z_max);
			
			z_max = max(x_max,y_max);
			z_min = min(x_min,y_min);
			const int c_point = 4;
			GLdouble x_win[c_point] = { z_min, z_min, z_max, z_max };
			GLdouble y_win[c_point] = { z_min, z_max, z_max, z_min };
			GLdouble z_win[c_point];

			for (int i = 0; i < c_point; i++)
				z_win[i] = -((x_win[i] * CPlane[0]) + (y_win[i] * CPlane[1]) + CPlane[3]) / CPlane[2];
			
			glPushAttrib(GL_ENABLE_BIT);
			glDisable(GL_CLIP_PLANE5);
			glEnable(GL_STENCIL_TEST);
			glEnable(GL_DEPTH_TEST);
			glDisable(GL_LIGHTING);
			glStencilFunc(GL_EQUAL, 3, 3);
			glColor4fv(Color.array);
			glNormal3d(CPlane[0], CPlane[1], CPlane[2]);
			glBegin(GL_QUADS);
				for (int i=0;i<4;i++)
					glVertex3d(x_win[i],y_win[i],z_win[i]);
			glEnd();
			glFinish();
			glPopAttrib();
			glPopMatrix();
	}

}

void CutGL::EndDrawCPlane(void)
{
	is_BeginTest = false;
	is_EndTest = false;
}
