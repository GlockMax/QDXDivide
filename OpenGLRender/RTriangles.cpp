#include "stdafx.h"
#include "BPoint.h"
#include "GLee.h"
#include "RTriangles.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

RTriangles::RTriangles(void)
{
	for(int i = 0; i < VBOsNum; ++i)
		VBO[i] = 0;
	for(int i = 0; i < PrelIndSize; ++i)
		PrelInd[i] = nullptr;
}

RTriangles::RTriangles(const RTriangles &inTriangles) : NP(inTriangles.NP)
{
	PrelSize = inTriangles.PrelSize; // A number of PrelInd elements
	ActSizeI = inTriangles.ActSizeI;// An actual  number of indexes
	for (int i = 0; i < PrelIndSize; ++i)
	{
		if (inTriangles.PrelInd[i] != nullptr)
		{
			PrelInd[i] = new GLuint[PrelPartSize];
			memcpy(PrelInd[i], inTriangles.PrelInd[i], PrelPartSize * sizeof(PrelInd[i][0]));
		}
		else
			PrelInd[i] = nullptr;
	}
	for (int i = 0; i < VBOsNum; ++i)
		VBO[i] = 0;
}

RTriangles::~RTriangles(void)
{
	DeleteVBO();
	for(int i = 0; i < PrelIndSize; ++i)
		delete[] PrelInd[i];
}

void RTriangles::DeleteVBO()
{
	for (int i = 0; i < VBOsNum; ++i)
	{
		if (VBO[i] != 0)
		{
			VBO2Delete.push(VBO[i]);// Needed to ensure deleting of a VBO in the main thread only
			VBO[i] = 0;
		}
	}
}

GLfloat *RTriangles::GetNextNP(void) { return NP.Next(); }
GLfloat *RTriangles::GetAtNP(int Ind) const { return NP.GetAt(Ind); }
int RTriangles::GetStride(void) const { return STRIDE; }

GLfloat *RTriangles::Open4Read(size_t &Size)
{
	if (!NP.IsJoined())
		NP.JoinParts();
	Size = NP.GetSize();
	return (GLfloat *)NP.GetPart(0);
}

void RTriangles::Transform(const BMatr &Tr)
{
	int Size = int(NP.GetSize());
	for (int i = 0; i < Size; ++i)
	{
		GLfloat* BUF = GetAtNP(i);
		BPoint V(BUF[0], BUF[1], BUF[2], 0.);
		V = V * Tr;
		BUF[0] = static_cast<float>(V.GetX());
		BUF[1] = static_cast<float>(V.GetY());
		BUF[2] = static_cast<float>(V.GetZ());
		BPoint P(BUF[3], BUF[4], BUF[5], 1.);
		P = P * Tr;
		BUF[3] = static_cast<float>(P.GetX());
		BUF[4] = static_cast<float>(P.GetY());
		BUF[5] = static_cast<float>(P.GetZ());
	}
}
