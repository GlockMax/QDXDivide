#include "stdafx.h"
#include "BBox.h"
#include "MQuadMask.h"
#include "MForestSave.h"
#include "MQuadBody.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

MQuadBody::MQuadBody(BStockGrID Position) : MBody(Position)
{
	pModifiedCells = NULL;
	RowsNum = 0;
	ColsNum = 0;
	Win.Set(0, 0, 0, 0);
}

MQuadBody::MQuadBody(const BPoint & Minp, const BPoint & Maxp, int Rowsn, int Colsn, BStockGrID Position) : MBody(Position)
{
	pModifiedCells = NULL;
	ReCreate(Minp, Maxp, Rowsn, Colsn);
}

void MQuadBody::ReCreate( const BPoint & Minp, const BPoint & Maxp, int Rowsn, int Colsn)
{
	MBody::ReCreate(Minp, Maxp);
	RowsNum = Rowsn;
	ColsNum = Colsn;
	Xstep = (MaxPoint.GetX() - MinPoint.GetX()) / ColsNum;
	Ystep = (MaxPoint.GetY() - MinPoint.GetY()) / RowsNum;
	ResetpModifiedCells();
	RemoveWindow();
}

void MQuadBody::ResetpModifiedCells()
{
	delete pModifiedCells;
	pModifiedCells = new MQuadMask(ColsNum, RowsNum);
}

MQuadBody::~MQuadBody(void)
{
	delete pModifiedCells;
}

void MQuadBody::ClearModified(void)
{
	if (pNextMBody)
		((MQuadBody*)pNextMBody)->ClearModified();
	pModifiedCells->Clear();
}

const MBaseMask &MQuadBody::GetModifiedCells(void) const
{
	return *pModifiedCells;
}

MBaseMask &MQuadBody::GetModifiedCells(void)
{
	return *pModifiedCells;
}

bool MQuadBody::IsChanged() const
{
	static_cast<MQuadMask *>(pModifiedCells)->StartIterate();
	int tempX = 0, tempY = 0;
	return static_cast<MQuadMask *>(pModifiedCells)->NextPair(&tempX, &tempY);
}

void MQuadBody::SetWindow(const BBox& Bounds)
{
	if (pNextMBody)
		((MQuadBody*)pNextMBody)->SetWindow(Bounds);
	if(	Bounds.Zmin >= MaxPoint.GetZ() ||
		Bounds.Zmax <= MinPoint.GetZ() ||
		Bounds.Ymin >= MaxPoint.GetY() ||
		Bounds.Ymax <= MinPoint.GetY() ||
		Bounds.Xmin >= MaxPoint.GetX() ||
		Bounds.Xmax <= MinPoint.GetX() )
	{
		Win.Set(0, 0, 0, 0, 0, 0);
		return;
	}

	int xS,xE,yS,yE;
	GetCellIndexes(Bounds.Xmin, Bounds.Ymin, xS, yS);
	GetCellIndexes(Bounds.Xmax, Bounds.Ymax, xE, yE);
	Win.Set(xS, xE + 1, yS, yE + 1);
}

void MQuadBody::CreateFullCopy(MBody* pQt)
{
	MBody::CreateFullCopy(pQt);
	MQuadBody* pQ = dynamic_cast<MQuadBody*>(pQt);
	if (pQ == nullptr)
		return;
	pQ->RowsNum = RowsNum;
	pQ->ColsNum = ColsNum;
	pQ->Xstep = Xstep;
	pQ->Ystep = Ystep;

	pQ->Win = Win;

//	pQ->pModifiedCells = nullptr;
	pQ->ResetpModifiedCells();
}

void MQuadBody::Window::Set(int xS, int xE, int yS, int yE)
{
	Defined = true;
	ixStart = xS;
	ixEnd = xE;
	iyStart = yS;
	iyEnd = yE;
	izStart = 0;
	izEnd = 0;
}

void MQuadBody::Window::Set(int xS, int xE, int yS, int yE, int zS, int zE)
{
	Defined = true;
	ixStart = xS;
	ixEnd = xE;
	iyStart = yS;
	iyEnd = yE;
	izStart = zS;
	izEnd = zE;
}

void MQuadBody::Window::Expand(int x, int y)
{
	if(!Defined)
	{
		Set(x, x + 1, y, y + 1);
		return;
	}
	if( x < ixStart ) 
		ixStart = x;
	else if( x >= ixEnd)
		ixEnd = x + 1;
	if( y < iyStart ) 
		iyStart = y;
	else if( y >= iyEnd)
		iyEnd = y + 1;
}

void MQuadBody::Window::Expand(int x, int y, int z)
{
	if(!Defined)
	{
		Set(x, x, y, y, z, z);
		return;
	}
	if( x < ixStart ) 
		ixStart = x;
	else if( x > ixEnd)
		ixEnd = x;
	if (y < iyStart)
		iyStart = y;
	else if (y > iyEnd)
		iyEnd = y;
	if (z < izStart)
		izStart = z;
	else if (z > izEnd)
		izEnd = z;
}

void MQuadBody::Save(MForestSave &SaveData)
{
	MBody::Save(SaveData);
	CFile *f = SaveData.File;
	f->Write((void *)&ColsNum, sizeof(ColsNum));
	f->Write((void *)&RowsNum, sizeof(RowsNum));
	f->Write((void *)&Xstep, sizeof(Xstep));
	f->Write((void *)&Ystep, sizeof(Ystep));
}

void MQuadBody::Load(CFile * f)
{
	MBody::Load(f);
	f->Read((void *)&ColsNum, sizeof(ColsNum));
	f->Read((void *)&RowsNum, sizeof(RowsNum));
	f->Read((void *)&Xstep, sizeof(Xstep));
	f->Read((void *)&Ystep, sizeof(Ystep));
}

void MQuadBody::RemoveWindow()
{
	if (pNextMBody)
		((MQuadBody*)pNextMBody)->RemoveWindow();
	Win.Set(0, ColsNum, 0, RowsNum);
}

void MQuadBody::GetCellIndexes(double x, double y, int & ix, int & iy)
{
	ix = min(ColsNum - 1, max(0,int((x - MinPoint.GetX()) / Xstep)));
	iy = min(RowsNum - 1, max(0,int((y - MinPoint.GetY()) / Ystep)));
}

int MQuadBody::GetRowsNum(void) const
{
	return RowsNum;
}

int MQuadBody::GetColsNum(void) const
{
	return ColsNum;
}

MQuadBody::RayMarch2D::RayMarch2D(const BBox& Gabar, int ColsNum, int RowsNum, const BPoint& P, const BPoint& V)
{
	StV = V;

	CurInd[0] = -1;
	CurInd[1] = -1;
	CurInd[2] = -1;

	if (!Gabar.IsDefined())
		return;
	if (Gabar.Contains(P))
	{
		CurP = P;
	}
	else
	{
		CurP = Gabar.RayCasting(P, V, 1.e-6, FirstN);
	}

	if (!CurP.IsPoint())
		return;
	Start = CurP;

	MaxInd[0] = ColsNum - 1;
	MaxInd[1] = RowsNum - 1;
	MaxInd[2] = 0;

	Size[0] = Gabar.dX() / ColsNum;
	Size[1] = Gabar.dY() / RowsNum;
	Size[2] = Gabar.dZ();

	Start -= Gabar.GetMinPoint();
	for (int i = 0; i < 3; ++i)
	{
		CurInd[i] = int(Start[i] / Size[i]);
		CurInd[i] = min(CurInd[i], MaxInd[i]);
		Start[i] -= CurInd[i] * Size[i];
		Start[i] /= Size[i];
		Step[i] = V[i] * (1. / Size[i]);
	}
	Step.SetH(0.);
	Step *= 1. / sqrt(Step.D2());
}

bool MQuadBody::RayMarch2D::GetNextCell(int& i, int& j)
{
	if (CurInd[0] < 0)
		return false;

	i = CurInd[0];
	j = CurInd[1];

	double tMin = 2.;
	int iMin = -1;
	for (int l = 0; l < 3; ++l)
	{
		double Rest = Step[l] > 0. ? 1. - Start[l] : Start[l];
		double t = Rest / fmax(fabs(Step[l]), 0.5 * Rest);
		if (t < tMin)
		{
			tMin = t;
			iMin = l;
		}
	}
	if (iMin < 0)
		return false; // internal error

	CurV = StV * (Size[iMin] * tMin * Step[iMin] / StV[iMin]);

	CurP += CurV;

	Start += Step * tMin;
	Start[iMin] = Step[iMin] > 0. ? 0. : 1.;
	CurInd[iMin] += Step[iMin] > 0. ? 1 : -1;
	if (CurInd[iMin] < 0 || CurInd[iMin] > MaxInd[iMin])
		CurInd[0] = -1;
	return true;
}

const BPoint MQuadBody::RayMarch2D::GetP() const
{
	return CurP - CurV;
}

const BPoint& MQuadBody::RayMarch2D::GetV() const
{
	return CurV;
}

const BPoint& MQuadBody::RayMarch2D::GetFirstN() const
{
	return FirstN;
}
