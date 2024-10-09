#include "stdafx.h"
#include "BMatr.h"
#include "NCadrGeom.h"
#include "NGrooveChain.h"

NGrooveChain::NGrooveChain(double Tol)
{
	Clear();
	SetTol(Tol);
}

NGrooveChain::~NGrooveChain(void)
{
}
int NGrooveChain::GetSamplesNum(void)
{
	return SamplesNum;
}
bool NGrooveChain::GetSample(int Num, SMachStateFix1 & P)
{
	if(NCadrs < 1)
		return false;
	if(SamplesNum < 2)
		return false;
	BMatr CorrMatr;
	CorrMatr.SetE();
	if(Num >= SamplesNum - 1)
	{// Very last point
		int i = NCadrs - 1;
		for(; i >= 0; --i)
			if(!Cadrs[i]->IsUndef())
				break;
		Cadrs[i]->CalcCurCoordMS(CorrMatr, 1., P);
	}
	else
	{
		int Count = 0, i = 0;
		for(; Count <= Num && i < NCadrs; ++i)
			Count += CadrsSamplesNum[i];

		if(Count == Num)
		{// Last point of one cadr
			Cadrs[i]->CalcCurCoordMS(CorrMatr, 0., P);
		}
		else
		{// Intermediate point
			--i;
			Num -= Count - CadrsSamplesNum[i]; 
			double t = double(Num) / CadrsSamplesNum[i];
			Cadrs[i]->CalcCurCoordMS(CorrMatr, t, P);
		}
	}
	return true;
}
bool NGrooveChain::Define(void)
{
	if(Defined)
		return false;

	Defined = false;

	CalcSamplesNum();
	if(SamplesNum <= 0)
		return false;

	if(NCadrs <= 0)
		return false;

	Defined = true;
	return true;
}
void NGrooveChain::Clear(void)
{
	Defined = false;
	SamplesNum = 0;
	NCadrs = 0;
	Orient = 0;
	Fast = false;
}
void NGrooveChain::CalcSamplesNum(void)
{
	SamplesNum = 0;

	for(int i = 0; i < NCadrs; ++i)
		SamplesNum += (CadrsSamplesNum[i] = Cadrs[i]->GetNumAppr(Toler));

	if(SamplesNum > 0)
		++SamplesNum;
}
int NGrooveChain::SendCadrs(NCadrGeom *InCadrs, int Size, int iOrient)
{
	const double MaxAngle = 100.;
	Clear();
	if(Size <= 0)
		return 0;
	Orient = iOrient;
	NCadrs = 1;
	Cadrs[0] = &InCadrs[0];
	BPoint DeltaABC;
	Cadrs[0]->GetDeltaABC(DeltaABC);
	double CurAngle = DeltaABC.CAbsSum();
	if(CurAngle >= MaxAngle)
		return NCadrs;
	int IsFast = 0; // -1 - Yes; +1 - No ; 0 - Unknown
	IsFast = Cadrs[0]->HaveGeom5() ? (Cadrs[0]->IsFast() ? -1 : 1) : 0;
	Fast = (IsFast == -1);
	for(NCadrs = 1; NCadrs < Size - 1; ++NCadrs)
	{
		InCadrs[NCadrs].GetDeltaABC(DeltaABC);
		CurAngle += DeltaABC.CAbsSum();
		if(CurAngle >= MaxAngle)
			break;
		else
		{// ensure all cadrs are fast or all cadrs are not fast
			int LocFast = InCadrs[NCadrs].HaveGeom5() ? (InCadrs[NCadrs].IsFast() ? -1 : 1) : 0;
			if (IsFast == 0)
				IsFast = LocFast;
			else
			{
				if(IsFast != 0)
					Fast = (IsFast == -1);
				if (LocFast != 0 && IsFast != LocFast)
					break;
			}
			Cadrs[NCadrs] = &InCadrs[NCadrs];
		}
	}
	return NCadrs;
}
