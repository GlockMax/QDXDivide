#pragma once
#include "float.h"
#include "BMatr.h"
#include "DXMemory.h"
#include "MQuadBody.h"
#include "MElemIDProc.h"

class BSP_MODEL_API DXDirection
{
public:
	enum DX_Dir
	{
		E,
		X,
		Y,
		Z
	};
protected:
	DX_Dir Dir;
	int n0; // A number of POINTS in 0 and 1 directions
	int n1;
	int n0Big; // A number of INTERVALS in 0 and 1 directions in one big cell
	int n1Big;
	double Min0;
	double Min1;
	double MinVal;
	double Step0;
	double Step1;
	double UpLimit;
	BMatr Shift;
	bool Changed;	
	MQuadBody::Window SubWin;
	class MBaseMask *pModifiedCellsD;
	const class DXSubModel *pSubModel;
public:
	DXDirection(void);
	virtual ~DXDirection(void);
	//bool Init(DX_Dir, int s0, int s1, double St0, double St1, int s0Big, int s1Big);
	bool Init(const DXSubModel *pSubModel, DX_Dir, int s0, int s1, double St0, double St1, int s0Big, int s1Big, double v0, double v1, double vv);
	bool IsDefined() const;
	int GetN0(void) const { return n0;}
	int GetN1(void) const { return n1;}
	int GetN0Big(void) const { return n0Big;}
	int GetN1Big(void) const { return n1Big;}
	double GetStep0(void) const { return Step0;}
	double GetStep1(void) const { return Step1;}
	void GetCoordIndexes(int *pI0, int *pI1, int *pI2) const;
	bool IsChanged(void) const { return Changed;}
	void ClearChanged(void);
	void SetChanged(void) { Changed = true;}
	void SetChanged( int iXCur, int iYCur);
    void SetWindow(const BBox& Bounds);
    void ExpandWindow(const BBox& Bounds);
	void SetWindow(int xS, int xE, int yS, int yE);
	void RemoveWindow();
	void ClearWindow(void) { SubWin.Clear();}
	void GetCellIndexes(double x, double y, int & ix, int & iy) const;
	void SetMin(double v0, double v1, double vv);
	void SetUpLimit(double Limit) { UpLimit = Limit;}
	class MBaseMask * GetpModifiedCellsD(void) const { return pModifiedCellsD; }
	double GetMinVal(void) const { return MinVal;}
	DX_Dir GetDir() const { return Dir; }
public:
	static int SphYpGpInt(const double *n, double Y, double R2, double  X[2], double  Z[2]);
	static int intCeil(double X) { X += 0.999999; return (X >= 0.) ? int(X) : int(X - 1.);}
	virtual bool Save2File(CFile* f) const;
	virtual void Load(CFile* f);
	virtual void CreateFullCopy(DXDirection* pIn) const;
	void SetSubModel(DXSubModel* pSModel) { pSubModel = pSModel; }
protected:
	void SetNewValues(int i0, int i1, class DXMemoryPtOnePart *pActMemory, DXMemID MemID, DX_DEPTH Z, DX_ID ID);
	void RemoveOldValues(int i0, int i1, class DXMemoryPtOnePart *pActMemory, DXMemID MemID);
	void SetChanged(int i0, int i1, DX_DEPTH Z);
	void SetMBit(int Col, int Row, int Level);
};

