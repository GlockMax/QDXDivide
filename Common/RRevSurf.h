#pragma once
#include "BaseRender.h"
#include "RenderMode.h"
#include <vector>

class OPENGLRENDER_API RRevSurf :
	public BaseRender
{
public:
	RRevSurf(bool Edges = true, double Width = 1.);
	virtual ~RRevSurf();

	bool Create(int PointsNumber, GLfloat *ctlarray);
	bool Draw(RenderMode GlobRMode, RenderMode LocRMode);
//	bool Close() override { return true; }
	void StoreInBuf(GLfloat *pBuf, const BPoint &P, const BPoint &N, int StripInd) const;
	void MarkEmpty() { VBOSize = 0; }
protected:
	GLuint VBO;
	int VBOSize; // in points
	int SurfSize;
	const int STRIDE = sizeof(GLfloat) * 6;
	double AngleStep;
	bool SharpEdges;
	double LineWidth;
};

