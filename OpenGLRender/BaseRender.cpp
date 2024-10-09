#include "stdafx.h"
#include "math.h"
#include "GLee.h"
#include "GL\GL.h"
#include "GL\GLu.h"
#include "BBox.h"
#include "glbmp-1.1\glbmp.h"
#include "RenderMode.h"
#include "BaseRender.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

concurrency::concurrent_queue<GLuint> BaseRender::VBO2Delete;
bool BaseRender::multisampleOK = true;
bool BaseRender::vboOK = true;
bool BaseRender::shadersOK = true;

/*
Параметры визуализации объекта
1. Shader
2. Сглаживание
3. Кромки
4. Цвет кромок
5. Толщина линии для кромок
6. Glitter
7. WFrame
8. Прозрачность
9. Один цвет или разные цвета
10. Цвет (может быть задан снаружи или использован внутренний)
11. Толщина линий
12. Текстура ?
13. Z - test
14. Видимость (виден ли)
15. Угол для сглаживания
16. Multisampling
*/

PhongShader BaseRender::PhShader;
GlitShader BaseRender::GlShader;
RTranslucentProg BaseRender::TranslucentPr;

BaseRender::BaseRender(void)
{
	FeaturesPresent = IsVBOEnabled();
	ScaleSens = false;
	CurrentScale = 0.;
	Mapped = false;
}

BaseRender::~BaseRender(void)
{
}

bool BaseRender::Create(bool ScaleSensitive)
{
	ScaleSens = ScaleSensitive;

	return true;
}

double BaseRender::GetModelScale(void)
{
	GLdouble modelMatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	return sqrt(modelMatrix[0] * modelMatrix[0] + modelMatrix[1] * modelMatrix[1] + modelMatrix[2] * modelMatrix[2]);
}

bool BaseRender::NeedReCreate(void)
{
	double Scale = GetModelScale();
	return (Scale > BR_ScaleRange * CurrentScale) || (Scale < CurrentScale / BR_ScaleRange);
}

bool BaseRender::IsVBOEnabled(void)
{
	return (GLEE_VERSION_1_5 == GL_TRUE);
}

void BaseRender::DrawBall(const class BPoint &Center, double R, bool Screen /*= false*/)
{
	if(!Center.IsPoint())
		return;
	double Scale = 1.;
	if(Screen)
	{
		double x, y, z;
		GetPixelDim(x, y, z);
		Scale = x;
	}
	const int SlicesNum = 15;
	GLUquadricObj *qobj = gluNewQuadric();
	glPushMatrix();

	glTranslated(Center.GetX(), Center.GetY(), Center.GetZ());
	gluSphere(qobj, R * Scale, SlicesNum, SlicesNum);
	glPopMatrix();
	gluDeleteQuadric(qobj);
}

void BaseRender::DrawBox(const BBox &Box, int Mode)
{
	if(!Box.IsDefined())
		return;
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	Shader::SaveState();
	Shader::ShaderOFF();

	GLfloat Buf[8 * 3];
	Buf[ 0 * 3 + 0] = GLfloat(Box.GetMinPoint().GetX());
	Buf[ 0 * 3 + 1] = GLfloat(Box.GetMinPoint().GetY());
	Buf[ 0 * 3 + 2] = GLfloat(Box.GetMinPoint().GetZ());

	Buf[ 1 * 3 + 0] = GLfloat(Box.GetMaxPoint().GetX());
	Buf[ 1 * 3 + 1] = GLfloat(Box.GetMinPoint().GetY());
	Buf[ 1 * 3 + 2] = GLfloat(Box.GetMinPoint().GetZ());

	Buf[ 2 * 3 + 0] = GLfloat(Box.GetMaxPoint().GetX());
	Buf[ 2 * 3 + 1] = GLfloat(Box.GetMaxPoint().GetY());
	Buf[ 2 * 3 + 2] = GLfloat(Box.GetMinPoint().GetZ());

	Buf[ 3 * 3 + 0] = GLfloat(Box.GetMinPoint().GetX());
	Buf[ 3 * 3 + 1] = GLfloat(Box.GetMaxPoint().GetY());
	Buf[ 3 * 3 + 2] = GLfloat(Box.GetMinPoint().GetZ());

	Buf[ 4 * 3 + 0] = GLfloat(Box.GetMinPoint().GetX());
	Buf[ 4 * 3 + 1] = GLfloat(Box.GetMinPoint().GetY());
	Buf[ 4 * 3 + 2] = GLfloat(Box.GetMaxPoint().GetZ());

	Buf[ 5 * 3 + 0] = GLfloat(Box.GetMaxPoint().GetX());
	Buf[ 5 * 3 + 1] = GLfloat(Box.GetMinPoint().GetY());
	Buf[ 5 * 3 + 2] = GLfloat(Box.GetMaxPoint().GetZ());

	Buf[ 6 * 3 + 0] = GLfloat(Box.GetMaxPoint().GetX());
	Buf[ 6 * 3 + 1] = GLfloat(Box.GetMaxPoint().GetY());
	Buf[ 6 * 3 + 2] = GLfloat(Box.GetMaxPoint().GetZ());

	Buf[ 7 * 3 + 0] = GLfloat(Box.GetMinPoint().GetX());
	Buf[ 7 * 3 + 1] = GLfloat(Box.GetMaxPoint().GetY());
	Buf[ 7 * 3 + 2] = GLfloat(Box.GetMaxPoint().GetZ());

	glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, Buf);

	glBegin(GL_LINE_STRIP);
	glArrayElement(0);
	glArrayElement(1);
	glArrayElement(2);
	glArrayElement(3);
	glArrayElement(0);
	glArrayElement(4);
	glArrayElement(5);
	glArrayElement(6);
	glArrayElement(7);
	glArrayElement(4);
	glEnd();

	glBegin(GL_LINES);
	glArrayElement(1);
	glArrayElement(5);
	glArrayElement(2);
	glArrayElement(6);
	glArrayElement(3);
	glArrayElement(7);
	glEnd();
	if(Mode & 1)
	{
		glLineWidth(1.);
		glBegin(GL_LINES);
		glArrayElement(4);
		glArrayElement(6);
		glArrayElement(5);
		glArrayElement(7);
		glEnd();
	}
	glPopClientAttrib();
	Shader::RestoreState();
	glPopAttrib();
}

void BaseRender::DrawCone(double RInt0, double RExt0, double RInt1, double RExt1, double H, int Slices, bool Edges/* = false*/)
{
	double HalfAngleDeg = 180. / Slices;
	BMatr M2;
	M2.RotZ(BPoint(0., 0., 0., 1.), 2. * HalfAngleDeg);

	BPoint PExt0(RExt0, 0., 0., 1.);
	BPoint PExt1(RExt1, 0., H, 1.);
	BPoint PInt0(RInt0, 0., 0., 1.);
	BPoint PInt1(RInt1, 0., H, 1.);
	if (Edges)
	{
		for (int i = 0; i < Slices; ++i)
		{
			glRotated(2. * HalfAngleDeg, 0., 0., 1.);
			glBegin(GL_LINES);
				glVertex3dv(PExt0.GetArray());
				glVertex3dv((PExt0 * M2).GetArray());
				glVertex3dv(PExt1.GetArray());
				glVertex3dv((PExt1 * M2).GetArray());
				if (!(RInt0 == 0. && RInt1 == 0.))
				{
					glVertex3dv(PInt0.GetArray());
					glVertex3dv((PInt0 * M2).GetArray());
					glVertex3dv(PInt1.GetArray());
					glVertex3dv((PInt1 * M2).GetArray());
				}
			glEnd();
		}
	}
	else
	{
		BPoint NSurf(1., 0., 0., 0.);
		BPoint NZ(0., 0., 1., 0.);
		for (int i = 0; i < Slices; ++i)
		{
			glRotated(2. * HalfAngleDeg, 0., 0., 1.);
			glBegin(GL_QUADS);
			glNormal3dv(NSurf.GetArray());
			glVertex3dv(PExt0.GetArray());
			glNormal3dv((NSurf * M2).GetArray());
			glVertex3dv((PExt0 * M2).GetArray());
			glNormal3dv((NSurf * M2).GetArray());
			glVertex3dv((PExt1 * M2).GetArray());
			glNormal3dv(NSurf.GetArray());
			glVertex3dv(PExt1.GetArray());

			glNormal3dv(NZ.GetArray());
			glVertex3dv(PExt0.GetArray());
			glVertex3dv((PExt0 * M2).GetArray());
			glVertex3dv((PInt0 * M2).GetArray());
			glVertex3dv(PInt0.GetArray());
			glVertex3dv(PExt1.GetArray());
			glVertex3dv((PExt1 * M2).GetArray());
			glVertex3dv((PInt1 * M2).GetArray());
			glVertex3dv(PInt1.GetArray());

			if (!(RInt0 == 0. && RInt1 == 0.))
			{
				glNormal3dv(NSurf.GetArray());
				glVertex3dv(PInt0.GetArray());
				glNormal3dv((NSurf * M2).GetArray());
				glVertex3dv((PInt0 * M2).GetArray());
				glNormal3dv((NSurf * M2).GetArray());
				glVertex3dv((PInt1 * M2).GetArray());
				glNormal3dv(NSurf.GetArray());
				glVertex3dv(PInt1.GetArray());

			}
			glEnd();
		}
	}
}

void BaseRender::SetOGLDiag(bool multisampleOK, bool shadersOK, bool vboOK)
{
	BaseRender::multisampleOK = multisampleOK;
	BaseRender::shadersOK = shadersOK;
	BaseRender::vboOK = vboOK;
}

int BaseRender::GetOGLDiagCode()
{
	return multisampleOK ? 0 : 1 + shadersOK ? 0 : 2 + vboOK ? 0 : 4;
}

const BPoint BaseRender::GetViewDir()
{
	GLint viewport[4];
	GLdouble projMatrix[16], modelMatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	glGetIntegerv(GL_VIEWPORT, viewport);
	GLdouble x = 0., y = 0., z = 0.;
	gluUnProject(viewport[0], viewport[1], 0., modelMatrix, projMatrix, viewport, &x, &y, &z);
	BPoint P(x, y, z, 1.);
	gluUnProject(viewport[0], viewport[1], 1., modelMatrix, projMatrix, viewport, &x, &y, &z);
	P = BPoint(x, y, z, 1.) - P;
	double d = sqrt(P.D2());
	if (d > 1.e-6)
		d = 1. / d;
	else
		d = 1.;
	return P * d;
}

void BaseRender::DrawFlatArrow(const BPoint& Start, const BPoint& End, double PixLength/* = 25.*/, double PixHalfWidth/* = 3.*/)
{
	glBegin(GL_LINES);
	glVertex3dv(Start.GetArray());
	glVertex3dv(End.GetArray());
	glEnd();

	double magn[3];
	GetPixelDim(magn[0], magn[1], magn[2]);
	const double Scale = magn[0];
	const double Length = PixLength * Scale;
	const double HalfWidth = PixHalfWidth * Scale;
	const BPoint Dir = End - Start;
	if (Dir.D2() < Length * Length)
		return;
	BPoint Dir1 = Dir;
	Dir1.Unit();
	BPoint Norm = GetViewDir() % Dir;
	Norm.Unit();
	const BPoint P[3] = { End, End - Dir1 * Length + Norm * HalfWidth, End - Dir1 * Length - Norm * HalfWidth };

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES);
	for (unsigned int i = 0; i < 3; ++i)
		glVertex3dv(P[i].GetArray());
	glEnd();
}

void BaseRender::GetPixelDim(double &x, double &y, double &z)
{
// Returns pixel dimensions in model coordinates in x,y,z vars
	GLdouble x0, y0, z0, x1, y1, z1;
	GLint viewport[4];
	GLdouble projMatrix[16],modelMatrix[16];
	glMatrixMode(GL_MODELVIEW);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	glGetIntegerv(GL_VIEWPORT, viewport);
	gluUnProject(0., 0., 0., modelMatrix, projMatrix, viewport, &x0, &y0, &z0); 
	gluUnProject(1., 0., 0., modelMatrix, projMatrix, viewport, &x1, &y1, &z1); 
	x = (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0) + (z1 - z0) * (z1 - z0);
	gluUnProject(0., 1., 0., modelMatrix, projMatrix, viewport, &x1, &y1, &z1); 
	y = (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0) + (z1 - z0) * (z1 - z0);
	gluUnProject(0., 0., 1., modelMatrix, projMatrix, viewport, &x1, &y1, &z1); 
	z = (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0) + (z1 - z0) * (z1 - z0);
	if(x > 0)
		x = sqrt(x);
	if(y > 0)
		y = sqrt(y);
	if(z > 0)
		z = sqrt(z);
}
//---добавлено 25.02.2008, GL_SPHERE_MAP текстурирование
//int BaseRender::LoadGLTextures(char *path, GLuint &texture)                // Загрузить картинки и создать текстуры
//{
//	glbmp_t TextureImage[1]; // Выделим место для хранения текстур   
//
//	if(!glbmp_LoadBitmap(path, 0, &TextureImage[0]))
//		return FALSE;
//
//	glGenTextures(1, &texture);  										   
//	
//	glBindTexture(GL_TEXTURE_2D, texture);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[0].width,
//                    TextureImage[0].width, GL_RGB, GL_UNSIGNED_BYTE,
//                   TextureImage[0].rgb_data);
//
//	glbmp_FreeBitmap(&TextureImage[0]);
//	return TRUE; // Вернем статус
//}

BMatr BaseRender::GetUnprojectMatr()
{
// Get matrix which maps screen coordinates to model coordinates
	BMatr r;
	GLint viewport[4];
	GLdouble projMatrix[16],modelMatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX ,modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX ,projMatrix);
	glGetIntegerv(GL_VIEWPORT ,viewport);

//	point.y = viewport[3] - point.y;
	double x,y,z;
	gluUnProject(0.,0.,0.,modelMatrix, projMatrix,viewport,&x,&y,&z);
	BPoint p0(x,y,z,1.);
	gluUnProject(1.,0.,0.,modelMatrix, projMatrix,viewport,&x,&y,&z);
	BPoint p(x,y,z,1.);
	r.SetRow(0,p-p0);
	gluUnProject(0.,1.,0.,modelMatrix, projMatrix,viewport,&x,&y,&z);
	p.Set(x,y,z,1.);
	r.SetRow(1,p-p0);
	gluUnProject(0.,0.,1.,modelMatrix, projMatrix,viewport,&x,&y,&z);
	p.Set(x,y,z,1.);
	r.SetRow(2,p-p0);
	p.Set(0.,0.,0.,1.);
	r.SetRow(3,p);
	return r;
}

const BPoint BaseRender::GetShiftVec()
{
// Get vector for the minimal shift in screen Z direction to the viewer
	BMatr r;
	GLint viewport[4];
	GLdouble projMatrix[16],modelMatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX ,modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX ,projMatrix);
	glGetIntegerv(GL_VIEWPORT ,viewport);

//	point.y = viewport[3] - point.y;
	double x, y, z;
	gluUnProject(0., 0., 1.e-12, modelMatrix, projMatrix,viewport, &x, &y, &z);
	BPoint p0(x, y, z, 1.);
	gluUnProject(0., 0., 0., modelMatrix, projMatrix,viewport,&x,&y,&z);
	BPoint p(x, y, z, 1.);
	return p - p0;
}

bool BaseRender::IsMultisampleSupported(HDC hDC, int *pPixelFormat)
{
	bool arbMultisampleSupported = false;
// Multisampling
//	return false;
	if(GLEE_ARB_multisample == GL_TRUE)
	{
		if(wglChoosePixelFormatARB)
		{
			BOOL valid;
			UINT NumFormats;
			float fAttributes[] = {0,0};

			// Эти атрибуты – биты, которые мы хотим протестировать в нашем типе

			int iAttributes[] = {
			WGL_SAMPLES_ARB, 4,            // проверка на 4x тип
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE, // Истинна, если формат пикселя может быть использован в окне
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE, // Истинна, если поддерживается OpenGL
			WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB, // Полная аппаратная поддержка
			WGL_COLOR_BITS_ARB, 24,          // Цветность
			WGL_ALPHA_BITS_ARB, 8,           // Размерность альфа-канала
			WGL_DEPTH_BITS_ARB, 24,          // Глубина буфера глубины
			WGL_STENCIL_BITS_ARB, 8,         // Глубина буфера шаблона
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE,  // Истина, если используется двойная буферизация
			WGL_ACCUM_BITS_ARB, 32,			// Для zoom window
			WGL_SAMPLE_BUFFERS_ARB, GL_TRUE, // Что мы и хотим
			0,0};
			// Сначала посмотрим, сможем ли мы получить формат пикселя для 4x типа
			const int MaxFormats = 32;
			int PixelFormats[MaxFormats];

			valid = wglChoosePixelFormatARB(hDC, iAttributes, fAttributes, MaxFormats, PixelFormats, &NumFormats);

			// Если вернулось True, и наш счетчик форматов не меньше 1
			if (valid && NumFormats >= 1)
			{
				arbMultisampleSupported  = true;
				*pPixelFormat = PixelFormats[0];
				return arbMultisampleSupported;
			}

			// Формат пикселя с 4x выборкой отсутствует, проверяем на 2x тип
			iAttributes[1] = 2;
			valid = wglChoosePixelFormatARB(hDC, iAttributes, fAttributes, MaxFormats, PixelFormats, &NumFormats);
			if (valid && NumFormats >= 1)
			{
				arbMultisampleSupported  = true;
				*pPixelFormat = PixelFormats[0];
				return arbMultisampleSupported;
			}
		}
		return arbMultisampleSupported;
	}
	return arbMultisampleSupported;
}

bool BaseRender::CheckFramebuffer()
{
	return (glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		&& (glCheckFramebufferStatus(GL_READ_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

bool BaseRender::SetShader(RenderMode RM)
{
	switch(RM)
	{
	case RM_SHADE:
		PhShader.ShaderON();
		return true;
	case RM_GLITTER:
		GlShader.ShaderON();
		return true;
	case RM_TRANSLUCENT:
		return true;
	default:
		return false;
	}
	return false;
}

void BaseRender::SetWFMode(void)
{
	glPushAttrib(GL_ENABLE_BIT | GL_POLYGON_BIT );
	glDisable(GL_LIGHTING);
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	Shader::SaveState();
	Shader::ShaderOFF();
}

void BaseRender::RemoveWFMode(void)
{
	glPopAttrib();
	Shader::RestoreState();
}

void BaseRender::ClearVBOs()
{// Do it in the main thread only
	GLuint n;
	while (VBO2Delete.try_pop(n))
		glDeleteBuffers(1, &n);
}

void BaseRender::UnbindArrays()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
