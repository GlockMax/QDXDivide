#pragma once
#include "MElemIDProc.h"
#include "BPoint.h"

class DXVertex //
{
public:
	static const class DXSubModel3X *pDXSubModel;

	DXVertex(void);
	~DXVertex(void);
	DXVertex(int I, int J, int K);
	void SetID(int I, int J, int K);
	static void SetModel(const class DXSubModel3X &CurModel);
	double GetX() const;
	double GetY() const;
	double GetZ() const;
	const BPoint GetPointBot() const;
	const BPoint &GetPoint() const;
	const BPoint &GetNormal() const;
	int GetSurfaceID() const;
	const class SOrigSurf *GetSurface() const;
	PackedColor GetPCol(void) const { return PCol;}
	void SetPCol(PackedColor InCol) { PCol = InCol;}
	bool GenRenderIndex(int *pCurRenderIndex);
	int GetRenderIndex(void) const { return RenderIndex;}
	bool IsEmpty(void) const { return !Point.IsPoint();}
	void SetEmpty(void);
	static const class DXSubModel3X &GetModel(void) { return *pDXSubModel;}
protected:
	void CalcData();
	int i, j, k;
	PackedColor PCol;
	BPoint Normal;
	BPoint Point;
	int RenderIndex;
	class SOrigSurf *pSurf;
};





		


