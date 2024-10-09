#include "stdafx.h"
#include "GLee.h"
#include "GL\GL.h"
#include "ConstDef.h"
#include "NCMComData.h"
#include "MBSPCell.h"

MBSPCell::MBSPCell(int iix, int iiy, double iXmin, double iXmax, double iYmin, double iYmax)
{
	Shift = 0;
	ix = iix;
	iy = iiy;
	Xmin = iXmin;
	Xmax = iXmax;
	Ymin = iYmin;
	Ymax = iYmax;
	YIndex = NCMComData::IsTurnEnabled() ? 2 : 1; // index for the coordinate to be compared with Ymax and Ymin
	for (int i = 0; i < 4; ++i)
		Sides[i].reserve(3);
}

void MBSPCell::DrawSharpEdges() const
{
	glBegin(GL_LINES);
	for(size_t i = 0; i < LinesDCEL.Size(); i += 2)
	{
		const auto& El0 = LinesDCEL[i];
		glVertex3fv(El0.GetPoint());
		const auto& El1 = LinesDCEL[i + 1];
		glVertex3fv(El1.GetPoint());
	}
	glEnd();
}

void MBSPCell::FillFromBDCEL(BDCEL& El, bool Sharp, bool Error)
{
	// calculate size of LinesDCEL
	int Size = 0;
	for (int i = 0; i < El.GetEdgesCount(); i += 2)
	{
		if (Error)
		{
			if (El.IsBorderEdge(i))
			{
				++Size;
				continue;
			}
		}
		if (Sharp)
		{
			if (El.PairedHalfEdge(i) == i + 1)
			{
				if (El.IsSharpEdge(i))
					++Size;
			}
		}
	}
	if (Size == 0)
		return;
	LinesDCEL.Resize(Size * 2);
	UINT MainInd = 0;
	for (size_t StartInd = 0; StartInd < El.GetEdgesCount(); StartInd += 2)
	{
		if (El.IsMarkedEdge(int(StartInd)) || El.IsDummyEdge(int(StartInd)) || El.IsBorderEdge(int(StartInd)))
			continue;
		if (!El.IsSharpEdge(int(StartInd)))
			continue;
		// forward
		UINT Cur = UINT(StartInd);
		El.MarkEdgePair(Cur);
		ReplaceEdge(El, MainInd, Cur);// edge start is not bound point
		UINT PrevMainInd = UINT(MainInd);
		MainInd += 2;
		UINT ForStart = El.FindNextSharp(Cur);
		for (UINT Next = ForStart; Next != UINT_MAX; Cur = Next, Next = El.FindNextSharp(Next))
		{
			El.MarkEdgePair(Next);
			ReplaceEdge(El, MainInd, Next);// edge start is not bound point
			LinesDCEL.Connect(MainInd, MainInd - 2);
			MainInd += 2;
		}
		// process end point
		ProcEndPoint(MainInd - 1);
		// backward
		UINT BackStart = El.FindNextSharp(StartInd + 1);
		for (UINT Next = BackStart; Next != UINT_MAX; Next = El.FindNextSharp(Next))
		{
			El.MarkEdgePair(Next);
			ReplaceEdge(El, MainInd, Next);// edge start is not bound point
			LinesDCEL.Connect(MainInd, PrevMainInd);
			PrevMainInd = MainInd;
			MainInd += 2;
		}
		// process end point
		ProcEndPoint(PrevMainInd + ((BackStart == UINT_MAX) ? 0 : 1));// 1 if cycle body works; 0 otherwize
	}
}

void MBSPCell::ReplaceEdge(const BDCEL& El, size_t OutInd, size_t DcelInd)
{
	const float* pS = El.Get3fvByVn(El.GetESVn(int(DcelInd)));
	const float* pE = El.Get3fvByVn(El.GetEEVn(int(DcelInd)));
	LinesDCEL.ReplaceEdge(OutInd, pS, pE, UINT_MAX, UINT_MAX);
}

void MBSPCell::ProcEndPoint(UINT LinesInd)
{
	const float* pPoint = LinesDCEL[LinesInd].GetPoint();
	if (fabs(pPoint[0] - Xmin) < MIND)
		Sides[0].emplace_back(pPoint, LinesInd);
	else if (fabs(pPoint[0] - Xmax) < MIND)
		Sides[2].emplace_back(pPoint, LinesInd);
	else if (fabs(pPoint[YIndex] - Ymin) < MIND)
		Sides[1].emplace_back(pPoint, LinesInd);
	else if (fabs(pPoint[YIndex] - Ymax) < MIND)
		Sides[3].emplace_back(pPoint, LinesInd);
}

void MBSPCell::Bind(bool XShift, MBSPCell* pNextCell, BLinesDCEL& Result) const
{
	if (pNextCell == (MBSPCell*)QR_EMPTY)
		return;
	const auto& CurSide = Sides[XShift ? 2 : 3];
	const auto& NextSide = pNextCell->Sides[XShift ? 0 : 1];
	// find edges to bind
	for each(auto& Cur in CurSide)
		for each (auto & Next in NextSide)
		{
			double Dist2 = (Cur.pP[0] - Next.pP[0]) * (Cur.pP[0] - Next.pP[0]) + (Cur.pP[1] - Next.pP[1]) * (Cur.pP[1] - Next.pP[1]) + (Cur.pP[2] - Next.pP[2]) * (Cur.pP[2] - Next.pP[2]);
			if (Dist2 < MIND * MIND)
				Result.Bind(Cur.SEdgesInd + Shift, Next.SEdgesInd + pNextCell->Shift);
		}
}
