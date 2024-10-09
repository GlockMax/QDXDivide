#pragma once

#include "OPENGLRENDER_API.h"
#include "GLee.h"
#include "GL\GL.h"
#include "assert.h"
#include "Shader.h"
#include "RTranslucentProg.h"
#include "concurrent_queue.h"

#pragma warning (disable : 4251)

const double BR_ScaleRange = 1.4;

class OPENGLRENDER_API BaseRender
{
public:
	virtual ~BaseRender(void);
	bool Create(bool ScaleSensitive);
//	virtual bool Close(void) = 0;
	static double GetModelScale(void);
	static void GetPixelDim(double &x, double &y, double &z);
	static bool IsVBOEnabled(void);
	static void DrawBox(const class BBox &Box, int Mode = 0);
	static void DrawBall(const class BPoint &Center, double R, bool Screen = false);
	static class BMatr GetUnprojectMatr();
	static const class BPoint GetShiftVec();
	static bool IsMultisampleSupported(HDC hDC, int *pPixelFormat);
	static bool CheckFramebuffer(void);
	static bool SetShader(enum RenderMode RM);
	static void SetWFMode(void);
	static void RemoveWFMode(void);
	static void ClearVBOs();
	static void UnbindArrays();
	static void DrawCone(double RInt0, double RExt0, double RInt1, double RExt1, double H, int Slices, bool Edges = false);
	static void SetOGLDiag(bool multisampleOK, bool shadersOK, bool vboOK);
	static int GetOGLDiagCode();
	static void DrawFlatArrow(const BPoint& Start, const BPoint& End, double PixLength = 25., double PixHalfWidth = 3.);
	static const BPoint GetViewDir(void);
	//	static int LoadGLTextures(char *path, unsigned int &texture);
//---
protected:
	bool FeaturesPresent;
 	bool ScaleSens;
	double CurrentScale;
	bool Mapped;
	static concurrency::concurrent_queue<GLuint> VBO2Delete;
	static bool multisampleOK;
	static bool vboOK;
	static bool shadersOK;

protected:
	BaseRender(void);
	bool NeedReCreate(void);
public:
	static PhongShader PhShader;
	static GlitShader GlShader;
	static RTranslucentProg TranslucentPr;
};
