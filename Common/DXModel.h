#pragma once

#include "BSP_MODEL_API.h"
#include "BPoint.h"
#include "BBox.h"

class BSP_MODEL_API DXModel
{
	friend class DXSubModel;
	friend class DXSubModel5X;
	friend class DXDirection;
	friend class GQuadRenderDX5X;
	friend class DXMap;
	void Set(const BBox &InGabar, int XSteps, int YSteps, int ZSteps, int Rowsn, int Colsn, int Levelsn = -1);
public:
	DXModel(void);
	~DXModel(void);
	void Set(const BBox &InGabar, double CellSize, int Rowsn, int Colsn, int Levelsn = -1);
	void Set(const BBox &InGabar, int MaxNumber, int Rowsn, int Colsn, int Levelsn = -1);
	bool operator == (const DXModel &In) const;
	bool operator != (const DXModel &In) const { return !operator == (In);}
	int GetColsNum() const { return ColsNum; }
	int GetRowsNum() const { return RowsNum; }
	int GetLevelsNum() const { return LevelsNum; }
	bool Save2File(CFile* f) const;
	void Load(CFile* f);
protected:
	BBox GGabar;
	double sX; // Step sizes
	double sY;
	double sZ;
	int nX; // Number of intervals along (X, Y, Z) axis
	int nY;
	int nZ;
	int ColsNum; // Big cells numbers
	int RowsNum;
	int LevelsNum;
	int nBig[3];  // Big cells sizes (a number of intervals)
};

