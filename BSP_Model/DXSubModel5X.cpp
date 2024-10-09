#include "..\Common\DXSubModel5X.h"
#include "..\Common\DXSubModel5X.h"
#include "stdafx.h"
#include "math.h"
#include "ppl.h"
#include "MBSPForest.h"
#include "MBSPNode.h"
#include "ConstDef.h"
#include "SOSphere.h"
#include "SOPlane.h"
#include "SOCyl.h"
#include "SOECyl.h"
#include "MBSPMerge.h"
#include "MBodyParams.h"
#include "MElemIDProc.h"
#include "MFace.h"
#include "DXDirection3X.h"
#include "CadrData.h"
#include "MCubeMask.h"
#include "MForestSave.h"
#include "DXSubModel5X.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

const int F_C0C1 = 0x0F;
const double SMALLVAL = 1e-4; // величина, которую добавляем, когда точка попадает в вершину ячейки
//конструктор
DXSubModel5X::DXSubModel5X(/*const*/ DXModel &ParentModel, BStockGrID Position)
	: DXSubModel(ParentModel, Position)
{
	StoreInA = true;
	ReCreate();
}

//деструктор
DXSubModel5X::~DXSubModel5X(void)
{

}
//очистка
void DXSubModel5X::Reset()
{
	ClearMemory();
	Directions[0].ClearAllValues(&GetMemory(0), Parent.GGabar.dZ());//!PARALLEL
	Directions[1].ClearAllValues(&GetMemory(1), Parent.GGabar.dY());
	Directions[2].ClearAllValues(&GetMemory(2), Parent.GGabar.dX());
	DirectionsA[0].MakeEmpty();
	DirectionsA[1].MakeEmpty();
	DirectionsA[2].MakeEmpty();
	AuxDirs[0].RemoveWindow();
	AuxDirs[1].RemoveWindow();
	AuxDirs[2].RemoveWindow();
	AuxAuxDirs[0].Clear();
	AuxAuxDirs[1].Clear();
	AuxAuxDirs[2].Clear();
	AuxAuxDirs[0].SetUpLimit(Parent.GGabar.dZ());
	AuxAuxDirs[1].SetUpLimit(Parent.GGabar.dY());
	AuxAuxDirs[2].SetUpLimit(Parent.GGabar.dX());
}
void DXSubModel5X::MakeEmpty()
{
	ClearMemory();
	Directions[0].MakeEmpty();
	Directions[1].MakeEmpty();
	Directions[2].MakeEmpty();
	DirectionsA[0].MakeEmpty();
	DirectionsA[1].MakeEmpty();
	DirectionsA[2].MakeEmpty();
	AuxDirs[0].RemoveWindow();
	AuxDirs[1].RemoveWindow();
	AuxDirs[2].RemoveWindow();
	AuxAuxDirs[0].Clear();
	AuxAuxDirs[1].Clear();
	AuxAuxDirs[2].Clear();
	AuxAuxDirs[0].SetUpLimit(Parent.GGabar.dZ());
	AuxAuxDirs[1].SetUpLimit(Parent.GGabar.dY());
	AuxAuxDirs[2].SetUpLimit(Parent.GGabar.dX());
	Directions[0].SetUpLimit(Parent.GGabar.dZ());
	Directions[1].SetUpLimit(Parent.GGabar.dY());
	Directions[2].SetUpLimit(Parent.GGabar.dX());
}

//пересоздать
void DXSubModel5X::ReCreate()
{
	DXSubModel::ReCreate();
	if(nX != 0 && nY != 0) //создается дексель модель в направлении Z
	{
		Directions[0].Init(this, DXDirection::Z, nX + 1, nY + 1, Parent.sX, Parent.sY, Parent.nBig[0], Parent.nBig[1], Parent.GGabar.Xmin, Parent.GGabar.Ymin, Parent.GGabar.Zmin);
		DirectionsA[0].Init(this, DXDirection::Z, nX + 1, nY + 1, Parent.sX, Parent.sY, Parent.nBig[0], Parent.nBig[1], Parent.GGabar.Xmin, Parent.GGabar.Ymin, Parent.GGabar.Zmin);
		AuxDirs[0].Init(this, DXDirection::Z, nX + 1, nY + 1, Parent.sX, Parent.sY, Parent.nBig[0], Parent.nBig[1], Parent.GGabar.Xmin, Parent.GGabar.Ymin, Parent.GGabar.Zmin);
		AuxDirs[0].Clear();
		AuxAuxDirs[0].Init(this, DXDirection::Z, nX + 1, nY + 1, Parent.sX, Parent.sY, Parent.nBig[0], Parent.nBig[1], Parent.GGabar.Xmin, Parent.GGabar.Ymin, Parent.GGabar.Zmin);
		AuxAuxDirs[0].Clear();
	}
	if(nX != 0 && nZ != 0) //создается дексель модель в направлении Y
	{
		Directions[1].Init(this, DXDirection::Y, nZ + 1, nX + 1, Parent.sZ, Parent.sX, Parent.nBig[2], Parent.nBig[0], Parent.GGabar.Zmin, Parent.GGabar.Xmin, Parent.GGabar.Ymin);
		DirectionsA[1].Init(this, DXDirection::Y, nZ + 1, nX + 1, Parent.sZ, Parent.sX, Parent.nBig[2], Parent.nBig[0], Parent.GGabar.Zmin, Parent.GGabar.Xmin, Parent.GGabar.Ymin);
		AuxDirs[1].Init(this, DXDirection::Y, nZ + 1, nX + 1, Parent.sZ, Parent.sX, Parent.nBig[2], Parent.nBig[0], Parent.GGabar.Zmin, Parent.GGabar.Xmin, Parent.GGabar.Ymin);
		AuxDirs[1].Clear();
		AuxAuxDirs[1].Init(this, DXDirection::Y, nZ + 1, nX + 1, Parent.sZ, Parent.sX, Parent.nBig[2], Parent.nBig[0], Parent.GGabar.Zmin, Parent.GGabar.Xmin, Parent.GGabar.Ymin);
		AuxAuxDirs[1].Clear();
	}
	if(nZ != 0 && nY != 0) //создается дексель модель в направлении X
	{
		Directions[2].Init(this, DXDirection::X, nY + 1, nZ + 1, Parent.sY, Parent.sZ, Parent.nBig[1], Parent.nBig[2], Parent.GGabar.Ymin, Parent.GGabar.Zmin, Parent.GGabar.Xmin);
		DirectionsA[2].Init(this, DXDirection::X, nY + 1, nZ + 1, Parent.sY, Parent.sZ, Parent.nBig[1], Parent.nBig[2], Parent.GGabar.Ymin, Parent.GGabar.Zmin, Parent.GGabar.Xmin);
		AuxDirs[2].Init(this, DXDirection::X, nY + 1, nZ + 1, Parent.sY, Parent.sZ, Parent.nBig[1], Parent.nBig[2], Parent.GGabar.Ymin, Parent.GGabar.Zmin, Parent.GGabar.Xmin);
		AuxDirs[2].Clear();
		AuxAuxDirs[2].Init(this, DXDirection::X, nY + 1, nZ + 1, Parent.sY, Parent.sZ, Parent.nBig[1], Parent.nBig[2], Parent.GGabar.Ymin, Parent.GGabar.Zmin, Parent.GGabar.Xmin);
		AuxAuxDirs[2].Clear();
	}
//	Reset();//обнуление всех значений созданных дексель моделей
}

void DXSubModel5X::MarkUsedFacesOrigCh(class BDynBitArray & , class BDynBitArray & UsedO)
{
	MarkUsedOrigCh(UsedO);
}

void DXSubModel5X::SetNewIndexes(int* newind)
{
}

void DXSubModel5X::MarkUsedPoints(BDynBitArray& Used) const
{
}

void DXSubModel5X::MarkUsedOrigCh(class BDynBitArray & Used)
{
	for(int ld = 0; ld < 3; ++ld)
	{
		Directions[ld].MarkUsedOrig(&GetMemory(ld), Used);//!PARALLEL (Used should be thread safe)
	}
}

//создание
void DXSubModel5X::CreateCh(const BBox &Bounds, bool Convex, bool ToAux)
{
	Bounds;
	Convex;
	ToAux;
}

void DXSubModel5X::CopyMergeCh(MBSPMerge & mbsp, MBody & body)
{
	mbsp;
	body;
}

void DXSubModel5X::SafeMergeCh(const MBSPMerge & Op)
{
	Op;
}
////очистить
//void DXSubModel5X::ClearCh(void)
//{
//	Reset();
//}
//инвертировать
MBody * DXSubModel5X::InvertCh(bool ToAux)
{
	ToAux;
	//Directions[0].Invert();
	//Directions[1].Invert();
	//Directions[2].Invert();
	return NULL;
}

int DXSubModel5X::Subtr1Dir(DXDirection::DX_Dir DirSym)
{
	int k = 0;
	switch(DirSym)
	{
	case DXDirection::Z:
		k = 0;
		break;
	case DXDirection::Y:
		k = 1;
		break;
	case DXDirection::X:
		k = 2;
		break;
	}
	return Subtr1Dir(k);
}

void DXSubModel5X::SubtrAllDir()
{
	this->Subtr1Dir(0);
	this->Subtr1Dir(1);
	this->Subtr1Dir(2);
}

int DXSubModel5X::Subtr1Dir(int k)
{
//Check();
	DXMemoryPtOnePart &Mem = GetMemory(k);// PARALLEL
	Mem.Lock();
	int Res = Directions[k].Subtract(&Mem, AuxDirs[k]);
	Mem.Unlock();
	return Res;
}

int DXSubModel5X::SubtrDirectionsA()
{
	//using namespace concurrency;
	//parallel_for(0, 3, [&](int k) 
	for (int k = 0; k < 3; ++k)
	{
		DXMemoryPtOnePart &Mem = GetMemory(k);// PARALLEL
		Mem.Lock();
		Directions[k].Subtract(&Mem, DirectionsA[k]);
		Mem.Unlock();
	};
//Check();
	return 0;
}

int DXSubModel5X::StoreFace(MFace * pFace, ElemID ElID, cadrID CaID, bool ToAux)
{
	if(!pFace)
		return 0;
	RecalcID(*pFace, ElID);

	const int n = pFace->GetCount();

	BPoint Min(0),Max(0);
	pFace->GetGabar(Min, Max);
	bool Cout[3] = { false, false, false };
	Cout[0] |= Min.GetX() > MaxPoint.GetX();
	Cout[0] |= Max.GetX() <= MinPoint.GetX();
	Cout[1] |= Min.GetY() > MaxPoint.GetY();
	Cout[1] |= Max.GetY() <= MinPoint.GetY();
	Cout[2] |= Min.GetZ() > MaxPoint.GetZ();
	Cout[2] |= Max.GetZ() <= MinPoint.GetZ();
	//	if(Min.GetZ() > MaxPoint.GetZ())		return 0;
	//	if(Max.GetZ() <= MinPoint.GetZ())		return 0;
	//if(Min.GetX() > MaxPoint.GetX())		return 0;
	//if(Max.GetX() < MinPoint.GetX())		return 0;
	//if(Min.GetY() > MaxPoint.GetY())		return 0;
	//if(Max.GetY() < MinPoint.GetY())		return 0;
	DXDirection5X *Dirs = StoreInA ? DirectionsA : Directions;
	//using namespace concurrency;
	//parallel_for(0, 3, [&](int k) // Slower (15%)
	for(int k = 0; k < 3; ++k)
	{
		const bool C0 = Cout[(2 - k + 1) % 3];
		const bool C1 = Cout[(2 - k + 2) % 3];
		if (!(C0 || C1))
		{
			DXMemoryPtOnePart &Mem = GetMemory(k);// PARALLEL
			DXDirection5X & Direction = Dirs[k];
			switch (n)
			{
			case 3:
				Direction.RastTriangle(&Mem, *pFace->GetPoint(0), *pFace->GetPoint(1), *pFace->GetPoint(2),
					pFace->GetPlane().GetNormal(), DX_ID(ElID, CaID));
				break;
				//case 4:
				//	Direction.InsRect(&Mem, *pFace->GetPoint(0), *pFace->GetPoint(1), *pFace->GetPoint(2), *pFace->GetPoint(3), 
				//		pFace->GetPlane().GetNormal(), ElID);
				//	break;
			default:
				{
					for (int i = 2; i < n; ++i)
					{
						Direction.RastTriangle(&Mem, *pFace->GetPoint(0), *pFace->GetPoint(i - 1), *pFace->GetPoint(i),
							pFace->GetPlane().GetNormal(), DX_ID(ElID, CaID));
					}
				}
				break;
			}
		}
	}//);
	return 0;
}
// проверка
bool DXSubModel5X::CheckCh(void) const
{
	return Directions[0].Check(&GetMemoryC(0)) && Directions[1].Check(&GetMemoryC(1)) && Directions[2].Check(&GetMemoryC(2));// PARALLEL
}
bool DXSubModel5X::Heal(void)
{
	return Directions[0].Heal(&Memory.GetDirPart(0)) && Directions[1].Heal(&Memory.GetDirPart(1)) && Directions[2].Heal(&(Memory.GetDirPart(2)));
}
// нормализация
void DXSubModel5X::Normalization()
{
	DXMemID CurPointPrev;
	
	for (int k = 0; k < 3; k++) // для каждого направления
	{
		const DXDirection5X & Direction = GetDirection(k);
		// получим шаг для соответствующего декселя
		double Step = 1.;
		int k0 = 0;
		int k1 = 0;
		int k2 = 0;
		int i0Start = 0;
		int i0End = 0;
		int i1Start = 0;
		int i1End = 0;
		int i2Start = 0;
		int i2End = 0;
		switch (k)
		{
		case 0:
			Step = GetZstep();  
			k0 = 0;
			k1 = 1;
			k2 = 2;
			i0Start = GetWindow().ixStart;
			i0End = GetWindow().ixEnd;
			i1Start = GetWindow().iyStart;
			i1End = GetWindow().iyEnd;
			i2Start = GetWindow().izStart;
			i2End = GetWindow().izEnd;
			break;
		case 1:
			Step = GetYstep(); 
			k0 = 2;
			k1 = 0;
			k2 = 1;
			i0Start = GetWindow().izStart;
			i0End = GetWindow().izEnd;
			i1Start = GetWindow().ixStart;
			i1End = GetWindow().ixEnd;
			i2Start = GetWindow().iyStart;
			i2End = GetWindow().iyEnd;
			break;
		case 2:
			Step = GetXstep(); 
			k0 = 1;
			k1 = 2;
			k2 = 0;
			i0Start = GetWindow().iyStart;
			i0End = GetWindow().iyEnd;
			i1Start = GetWindow().izStart;
			i1End = GetWindow().izEnd;
			i2Start = GetWindow().ixStart;
			i2End = GetWindow().ixEnd;
			break;
		}
		DXMemoryPtOnePart &Mem = GetMemory(k);// PARALLEL
		int max0 = Direction.GetN0();
		int max1 = Direction.GetN1();
		for (int B0 = i0Start; B0 < i0End; ++B0)
		{
			for (int I0 = B0 * GetParent().nBig[k0]; I0 < min(max0, (B0 + 1) * GetParent().nBig[k0]); ++I0)
			{
				for (int B1 = i1Start; B1 < i1End; ++B1)
				{
					for (int I1 = B1 * GetParent().nBig[k1]; I1 < min(max1, (B1 + 1) * GetParent().nBig[k1]); ++I1)
					{
						DXMemID CurPointStart = Direction.GetDexelStartPoint(I0, I1); // получаем первую точку декселя
						DXMemID CurPointFirst = CurPointStart; // получим первую точку первого сегмента

						while (CurPointFirst != DXP_END)
						{
							DXMemID CurPointSecond = Mem.GetAtNext(CurPointFirst); // получаем вторую точку сегмента
							if (CurPointSecond == DXP_END)
								break;
							DX_DEPTH Z0 = Mem.GetAtZ(CurPointFirst); // получим координату первой точки сегмента
							DX_DEPTH Z1 = Mem.GetAtZ(CurPointSecond); // получаем координату второй точки сегмента

							int C0 = (int)(Z0 / Step); // получим индекс ячейки в направлении декселя, в которую попадает первая точка сегмента
							int C1 = (int)(Z1 / Step); // получим индекс ячейки в направлении декселя, в которую попадает вторая точка сегмента

							int B20 = C0 / GetParent().nBig[k2];

							if (B20 >= i2End)
								break;

							if (C0 == C1) // если индексы ячеек для точек одного сегмента совпадают
							{
								if (CurPointFirst == CurPointStart)
								{
									CurPointStart = Mem.GetAtNext(CurPointSecond);// получаем первую точку декселя
									Directions[k].SetDexelStartPoint(I0, I1, CurPointStart);
									CurPointFirst = CurPointStart; // определяем первую точку следующего сегмента
								}
								else
								{
									Mem.SetAtNext(CurPointPrev, Mem.GetAtNext(CurPointSecond));
									CurPointFirst = Mem.GetAtNext(CurPointPrev); //удаляем сегмент из модели и получаем первую точку следующего сегмента
								}
							}
							else
							{// Can be faster
								bool C0Outside = (C0 < i2Start);
								bool C1Outside = (C1 >= i2End);
								if (!C0Outside)
									if (BusyUnBusy(I0, I1, C0, k)) // если первый конец ребра, на которой лежит первая точка сегмента непустой 
										Mem.SetAtZ(CurPointFirst, (DX_DEPTH)(C0*Step - SMALLVAL)); // переместим точку в предыдущее по индексу ребро

								if (!C1Outside)
									if (BusyUnBusy(I0, I1, C1 + 1, k)) // если второй конец ребра, на которой лежит вторая точка сегмента непустой 
										Mem.SetAtZ(CurPointSecond, (DX_DEPTH)((C1 + 1)*Step + SMALLVAL)); // переместим точку в следующее по индексу ребро

								CurPointFirst = Mem.GetAtNext(CurPointSecond); // получаем первую точку следующего сегмента
								CurPointPrev = CurPointSecond; // получим точку перед сегментом
							}
						}

						CurPointFirst = CurPointStart;// Direction.GetDexelStartPoint(I0, I1); // получаем первую точку декселя

						while (CurPointFirst != DXP_END)
						{
							DXMemID CurPointSecond = Mem.GetAtNext(CurPointFirst); // получаем вторую точку сегмента
							if (CurPointSecond == DXP_END)
								break;
							DXMemID CurPointFirstNext = Mem.GetAtNext(CurPointSecond); // получим первую точку следующего сегмента
							if (CurPointFirstNext != DXP_END) // если есть еще сегмент
							{
								DX_DEPTH Z0, Z1;
								Z0 = Mem.GetAtZ(CurPointSecond); // получим координату второй точки первого сегмента сегмента
								Z1 = Mem.GetAtZ(CurPointFirstNext); // получаем координату первой точки второго сегмента

								if (Z0 > Z1)
									Mem.SetAtNext(CurPointFirst, Mem.GetAtNext(CurPointFirstNext));
								else
									CurPointFirst = CurPointFirstNext;
							}
							else
								CurPointFirst = Mem.GetAtNext(CurPointSecond);
						}
					}
				}
			}
		}
	}
}

// проверка на пустоту, проходит ли через вершину сегмент декселя одного направления
bool DXSubModel5X::CheckEmpty(int i0, int i1, DX_DEPTH Val) //true - если через вершину проходит сегмент
{
	for (DXMemID ID = Directions[0].GetDexelStartPoint(i0, i1); ID != DXP_END; )
	{
		if (Memory.GetAtZ(ID) > Val)
			break;
		ID = Memory.GetAtNext(ID);
//#ifdef _DEBUG
		if(ID == DXP_END)
			return false;
//#endif
		if (Memory.GetAtZ(ID) >= Val)
			return true;
		ID = Memory.GetAtNext(ID);
	}
	return false;

}


// проверка на занятость, т.е. проходит ли через вершину дексель любого из направлений
bool DXSubModel5X::BusyUnBusy(int I0, int I1, int I2, int NumDir)
{
	int Ind[3];
	switch(NumDir)
	{
	case 0:
	default:
		Ind[0] = I0;
		Ind[1] = I1;
		Ind[2] = I2;
		break;
	case 1:
		Ind[0] = I1;
		Ind[1] = I2;
		Ind[2] = I0;
		break;
	case 2:
		Ind[0] = I2;
		Ind[1] = I0;
		Ind[2] = I1;
		break;
	}
	if (CheckEmpty(Ind[0], Ind[1], DX_DEPTH(Ind[2] * GetZstep())))
		return true;
	return false;
}

int DXSubModel5X::InsFullBCadr(const BPoint & P0v, const BPoint & P1v, const BPoint & Nv, double R, ElemID ElID, const BMatr &Shift, class CadrData &Data, const CadrData& MaskData)
{
	Data.DXType = TT_NO;
	if(R < MIND)
		return 0;
	// Insert sphere cadr
	// P0, P1 - tool positions
	// R - sphere radius
	
	// Move given points
	Data.N = Nv * Shift;
	Data.P0 = P0v * Shift;
	Data.P1 = P1v * Shift;
	Data.R = R;

	Data.Bounds.Init();
	Data.Bounds.Expand(Data.P0 - BPoint(R, R, R, 1.));
	Data.Bounds.Expand(Data.P0 + BPoint(R, R, R, 1.));
	Data.Bounds.Expand(Data.P1 - BPoint(R, R, R, 1.));
	Data.Bounds.Expand(Data.P1 + BPoint(R, R, R, 1.));

	Data.DXType = TT_5X_FULL_BALL;
	// Spheres
	Data.SphID1 = ElID;
	SOSphere *pSph1 = new SOSphere(Data.R, Data.P0);
	MElemIDProc::SetSurfID(&Data.SphID1, MFOREST.AddOrigSurfInside(pSph1));
	Data.SphID2 = ElID;
	SOSphere *pSph2 = new SOSphere(Data.R, Data.P1);
	MElemIDProc::SetSurfID(&Data.SphID2, MFOREST.AddOrigSurfInside(pSph2));
	// Cylinder
	BPoint Ve = (Data.P1 - Data.P0).Unit();
	Data.CylIDb = ElID;
	SOCyl *pCylb = new SOCyl(Data.P0, Ve, Data.R);
	MElemIDProc::SetSurfID(&Data.CylIDb, MFOREST.AddOrigSurfInside(pCylb));
	return 0;
}

int DXSubModel5X::InsBCadr(const BPoint & P0v, const BPoint & P1v, const BPoint & Nv, double R, double H, ElemID ElID, const BMatr &Shift, CadrData &Data, const CadrData& MaskData)
{
	if(R < MIND)
		return 0;
	// Insert sphere cadr
	// P0, P1 - tool positions 
	// Nv - cylinder axis direction
	// R - cylinder radius
	// H - whole tool height (cylinder + sphere)
	Data.DXType = TT_NO;
	// Move given points
	Data.N = Nv * Shift;
	Data.P0 = P0v * Shift + Data.N * R;
	Data.P1 = P1v * Shift + Data.N * R;
	Data.P0Up = Data.P0 + Data.N * (H - R);
	Data.P1Up = Data.P1 + Data.N * (H - R);
	Data.R = R;

	Data.Bounds.Init();
	Data.Bounds.Expand(Data.P0 - BPoint(R, R, R, 1.));
	Data.Bounds.Expand(Data.P0 + BPoint(R, R, R, 1.));
	Data.Bounds.Expand(Data.P1 - BPoint(R, R, R, 1.));
	Data.Bounds.Expand(Data.P1 + BPoint(R, R, R, 1.));
	Data.Bounds.Expand(Data.P0Up - BPoint(R, R, R, 1.));
	Data.Bounds.Expand(Data.P0Up + BPoint(R, R, R, 1.));
	Data.Bounds.Expand(Data.P1Up - BPoint(R, R, R, 1.));
	Data.Bounds.Expand(Data.P1Up + BPoint(R, R, R, 1.));

	BPoint V(Data.P1 - Data.P0);
	double VD2 = V.D2();
	if(VD2 < MIND * MIND)
		return 0;
	Data.DXType = TT_3X_CYL_BALL;
	Data.IsPlane = false;
	BPoint Ve = V * (1. / sqrt(VD2));
	//добавляем в элементы верхний наклонный цилиндр
	SOrigSurf *pTCyl;
	double VN = V * Data.N;
	if(fabs(VN) < MIND * MIND)//проверяем на вырожденность
	{
		pTCyl = new SOPlane(Data.N, Data.P0Up);
		Data.IsPlane = true;
	}
	else
	{
		pTCyl = new SOECyl(Data.P1Up, Ve * (-1.), Data.N * (-1.), Data.R);
		Data.IsPlane = false;
	}
	Data.CylIDt = ElID;
	MElemIDProc::SetSurfID(&Data.CylIDt, MFOREST.AddOrigSurfInside(pTCyl));

	// Planes
	Data.Plane1ID = ElID;
	Data.N1 = Ve % Data.N;
	double N1D2 = Data.N1.D2();	
	Data.CylRight = (N1D2 < MIND * MIND);
	if(!Data.CylRight)
		Data.N1 *= 1. / sqrt(N1D2);
	SOPlane *pPlane1 = new SOPlane(Data.N1, Data.P0 + Data.N1 * R);
	MElemIDProc::SetSurfID(&Data.Plane1ID, MFOREST.AddOrigSurfInside(pPlane1));
	Data.Plane2ID = ElID;
	BPoint N2 = Data.N1 * (-1.);
	SOPlane *pPlane2 = new SOPlane(N2, Data.P0 + N2 * R);
	MElemIDProc::SetSurfID(&Data.Plane2ID, MFOREST.AddOrigSurfInside(pPlane2));
	// Start and end cylinders
	Data.CylID1 = ElID;
	SOCyl *pCyl1 = new SOCyl(Data.P0, Data.N, Data.R);
	MElemIDProc::SetSurfID(&Data.CylID1, MFOREST.AddOrigSurfInside(pCyl1));
	Data.CylID2 = ElID;
	SOCyl *pCyl2 = new SOCyl(Data.P1, Data.N, Data.R);
	MElemIDProc::SetSurfID(&Data.CylID2, MFOREST.AddOrigSurfInside(pCyl2));
	// Bottom cylinder
	Data.CylIDb = ElID;
	SOCyl *pCylb = new SOCyl(Data.P0, Ve, Data.R);
	MElemIDProc::SetSurfID(&Data.CylIDb, MFOREST.AddOrigSurfInside(pCylb));
	// Spheres
	Data.SphID1 = ElID;
	SOSphere *pSph1 = new SOSphere(Data.R, Data.P0);
	MElemIDProc::SetSurfID(&Data.SphID1, MFOREST.AddOrigSurfInside(pSph1));
	Data.SphID2 = ElID;
	SOSphere *pSph2 = new SOSphere(Data.R, Data.P1);
	MElemIDProc::SetSurfID(&Data.SphID2, MFOREST.AddOrigSurfInside(pSph2));
	// Top Flat disk
	SOrigSurf *pFDisk = new SOPlane(Data.N, Data.P0Up);
	Data.DiskIDt = ElID;
	MElemIDProc::SetSurfID(&Data.DiskIDt, MFOREST.AddOrigSurfInside(pFDisk));

	return 0;
}

int DXSubModel5X::InsCCadr(const BPoint & P0, const BPoint & P1, const BPoint & Nv, double R, double H, ElemID ElID, const BMatr &Shift, CadrData &Data, const CadrData& MaskData)
{
	if(R < MIND)
		return 0;
	
	// Insert cylinder cadr
	// P0, P1 - tool positions
	// Nv - cylinder axis direction
	// R - cylinder radius
	// H - cylinder height
	Data.DXType = TT_NO;
	// Move given points
	Data.P0 = P0 * Shift;
	Data.P1 = P1 * Shift;
	Data.N = Nv * Shift;
	Data.P0Up = Data.P0 + Data.N * H;
	Data.P1Up = Data.P1 + Data.N * H;
	Data.R = R;

	Data.Bounds.Init();
	Data.Bounds.Expand(Data.P0 - BPoint(R, R, R, 1.));
	Data.Bounds.Expand(Data.P0 + BPoint(R, R, R, 1.));
	Data.Bounds.Expand(Data.P1 - BPoint(R, R, R, 1.));
	Data.Bounds.Expand(Data.P1 + BPoint(R, R, R, 1.));
	Data.Bounds.Expand(Data.P0Up - BPoint(R, R, R, 1.));
	Data.Bounds.Expand(Data.P0Up + BPoint(R, R, R, 1.));
	Data.Bounds.Expand(Data.P1Up - BPoint(R, R, R, 1.));
	Data.Bounds.Expand(Data.P1Up + BPoint(R, R, R, 1.));

	BPoint V = Data.P1 - Data.P0;
	double VD = V.D2();
	if(VD < MIND * MIND)
		return 0;
	VD = 1. / sqrt(VD);
	Data.DXType = TT_3X_CYL;

	//добавляем в элементы наклонные цилиндры
	SOrigSurf *pBCyl, *pTCyl;
	double VN = V * Data.N;
	if(fabs(VN) < MIND * MIND)//проверяем их на вырожденность
	{
		pBCyl = new SOPlane(Data.N * (-1.), Data.P0);
		pTCyl = new SOPlane(Data.N, Data.P0Up);
		Data.IsPlane = true;
	}
	else
	{
		pBCyl = new SOECyl(Data.P0, V * VD, Data.N, Data.R);
		pTCyl = new SOECyl(Data.P1Up, V * (-VD), Data.N * (-1.), Data.R);
		Data.IsPlane = false;
	}
	SOrigSurf *pFDisk = new SOPlane(Data.N, Data.P0Up);
	Data.DiskIDt = ElID;
	MElemIDProc::SetSurfID(&Data.DiskIDt, MFOREST.AddOrigSurfInside(pFDisk));
	pFDisk = new SOPlane(Data.N * (-1.), Data.P1);
	Data.SphID2 = ElID;
	MElemIDProc::SetSurfID(&Data.SphID2, MFOREST.AddOrigSurfInside(pFDisk));

	Data.CylIDb = ElID;
	MElemIDProc::SetSurfID(&Data.CylIDb, MFOREST.AddOrigSurfInside(pBCyl));
	Data.CylIDt = ElID;
	MElemIDProc::SetSurfID(&Data.CylIDt, MFOREST.AddOrigSurfInside(pTCyl));

	Data.Plane1ID = ElID;
	Data.N1 = V % Data.N;
	double N1D2 = Data.N1.D2();	
	Data.CylRight = (N1D2 < MIND * MIND);
	if(!Data.CylRight)
		Data.N1 *= 1. / sqrt(N1D2);
	SOPlane *pPlane1 = new SOPlane(Data.N1, Data.P0 + Data.N1 * R);
	MElemIDProc::SetSurfID(&Data.Plane1ID, MFOREST.AddOrigSurfInside(pPlane1));
	Data.Plane2ID = ElID;
	BPoint N2 = Data.N1 * (-1.);
	SOPlane *pPlane2 = new SOPlane(N2, Data.P0 + N2 * R);
	MElemIDProc::SetSurfID(&Data.Plane2ID, MFOREST.AddOrigSurfInside(pPlane2));
	Data.CylID1 = ElID;
	SOCyl *pCyl1 = new SOCyl(Data.P0, Data.N, Data.R);
	MElemIDProc::SetSurfID(&Data.CylID1, MFOREST.AddOrigSurfInside(pCyl1));
	Data.CylID2 = ElID;
	SOCyl *pCyl2 = new SOCyl(Data.P1, Data.N, Data.R);
	MElemIDProc::SetSurfID(&Data.CylID2, MFOREST.AddOrigSurfInside(pCyl2));

	return 0;
}

int DXSubModel5X::Merge1Dir(int k)
{
	//AuxAuxDirs[k].Check();
	if (NCMProject::GetProject().GetGlobalState().GetMultiThreadDX())
	{
		int i0, i1;
		if(!AuxDirs[k].Merge(AuxAuxDirs[k], i0, i1))
		{
			Subtr1Dir(k);
			AuxDirs[k].Move(AuxAuxDirs[k]);
		}
	}
	else
	{
		AuxDirs[k].Move(AuxAuxDirs[k]);
		Subtr1Dir(k);
	}
	return 0;
}

bool DXSubModel5X::IsChanged(void) const
{
	return Directions[0].IsChanged() || Directions[1].IsChanged() || Directions[2].IsChanged();
}

void DXSubModel5X::ClearChanged(void)
{
	Directions[0].ClearChanged();
	Directions[1].ClearChanged();
	Directions[2].ClearChanged();
}

void DXSubModel5X::RastTriangle(const BPoint & P0, const BPoint & P1, const BPoint & P2, const BPoint & N, DX_ID PairID)
{
	Directions[0].RastTriangle(&GetMemory(0), P0, P1, P2, N, PairID);// PARALLEL
	Directions[1].RastTriangle(&GetMemory(1), P0, P1, P2, N, PairID);
	Directions[2].RastTriangle(&GetMemory(2), P0, P1, P2, N, PairID);
}

void DXSubModel5X::MakeCyl(double D, double H, const BMatr &Tr)
{
	Directions[0].ClearChanged();
	Directions[1].ClearChanged();
	Directions[2].ClearChanged();
	// Make points
	double R = D * 0.5;
	BPoint P0(0., 0., 0., 1.);
	BPoint P1(0., 0., H, 1.);
	BPoint N(0., 0., 1., 0.);
	BPoint NRightCyl(R, 0., 0., 0.);
	BPoint V(0., R, 0., 0.);
	P0 = P0 * Tr;
	P1 = P1 * Tr;
	N = N * Tr;
	NRightCyl = NRightCyl * Tr;

	BBox Bounds;
	Bounds.Expand(P0 - NRightCyl - V);
	Bounds.Expand(P0 + NRightCyl + V);
	Bounds.Expand(P1 - NRightCyl - V);
	Bounds.Expand(P1 + NRightCyl + V);

	ElemID ElIDCyl = MElemIDProc::ID_NULL;
	SOCyl *pCyl = new SOCyl(P0, N, -R);
	MElemIDProc::SetSurfID(&ElIDCyl, MFOREST.AddOrigSurfInside(pCyl));
	SOrigSurf *pFDisk = new SOPlane(N, P0);
	ElemID FlatID0 = MElemIDProc::ID_NULL;
	MElemIDProc::SetSurfID(&FlatID0, MFOREST.AddOrigSurfInside(pFDisk));
	pFDisk = new SOPlane(N * (-1.), P1);
	ElemID FlatID1 = MElemIDProc::ID_NULL;
	MElemIDProc::SetSurfID(&FlatID1, MFOREST.AddOrigSurfInside(pFDisk));


	AuxDirs[0].Clear();
	AuxDirs[0].RastHCylinder(P0, P1, NRightCyl, R, DX_ID(ElIDCyl, cadrID(-1)), true, DX_ID(FlatID1, cadrID(-1)));
	AuxDirs[0].RastHCylinder(P1, P0, NRightCyl * (-1.), R, DX_ID(ElIDCyl, cadrID(-1)), true, DX_ID(FlatID0, cadrID(-1)));
	Directions[0].Store(&GetMemory(0), AuxDirs[0]);// PARALLEL
	AuxDirs[0].Clear();

	AuxDirs[1].Clear();
	AuxDirs[1].RastHCylinder(P0, P1, NRightCyl, R, DX_ID(ElIDCyl, cadrID(-1)), true, DX_ID(FlatID1, cadrID(-1)));
	AuxDirs[1].RastHCylinder(P1, P0, NRightCyl * (-1.), R, DX_ID(ElIDCyl, cadrID(-1)), true, DX_ID(FlatID0, cadrID(-1)));
	Directions[1].Store(&GetMemory(1), AuxDirs[1]);
	AuxDirs[1].Clear();

	AuxDirs[2].Clear();
	AuxDirs[2].RastHCylinder(P0, P1, NRightCyl, R, DX_ID(ElIDCyl, cadrID(-1)), true, DX_ID(FlatID1, cadrID(-1)));
	AuxDirs[2].RastHCylinder(P1, P0, NRightCyl * (-1.), R, DX_ID(ElIDCyl, cadrID(-1)), true, DX_ID(FlatID0, cadrID(-1)));
	Directions[2].Store(&GetMemory(2), AuxDirs[2]);
	AuxDirs[2].Clear();
}

void DXSubModel5X::MakeFromBSP(MQuadTree& QTree, DXDirection5X* Dirs /*= nullptr*/)
{
	if (Dirs == NULL)
		Dirs = Directions;
	int NodeCount = 0;
	MBSPNode** MyTree;
	QTree.MakeTree(NodeCount, &MyTree);

	using namespace concurrency;
	parallel_invoke(
		[&] {Dirs[0].MakeFromBSP(&GetMemory(0), MyTree[0], QTree);  },// PARALLEL
		[&] {Dirs[1].MakeFromBSP(&GetMemory(1), MyTree[0], QTree);  },
		[&] {Dirs[2].MakeFromBSP(&GetMemory(2), MyTree[0], QTree);  }
	);
	//Dirs[0].MakeFromBSP(&GetMemory(0), MyTree[0], QTree);
	//Dirs[1].MakeFromBSP(&GetMemory(1), MyTree[0], QTree);
	//Dirs[2].MakeFromBSP(&GetMemory(2), MyTree[0], QTree);
	for (int i = 0; i <= NodeCount; ++i)
		delete MyTree[i];
	delete[] MyTree;
}

void DXSubModel5X::MakeFromQRendMill(const GQuadRenderMill* pRendMill)
{
	DXDirection5X* Dirs = Directions;
	using namespace concurrency;
	parallel_invoke(
		[&] {Dirs[0].MakeFromQRendMill(&GetMemory(0), pRendMill);  },
		[&] {Dirs[1].MakeFromQRendMill(&GetMemory(1), pRendMill);  },
		[&] {Dirs[2].MakeFromQRendMill(&GetMemory(2), pRendMill);  }
	);
}

int DXSubModel5X::RastFullBCadr(int Dir, const CadrData &Data)
{
	if (Data.IsLast())
	{
		return 0;
	}

	const DX_ID TempID0 = DX_ID(0, cadrID(-1));
	AuxDirs[Dir].ClearWindow();
	AuxDirs[Dir].ExpandWindow(Data.Bounds);

	BPoint V = Data.P1 - Data.P0;
	BPoint NRightCyl(Data.N);//произвольный вектор, перпендикулярный оси цилиндра
	NRightCyl.Unit();
	if(fabs(V.GetX()) < fabs(V.GetY()))
		if(fabs(V.GetX()) < fabs(V.GetZ()))
			NRightCyl.SetX(NRightCyl.GetX() + 1.);
		else
			NRightCyl.SetZ(NRightCyl.GetZ() + 1.);
	else
		if(fabs(V.GetY()) < fabs(V.GetZ()))
			NRightCyl.SetY(NRightCyl.GetY() + 1.);
		else
			NRightCyl.SetZ(NRightCyl.GetZ() + 1.);
	NRightCyl = (NRightCyl % V).Unit();


	//Insert cylinder
	AuxDirs[Dir].RastHCylinder(Data.P0, Data.P1, NRightCyl, Data.R, DX_ID(Data.CylIDb, Data.HistID), false, TempID0);
	AuxDirs[Dir].RastHCylinder(Data.P0, Data.P1, NRightCyl * (-1.), Data.R, DX_ID(Data.CylIDb, Data.HistID), false, TempID0);
	//Insert end sphere
	AuxDirs[Dir].RastSphSegment(Data.P1, 0, NULL, NULL, Data.R, DX_ID(Data.SphID2, Data.HistID));

	AuxDirs[Dir].RastSphSegment(Data.P0, 0, NULL, NULL, Data.R, DX_ID(Data.SphID1, Data.HistID));

	Subtr1Dir(Dir);
	return 0;
}

int DXSubModel5X::RastBCadr(int Dir, const CadrData &Data)
{
	if (Data.IsLast())
	{
		Subtr1Dir(Dir);
		return 0;
	}

	const DX_ID TempID0 = DX_ID(0, cadrID(-1));
	DXDirection3X &ADir = AuxAuxDirs[Dir];
	ADir.SetWindow(Data.Bounds);
	if(Data.CylRight)
	{
		BPoint NRightCyl(Data.N);//произвольный вектор, перпендикулярный оси цилиндра
		NRightCyl.Unit();
		if(fabs(Data.N.GetX()) < fabs(Data.N.GetY()))
			if(fabs(Data.N.GetX()) < fabs(Data.N.GetZ()))
				NRightCyl.SetX(NRightCyl.GetX() + 1.);
			else
				NRightCyl.SetZ(NRightCyl.GetZ() + 1.);
		else
			if(fabs(Data.N.GetY()) < fabs(Data.N.GetZ()))
				NRightCyl.SetY(NRightCyl.GetY() + 1.);
			else
				NRightCyl.SetZ(NRightCyl.GetZ() + 1.);
		NRightCyl = (NRightCyl % Data.N).Unit();
		BPoint vA = NRightCyl % Data.N;

		ADir.RastHCylinder(Data.P0Up, Data.P1, vA, Data.R, DX_ID(Data.CylID1, Data.HistID), false, TempID0);
		ADir.RastHCylinder(Data.P1, Data.P0Up, vA * (-1.), Data.R, DX_ID(Data.SphID2, Data.HistID), true, DX_ID(Data.DiskIDt, Data.HistID));
		BPoint Normals[1];
		BPoint Points[1];
		Normals[0] = Data.N * (-1.);
		Points[0] = Data.P1;
		ADir.RastSphSegment(Data.P1, 1, Points, Normals, Data.R, DX_ID(Data.SphID2, Data.HistID));
	}
	else
	{// Общий случай
		//Insert spheres
		BPoint Normals[2];
		BPoint Points[2];
		Normals[0] = Data.P0 - Data.P1;
		Normals[1] = Data.N * (-1.);
		Points[0] = Data.P0;
		Points[1] = Data.P0;
		ADir.RastSphSegment(Data.P0, 2, Points, Normals, Data.R, DX_ID(Data.SphID1, Data.HistID));
		Normals[0] *= -1.;
		Points[0] = Data.P1;
		Points[1] = Data.P1;
		ADir.RastSphSegment(Data.P1, 2, Points, Normals, Data.R, DX_ID(Data.SphID2, Data.HistID));
		//Insert bottom cylinder
		ADir.RastHCylinder(Data.P0, Data.P1, Data.N * (-1.), Data.R, DX_ID(Data.CylIDb, Data.HistID), false, TempID0);
		//Insert top inclined cylinder part
		ADir.RastICylinder(Data.P1Up, Data.P0Up, Data.N * (-1.), Data.R, Data.IsPlane, DX_ID(Data.CylIDt, Data.HistID), true, DX_ID(Data.DiskIDt, Data.HistID));
		// Insert rectangles
		bool FindMax = (Data.N1.GetArray()[2 - Dir] < 0.) ? true : false;

		BPoint NN1 = Data.N1 * Data.R;
		ADir.InsRect(Data.P0 + NN1, Data.P1 + NN1, Data.P1Up + NN1, Data.P0Up + NN1, Data.N1, !FindMax, (!FindMax) ? 1 : 0, DX_ID(Data.Plane1ID, Data.HistID));
		BPoint NN2 = NN1 * (-1.);
		ADir.InsRect(Data.P0 + NN2, Data.P1 + NN2, Data.P1Up + NN2, Data.P0Up + NN2, Data.N1, FindMax, (FindMax) ? 1 : 0, DX_ID(Data.Plane2ID, Data.HistID));
		// Insert cylinder part
		ADir.RastHCylinder(Data.P0Up, Data.P0, Data.P0 - Data.P1, Data.R, DX_ID(Data.CylID1, Data.HistID), false, TempID0);
		ADir.RastHCylinder(Data.P1Up, Data.P1, Data.P1 - Data.P0, Data.R, DX_ID(Data.CylID2, Data.HistID), false, TempID0);
	}
	Merge1Dir(Dir);
	return 0;
}
int DXSubModel5X::RastCCadr(int Dir, const CadrData &Data)
{
	if (Data.IsLast())
	{
		Subtr1Dir(Dir);
		return 0;
	}
	
	const DX_ID TempID0 = DX_ID(0, cadrID(-1));
	DXDirection3X &ADir = AuxAuxDirs[Dir];

	ADir.SetWindow(Data.Bounds);
	if(Data.CylRight)
	{//Цилиндр прямой - инструмент движется вдоль своей оси
		BPoint NRightCyl(Data.N);//произвольный вектор, перпендикулярный оси цилиндра
		NRightCyl.Unit();
		if(fabs(Data.N.GetX()) < fabs(Data.N.GetY()))
			if(fabs(Data.N.GetX()) < fabs(Data.N.GetZ()))
				NRightCyl.SetX(NRightCyl.GetX()+1.);
			else
				NRightCyl.SetZ(NRightCyl.GetZ()+1.);
		else
			if(fabs(Data.N.GetY()) < fabs(Data.N.GetZ()))
				NRightCyl.SetY(NRightCyl.GetY()+1.);
			else
				NRightCyl.SetZ(NRightCyl.GetZ()+1.);
		NRightCyl = (NRightCyl % Data.N).Unit();

		BPoint vA = NRightCyl % Data.N;
		ADir.RastHCylinder(Data.P0Up, Data.P1, vA, Data.R, DX_ID(Data.CylID1, Data.HistID), true, DX_ID(Data.SphID2, Data.HistID));
		ADir.RastHCylinder(Data.P1, Data.P0Up, vA * (-1.), Data.R, DX_ID(Data.CylID1, Data.HistID), true, DX_ID(Data.DiskIDt, Data.HistID));
	}
	else
	{//Цилиндр наклонный - общий случай
		//Insert bottom inclined cylinder part
		ADir.RastICylinder(Data.P0, Data.P1, Data.N, Data.R, Data.IsPlane, DX_ID(Data.CylIDb, Data.HistID), true, DX_ID(Data.SphID2, Data.HistID));
		//Insert top inclined cylinder part
		ADir.RastICylinder(Data.P1Up, Data.P0Up, Data.N * (-1.), Data.R, Data.IsPlane, DX_ID(Data.CylIDt, Data.HistID), true, DX_ID(Data.DiskIDt, Data.HistID));

		// Insert rectangles
		bool FindMax = (Data.N1.GetArray()[2 - Dir] < 0.) ? true : false;

		BPoint NN1 = Data.N1 * Data.R;
		ADir.InsRect(Data.P0 + NN1, Data.P1 + NN1, Data.P1Up + NN1, Data.P0Up + NN1, Data.N1, !FindMax, (!FindMax) ? 1 : 0, DX_ID(Data.Plane1ID, Data.HistID));
		BPoint NN2 = NN1 * (-1.);
		ADir.InsRect(Data.P0 + NN2, Data.P1 + NN2, Data.P1Up + NN2, Data.P0Up + NN2, Data.N1, FindMax, (FindMax) ? 1 : 0, DX_ID(Data.Plane2ID, Data.HistID));
		// Insert cylinder part
		ADir.RastHCylinder(Data.P0Up, Data.P0, Data.P0 - Data.P1, Data.R, DX_ID(Data.CylID1, Data.HistID), false, TempID0);
		ADir.RastHCylinder(Data.P1Up, Data.P1, Data.P1 - Data.P0, Data.R, DX_ID(Data.CylID2, Data.HistID), false, TempID0);
	}
	Merge1Dir(Dir);
	return 0;
}

void DXSubModel5X::FillChangedAreaWin()
{
	if (!pModifiedCells)
		return;

	Win.SetDefined(((MCubeMask *)pModifiedCells)->GetUsedArea(Win.ixStart, Win.ixEnd, Win.iyStart, Win.iyEnd, Win.izStart, Win.izEnd));

	++Win.ixEnd;
	++Win.iyEnd;
	++Win.izEnd;
}

void DXSubModel5X::Intersect()
{
	Directions[0].Intersect(&GetMemory(0), DirectionsA[0]);// PARALLEL
	Directions[1].Intersect(&GetMemory(1), DirectionsA[1]);
	Directions[2].Intersect(&GetMemory(2), DirectionsA[2]);
}

void DXSubModel5X::JoinModifiedCells()
{
	if (pModifiedCells == nullptr || Directions[0].GetpModifiedCellsD() == nullptr 
		|| Directions[1].GetpModifiedCellsD() == nullptr || Directions[2].GetpModifiedCellsD() == nullptr)
		return;
	*(static_cast<MCubeMask *>(pModifiedCells)) |= *(static_cast<MCubeMask *>(Directions[0].GetpModifiedCellsD()));
	*(static_cast<MCubeMask *>(pModifiedCells)) |= *(static_cast<MCubeMask *>(Directions[1].GetpModifiedCellsD()));
	*(static_cast<MCubeMask *>(pModifiedCells)) |= *(static_cast<MCubeMask *>(Directions[2].GetpModifiedCellsD()));
}

void DXSubModel5X::ClearModified()
{
	MQuadBody::ClearModified();
	Directions[0].ClearModifiedD();
	Directions[1].ClearModifiedD();
	Directions[2].ClearModifiedD();
}

void DXSubModel5X::Translate(const BMatr & M)
{
	DXSubModel::Translate(M);
	MBSPForest::NTransfData TransfData;
	GetTransfData(TransfData);
	MFOREST.TransformSelected(TransfData, M);
	Parent.GGabar.Transform(M);
	if (nX != 0 && nY != 0) //создается дексель модель в направлении Z
	{
		Directions[0].SetMin(Parent.GGabar.Xmin, Parent.GGabar.Ymin, Parent.GGabar.Zmin);
		DirectionsA[0].SetMin(Parent.GGabar.Xmin, Parent.GGabar.Ymin, Parent.GGabar.Zmin);
		AuxDirs[0].SetMin(Parent.GGabar.Xmin, Parent.GGabar.Ymin, Parent.GGabar.Zmin);
		AuxAuxDirs[0].SetMin(Parent.GGabar.Xmin, Parent.GGabar.Ymin, Parent.GGabar.Zmin);
	}
	if (nX != 0 && nZ != 0) //создается дексель модель в направлении Y
	{
		Directions[1].SetMin(Parent.GGabar.Zmin, Parent.GGabar.Xmin, Parent.GGabar.Ymin);
		DirectionsA[1].SetMin(Parent.GGabar.Zmin, Parent.GGabar.Xmin, Parent.GGabar.Ymin);
		AuxDirs[1].SetMin(Parent.GGabar.Zmin, Parent.GGabar.Xmin, Parent.GGabar.Ymin);
		AuxAuxDirs[1].SetMin(Parent.GGabar.Zmin, Parent.GGabar.Xmin, Parent.GGabar.Ymin);
	}
	if (nZ != 0 && nY != 0) //создается дексель модель в направлении X
	{
		Directions[2].SetMin(Parent.GGabar.Ymin, Parent.GGabar.Zmin, Parent.GGabar.Xmin);
		DirectionsA[2].SetMin(Parent.GGabar.Ymin, Parent.GGabar.Zmin, Parent.GGabar.Xmin);
		AuxDirs[2].SetMin(Parent.GGabar.Ymin, Parent.GGabar.Zmin, Parent.GGabar.Xmin);
		AuxAuxDirs[2].SetMin(Parent.GGabar.Ymin, Parent.GGabar.Zmin, Parent.GGabar.Xmin);
	}
	AuxAuxDirs[0].SetUpLimit(Parent.GGabar.dZ());
	AuxAuxDirs[1].SetUpLimit(Parent.GGabar.dY());
	AuxAuxDirs[2].SetUpLimit(Parent.GGabar.dX());
	AuxDirs[0].SetUpLimit(Parent.GGabar.dZ());
	AuxDirs[1].SetUpLimit(Parent.GGabar.dY());
	AuxDirs[2].SetUpLimit(Parent.GGabar.dX());
	Directions[0].SetUpLimit(Parent.GGabar.dZ());
	Directions[1].SetUpLimit(Parent.GGabar.dY());
	Directions[2].SetUpLimit(Parent.GGabar.dX());
	DirectionsA[0].SetUpLimit(Parent.GGabar.dZ());
	DirectionsA[1].SetUpLimit(Parent.GGabar.dY());
	DirectionsA[2].SetUpLimit(Parent.GGabar.dX());
}

void DXSubModel5X::GetTransfData(MBSPForest::NTransfData & TransfData) const
{
	for (int Dir = 0; Dir < 3; ++Dir)
	{
		GetMemory(Dir).GetTransfData(TransfData);
	}
}

void DXSubModel5X::Save(MForestSave& SaveData)
{
	DXSubModel::Save(SaveData);
	Memory.Save(SaveData);
	for (int i = 0; i < 3; ++i)
		Directions[i].Save2File(SaveData.File);
}

void DXSubModel5X::Load(CFile* f)
{
	DXSubModel::Load(f);
	Memory.Load(f);
	for (int i = 0; i < 3; ++i)
	{
		Directions[i].SetSubModel(this);
		Directions[i].Load(f);
	}
}

void DXSubModel5X::CreateFullCopy(MBody* pQt)
{
	DXSubModel::CreateFullCopy(pQt);
	DXSubModel5X* pQ = dynamic_cast<DXSubModel5X*>(pQt);
	if (pQ == nullptr)
		return;
	Memory.CreateFullCopy(&pQ->Memory);
	for (int i = 0; i < 3; ++i)
	{
		Directions[i].CreateFullCopy(&pQ->Directions[i]);
		Directions[i].SetSubModel(this);
	}
}

double DXSubModel5X::GetNearestPoint(const BPoint& OrP, MBodyPointInf& Inf) const
{
	MBodyPointInf Infs[3];
	double D2[3];
	for (int i = 0; i < 3; ++i)
		D2[i] = GetDirection(i).GetNearestPoint(&GetMemory(i), OrP, 3, Infs[i]);
	double Dmin = D2[0];
	int imin = 0;
	for (int i = 1; i < 3; ++i)
	{
		if (D2[i] < Dmin)
		{
			Dmin = D2[i];
			imin = i;
		}
	}
	Inf = Infs[imin];
	return Dmin;
}

void DXSubModel5X::MakeBox(const BBox& Box)
{
	using namespace concurrency;
	parallel_invoke(
		[&] {Directions[0].MakeBox(&GetMemory(0), Box); },
		[&] {Directions[1].MakeBox(&GetMemory(1), Box); },
		[&] {Directions[2].MakeBox(&GetMemory(2), Box); }
	);
}
