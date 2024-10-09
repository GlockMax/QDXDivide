#include "StdAfx.h"
#include "BMatr.h"
#include "BPointsBuf.h"
#include "NSurfSettings.h"
#include "NLine.h"
#include "RRevSurf.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define BUFFER_OFFSET(i) ((void*)(i))

RRevSurf::RRevSurf(bool Edges/* = true*/, double Width/* = 1.*/)
{
	VBO = 0;
	VBOSize = -1;
	AngleStep = 10.;
	SharpEdges = Edges;
	LineWidth = Width;
	SurfSize = 0;
}

RRevSurf::~RRevSurf()
{
	if (VBO != 0)
	{
		VBO2Delete.push(VBO);// Needed to ensure deleting of a VBO in the main thread only
		VBO = 0;
	}
}

void RRevSurf::StoreInBuf(GLfloat *Buf, const BPoint &P, const BPoint &E, int StripInd) const
{
	double BaseAngle = AngleStep * StripInd;
	BMatr ML; ML.RotZ(BPoint(0., 0., 0., 1.), BaseAngle - AngleStep * 0.5);
	BMatr MR; MR.RotZ(BPoint(0., 0., 0., 1.), BaseAngle + AngleStep * 0.5);
	BPoint Q[2] = { P * ML, P * MR };
	BPoint N[2] = { E * ML, E * MR };
	for (int k = 0; k < 2; ++k)
	{
		Buf[0] = GLfloat(N[k].GetX());
		Buf[1] = GLfloat(N[k].GetY());
		Buf[2] = GLfloat(N[k].GetZ());
		Buf[3] = GLfloat(Q[k].GetX());
		Buf[4] = GLfloat(Q[k].GetY());
		Buf[5] = GLfloat(Q[k].GetZ());
		Buf += 6;
	}

}


bool RRevSurf::Create(int PointsNumber, GLfloat *ctlarray)
{
	// ctlarray in XZ plane up->down with X > 0
	// rotate around Z axis
	if (PointsNumber < 2)
		return false;
	glDeleteBuffers(1, &VBO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	if (!glIsBuffer(VBO))
	{
		glDeleteBuffers(1, &VBO);
		return false;
	}

	double MaxX = 0;
	std::vector<BPointsBuf> Pts(PointsNumber / 3);

	VBOSize = 0;// in points
	for (int k = 0; k < PointsNumber / 3; ++k)
	{
		NLine Line;
		for (int l = 0; l < 4; ++l)
		{
			int ind = k * 12 + l * 4;
			Line.p[l].Set(ctlarray[ind + 0], ctlarray[ind + 1], ctlarray[ind + 2], ctlarray[ind + 3]);
			MaxX = fmax(MaxX, ctlarray[ind + 0] / ctlarray[ind + 3]);
		}
		Line.Bernstain2Ball();
		Line.Approximate(NSurfSettings::GetTolerance() / 5., Pts[k], true);
		VBOSize += Pts[k].GetSize() * 2;// - 2 to remove tangent vectors
		// Add tangent vectors
		Pts[k].AddPoint(Line.Tangent(0).Unit());
		Pts[k].AddPoint(Line.Tangent(1).Unit());
	}

	const int ApprNum = NSurfSettings::GetNumAproxLine(MaxX, 360.);
	const int StripCount = 5 * ApprNum;
	AngleStep = 360. / StripCount;
	VBOSize = (VBOSize + 2) * StripCount;// +2 - 2 exta points to connect stripes
	SurfSize = VBOSize;
	const int lLim = PointsNumber / 3;
	VBOSize += lLim * StripCount;
	glBufferData(GL_ARRAY_BUFFER, STRIDE * VBOSize, NULL, GL_STATIC_DRAW);// two points for each point of each line
	void *pMap = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	if (pMap == nullptr)
	{
		glDeleteBuffers(1, &VBO);
		return false;
	}

	GLfloat *pBuffer = static_cast<GLfloat *>(pMap);
	BMatr MatrY90;
	MatrY90.RotY(BPoint(0., 0., 0., 1.), 90.);
	int EdgeInd = 0;
	GLfloat* pEdgeBase = pBuffer + SurfSize * 6;
	for (int StripInd = 0; StripInd < StripCount; ++StripInd)
	{
		const bool Even = (StripInd % 2 == 0);
		const int FirstL = Even ? 0 : 1;
		const int LastL = Even ? 1 : 0;
		const int Sign = Even ? 1 : -1;
		for (int f = 0; f < lLim; ++f)
		{
			const int k = Even ? f : lLim - 1 - f;
			const int LastInd = Pts[k].GetSize() - 2 - 1;// exclude tangent vectors
			BPoint P = Pts[k].GetPoint(LastInd * FirstL);
			P.Norm();
			BPoint N = Pts[k].GetPoint(LastInd + 1 + FirstL);
			StoreInBuf(pBuffer, P, N * MatrY90, StripInd);
			// store points for edges
			memcpy_s(pEdgeBase + EdgeInd * 3, 3 * sizeof(float), pBuffer + 3, 3 * sizeof(float));
			memcpy_s(pEdgeBase + (EdgeInd + 1) * 3, 3 * sizeof(float), pBuffer + 3 + 6, 3 * sizeof(float));
			EdgeInd += 2;
			pBuffer += 2 * 6;
			for (int g = 1; g < LastInd; ++g)
			{
				const int l = Even ? g : LastInd - g;
				BPoint PPrev = P;
				P = Pts[k].GetPoint(l);
				P.Norm();
				BPoint PNext = Pts[k].GetPoint(l + Sign);
				PNext.Norm();
				N = (((P - PPrev).Unit() + (PNext - P).Unit()).Unit()) * Sign;
				StoreInBuf(pBuffer, P, N * MatrY90, StripInd);
				pBuffer += 2 * 6;
			}
			P = Pts[k].GetPoint(LastInd * LastL);
			P.Norm();
			N = Pts[k].GetPoint(LastInd + 1 + LastL);
			StoreInBuf(pBuffer, P, N * MatrY90, StripInd);
			pBuffer += 2 * 6;
		}
		// add 2 points to connect stripes
		memcpy_s(pBuffer, 6 * sizeof(float), pBuffer - 6, 6 * sizeof(float));
		pBuffer += 6;
		memcpy_s(pBuffer, 6 * sizeof(float), pBuffer - 6, 6 * sizeof(float));
		pBuffer += 6;
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);
	return true;
}

bool RRevSurf::Draw(RenderMode GlobRMode, RenderMode LocRMode)
{
	if (VBOSize == 0)
		return true;
	if (VBOSize < 0 || !glIsBuffer(VBO))
		return false;
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, STRIDE, BUFFER_OFFSET(sizeof(float) * 3));
	glNormalPointer(GL_FLOAT, STRIDE, BUFFER_OFFSET(0));

	glPushAttrib(GL_ENABLE_BIT);
	if (SharpEdges)
	{
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1., 1.);
	}
	if (GlobRMode == RM_TRANSLUCENT && LocRMode == RM_TRANSLUCENT || LocRMode != RM_TRANSLUCENT && GlobRMode != RM_TRANSLUCENT)
	{
		glDrawArrays(GL_QUAD_STRIP, 0, SurfSize);
	}
	if (SharpEdges && GlobRMode != RM_TRANSLUCENT)
	{
		glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(unsigned long long(STRIDE) * SurfSize));
		SetWFMode();
		glPushAttrib(GL_LINE_BIT | GL_CURRENT_BIT);
		glColor3f(0., 0., 0.);
		glLineWidth(GLfloat(LineWidth));
		glDrawArrays(GL_LINES, 0, (VBOSize - SurfSize) * 2);
		glPopAttrib();
		RemoveWFMode();
	}
	glPopAttrib();
	glPopClientAttrib();

	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	return true;
}
