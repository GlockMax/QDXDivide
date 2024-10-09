#include "stdafx.h"
#include "ConstDef.h"
#include "BBox.h"
#include "MBSPForest.h"
#include "MElemIDProc.h"
#include "SOPlane.h"
#include "BDynBitArray.h"
#include "MCubeMask.h"
#include "DXMemory.h"
#include "DXSubModel.h"
#include "DXDirection.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

DXDirection::DXDirection(void)
{
	n0 = 0;
	n1 = 0;
	Dir = E;
	RemoveWindow();
	SetMin(0., 0., 0.);
	UpLimit = DX_DEPTH_MAX;
	pModifiedCellsD = nullptr;
	pSubModel = nullptr;
}


DXDirection::~DXDirection(void)
{
	delete pModifiedCellsD;
}

bool DXDirection::Init(const DXSubModel *ipSubModel, DX_Dir NewDir, int s0, int s1, double St0, double St1, int s0Big, int s1Big, double v0, double v1, double vv)
{
	n0 = s0;
	n1 = s1;
	n0Big = s0Big;
	n1Big = s1Big;
	Step0 = St0;
	Step1 = St1;
	Dir = NewDir;
	RemoveWindow();
	SetMin(v0, v1, vv);
	pSubModel = ipSubModel;
	return true;
}

bool DXDirection::IsDefined() const
{
	return true;
}

void DXDirection::GetCoordIndexes(int *pI0, int *pI1, int *pI2) const
{
	// Returns indexes in ordinary (XYZ) vector of this Direction coordinates
	// pI0 - index (012) of the i0 coordinate
	// pI1 - index (012) of the i1 coordinate
	// pI2 - index (012) of the depth coordinate
	// for ex. for Y direction pI0 = 2, pI1 = 0, pI2 = 1
	switch(Dir)
	{
	case Z:
		*pI0 = 0;
		*pI1 = 1;
		*pI2 = 2;
		break;
	case X:
		*pI0 = 2;
		*pI1 = 1;
		*pI2 = 0;
		break;
	case Y:
		*pI0 = 2;
		*pI1 = 0;
		*pI2 = 1;
		break;
	default:
		break;
	}
}

void DXDirection::SetWindow(int xS, int xE, int yS, int yE)
{
	SubWin.Set(xS, xE, yS, yE);
}

void DXDirection::SetWindow(const BBox& Bounds)
{
	double VMin0 = 0., VMin1 = 0., VMax0 = 0., VMax1 = 0.;
	switch(Dir)
	{
	case Z:
		VMin0 = Bounds.Xmin;
		VMin1 = Bounds.Ymin;
		VMax0 = Bounds.Xmax;
		VMax1 = Bounds.Ymax;
		break;
	case Y:
		VMin0 = Bounds.Zmin;
		VMin1 = Bounds.Xmin;
		VMax0 = Bounds.Zmax;
		VMax1 = Bounds.Xmax;
		break;
	case X:
		VMin0 = Bounds.Ymin;
		VMin1 = Bounds.Zmin;
		VMax0 = Bounds.Ymax;
		VMax1 = Bounds.Zmax;
		break;
	}
	int xS, xE, yS, yE;
	GetCellIndexes(VMin0, VMin1, xS, yS);
	GetCellIndexes(VMax0, VMax1, xE, yE);
	SubWin.Set(xS, xE + 1, yS, yE + 1);
}

void DXDirection::ExpandWindow(const BBox& Bounds)
{
	double VMin0 = 0., VMin1 = 0., VMax0 = 0., VMax1 = 0.;
	switch(Dir)
	{
	case Z:
		VMin0 = Bounds.Xmin;
		VMin1 = Bounds.Ymin;
		VMax0 = Bounds.Xmax;
		VMax1 = Bounds.Ymax;
		break;
	case Y:
		VMin0 = Bounds.Zmin;
		VMin1 = Bounds.Xmin;
		VMax0 = Bounds.Zmax;
		VMax1 = Bounds.Xmax;
		break;
	case X:
		VMin0 = Bounds.Ymin;
		VMin1 = Bounds.Zmin;
		VMax0 = Bounds.Ymax;
		VMax1 = Bounds.Zmax;
		break;
	}
	int xS, xE, yS, yE;
	GetCellIndexes(VMin0, VMin1, xS, yS);
	GetCellIndexes(VMax0, VMax1, xE, yE);
	SubWin.Expand(xS, yS);
	if (xE >= n0)
		xE = n0 - 1;
	if (yE >= n1)
		yE = n1 - 1;
	SubWin.Expand(xE, yE);
}

void DXDirection::RemoveWindow()
{
	SubWin.Set(0, n0, 0, n1);
}

void DXDirection::GetCellIndexes(double x0, double x1, int & i0, int & i1) const
{// x0, x1 defined in the real world
	i0 = int((x0 - Min0) / Step0);
	i0 = min(n0 - 1, max(0, i0));
	i1 = int((x1 - Min1) / Step1);
	i1 = min(n1 - 1, max(0, i1));
}

void DXDirection::SetMin(double v0, double v1, double vv)
{
	Min0 = v0; Min1 = v1; MinVal = vv;
	switch(Dir)
	{
	case Y:
		Shift = BMatr::MatrZX2XY;
		break;
	case X:
		Shift = BMatr::MatrYZ2XY;
		break;
	case Z:
		Shift.SetE();
		break;
	}
	Shift = Shift * BMatr().Trans(-Min0, -Min1, -MinVal);
}

int DXDirection::SphYpGpInt(const double *n, double Y , double R2 , double  X[2], double  Z[2])
{
	// Определяются координаты X[2] и Z[2] точек пересечения сферы
	// радиусом R и плоскостей y = Y и P * N = 0
	// Возвращаемые точки упорядочены по X
	// Возвращаемое значение: first bit - X0 allowed, second bit - X1 allowed
	// abs(Y) не может быть > R

	double d2 = n[0] * n[0] + n[2] * n[2];
	if(d2 <= MIND * MIND)
		return -1;// N plane is parallel to Y plane
	double zd2 = 1. / d2;
	double z = - n[1] * Y * zd2;
	// Find point nearest to [0 0 0 1]
	double Xm = n[0] * z;
	double Zm = n[2] * z;
	// Find t
	double t = R2 - Xm * Xm - Zm * Zm;
	if(t <= 0.)
		return -1;
	t = sqrt(t * zd2);
	// Find intersection points
	if( n[2] < 0. )
		t = -t;
	X[0] = - n[2] * t + Xm; 
	X[1] =	 n[2] * t + Xm;
	Z[0] =   n[0] * t + Zm; 
	Z[1] = - n[0] * t + Zm;

	return ((Z[0] <= 0.) ? 0x1 : 0x0) | ((Z[1] <= 0.) ? 0x2 : 0x0);
}

bool DXDirection::Save2File(CFile* f) const
{
	f->Write(&Dir, sizeof(Dir));
	f->Write(&n0, sizeof(n0));
	f->Write(&n1, sizeof(n1));
	f->Write(&n0Big, sizeof(n0Big));
	f->Write(&n1Big, sizeof(n1Big));
	f->Write(&Min0, sizeof(Min0));
	f->Write(&Min1, sizeof(Min1));
	f->Write(&MinVal, sizeof(MinVal));
	f->Write(&Step0, sizeof(Step0));
	f->Write(&Step1, sizeof(Step1));
	f->Write(&UpLimit, sizeof(UpLimit));
	f->Write(&Shift, sizeof(Shift));
	return true;
}

void DXDirection::Load(CFile* f)
{
	f->Read(&Dir, sizeof(Dir));
	f->Read(&n0, sizeof(n0));
	f->Read(&n1, sizeof(n1));
	f->Read(&n0Big, sizeof(n0Big));
	f->Read(&n1Big, sizeof(n1Big));
	f->Read(&Min0, sizeof(Min0));
	f->Read(&Min1, sizeof(Min1));
	f->Read(&MinVal, sizeof(MinVal));
	f->Read(&Step0, sizeof(Step0));
	f->Read(&Step1, sizeof(Step1));
	f->Read(&UpLimit, sizeof(UpLimit));
	f->Read(&Shift, sizeof(Shift));
}

void DXDirection::CreateFullCopy(DXDirection* pIn) const
{
}

void DXDirection::SetChanged( int iXCur, int iYCur)
{ 
	Changed = true;
	if(pModifiedCellsD != nullptr)
	{
		int ixModif = iXCur / n0Big;
		bool xBorder = ((iXCur % n0Big) == 0) && (iXCur != 0) && ixModif > 0;
 		int iyModif = iYCur / n1Big;
		bool yBorder = ((iYCur % n1Big) == 0) && (iYCur != 0) && iyModif > 0;
		pModifiedCellsD->Set3Bits(ixModif, iyModif, xBorder, yBorder);
	}
}

void DXDirection::ClearChanged(void)
{ 
	Changed = false; 
}

void DXDirection::SetNewValues(int i0, int i1, DXMemoryPtOnePart *pActMemory, DXMemID MemID, DX_DEPTH Z, DX_ID ID)
{
	SetChanged(i0, i1, Z);
	pActMemory->SetAt(MemID, Z, ID);
}

void DXDirection::RemoveOldValues(int i0, int i1, class DXMemoryPtOnePart *pActMemory, DXMemID MemID)
{
	SetChanged(i0, i1, pActMemory->GetAtZ(MemID));
	pActMemory->RemoveAt(MemID);
}

void DXDirection::SetChanged(int i0, int i1, DX_DEPTH iZ)
{
	Changed = true;
	if (pModifiedCellsD == nullptr || pSubModel == nullptr)
		return;
	
	const double ZRelTol = 1.e-2; // ZRelTol * ZStep mast be grater than DEX_DIST
	int ixModif = -1;
	int iyModif = -1;
	int izModif = -1;
	bool BorderValue = false;
	switch (Dir)
	{
	case Z:
		{
			ixModif = i0;
			iyModif = i1;

			double ZStep = pSubModel->GetZstep();
			double z2 = iZ / ZStep;
			double zShift = ZStep * ZRelTol;
			izModif = int(z2 + zShift);
			int i2Dn = int(z2 - zShift);
			BorderValue = (izModif != i2Dn);
			break;
		}
	case Y:
		{
			ixModif = i1;

			double ZStep = pSubModel->GetYstep();
			double z2 = iZ / ZStep;
			double zShift = ZStep * ZRelTol;
			iyModif = int(z2 + zShift);
			int i2Dn = int(z2 - zShift);
			BorderValue = (iyModif != i2Dn);

			izModif = i0;
			break;
		}
	case X:
		{
			double ZStep = pSubModel->GetXstep();
			double z2 = iZ / ZStep;
			double zShift = ZStep * ZRelTol;
			ixModif = int(z2 + zShift);
			int i2Dn = int(z2 - zShift);
			BorderValue = (ixModif != i2Dn);

			iyModif = i0;
			izModif = i1;
			break;
		}
	}
	if (ixModif < 0 || iyModif < 0 || izModif < 0)
		return;
	div_t dx, dy, dz;
	dx.quot = ixModif / pSubModel->GetParent().nBig[0];
	dy.quot = iyModif / pSubModel->GetParent().nBig[1];
	dz.quot = izModif / pSubModel->GetParent().nBig[2];
	dx.rem = ixModif - dx.quot * pSubModel->GetParent().nBig[0];
	dy.rem = iyModif - dy.quot * pSubModel->GetParent().nBig[1];
	dz.rem = izModif - dz.quot * pSubModel->GetParent().nBig[2];
	bool xMax = (dx.quot >= pSubModel->GetParent().ColsNum);
	bool yMax = (dy.quot >= pSubModel->GetParent().RowsNum);
	bool zMax = (dz.quot >= pSubModel->GetParent().LevelsNum);
	bool xBorder = (Dir == X ? BorderValue : dx.rem == 0) && dx.quot > 0;
	bool yBorder = (Dir == Y ? BorderValue : dy.rem == 0) && dy.quot > 0;
	bool zBorder = (Dir == Z ? BorderValue : dz.rem == 0) && dz.quot > 0;

	if (!(xMax || yMax || zMax))
		SetMBit(dx.quot, dy.quot, dz.quot);
	if (xBorder && !(yMax || zMax))
		SetMBit(dx.quot - 1, dy.quot, dz.quot);
	if (yBorder && !(xMax || zMax))
		SetMBit(dx.quot, dy.quot - 1, dz.quot);
	if (zBorder && !(yMax || xMax))
		SetMBit(dx.quot, dy.quot, dz.quot - 1);
	if (xBorder && yBorder && !zMax)
		SetMBit(dx.quot - 1, dy.quot - 1, dz.quot);
	if (xBorder && zBorder && !yMax)
		SetMBit(dx.quot - 1, dy.quot, dz.quot - 1);
	if (zBorder && yBorder && !xMax)
		SetMBit(dx.quot, dy.quot - 1, dz.quot - 1);
	if (xBorder && yBorder && zBorder)
		SetMBit(dx.quot - 1, dy.quot - 1, dz.quot - 1);
}

void DXDirection::SetMBit(int Col, int Row, int Level)
{
	static_cast<MCubeMask *>(pModifiedCellsD)->SetBit(Col, Row, Level);
}