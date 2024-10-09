#pragma once
#include "NColor.h"
#include "NCadrGeom.h"
#include "BPoint.h"
#include "RElemArray.h"
#include "baserender.h"
#include "BProgVisInterval.h"


class OPENGLRENDER_API RLines :
	public BaseRender
{
public:
	RLines(void);
	virtual ~RLines(void);
	virtual bool Create(bool ScaleSensitive, INT_PTR ElemsNum, const NColor &CBold);
	bool DrawInt(bool WithNames, const std::vector<BProgVisInterval> & VisInt, int BoldCadr = -1, double t = -1.);
	bool Close(void);
	bool AddElem(const NCadrGeom *pGeom, const BPoint &AxisVec, int FirstName, int SecondName, int k, const NColor &Col, const NColor &AxisCol);
	void SetTolerance(double Tol) { GeomTolerance = Tol;}
	void SetUseAxes(bool Use) { UseAxes = Use; }
	bool GetUseAxes() const { return UseAxes; }
	void SetDrawNodes(bool Draw) { DrawNodes = Draw; }
	bool GetDrawNodes() const { return DrawNodes; }
	void SetAxisLength(double Length) { AxisLength = Length; }
	void SetAxisThickness(double Thick) { AxisThickness = Thick; }
	int StoreSample(BYTE*& pBuf);


protected:
	enum LineVBOs
	{
		LV_VERTEX = 0,
	};
public:
	class OPENGLRENDER_API LineVertex
	{
	public:
		LineVertex(const BPoint &P, const NColor &Col);
		LineVertex() {}
		const LineVertex Compose(const LineVertex &Vec, double Scale) const;
	public:
		GLfloat x;
		GLfloat y;
		GLfloat z;
		GLubyte r;
		GLubyte g;
		GLubyte b;
	};
protected:
	static const int Count = 1; // A number of LineVBOs elements
protected:
	void DrawWithNames(INT_PTR First, INT_PTR Last);
	void DrawWithoutNames(INT_PTR First, INT_PTR Last);
	void DrawPart(bool WithNames, INT_PTR First, INT_PTR Last);
	void DrawElemPart(INT_PTR Elem, double t);
protected:
	bool DrawNodes;
	bool UseAxes;
	GLuint VBO[Count];
	RElemArray RElems;
	std::vector<LineVertex> Axes;
	std::vector<LineVertex> Elems;
	size_t ElemsInd;
	class NColor ColorBold;
	double GeomTolerance;
	double AxisLength;
	double AxisScale;
	double AxisThickness;
public:
	static bool DrawWithNamesException;
};
