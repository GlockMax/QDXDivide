//#include "windows.h"
#include "stdafx.h"
#include "stdio.h"
#include "assert.h"
#include "GLee.h"
#include "GL/gl.h"
#include "GL/glu.h"
#include "BPointsBuf.h"
#include "BConst.h"
#include "RLines.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

bool RLines::DrawWithNamesException = false;


RLines::RLines(void)
: ColorBold(1., 0., 1., 1.)
{
	GeomTolerance = 0.01 * BConst::GetScale();
	for(int i = 0; i < Count; ++i)
		VBO[i] = 0;
	DrawNodes = false;
	UseAxes = false;
	AxisLength = 30.;// In pixels
	AxisScale = 1.;
	AxisThickness = 1.;
}

RLines::~RLines(void)
{
	glDeleteBuffers(Count, VBO);
}

bool RLines::Create(bool ScaleSensitive, INT_PTR ElemsNum, const NColor &CBold)
{
	if(!FeaturesPresent)
		return false;
	BaseRender::Create(ScaleSensitive);
	ColorBold = CBold;
	glBindBuffer(GL_ARRAY_BUFFER, VBO[LV_VERTEX]);
	if(!glIsBuffer(VBO[LV_VERTEX]))
	{
		glDeleteBuffers(Count, VBO);
		glGenBuffers(Count, VBO);
	}
	RElems.SetSize(ElemsNum);
	ElemsInd = 0;
	Axes.reserve(UseAxes ? ElemsNum * 2 : 0);
	Elems.reserve(ElemsNum);
	return true;
}

bool RLines::AddElem(const NCadrGeom *pGeom, const BPoint &AxisVec, int FirstName, int SecondName, int k, const NColor &Col, const NColor &AxisCol)
{
	if(!FeaturesPresent)
		return false;
	if(!pGeom->HaveGeom5())
		return true;

	int PtsStartInd = 1;
	if(Elems.size() == 0)// First cadr
		PtsStartInd = 0;

	auto SInd = Elems.size() - PtsStartInd;

	BPointsBuf Pts;
	pGeom->GetPolyLine(Pts, GeomTolerance, true);
	if (UseAxes)
	{
		Axes.push_back(LineVertex(Pts.GetPoint(0), AxisCol));
		Axes.push_back(LineVertex(AxisVec, AxisCol));
	}
	for(int i = PtsStartInd; i < Pts.GetSize(); ++i)
	{
		Elems.push_back(LineVertex(Pts.GetPoint(i), Col));
	}
	RElems.SetElem(ElemsInd, SInd, Pts.GetSize(), FirstName, SecondName, k);

	++ElemsInd;
	assert(INT_PTR(ElemsInd) <= RElems.GetSize());
	return true;
}

int RLines::StoreSample(BYTE*& pBuf)
{
	pBuf = nullptr;
	int ElemsCount = 0;
	if (RElems.GetSize() > 0)
	{
		int LastInd = min(100, int(RElems.GetSize() - 1));
		ElemsCount = RElems.GetElem(LastInd).StartInd + RElems.GetElem(LastInd).Size - RElems.GetElem(0).StartInd;
	}
	if (ElemsCount > 0)
	{
		if (FeaturesPresent)
		{
			glBindBuffer(GL_ARRAY_BUFFER, VBO[LV_VERTEX]);
			if (glIsBuffer(VBO[LV_VERTEX]))
			{
				LineVertex* ABPtr = (LineVertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
				if (ABPtr != NULL)
				{
					int Size = ElemsCount * sizeof(LineVertex);
					pBuf = new BYTE[Size];
					memcpy_s(pBuf, Size, ABPtr, Size);
					glUnmapBuffer(GL_ARRAY_BUFFER);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					return Size;
				}
			}
		}
	}
	return 0;
}

bool RLines::DrawInt(bool WithNames, const std::vector<BProgVisInterval> & VisInt, int BoldCadr /*= -1*/, double t)
{
	if(Mapped)
		return false;
	if(!FeaturesPresent)
		return false;

	glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	if(VisInt.empty())
	{
		DrawPart(WithNames, 0, RElems.GetSize() - 1);
	}
	else
	{
		for (std::vector<BProgVisInterval>::size_type iInt = 0; iInt < VisInt.size(); ++iInt)
		{
			const BProgVisInterval &Int = VisInt[iInt];
			auto First = RElems.FindFirst(Int.GetStart());
			auto Last = RElems.FindLast(Int.GetEnd());
			if(Last < 0 || First < 0)
				continue;
			if (Int.GetUseColor())
			{
				glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
				glDisableClientState(GL_COLOR_ARRAY);
				glPushAttrib(GL_CURRENT_BIT);
				glColor4fv(Int.GetColor().array);
			}
			if(t >= 0. && t < 1.)
				--Last;

			DrawPart(WithNames, First, Last);

			// Draw last partial cadr
			if(t > 0. && t < 1.)
			{
				++Last;
				DrawElemPart(Last, t);
			}
			// Draw bold cadr
			if(Int.Inside(BoldCadr))
			{
				bool IsEqual;
				First = RElems.Find(BoldCadr, &IsEqual);
				if(IsEqual)
				{
					glDisableClientState(GL_COLOR_ARRAY);
					glPushAttrib(GL_LINE_BIT | GL_CURRENT_BIT);
					glColor4fv(ColorBold.array);
					glLineWidth(3.);
					DrawWithoutNames(First, First);
					glPopAttrib();
					glEnableClientState(GL_COLOR_ARRAY);
				}
			}
			if (Int.GetUseColor())
			{
				glPopClientAttrib();
				glPopAttrib();
			}

		}
	}

	glPopClientAttrib();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return true;
}
void RLines::DrawPart(bool WithNames, INT_PTR First, INT_PTR Last)
{
	if(WithNames)
		DrawWithNames(First, Last);
	else
		DrawWithoutNames(First, Last);
}
RLines::LineVertex::LineVertex(const BPoint &P, const NColor &Col)
{
	x = (GLfloat)P.GetX();
	y = (GLfloat)P.GetY();
	z = (GLfloat)P.GetZ();
	r = Col.r();
	g = Col.g();
	b = Col.b();

}

bool RLines::Close()
{
	if(!FeaturesPresent)
		return false;
	glBindBuffer(GL_ARRAY_BUFFER, VBO[LV_VERTEX]);
	if (!glIsBuffer(VBO[LV_VERTEX]))
		return false;
	glBufferData(GL_ARRAY_BUFFER, sizeof(LineVertex) * (Elems.size() + 1), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[LV_VERTEX]);
	if (!glIsBuffer(VBO[LV_VERTEX]))
		return false;
	LineVertex* ABPtr = (LineVertex *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	if (ABPtr == NULL)
		return false;
	for (int i = 0; i < Elems.size(); ++i)
		ABPtr[i] = Elems[i];
	Elems.resize(0);
	RElems.SetSize(ElemsInd);
	bool ret = false;
	ret = (glUnmapBuffer(GL_ARRAY_BUFFER) == GL_TRUE);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return ret;

}

void RLines::DrawWithNames(INT_PTR First, INT_PTR Last)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO[LV_VERTEX]);
	if (!glIsBuffer(VBO[LV_VERTEX]))
		return;

	glColorPointer(3, GL_UNSIGNED_BYTE, sizeof(LineVertex), ((char *)NULL) + sizeof(GLfloat) * 3);
	glVertexPointer(3, GL_FLOAT, sizeof(LineVertex), NULL);

	for(auto i = First; i <= Last; ++i)
	{
		const RElem &Elem = RElems.GetElem(i);
		glPushName(Elem.FirstName);
		if(Elem.SecondName > 0)
			glPushName(Elem.SecondName);
		__try
		{
			glDrawArrays(GL_LINE_STRIP, Elem.StartInd, Elem.Size);
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{ 
			DrawWithNamesException = true;
			glPopName();
			if (Elem.SecondName > 0)
				glPopName();
			break;
		}
		if (UseAxes && INT_PTR(Axes.size()) > i * 2 + 1)
		{
			glBegin(GL_LINES);
			const LineVertex &Vert = Axes[i * 2];
			const LineVertex &Vect = Axes[i * 2 + 1];
			glVertex3f(Vert.x, Vert.y, Vert.z);
			glVertex3f(GLfloat(Vert.x + Vect.x * AxisScale), GLfloat(Vert.y + Vect.y * AxisScale), GLfloat(Vert.z + Vect.z * AxisScale));
			glEnd();
		}
		glPopName();
		if(Elem.SecondName > 0)
			glPopName();
	}
}

void RLines::DrawWithoutNames(INT_PTR First, INT_PTR Last)
{
	if(Last < First)
		return;
	glBindBuffer(GL_ARRAY_BUFFER, VBO[LV_VERTEX]);
	if (!glIsBuffer(VBO[LV_VERTEX]))
		return;

	glColorPointer(3, GL_UNSIGNED_BYTE, sizeof(LineVertex), ((char *)NULL) + sizeof(GLfloat) * 3);
	glVertexPointer(3, GL_FLOAT, sizeof(LineVertex), NULL);

	const RElem &LastElem = RElems.GetElem(Last);
	const RElem &FirstElem = RElems.GetElem(First);
	int Count = LastElem.StartInd + LastElem.Size - FirstElem.StartInd;
	if(Count <= 1)
		return;
//	glPushAttrib(GL_LIGHTING_BIT);// This doesn't work (sometimes) on NVIDIA GeForce 8500 GT ???
	glShadeModel(GL_FLAT);
	glDrawArrays(GL_LINE_STRIP, FirstElem.StartInd, Count);
	if (DrawNodes)
	{
		for (INT_PTR Cur = First; Cur <= Last; ++Cur)
		{
			glDrawArrays(GL_POINTS, RElems.GetElem(Cur).StartInd, 1);
		}
	}
	if (UseAxes)
	{
		double x, y, z;
		BaseRender::GetPixelDim(x, y, z);
		AxisScale = x * AxisLength;
		glLineWidth(GLfloat(AxisThickness));
		GLuint LocVBO;
		glGenBuffers(1, &LocVBO);
		glBindBuffer(GL_ARRAY_BUFFER, LocVBO);
		if (glIsBuffer(LocVBO) && Axes.size() > 0)
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(LineVertex) * Axes.size(), NULL, GL_DYNAMIC_DRAW);
			LineVertex *Vertexes = (LineVertex *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
			glColorPointer(3, GL_UNSIGNED_BYTE, sizeof(LineVertex), ((char *)NULL) + sizeof(GLfloat) * 3);
			glVertexPointer(3, GL_FLOAT, sizeof(LineVertex), NULL);
			for (INT_PTR Cur = First; Cur <= Last; ++Cur)
			{
				auto i = 2 * Cur;
				Vertexes[i] = Axes[i];
				Vertexes[i + 1] = Vertexes[i].Compose(Axes[i + 1], AxisScale);
			}
			glUnmapBuffer(GL_ARRAY_BUFFER);
			glDrawArrays(GL_LINES, GLsizei(First * 2), GLsizei((Last - First + 1) * 2));
			glDeleteBuffers(1, &LocVBO);
		}



	}
	glShadeModel(GL_SMOOTH);
//	glPopAttrib();
}

void RLines::DrawElemPart(INT_PTR Elem, double t)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO[LV_VERTEX]);
	if (!glIsBuffer(VBO[LV_VERTEX]))
		return;

	glColorPointer(3, GL_UNSIGNED_BYTE, sizeof(LineVertex), ((char *)NULL) + sizeof(GLfloat) * 3);
	glVertexPointer(3, GL_FLOAT, sizeof(LineVertex), NULL);

	// Draw complete lines
	const RElem &CurElem = RElems.GetElem(Elem);
	int Count = int((CurElem.Size - 1) * t) + 1;
	if(Count > 1)
		glDrawArrays(GL_LINE_STRIP, CurElem.StartInd, Count);
	// Draw part of the last line
	double tau = t * (CurElem.Size - 1) - double(Count - 1);
	if(tau < 1.e-4)
		return;

	int StartInd = CurElem.StartInd + Count - 1;
	GLfloat t0 = GLfloat(-1000. * tau);
	GLfloat t1 = t0 + 1001.f;
	GLuint TexName;
	GLfloat Texture[8] = { 1., 1., 1., 1.,    0., 0., 0., 0.};
		
	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_TEXTURE_1D);
	glGenTextures(1, &TexName);
	glBindTexture(GL_TEXTURE_1D, TexName);
	glTexImage1D(GL_TEXTURE_1D, 0, 4, 2, 0, GL_RGBA, GL_FLOAT, Texture);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,  GL_MODULATE); 
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glBegin(GL_LINES);
	glTexCoord1f(t0);
	glArrayElement(StartInd);
	glTexCoord1f(t1);
	glArrayElement(StartInd + 1);
	glEnd();
	glDeleteTextures(1, &TexName);
	glPopAttrib();
}

const RLines::LineVertex RLines::LineVertex::Compose(const LineVertex &Vec, double Scale) const
{
	LineVertex New = *this;
	New.x += GLfloat(Vec.x * Scale);
	New.y += GLfloat(Vec.y * Scale);
	New.z += GLfloat(Vec.z * Scale);
	return New;
}
