#include "stdafx.h"
#include "ConstDef.h"
#include "BBox.h"
#include "MBSPForest.h"
#include "SOCyl.h"
#include "SOSphere.h"
#include "MFace.h"
#include "MElemIDProc.h"
#include "SOPlane.h"
#include "BDynBitArray.h"
#include "MCubeMask.h"
#include "stdlib.h"
#include "DXRasterDisk.h"
#include "MBSPNode.h"
#include "DXDirection3X.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

DXDirection3X::DXDirection3X(int ILayersNum) : DXDirection()
{
	LayersNum = ILayersNum;
	typedef DX_DEPTH * PDX_DEPTH;
	Depth = new PDX_DEPTH[LayersNum];
	typedef DX_ID * PDX_ID;
	IDs = new PDX_ID[LayersNum];
	for(int i = 0; i < LayersNum; ++i)
	{
		Depth[i] = (DX_DEPTH *)NULL;
		IDs[i] = (DX_ID *)NULL;
	}
}


DXDirection3X::~DXDirection3X(void)
{
	for(int i = 0; i < LayersNum; ++i)
	{
		delete[] Depth[i];
		delete[] IDs[i];
	}
	delete[] Depth;
	delete[] IDs;
}

bool DXDirection3X::Init(const DXSubModel *ipSubModel, DX_Dir NewDir, int s0, int s1, double St0, double St1, int s0Big, int s1Big, double v0, double v1, double vv)
{
	DXDirection::Init(ipSubModel, NewDir, s0, s1, St0, St1, s0Big, s1Big, v0, v1, vv);
	for(int i = 0; i < LayersNum; ++i)
	{
		delete Depth[i];
		Depth[i] = new DX_DEPTH[s0 * s1];
		if(Depth[i] == NULL)
			return false;
		delete IDs[i];
		IDs[i] = new DX_ID[s0 * s1];
		if(IDs[i] == NULL)
			return false;
	}
	return true;
}

void DXDirection3X::Clear(void)
{
	if(!IsDefined())
		return;
	for(int i = 0; i < LayersNum; ++i)
	{
		DX_DEPTH DVal = DX_DEPTH_MAX;
		for(int k = 0; k < 2; ++k, ++i)
		{
			DX_DEPTH * DLayer = Depth[i];
			//for(int i0 = 0; i0 < n0; ++i0)
			for(int i0 = SubWin.ixStart; i0 < SubWin.ixEnd; ++i0)
			{
				DX_DEPTH *DCol = DLayer + i0 * n1;
				//for(int i1 = 0; i1 < n1; ++i1)
				for(int i1 = SubWin.iyStart; i1 < SubWin.iyEnd; ++i1)	
				    DCol[i1] = DVal;
			}
			DVal = DX_DEPTH_MIN;
		}
	}
}

void DXDirection3X::ClearAllValues(double Val, double ShiftVal)
{
	if(!IsDefined())
		return;
	BPoint PlaneNormal;
	BPoint TPlanePoint;
	BPoint BPlanePoint;
	switch(Dir)
	{
	case Z:
		PlaneNormal.Set(0., 0., -1., 0.);
		TPlanePoint.Set(0., 0., Val + ShiftVal, 1.);
		BPlanePoint.Set(0., 0., ShiftVal, 1.);
		break;
	case X:
		PlaneNormal.Set(-1., 0., 0., 0.);
		TPlanePoint.Set(Val + ShiftVal, 0., 0., 1.);
		BPlanePoint.Set(ShiftVal, 0., 0., 1.);
		break;
	case Y:
		PlaneNormal.Set(0., -1., 0., 0.);
		TPlanePoint.Set(0., Val + ShiftVal, 0., 1.);
		BPlanePoint.Set(0., ShiftVal, 0., 1.);
		break;
	}
	ElemID IDup = 0;
	MElemIDProc::SetSurfID(&IDup, MFOREST.AddOrigSurfInside(new SOPlane(PlaneNormal, TPlanePoint)));
	ElemID IDdown = 0;
	MElemIDProc::SetSurfID(&IDdown, MFOREST.AddOrigSurfInside(new SOPlane(PlaneNormal * (-1.), BPlanePoint)));
	cadrID Cadr(-1);
	for(int i = 0; i < LayersNum; ++i)
	{
		DX_DEPTH DVal = DX_DEPTH(0.);
		DX_ID IVal = DX_ID(IDdown, Cadr);
		for(int k = 0; k < 2; ++k, ++i)
		{
			DX_DEPTH * DLayer = Depth[i];
			DX_ID * ILayer = IDs[i];
			for(int i0 = 0; i0 < n0; ++i0)
			{
				DX_DEPTH *DCol = DLayer + i0 * n1;
				DX_ID *ICol = ILayer + i0 * n1;
				for(int i1 = 0; i1 < n1; ++i1)
				{
					DCol[i1] = DVal;
					ICol[i1] = IVal;
				}
			}
			DVal = DX_DEPTH(Val);
			IVal = DX_ID(IDup, Cadr);
		}
	}
}

bool DXDirection3X::IsDefined() const
{
	for(int i = 0; i < LayersNum; ++i)
	{
		if(Depth[i] == NULL)
			return false;
		if(IDs[i] == NULL)
			return false;
	}
	return true;
}

void DXDirection3X::MarkUsedOrig(BDynBitArray & Used)
{
	for(int k = 0; k < LayersNum; ++k)
	{
		for(int i0 = 0; i0 < n0; ++i0)
		{
			DX_ID *pColumnI = GetColumnI(k, i0);
			for(int i1 = 0; i1 < n1; ++i1)
			{
				ElemID ElID = (pColumnI++)->Elem;
				int SurfNum = MFOREST.GetIDProc()->GetSurfID(ElID);
				if(SurfNum >= 0 && SurfNum != ID_EMPTY)
					Used.SetTrueAt(SurfNum);
			}
		}
	}
}

void DXDirection3X::InvertDepth(int Layer)
{
	for(int i0 = 0; i0 < n0; ++i0)
	{
		DX_DEPTH *pColumnD = GetColumnD(Layer, i0);
		for(int i1 = 0; i1 < n1; ++i1)
		{
			*pColumnD *= -1.;
			++pColumnD;
		}
	}
}

int DXDirection3X::StoreFace(const MFace * pFace, DX_ID PairID)
{
	if(!pFace)
		return 0;
	MBody::RecalcID(*pFace, PairID.Elem);
	int n = pFace->GetCount();

	//BPoint Min(0),Max(0);
	//pFace->GetGabar(Min,Max);
	//if(Min.GetZ() >= MaxPoint.GetZ())		return 0;
	//if(Max.GetZ() <= MinPoint.GetZ())		return 0;
	//if(Min.GetX() >= MaxPoint.GetX())		return 0;
	//if(Max.GetX() <= MinPoint.GetX())		return 0;
	//if(Min.GetY() >= MaxPoint.GetY())		return 0;
	//if(Max.GetY() <= MinPoint.GetY())		return 0;
	const BPoint &N = pFace->GetPlane().GetNormal();
	int Layer = ((N * Shift).GetZ() < 0.) ? 0 : 1;
	bool FindMax = (Layer == 1);
	switch(n)
	{
	case 3:
		RastTriangle(*pFace->GetPoint(0), *pFace->GetPoint(1), *pFace->GetPoint(2),
			N, FindMax, Layer, PairID);
		break;
	case 4:
		InsRect(*pFace->GetPoint(0), *pFace->GetPoint(1), *pFace->GetPoint(2), *pFace->GetPoint(3), 
			N, FindMax, Layer, PairID);
		break;
	default:
		{
			for(int i = 2; i < pFace->GetCount(); ++i)
			{
				RastTriangle(*pFace->GetPoint(0), *pFace->GetPoint(i - 1), *pFace->GetPoint(i),
					N, FindMax, Layer, PairID);
			}
		}
		break;
	}
	return 0;
}

DX_ID DXDirection3X::RastTriangle(const BPoint & P0v, const BPoint & P1v, const BPoint & P2v, const BPoint & iN, bool FindMax, int Layer, DX_ID PairID)
{
	static const double MIN_COS = cos(PI * 0.5 - MINAR * 0.001);
	// Move given points 
	BPoint N = iN * Shift;
	if(fabs(N.GetZ()) < MIN_COS)
		return PairID;
	BPoint P0 = P0v * Shift; 
	BPoint P1 = P1v * Shift;
	BPoint P2 = P2v * Shift; 

	// Make oriented lines
	const BPoint *Pts[3] = {&P0, &P1, &P2};
	double X[3] = { P0.GetX(), P1.GetX(), P2.GetX()};
	// Sort points by X -> SortX
	int SortX[3] = { 0, 1, 2};
	for(int is = 0; is < 3; ++is)
	{
		double Xmin = X[SortX[is]];
		int Imin = is;
		for(int iis = is + 1; iis < 3; ++iis)
		{
			if(X[SortX[iis]] < Xmin)
			{
				Xmin = X[SortX[iis]];
				Imin = iis;
			}
		}
		int b = SortX[is]; SortX[is] = SortX[Imin]; SortX[Imin] = b;
	}
	// Ensure uneque
	for(int bk = 1; bk < 3; ++bk)
	{
		if(X[SortX[bk]] == X[SortX[bk - 1]])
			X[SortX[bk - 1]] -= (4 - bk) * 10e-9;
	}
	
	struct Otr
	{
		int NPb, NPe; // The numbers of original points
	};
	Otr Otrs[3] = { {0, 1}, {1, 2}, {2, 0}};
	for(int io = 0; io < 3; ++io)
	{
		if(X[Otrs[io].NPb] > X[Otrs[io].NPe])
		{
			int b = Otrs[io].NPb;
			Otrs[io].NPb = Otrs[io].NPe;
			Otrs[io].NPe = b;
		}
	}
	// Fill iXb, iXe
	struct Status
	{
		double YstBot, YstTop, ZstBot, dZdXBot, dYdXBot, dYdXTop;
		int iXb, iXe, NBot, NTop;
	};
	double zStep0 = 1. / GetStep0();
	double zStep1 = 1. / GetStep1();
	Status S[2];
	S[0].iXb = intCeil(X[SortX[0]] * zStep0);
	if(S[0].iXb > GetN0())
		return PairID;
	S[0].iXe = intCeil(X[SortX[1]] * zStep0);
	S[1].iXb = S[0].iXe;
	S[1].iXe = intCeil(X[SortX[2]] * zStep0);
	if(S[1].iXe <= 0 || S[0].iXb == S[1].iXe)
		return PairID;
	// Raster vertical if any
	int IndS = 0; // Index of the status array (S)
	if(S[0].iXb == S[0].iXe)
		IndS = 1;

	// Main case
//	if(IndS < 2)
	{
		// Fill S[0]
		// Find top and bottom lines for S[0]
		int kos[2];// Find 2 leftmost lines
		int kosi = 0;
		for(int ko = 0; ko < 3; ++ko)
		{
			if(Otrs[ko].NPb == SortX[0])
				kos[kosi++] = ko;
		}
		if(kosi != 2)// Internal error
			return PairID;
		double X0 = X[SortX[0]];
		double dX0 = X[Otrs[kos[0]].NPe] - X0;
		double dX1 = X[Otrs[kos[1]].NPe] - X0;
		double Y0 = Pts[SortX[0]]->GetY();
		double dY0 = Pts[Otrs[kos[0]].NPe]->GetY() - Y0;
		double dY1 = Pts[Otrs[kos[1]].NPe]->GetY() - Y0;
		if(dY0 * dX1 < dY1 * dX0) // dX0 and dX1 can't be zero simultaneously
		{
			S[0].NBot = kos[0];
			S[0].NTop = kos[1];
		}
		else
		{
			S[0].NBot = kos[1];
			S[0].NTop = kos[0];
		}
		
//		for(int kk = 1; kk < 3; ++kk)// Find top and bottom lines for S[*]
		{
			// Find the line starting in the current point
			int NewLine = -1;
			for(int ko = 0; ko < 3; ++ko)
			{
				if(Otrs[ko].NPb == SortX[1])
				{
					NewLine = ko;
					break;
				}
			}
			if(NewLine == -1)
				return PairID;// Internal error
			S[1].NTop = S[0].NTop;
			S[1].NBot = S[0].NBot;
			if(Otrs[S[0].NTop].NPe == SortX[1])// Top line has ended
				S[1].NTop = NewLine;
			else
				S[1].NBot = NewLine;
		}
		for(int kk = 0; kk < 2; ++kk)// Fill the rest of S[*]
		{
			const Otr &OTop = Otrs[S[kk].NTop]; 
			double zdX = (X[OTop.NPe] - X[OTop.NPb]);
			if(zdX != 0.)
				zdX = 1. / zdX;
			S[kk].dYdXTop = zdX * (Pts[OTop.NPe]->GetY() - Pts[OTop.NPb]->GetY()) * GetStep0();
			double t = (S[kk].iXb * GetStep0() - X[OTop.NPb]) * zdX;
			S[kk].YstTop = Pts[OTop.NPb]->GetY() + t * (Pts[OTop.NPe]->GetY() - Pts[OTop.NPb]->GetY());

			const Otr &OBot = Otrs[S[kk].NBot]; 
			zdX = (X[OBot.NPe] - X[OBot.NPb]);
			if(zdX != 0.)
				zdX = 1. / zdX;
			S[kk].dYdXBot = zdX * (Pts[OBot.NPe]->GetY() - Pts[OBot.NPb]->GetY()) * GetStep0();
			S[kk].dZdXBot = zdX * (Pts[OBot.NPe]->GetZ() - Pts[OBot.NPb]->GetZ()) * GetStep0();
			t = (S[kk].iXb * GetStep0() - X[OBot.NPb]) * zdX;
			S[kk].YstBot = Pts[OBot.NPb]->GetY() + t * (Pts[OBot.NPe]->GetY() - Pts[OBot.NPb]->GetY());
			S[kk].ZstBot = Pts[OBot.NPb]->GetZ() + t * (Pts[OBot.NPe]->GetZ() - Pts[OBot.NPb]->GetZ());
		}
		double dZdYPlane = - GetStep1() * N.GetY() / N.GetZ();
		double dNZ = 1. / N.GetZ();
		for(; IndS < 2; ++IndS)
		{
			if(S[IndS].iXb == S[IndS].iXe)
				continue;
			if(S[IndS].iXb > GetN0())
				return PairID;
			if(S[IndS].iXe < 0)
				continue;
			if(S[IndS].iXe > GetN0())
				S[IndS].iXe = GetN0();
			double ZCurG = S[IndS].ZstBot;
			double YBotG = S[IndS].YstBot;
			double YTopG = S[IndS].YstTop;
			for(int iXCur = S[IndS].iXb; iXCur < S[IndS].iXe; ++iXCur)
			{
				if(iXCur >= 0 && YTopG >= 0.)
				{
					//double ZCur = ZCurG;
					int iyMin = intCeil(YBotG * zStep1 - 1.e-5);
					double ZCur = iyMin * GetStep1() * N.GetY() + iXCur * GetStep0() * N.GetX() - P0 * N;
					ZCur = - ZCur * dNZ;
					int iyMax = int(YTopG * zStep1 + 1.e-5);// intFloat not needed because of YTopG >= 0.
					int YLim = GetN1() - 1;
					iyMax = min(iyMax, YLim);
					DX_DEPTH *pColumnD = GetColumnD(Layer, iXCur) + iyMin;
					DX_ID *pColumnI = GetColumnI(Layer, iXCur);
					for(int iYCur = iyMin; iYCur <= iyMax; ++iYCur)
					{
						if(iYCur >= 0)
						{
							DX_DEPTH &CDepth = *pColumnD;
							bool ZCurGrater = (ZCur > CDepth);
							if(FindMax == ZCurGrater)
							{
								CDepth = DX_DEPTH(ZCur);
								*(pColumnI + iYCur) = PairID;
								SetChanged(iXCur, iYCur);
							}
						}
						++pColumnD;
						ZCur += dZdYPlane;
					}
				}
				ZCurG += S[IndS].dZdXBot;
				YBotG += S[IndS].dYdXBot;
				YTopG += S[IndS].dYdXTop;
			}
		}
	}
	return PairID;
}

DX_ID DXDirection3X::InsRect(const BPoint & P0v, const BPoint & P1v, const BPoint & P2v, const BPoint & P3v, const BPoint & iN, bool FindMax, int Layer, DX_ID PairID)
{
	static const double MIN_COS = cos(PI * 0.5 - MINAR * 0.001);
	// Move given points 
	BPoint N = iN * Shift;
	if(fabs(N.GetZ()) < MIN_COS)
		return PairID;
	BPoint P0 = P0v * Shift; 
	BPoint P1 = P1v * Shift;
	BPoint P2 = P2v * Shift; 
	BPoint P3 = P3v * Shift;

	// Make oriented lines
	const BPoint *Pts[4] = {&P0, &P1, &P2, &P3};
	double X[4] = { P0.GetX(), P1.GetX(), P2.GetX(), P3.GetX()};
	// Sort points by X -> SortX
	int SortXb[4];
	if(X[0] < X[1])
	{ SortXb[0] = 0; SortXb[1] = 1;}
	else
	{ SortXb[0] = 1; SortXb[1] = 0;}
	if(X[2] < X[3])
	{ SortXb[2] = 2; SortXb[3] = 3;}
	else
	{ SortXb[2] = 3; SortXb[3] = 2;}
	int SortX[4];
	int j1 = 0, j2 = 2;
	for(int bk = 0;;) 
	{
		if(X[SortXb[j1]] < X[SortXb[j2]])
		{
			SortX[bk++] = SortXb[j1++];
			if(j1 > 1)
			{
				while(j2 <= 3)
					SortX[bk++] = SortXb[j2++];
				break;
			}
		}
		else
		{
			SortX[bk++] = SortXb[j2++];
			if(j2 > 3)
			{
				while(j1 <= 1)
					SortX[bk++] = SortXb[j1++];
				break;
			}
		}
	}
	// Ensure uneque
	for(int bk = 1; bk < 4; ++bk)
	{
		if(X[SortX[bk]] == X[SortX[bk - 1]])
			X[SortX[bk - 1]] -= (4 - bk) * 10e-7;
	}

	struct Otr
	{
		int NPb, NPe; // The numbers of original points
	};
	Otr Otrs[4] = { {0, 1}, {1, 2}, {2, 3}, {3, 0}};
	for(int io = 0; io < 4; ++io)
	{
		if(X[Otrs[io].NPb] > X[Otrs[io].NPe])
		{
			int b = Otrs[io].NPb;
			Otrs[io].NPb = Otrs[io].NPe;
			Otrs[io].NPe = b;
		}
	}
	// Fill iXb, iXe
	struct Status
	{
		double YstBot, YstTop, ZstBot, dZdXBot, dYdXBot, dYdXTop;
		int iXb, iXe, NBot, NTop;
	};
	double zStep0 = 1. / GetStep0();
	double zStep1 = 1. / GetStep1();
	Status S[3];
	S[0].iXb = intCeil(X[SortX[0]] * zStep0);
	if(S[0].iXb > GetN0())
		return PairID;
	S[0].iXe = intCeil(X[SortX[1]] * zStep0);
	S[1].iXb = S[0].iXe;
	S[1].iXe = intCeil(X[SortX[2]] * zStep0);
	S[2].iXb = S[1].iXe;
	S[2].iXe = intCeil(X[SortX[3]] * zStep0);
	if(S[2].iXe <= 0 || S[0].iXb == S[2].iXe)
		return PairID; // Don't raster anything
	//Check for vertical
	int IndS = 0; // Index of the status array (S)
	if(S[0].iXb == S[0].iXe)
	{// Raster line
		//
		IndS = 1;
		if(S[1].iXb == S[1].iXe)
			IndS = 2;
	}

	// Fill S[0]
	// Find top and bottom lines for S[0]
	int kos[2];// Find 2 leftmost lines
	int kosi = 0;
	for(int ko = 0; ko < 4; ++ko)
	{
		if(Otrs[ko].NPb == SortX[0])
			kos[kosi++] = ko;
	}
	if(kosi != 2)// Internal error
		return PairID;
	double X0 = X[SortX[0]];
	double dX0 = X[Otrs[kos[0]].NPe] - X0;
	double dX1 = X[Otrs[kos[1]].NPe] - X0;
	double Y0 = Pts[SortX[0]]->GetY();
	double dY0 = Pts[Otrs[kos[0]].NPe]->GetY() - Y0;
	double dY1 = Pts[Otrs[kos[1]].NPe]->GetY() - Y0;
	if(dY0 * dX1 < dY1 * dX0) // dX0 and dX1 can't be zero simultaneously
	{
		S[0].NBot = kos[0];
		S[0].NTop = kos[1];
	}
	else
	{
		S[0].NBot = kos[1];
		S[0].NTop = kos[0];
	}
		
	for(int kk = 1; kk < 3; ++kk)// Find top and bottom lines for S[*]
	{
		// Find the line starting in the current point
		int NewLine = -1;
		for(int ko = 0; ko < 4; ++ko)
		{
			if(Otrs[ko].NPb == SortX[kk])
			{
				NewLine = ko;
				break;
			}
		}
		if(NewLine == -1)
			return PairID;// Internal error
		S[kk].NTop = S[kk - 1].NTop;
		S[kk].NBot = S[kk - 1].NBot;
		if(Otrs[S[kk - 1].NTop].NPe == SortX[kk])// Top line has ended
			S[kk].NTop = NewLine;
		else
			S[kk].NBot = NewLine;
	}
	for(int kk = 0; kk < 3; ++kk)// Fill the rest of S[*]
	{
		const Otr &OTop = Otrs[S[kk].NTop]; 
		double zdX = (X[OTop.NPe] - X[OTop.NPb]);
		if(zdX != 0.)
			zdX = 1. / zdX;
		S[kk].dYdXTop = zdX * (Pts[OTop.NPe]->GetY() - Pts[OTop.NPb]->GetY()) * GetStep0();
		double t = (S[kk].iXb * GetStep0() - X[OTop.NPb]) * zdX;
		S[kk].YstTop = Pts[OTop.NPb]->GetY() + t * (Pts[OTop.NPe]->GetY() - Pts[OTop.NPb]->GetY());

		const Otr &OBot = Otrs[S[kk].NBot]; 
		zdX = (X[OBot.NPe] - X[OBot.NPb]);
		if(zdX != 0.)
			zdX = 1. / zdX;
		S[kk].dYdXBot = zdX * (Pts[OBot.NPe]->GetY() - Pts[OBot.NPb]->GetY()) * GetStep0();
		S[kk].dZdXBot = zdX * (Pts[OBot.NPe]->GetZ() - Pts[OBot.NPb]->GetZ()) * GetStep0();
		t = (S[kk].iXb * GetStep0() - X[OBot.NPb]) * zdX;
		S[kk].YstBot = Pts[OBot.NPb]->GetY() + t * (Pts[OBot.NPe]->GetY() - Pts[OBot.NPb]->GetY());
		S[kk].ZstBot = Pts[OBot.NPb]->GetZ() + t * (Pts[OBot.NPe]->GetZ() - Pts[OBot.NPb]->GetZ());
	}
	double dNZ = 1. / N.GetZ();
	double dZdYPlane = - GetStep1() * N.GetY() * dNZ;
	for(; IndS < 3; ++IndS)
	{
		if(S[IndS].iXb == S[IndS].iXe)
			continue;
		if(S[IndS].iXb > GetN0())
			return PairID;
		if(S[IndS].iXe <= 0)
			continue;
		if(S[IndS].iXe > GetN0())
			S[IndS].iXe = GetN0();
		double ZCurG = S[IndS].ZstBot;
		double YBotG = S[IndS].YstBot;
		double YTopG = S[IndS].YstTop;
		for(int iXCur = S[IndS].iXb; iXCur < S[IndS].iXe; ++iXCur)
		{
			if(iXCur >= 0 && YTopG >= 0.)
			{
//				double ZCur = ZCurG;
				int ixModif = iXCur / n0Big;
				bool xBorder = ((iXCur % n0Big) == 0) && (iXCur != 0) && ixModif > 0;
				int iyMin = intCeil(YBotG * zStep1 - 1.e-5);
				iyMin = max(iyMin, 0);
				double ZCur = iyMin * GetStep1() * N.GetY() + iXCur * GetStep0() * N.GetX() - P0 * N;
				ZCur = - ZCur * dNZ - dZdYPlane;
				int iyMax = int(YTopG * zStep1 + 1.e-5);// intFloor not needed because YTopG >= 0.
				int YLim = GetN1() - 1;
				iyMax = min(iyMax, YLim);
				iyMax = max(iyMax, iyMin - 1);
				DX_DEPTH *pColumnD = GetColumnD(Layer, iXCur);
				DX_ID *pColumnI = GetColumnI(Layer, iXCur);
				for(int iYCur = iyMin; iYCur <= iyMax; ++iYCur)
				{
					ZCur += dZdYPlane;
					DX_DEPTH &CDepth = *(pColumnD + iYCur);
					bool ZCurGrater = (ZCur > CDepth);
					if(FindMax == ZCurGrater)
					{
						div_t d = div(iYCur, n1Big);
						CDepth = DX_DEPTH(ZCur);
						*(pColumnI + iYCur) = PairID;
						SetChanged();
						bool yBorder = (d.rem == 0) && (iYCur != 0) && (d.quot > 0);
 						int iyModif = d.quot;
						if(pModifiedCellsD)
							pModifiedCellsD->Set3Bits(ixModif, iyModif, xBorder, yBorder);
					}
				}
			}
			ZCurG += S[IndS].dZdXBot;
			YBotG += S[IndS].dYdXBot;
			YTopG += S[IndS].dYdXTop;
		}
	}
	return PairID;
}

DX_ID DXDirection3X::RastCylinder(const BPoint & P0, const BPoint & P1, const BPoint & , const BPoint & , double R, DX_ID PairID)
{
	// Insert cylinder part of ball cadr
	// P0v, P1v - tool positions
	// R - sphere radius
	BPoint V(P1 - P0);
	BPoint Vxy(V);	
	Vxy.ProjXY();
	double Dxy2 = Vxy.D2();
	bool IsVert = (Dxy2 < MIND*MIND);
	if(IsVert)
		return DX_ID();
	bool IsHor = (fabs(V.GetZ()) < MIND);
	double LxyI = 1. / sqrt(Dxy2);
	BPoint VxyE(Vxy * LxyI);
	BPoint NxyE(-VxyE.GetY(), VxyE.GetX(), 0., 0.);
	BPoint NxyER(NxyE * R);
	// Calculate gabar
	double Vx2, Vy2, Vz2, h;
	V.Get(Vx2, Vy2, Vz2, h);
	Vx2 *= Vx2;
	Vy2 *= Vy2;
	Vz2 *= Vz2;
	double zVD2 = 1. / (Vx2 + Vy2 + Vz2);
	BPoint Vgab(sqrt((Vy2 + Vz2) * zVD2), sqrt((Vx2 + Vz2) * zVD2), sqrt((Vx2 + Vy2) * zVD2), 0.);
	Vgab = Vgab * R;
	BBox Box;
	Box.Expand(P0 + Vgab);
	Box.Expand(P1 + Vgab);
	Box.Expand(P0 - Vgab);
	Box.Expand(P1 - Vgab);
	BPoint MidVR(NxyER * BMatr().rotg(90., P1, P0));
	BPoint MidVE = MidVR * (1. / R);
	// Calculate planes normal to V passing through P0 and P1
	double A = V.GetX();
	double B = V.GetY();
	double C = V.GetZ();
	double D0 = - (V * P0);
	double D1 = - (V * P1);
	if(!IsHor)
	{
		double zC = -1. / C;
		A *= zC;
		B *= zC;
		D0 *= zC;
		D1 *= zC;
		C = 1.;
	}
	if(D0 > D1)
	{
		double b = D0; D0 = D1; D1 = b;
	}
	double CosAI = 1. / sqrt(1. - V.GetZ() * V.GetZ() / V.D2());
	// For each XY point
	const double *MidVEc = MidVE.GetArray();
	double xP0 = P0.GetX();
	double xP1 = P1.GetX();
	double yP0 = P0.GetY();
	double yP1 = P1.GetY();
	unsigned int ixMin = max(0, int(Box.Xmin / Step0 + 1. - 1.e-5));
	double xStart = ixMin * Step0;
	unsigned int iyMin = max(0, int(Box.Ymin / Step1 + 1. - 1.e-5));
	double yStart = iyMin * Step1;
	double qz =   - P0.GetZ();
	unsigned int ixMax = max(0, min(n0, int(Box.Xmax / Step0) + 1));
	unsigned int iyMax = max(0, min(n1, int(Box.Ymax / Step1) + 1));
	double qx = xStart - xP0;
	double R2 = R * R;
	for(unsigned int ix = ixMin; ix < ixMax; ++ix, qx += Step0)
	{
//		int ixModif = ix / n0Big;
//		bool xBorder = ((ix % n0Big) == 0) && (ix != 0) && ixModif > 0;
		double sP0 = qx * VxyE.GetX() - P0.GetZ() * VxyE.GetZ();
		double sP1 = (qx + xP0 - xP1) * VxyE.GetX() - P1.GetZ() * VxyE.GetZ();
		double Zcb = MidVEc[0] * qx + MidVEc[2] * qz;
		double qy = yStart - yP0;
		DX_DEPTH *pColumnDD = GetColumnD(0, ix) + iyMin;
		DX_ID *pColumnID = GetColumnI(0, ix);
		DX_DEPTH *pColumnDU = GetColumnD(1, ix) + iyMin;
		DX_ID *pColumnIU = GetColumnI(1, ix);
		double bpx = A * (qx + xP0);
		double qqX = qx * NxyE.GetX();
		for(unsigned int iy = iyMin; iy < iyMax; ++iy, qy += Step1)
		{
			DX_DEPTH &CDepthD = *(pColumnDD++);
			DX_DEPTH &CDepthU = *(pColumnDU++);
			if(Box.Zmin >= CDepthD && Box.Zmax <= CDepthU)
				continue;
			if(IsHor)
			{
				if(qy * VxyE.GetY() + sP0 < 0.)
					continue;
				if((qy + yP0 - yP1) * VxyE.GetY() + sP1 > 0.)
					continue;
			}
			double bp = B * (qy + yP0) + bpx;
            
			double z0 = (IsHor ? -1.e6 : bp) + D0;  // Z coordinates of the limiting planes
			double z1 = (IsHor ?  1.e6 : bp) + D1;  
			if(z0 > CDepthD && z1 < CDepthU)
				continue;


			//Calc cylinder Z
			double Bxy = qqX + qy * NxyE.GetY();
			double Dz2 = R2 - Bxy * Bxy;
			if(Dz2 < 0.)
				continue;
			double Sf = (MidVEc[1] * qy + Zcb) * CosAI;
			double Ss = sqrt(Dz2) * CosAI;
			double Zcd = Sf - Ss; // Zdown
			double Zcu = Sf + Ss; // Zup
			if(Zcd < z0 && Zcu > z0)
				Zcd = z0;
			if(Zcu > z1 && Zcd < z1)
				Zcu = z1;
			if(Zcu <= 0.)
				continue;
			if(Zcd >= UpLimit)
				continue;
			if(Zcd <= 0.)
				Zcd = -MIND;
			if(Zcu >= UpLimit)
				Zcu = UpLimit + MIND;
			if(Zcd < CDepthD && Zcd >= z0 && Zcd <= z1)
			{
				CDepthD = DX_DEPTH(Zcd);
				*(pColumnID + iy) = PairID;
			}
			if(Zcu > CDepthU && Zcu >= z0 && Zcu <= z1)
			{
				CDepthU = DX_DEPTH(Zcu);
				*(pColumnIU + iy) = PairID;
			}
		}
	}
	return PairID; //was 0
}


DX_ID DXDirection3X::RastICylinder(const BPoint & P0v, const BPoint & P1v, const BPoint & iN, double R, bool IsPlane, DX_ID PairID, bool flat_disk, DX_ID FPairID)
{

	// Move given points 
	BPoint N = iN * Shift;
	BPoint P0 = P0v * Shift; 
	BPoint P1 = P1v * Shift;
	DXRastDisk Disk(P0, P1, R, *this); 
	if(IsPlane)
		Disk.DegradedCylinder(N, PairID);
	else
		Disk.InclinedCylinder(N, PairID);
	if(flat_disk)
		Disk.FlatDisk(N, FPairID);
	return PairID;
}

DX_ID DXDirection3X::RastHCylinder(const BPoint & P0v, const BPoint & P1v, const BPoint &ArcCntrv, double R, DX_ID PairID, bool flat_disk, DX_ID FPairID)
{

	// Move given points 
	BPoint P0 = P0v * Shift; 
	BPoint P1 = P1v * Shift;
	BPoint ArcCntr = ArcCntrv * Shift;
	DXRastDisk Disk(P0, P1, R, *this);
	Disk.RightCylinder(ArcCntr, PairID);
	if(flat_disk)
		Disk.FlatDisk((P0-P1), FPairID);//логично, что диск должен быть в конце, значит внутренняя нормаль P0-P1
	return PairID;
}

DX_ID DXDirection3X::RastCylSegment(const BPoint & pP0, const BPoint & pP1, const BPoint &vA, const BPoint &vB, double R, DX_ID PairID, bool flat_disk, DX_ID FPairID)
{
	// Move given points 
	BPoint P0 = pP0 * Shift; 
	BPoint P1 = pP1 * Shift;
	BPoint A = vA * Shift;
	BPoint B = vB * Shift;
	DXRastDisk Disk(P0, P1, R, *this);
	Disk.RightCylinderSegment(A,B, PairID);
	if(flat_disk)
		Disk.FlatDisk((P0-P1), FPairID);//логично, что диск должен быть в конце, значит внутренняя нормально P0-P1
	return PairID;
}

DX_ID DXDirection3X::RastSphSegment(const BPoint & vP0, int NPlanes, const BPoint * vP, const BPoint * vN, double R, DX_ID PairID)
{
	// Sphere segment corresponding to P0
	// Limiting planes normals (vN) and points (vP)
	// Point is allowed if (X - Pi) * Ni > 0
	BPoint P0 = vP0 * Shift;
	BPoint *P = new BPoint[NPlanes];
	BPoint *N = new BPoint[NPlanes];
	double *T = new double[NPlanes];
	double *Tx = new double[NPlanes];
	for(int i = 0; i < NPlanes; ++i)
	{
		P[i] = vP[i] * Shift;
		N[i] = vN[i] * Shift;
	}
	BBox Box;
	Box.Expand(P0 - BPoint(R, R, R, 1.));
	Box.Expand(P0 + BPoint(R, R, R, 1.));
	int ixMin = int(max(0, int(Box.Xmin / Step0 + 1 - 1.e-5)));
	int ixMax = int(max(0, min(n0, int(Box.Xmax / Step0 + 1.e-5) + 1)));
	int iyMin = int(max(0, int(Box.Ymin / Step1 + 1 - 1.e-5)));
	int iyMax = int(max(0, min(n1, int(Box.Ymax / Step1 + 1.e-5) + 1)));
	for(int ix = ixMin; ix < ixMax; ++ix)
	{
		double x = ix * Step0;
		double xp = x - P0.GetX();

		DX_DEPTH *pColumnDD = GetColumnD(0, ix) + iyMin;
		DX_ID *pColumnID = GetColumnI(0, ix);
		DX_DEPTH *pColumnDU = GetColumnD(1, ix) + iyMin;
		DX_ID *pColumnIU = GetColumnI(1, ix);
		for(int PlaneInd = 0; PlaneInd < NPlanes; ++PlaneInd)
			Tx[PlaneInd] = (x - P[PlaneInd].GetX()) * N[PlaneInd].GetX();

		for(int iy = iyMin; iy < iyMax; ++iy)
		{
			DX_DEPTH &CDepthD = *(pColumnDD++);
			DX_DEPTH &CDepthU = *(pColumnDU++);
			if(Box.Zmin >= CDepthD && Box.Zmax <= CDepthU)
				continue;

			double y = iy * Step1;
			double yp = y - P0.GetY();
			double zp = R * R - xp * xp - yp * yp;
			if(zp < 0.)
				continue;
			zp = sqrt(zp);
			double Zcd = P0.GetZ() - zp; // Zdown
			double Zcu = P0.GetZ() + zp; // Zup

			for(int PlaneInd = 0; PlaneInd < NPlanes; ++PlaneInd)
				T[PlaneInd] = Tx[PlaneInd] + (y - P[PlaneInd].GetY()) * N[PlaneInd].GetY();

			bool PointOK = true;
			for(int PlaneInd = 0; PlaneInd < NPlanes && PointOK; ++PlaneInd)
				PointOK &= ((N[PlaneInd].GetZ() * (Zcd - P[PlaneInd].GetZ()) + T[PlaneInd]) > 0.);
			if(Zcd < CDepthD && PointOK)
			{
				if(Zcd <= 0.)
					Zcd = -MIND;
				CDepthD = DX_DEPTH(Zcd);
				*(pColumnID + iy) = PairID;
			}
			PointOK = true;
			for(int PlaneInd = 0; PlaneInd < NPlanes && PointOK; ++PlaneInd)
				PointOK &= ((N[PlaneInd].GetZ() * (Zcu - P[PlaneInd].GetZ()) + T[PlaneInd]) > 0.);
			if(Zcu > CDepthU && PointOK)
			{
				if(Zcu >= UpLimit)
					Zcu = UpLimit + MIND;
				CDepthU = DX_DEPTH(Zcu);
				*(pColumnIU + iy) = PairID;
			}
		}
	}
	delete [] P;
	delete [] N;
	delete [] T;
	delete [] Tx;
	return PairID;
}

bool DXDirection3X::Merge(DXDirection3X & In, int &ir0, int &ir1)
{
	// Returns true if merging completed successfully
	// Returns false if In dexel doesn't intersect with this dexel
	// i0, i1 - indexes of the first wrong dexel
	SubWin.Expand(In.SubWin.ixEnd - 1, In.SubWin.iyEnd - 1);
	SubWin.Expand(In.SubWin.ixStart, In.SubWin.iyStart);
	for(int i0 = In.SubWin.ixStart; i0 < In.SubWin.ixEnd; ++i0)
	{
		for(int i1 = In.SubWin.iyStart; i1 < In.SubWin.iyEnd; ++i1)
		{
			DX_DEPTH &CDd = DepthEl(0, i0, i1);
			DX_DEPTH &CDu = DepthEl(1, i0, i1);
			DX_DEPTH &CDdI = In.DepthEl(0, i0, i1);
			DX_DEPTH &CDuI = In.DepthEl(1, i0, i1);
			bool Uu = (CDu != DX_DEPTH_MIN && CDdI != DX_DEPTH_MAX && CDdI > CDu);
			bool Dd = (CDuI != DX_DEPTH_MIN && CDd != DX_DEPTH_MAX && CDuI < CDd);
			if(Uu || Dd)
			{
				ir0 = i0;
				ir1 = i1;
				return false;
			}
			if(CDdI < CDd)
			{
				CDd = CDdI;
				IDsEl(0, i0, i1) = In.IDsEl(0, i0, i1);
			}
			CDdI = DX_DEPTH_MAX;
			if(CDuI > CDu)
			{
				CDu = CDuI;
				IDsEl(1, i0, i1) = In.IDsEl(1, i0, i1);
			}
			CDuI = DX_DEPTH_MIN;
		}
	}
	return true;
}

void DXDirection3X::Move(DXDirection3X & In)
{
	SubWin = In.SubWin;
	int MemShift = SubWin.ixStart * n1 + SubWin.iyStart;
	int iySize = SubWin.iyEnd - SubWin.iyStart;
	int MemSizeD = iySize * sizeof(DX_DEPTH);
	DX_DEPTH DVal = DX_DEPTH_MAX;
	for(int i = 0; i < LayersNum; ++i)
	{
		DX_DEPTH *DCol = Depth[i] + MemShift;
		DX_DEPTH *DColIn = In.Depth[i] + MemShift;
		int i0 = SubWin.ixStart;
		memcpy(DCol, DColIn, MemSizeD);// Copy
		++i0;
		for(int i1 = 0; i1 < iySize; ++i1)// Clear source
			DColIn[i1] = DVal;
		DCol += n1;
		DColIn += n1;
		for(; i0 < SubWin.ixEnd; ++i0, DCol += n1, DColIn += n1)
		{
			memcpy(DCol, DColIn, MemSizeD); // Copy
			memcpy(DColIn, DColIn - n1, MemSizeD);// Clear source
		}
		DVal = DX_DEPTH_MIN;
	}
	int MemSizeI = iySize * sizeof(DX_ID);
	for(int i = 0; i < LayersNum; ++i)
	{
		DX_ID *ICol = IDs[i] + MemShift;
		DX_ID *IColIn = In.IDs[i] + MemShift;
		for(int i0 = SubWin.ixStart; i0 < SubWin.ixEnd; ++i0, ICol += n1, IColIn += n1)
			memcpy(ICol, IColIn, MemSizeI);
	}
}

void DXDirection3X::MakeFromBSP(const MBSPNode *pTree)
{
	double (BPoint::*GetVal)(void) const = &BPoint::GetZ;
	switch(Dir)
	{
	case Z:
		GetVal = &BPoint::GetZ;
		break;
	case Y:
		GetVal = &BPoint::GetY;
		break;
	case X:
		GetVal = &BPoint::GetX;
		break;
	}
	ClearChanged();
	//MakeEmpty();
	BMatr ShiftI = Shift.invr();
	BPoint VDir(0., 0., 1., 0.);
	VDir = VDir * ShiftI;
	std::list<MBSPNode *> Nodes;
	for(int i0 = 0; i0 < GetN0(); ++i0)
	{
		double v0 = GetStep0() * i0;// + Min0;
		for(int i1 = 0; i1 < GetN1(); ++i1)
		{
			double v1 = GetStep1() * i1;// + Min1;
			BPoint BaseP(v0, v1, -1., 1.);
			BaseP = BaseP * ShiftI;
			pTree->RayShredding(BaseP, VDir, Nodes);
			DX_DEPTH ZValMinMax[2] = {DX_DEPTH(1.e12), DX_DEPTH(-1.e12)};
			DX_ID IDValMinMax[2] = {DX_ID(0, cadrID(-1)), DX_ID(0, cadrID(-1)) };
			auto iter = Nodes.cbegin();
			for (unsigned int i = 0; i < Nodes.size(); ++i, iter++)
			{
				const MBSPNode *pNode = *iter;
				BPoint R;
				pNode->GetFace()->GetPlane().IntersLine(BaseP, VDir, R);
				DX_DEPTH Z = DX_DEPTH((R.*GetVal)());
				if(Z < ZValMinMax[0])
				{
					ZValMinMax[0] = Z;
					int BFaceN = pNode->GetBFaceN();
					IDValMinMax[0].Elem = max(BFaceN, 0);
				}
				if(Z > ZValMinMax[1])
				{
					ZValMinMax[1] = Z;
					int BFaceN = pNode->GetBFaceN();
					IDValMinMax[1].Elem = max(BFaceN, 0);
				}
			}
			DepthEl(0, i0, i1) = ZValMinMax[0];
			DepthEl(1, i0, i1) = ZValMinMax[1];
			IDsEl(0, i0, i1) = IDValMinMax[0];
			IDsEl(1, i0, i1) = IDValMinMax[1];
			Nodes.clear();
		}
	}
}

bool DXDirection3X::Check() const
{
	bool res = true;
	for (int i0 = SubWin.ixStart; i0 < SubWin.ixEnd; ++i0)
	{
		for (int i1 = SubWin.iyStart; i1 < SubWin.iyEnd; ++i1)
		{
			DX_DEPTH CDd = DepthEl(0, i0, i1);
			DX_DEPTH CDu = DepthEl(1, i0, i1);
			bool EmptyUp = (CDu == DX_DEPTH_MIN);
			bool EmptyDown = (CDd == DX_DEPTH_MAX);
			if (!EmptyUp && EmptyDown)
			{
				Beep(500, 500);
				res = false;
			}
		}
	}
	return res;

}