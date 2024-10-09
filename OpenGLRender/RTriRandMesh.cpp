#include "stdafx.h"
#include "math.h"
#include "NCMComData.h"
#include "ConstDef.h"
#include "BMatr.h"
#include "NObjEnums.h"
#include "RenderMode.h"
#include "GLee.h"
#include "Gl\glu.h"
#include "RTriRandMesh.h"

//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
//#endif

#define BUFFER_OFFSET(i) ((void*)(i))

RTriRandMesh::RTriRandMesh(void) : RTriangles()
{
	ActSizeI = 0;
	PrelSize = 0;
}


RTriRandMesh::~RTriRandMesh(void)
{
	ActSizeI = 0;
}

int RTriRandMesh::GetTriNum(void) const
{
	return ActSizeI / 3;
}

int RTriRandMesh::ExtractTriangle(int Ind, class BPoint &p0, BPoint &p1, BPoint &p2, BPoint &n) const
{
	ExtractTriangle(Ind, p0, p1, p2);
	n = (p2 - p0) % (p1 - p0);
	n = n * (1. / sqrt(n.D2()));
	return 0;
}

int RTriRandMesh::ExtractTriangle(int Ind, BPoint & p0, BPoint & p1, BPoint & p2) const
{
	auto iI = Ind * 3;
	if (iI >= ActSizeI)
		return -1;
	auto K0 = (iI + 1) / PrelPartSize;
	auto K1 = iI % PrelPartSize;
	GLuint k00 = PrelInd[K0][K1++];
	GLfloat *Bufk00 = NP.GetAt(k00) + 3;
	if (Bufk00[0] != Bufk00[0] || Bufk00[1] != Bufk00[1] || Bufk00[2] != Bufk00[2])
		return -1;
	p0.Set(Bufk00[0], Bufk00[1], Bufk00[2], 1.);

	k00 = PrelInd[K0][K1++];
	Bufk00 = NP.GetAt(k00) + 3;
	if (Bufk00[0] != Bufk00[0] || Bufk00[1] != Bufk00[1] || Bufk00[2] != Bufk00[2])
		return -1;
	p1.Set(Bufk00[0], Bufk00[1], Bufk00[2], 1.);

	k00 = PrelInd[K0][K1++];
	Bufk00 = NP.GetAt(k00) + 3;
	if (Bufk00[0] != Bufk00[0] || Bufk00[1] != Bufk00[1] || Bufk00[2] != Bufk00[2])
		return -1;
	p2.Set(Bufk00[0], Bufk00[1], Bufk00[2], 1.);
	if (NCMComData::IsTurnEnabled())
	{
		BMatr MTMatr;
		MTMatr.RotX(0., 0., 0., -90.);
		p0 = p0 * MTMatr;
		p1 = p1 * MTMatr;
		p2 = p2 * MTMatr;
	}
	return 0;
}

int RTriRandMesh::ExtractTriangleInds(int Ind, int* pi0, int* pi1, int* pi2) const
{
	auto iI = Ind * 3;
	if (iI >= ActSizeI)
		return -1;
	auto K0 = (iI + 1) / PrelPartSize;
	auto K1 = iI % PrelPartSize;
	*pi0 = PrelInd[K0][K1++];
	*pi1 = PrelInd[K0][K1++];
	*pi2 = PrelInd[K0][K1++];
	return 0;
}

double RTriRandMesh::RayCasting(const BPoint & P, const BPoint & V, double MinW, BPoint & N, BPoint& v0, BPoint& v1, BPoint& v2) const
{
	double MinT = 1.e12;
	for (int it = 0; it < GetTriNum(); ++it)
	{
		BPoint p0(0), p1(0), p2(0);
		if (ExtractTriangle(it, p0, p1, p2) < 0)
			continue;
		p0 -= P;
		p1 -= P;
		p2 -= P;
		//Calculate normal
		BPoint n = (p1 - p0) % (p2 - p0);
		//BPoint n = (p1 - p0) % (p2 - p0);
		//n = n * ((n * n0 < 0) ? 1. : -1.);// Sign should be inverted according to GQuadRenderMill
		double z = n * V;
		if (fabs(z) < MINAR)
			continue;
		z = 1. / z;
		double t = p0 * n * z;
		if (t < 0. || t > MinT)
			continue;
		BPoint IntP(V * t);
		if (((p1 - p0) % (p0 - IntP)) * n > 0 && IntP.LineD2(p1, p0) > MinW * MinW)
			continue;
		if (((p2 - p1) % (p1 - IntP)) * n > 0 && IntP.LineD2(p2, p1) > MinW * MinW)
			continue;
		if (((p0 - p2) % (p2 - IntP)) * n > 0 && IntP.LineD2(p0, p2) > MinW * MinW)
			continue;
		MinT = t;
		N = n * (-1. / sqrt(n.D2()));
		v0 = p0 + P;
		v1 = p1 + P;
		v2 = p2 + P;
	}
	return MinT;
}

int RTriRandMesh::AddTriInd(int i0, int i1, int i2)
{
	int K0 = (ActSizeI + 1) / PrelPartSize;
	while(K0 >= PrelSize)
	{
		PrelInd[PrelSize++] = new GLuint[PrelPartSize];
	}
	int K1 = ActSizeI % PrelPartSize;
	PrelInd[K0][K1++] = i0;
	PrelInd[K0][K1++] = i1;
	PrelInd[K0][K1++] = i2;
	ActSizeI += 3;
	return ActSizeI;
}

bool RTriRandMesh::DrawTriSurf()
{
	if (VBO[VBO_NP] == 0)
	{
		if (GetActSizeP() == 0)
			return false;
		glGenBuffers(1, &VBO[VBO_NP]);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[VBO_NP]);

		if (!glIsBuffer(VBO[VBO_NP]))
			return false;

		glBufferData(GL_ARRAY_BUFFER, STRIDE * GetActSizeP(), NULL, GL_STATIC_DRAW);
		size_t CurShift = 0;
		size_t i = 0;
		for (size_t CurSize = NP.BaseSize; i < NP.PartsNum - 1; ++i, CurSize *= 2)
		{
			glBufferSubData(GL_ARRAY_BUFFER, STRIDE * CurShift, STRIDE * CurSize, NP.Parts[i]);
			CurShift += CurSize;
		}
		glBufferSubData(GL_ARRAY_BUFFER, STRIDE * CurShift, STRIDE * NP.LastPos, NP.Parts[i]);
	}
	else
		glBindBuffer(GL_ARRAY_BUFFER, VBO[VBO_NP]);

	if (VBO[VBO_IND] == 0)
	{
		if (PrelSize > 0)
		{
			glGenBuffers(1, &VBO[VBO_IND]);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[VBO_IND]);

			if (!glIsBuffer(VBO[VBO_IND]))
				return false;

			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PrelInd[0][0]) * ActSizeI, NULL, GL_STATIC_DRAW);
			for (auto i = 0; i < PrelSize - 1; ++i)
			{
				glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PrelInd[0][0]) * PrelPartSize * i
					, sizeof(PrelInd[0][0]) * PrelPartSize, PrelInd[i]);
			}
			auto Rest = ActSizeI % PrelPartSize;
			if (Rest == 0)
				Rest = PrelPartSize;
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PrelInd[0][0]) * PrelPartSize * (PrelSize - 1)
				, sizeof(PrelInd[0][0]) * Rest, PrelInd[PrelSize - 1]);
		}
	}
	else
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[VBO_IND]);

	glColorPointer(4, GL_UNSIGNED_BYTE, STRIDE, BUFFER_OFFSET(sizeof(float) * 6));// Alfa component from current color used (<- is not true)
	glVertexPointer(3, GL_FLOAT, STRIDE, BUFFER_OFFSET(sizeof(float) * 3));
	glNormalPointer(GL_FLOAT, STRIDE, BUFFER_OFFSET(0));

	glDrawRangeElements(GL_TRIANGLES, 0, GetActSizeP() - 1, ActSizeI, GL_UNSIGNED_INT, 0);

	return true;
}

bool RTriRandMesh::DrawTriEdges(enum RenderMode GlobRMode, const std::vector<unsigned int>& Indexes, const NColor& EdgesColor, double EdgesWidth)
{
	if (Indexes.size() >= 2 && EdgesWidth > 0.01)
	{
		if (VBO[VBO_NP] == 0)
		{
			if (GetActSizeP() == 0)
				return false;
			glGenBuffers(1, &VBO[VBO_NP]);

			glBindBuffer(GL_ARRAY_BUFFER, VBO[VBO_NP]);

			if (!glIsBuffer(VBO[VBO_NP]))
				return false;

			glBufferData(GL_ARRAY_BUFFER, STRIDE * GetActSizeP(), NULL, GL_STATIC_DRAW);
			size_t CurShift = 0;
			size_t i = 0;
			for (size_t CurSize = NP.BaseSize; i < NP.PartsNum - 1; ++i, CurSize *= 2)
			{
				glBufferSubData(GL_ARRAY_BUFFER, STRIDE * CurShift, STRIDE * CurSize, NP.Parts[i]);
				CurShift += CurSize;
			}
			glBufferSubData(GL_ARRAY_BUFFER, STRIDE * CurShift, STRIDE * NP.LastPos, NP.Parts[i]);
		}
		else
			glBindBuffer(GL_ARRAY_BUFFER, VBO[VBO_NP]);

		glVertexPointer(3, GL_FLOAT, STRIDE, BUFFER_OFFSET(sizeof(float) * 3));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
		glLineWidth(float(EdgesWidth));
		glColor3fv(EdgesColor.array);

		glPushName(OG_SKELETON);
		glDrawElements(GL_LINES, GLsizei(Indexes.size()), GL_UNSIGNED_INT, Indexes.data());
		glPopName();

	}
	//	GLenum errCode = glGetError();

	return true;
}
