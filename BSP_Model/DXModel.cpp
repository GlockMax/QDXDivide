#include "stdafx.h"
#include "math.h"
#include "DXModel.h"


DXModel::DXModel(void)
{
	nX = 0;
	nY = 0;
	nZ = 0;
	RowsNum = 1;
	ColsNum = 1;
	nBig[0] = -1;
	nBig[1] = -1;
	nBig[2] = -1;
	sX = 0.;
	sY = 0.;
	sZ = 0.;
}


DXModel::~DXModel(void)
{
}

void DXModel::Set(const BBox &InGabar, double CellSize, int Rowsn, int Colsn, int Levelsn)
{
	Set(InGabar, max(1, int(InGabar.dX() / CellSize)), max(1, int(InGabar.dY() / CellSize)), max(1, int(InGabar.dZ() / CellSize)), Rowsn, Colsn, Levelsn);
}

void DXModel::Set(const BBox &InGabar, int MaxNumber, int Rowsn, int Colsn, int Levelsn)
{
	if(!InGabar.IsDefined())
		return;
	double S = InGabar.dX() * InGabar.dY() + InGabar.dX() * InGabar.dZ() + InGabar.dZ() * InGabar.dY();
	double CellSize = sqrt(2. * S / MaxNumber); 

	Set(InGabar, CellSize, Rowsn, Colsn, Levelsn);
}

void DXModel::Set(const BBox &InGabar, int XSteps, int YSteps, int ZSteps, int Rowsn, int Colsn, int Levelsn)
{
	GGabar = InGabar;
	if(!GGabar.IsDefined())
		return;
	nBig[0] = max(2, XSteps / Colsn + 1);// Big cell must have at least 2 elements for each coordinate
	nBig[1] = max(2, YSteps / Rowsn + 1);
	nBig[2] = max(2, ZSteps / Levelsn + 1);
	nX = nBig[0] * Colsn; // Ensure nX is multiple of Colsn
	nY = nBig[1] * Rowsn; // Ensure nY is multiple of Rowsn
	nZ = nBig[2] * Levelsn; // Ensure nZ is multiple of Levelsn
	ColsNum = Colsn;
	RowsNum = Rowsn;
	LevelsNum = Levelsn;
	sX = (GGabar.dX() + 1.e-5) / nX;
	sY = (GGabar.dY() + 1.e-5) / nY;
	sZ = (GGabar.dZ() + 1.e-5) / nZ;

}

bool DXModel::operator == (const DXModel &In) const
{
	return GGabar == In.GGabar &&
		nX == In.nX &&
		nY == In.nY &&
		nZ == In.nZ &&
		ColsNum == In.ColsNum &&
		RowsNum == In.RowsNum &&
		sX == In.sX &&
		sY == In.sY &&
		sZ == In.sZ;
}

bool DXModel::Save2File(CFile* f) const
{
	f->Write(&GGabar, sizeof(GGabar));
	f->Write(&sX, sizeof(sX));
	f->Write(&sY, sizeof(sY));
	f->Write(&sZ, sizeof(sZ));
	f->Write(&nX, sizeof(nX));
	f->Write(&nY, sizeof(nY));
	f->Write(&nZ, sizeof(nZ));
	f->Write(&ColsNum, sizeof(ColsNum));
	f->Write(&RowsNum, sizeof(RowsNum));
	f->Write(&LevelsNum, sizeof(LevelsNum));
	f->Write(&nBig, sizeof(nBig));
	return true;
}

void DXModel::Load(CFile* f)
{
	f->Read(&GGabar, sizeof(GGabar));
	f->Read(&sX, sizeof(sX));
	f->Read(&sY, sizeof(sY));
	f->Read(&sZ, sizeof(sZ));
	f->Read(&nX, sizeof(nX));
	f->Read(&nY, sizeof(nY));
	f->Read(&nZ, sizeof(nZ));
	f->Read(&ColsNum, sizeof(ColsNum));
	f->Read(&RowsNum, sizeof(RowsNum));
	f->Read(&LevelsNum, sizeof(LevelsNum));
	f->Read(&nBig, sizeof(nBig));
}
