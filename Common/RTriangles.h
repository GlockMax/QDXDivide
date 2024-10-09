#pragma once
#include "OPENGLRENDER_API.h"
#include "BPartialArray.h"
#include "BaseRender.h"


class OPENGLRENDER_API RTriangles : public BaseRender
{
public:
	RTriangles(void);
	RTriangles(const RTriangles &inTriangles);
	virtual ~RTriangles(void);
	void DeleteVBO();
	virtual int GetTriNum(void) const = 0;
	virtual int ExtractTriangle(int Ind, class BPoint &p0, BPoint &p1, BPoint &p2, BPoint &n) const = 0;
	GLfloat *GetNextNP(void);
	GLfloat *GetAtNP(int Ind) const;
	int GetStride(void) const;
	int GetActSizeP(void) const { return int(NP.GetSize()); }
	GLfloat* Open4Read(size_t& Size);
	const GLfloat* GetpC0() { return (GLfloat*)NP.GetPart(0); }
	void Transform(const class BMatr &Tr);
protected:
	enum
	{
		VBO_NP = 0,
		VBO_IND = 1
	};
	static const int VBOsNum = 2;
	static const int STRIDE = 32;
	static const int PrelPartSize = 1000 * 3; // Should be divisible by 3
	static const int PrelIndSize = 256;

	GLuint VBO[VBOsNum];
	typedef GLfloat Vertex[STRIDE / sizeof(GLfloat)];
	BPartialArray<Vertex> NP;
	unsigned int *PrelInd[PrelIndSize];
	int PrelSize; // A number of PrelInd elements
	int ActSizeI;// An actual  number of indexes
};

