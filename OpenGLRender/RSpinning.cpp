#include "stdafx.h"
#include "GLee.h"
#include "ConstDef.h"
#include "BaseRender.h"
#include "RenderMode.h"
#include "RSpinning.h"

RSpinning::RSpinning()
{
	Triangles = nullptr;
	PrevMatr.Set0();
}

RSpinning::~RSpinning()
{
	delete[] Triangles;
}

void RSpinning::CreateRenders()
{
	MakeTriMesh(Contours, 120);
}

void RSpinning::Draw(enum RenderMode GlobRMode, enum RenderMode ExtRMode, bool WFrame) const
{
	const RenderMode ActRMode = ExtRMode;

	if (GlobRMode == RM_TRANSLUCENT && ActRMode != RM_TRANSLUCENT)
		return;
	if (GlobRMode != RM_TRANSLUCENT && ActRMode == RM_TRANSLUCENT)
		return;

	if (WFrame)
		BaseRender::SetWFMode();
	else
		BaseRender::SetShader(ActRMode);

	glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	constexpr int Stride = 6;
	glInterleavedArrays(GL_N3F_V3F, 0, Triangles + 1);

	GLsizei Count = min(3 * long(Triangles[0]), 64000000);
	glDrawArrays(GL_TRIANGLES, 0, Count);

	if (WFrame)
		BaseRender::RemoveWFMode();

	glPopClientAttrib();
}

void RSpinning::StorePrevState(const BMatr& RelMatr)
{
	PrevMatr = RelMatr;
}

bool RSpinning::CheckPrevState(const BMatr& RelMatr)
{
	return PrevMatr.IsApprSame(RelMatr);
}

void RSpinning::MakeTriMesh(const std::list<BClrContour>& Contours, size_t SectorsNum)
{
	constexpr size_t Stride = 6;
	BMatr StepMatr;
	StepMatr.RotY(0., 0., 0., 360. / SectorsNum);

	delete[] Triangles;
	size_t TriNum = 0;
	for each (const auto & Cont in Contours)
		TriNum += (Cont.size() - 1) * 2 * SectorsNum;
	Triangles = new float[1 + TriNum * 3 * Stride];
	Triangles[0] = static_cast<float>(TriNum);

	for each (const auto & Cont in Contours)
	{
		if (Cont.empty())
			continue;
		size_t TriInd = 0;
		for (auto itn = Cont.begin(), itc = itn++; itn != Cont.end(); ++itc, ++itn)
		{
			BPoint P0 = *itc;
			BPoint P1 = *itn;
			Gabar.Expand(P0.GetZ(), P0.GetX(), P0.GetY());
			Gabar.Expand(P1.GetZ(), P1.GetX(), P1.GetY());
			BPoint VecNext = (*itc - *itn).Unit();
			BPoint Norm = (VecNext * BMatr().RotZ(0., 0., 0., 90.)).Unit();
			for (int i = 0; i < SectorsNum; ++i, TriInd += 2 * 3 * Stride)
			{
				BPoint P2 = P0 * StepMatr;
				BPoint P3 = P1 * StepMatr;

				BPoint Norm1 = Norm * StepMatr;
				// Triangle must have right orientation (ccw from normal end)
				StorePoint(Triangles + TriInd + 0 * Stride + 1, P0, Norm);
				StorePoint(Triangles + TriInd + 1 * Stride + 1, P2, Norm1);
				StorePoint(Triangles + TriInd + 2 * Stride + 1, P1, Norm);

				StorePoint(Triangles + TriInd + 3 * Stride + 1, P1, Norm);
				StorePoint(Triangles + TriInd + 4 * Stride + 1, P2, Norm1);
				StorePoint(Triangles + TriInd + 5 * Stride + 1, P3, Norm1);

				Gabar.Expand(P2.GetZ(), P2.GetX(), P2.GetY());
				Gabar.Expand(P3.GetZ(), P3.GetX(), P3.GetY());

				Norm = Norm1;
				P0 = P2;
				P1 = P3;
			}
		}
	}
}

const BPoint RSpinning::RayCasting(const BPoint& P, const BPoint& V, double MinW, BPoint& N) const
{
	// Find the intersection point nearest to P in the V direction
		// N - external normal in P (from Stl)
	if (!Triangles)
		return BPoint(0., 0., 0., 1.);
	double MinT = 1.e12;
	for (int TriInd = 0; TriInd < int(Triangles[0]); ++TriInd)
	{
		int i = TriInd * 18 + 1;
		BPoint n0(Triangles[i + 0], Triangles[i + 1], Triangles[i + 2], 0.);
		BPoint p0(BPoint(Triangles[i + 3], Triangles[i + 4], Triangles[i + 5], 1.) - P);
		BPoint p1(BPoint(Triangles[i + 9], Triangles[i + 10], Triangles[i + 11], 1.) - P);
		BPoint p2(BPoint(Triangles[i + 15], Triangles[i + 16], Triangles[i + 17], 1.) - P);
		//Calculate normal
		BPoint n = (p1 - p0) % (p2 - p0);
		n = n * ((n * n0 < 0) ? -1. : 1.);
		double z = n * V;
		if (fabs(z) < MINAR)
			continue;
		z = 1. / z;
		double t = (p0 * n) * z;
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
		N = n0;
	}
	if (MinT >= 1.e6)
		return 	BPoint(0., 0., 0., -1.);

	if (V * N > 0.)
		N = N * (-1.);
	return V * MinT + P;
}


void RSpinning::StorePoint(float* Dest, const BPoint& P, const BPoint& N)
{
	Dest[0] = float(N.GetZ());
	Dest[1] = float(N.GetX());
	Dest[2] = float(N.GetY());
	Dest[3] = float(P.GetZ());
	Dest[4] = float(P.GetX());
	Dest[5] = float(P.GetY());
}

