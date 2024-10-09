#pragma once

#include "DXModel.h"
#include "MElemIDProc.h"
#include "NCadrGeom.h"
#include "MQuadBody.h"
#include "MBodyPointInf.h"
#include "DXDirection.h"

class BSP_MODEL_API DXSubModel : public MQuadBody
{
public:
	virtual ~DXSubModel(void);
	DXSubModel(/*const*/ DXModel &ParentModel, BStockGrID Position);
	void ReCreate();
	double GetXstep(void) const { return Parent.sX;}
	double GetYstep(void) const { return Parent.sY;}
	double GetZstep(void) const { return Parent.sZ;}
	int GetNX(void) const { return nX; }
	int GetNY(void) const { return nY; }
	int GetNZ(void) const { return nZ; }
	int GetWNX(void) const { return (Parent.nBig[0]) * (Win.ixEnd - Win.ixStart) + 1; }
	int GetWNY(void) const { return (Parent.nBig[1]) * (Win.iyEnd - Win.iyStart) + 1; }
	int GetWNZ(void) const { return (Parent.nBig[2]) * (Win.izEnd - Win.izStart) + 1; }
	int GetWStartX(void) const { return (Parent.nBig[0]) * Win.ixStart; }
	int GetWStartY(void) const { return (Parent.nBig[1]) * Win.iyStart; }
	const BBox &GetGabar(void) const { return Parent.GGabar; }
	const DXModel &GetParent(void) const { return Parent;}
	void GetCellIndexes(double x, double y, double z, int & ix, int & iy, int &iz);
	void SetWindow(const BBox& Bounds) override; // List Method
	void SetMaxWin(void);
	void Save(MForestSave& SaveData) override;
	void Load(CFile* f) override;
	void CreateFullCopy(MBody* pQ) override;
protected:
	int nX; // Number of intervals along (X, Y, Z) axis
	int nY;
	int nZ;
	/*const*/ DXModel &Parent;
public:
//	MBodyParams GetParams();
	static int InsSpecCadr(DX_TOOLTYPE DXType,
		const BPoint& P0v, const BPoint& P1v, const BPoint& Nv,
		double R, double H, DX_ID PairID, const BMatr &Shift, bool FirstCadr, bool LastCadr, class CadrData& Data, const CadrData& MaskData);


	virtual int RastBCadr(int Dir, const CadrData& Data) = 0;
	virtual int RastFullBCadr(int Dir, const CadrData& Data) = 0;
	virtual int RastCCadr(int Dir, const CadrData& Data) = 0;
	virtual bool CheckCh(void) const = 0;
public:
	virtual void ClearChanged(void) = 0;
	virtual bool IsChanged(void) const = 0;
	int RastSpecCadr(int Dir, const CadrData& Data);// List method

protected:
	void ResetpModifiedCells(void) override;
};

