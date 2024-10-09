#include "stdafx.h"
#include "ConstDef.h"
#include "MBSPForest.h"
#include "MBSPNode.h"
#include "MElemIDProc.h"
#include "SOPlane.h"
#include "BDynBitArray.h"
#include "DXDirection3X.h"
#include "MCubeMask.h"
#include "DXSubModel.h"
#include "GQuadRenderMill.h"
#include <list>
#include "DXDirection5X.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//конструктор по умолчанию
DXDirection5X::DXDirection5X() : DXDirection()
{
	Dexels = NULL;
}

//деструктор
DXDirection5X::~DXDirection5X(void)
{
	if(Dexels)
	{
		delete[] Dexels[0];
		delete[] Dexels;
	}
}
//инициализация
bool DXDirection5X::Init(const DXSubModel *ipSubModel, DX_Dir NewDir, int s0, int s1, double St0, double St1, int s0Big, int s1Big, double v0, double v1, double vv)
{
	DXDirection::Init(ipSubModel, NewDir, s0, s1, St0, St1, s0Big, s1Big, v0, v1, vv);

	return Init();
}

bool DXDirection5X::Init()
{
	if (Dexels)
	{
		delete[] Dexels[0];
		delete[] Dexels;
	}
	if (n0 == 0 || n1 == 0)
		return false;
	typedef DXMemID* pDXPointID;
	Dexels = new pDXPointID[n0];
	DXMemID* pStart = new DXMemID[n1 * n0];
	if (pStart == NULL)
		return false;
	for (int k = 0; k < n0 * n1; ++k)
		pStart[k] = DXP_END;
	for (int i0 = 0; i0 < n0; ++i0, pStart += n1)
		Dexels[i0] = pStart;
	if (pSubModel != nullptr)
	{
		delete pModifiedCellsD;
		pModifiedCellsD = new MCubeMask(pSubModel->GetParent().GetColsNum(),
			pSubModel->GetParent().GetRowsNum(), pSubModel->GetParent().GetLevelsNum());
	}
	return true;
}

void DXDirection5X::ClearModifiedD()
{
	if(pModifiedCellsD != nullptr)
		pModifiedCellsD->Clear();
}

bool DXDirection5X::Save2File(CFile* f) const
{
	DXDirection::Save2File(f);
	f->Write(Dexels[0], sizeof(Dexels[0][0]) * n0 * n1); // this works properly if Dexels[0] = pStart in DXDirection5X::Init
	return true;
}

void DXDirection5X::Load(CFile* f)
{
	DXDirection::Load(f);
	Init();
	f->Read(Dexels[0], sizeof(Dexels[0][0]) * n0 * n1); // this works properly if Dexels[0] = pStart in DXDirection5X::Init
}

void DXDirection5X::CreateFullCopy(DXDirection* pInt) const
{
	DXDirection::CreateFullCopy(pInt);
	DXDirection5X* pIn = dynamic_cast<DXDirection5X*>(pInt);
	if (pIn == nullptr)
		return;
	pIn->Init();
	memcpy_s(pIn->Dexels[0], sizeof(Dexels[0][0]) * n0 * n1, Dexels[0], sizeof(Dexels[0][0]) * n0 * n1); // this works properly if Dexels[0] = pStart in DXDirection5X::Init
}

double DXDirection5X::GetNearestPoint(const DXMemoryPtOnePart* pActMemory, const BPoint& OrP, int OneCoordSteps, MBodyPointInf& Inf) const
{
	double Xl = 0.;
	double Yl = 0.;
	double Zl = 0.;
	switch (Dir)
	{
	case Z:
		Xl = OrP.GetX();
		Yl = OrP.GetY();
		Zl = OrP.GetZ();
		break;
	case X:
		Zl = OrP.GetX();
		Xl = OrP.GetY();
		Yl = OrP.GetZ();
		break;
	case Y:
		Yl = OrP.GetX();
		Zl = OrP.GetY();
		Xl = OrP.GetZ();
		break;
	}
	int ix = 0, iy = 0;
	GetCellIndexes(Xl, Yl, ix, iy);
	int bx = -1, by = -1;
	double D2 = 1.e6;
	DXMemID Found = DXP_END;
	bool Xinternal = ix < n0 - 1;
	bool Yinternal = iy < n1 - 1;
	FindNearest(pActMemory, Xl, Yl, Zl, ix, iy, D2, bx, by, Found);
	if (Xinternal)
		FindNearest(pActMemory, Xl, Yl, Zl, ix + 1, iy, D2, bx, by, Found);
	if (Yinternal)
		FindNearest(pActMemory, Xl, Yl, Zl, ix, iy + 1, D2, bx, by, Found);
	if (Xinternal && Yinternal)
		FindNearest(pActMemory, Xl, Yl, Zl, ix + 1, iy + 1, D2, bx, by, Found);
	if (D2 > GetStep0() || D2 > GetStep1())
	{
	}
	else
		FillPointInf(pActMemory, bx, by, Found, Inf);
	return D2;
}

void DXDirection5X::FindNearest(const DXMemoryPtOnePart* pActMemory, double Xl, double Yl, double Zl, int ix, int iy, double& D2, int& bx, int& by, DXMemID& Found) const
{
	const double DX = Xl - ix * Step0 - Min0;
	const double DY = Yl - iy * Step1 - Min1;
	const double DXY2 = DX * DX + DY * DY;
	if (DXY2 >= D2)
		return;
	for (auto CurID = GetDexelStartPoint(ix, iy); CurID != DXP_END; CurID = pActMemory->GetAtNext(CurID))
	{
		const double CurZ = pActMemory->GetAtZ(CurID) + MinVal - Zl;
		const double D2l = CurZ * CurZ + DXY2;
		if (D2l < D2)
		{
			D2 = D2l;
			bx = ix;
			by = iy;
			Found = CurID;
		}
		if (CurZ >= 0.)
			break;
	}
}

void DXDirection5X::FillPointInf(const DXMemoryPtOnePart* pActMemory, int bx, int by, DXMemID Found, MBodyPointInf& Inf) const
{
	if (bx < 0 || by < 0 || Found == DXP_END)
		return;
	Inf.ElemID_ = pActMemory->GetAtID(Found).Elem;
	Inf.HistCadrID_ = pActMemory->GetAtID(Found).Cadr;
	const MElemIDProc* pProc = MFOREST.GetIDProc();
	const int SurfNum = pProc->GetSurfID(Inf.ElemID_);
	float BUF[3];
	if (SurfNum < 0 )
	{
		pProc->GetNormalFromSurfID(SurfNum, BUF);
	}
	else
	{
		SOrigSurf* pSurf = MFOREST.GetOrigSurf(SurfNum);
		if (pSurf == nullptr)
		{
			*(BUF) = 0.;
			*(BUF + 1) = 0.;
			*(BUF + 2) = -1.;
		}
		else
			pSurf->GetNormalFast(BUF);
	}
	Inf.Nr_.Set(BUF[0], BUF[1], BUF[2], 0.);
	const double c0 = Min0 + Step0 * bx;
	const double c1 = Min1 + Step1 * by;
	const double c2 = pActMemory->GetAtZ(Found) + MinVal;
	switch (Dir)
	{
	case Z:
		Inf.Pt_.Set(c0, c1, c2, 1.);
		break;
	case X:
		Inf.Pt_.Set(c2, c0, c1, 1.);
		break;
	case Y:
		Inf.Pt_.Set(c1, c2, c0, 1.);
		break;
	}
//	Inf.HistCadrID_ = pActMemory->GetAtHC(Found);
}

//очистка всех значений
void DXDirection5X::ClearAllValues(DXMemoryPtOnePart *pActMemory, double Val)
{
	if(!IsDefined())
		return;
	SetUpLimit(Val);
	BPoint PlaneNormal;
	BPoint PlanePoint;
	BPoint MinVec;
	switch (Dir)
	{
	case Z:
		PlaneNormal.Set(0., 0., -1., 0.);
		PlanePoint.Set(0., 0., Val, 1.);
		MinVec.Set(Min0, Min1, MinVal, 0.);
		break;
	case X:
		PlaneNormal.Set(-1., 0., 0., 0.);
		PlanePoint.Set(Val, 0., 0., 1.);
		MinVec.Set(MinVal, Min0, Min1, 0.);
		break;
	case Y:
		PlaneNormal.Set(0., -1., 0., 0.);
		PlanePoint.Set(0., Val, 0., 1.);
		MinVec.Set(Min1, MinVal, Min0, 0.);
		break;
	}
	ElemID IDup = 0;
	MElemIDProc::SetSurfID(&IDup, MFOREST.AddOrigSurfInside(new SOPlane(PlaneNormal, PlanePoint + MinVec)));
	ElemID IDdown = 0;
	MElemIDProc::SetSurfID(&IDdown, MFOREST.AddOrigSurfInside(new SOPlane(PlaneNormal * (-1.), BPoint(0., 0., 0., 1.) + MinVec)));
	double MinValT = 1.e-4; //1.e-1; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! заменить значение на 0., чтобы пододвинуть модель в ноль. (в начало отсчета системы координат), а именно ее левый нижний угол
	cadrID Cadr(-1);
	for (int i0 = 0; i0 < n0; ++i0)
		for (int i1 = 0; i1 < n1; ++i1)
		{
			if (i0 == 0 || i1 == 0 || i0 == n0 -1 || i1 == n1 - 1)
			{
			    Dexels[i0][i1] = DXP_END;
			}
			else
			{
				DXMemID P = pActMemory->FindEmptyPos();
				pActMemory->SetAtNext(P, DXP_END);
			    pActMemory->SetAt(P, DX_DEPTH(MinValT), DX_ID(IDdown, Cadr));
			    //Dexels[i0][i1] = P; 
				SetDexelStartPoint(i0, i1, P);
			    P = pActMemory->InsertAfter(P);
			    pActMemory->SetAt(P, DX_DEPTH(Val - MinValT), DX_ID(IDup, Cadr));
			}
		}
}

void DXDirection5X::MakeEmpty()
{
	int ln1 = n1;
	for (int i0 = 0; i0 < n0; ++i0)
//#pragma loop(ivdep)
//#pragma loop(hint_parallel(8))
		for (int i1 = 0; i1 < ln1; ++i1)
		    Dexels[i0][i1] = DXP_END;
}
//определение
bool DXDirection5X::IsDefined() const
{
	return true;
}

int DXDirection5X::Store(DXMemoryPtOnePart *pActMemory, const DXDirection3X & Dir3X)
{
	// It is assumed that all Dexels[*][*] == DXP_END
	if(Dir3X.GetN0() != n0 || Dir3X.GetN1() != n1 || Dir3X.Dir != Dir)
		return -1;
	for(int i0 = Dir3X.SubWin.ixStart; i0 < Dir3X.SubWin.ixEnd; ++i0)
	{
		DX_DEPTH *pColumnDB = Dir3X.GetColumnD(0, i0);
		DX_DEPTH *pColumnDT = Dir3X.GetColumnD(1, i0);
		DX_ID *pColumnIB = Dir3X.GetColumnI(0, i0);
		DX_ID *pColumnIT = Dir3X.GetColumnI(1, i0);
		for(int i1 = Dir3X.SubWin.iyStart; i1 < Dir3X.SubWin.iyEnd; ++i1)
		{
			DX_DEPTH Z3T = pColumnDT[i1];
			DX_DEPTH Z3B = pColumnDB[i1];
			bool EmptyT = (Z3T == DX_DEPTH_MIN);
			bool EmptyB = (Z3B == DX_DEPTH_MAX);
			if(EmptyT || EmptyB)
				continue;
			DXMemID P = pActMemory->FindEmptyPos();
			//Dexels[i0][i1] = P;
			SetDexelStartPoint(i0, i1, P);
			SetNewValues(i0, i1, pActMemory, P, pColumnDB[i1], pColumnIB[i1]);
			DXMemID PN = pActMemory->FindEmptyPos();
			pActMemory->SetAtNext(P, PN);
			SetNewValues(i0, i1, pActMemory, PN, pColumnDT[i1], pColumnIT[i1]);
			pActMemory->SetAtNext(PN, DXP_END);
		}
	}
	return 0;
}

//вычитание
int DXDirection5X::Subtract(DXMemoryPtOnePart *pActMemory, DXDirection3X & Dir3X)
{

	if(Dir3X.GetN0() != n0 || Dir3X.GetN1() != n1 || Dir3X.Dir != Dir)
		return -1;
	if (!Dir3X.SubWin.IsDefined())
		return 0;
	for(int i0 = Dir3X.SubWin.ixStart; i0 < Dir3X.SubWin.ixEnd; ++i0)
	{
		DX_DEPTH *pColumnDB = Dir3X.GetColumnD(0, i0);
		DX_DEPTH *pColumnDT = Dir3X.GetColumnD(1, i0);
		DX_ID *pColumnIB = Dir3X.GetColumnI(0, i0);
		DX_ID *pColumnIT = Dir3X.GetColumnI(1, i0);
		DXMemID *pColumnP5 = Dexels[i0];
		for(int i1 = Dir3X.SubWin.iyStart; i1 < Dir3X.SubWin.iyEnd; ++i1)
		{
			DX_DEPTH Z3T = pColumnDT[i1];
			DX_DEPTH Z3B = pColumnDB[i1];
			bool EmtyT = (Z3T == DX_DEPTH_MIN);
			bool EmtyB = (Z3B == DX_DEPTH_MAX);
			if( EmtyT && EmtyB)
				continue;
			// Clear 3X direction
			pColumnDT[i1] = DX_DEPTH_MIN;
			pColumnDB[i1] = DX_DEPTH_MAX;
			//
			DXMemID P5Start = pColumnP5[i1];
			if(P5Start == DXP_END) // Empty dexel 
				continue;
			if(EmtyT)
				Z3T = DX_DEPTH_MAX;
			if(EmtyB)
				Z3B = DX_DEPTH_MIN;
			// Position DX3 dexel against DX5 dexel
			int IZ5B = 0;
			DXMemID P5B = DXP_END;
			bool EndDex = false;
			for(DXMemID P5Next = P5Start; !EndDex; ++IZ5B, EndDex = (P5Next == DXP_END))
			{
				if(pActMemory->GetAtZ(P5Next) > Z3B)
					break;
				P5B = P5Next;
				P5Next = pActMemory->GetAtNext(P5Next);
			}
			if(EndDex)
				continue;
			int IZ5T = 0;
			DXMemID P5T = DXP_END;
			for(DXMemID P5Next = P5Start; P5Next != DXP_END; ++IZ5T)
			{
				if(pActMemory->GetAtZ(P5Next) > Z3T)
					break;
				P5T = P5Next;
				P5Next = pActMemory->GetAtNext(P5Next);
			}
			if(IZ5T <= 0)
				continue;

			//
			if(IZ5T == IZ5B) // DX3 dexel lies in otr or in (empty) interval
			{
				if(IZ5B % 2 == 0) // DX3 dexel lies in (empty) interval 	
					continue;
				DXMemID P5New = pActMemory->InsertAfter(P5B);
				SetNewValues(i0, i1, pActMemory, P5New, Z3B, pColumnIB[i1]);
				P5New = pActMemory->InsertAfter(P5New);
				SetNewValues(i0, i1, pActMemory, P5New, Z3T, pColumnIT[i1]);
			}
			else
			{
				// DX3 dexel touches more than one otr or interval
				if(IZ5B % 2 != 0) // Bottom of otr
				{ // Replace the top of this otr by Z3B
					DXMemID P5BEnd = pActMemory->GetAtNext(P5B);
					SetChanged(i0, i1, pActMemory->GetAtZ(P5BEnd));
					SetNewValues(i0, i1, pActMemory, P5BEnd, Z3B, pColumnIB[i1]);
				}
				if(IZ5T % 2 != 0) // Bottom of otr
				{ // Replace the bottom of this otr by Z3T
					SetChanged(i0, i1, pActMemory->GetAtZ(P5T));
					SetNewValues(i0, i1, pActMemory, P5T, Z3T, pColumnIT[i1]);
				}
				// Delete superfluous otrs
				DXMemID P5Del0 = P5B;
				++IZ5B;
				if(IZ5B % 2 == 0)
				{
					P5Del0 = pActMemory->GetAtNext(P5Del0);
					++IZ5B;
				}// P5Del0 points now to the top of the last otr before otrs to delete
				// IZ5B == 1 -> deletion started at first otr
				if(IZ5B < IZ5T)
				{
					DXMemID Del = (IZ5B == 1) ? P5Start : pActMemory->GetAtNext(P5Del0);
					for(; IZ5B < IZ5T && Del != DXP_END; IZ5B += 2)
					{
						RemoveOldValues(i0, i1, pActMemory, Del);
						Del = pActMemory->GetAtNext(Del);
						RemoveOldValues(i0, i1, pActMemory, Del);
						Del = pActMemory->GetAtNext(Del);
					}
					if(P5Del0 == DXP_END)// deletion started at first otr
						pColumnP5[i1] = Del;
					else
						pActMemory->SetAtNext(P5Del0, Del);
				}
			}
		}
	}
	Dir3X.ClearWindow();
	return 0;
}

int DXDirection5X::Subtract(DXMemoryPtOnePart *pActMemory, DXDirection5X & Dir5X)
{
	// This method doesn't use FindEmptyPos
	if(Dir5X.GetN0() != n0 || Dir5X.GetN1() != n1 || Dir5X.Dir != Dir)
		return -1;
	for(int i0 = Dir5X.SubWin.ixStart; i0 < Dir5X.SubWin.ixEnd; ++i0)
	{
		DXMemID *pColumnP5 = Dexels[i0];
		for(int i1 = Dir5X.SubWin.iyStart; i1 < Dir5X.SubWin.iyEnd; ++i1)
		{
			DXMemID P5Start = pColumnP5[i1];
			DXMemID Dir5XStart = Dir5X.Dexels[i0][i1];
			if (P5Start == DXP_END) // Empty dexel 
			{
				for (DXMemID Dir5XNext = Dir5XStart; Dir5XNext != DXP_END;)
				{
					DXMemID B = pActMemory->GetAtNext(Dir5XNext);
					pActMemory->RemoveAt(Dir5XNext); // Clear Dir5X
					Dir5XNext = B;
				}
				continue;
			}

			for(DXMemID Dir5XNext = Dir5XStart; Dir5XNext != DXP_END;)
			{
				if (P5Start == DXP_END) // Empty dexel. P5Start can be changed in this loop
					break;
				DX_DEPTH Z3B = pActMemory->GetAtZ(Dir5XNext);
				DX_ID I3B = pActMemory->GetAtID(Dir5XNext);
				DXMemID B = pActMemory->GetAtNext(Dir5XNext);
				//pActMemory->RemoveAt(Dir5XNext); // Clear Dir5X
				DXMemID Dir5B = Dir5XNext; // ID of the bottom in Dir5X
				Dir5XNext = B;
				if(Dir5XNext == DXP_END)// internal error
					break;
				DX_DEPTH Z3T = pActMemory->GetAtZ(Dir5XNext);
				DX_ID I3T = pActMemory->GetAtID(Dir5XNext);
				B = pActMemory->GetAtNext(Dir5XNext); // Clear Dir5X
				//pActMemory->RemoveAt(Dir5XNext); // Clear Dir5X
				DXMemID Dir5T = Dir5XNext; // ID of the top in Dir5X
				Dir5XNext = B;

				if (Z3T <= Z3B)
				{
					pActMemory->RemoveAt(Dir5B); // Clear Dir5X
					pActMemory->RemoveAt(Dir5T); // Clear Dir5X
					continue;
				}

			// Position new pair against DX5 dexel
				int IZ5B = 0;
				DXMemID P5B = DXP_END;
				bool EndDex = false;
				for(DXMemID P5Next = P5Start; !EndDex; ++IZ5B, EndDex = (P5Next == DXP_END))
				{
					double Val = pActMemory->GetAtZ(P5Next);
					if(Val > Z3B)
						break;
					if (Val == Z3B)
						if (IZ5B % 2 == 0)// start point
							break;
					P5B = P5Next;
					P5Next = pActMemory->GetAtNext(P5Next);
				}
				if(EndDex)
				{
					pActMemory->RemoveAt(Dir5B); // Clear Dir5X
					pActMemory->RemoveAt(Dir5T); // Clear Dir5X
					continue;
				}

				int IZ5T = 0;
				DXMemID P5T = DXP_END;
				for(DXMemID P5Next = P5Start; P5Next != DXP_END; ++IZ5T)
				{
					double Val = pActMemory->GetAtZ(P5Next);
					if (Val > Z3T)
						break;
					if (Val == Z3T)
						if (IZ5T % 2 == 0)// start point
							break;
					P5T = P5Next;
					P5Next = pActMemory->GetAtNext(P5Next);
				}
				if(IZ5T <= 0)
				{
					pActMemory->RemoveAt(Dir5B); // Clear Dir5X
					pActMemory->RemoveAt(Dir5T); // Clear Dir5X
					continue;
				}


				//
				if(IZ5T == IZ5B) // DX3 dexel lies in otr or in (empty) interval
				{
					if(IZ5B % 2 == 0) // DX3 dexel lies in (empty) interval 	
					{
						pActMemory->RemoveAt(Dir5B); // Clear Dir5X
						pActMemory->RemoveAt(Dir5T); // Clear Dir5X
						continue;
					}

					SetChanged(i0, i1, Z3B);
					SetChanged(i0, i1, Z3T);
					pActMemory->InsertAfter(P5B, Dir5B);
					pActMemory->InsertAfter(Dir5B, Dir5T);
				}
				else
				{
					pActMemory->RemoveAt(Dir5B); // Clear Dir5X
					pActMemory->RemoveAt(Dir5T); // Clear Dir5X

					// DX3 dexel touches more than one otr or interval
					if (IZ5B % 2 != 0) // P5B points to Bottom of otr
					{
						 // Replace the top of this otr by Z3B
						DXMemID P5BEnd = pActMemory->GetAtNext(P5B);
						auto& ZValEx = pActMemory->GetAtZRef(P5BEnd);
						if (ZValEx != Z3B)
						{
							SetChanged(i0, i1, ZValEx);
							SetChanged(i0, i1, Z3B);
							ZValEx = Z3B;
							pActMemory->SetAtID(P5BEnd, I3B);
						}
					}
					if(IZ5T % 2 != 0) // P5T points to Bottom of otr
					{ // Replace the bottom of this otr by Z3T
						auto &ZValEx = pActMemory->GetAtZRef(P5T);
						if (ZValEx != Z3T)
						{
							SetChanged(i0, i1, ZValEx);
							SetChanged(i0, i1, Z3T);
							ZValEx = Z3T;
							pActMemory->SetAtID(P5T, I3T);
						}
					}
					// Delete superfluous otrs
					DXMemID P5Del0 = P5B;
					++IZ5B;
					if (IZ5B % 2 == 0)
					{
						P5Del0 = pActMemory->GetAtNext(P5Del0);
						++IZ5B;
					}
					// P5Del0 points now to the top of the last otr before otrs to delete
					// IZ5B == 1 -> deletion started at first otr
					if(IZ5B < IZ5T)
					{
						DXMemID Del = (IZ5B == 1) ? P5Start : pActMemory->GetAtNext(P5Del0);
						for(; IZ5B < IZ5T && Del != DXP_END; IZ5B += 2)
						{
							RemoveOldValues(i0, i1, pActMemory, Del);
							Del = pActMemory->GetAtNext(Del);
							RemoveOldValues(i0, i1, pActMemory, Del);
							Del = pActMemory->GetAtNext(Del);
						}
						if (P5Del0 == DXP_END)// deletion started at first otr
						{
							pColumnP5[i1] = Del;
							P5Start = Del;
						}
						else
							pActMemory->SetAtNext(P5Del0, Del);
					}
				}
			}
			// DEBUG
			//bool EndDex = false;
			//double n = 0.;
			//for (DXMemID P5Next = pColumnP5[i1]; !EndDex; EndDex = (P5Next == DXP_END))
			//{
			//	ASSERT(pActMemory->GetAtFlag(P5Next) == false);
			//	double p = pActMemory->GetAtZ(P5Next);
			//	if (p == n)
			//		break;
			//	auto P5B = P5Next;
			//	P5Next = pActMemory->GetAtNext(P5Next);
			//	n = p;
			//}
			// END:DEBUG

		}
	}
	for(int i0 = Dir5X.SubWin.ixStart; i0 < Dir5X.SubWin.ixEnd; ++i0) // Clear Dir5X
		for(int i1 = Dir5X.SubWin.iyStart; i1 < Dir5X.SubWin.iyEnd; ++i1)
				Dir5X.Dexels[i0][i1] = DXP_END;
	Dir5X.ClearWindow();
	return 0;
}

int DXDirection5X::Intersect(DXMemoryPtOnePart *pActMemory, DXDirection5X & Dir5X)
{
	if (Dir5X.GetN0() != n0 || Dir5X.GetN1() != n1 || Dir5X.Dir != Dir)
		return -1;
	for (int i0 = Dir5X.SubWin.ixStart; i0 < Dir5X.SubWin.ixEnd; ++i0)
	{
		DXMemID *pColumnP5 = Dexels[i0];
		for (int i1 = Dir5X.SubWin.iyStart; i1 < Dir5X.SubWin.iyEnd; ++i1)
		{
			DXMemID P5Start = pColumnP5[i1];
			if (P5Start == DXP_END) // Empty dexel 
				continue;

			DXMemID Dir5XStart = Dir5X.Dexels[i0][i1];
			if (Dir5XStart == DXP_END)
			{// Remove P5Start dexel
				for (; P5Start != DXP_END; )
				{
					DXMemID B = pActMemory->GetAtNext(P5Start); // Clear P5Start
					pActMemory->RemoveAt(P5Start); // Clear P5Start
					P5Start = B;
				}
				pColumnP5[i1] = DXP_END;
				continue;
			}

			for (DXMemID Dir5XNext = Dir5XStart; Dir5XNext != DXP_END;)
			{
				DX_DEPTH Z3B;
				DX_ID I3B;
				DX_DEPTH Z3T;
				DX_ID I3T;
				if (Dir5XNext == Dir5XStart)
				{
					Z3B = DX_DEPTH(-1.);
					I3B = DX_ID(0, cadrID(-1));
					Z3T = pActMemory->GetAtZ(Dir5XNext);
					I3T = pActMemory->GetAtID(Dir5XNext);
					DXMemID B = pActMemory->GetAtNext(Dir5XNext); // Clear Dir5X
					pActMemory->RemoveAt(Dir5XNext); // Clear Dir5X
					Dir5XNext = B;
				}
				else
				{
					Z3B = pActMemory->GetAtZ(Dir5XNext);
					I3B = pActMemory->GetAtID(Dir5XNext);
					DXMemID B = pActMemory->GetAtNext(Dir5XNext);
					pActMemory->RemoveAt(Dir5XNext); // Clear Dir5X
					Dir5XNext = B;
					if (Dir5XNext == DXP_END)
					{
						Z3T = DX_DEPTH(UpLimit);
						I3T = DX_ID(0, cadrID(-1));
					}
					else
					{
						Z3T = pActMemory->GetAtZ(Dir5XNext);
						I3T = pActMemory->GetAtID(Dir5XNext);
						B = pActMemory->GetAtNext(Dir5XNext); // Clear Dir5X
						pActMemory->RemoveAt(Dir5XNext); // Clear Dir5X
						Dir5XNext = B;
					}
				}

				if (Z3T <= Z3B)
					continue;

				// Position new pair against DX5 dexel
				int IZ5B = 0;
				DXMemID P5B = DXP_END;
				bool EndDex = false;
				for (DXMemID P5Next = P5Start; !EndDex; ++IZ5B, EndDex = (P5Next == DXP_END))
				{
					if (pActMemory->GetAtZ(P5Next) > Z3B)
						break;
					P5B = P5Next;
					P5Next = pActMemory->GetAtNext(P5Next);
				}
				if (EndDex)
					continue;
				int IZ5T = 0;
				DXMemID P5T = DXP_END;
				for (DXMemID P5Next = P5Start; P5Next != DXP_END; ++IZ5T)
				{
					if (pActMemory->GetAtZ(P5Next) > Z3T)
						break;
					P5T = P5Next;
					P5Next = pActMemory->GetAtNext(P5Next);
				}
				if (IZ5T <= 0)
					continue;

				//
				if (IZ5T == IZ5B) // DX3 dexel lies in otr or in (empty) interval
				{
					if (IZ5B % 2 == 0) // DX3 dexel lies in (empty) interval 	
						continue;
					DXMemID P5New = pActMemory->InsertAfter(P5B);
					SetNewValues(i0, i1, pActMemory, P5New, Z3B, I3B);
					P5New = pActMemory->InsertAfter(P5New);
					SetNewValues(i0, i1, pActMemory, P5New, Z3T, I3T);
				}
				else
				{
					// DX3 dexel touches more than one otr or interval
					if (IZ5B % 2 != 0) // Bottom of otr
					{ // Replace the top of this otr by Z3B
						DXMemID P5BEnd = pActMemory->GetAtNext(P5B);
						SetChanged(i0, i1, pActMemory->GetAtZ(P5BEnd));
						SetNewValues(i0, i1, pActMemory, P5BEnd, Z3B, I3B);
					}
					if (IZ5T % 2 != 0) // Bottom of otr
					{ // Replace the bottom of this otr by Z3T
						SetChanged(i0, i1, pActMemory->GetAtZ(P5T));
						SetNewValues(i0, i1, pActMemory, P5T, Z3T, I3T);
					}
					// Delete superfluous otrs
					DXMemID P5Del0 = P5B;
					++IZ5B;
					if (IZ5B % 2 == 0)
					{
						P5Del0 = pActMemory->GetAtNext(P5Del0);
						++IZ5B;
					}// P5Del0 points now to the top of the last otr before otrs to delete
					// IZ5B == 1 -> deletion started at first otr
					if (IZ5B < IZ5T)
					{
						DXMemID Del = (IZ5B == 1) ? P5Start : pActMemory->GetAtNext(P5Del0);
						for (; IZ5B < IZ5T && Del != DXP_END; IZ5B += 2)
						{
							RemoveOldValues(i0, i1, pActMemory, Del);
							Del = pActMemory->GetAtNext(Del);
							RemoveOldValues(i0, i1, pActMemory, Del);
							Del = pActMemory->GetAtNext(Del);
						}
						if (P5Del0 == DXP_END)// deletion started at first otr
							pColumnP5[i1] = Del;
						else
							pActMemory->SetAtNext(P5Del0, Del);
					}
				}
			}
		}
	}
	for (int i0 = Dir5X.SubWin.ixStart; i0 < Dir5X.SubWin.ixEnd; ++i0) // Clear Dir5X
		for (int i1 = Dir5X.SubWin.iyStart; i1 < Dir5X.SubWin.iyEnd; ++i1)
			Dir5X.Dexels[i0][i1] = DXP_END;
	Dir5X.ClearWindow();
	return 0;
}

//тест вычитания
//void DXDirection5X::TestSubtract(void)
//{
	//DXDirection5X Dir5X;
	////Dir5X.Init(Y, 2, 2, 10., 10., 2, 2);
	//Dir5X.Init(Y, 2, 2, 10., 10., 2, 2, 0, 0, 0);
	//
	//Dir5X.ClearAllValues(5.);
	//DXDirection3X Dir3X;
	////Dir3X.Init(Y, 2, 2, 10., 10., 2, 2);
	//Dir3X.Init(Y, 2, 2, 10., 10., 2, 2, 0, 0, 0);
	//
	//Dir3X.ClearAllValues(5., 0);
	////
	//Dir3X.GetColumnD(0, 0)[0] = 0.2;
	//Dir3X.GetColumnD(1, 0)[0] = 0.6;
	//Dir5X.Subtract(Dir3X);
	//Dir3X.GetColumnD(0, 0)[0] = 0.1;
	//Dir3X.GetColumnD(1, 0)[0] = 0.15;
	//Dir5X.Subtract(Dir3X);
	//Dir3X.GetColumnD(0, 0)[0] = 0.05;
	//Dir3X.GetColumnD(1, 0)[0] = 0.7;
	//Dir5X.Subtract(Dir3X);
//}
//дать начальную точку dexel
DXMemID DXDirection5X::GetDexelStartPoint(int i0, int i1) const
{
	if(!Dexels)
		return DXP_END;
	return Dexels[i0][i1];
}
//проверка
bool DXDirection5X::Check(const DXMemoryPtOnePart *pActMemory) const
{
	for(int i0 = 0; i0 < n0; ++i0)
		for(int i1 = 0; i1 < n1; ++i1)
		{
			int Count = 0;
			for (DXMemID P = Dexels[i0][i1]; P != DXP_END && Count < 99; P = pActMemory->GetAtNext(P), ++Count)
			{
				double Z = pActMemory->GetAtZ(P);
				if (Z < 0. || Z > UpLimit)
					return false;
			}
			if(Count % 2 != 0)
				return false;
		}
	return true;
}

bool DXDirection5X::Heal(DXMemoryPtOnePart *pActMemory)
{
	for(int i0 = 0; i0 < n0; ++i0)
		for(int i1 = 0; i1 < n1; ++i1)
		{
			int Count = 0;
			for(DXMemID P = Dexels[i0][i1]; P != DXP_END && Count < 99; P = pActMemory->GetAtNext(P), ++Count);
			if(Count % 2 != 0)
			{// Bad dexel
				for(DXMemID P = Dexels[i0][i1]; P != DXP_END ; )
				{
					DXMemID PNext = pActMemory->GetAtNext(P);
					RemoveOldValues(i0, i1, pActMemory, P);
					P = PNext;
				}
				Dexels[i0][i1] = DXP_END;
			}
		}
	return true;
}

void DXDirection5X::MarkUsedOrig(DXMemoryPtOnePart *pActMemory, BDynBitArray & Used)
{
	for(int i0 = 0; i0 < n0; ++i0)
		for(int i1 = 0; i1 < n1; ++i1)
		{
			for(DXMemID P = Dexels[i0][i1]; P != DXP_END; P = pActMemory->GetAtNext(P))
			{
				ElemID ID = pActMemory->GetAtID(P).Elem;
				int SurfNum = MFOREST.GetIDProc()->GetSurfID(ID);
				if (SurfNum >= 0 && SurfNum != ID_EMPTY)
				{
					if (!MElemIDProc::IsSurfIndEmpty(SurfNum))
						Used.SetTrueAt(SurfNum);
				}
			}
		}
}

int DXDirection5X::InsOneDexel(DXMemoryPtOnePart *pActMemory, int i0, int i1, DX_DEPTH *ZVal, DX_ID *IDVal, size_t Count)
{
	if(Count == 0)
		return 0;
	DXMemID P = pActMemory->FindEmptyPos();
	SetDexelStartPoint(i0, i1, P);
	if (P == DXP_END)
		return -1;
	DXMemID CurID = P;
	for(size_t i = 0; i < Count; ++i)
	{
		SetNewValues(i0, i1, pActMemory, CurID, ZVal[i] - DX_DEPTH(MinVal), IDVal[i]);
		if (i == Count - 1)
		{
			P = DXP_END;
			pActMemory->SetAtNext(CurID, P);
		}
		else
		{
			P = pActMemory->FindEmptyPos();
			pActMemory->SetAtNext(CurID, P);
			if (P == DXP_END)
				return -1;
		}
		CurID = P;
	}
	return 0;
}

int DXDirection5X::InsOneDexel(DXMemoryPtOnePart* pActMemory, int i0, int i1, const std::list<std::pair<BPoint, unsigned int>>& Nodes, double (BPoint::* GetVal)(void) const)
{
	if (Nodes.empty())
		return 0;
	DXMemID P = pActMemory->FindEmptyPos();
	SetDexelStartPoint(i0, i1, P);
	if (P == DXP_END)
		return -1;
	DXMemID CurID = P;
	for each(const auto& Node in Nodes)
	{
		SetNewValues(i0, i1, pActMemory, CurID, static_cast<DX_DEPTH>((Node.first.*GetVal)()) - DX_DEPTH(MinVal), DX_ID(Node.second, cadrID(-1)));
		if (Node == Nodes.back())
		{
			P = DXP_END;
			pActMemory->SetAtNext(CurID, P);
		}
		else
		{
			P = pActMemory->FindEmptyPos();
			pActMemory->SetAtNext(CurID, P);
			if (P == DXP_END)
				return -1;
		}
		CurID = P;
	}
	return 0;
}


int DXDirection5X::InsOnePoint(DXMemoryPtOnePart *pActMemory, int i0, int i1, DX_DEPTH ZVal, DX_ID IDVal)
{
	DXMemID P = pActMemory->FindEmptyPos();
	if(P == DXP_END)
		return -1;
	DXMemID Start = GetDexelStartPoint(i0, i1);
	pActMemory->SetAt(P, ZVal, IDVal);
	if(Start == DXP_END || pActMemory->GetAtZ(Start) > ZVal + MIND)
	{
		SetDexelStartPoint(i0, i1, P);
		pActMemory->SetAtNext(P, Start);
		return 0;
	}
	for(DXMemID CurID = Start; ; )
	{
		DXMemID NextID = pActMemory->GetAtNext(CurID);
		if(NextID == DXP_END || pActMemory->GetAtZ(NextID) > ZVal + MIND)
		{
			if(fabs(pActMemory->GetAtZ(CurID) - ZVal) > MIND)
				pActMemory->InsertAfter(CurID, P);
			break;
		}
		CurID = NextID;
	}
	return 0;
}

DX_ID DXDirection5X::RastTriangle(DXMemoryPtOnePart *pActMemory, const BPoint & P0v, const BPoint & P1v, const BPoint & P2v, const BPoint & iN, DX_ID PairID)
{
	static const double MIN_COS = cos(PI * 0.5 - MINAR * 0.004);
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
	// Ensure unique
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
			t = min(1., t);
			t = max(0., t);
			S[kk].YstTop = Pts[OTop.NPb]->GetY() + t * (Pts[OTop.NPe]->GetY() - Pts[OTop.NPb]->GetY());

			const Otr &OBot = Otrs[S[kk].NBot]; 
			zdX = (X[OBot.NPe] - X[OBot.NPb]);
			if(zdX != 0.)
				zdX = 1. / zdX;
			S[kk].dYdXBot = zdX * (Pts[OBot.NPe]->GetY() - Pts[OBot.NPb]->GetY()) * GetStep0();
			S[kk].dZdXBot = zdX * (Pts[OBot.NPe]->GetZ() - Pts[OBot.NPb]->GetZ()) * GetStep0();
			t = (S[kk].iXb * GetStep0() - X[OBot.NPb]) * zdX;
			t = min(1., t);
			t = max(0., t);
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
					for(int iYCur = iyMin; iYCur <= iyMax; ++iYCur)
					{
						if(iYCur >= 0)
						{
							InsOnePoint(pActMemory, iXCur, iYCur, DX_DEPTH(ZCur), PairID);
						}
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

void DXDirection5X::MakeBox(DXMemoryPtOnePart* pActMemory, const BBox& Box)
{
	if (!IsDefined())
		return;
	double Val = 0., Delta = 0.;
	BPoint PlaneNormal;
	switch (Dir)
	{
	case Z:
		PlaneNormal.Set(0., 0., -1., 0.);
		Val = Box.dZ();
		Delta = Box.GetMinPoint().GetZ() - MinVal;
		break;
	case X:
		PlaneNormal.Set(-1., 0., 0., 0.);
		Val = Box.dX();
		Delta = Box.GetMinPoint().GetX() - MinVal;
		break;
	case Y:
		PlaneNormal.Set(0., -1., 0., 0.);
		Val = Box.dY();
		Delta = Box.GetMinPoint().GetY() - MinVal;
		break;
	}
	SetUpLimit(Val + Delta);
	ElemID IDup = 0;
	MElemIDProc::SetSurfID(&IDup, MFOREST.AddOrigSurfInside(new SOPlane(PlaneNormal, Box.GetMaxPoint())));
	ElemID IDdown = 0;
	MElemIDProc::SetSurfID(&IDdown, MFOREST.AddOrigSurfInside(new SOPlane(PlaneNormal * (-1.), Box.GetMinPoint())));
	cadrID Cadr(-1);
	for (int i0 = 0; i0 < n0; ++i0)
		for (int i1 = 0; i1 < n1; ++i1)
		{
			if (i0 == 0 || i1 == 0 || i0 == n0 - 1 || i1 == n1 - 1)
			{
				Dexels[i0][i1] = DXP_END;
			}
			else
			{
				DXMemID P = pActMemory->FindEmptyPos();
				pActMemory->SetAtNext(P, DXP_END);
				pActMemory->SetAt(P, DX_DEPTH(Delta), DX_ID(IDdown, Cadr));
				//Dexels[i0][i1] = P; 
				SetDexelStartPoint(i0, i1, P);
				P = pActMemory->InsertAfter(P);
				pActMemory->SetAt(P, DX_DEPTH(Val + Delta), DX_ID(IDup, Cadr));
			}
		}
}

void DXDirection5X::MakeFromBSP(DXMemoryPtOnePart *pActMemory, const MBSPNode *pTree, MQuadTree& QTree)
{
	double (BPoint::*GetVal)(void) const = &BPoint::GetZ;
	switch (Dir)
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
	MakeEmpty();
	BMatr ShiftI = Shift.invr();
	BPoint VDir(0., 0., 1., 0.);
	VDir = VDir * ShiftI;
	MBSPNode::StackForShooting St;
	std::list<std::pair<BPoint, unsigned int>> Nodes;// intersection point and ID
	for(int i0 = 0; i0 < GetN0(); ++i0)
	{
		const double v0 = GetStep0() * i0;// + Min0;
		for(int i1 = 0; i1 < GetN1(); ++i1)
		{
			const double v1 = GetStep1() * i1;// + Min1;
			BPoint BaseP(v0, v1, -1., 1.);
			BaseP = BaseP * ShiftI;
			pTree->RayShreddingSpec(BaseP, VDir, QTree, St, Nodes);
			InsOneDexel(pActMemory, i0, i1, Nodes, GetVal);
			Nodes.clear();
		}
	}
}

void DXDirection5X::MakeFromQRendMill(DXMemoryPtOnePart* pActMemory, const GQuadRenderMill* pRendMill)
{
	double (BPoint:: * GetVal)(void) const = &BPoint::GetZ;
	switch (Dir)
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
	MakeEmpty();
	for (unsigned int ix = 0; ix < pRendMill->GetColsNum(); ++ix)
		for (unsigned int iy = 0; iy < pRendMill->GetRowsNum(); ++iy)
		{
			GLfloat* pC = pRendMill->GetArray()[ix][iy];
			if (GQuadRenderMill::Empty(pC))
				continue;
			for (int it = 0; it < int(pC[0]); ++it)
			{
				GLfloat* b = (GLfloat*)pC + 1 + it * 3 * 7;// See StoreTriangle
				BPoint p0(0), p1(0), p2(0);
				GQuadRenderMill::ExtractTriangle(b, p0, p1, p2);
				ElemID elemID;
				GQuadRenderMill::ExtractID(pC, it, elemID);
				BPoint no(b[0], b[1], b[2], 0.);// Normal in the vertex
				//Calculate normal
				BPoint n = (p1 - p0) % (p2 - p0);
				n = n * ((n * no < 0) ? -1. : 1.);
				RastTriangle(pActMemory, p0, p1, p2, n, DX_ID(elemID, cadrID(-1)));
			}
		}
}

